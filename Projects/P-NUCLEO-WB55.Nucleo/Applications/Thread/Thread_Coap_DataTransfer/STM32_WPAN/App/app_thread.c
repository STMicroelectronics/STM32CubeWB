/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/app_thread.c
 * Description        : Thread Application.
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_thread.h"
#include "stm32wbxx_core_interface_def.h"
#include "openthread_api_wb.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32_lpm.h"
#include "stm32_seq.h"
#if (CFG_USB_INTERFACE_ENABLE != 0)
#include "vcp.h"
#include "vcp_conf.h"
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
#include "uart.h"
#endif /* ENABLE_OPENTHREAD_CLI */

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "data_transfer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define C_SIZE_CMD_STRING       256U
#define C_PANID                 0x2226U
#define C_CHANNEL_NB            19U

/* USER CODE BEGIN PD */
#define C_RESSOURCE_DATA_TRANSFER   "dataTransfer"
#define C_RESSOURCE_Provisioning    "provisioning"
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void APP_THREAD_CheckWirelessFirmwareInfo(void);
static void APP_THREAD_DeviceConfig(void);
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext);
static void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode);
#if (CFG_FULL_LOW_POWER == 0)
static void Send_CLI_To_M0(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
static void Send_CLI_Ack_For_OT(void);
static void HostTxCb( void );
static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint32_t ProcessCmdString(uint8_t* buf , uint32_t len);
#else
#if (CFG_FULL_LOW_POWER == 0)
static void RxCpltCallback(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
extern void otAppCliInit(otInstance *aInstance);
#endif /* ENABLE_OPENTHREAD_CLI */

/* USER CODE BEGIN PFP */
static void APP_THREAD_CheckMsgValidity(void);
static void APP_THREAD_SendNextBuffer(void);

static void APP_THREAD_CoapDataReqHandler(void                * pContext,
                                          otMessage           * pMessage,
                                          const otMessageInfo * pMessageInfo);
static void APP_THREAD_SendDataResponse(otMessage * pMessage,
    const otMessageInfo * pMessageInfo);
static void APP_THREAD_DataRespHandler(
    void * pContext,
    otMessage * pMessage,
    const otMessageInfo * pMessageInfo,
    otError Result);

static void APP_THREAD_AskProvisioning(void);
static void APP_THREAD_ProvisioningReqHandler(
    void * pContext,
    otMessage * pMessage,
    const otMessageInfo * pMessageInfo);
static otError APP_THREAD_ProvisioningRespSend(otMessage * pMessage,
    const otMessageInfo * pMessageInfo);
static void APP_THREAD_ProvisioningReqSend(void);
static void APP_THREAD_ProvisioningRespHandler(
    void * pContext,
    otMessage * pMessage,
    const otMessageInfo * pMessageInfo,
    otError Result);

static void APP_THREAD_CoapSendRequest(otCoapResource* aCoapRessource,
    otCoapType aCoapType,
    otCoapCode aCoapCode,
    const char *aStringAddress,
    const otIp6Address* aPeerAddress,
    uint8_t* aPayload,
    uint16_t Size,
    otCoapResponseHandler aHandler,
    void *aContext);
/* USER CODE END PFP */

/* Private variables -----------------------------------------------*/
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint8_t TmpString[C_SIZE_CMD_STRING];
static uint8_t VcpRxBuffer[sizeof(TL_CmdSerial_t)];        /* Received Data over USB are stored in this buffer */
static uint8_t VcpTxBuffer[sizeof(TL_EvtPacket_t) + 254U]; /* Transmit buffer over USB */
#else
#if (CFG_FULL_LOW_POWER == 0)
static uint8_t aRxBuffer[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_FULL_LOW_POWER == 0)
static uint8_t CommandString[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
static __IO uint16_t indexReceiveChar = 0;
static __IO uint16_t CptReceiveCmdFromUser = 0;

static TL_CmdPacket_t *p_thread_otcmdbuffer;
static TL_EvtPacket_t *p_thread_notif_M0_to_M4;
static __IO uint32_t  CptReceiveMsgFromM0 = 0;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_TH_Config_t ThreadConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ThreadNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliNotBuffer;
extern uint8_t g_ot_notification_allowed;

/* USER CODE BEGIN PV */
static otCoapResource OT_RessourceDataTransfer = {C_RESSOURCE_DATA_TRANSFER, APP_THREAD_CoapDataReqHandler,"DataTransferCtx", NULL};
static otCoapResource OT_RessourceProvisionning = {C_RESSOURCE_Provisioning, APP_THREAD_ProvisioningReqHandler,"ProvisioningCtx", NULL};
static otMessageInfo OT_MessageInfo = {0};
static uint8_t OT_Command = 0;
static uint16_t OT_BufferIdRead = 0U;
static uint16_t OT_BufferIdSend = 0U;
static uint8_t OT_BufferSend[COAP_PAYLOAD_MAX_LENGTH] = {0};
static uint8_t OT_BufferReceived[COAP_PAYLOAD_MAX_LENGTH] = {0};
static otMessage   * pOT_Message = NULL;
static otMessage   * pOT_MessageResponse = NULL;
static otIp6Address   OT_PeerAddress = { .mFields.m8 = { 0 } };
/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/

void APP_THREAD_Init( void )
{
  /* USER CODE BEGIN APP_THREAD_INIT_1 */

  /* USER CODE END APP_THREAD_INIT_1 */

  SHCI_CmdStatus_t ThreadInitStatus;

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_THREAD_CheckWirelessFirmwareInfo();

#if (CFG_USB_INTERFACE_ENABLE != 0)
  VCP_Init(&VcpTxBuffer[0], &VcpRxBuffer[0]);
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

  /* Register cmdbuffer */
  APP_THREAD_RegisterCmdBuffer(&ThreadOtCmdBuffer);

  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_THREAD, UTIL_LPM_DISABLE);

  /* Init config buffer and call TL_THREAD_Init */
  APP_THREAD_TL_THREAD_INIT();

  /* Configure UART for sending CLI command from M4 */
  APP_THREAD_Init_UART_CLI();

  /* Send Thread start system cmd to M0 */
  ThreadInitStatus = SHCI_C2_THREAD_Init();

  /* Prevent unused argument(s) compilation warning */
  UNUSED(ThreadInitStatus);

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_MSG_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_THREAD_ProcessMsgM0ToM4);

  /* USER CODE BEGIN INIT TASKS */
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_THREAD_AskProvisioning);
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_THREAD_SendNextBuffer);
 
  /* USER CODE END INIT TASKS */

  /* Initialize and configure the Thread device*/
  APP_THREAD_DeviceConfig();

  /* USER CODE BEGIN APP_THREAD_INIT_2 */
  /* USER CODE END APP_THREAD_INIT_2 */
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_THREAD_Error(uint32_t ErrId, uint32_t ErrCode)
{
  /* USER CODE BEGIN APP_THREAD_Error_1 */

  /* USER CODE END APP_THREAD_Error_1 */
  switch(ErrId)
  {
  case ERR_REC_MULTI_MSG_FROM_M0 :
    APP_THREAD_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_M0 ", ErrCode);
    break;
  case ERR_THREAD_SET_STATE_CB :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_STATE_CB ",ErrCode);
    break;
  case ERR_THREAD_SET_CHANNEL :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_CHANNEL ",ErrCode);
    break;
  case ERR_THREAD_SET_PANID :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_PANID ",ErrCode);
    break;
  case ERR_THREAD_IPV6_ENABLE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_IPV6_ENABLE ",ErrCode);
    break;
  case ERR_THREAD_START :
    APP_THREAD_TraceError("ERROR: ERR_THREAD_START ", ErrCode);
    break;
  case ERR_THREAD_ERASE_PERSISTENT_INFO :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_ERASE_PERSISTENT_INFO ",ErrCode);
    break;
  case ERR_THREAD_SET_NETWORK_KEY :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_NETWORK_KEY ",ErrCode);
    break;
  case ERR_THREAD_CHECK_WIRELESS :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_CHECK_WIRELESS ",ErrCode);
    break;
  /* USER CODE BEGIN APP_THREAD_Error_2 */
  case ERR_THREAD_COAP_START :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_START ",ErrCode);
    break;
  case ERR_THREAD_COAP_ADD_RESSOURCE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_ADD_RESSOURCE ",ErrCode);
    break;
  case ERR_THREAD_MESSAGE_READ :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_MESSAGE_READ ",ErrCode);
    break;
  case ERR_THREAD_COAP_SEND_RESPONSE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_RESPONSE ",ErrCode);
    break;
  case ERR_THREAD_COAP_APPEND :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_APPEND ",ErrCode);
    break;
  case ERR_THREAD_COAP_SEND_REQUEST :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_REQUEST ",ErrCode);
    break;
  case ERR_THREAD_SETUP :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SETUP ", ErrCode);
    break;
  case ERR_THREAD_LINK_MODE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_LINK_MODE ", ErrCode);
    break;
  case ERR_ALLOC_MSG :
    APP_THREAD_TraceError("ERROR : ERR_ALLOC_MSG ", ErrCode);
    break;
  case ERR_FILE_RESP_HANDLER :
    APP_THREAD_TraceError("ERROR : ERR_FILE_RESP_HANDLER ", ErrCode);
    break;
  case ERR_MSG_COMPARE_FAILED :
    APP_THREAD_TraceError("ERROR : ERR_MSG_COMPARE_FAILED ", ErrCode);
    break;
  case ERR_NEW_MSG_ALLOC:
    APP_THREAD_TraceError("ERROR : ERR_NEW_MSG_ALLOC ", ErrCode);
    break;
  case ERR_PROVISIONING_RESP:
    APP_THREAD_TraceError("ERROR : ERR_PROVISIONING_RESP ", ErrCode);
    break;
  case ERR_THREAD_DATA_RESPONSE:
    APP_THREAD_TraceError("ERROR : ERR_THREAD_DATA_RESPONSE ", ErrCode);
    break;
  case ERR_APPEND:
    APP_THREAD_TraceError("ERROR : ERR_APPEND ", ErrCode);
    break;
  case ERR_HEADER_INIT:
    APP_THREAD_TraceError("ERROR : ERR_HEADER_INIT ", ErrCode);
    break;
  case ERR_TOKEN:
    APP_THREAD_TraceError("ERROR : ERR_TOKEN ", ErrCode);
    break;
  /* USER CODE END APP_THREAD_Error_2 */
  default :
    APP_THREAD_TraceError("ERROR Unknown ", 0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief Thread initialization.
 * @param  None
 * @retval None
 */
static void APP_THREAD_DeviceConfig(void)
{
  otError error;
  otNetworkKey networkKey = {{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}};

#ifdef ENABLE_OPENTHREAD_CLI
  static otInstance *PtOpenThreadInstance;
  otInstanceFinalize(NULL);
  PtOpenThreadInstance = otInstanceInitSingle();
  otAppCliInit(PtOpenThreadInstance);
#endif /* ENABLE_OPENTHREAD_CLI */

  error = otInstanceErasePersistentInfo(NULL);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_ERASE_PERSISTENT_INFO,error);
  }
  error = otSetStateChangedCallback(NULL, APP_THREAD_StateNotif, NULL);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_STATE_CB,error);
  }
  error = otLinkSetChannel(NULL, C_CHANNEL_NB);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_CHANNEL,error);
  }
  error = otLinkSetPanId(NULL, C_PANID);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_PANID,error);
  }
  error = otThreadSetNetworkKey(NULL, &networkKey);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_NETWORK_KEY,error);
  }
  error = otIp6SetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_IPV6_ENABLE,error);
  }
  error = otThreadSetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_START,error);
  }

  /* USER CODE BEGIN DEVICECONFIG */
  /* Start the COAP server */
  error = otCoapStart(NULL, OT_DEFAULT_COAP_PORT);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_COAP_START,error);
  }
  /* Add COAP resources */
  otCoapAddResource(NULL, &OT_RessourceDataTransfer);

  /* Add APP_THREAD_AskProvisioning resources */
  otCoapAddResource(NULL, &OT_RessourceProvisionning);
  
  /* USER CODE END DEVICECONFIG */
}

