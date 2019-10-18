/**
 ******************************************************************************
  * File Name          : App/app_zigbee.c
  * Description        : Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "zigbee_interface.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "stm32_seq.h"

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void APP_ZIGBEE_Demo(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);

/* Private variables -----------------------------------------------*/
static TL_CmdPacket_t *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t *p_ZIGBEE_notif_M0_to_M4;
static __IO uint32_t CptReceiveMsgFromM0 = 0;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifLoggingBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

/* Functions Definition ------------------------------------------------------*/

void APP_ZIGBEE_Init(void)
{
    SHCI_CmdStatus_t ZigbeeInitStatus;

    APP_DBG("APP_ZIGBEE_Init");

    /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
    APP_ZIGBEE_CheckWirelessFirmwareInfo();

    /* Register cmdbuffer */
    APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

    /* Init config buffer and call TL_ZIGBEE_Init */
    APP_ZIGBEE_TL_INIT();

    ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
    /* Prevent unused argument(s) compilation warning */
    UNUSED(ZigbeeInitStatus);

    /* Register task */
    /* Create the different tasks */
    UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_MSG_FROM_M0_TO_M4, UTIL_SEQ_RFU,APP_ZIGBEE_ProcessMsgM0ToM4);

    /* Create the zigbee demo task */
    UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP, UTIL_SEQ_RFU,APP_ZIGBEE_Demo);
    /* run the task */
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_Init */

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode)
{
    switch (ErrId) {
        default:
            APP_ZIGBEE_TraceError("ERROR Unknown ", 0);
            break;
    }
} /* APP_ZIGBEE_Error */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (Zigbee or other module if any)
  * @retval None
  */
static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode)
{
    APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
    while (1U == 1U) {
        BSP_LED_Toggle(LED1);
        HAL_Delay(500U);
        BSP_LED_Toggle(LED2);
        HAL_Delay(500U);
        BSP_LED_Toggle(LED3);
        HAL_Delay(500U);
    }
} /* APP_ZIGBEE_TraceError */

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Zigbee
 *        and display associated informations
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void)
{
    WirelessFwInfo_t wireless_info_instance;
    WirelessFwInfo_t *p_wireless_info = &wireless_info_instance;

    if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success) {
        APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
    }
    else {
        APP_DBG("**********************************************************");
        APP_DBG("WIRELESS COPROCESSOR FW:");
        /* Print version */
        APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

        switch (p_wireless_info->StackType) {
            case INFO_STACK_TYPE_ZIGBEE:
                APP_DBG("FW Type : Zigbee stack");
                break;
            default:
                /* No Zigbee device supported ! */
                APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
                break;
        }
        APP_DBG("**********************************************************");
    }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer)
{
    p_ZIGBEE_otcmdbuffer = p_buffer;
} /* APP_ZIGBEE_RegisterCmdBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdPayloadBuffer(void)
{
    return (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;
} /* ZIGBEE_Get_OTCmdPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdRspPayloadBuffer(void)
{
    return (Zigbee_Cmd_Request_t *)((TL_EvtPacket_t *)p_ZIGBEE_otcmdbuffer)->evtserial.evt.payload;
} /* ZIGBEE_Get_OTCmdRspPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_NotificationPayloadBuffer(void)
{
    return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_notif_M0_to_M4)->evtserial.evt.payload;
} /* ZIGBEE_Get_NotificationPayloadBuffer */

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransfer(void)
{
    Zigbee_Cmd_Request_t *cmd_req = (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;

    /* Zigbee OT command cmdcode range 0x280 .. 0x3DF = 352 */
    p_ZIGBEE_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
    /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
     * + ID (4 bytes) + Size (4 bytes) */
    p_ZIGBEE_otcmdbuffer->cmdserial.cmd.plen = 8U + (cmd_req->Size * 4U);

    TL_ZIGBEE_SendAppliCmdToM0();

    /* Wait completion of cmd */
    Wait_Getting_Ack_From_M0();
} /* ZIGBEE_CmdTransfer */

/**
 * @brief  This function is called when the M0+ acknoledge the fact that it has received a Cmd
 *
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_CmdEvtReceived(TL_EvtPacket_t *Otbuffer)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(Otbuffer);

    Receive_Ack_From_M0();
} /* TL_ZIGBEE_CmdEvtReceived */

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_NotReceived(TL_EvtPacket_t *Notbuffer)
{
    p_ZIGBEE_notif_M0_to_M4 = Notbuffer;

    Receive_Notification_From_M0();
} /* TL_ZIGBEE_NotReceived */

/**
  * @brief  This function is called before sending any ot command to the M0
  *         core. The purpose of this function is to be able to check if
  *         there are no notifications coming from the M0 core which are
  *         pending before sending a new ot command.
  * @param  None
  * @retval None
  */
void Pre_ZigbeeCmdProcessing(void)
{
    UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
} /* Pre_ZigbeeCmdProcessing */

/**
  * @brief  This function waits for getting an acknowledgment from the M0.
  *
  * @param  None
  * @retval None
  */
static void Wait_Getting_Ack_From_M0(void)
{
    UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
} /* Wait_Getting_Ack_From_M0 */

/**
  * @brief  Receive an acknowledgment from the M0+ core.
  *         Each command send by the M4 to the M0 are acknowledged.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Ack_From_M0(void)
{
    UTIL_SEQ_SetEvt(EVENT_ACK_FROM_M0_EVT);
} /* Receive_Ack_From_M0 */

/**
  * @brief  Receive a notification from the M0+ through the IPCC.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Notification_From_M0(void)
{
    CptReceiveMsgFromM0++;
    UTIL_SEQ_SetTask(1U << CFG_TASK_MSG_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
} /* Receive_Notification_From_M0 */

/**
 * @brief Perform initialization of TL for Zigbee.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_TL_INIT(void)
{
    ZigbeeConfigBuffer.p_ZigbeeOtCmdRspBuffer = (uint8_t *)&ZigbeeOtCmdBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotAckBuffer = (uint8_t *)ZigbeeNotifRspEvtBuffer;
    ZigbeeConfigBuffer.p_ZigbeeLoggingBuffer = (uint8_t *)ZigbeeNotifLoggingBuffer;
    TL_ZIGBEE_Init(&ZigbeeConfigBuffer);
} /* APP_ZIGBEE_TL_INIT */

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_ProcessMsgM0ToM4(void)
{
    if (CptReceiveMsgFromM0 != 0) {
        /* If CptReceiveMsgFromM0 is > 1. it means that we did not serve all the events from the radio */
        if (CptReceiveMsgFromM0 > 1U) {
            APP_ZIGBEE_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
        }
        else {
            Zigbee_CallBackProcessing();
        }
        /* Reset counter */
        CptReceiveMsgFromM0 = 0;
    }
} /* APP_ZIGBEE_ProcessMsgM0ToM4 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
