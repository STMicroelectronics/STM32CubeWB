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
#include "app_ble.h"
#include "stm32_lpm.h"

#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.identify.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT            17U
#define SW1_GROUP_ADDR          0x0001
#define CHANNEL                 13U
#define CHANNELMASK_12TO14      0x00007000U /* Channels 12-14 */
#define SED_SLEEP_TIME_30S                          1

#define COLOR_DEFAULT           "\x1b[0m"
#define COL_CYAN    "\x1b[0;96m"
#define COL_MAGENTA "\x1b[0;95m"
#define COL_RED     "\x1b[0;91m"
#define COL_NORM    "\x1b[0m"

#define DELAY_5_MS                        (0.005*1000*1000/CFG_TS_TICK_VAL) /**< 5ms */
#define DELAY_20_MS                       (0.02*1000*1000/CFG_TS_TICK_VAL) /**< 20ms */
#define DELAY_40_MS                       (0.04*1000*1000/CFG_TS_TICK_VAL) /**< 40ms */
#define DELAY_50_MS                       (0.05*1000*1000/CFG_TS_TICK_VAL) /**< 50ms */
#define DELAY_100_MS                      (100*1000/CFG_TS_TICK_VAL)  /**< 100 ms */
#define DELAY_300_MS                      (300000/CFG_TS_TICK_VAL)  /**< 500 ms */
#define DELAY_500_MS                      (0.5*1000*1000/CFG_TS_TICK_VAL)  /**< 500 ms */
#define DELAY_1_S                         (1000*1000/CFG_TS_TICK_VAL)  /**< 1 s */

#define TOGGLE_INTERVAL (1 * DELAY_1_S)       /* 1 sec interval between 2 toggles */

#define OUTPUT_LINE_MAX_LEN             256

/******************************************************************************
 * Persistence
 ******************************************************************************
 */
/* For certification testing, we need a little more than 2K for one of the tests.
 * Set to 4K to be safe. 4K is enough for a Coordinator to persist an 80-node
 * network. */
#define ST_PERSIST_MAX_ALLOC_SZ            (4U * 1024U)
#define ST_PERSIST_FLASH_DATA_OFFSET       4U

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_OnOff_Toggle(void);
static void APP_ZIGBEE_Process_OnOff_Toggle(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);


#ifdef WITH_PERSISTANT
static const void * APP_ZIGBEE_persist_load(unsigned int *bufLen);
static void APP_ZIGBEE_persist_delete(void);
static void APP_ZIGBEE_persist_buf_free(const void *buf);
#endif // WITH_PERSISTANT

static uint32_t APP_ZIGBEE_GetStartNb(void);
static void APP_ZIGBEE_IncrementStartNb(void);
static void toggle_cb(struct ZbZclCommandRspT *rsp, void *arg);

/* Private variables -----------------------------------------------*/
static TL_CmdPacket_t *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t CptReceiveNotifyFromM0 = 0;
static __IO uint32_t CptReceiveRequestFromM0 = 0;


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
static struct zigbee_app_info zigbee_app_info;

static uint32_t join_start_time;
static double join_time_duration;

/* Public variables -----------------------------------------------*/
uint8_t ZbStackType;            /* ZB stack type, static or dynamic, FFD or RFD */
uint32_t toggle_cnt = 0;
uint32_t toggle_fail = 0;
uint32_t disc_cnt = 0;
uint8_t Timer_ToggleOnOff_Id;

/* Keep track of number of Zigbee start */
static uint8_t zigbee_start_nb = 0U;
static bool zigbee_logging_done = FALSE;

/* Functions Definition ------------------------------------------------------*/
/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

void APP_ZIGBEE_Init(void)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;

  /* Do not allow stop mode before ZB is initialized */
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

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

  /* Task associated with Toggle On/Off */
  UTIL_SEQ_RegTask(1U << CFG_TASK_TOGGLE_ON_OFF, UTIL_SEQ_RFU, APP_ZIGBEE_OnOff_Toggle);

  /* Init the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /**
    * Create timer for Toggle On/Off process
    */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(Timer_ToggleOnOff_Id), hw_ts_Repeated, APP_ZIGBEE_Process_OnOff_Toggle);
  /**
    * Create timer for Network Status process
    */
  /* Initialize Zigbee stack layers and launch network formation */
  APP_ZIGBEE_StackLayersInit();

  APP_ZIGBEE_IncrementStartNb();

} /* APP_ZIGBEE_Init */


