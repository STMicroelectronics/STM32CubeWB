/**
  ******************************************************************************
  * File Name          : App/app_zigbee.c
  * Description        : Zigbee Application.
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

#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.identify.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT            17U
#define SW1_GROUP_ADDR          0x0001
#define CHANNEL                 13U

/* external definition */
extern const uint8_t sec_key_ha[ZB_SEC_KEYSIZE];
extern ConcurentModeStateType APP_GetCurrentProtocolMode(void);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_FreeAppConfig(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);
static void APP_ZIGBEE_persist_delete(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);

static enum ZbStatusCodeT APP_ZIGBEE_ZbStartupPersist(struct ZigBeeT* zb);
static void APP_ZIGBEE_persist_notify_cb(struct ZigBeeT *zb, void *cbarg);
static bool APP_ZIGBEE_persist_save(void);
static bool APP_ZIGBEE_persist_load(void);

static uint32_t APP_ZIGBEE_GetCompleteJoinCpt(void);
static void APP_ZIGBEE_IncrCompleteJoinCpt(void);
static void APP_ZIGBEE_ResetCompleteJoinCpt(void);

/* Private variables -----------------------------------------------*/
static TL_CmdPacket_t *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t CptReceiveNotifyFromM0 = 0;
static __IO uint32_t CptReceiveRequestFromM0 = 0;


extern int My_APP_GetCurrentProtocolMode(void);

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
uint8_t g_ot_notification_allowed = 0U;

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;
  uint32_t persistNumWrites;

  struct ZbZclClusterT *onoff_client_1;
};

/* NVM variables */
/* cache in uninit RAM to store/retrieve persistent data */
union cache
{
  uint8_t  U8_data[ST_PERSIST_MAX_ALLOC_SZ];     // in bytes
  uint32_t U32_data[ST_PERSIST_MAX_ALLOC_SZ/4U]; // in U32 words
};
__attribute__ ((section(".noinit"))) union cache cache_persistent_data;

static struct zigbee_app_info zigbee_app_info;
static uint32_t join_start_time;
static uint32_t join_time_duration;
static uint8_t zigbee_complete_join_cpt = 1U;

/* Functions Definition ------------------------------------------------------*/
/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

void APP_ZIGBEE_Init(void)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;

  APP_DBG("APP_ZIGBEE_Init");
  APP_DBG("STARTING ON CHANNEL = %d", CHANNEL);

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_ZIGBEE_CheckWirelessFirmwareInfo();

  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessNotifyM0ToM4);
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessRequestM0ToM4);

  /* Task associated with network creation process */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, UTIL_SEQ_RFU, APP_ZIGBEE_NwkForm);

  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);

  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();

} /* APP_ZIGBEE_Init */

void APP_ZIGBEE_Stop(void)
{
  APP_DBG("APP_ZIGBEE_Stop");

  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Free Zigbee application configuration */
  APP_ZIGBEE_FreeAppConfig();

  /* Save Persistent data */
  APP_DBG("Save persistent data");
  APP_ZIGBEE_persist_save();

  /* Zigbee STOP Procedure */
  /* Free memory allocated by Zigbee stack */
  if (zigbee_app_info.zb == NULL) {
      return;
  }
  ZbDestroy(zigbee_app_info.zb);
  zigbee_app_info.zb = NULL;
}

/**
 * @brief  Initialize Zigbee stack layers
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_StackLayersInit(void)
{
  HAL_Delay(1000);
  APP_DBG("APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
  APP_DBG("APP_ZIGBEE_StackLayersInit apres = %d",zigbee_app_info.zb);
  assert(zigbee_app_info.zb != NULL);

  /* Create the endpoint and cluster(s) */
   APP_ZIGBEE_ConfigEndpoints();

  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  zigbee_app_info.startupControl = ZbStartTypeJoin;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  join_start_time = HAL_GetTick();
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
}

/**
 * @brief  Configuration of the end points
 * @param  None
 * @retval None
 */