/**
 * @brief Thread notification when the state changes.
 * @param  aFlags  : Define the item that has been modified
 *         aContext: Context
 *
 * @retval None
 */
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pContext);

  /* USER CODE BEGIN APP_THREAD_STATENOTIF */
  static uint32_t provisioning = 0;
  /* USER CODE END APP_THREAD_STATENOTIF */

  if ((NotifFlags & (uint32_t)OT_CHANGED_THREAD_ROLE) == (uint32_t)OT_CHANGED_THREAD_ROLE)
  {
    switch (otThreadGetDeviceRole(NULL))
    {
    case OT_DEVICE_ROLE_DISABLED:
      /* USER CODE BEGIN OT_DEVICE_ROLE_DISABLED */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END OT_DEVICE_ROLE_DISABLED */
      break;
    case OT_DEVICE_ROLE_DETACHED:
      /* USER CODE BEGIN OT_DEVICE_ROLE_DETACHED */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END OT_DEVICE_ROLE_DETACHED */
      break;
    case OT_DEVICE_ROLE_CHILD:
      /* USER CODE BEGIN OT_DEVICE_ROLE_CHILD */
      BSP_LED_Off(LED2);
      BSP_LED_On(LED3);
      if (provisioning == 0)
      {
        HAL_Delay(3000U);
      }
      provisioning = 1U;
      /* USER CODE END OT_DEVICE_ROLE_CHILD */
      break;
    case OT_DEVICE_ROLE_ROUTER :
      /* USER CODE BEGIN OT_DEVICE_ROLE_ROUTER */
      BSP_LED_Off(LED2);
      BSP_LED_On(LED3);
      /* USER CODE END OT_DEVICE_ROLE_ROUTER */
      break;
    case OT_DEVICE_ROLE_LEADER :
      /* USER CODE BEGIN OT_DEVICE_ROLE_LEADER */
      BSP_LED_On(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END OT_DEVICE_ROLE_LEADER */
      break;
    default:
      /* USER CODE BEGIN DEFAULT */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END DEFAULT */
      break;
    }
  }
}

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode)
{
  /* USER CODE BEGIN TRACE_ERROR */
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  while(1U == 1U)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
  /* USER CODE END TRACE_ERROR */
}

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Thread
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_THREAD_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    APP_THREAD_Error((uint32_t)ERR_THREAD_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch(p_wireless_info->StackType)
    {
    case INFO_STACK_TYPE_THREAD_FTD :
      APP_DBG("FW Type : Thread FTD");
      break;
    case INFO_STACK_TYPE_THREAD_MTD :
      APP_DBG("FW Type : Thread MTD");
      break;
    case INFO_STACK_TYPE_BLE_THREAD_FTD_STATIC :
      APP_DBG("FW Type : Static Concurrent Mode BLE/Thread");
      break;
    default :
      /* No Thread device supported ! */
      APP_THREAD_Error((uint32_t)ERR_THREAD_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
    APP_DBG("**********************************************************");
  }
}
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
/**
 * @brief  This function is used to compare the message received versus
 *     the original message.
 * @param  None
 * @retval None
 */
static void APP_THREAD_CheckMsgValidity(void)
{
  uint16_t mMsgId=OT_BufferIdRead - 1U; /* Corrected MsgId (MsgId starts at 1, not at 0) */
  uint32_t mOffset = COAP_PAYLOAD_MAX_LENGTH*mMsgId; /* the starting offset */
  uint32_t i;
  bool valid =true;

  for(i = mOffset; i < mOffset+COAP_PAYLOAD_MAX_LENGTH; i++)
  {
    if(OT_BufferReceived[i-mOffset] != aDataBuffer[i])
    {
      valid =false;
    }
  }

  if(valid == false)
    APP_THREAD_Error(ERR_MSG_COMPARE_FAILED, 0);
}

/**
 * @brief  This function compute the next message to be send
 * @param  None
 * @retval None
 */
static void APP_THREAD_SendNextBuffer(void)
{
  uint16_t j;
  uint16_t mOffset;

  APP_DBG("APP_THREAD_SendNextBuffer ID : %d", OT_BufferIdSend);
  if (OT_BufferIdSend < 5U)
  {
    /* Prepare next buffers to be send */
    OT_BufferIdSend++;
    mOffset=(OT_BufferIdSend - 1U) * COAP_PAYLOAD_MAX_LENGTH;

    memset(OT_BufferSend, 0, COAP_PAYLOAD_MAX_LENGTH);
    for(j = mOffset; j < mOffset + COAP_PAYLOAD_MAX_LENGTH; j++)
    {
      OT_BufferSend[j - mOffset] = aDataBuffer[j];
    }

    /* Send the data in unicast mode */
    APP_THREAD_CoapSendRequest(&OT_RessourceDataTransfer,
        OT_COAP_TYPE_CONFIRMABLE,
        OT_COAP_CODE_PUT,
        NULL,
        &OT_PeerAddress,
        OT_BufferSend,
        sizeof(OT_BufferSend),
        &APP_THREAD_DataRespHandler,
        NULL);
  }
  else
  {
    /* Buffer transfer has been successfully  transferred */
    BSP_LED_On(LED1);
    APP_DBG("BUFFER HAS BEEN TRANSFERRED");
  }
}

/**
 * @brief Data request handler triggered at the reception of the COAP message
 * @param pContext : Context
 * @param pHeader header pointer
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @retval None
 */
static void APP_THREAD_CoapDataReqHandler(void                * pContext,
                                          otMessage           * pMessage,
                                          const otMessageInfo * pMessageInfo)
{
  static uint8_t l_id = 0;
  do
  {
    APP_DBG("--> APP_THREAD_CoapDataReqHandler : l_id = %d", l_id++);
    if (otCoapMessageGetType(pMessage) != OT_COAP_TYPE_CONFIRMABLE &&
        otCoapMessageGetType(pMessage) != OT_COAP_TYPE_NON_CONFIRMABLE)
    {
      APP_THREAD_Error(ERR_THREAD_COAP_ADD_RESSOURCE, 0);
      break;
    }

    if (otCoapMessageGetCode(pMessage) != OT_COAP_CODE_PUT)
    {
      APP_THREAD_Error(ERR_THREAD_COAP_ADD_RESSOURCE, 0);
      break;
    }

    if (otMessageRead(pMessage, otMessageGetOffset(pMessage), &OT_BufferReceived, sizeof(OT_BufferReceived)) != sizeof(OT_BufferReceived))
    {
      APP_THREAD_Error(ERR_THREAD_MESSAGE_READ, 0);
    }
    //OT_BufferIdRead = otCoapMessageGetMessageId(pMessage);
    //APP_DBG("OT_BufferIdRead = %d", OT_BufferIdRead);
    OT_BufferIdRead++;

    APP_THREAD_CheckMsgValidity();

    if (otCoapMessageGetType(pMessage) == OT_COAP_TYPE_CONFIRMABLE)
    {
      APP_THREAD_SendDataResponse(pMessage, pMessageInfo);
    }
  } while (false);
}

/**
 * @brief This function acknowledge the data reception by sending an ACK
 *    back to the sender.
 * @param  pRequestHeader coap header
 * @param  pMessageInfo message info pointer
 * @retval None
 */
static void APP_THREAD_SendDataResponse(otMessage    * pMessage,
    const otMessageInfo * pMessageInfo)
{
  otError  error = OT_ERROR_NONE;

  APP_DBG("APP_THREAD_SendDataResponse");
  
  pOT_MessageResponse = otCoapNewMessage(NULL, NULL);
  if (pOT_MessageResponse == NULL)
  {
    APP_THREAD_Error(ERR_NEW_MSG_ALLOC,error);
  }
  
  error = otCoapMessageInitResponse(pOT_MessageResponse,
      pMessage,
      OT_COAP_TYPE_ACKNOWLEDGMENT,
      OT_COAP_CODE_CHANGED);

  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_COAP_MESSAGE_INIT_RESPONSE,error);
  }

  error = otCoapSendResponse(NULL, pOT_MessageResponse, pMessageInfo);
  if (error != OT_ERROR_NONE && pOT_MessageResponse != NULL)
  {
    otMessageFree(pOT_MessageResponse);
    APP_THREAD_Error(ERR_THREAD_DATA_RESPONSE,error);
  }
}