void APP_ZIGBEE_Stop(void)
{
  APP_DBG("APP_ZIGBEE_Stop");

  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Save Persistent data */
  APP_DBG("Save persistent data");
#ifdef WITH_PERSISTANT
  APP_ZIGBEE_persist_save();
#endif //WITH_PERSISTANT

  /* Zigbee STOP Procedure */
  /* Free memory allocated by Zigbee stack */
  if (zigbee_app_info.zb == NULL) {
      return;
  }
  //ZbIfDetach(zigbee_app_info.zb, &zigbee_app_info.device);
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
  APP_DBG("APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
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

    if (zigbee_logging_done == FALSE)
    {
      /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
      ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);
      zigbee_logging_done = TRUE;
    }

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    zigbee_app_info.startupControl = ZbStartTypeJoin;
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);   
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/
    //config.channelList.list[0].channelMask = CHANNELMASK_12TO14; /* Channels in use*/

    /* Add Sleepy End device configuration */
    config.capability &= ~(MCP_ASSOC_CAP_RXONIDLE
                          | MCP_ASSOC_CAP_DEV_TYPE
                          | MCP_ASSOC_CAP_ALT_COORD);
    config.endDeviceTimeout = SED_SLEEP_TIME_30S /* 30sec sleep time unit */;

    APP_DBG("Network config : APP_STARTUP_SED");

    APP_DBG("*** zigbee_start_nb value = %d ***", APP_ZIGBEE_GetStartNb());
#ifdef WITH_PERSISTANT
    if(APP_ZIGBEE_GetStartNb() < 2U)
#endif
    {
      /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
       * a blocking call on the M4. */
      status = ZbStartupWait(zigbee_app_info.zb, &config);

      APP_DBG("ZbStartup Callback (status = 0x%02x)\n", status);
      zigbee_app_info.join_status = status;

      if (status == ZB_STATUS_SUCCESS) {
        join_time_duration = (double)(HAL_GetTick() - join_start_time)/1000;
        APP_DBG("%s==> JOIN SUCCESS, Duration = (%.2f seconds)%s\n", COL_MAGENTA, join_time_duration, COL_NORM);
        zigbee_app_info.join_delay = 0U;
        zigbee_app_info.init_after_join = true;
        BSP_LED_On(LED_BLUE);
      }
      else
      {
        APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
        zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
      }
    }
#ifdef WITH_PERSISTANT
    else
    {
      /* Restart from persistence */
      if (APP_ZIGBEE_ZbStartupPersist(zigbee_app_info.zb) == ZB_STATUS_SUCCESS)
      {
        APP_DBG("APP_ZIGBEE_ZbStartupPersist SUCCESS!");
        zigbee_app_info.join_status = ZB_STATUS_SUCCESS;
        BSP_LED_On(LED_BLUE);
      }
      else
      {
        APP_DBG("APP_ZIGBEE_ZbStartupPersist FAILED!");
      }
    }
#endif //WITH_PERSISTANT
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  else  /* JOIN successful */
  {
    zigbee_app_info.init_after_join = false;

    /* Do it only first time */
    if(APP_ZIGBEE_GetStartNb() == 1U)
    {
#if (CFG_FULL_LOW_POWER == 1)
      /* Enabling Stop mode */
      UTIL_LPM_SetStopMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
#endif /* CFG_FULL_LOW_POWER */
      /* Assign ourselves to the group addresses */
      APP_ZIGBEE_ConfigGroupAddr();

      /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
      uint32_t bcast_timeout = 3;
      ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));

      APP_DBG("==> Start_ZB Task Toggle");
      /* Start First Toggle */
      APP_ZIGBEE_Process_OnOff_Toggle();
      /* Next toggle after TOGGLE_INTERVAL */
      HW_TS_Start(Timer_ToggleOnOff_Id, (uint32_t)TOGGLE_INTERVAL);
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