static void APP_ZIGBEE_ConfigEndpoints(void)
{
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;

  /* Endpoint: SW1_ENDPOINT */
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  /* OnOff Client */
  zigbee_app_info.onoff_client_1 = ZbZclOnOffClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT);
  assert(zigbee_app_info.onoff_client_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.onoff_client_1);

} /* config_endpoints */

/**
 * @brief  Release application configuration
 * @param  None
 * @retval None
 */

static void APP_ZIGBEE_FreeAppConfig(void)
{
  struct ZbApsmeRemoveEndpointReqT req1;
  struct ZbApsmeRemoveEndpointConfT conf1;
  struct ZbApsmeRemoveAllGroupsReqT req2;
  struct ZbApsmeRemoveAllGroupsConfT conf2;

  memset(&req1, 0, sizeof(req1));

  /* Free OnOff Client */
  ZbZclClusterFree(zigbee_app_info.onoff_client_1);

  /* Remove end points and groups  */
  req1.endpoint = SW1_ENDPOINT;
  ZbZclRemoveEndpoint(zigbee_app_info.zb, &req1, &conf1);

  req2.endpt = SW1_ENDPOINT;
  ZbApsmeRemoveAllGroupsReq(zigbee_app_info.zb,&req2, &conf2);
} /* APP_ZIGBEE_FreeAppConfig */

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{

  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  {
    struct ZbStartupT config;
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    zigbee_app_info.startupControl = ZbStartTypeJoin;
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);   
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/

    APP_DBG("*** zigbee_complete_join_cpt value = %d ***", APP_ZIGBEE_GetCompleteJoinCpt());

    ZbPersistNotifyRegister(zigbee_app_info.zb,NULL,NULL);

    if(APP_ZIGBEE_GetCompleteJoinCpt() < 2U)
    {
      /* Clear the persistemt data before starting a complete join */
      APP_ZIGBEE_persist_delete();
      /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
       * a blocking call on the M4. */
      status = ZbStartupWait(zigbee_app_info.zb, &config);

      APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
      zigbee_app_info.join_status = status;

      if (status == ZB_STATUS_SUCCESS) {
        join_time_duration = HAL_GetTick() - join_start_time;
        APP_DBG("JOIN SUCCESS, Duration = (%d ms)", join_time_duration);
        zigbee_app_info.join_delay = 0U;
        zigbee_app_info.init_after_join = true;
        APP_ZIGBEE_IncrCompleteJoinCpt();
        BSP_LED_On(LED_BLUE);
      }
      else
      {
        APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
        zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
      }
    }
    else
    {
       /* Disabling of the notification */
       ZbPersistNotifyRegister(zigbee_app_info.zb,NULL,NULL);

       /* Restart from persistence */
       if (APP_ZIGBEE_ZbStartupPersist(zigbee_app_info.zb) == ZB_STATUS_SUCCESS)
       {
         APP_DBG("APP_ZIGBEE_ZbStartupPersist SUCCESS!");
         zigbee_app_info.join_status = ZB_STATUS_SUCCESS;

         /* Register Persistent data change notification */
         ZbPersistNotifyRegister(zigbee_app_info.zb,APP_ZIGBEE_persist_notify_cb,NULL);
         /* Call the callback once here to save persistence data */
         APP_ZIGBEE_persist_notify_cb(zigbee_app_info.zb,NULL);

         BSP_LED_On(LED_BLUE);
       }
       else
       {
           APP_DBG("APP_ZIGBEE_ZbStartupPersist FAILED!");
           APP_ZIGBEE_ResetCompleteJoinCpt();
       }
    }
  }
  /* If Network forming/joining was not successful reschedule the current task to retry the process
   * unless it is requested to switch to BLE
   */
  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
      && (APP_GetCurrentProtocolMode() == State_Zigbee))
  {
         UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_1);
  }
  else
  {
    zigbee_app_info.init_after_join = false;

    /* Do it only first time */
    if(APP_ZIGBEE_GetCompleteJoinCpt() == 2U)
    {
      /* Assign ourselves to the group addresses */
      APP_ZIGBEE_ConfigGroupAddr();

      /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
      uint32_t bcast_timeout = 3;
      ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
    }
  }
}