/**
 * @brief This function is used to handle the APP_THREAD_AskProvisioning handler
 *
 * @param pContext : Context
 * @param pHeader header pointer
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @retval None
 */
static void APP_THREAD_ProvisioningReqHandler(void                * pContext,
                                              otMessage           * pMessage,
                                              const otMessageInfo * pMessageInfo)
{
  APP_DBG("STEP 2: Receives Provisioning request");
  if (otCoapMessageGetType(pMessage) == OT_COAP_TYPE_CONFIRMABLE)
  {
    if (APP_THREAD_ProvisioningRespSend(pMessage, pMessageInfo) != OT_ERROR_NONE)
    {
      APP_THREAD_Error(ERR_PROVISIONING_RESP, 0);
    }
  }
}

/**
 * @brief This function is used to handle the APP_THREAD_AskProvisioning response
 *
 * @param pRequestHeader
 * @param pMessageInfo message info pointer
 * @retval error code
 */
static otError APP_THREAD_ProvisioningRespSend(otMessage    * pMessage,
    const otMessageInfo * pMessageInfo)
{
  otError  error = OT_ERROR_NONE;
  do{
    APP_DBG("STEP 3: Provisioning Send Response");

    pOT_MessageResponse = otCoapNewMessage(NULL, NULL);
    if (pOT_MessageResponse == NULL)
    {
      APP_DBG("WARNING : pOT_MessageResponse = NULL ! -> exit now");
      break;
    }

    otCoapMessageInitResponse(pOT_MessageResponse,
        pMessage,
        OT_COAP_TYPE_ACKNOWLEDGMENT,
        OT_COAP_CODE_CHANGED);

    otCoapMessageSetPayloadMarker(pOT_MessageResponse);

    error = otMessageAppend(pOT_MessageResponse, &OT_Command, sizeof(OT_Command));
    if (error != OT_ERROR_NONE)
    {
      APP_THREAD_Error(ERR_APPEND, error);
    }

    error = otMessageAppend(pOT_MessageResponse, otThreadGetMeshLocalEid(NULL), sizeof(otIp6Address));
    if (error != OT_ERROR_NONE)
    {
      break;
    }

    error = otCoapSendResponse(NULL, pOT_MessageResponse, pMessageInfo);
    if (error != OT_ERROR_NONE && pOT_MessageResponse != NULL)
    {
      otMessageFree(pOT_MessageResponse);
      APP_THREAD_Error(ERR_THREAD_COAP_SEND_RESP,error);
    }
  }while(false);


  return error;
}

