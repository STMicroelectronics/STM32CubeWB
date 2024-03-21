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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/**
  * @brief  APP_THREAD Status structures definition
  */
typedef enum
{
  APP_THREAD_OK       = 0x00,
  APP_THREAD_ERROR    = 0x01,
} APP_THREAD_StatusTypeDef;
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define C_SIZE_CMD_STRING       256U
#define C_PANID                 0x1111U
#define C_CHANNEL_NB            15U

/* USER CODE BEGIN PD */
#define C_RESSOURCE_FUOTA_PROVISIONING     "FUOTA_PROVISIONING"
#define C_RESSOURCE_FUOTA_PARAMETERS       "FUOTA_PARAMETERS"
#define C_RESSOURCE_FUOTA_SEND             "FUOTA_SEND"

/* Following CoAP resource is requested to reboot on Thread_Ota application */
#define C_RESSOURCE_FUOTA_REBOOT           "FUOTA_REBOOT"

/* Keyword found at the end of Thread Ota compatible application binaries */
#define FUOTA_MAGIC_KEYWORD_APP               0x94448A29

/* Keyword found at the end of encrypted Copro Wireless binaries */
#define FUOTA_MAGIC_KEYWORD_COPRO_WIRELESS    0xD3A12C5E

/* Define Address for Application FW Update */
#define FUOTA_APP_FW_BINARY_ADDRESS       (FLASH_BASE + CFG_APP_START_SECTOR_INDEX*0x1000)

/* Define Address for Copro Wireless FW Update */
#define FUOTA_COPRO_FW_BINARY_ADDRESS     (FLASH_BASE + CFG_APP_START_SECTOR_INDEX*0x1000)

#define FUOTA_NUMBER_WORDS_64BITS                50
#define FUOTA_PAYLOAD_SIZE                (FUOTA_NUMBER_WORDS_64BITS * 8)

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
static void APP_THREAD_CoapSendRequest(otCoapResource* aCoapRessource,
    otCoapType aCoapType,
    otCoapCode aCoapCode,
    const char *aStringAddress,
    const otIp6Address* aPeerAddress,
    uint8_t* aPayload,
    uint16_t Size,
    otCoapResponseHandler aHandler,
    void *aContext);

static void APP_THREAD_ProvisioningRespHandler(
    void* pContext,
    otMessage * pMessage,
    const otMessageInfo * pMessageInfo,
    otError Result);
static void APP_THREAD_CoapRespHandlerFuotaSend(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result);
static void APP_THREAD_CoapRespHandlerFuotaReboot(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result);
static void APP_THREAD_CoapRespHandlerFuotaParameters(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result);

static void APP_THREAD_FuotaSetParamApp(void);
static void APP_THREAD_FuotaSetParamCoproWireless(void);
static APP_THREAD_StatusTypeDef APP_THREAD_SetOtaContext(APP_THREAD_OtaFileTypeDef_t file_type);
static void APP_THREAD_FuotaProvisioning(void);
static void APP_THREAD_FuotaSend(void);
static uint32_t APP_THREAD_GetBinSize(void);
static void APP_THREAD_FuotaParameters(void);
static void APP_THREAD_FuotaReboot(void);
static uint32_t GetFirstSecureSector(void);
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
static otCoapResource OT_RessourceFuotaProvisioning = {C_RESSOURCE_FUOTA_PROVISIONING, NULL, NULL, NULL};
static otCoapResource OT_RessourceFuotaParameters = {C_RESSOURCE_FUOTA_PARAMETERS, NULL, NULL, NULL};
static otCoapResource OT_RessourceFuotaSend = {C_RESSOURCE_FUOTA_SEND, NULL, NULL, NULL};
static otCoapResource OT_RessourceFuotaReboot = {C_RESSOURCE_FUOTA_REBOOT, NULL, NULL, NULL};