/**
 * @brief  Set group addressing mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigGroupAddr(void)
{
  struct ZbApsmeAddGroupReqT req;
  struct ZbApsmeAddGroupConfT conf;

  memset(&req, 0, sizeof(req));
  req.endpt = SW1_ENDPOINT;
  req.groupAddr = SW1_GROUP_ADDR;
  ZbApsmeAddGroupReq(zigbee_app_info.zb, &req, &conf);

} /* APP_ZIGBEE_ConfigGroupAddr */

/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/
struct ZbStartupWaitInfo {
  bool active;
  enum ZbStatusCodeT status;
};


static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
  struct ZbStartupWaitInfo *info = cb_arg;

  info->status = status;
  info->active = false;
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_STARTUP_ENDED);
} /* ZbStartupWaitCb */

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL) {
    return ZB_STATUS_ALLOC_FAIL;
  }
  memset(info, 0, sizeof(struct ZbStartupWaitInfo));

  info->active = true;
  status = ZbStartup(zb, config, ZbStartupWaitCb, info);
  if (status != ZB_STATUS_SUCCESS)
  {
    free(info);
    return status;
  }
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_STARTUP_ENDED);
  status = info->status;
  free(info);
  return status;
} /* ZbStartupWait */


/**
 * @brief  Increment counter
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_IncrCompleteJoinCpt(void)
{
  zigbee_complete_join_cpt ++;
}

/**
 * @brief  Return counter value
 * @param  None
 * @retval Counter value
 */
static uint32_t APP_ZIGBEE_GetCompleteJoinCpt(void)
{
  return zigbee_complete_join_cpt;
}

/**
 * @brief  Reset counter value
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ResetCompleteJoinCpt(void)
{
  zigbee_complete_join_cpt = 1;
}

/**
 * @brief  Start Zigbee Network from persistent data
 * @param  zb: Zigbee device object pointer
 * @retval Zigbee stack Status code
 */
static enum ZbStatusCodeT APP_ZIGBEE_ZbStartupPersist(struct ZigBeeT* zb)
{
   bool read_status;
   enum ZbStatusCodeT status = ZB_STATUS_SUCCESS;

   /* Restore persistence */
   read_status = APP_ZIGBEE_persist_load();

   if (read_status)
   {
       /* Make sure the EPID is cleared, before we are allowed to restore persistence */
       uint64_t epid = 0U;
       ZbNwkSet(zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(uint64_t));

       /* Start-up from persistence */
       APP_DBG("APP_ZIGBEE_ZbStartupPersist: restoring stack persistence");
       status = ZbStartupPersist(zb, &cache_persistent_data.U8_data[4], cache_persistent_data.U32_data[0],NULL,NULL,NULL);
   }
   else
   {
       /* Failed to restart from persistence */
       APP_DBG("APP_ZIGBEE_ZbStartupPersist: no persistence data to restore");
       status = ZB_STATUS_ALLOC_FAIL;
   }

   return status;
}/* APP_ZIGBEE_ZbStartupPersist */

/**
 * @brief  notify to save persistent data callback
 * @param  zb: Zigbee device object pointer, cbarg: callback arg pointer
 * @retval None
 */
static void APP_ZIGBEE_persist_notify_cb(struct ZigBeeT *zb, void *cbarg)
{
  APP_DBG("Notification to save persistent data requested from stack");
  /* Save the persistent data */
  APP_ZIGBEE_persist_save();
}


/**
 * @brief  Load persistent data
 * @param  None
 * @retval true if success, false if fail
 */
static bool APP_ZIGBEE_persist_load()
{
    /* Check length range */
    if ((cache_persistent_data.U32_data[0] == 0) ||
        (cache_persistent_data.U32_data[0] > ST_PERSIST_MAX_ALLOC_SZ))
    {
        APP_DBG("No data or too large length : %d",cache_persistent_data.U32_data[0]);
        return false;
    }
    return true;
} /* APP_ZIGBEE_persist_load */

/**
 * @brief  Save persistent data
 * @param  None
 * @retval true if success , false if fail
 */