/**
 * @brief This function is used to manage the APP_THREAD_AskProvisioning request
 *
 * @param None
 * @retval None
 */
static void APP_THREAD_ProvisioningReqSend()
{
  static uint8_t PayloadWrite = 1;
  APP_DBG("STEP 1: APP_THREAD_ProvisioningReqSend : Send a CoAP CONFIRMABLE GET Request");
  /* Send a CONFIRMABLE GET Request */
  APP_THREAD_CoapSendRequest(&OT_RessourceProvisionning,
      OT_COAP_TYPE_CONFIRMABLE,
      OT_COAP_CODE_GET,
      MULICAST_FTD_MED,
      NULL,
      &PayloadWrite,
      sizeof(PayloadWrite),
      &APP_THREAD_ProvisioningRespHandler,
      NULL);
}

/**
 * @brief This function is used to manage the APP_THREAD_AskProvisioning response
 *    handler.
 *
 * @param pContext context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code if any
 * @retval None
 */
static void APP_THREAD_ProvisioningRespHandler(
    void                * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  static uint8_t resp_index = 0;

  if (Result == OT_ERROR_NONE)
  {
    APP_DBG("STEP 4: Response received");
    uint16_t l_number_bytes_read = otMessageRead(pMessage, otMessageGetOffset(pMessage), &OT_Command, sizeof(OT_Command));
    APP_DBG("l_number_bytes_read = %d", l_number_bytes_read);

    if (l_number_bytes_read == sizeof(OT_Command))
    {
      /* Retrieve the peer address */
      if (otMessageRead(pMessage,
          otMessageGetOffset(pMessage) + sizeof(OT_Command),
          &OT_PeerAddress,
          sizeof(OT_PeerAddress)) != sizeof(OT_PeerAddress))
      {
        APP_THREAD_Error(ERR_READ, 0);
      }
      APP_DBG("resp_index = %d", resp_index);
      resp_index++;
      APP_DBG("--> READY to start Data Transfer");
      /* Ask to start the first transfer */
 //     UTIL_SEQ_SetTask(TASK_SEND_BUFFER, CFG_SCH_PRIO_1);
    }
    else
    {
      APP_DBG("**** WARNING: APP_THREAD_ProvisioningRespHandler read 0 byte ! *****");
    }
  }
  else
  {
    APP_DBG("**** APP_THREAD_ProvisioningRespHandler failed ***** with ERROR code = %d ", Result);
  }
}