static uint8_t OT_Command = 0;
static otMessageInfo  OT_MessageInfo = {0};
static otMessage*     pOT_Message = NULL;

static otIp6Address   OT_PeerAddress = { .mFields.m8 = { 0 } };

static uint64_t FuotaTransferArray[FUOTA_NUMBER_WORDS_64BITS] = {0};

static APP_THREAD_OtaContext_t OtaContext;
/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/

void APP_THREAD_Init( void )
{
  /* USER CODE BEGIN APP_THREAD_INIT_1 */
  APP_DBG("Thread_Ota_Server Application");
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
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_THREAD_FuotaSetParamApp);
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_BUTTON_SW3, UTIL_SEQ_RFU, APP_THREAD_FuotaSetParamCoproWireless);
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_FUOTA_SEND, UTIL_SEQ_RFU, APP_THREAD_FuotaSend);
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_FUOTA_PARAMETERS, UTIL_SEQ_RFU, APP_THREAD_FuotaParameters);
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_FUOTA_REBOOT, UTIL_SEQ_RFU, APP_THREAD_FuotaReboot);
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
  case ERR_THREAD_MSG_COMPARE_FAILED:
    APP_THREAD_TraceError("ERROR : ERR_THREAD_MSG_COMPARE_FAILED ",ErrCode);
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
  otCoapAddResource(NULL, &OT_RessourceFuotaProvisioning);

  otCoapAddResource(NULL, &OT_RessourceFuotaSend);

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
      //APP_DBG("Use Peer Address");
      memcpy(&OT_MessageInfo.mPeerAddr, aPeerAddress, sizeof(OT_MessageInfo.mPeerAddr));
    }
    else
    {
      APP_DBG("ERROR: Address string and Peer Address not defined");
      APP_THREAD_Error(ERR_THREAD_COAP_ADDRESS_NOT_DEFINED, 0);
    }

    if(aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE)
    {
      //APP_DBG("aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE");
      error = otCoapSendRequest(NULL,
          pOT_Message,
          &OT_MessageInfo,
          NULL,
          NULL);
    }
    if(aCoapType == OT_COAP_TYPE_CONFIRMABLE)
    {
      //APP_DBG("aCoapType == OT_COAP_TYPE_CONFIRMABLE");
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

/**
 * @brief This function manages the response handler for provisioning.
 *
 * @param pContext  Context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code
 * @retval None
 */
static void APP_THREAD_ProvisioningRespHandler(
    void                * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  if (Result == OT_ERROR_NONE)
  {
    if ((otMessageRead(pMessage, otMessageGetOffset(pMessage), &OT_Command, sizeof(OT_Command)) == sizeof(OT_Command)))
    {
      /* Retrieve the Message */
      if (otMessageRead(pMessage,
          otMessageGetOffset(pMessage) + sizeof(OT_Command),
          &OT_PeerAddress,
          sizeof(OT_PeerAddress)) != sizeof(OT_PeerAddress))
      {
        APP_THREAD_Error(ERR_THREAD_MESSAGE_READ, 0);
      }
      APP_DBG("\r -> Successfully retrieved Remote device address \n");

      /* Notify provisioning OK */
      UTIL_SEQ_SetTask(TASK_FUOTA_PARAMETERS, CFG_SCH_PRIO_1);
    }
  }
  else
  {
    APP_DBG("WARNING: APP_THREAD_CoapDataRespHandler fail with ERROR code = %d",Result);
  }
}

/**
 * @brief This function manages the response handler for FUOTA binary data.
 *
 * @param pContext  context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code
 * @retval None
 */
static void APP_THREAD_CoapRespHandlerFuotaSend(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  if (Result == OT_ERROR_NONE)
  {
    UTIL_SEQ_SetEvt(EVENT_TRANSFER_64BITS_DONE);
  }
  else
  {
    APP_DBG("APP_THREAD_CoapRespHandlerFuotaSend : WARNING Result %d", Result);
  }
}

/**
 * @brief Set FUOTA parameters for APP FW.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaSetParamApp(void)
{
  APP_THREAD_StatusTypeDef status;

  status = APP_THREAD_SetOtaContext(APP_THREAD_OTA_FILE_TYPE_FW_APP);

  if (status == APP_THREAD_OK)
  {
    APP_THREAD_FuotaProvisioning();
  }
  else
  {
    APP_DBG("WARNING: Setting Ota Context failed, do not send FUOTA PROVISIONING");
  }
}

/**
 * @brief Set FUOTA parameters for Copro Wireless FW.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaSetParamCoproWireless(void)
{
  APP_THREAD_StatusTypeDef status;

  status = APP_THREAD_SetOtaContext(APP_THREAD_OTA_FILE_TYPE_FW_COPRO_WIRELESS);

  if (status == APP_THREAD_OK)
  {
    APP_THREAD_FuotaProvisioning();
  }
  else
  {
    APP_DBG("WARNING: Setting Ota Context failed, do not send FUOTA PROVISIONING");
  }
}

/**
 * @brief Set Ota Context.
 * @param  None
 * @retval None
 */
static APP_THREAD_StatusTypeDef APP_THREAD_SetOtaContext(APP_THREAD_OtaFileTypeDef_t file_type)
{
  if (file_type == APP_THREAD_OTA_FILE_TYPE_FW_APP)
  {
    OtaContext.file_type = APP_THREAD_OTA_FILE_TYPE_FW_APP;
    /* Set base address for FUOTA download */
    OtaContext.base_address = FUOTA_APP_FW_BINARY_ADDRESS;
    OtaContext.magic_keyword = FUOTA_MAGIC_KEYWORD_APP;
  }
  else if (file_type == APP_THREAD_OTA_FILE_TYPE_FW_COPRO_WIRELESS)
  {
    OtaContext.file_type = APP_THREAD_OTA_FILE_TYPE_FW_COPRO_WIRELESS;
    /* Set base address for FUOTA download */
    OtaContext.base_address = FUOTA_COPRO_FW_BINARY_ADDRESS;
    OtaContext.magic_keyword = FUOTA_MAGIC_KEYWORD_COPRO_WIRELESS;
  }
  else
  {
    APP_DBG("WARNING: Unrecognized file_type = %d", file_type);
    return APP_THREAD_ERROR;
  }

  /* Set binary size */
  OtaContext.binary_size = APP_THREAD_GetBinSize();

  /* Return the function status */
  return APP_THREAD_OK;
}

/**
 * @brief Sends Provisioning request.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaProvisioning(void)
{
  APP_DBG("Send FUOTA PROVISONING request");

  uint8_t l_provisioning_data = 0x1;

  /* Send a MULTICAST CONFIRMABLE GET Request */
  APP_THREAD_CoapSendRequest(&OT_RessourceFuotaProvisioning,
      OT_COAP_TYPE_CONFIRMABLE,
      OT_COAP_CODE_GET,
      MULICAST_FTD_MED,
      NULL,
      &l_provisioning_data,
      sizeof(l_provisioning_data),
      &APP_THREAD_ProvisioningRespHandler,
      NULL);
}