static void APP_ZIGBEE_IncrementStartNb(void)
{
  zigbee_start_nb++;
}

static uint32_t APP_ZIGBEE_GetStartNb(void)
{
  return zigbee_start_nb;
}

/* Persistence */
/**
 * @brief  Start Zigbee Network with data from persistent memory
 * @param  zb : Zigbee Device object
 * @retval None
 */
#ifdef WITH_PERSISTANT
enum ZbStatusCodeT APP_ZIGBEE_ZbStartupPersist(struct ZigBeeT *zb)
{
  const void *buf_ptr;
  unsigned int buf_len;
  enum ZbStatusCodeT status = ZB_STATUS_SUCCESS;

  /* Restore persistence */
  buf_ptr = APP_ZIGBEE_persist_load(&buf_len);
  APP_ZIGBEE_persist_delete();

  if (buf_ptr != NULL) {
    APP_DBG("APP_ZIGBEE_ZbStartupPersist: restoring stack persistence");
    /* FIXME 0 - CBKE config? */
    status = ZbStartupPersist(zb, buf_ptr, buf_len, NULL);
    APP_ZIGBEE_persist_buf_free(buf_ptr);

  }else
  {
    status = ZB_STATUS_ALLOC_FAIL;
  }

  return status;

}

/**
 * @brief  Load persistent data
 * @param  bufLen : pointer on buffer length
 * @retval None
 */
static const void * APP_ZIGBEE_persist_load(unsigned int *bufLen)
{
    uint8_t *buf;
    uint32_t persist_len;

    buf = malloc(ST_PERSIST_MAX_ALLOC_SZ);
    if (buf == NULL) {
        APP_DBG("APP_ZIGBEE_persist_load : memory exhausted!");
        return NULL;
    }

    /* Read the persistence length */
    if (utilsFlashRead(0, buf, ST_PERSIST_FLASH_DATA_OFFSET) != ST_PERSIST_FLASH_DATA_OFFSET) {
        APP_DBG("APP_ZIGBEE_persist_load : failed to read length from Flash!");
        return NULL;
    }
    persist_len = pletoh32(buf);
    APP_DBG("ZIGBBE Persistent data length = %d ", persist_len);
    if (persist_len > ST_PERSIST_MAX_ALLOC_SZ) {
        APP_DBG("APP_ZIGBEE_persist_load : invalid length = %d!", persist_len);
        return NULL;
    }

    if (utilsFlashRead(ST_PERSIST_FLASH_DATA_OFFSET, buf, persist_len) != persist_len) {
        APP_DBG("APP_ZIGBEE_persist_load : failed to read persist data from Flash!");
        return NULL;
    }

    APP_DBG("Successfully retrieved data from persistence");

    *bufLen = persist_len;
    return buf;
}

/**
 * @brief  Delete first word of persistent data (size = 0 meaning no data)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_persist_delete(void)
{
    uint8_t len_buf[ST_PERSIST_FLASH_DATA_OFFSET];

    putle32(len_buf, 0);
    if (utilsFlashWrite(0, len_buf, ST_PERSIST_FLASH_DATA_OFFSET) != ST_PERSIST_FLASH_DATA_OFFSET) {
        APP_DBG("APP_ZIGBEE_persist_delete : failed to write flash");
    }
}

/**
 * @brief  Free buffer
 * @param  buf : pointer on buffer to free
 * @retval None
 */
static void APP_ZIGBEE_persist_buf_free(const void *buf)
{
    free((void *)buf);
}

/**
 * @brief  Save persistent data in FLASH (Or Flash Emulation in RAM)
 * @param  None
 * @retval None
 */