/**
 * @brief This function initiates the APP_THREAD_AskProvisioning phase
 *
 * @param None
 * @retval None
 */
static void APP_THREAD_AskProvisioning(void)
{
  HAL_Delay(1000U);
  APP_THREAD_ProvisioningReqSend();
}

/**
 * @brief This function manages the data response handler
 *    and reschedules the sending of data.
 *
 * @param pContext context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code
 * @retval None
 */
static void APP_THREAD_DataRespHandler(
    void                * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pContext);
  UNUSED(pMessage);
  UNUSED(pMessageInfo);

  APP_DBG("APP_THREAD_DataRespHandler: Received response from Data Transfer with Result %d", Result);
  if (Result == OT_ERROR_NONE)
  {
    /* Ask to perform a new transfer */
    HAL_Delay(1000U);
    UTIL_SEQ_SetTask(1<<(uint32_t)CFG_TASK_BUTTON_SW2,CFG_SCH_PRIO_1);

  }
  else
  {
    APP_THREAD_Error(ERR_FILE_RESP_HANDLER,Result);
  }
}

/**
 * @brief Send a CoAP request with defined parameters.
 *
 * @param[in]  aCoapRessource   A pointer to a otCoapResource.
 * @param[in]  aCoapType        otCoapType.
 * @param[in]  aCoapCode        otCoapCode.
 * @param[in]  aStringAddress   A pointer to a NULL-terminated string representing the address. Example: "FF03::1" for Multicast.
 * @param[in]  aPeerAddress     A pointer to otIp6Address Peer Address.
 * @param[in]  aPayload         A pointer to payload.
 * @param[in]  aHandler         A pointer to CoAP response handler.
 * @param[in]  aContext         A pointer to application specific context.
 *
 * @retval none.
 */