static uint32_t APP_THREAD_GetBinSize(void)
{
  uint32_t flash_current_offset = 0;
  uint64_t read64 = 0;
  bool binary_parsing_on_going = TRUE;
  bool error_keyword_or_size = FALSE;
  uint32_t first_secure_sector_idx;
  uint32_t maximum_reachable_offset;
  uint32_t fuota_bin_size = 0;

  first_secure_sector_idx = GetFirstSecureSector();

  /* Compute Maximum reachable address */
  maximum_reachable_offset = (first_secure_sector_idx * 0x1000) - 1;
  APP_DBG("maximum_reachable_offset =  0x%x", maximum_reachable_offset);

  while(binary_parsing_on_going == TRUE){
    read64 = *(uint64_t*)(OtaContext.base_address + flash_current_offset);

    /* Test if current word contains magic keyword */
    if((read64 & 0x00000000FFFFFFFF) == OtaContext.magic_keyword)
    {
      binary_parsing_on_going = FALSE;
    }
    else if (((read64 & 0xFFFFFFFF00000000) >> 32) == OtaContext.magic_keyword)
    {
      binary_parsing_on_going = FALSE;
    }

    flash_current_offset += 8;

    /* Test current offset is still below SFSA (Non-Secure Memory Address) */
    if(flash_current_offset > maximum_reachable_offset)
    {
      error_keyword_or_size = TRUE;
      binary_parsing_on_going = FALSE;
    }
  }

  if(error_keyword_or_size == TRUE)
  {
    APP_DBG("Error: FUOTA Keyword not found in FLASH memory, aborting!");
    APP_THREAD_Error(ERR_THREAD_FUOTA_NO_KEYWORD, 0);
  }
  else
  {
    fuota_bin_size = flash_current_offset;
  }

  return fuota_bin_size;
}