bool APP_ZIGBEE_persist_save(void)
{
    uint8_t *buf;
    uint8_t len_buf[ST_PERSIST_FLASH_DATA_OFFSET];
    unsigned int len;

    len = ZbPersistGet(zigbee_app_info.zb, 0, 0);
    if (len == 0U) {
        /* If the persistence length was zero, then remove the file. */
        APP_DBG("APP_ZIGBEE_persist_save: no persistence data!");
        //cli_persist_delete(cli_p, filename);
        return true;
    }
    if (len > ST_PERSIST_MAX_ALLOC_SZ) {
        APP_DBG("APP_ZIGBEE_persist_save: persist size too large for storage (%d)", len);
        return false;
    }

    buf = malloc(ST_PERSIST_MAX_ALLOC_SZ);
    if (buf == NULL) {
        APP_DBG("APP_ZIGBEE_persist_save: memory exhausted");
        return false;
    }

    len = ZbPersistGet(zigbee_app_info.zb, buf, len);

    /* Write the length */
    putle32(len_buf, len);
    if (utilsFlashWrite(0, len_buf, ST_PERSIST_FLASH_DATA_OFFSET) != ST_PERSIST_FLASH_DATA_OFFSET) {
        APP_DBG("APP_ZIGBEE_persist_save: failed to write flash");
        free(buf);
        return false;
    }

    /* Write the persistent data */
    if (utilsFlashWrite(ST_PERSIST_FLASH_DATA_OFFSET, buf, len) != len) {
        APP_DBG("APP_ZIGBEE_persist_save: failed to write flash");
        free(buf);
        return false;
    }

    free(buf);

    zigbee_app_info.persistNumWrites++;
    APP_DBG("APP_ZIGBEE_persist_save: Persistence written (num writes = %d)", zigbee_app_info.persistNumWrites);
    return true;
}
#endif //WITH_PERSISTANT



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

    ZbStackType = p_wireless_info->StackType; /* Memorize ZB stack type */
    switch (p_wireless_info->StackType) {
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_STATIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_FFD_STATIC");
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC");
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_RFD_STATIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_RFD_STATIC");
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_RFD_DYNAMIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_RFD_DYNAMIC");
      break;
    case INFO_STACK_TYPE_ZIGBEE_FFD:
      APP_DBG("FW Type : FFD Zigbee stack");
      break;
   case INFO_STACK_TYPE_ZIGBEE_RFD:
      APP_DBG("FW Type : RFD Zigbee stack");
      break;
    default:
      /* No Zigbee device supported ! */
      APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

static void APP_ZIGBEE_SW1_Process(void)
{
  struct ZbApsAddrT dst;

  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_GROUP;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = SW1_GROUP_ADDR;

  APP_DBG("==> SENDING TOGGLE TO GROUP 0x0001");
  if (ZbZclOnOffClientToggleReq(zigbee_app_info.onoff_client_1, &dst, NULL, NULL) != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)");
  }
}

static void APP_ZIGBEE_Process_OnOff_Toggle(void)
{
  UTIL_SEQ_SetTask(1U << CFG_TASK_TOGGLE_ON_OFF, CFG_SCH_PRIO_1);
}

static void APP_ZIGBEE_OnOff_Toggle(void)
{
  struct ZbApsAddrT dst;

  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;

  if (ZbZclOnOffClientToggleReq(zigbee_app_info.onoff_client_1, &dst, toggle_cb, NULL) != ZCL_STATUS_SUCCESS)
  {
    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)");
  }
  toggle_cnt++;
  UTIL_SEQ_WaitEvt(EVENT_ON_OFF_RSP);
  APP_DBG_FULL(LOG_LEVEL_INFO, APPLI_LOG_REGION_GENERAL, "==> Nb TOGGLE ON/OFF sent: %d (%d Fail - %.2f%%) - %d BLE Disc %s",
      toggle_cnt, toggle_fail, (double)(toggle_fail * 100)/(double)toggle_cnt, disc_cnt, COLOR_DEFAULT);
}


static void toggle_cb(struct ZbZclCommandRspT *rsp, void *arg)
{
  if(rsp->status != ZCL_STATUS_SUCCESS)
  {
    APP_DBG("TOGGLE RSP FAIL status %d",rsp->status);
    toggle_fail++;
  }
  else
  {
    APP_DBG("TOGGLE RSP SUCCESS from %#08llx",rsp->src.extAddr);
  }
  UTIL_SEQ_SetEvt(EVENT_ON_OFF_RSP);
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