static void APP_THREAD_CoapSendRequest(otCoapResource* aCoapRessource,
    otCoapType aCoapType,
    otCoapCode aCoapCode,
    const char *aStringAddress,
    const otIp6Address* aPeerAddress,
    uint8_t* aPayload,
    uint16_t Size,
    otCoapResponseHandler aHandler,
    void* aContext)
{
  otError error = OT_ERROR_NONE;

  do{
    pOT_Message = otCoapNewMessage(NULL, NULL);
    if (pOT_Message == NULL)
    {
      APP_THREAD_Error(ERR_THREAD_COAP_NEW_MSG,error);
      break;
    }

    otCoapMessageInit(pOT_Message, aCoapType, aCoapCode);
    otCoapMessageAppendUriPathOptions(pOT_Message, aCoapRessource->mUriPath);
    otCoapMessageSetPayloadMarker(pOT_Message);

    if((aPayload != NULL) && (Size > 0))
    {
      error = otMessageAppend(pOT_Message, aPayload, Size);
      if (error != OT_ERROR_NONE)
      {
        APP_THREAD_Error(ERR_THREAD_COAP_APPEND,error);
        break;
      }
    }
    else
    {
      APP_DBG("APP_THREAD_CoapSendRequest: No payload passed");
    }

    memset(&OT_MessageInfo, 0, sizeof(OT_MessageInfo));
    OT_MessageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

    if((aPeerAddress == NULL) && (aStringAddress != NULL))
    {
      APP_DBG("Use String Address : %s ", aStringAddress);
      otIp6AddressFromString(aStringAddress, &OT_MessageInfo.mPeerAddr);
    }
    else
    if (aPeerAddress != NULL)
    {
      APP_DBG("Use Peer Address");
      memcpy(&OT_MessageInfo.mPeerAddr, aPeerAddress, sizeof(OT_MessageInfo.mPeerAddr));
    }
    else
    {
      APP_DBG("ERROR: Address string and Peer Address not defined");
      APP_THREAD_Error(ERR_THREAD_COAP_ADDRESS_NOT_DEFINED, 0);
    }

    if(aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE)
    {
      APP_DBG("aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE");
      error = otCoapSendRequest(NULL,
          pOT_Message,
          &OT_MessageInfo,
          NULL,
          NULL);
    }
    if(aCoapType == OT_COAP_TYPE_CONFIRMABLE)
    {
      APP_DBG("aCoapType == OT_COAP_TYPE_CONFIRMABLE");
      error = otCoapSendRequest(NULL,
          pOT_Message,
          &OT_MessageInfo,
          aHandler,
          aContext);
    }
  }while(false);
  if (error != OT_ERROR_NONE && pOT_Message != NULL)
  {
    otMessageFree(pOT_Message);
    APP_THREAD_Error(ERR_THREAD_COAP_SEND_REQUEST,error);
  }
}
/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_THREAD_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer)
{
  p_thread_otcmdbuffer = p_buffer;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)p_thread_otcmdbuffer->cmdserial.cmd.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdRspPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)((TL_EvtPacket_t *)p_thread_otcmdbuffer)->evtserial.evt.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_NotificationPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)(p_thread_notif_M0_to_M4)->evtserial.evt.payload;
}