static uint32_t GetFirstSecureSector(void)
{
  uint32_t first_secure_sector_idx, sfsa_field, sbrv_field, sbrv_field_sector;

  /* Read SFSA */
  sfsa_field = (READ_BIT(FLASH->SFR, FLASH_SFR_SFSA) >> FLASH_SFR_SFSA_Pos);
  APP_DBG("SFSA OB = 0x%x", sfsa_field);
  APP_DBG("SFSA Option Bytes set to sector = 0x%x (0x080%x)", sfsa_field, sfsa_field*4096);

  /* Read SBRV */
  /* Contains the word aligned CPU2 boot reset start address offset within the selected memory area by C2OPT. */
  sbrv_field = (READ_BIT(FLASH->SRRVR, FLASH_SRRVR_SBRV) >> FLASH_SRRVR_SBRV_Pos);
  APP_DBG("SBRV OB = 0x%x", sbrv_field);
  /* Divide sbrv_field by 1024 to be compared to SFSA value */
  sbrv_field_sector = sbrv_field / 1024;
  APP_DBG("SBRV Option Bytes set to sector = 0x%x (0x080%x)", sbrv_field_sector, sbrv_field*4);

  /* If SBRV is below SFSA then set first_secure_sector_idx to SBRV */
  if (sbrv_field_sector < sfsa_field)
  {
    first_secure_sector_idx = sbrv_field_sector;
  }
  else
  {
    first_secure_sector_idx = sfsa_field;
  }

  APP_DBG("first_secure_sector_idx = 0x%x", first_secure_sector_idx);

  return first_secure_sector_idx;
}

/**
 * @brief Task associated to FUOTA parameters to be sent.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaParameters(void)
{
  APP_DBG("FUOTA PROVISIONING OK, Set FUOTA PARAMETERS");

  if(OtaContext.file_type == APP_THREAD_OTA_FILE_TYPE_FW_APP)
  {
    APP_DBG("\r File type = FW_APP");
  }
  else  if (OtaContext.file_type == APP_THREAD_OTA_FILE_TYPE_FW_COPRO_WIRELESS)
  {
    APP_DBG("\r File type = FW_COPRO_WIRELESS");
  }
  else
  {
    APP_DBG("FUOTA_PARAMETERS: File Type not recognized");
    APP_THREAD_Error(ERR_THREAD_FUOTA_FILE_TYPE_NOT_RECOGNIZED, 0);
  }

  APP_DBG("\r Binary size = %d bytes", OtaContext.binary_size);
  APP_DBG("\r Magic keyword = 0x%x", OtaContext.magic_keyword);
  APP_DBG("\r Base address = 0x%x", OtaContext.base_address);

  /* Send a CONFIRMABLE PUT Request */
  APP_THREAD_CoapSendRequest(&OT_RessourceFuotaParameters,
      OT_COAP_TYPE_CONFIRMABLE,
      OT_COAP_CODE_PUT,
      NULL,
      &OT_PeerAddress,
      (uint8_t*)&OtaContext,
      sizeof(OtaContext),
      &APP_THREAD_CoapRespHandlerFuotaParameters,
      NULL);
}