static bool APP_ZIGBEE_persist_save(void)
{
    uint32_t len;

    /* Clear the RAM cache before saving */
    memset(cache_persistent_data.U8_data, 0x00, ST_PERSIST_MAX_ALLOC_SZ);

    len = ZbPersistGet(zigbee_app_info.zb, 0, 0);
    /* Check Length range */
    if (len == 0U)
    {
        /* If the persistence length was zero then no data available. */
        APP_DBG("APP_ZIGBEE_persist_save: no persistence data to save !");
        return false;
    }
    if (len > ST_PERSIST_MAX_ALLOC_SZ)
    {
        /* if persistence length to big to store */
        APP_DBG("APP_ZIGBEE_persist_save: persist size too large for storage (%d)", len);
        return false;
    }

    /* Store in cache the persistent data */
    len = ZbPersistGet(zigbee_app_info.zb, &cache_persistent_data.U8_data[ST_PERSIST_FLASH_DATA_OFFSET], len);

    /* Store in cache the persistent data length */
    cache_persistent_data.U32_data[0] = len;

    zigbee_app_info.persistNumWrites++;
    APP_DBG("APP_ZIGBEE_persist_save: Persistence written in cache RAM (num writes = %d) len=%d",
             zigbee_app_info.persistNumWrites, cache_persistent_data.U32_data[0]);

    return true;
} /* APP_ZIGBEE_persist_save */


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
 *        and display associated information
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
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_STATIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_FFD_STATIC");
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_RFD_STATIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_RFD_STATIC");
      break;
    default:
      /* No Zigbee device supported ! */
      APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

static void APP_ZIGBEE_SW1_Process()
{
  struct ZbApsAddrT dst;

  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_GROUP;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = SW1_GROUP_ADDR;

  APP_DBG("SW1 PUSHED (SENDING TOGGLE TO GROUP 0x0001)");
  if (ZbZclOnOffClientToggleReq(zigbee_app_info.onoff_client_1, &dst, NULL, NULL) != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)");
  }
}

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

Zigbee_Cmd_Request_t * ZIGBEE_Get_M0RequestPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_request_M0_to_M4)->evtserial.evt.payload;
}

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

  TL_ZIGBEE_SendM4RequestToM0();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();
} /* ZIGBEE_CmdTransfer */

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0 with notification
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransferWithNotif(void)
{
	g_ot_notification_allowed = 1;
	ZIGBEE_CmdTransfer();
}

/**
 * @brief  This function is called when the M0+ acknowledge  the fact that it has received a Cmd
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
  CptReceiveNotifyFromM0++;
  UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief  This function is called when a request from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_M0RequestReceived(TL_EvtPacket_t *Reqbuffer)
{
    p_ZIGBEE_request_M0_to_M4 = Reqbuffer;

    CptReceiveRequestFromM0++;
    UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief Perform initialization of TL for Zigbee.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_TL_INIT(void)
{
    ZigbeeConfigBuffer.p_ZigbeeOtCmdRspBuffer = (uint8_t *)&ZigbeeOtCmdBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotAckBuffer = (uint8_t *)ZigbeeNotifRspEvtBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotifRequestBuffer = (uint8_t *)ZigbeeNotifRequestBuffer;
    TL_ZIGBEE_Init(&ZigbeeConfigBuffer);
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void)
{
  if (CptReceiveNotifyFromM0 != 0)
  {
    /* Reset counter */
    CptReceiveNotifyFromM0 = 0;
    Zigbee_CallBackProcessing();
  }
}

/**
 * @brief Process the requests coming from the M0.
 * @param None
 * @return None
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
    if (CptReceiveRequestFromM0 != 0) {
        CptReceiveRequestFromM0 = 0;
        Zigbee_M0RequestProcessing();
    }
}

/**
 * @brief  Delete persistent data
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_persist_delete(void)
{
  /* Clear RAM cache */
   memset(cache_persistent_data.U8_data, 0x00, ST_PERSIST_MAX_ALLOC_SZ);
   APP_DBG("Persistent Data RAM cache cleared");

} /* APP_ZIGBEE_persist_delete */