static void Ot_Cmd_Transfer_Common(void)
{

  /* OpenThread OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_thread_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  uint32_t l_size = ((Thread_OT_Cmd_Request_t*)(p_thread_otcmdbuffer->cmdserial.cmd.payload))->Size * 4U + 8U;
  p_thread_otcmdbuffer->cmdserial.cmd.plen = l_size;

  TL_OT_SendCmd();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();

}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_Transfer(void)
{
  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0 with Notification M0 to M4 allowed.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_TransferWithNotif(void)
{
  /* Flag to specify to UTIL_SEQ_EvtIdle that M0 to M4 notifications are allowed */
  g_ot_notification_allowed = 1U;

  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is called when acknowledge from OT command is received from the M0+.
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_OT_CmdEvtReceived( TL_EvtPacket_t * Otbuffer )
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();

  /* Does not allow OpenThread M0 to M4 notification */
  g_ot_notification_allowed = 0U;
}

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_NotReceived( TL_EvtPacket_t * Notbuffer )
{
  p_thread_notif_M0_to_M4 = Notbuffer;

  Receive_Notification_From_M0();
}

/**
  * @brief  This function is called before sending any ot command to the M0
  *         core. The purpose of this function is to be able to check if
  *         there are no notifications coming from the M0 core which are
  *         pending before sending a new ot command.
  * @param  None
  * @retval None
  */
void Pre_OtCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
}

/**
  * @brief  This function waits for getting an acknowledgment from the M0.
  *
  * @param  None
  * @retval None
  */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
}

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
}

/**
  * @brief  Receive a notification from the M0+ through the IPCC.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Notification_From_M0(void)
{
  CptReceiveMsgFromM0++;
  UTIL_SEQ_SetTask(TASK_MSG_FROM_M0_TO_M4,CFG_SCH_PRIO_0);
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
#else
#if (CFG_FULL_LOW_POWER == 0)
static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    CommandString[indexReceiveChar++] = aRxBuffer[0];
    if (aRxBuffer[0] == '\r')
    {
      CptReceiveCmdFromUser = 1U;

      /* UART task scheduling*/
      UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1U, RxCpltCallback);
}
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
 * @brief Process the command strings.
 *        As soon as a complete command string has been received, the task
 *        in charge of sending the command to the M0 is scheduled
 * @param  None
 * @retval None
 */
static uint32_t  ProcessCmdString( uint8_t* buf , uint32_t len )
{
  uint32_t i,j,tmp_start;
  tmp_start = 0;
  uint32_t res = 0;

  i= 0;
  while ((buf[i] != '\r') && (i < len))
  {
    i++;
  }

  if (i != len)
  {
    memcpy(CommandString, buf,(i+1));
    indexReceiveChar = i + 1U; /* Length of the buffer containing the command string */
    UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
    tmp_start = i;
    for (j = 0; j < (len - tmp_start - 1U) ; j++)
    {
      buf[j] = buf[tmp_start + j + 1U];
    }
    res = len - tmp_start - 1U;
  }
  else
  {
    res = len;
  }
  return res; /* Remaining characters in the temporary buffer */
}
#endif/* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_FULL_LOW_POWER == 0)
/**
 * @brief Process sends receive CLI command to M0.
 * @param  None
 * @retval None
 */
static void Send_CLI_To_M0(void)
{ 
#ifdef ENABLE_OPENTHREAD_CLI
  /* Don't use the ThreadCliCmdBuffer when buffer is too large as data may be overwritten.
     Use locals variables instead */
  uint16_t l_plen = 0;
  uint8_t l_ThreadCliCmdBuffer[255] = {0};
  
  memcpy(l_ThreadCliCmdBuffer, CommandString, indexReceiveChar);
  l_plen = indexReceiveChar;
  
    /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);
  
  APP_DBG("[Send_CLI_To_M0] payload : %s", l_ThreadCliCmdBuffer);

  otPlatUartReceived(l_ThreadCliCmdBuffer, l_plen);  
#else
  memset(ThreadCliCmdBuffer.cmdserial.cmd.payload, 0x0U, 255U);
  memcpy(ThreadCliCmdBuffer.cmdserial.cmd.payload, CommandString, indexReceiveChar);
  ThreadCliCmdBuffer.cmdserial.cmd.plen = indexReceiveChar;
  ThreadCliCmdBuffer.cmdserial.cmd.cmdcode = 0x0;

  /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);

  APP_DBG("[Send_CLI_To_M0] payload : %s", ThreadCliCmdBuffer.cmdserial.cmd.payload);

  TL_CLI_SendCmd();