/**
 * @brief Task associated to FUOTA binary data sending.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaSend(void)
{
  uint32_t flash_current_offset = 0x0;
  bool binary_transfer_on_going = TRUE;
  uint32_t l_debug_count = 0;
  uint32_t l_current_index_progress = 0;
  uint32_t FuotaProgressArray[100] = {0};
  uint32_t FuotaTransferArraySize = 0;
  uint32_t l_start_transfer_time = 0;
  uint32_t l_end_transfer_time = 0;
  double l_transfer_time = 0;
  double l_transfer_throughput = 0;
  uint8_t l_Fuota_progress_multiplied = 0;

  APP_DBG("FUOTA PARAMETERS SET, START FUOTA BINARY TRANSFER");

  /* Number of elements in OtaContext.binary_size */
  FuotaTransferArraySize = OtaContext.binary_size / FUOTA_PAYLOAD_SIZE;
  APP_DBG("\r -> %d elements of %d bytes to be transferred", FuotaTransferArraySize, FUOTA_PAYLOAD_SIZE);
  if(FuotaTransferArraySize < 100)
  {
    l_Fuota_progress_multiplied = 1;
    FuotaTransferArraySize = FuotaTransferArraySize * 10;
  }
  for (int index = 1; index < 100; index++)
  {
    FuotaProgressArray[index] = (FuotaTransferArraySize / 100) * index;
  }

  l_start_transfer_time = HAL_GetTick();

  while(binary_transfer_on_going == TRUE){

    /* Get data to be sent */
    /* Read data from flash memory */
    memcpy(FuotaTransferArray, (uint8_t*)(OtaContext.base_address + flash_current_offset), FUOTA_PAYLOAD_SIZE);

    /* Send a CONFIRMABLE PUT Request */
    APP_THREAD_CoapSendRequest(&OT_RessourceFuotaSend,
        OT_COAP_TYPE_CONFIRMABLE,
        OT_COAP_CODE_PUT,
        NULL,
        &OT_PeerAddress,
        (uint8_t*)&FuotaTransferArray,
        FUOTA_PAYLOAD_SIZE,
        &APP_THREAD_CoapRespHandlerFuotaSend,
        NULL);

    UTIL_SEQ_WaitEvt(EVENT_TRANSFER_64BITS_DONE);

    /* Test if magic Keyword is in FuotaTransferArray */
    for (int index = 0; index < FUOTA_NUMBER_WORDS_64BITS; ++index) {
      if((FuotaTransferArray[index] & 0x00000000FFFFFFFF) == OtaContext.magic_keyword)
      {
        APP_DBG("1 - FUOTA_MAGIC_KEYWORD found at flash_current_offset = %d", (flash_current_offset + (index*8)));
        binary_transfer_on_going = FALSE;
      }
      else
      if (((FuotaTransferArray[index] & 0xFFFFFFFF00000000) >> 32) == OtaContext.magic_keyword)
      {
        APP_DBG("2 - FUOTA_MAGIC_KEYWORD found at flash_current_offset = %d", (flash_current_offset + (index*8)));
        binary_transfer_on_going = FALSE;
      }
    }

    flash_current_offset += FUOTA_PAYLOAD_SIZE;

    /* Display Transfer Progress */
    if(l_debug_count == FuotaProgressArray[l_current_index_progress+1])
    {
      l_current_index_progress += 1;
      if(l_Fuota_progress_multiplied == 1)
      {
        APP_DBG("FUOTA Transfer %d%...", l_current_index_progress*10);
      }
      else
      {
        APP_DBG("FUOTA Transfer %d%...", l_current_index_progress);
      }
    }

    l_debug_count++;
  }

  l_end_transfer_time = HAL_GetTick();
  l_transfer_time = (double)(l_end_transfer_time - l_start_transfer_time) / 1000;
  l_transfer_throughput = (((double)OtaContext.binary_size/l_transfer_time) / 1000) * 8;

  APP_DBG("**************************************************************");
  APP_DBG(" FUOTA_SERVER : END OF TRANSFER COMPLETED");
  APP_DBG("  - Payload size = %d bytes", FUOTA_PAYLOAD_SIZE);
  APP_DBG("  - Transfer time = %.2f seconds", l_transfer_time);
  APP_DBG("  - Average throughput = %.2f kbit/s", l_transfer_throughput);
  APP_DBG("**************************************************************");

  /* Notify end of transfer to Thread_Ota ? */
}