#endif /* ENABLE_OPENTHREAD_CLI */
}
#endif /* (CFG_FULL_LOW_POWER == 0) */

#ifdef ENABLE_OPENTHREAD_CLI
/**
 * @brief  Wrapper function to flush UART data (called from the OpenThread stack)
           Not used but definition needed.
 * @param  None
 * @retval OT_ERROR_NONE
 */
otError otPlatUartFlush(void)
{
  return OT_ERROR_NONE;
}

/**
 * @brief  Wrapper function to send data through the UART from the OpenThread stack
 * @param  aBuf: Buffer of data to transmit
 * @param  aBufLength: Number of data to transmit (in bytes)
 * @retval OT_ERROR_NONE
 */
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
   /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)aBuf, "> ") != 0)
  {
    /* Write to CLI UART */
    HW_UART_Transmit(CFG_CLI_UART, (uint8_t*)aBuf, aBufLength, 100);
  }

  otPlatUartSendDone();
  
  return OT_ERROR_NONE;
}
#endif /* ENABLE_OPENTHREAD_CLI */

/**
 * @brief Send notification for CLI TL Channel.
 * @param  None
 * @retval None
 */
static void Send_CLI_Ack_For_OT(void)
{

  /* Notify M0 that characters have been sent to UART */
  TL_THREAD_CliSendAck();
}

/**
 * @brief Perform initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_THREAD_Init_UART_CLI(void)
{
#if (CFG_FULL_LOW_POWER == 0)
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SEND_CLI_TO_M0, UTIL_SEQ_RFU,Send_CLI_To_M0);
#endif /* (CFG_FULL_LOW_POWER == 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
#else
#if (CFG_FULL_LOW_POWER == 0)
  MX_LPUART1_UART_Init();
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1, RxCpltCallback);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
}

/**
 * @brief Perform initialization of TL for THREAD.
 * @param  None
 * @retval None
 */
void APP_THREAD_TL_THREAD_INIT(void)
{
  ThreadConfigBuffer.p_ThreadOtCmdRspBuffer = (uint8_t*)&ThreadOtCmdBuffer;
  ThreadConfigBuffer.p_ThreadNotAckBuffer = (uint8_t*)ThreadNotifRspEvtBuffer;
  ThreadConfigBuffer.p_ThreadCliRspBuffer = (uint8_t*)&ThreadCliCmdBuffer;
  ThreadConfigBuffer.p_ThreadCliNotBuffer = (uint8_t*)&ThreadCliNotBuffer;

  TL_THREAD_Init( &ThreadConfigBuffer );
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_CliNotReceived( TL_EvtPacket_t * Notbuffer )
{
  TL_CmdPacket_t* l_CliBuffer = (TL_CmdPacket_t*)Notbuffer;
  uint8_t l_size = l_CliBuffer->cmdserial.cmd.plen;

  /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)l_CliBuffer->cmdserial.cmd.payload, "> ") != 0)
  {
    /* Write to CLI UART */
#if (CFG_USB_INTERFACE_ENABLE != 0)
    VCP_SendData( l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#else
    HW_UART_Transmit_IT(CFG_CLI_UART, l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#endif /*USAGE_OF_VCP */
  }
  else
  {
    Send_CLI_Ack_For_OT();
  }
}

/**
 * @brief  End of transfer callback for CLI UART sending.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void HostTxCb(void)
{
  Send_CLI_Ack_For_OT();
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
void APP_THREAD_ProcessMsgM0ToM4(void)
{
  if (CptReceiveMsgFromM0 != 0)
  {
    /* If CptReceiveMsgFromM0 is > 1. it means that we did not serve all the events from the radio */
    if (CptReceiveMsgFromM0 > 1U)
    {
      APP_THREAD_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
    }
    else
    {
      OpenThread_CallBack_Processing();
    }
    /* Reset counter */
    CptReceiveMsgFromM0 = 0;
  }
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
 * @brief  This function is called when thereare some data coming
 *         from the Hyperterminal via the USB port
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data received
 * @param  Len: Number of data received (in bytes)
 * @retval Number of characters remaining in the buffer and not yet processed
 */
void VCP_DataReceived(uint8_t* Buf , uint32_t *Len)
{
  uint32_t i,flag_continue_checking = TRUE;
  uint32_t char_remaining = 0;
  static uint32_t len_total = 0;

  /* Copy the characters in the temporary buffer */
  for (i = 0; i < *Len; i++)
  {
    TmpString[len_total++] = Buf[i];
  }

  /* Process the buffer commands one by one     */
  /* A command is limited by a \r caracaters    */
  while (flag_continue_checking == TRUE)
  {
    char_remaining = ProcessCmdString(TmpString,len_total);
    /* If char_remaining is equal to len_total, it means that the command string is not yet
     * completed.
     * If char_remaining is equal to 0, it means that the command string has
     * been entirely processed.
     */
    if ((char_remaining == 0) || (char_remaining == len_total))
    {
      flag_continue_checking = FALSE;
    }
    len_total = char_remaining;
  }
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