/**
 * @brief Task associated to the push button.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FuotaReboot(void)
{
  uint32_t l_data = 0x1;
  APP_DBG("Send a request to current OTA application to reboot on Thread_Ota");

  /* Send a CONFIRMABLE PUT Request */
  APP_THREAD_CoapSendRequest(&OT_RessourceFuotaReboot,
      OT_COAP_TYPE_CONFIRMABLE,
      OT_COAP_CODE_PUT,
      MULICAST_FTD_MED,
      NULL,
      (uint8_t*)&l_data,
      sizeof(l_data),
      &APP_THREAD_CoapRespHandlerFuotaReboot,
      NULL);

  UTIL_SEQ_WaitEvt(EVENT_FUOTA_REBOOT_RESP_DONE);

  APP_DBG("Reboot request ACK from OTA application");
}

/**
 * @brief This function manages the response handler for the request of reboot on Thread_Ota.
 *
 * @param pContext  Context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code
 * @retval None
 */
static void APP_THREAD_CoapRespHandlerFuotaReboot(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  if (Result == OT_ERROR_NONE)
  {
    UTIL_SEQ_SetEvt(EVENT_FUOTA_REBOOT_RESP_DONE);
  }
  else
  {
    APP_DBG("APP_THREAD_CoapRespHandlerFuotaReboot : WARNING Result %d", Result);
  }
}

/**
 * @brief This function manages the response handler for the request of reboot on Thread_Ota.
 *
 * @param pContext  Context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param Result error code
 * @retval None
 */
static void APP_THREAD_CoapRespHandlerFuotaParameters(
    void * pContext,
    otMessage           * pMessage,
    const otMessageInfo * pMessageInfo,
    otError             Result)
{
  if (Result == OT_ERROR_NONE)
  {
    /* Read Message */
    if (otMessageRead(pMessage, otMessageGetOffset(pMessage), &OT_Command, sizeof(OT_Command)) == sizeof(OT_Command))
    {
      if (OT_Command == APP_THREAD_OK)
      {
        APP_DBG("FUOTA PARAMETERS: Correct ACK received");
        UTIL_SEQ_SetTask(TASK_FUOTA_SEND, CFG_SCH_PRIO_0);
      }
      else if (OT_Command == APP_THREAD_ERROR)
      {
        APP_DBG("FUOTA PARAMETERS: Bad ACK received");
        APP_DBG("\r -> STOP FUOTA process!");
      }
      else
      {
        APP_DBG("FUOTA PARAMETERS: Unrecognized ACK received");
        APP_DBG("\r -> STOP FUOTA process!");
      }
    }
    else
    {
      APP_DBG("WARNING : APP_THREAD_CoapRespHandlerFuotaParameters otMessageRead failed!");
    }
  }
  else
  {
    APP_DBG("APP_THREAD_CoapRespHandlerFuotaReboot : WARNING Result %d", Result);
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

  /* Copy the characters  in the temporary buffer */
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
