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

#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.identify.h"

#include "ee.h"
#include "hw_flash.h"
#include "flash_driver.h"

#include "app_ble.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT            17U
#define SW1_GROUP_ADDR          0x0001
#define CHANNEL                 13U
#define HW_TS_SERVER_1MS_NB_TICKS                    (1*1000/CFG_TS_TICK_VAL)

#define CFG_NVM                 1U /* use FLASH */
//#undef CFG_NVM 

#define COLOR_DEFAULT           "\x1b[0m"
#define COL_CYAN    "\x1b[0;96m"
#define COL_MAGENTA "\x1b[0;95m"
#define COL_RED     "\x1b[0;91m"
#define COL_NORM    "\x1b[0m"

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);

static uint32_t APP_ZIGBEE_GetStartNb(void);
static void APP_ZIGBEE_IncrementStartNb(void);

static bool APP_ZIGBEE_persist_load(void);
bool APP_ZIGBEE_persist_save(void);
static void APP_ZIGBEE_persist_delete(void);
static void APP_ZIGBEE_persist_notify_cb(struct ZigBeeT *zb, void *cbarg);
enum ZbStatusCodeT APP_ZIGBEE_ZbStartupPersist(struct ZigBeeT *zb);
static void APP_ZIGBEE_PersistCompleted_callback(enum ZbStatusCodeT status,void *arg);

#ifdef CFG_NVM
static void APP_ZIGBEE_NVM_Init(void);
static bool APP_ZIGBEE_NVM_Read(void);
static bool APP_ZIGBEE_NVM_Write(void);
static void APP_ZIGBEE_NVM_Erase(void);
#endif /* CFG_NVM */

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

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;
  bool fresh_startup;
  uint32_t persistNumWrites;

  struct ZbZclClusterT *onoff_client_1;
};
static struct zigbee_app_info zigbee_app_info;

static uint32_t join_start_time;
static double join_time_duration;

/* NVM variables */
/* cache in uninit RAM to store/retrieve persistent data */
union cache
{
  uint8_t  U8_data[ST_PERSIST_MAX_ALLOC_SZ];     // in bytes 
  uint32_t U32_data[ST_PERSIST_MAX_ALLOC_SZ/4U]; // in U32 words
};
__attribute__ ((section(".noinit"))) union cache cache_persistent_data;

/* Public variables -----------------------------------------------*/
uint8_t ZbStackType;            /* ZB stack type, static or dynamic */
uint32_t toggle_cnt = 0;
uint32_t toggle_fail = 0;
uint32_t disc_cnt = 0;

/* Keep track of number of Zigbee start */
static uint8_t zigbee_start_nb = 0U;
static bool zigbee_logging_done = FALSE;

uint8_t TS_ID1, TS_ID2;

/* Functions Definition ------------------------------------------------------*/
/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);
extern uint16_t APP_BLE_GetConnectionInterval(bool *deviceConnected);
extern bool APP_BLE_ConnectionUpdate(uint16_t connectionInterval);

void APP_ZIGBEE_Init_Step1(void)
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

  /* Task associated with push button SW1 and SW2 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);

  /* Init the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);
} /* APP_ZIGBEE_Init_Step1 */


void APP_ZIGBEE_Init_Step2(void)
{
  /* NVM Init */
#if CFG_NVM
  APP_ZIGBEE_NVM_Init(); 
#endif
  
  /* Initialize Zigbee stack layers and launch network formation */
  APP_ZIGBEE_StackLayersInit();

  APP_ZIGBEE_IncrementStartNb();

} /* APP_ZIGBEE_Init_Step2 */

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
  enum ZbStatusCodeT status;
  
  HAL_Delay(1000);
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
  
  /* STEP 1 - TRY to START FROM PERSISTENCE */
  
  /* define if we need to do a fresh start */
  zigbee_app_info.fresh_startup = true;
  
  /* First we disable the persistent notification */
  ZbPersistNotifyRegister(zigbee_app_info.zb,NULL,NULL);
 
  /* Call a startup from persistence */
  status = APP_ZIGBEE_ZbStartupPersist(zigbee_app_info.zb);
  if(status == ZB_STATUS_SUCCESS)
  {
     /* no fresh stratup need anymore */
     zigbee_app_info.fresh_startup = false; 
     APP_DBG("ZbStartupPersist: SUCCESS, restarted from persistence");
     BSP_LED_On(LED_GREEN);
  }
  else
  {
       /* Start-up form persistence failed perform a fresh ZbStartup */
       APP_DBG("ZbStartupPersist: FAILED to restart from persistence with status: 0x%02x",status);
  }
  
  if(zigbee_app_info.fresh_startup)
  {
    /* Go for fresh start */
    join_start_time = HAL_GetTick();
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
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

    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    zigbee_app_info.startupControl = ZbStartTypeJoin;
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);   
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/

    APP_DBG("*** zigbee_start_nb value = %d ***", APP_ZIGBEE_GetStartNb());
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
        
        /* Register Persistent data change notification */
        ZbPersistNotifyRegister(zigbee_app_info.zb,APP_ZIGBEE_persist_notify_cb,NULL);
        /* Call the callback once here to save persistence data */
        APP_ZIGBEE_persist_notify_cb(zigbee_app_info.zb,NULL);
      }
      else
      {
        APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
        zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
      }
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  else
  {
    zigbee_app_info.init_after_join = false;

    /* Do it only first time */
    if(APP_ZIGBEE_GetStartNb() == 1U)
    {
      /* Assign ourselves to the group addresses */
      APP_ZIGBEE_ConfigGroupAddr();

      /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
      uint32_t bcast_timeout = 3;
      ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));

      switch (ZbStackType) { /* Check ZB stack type */
      case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_STATIC: /* Start ZB only */
        break;
      case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC:  /* Start ZB/BLE Dynamic mode */
        /* Start BLE App + Advertising*/
        APP_DBG("==> Restart_BLE Advertising");
        UTIL_SEQ_SetTask(1U << CFG_TASK_APP_ADV_START, CFG_SCH_PRIO_1);
        break;
      default:
        /* No Zigbee device supported ! */
        APP_DBG("FW Type : No ZB STACK type detected");
        break;
      }
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

/**
 * @brief  startup wait function
 * @param  zb :Zigbee device object pointer, config: startup config pointer
 * @param  ErrCode
 * @retval zigbee status stack code
 */
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
  if (status != ZB_STATUS_SUCCESS) {
    info->active = false;
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


/*************************************************************
 *
 * NVM FUNCTIONS
 *
 *************************************************************/
/**
 * @brief  notify to save persitent data callback
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
 * @brief  Start Zigbee Network from persistent data
 * @param  zb: Zigbee device object pointer
 * @retval Zigbee stack Status code
 */
 enum ZbStatusCodeT APP_ZIGBEE_ZbStartupPersist(struct ZigBeeT* zb)
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
       status = ZbStartupPersist(zb, &cache_persistent_data.U8_data[4], cache_persistent_data.U32_data[0],NULL,APP_ZIGBEE_PersistCompleted_callback,NULL);
   }
   else
   {
       /* Failed toi restart from persistence */ 
       APP_DBG("APP_ZIGBEE_ZbStartupPersist: no persistence data to restore");
       status = ZB_STATUS_ALLOC_FAIL;
   }

   /* Only for debug purpose, depending of persistent data, following traces 
      could display bytes that are irrelevants to on off cluster */ 
   if(status == ZB_STATUS_SUCCESS)
   {
     /* read the last bytes of data where the ZCL on off persitent data shall be*/
      uint32_t len = cache_persistent_data.U32_data[0] + 4 ;
      APP_DBG("ClusterID %02x %02x",cache_persistent_data.U8_data[len-9],cache_persistent_data.U8_data[len-10]);
      APP_DBG("Endpoint %02x %02x",cache_persistent_data.U8_data[len-7],cache_persistent_data.U8_data[len-8]);
      APP_DBG("Direction %02x",cache_persistent_data.U8_data[len-6]);
      APP_DBG("AttrID %02x %02x",cache_persistent_data.U8_data[len-4],cache_persistent_data.U8_data[len-5]);
      APP_DBG("Len %02x %02x",cache_persistent_data.U8_data[len-2],cache_persistent_data.U8_data[len-3]);
      APP_DBG("Value %02x",cache_persistent_data.U8_data[len-1]);
   }
   
   return status;
}/* APP_ZIGBEE_ZbStartupPersist */

/**
 * @brief  timer callback to wait end of restore cluster persistence form M0 
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PersistCompleted_callback(enum ZbStatusCodeT status,void *arg)
{
   if(status == ZB_WPAN_STATUS_SUCCESS)
   {
     APP_DBG("Persist complete callback entered with SUCCESS");
   }
   else
   {
     APP_DBG("Error in persist complete callback %x",status);
   }
    /* STEP3 - Activate back the persistent notofacation */
   
    /* Register Persistent data change notification */
    ZbPersistNotifyRegister(zigbee_app_info.zb,APP_ZIGBEE_persist_notify_cb,NULL);
                                                         
    /* Call the callback once here to save persistence data */
    APP_ZIGBEE_persist_notify_cb(zigbee_app_info.zb,NULL); 
}/* APP_ZIGBEE_PersistCompleted_callback */


/**
 * @brief  Load persitent data 
 * @param  None
 * @retval true if sucess, false if fail
 */
static bool APP_ZIGBEE_persist_load(void)
{
#ifdef CFG_NVM
    APP_DBG("Retrieving persistent data from FLASH");
    return APP_ZIGBEE_NVM_Read();
#else
    /* Check length range */
    if ((cache_persistent_data.U32_data[0] == 0) ||
        (cache_persistent_data.U32_data[0] > ST_PERSIST_MAX_ALLOC_SZ))
    {
        APP_DBG("No data or too large lenght : %d",cache_persistent_data.U32_data[0]);
        return false;
    }
    return true;
#endif /* CFG_NVM */
} /* APP_ZIGBEE_persist_load */

/**
 * @brief  Save persistent data 
 * @param  None
 * @retval true if success , false if fail
 */
 bool APP_ZIGBEE_persist_save(void)
{
    uint32_t len;

    /* Clear the RAM cache before saving */
    memset(cache_persistent_data.U8_data, 0x00, ST_PERSIST_MAX_ALLOC_SZ);

    /* Call the satck API t get current persistent data */
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

#ifdef CFG_NVM
    if(!APP_ZIGBEE_NVM_Write())
    {
      return false;
    }
    APP_DBG("APP_ZIGBEE_persist_save: Persistent data FLASHED");
#endif /* CFG_NVM */
    
    return true;
} /* APP_ZIGBEE_persist_save */

/**
 * @brief  Delete persistent data 
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_persist_delete(void)
{
  uint16_t c_int = 0; 
  bool deviceConnected = false;
  bool connectionUpdateSuccess = false;
  bool connectionUpdateNeeded = false;
  
  /* Clear RAM cache */
   memset(cache_persistent_data.U8_data, 0x00, ST_PERSIST_MAX_ALLOC_SZ);
   APP_DBG("Persistent Data RAM cache cleared");
#ifdef CFG_NVM
   /* Check if there is at least 25ms connection interval window for a FLASH erase */
   c_int = APP_BLE_GetConnectionInterval(&deviceConnected);
   if(deviceConnected){
     if( ((c_int * 10) / 8) < 25 ){
       connectionUpdateNeeded = true;
       APP_DBG("Connection interval of %dms too short for an erase.", ((c_int * 10) / 8));
       APP_DBG("Need a temporary BLE connection interval update.");
       
       /* Update BLE connection interval to 25ms */
       connectionUpdateSuccess = APP_BLE_ConnectionUpdate(25*8/10);
       if(!connectionUpdateSuccess){
         APP_DBG("Cannot update connection interval");
         return;
       }
     }
   }

   APP_ZIGBEE_NVM_Erase();
   APP_DBG("FLASH ERASED");

   if(connectionUpdateNeeded){
     /* Restore the previous BLE connection interval */
     APP_DBG("Restore previous connection interval");
     connectionUpdateSuccess = APP_BLE_ConnectionUpdate(c_int);
     if(!connectionUpdateSuccess){
       APP_DBG("Cannot update connection interval");
     }
   }
#endif /* CFG_NVM */
} /* APP_ZIGBEE_persist_delete */


#ifdef CFG_NVM
/**
 * @brief  Init the NVM 
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NVM_Init(void)
{
  int eeprom_init_status;
  
  APP_DBG("Flash starting address = %x",HW_FLASH_ADDRESS  + CFG_NVM_BASE_ADDRESS);
  eeprom_init_status = EE_Init( 0 , HW_FLASH_ADDRESS + CFG_NVM_BASE_ADDRESS );
 
  if(eeprom_init_status != EE_OK)
  {
    /* format NVM since init failed */
    eeprom_init_status= EE_Init( 1, HW_FLASH_ADDRESS + CFG_NVM_BASE_ADDRESS );
  }
  APP_DBG("EE_init status = %d",eeprom_init_status);

} /* APP_ZIGBEE_NVM_Init */

/**
*@brief  Read the persistent data from NVM
* @param  None
* @retval true if success , false if failed
*/
static bool APP_ZIGBEE_NVM_Read(void)
{
    uint16_t num_words = 0;
    bool status = true;
    int ee_status = 0;
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR); 

    /* Read the data length from cache */
    ee_status = EE_Read(0, ZIGBEE_DB_START_ADDR, &cache_persistent_data.U32_data[0]);
    if (ee_status != EE_OK)
    {
        APP_DBG("Read -> persistent data lenght not found ERASE to be done - Read Stopped");
        status = false;
    }
      /* Check length is not too big nor zero */
    else if((cache_persistent_data.U32_data[0] == 0) || 
            (cache_persistent_data.U32_data[0]> ST_PERSIST_MAX_ALLOC_SZ))
    {
            APP_DBG("No data or too large length : %d", cache_persistent_data.U32_data[0]);
            status = false;
    }
        /* Length is within range */
    else
    {
           /* Adjust the length to be U32 aligned */
            num_words = (uint16_t) (cache_persistent_data.U32_data[0]/4) ;
            if (cache_persistent_data.U32_data[0] % 4 != 0)
            {
                num_words++;
            }

            /* copy the read data from Flash to cache including length */
            for (uint16_t local_length = 1; local_length <= num_words; local_length++)
            {
                /* read data from first data in U32 unit */
                ee_status = EE_Read(0, local_length + ZIGBEE_DB_START_ADDR, &cache_persistent_data.U32_data[local_length] );
                if (ee_status != EE_OK)
                {
                    APP_DBG("Read not found leaving");
                    status = false;
                    break;
                }
            }
    }
    
    HAL_FLASH_Lock();
    if(status)
    {
        APP_DBG("READ PERSITENT DATA LEN = %d",cache_persistent_data.U32_data[0]);
    }
    return status;
} /* APP_ZIGBEE_NVM_Read */

/**
 * @brief  Write the persistent data in NVM
 * @param  None
 * @retval None
 */
static bool APP_ZIGBEE_NVM_Write(void)
{
    int ee_status = 0;
    
    uint16_t num_words;
    uint16_t local_current_size;
    

    num_words = 1U; /* 1 words for the length */
    num_words+= (uint16_t) (cache_persistent_data.U32_data[0]/4);
    
    
    /* Adjust the length to be U32 aligned */
    if (cache_persistent_data.U32_data[0] % 4 != 0)
    {
        num_words++;
    }
    
    //save data in flash
    for (local_current_size = 0; local_current_size < num_words; local_current_size++)
    {
        ee_status = EE_Write(0, (uint16_t)local_current_size + ZIGBEE_DB_START_ADDR, cache_persistent_data.U32_data[local_current_size]);
        if (ee_status != EE_OK)
        {
           if(ee_status == EE_CLEAN_NEEDED) /* Shall not be there if CFG_EE_AUTO_CLEAN = 1*/
           {
              APP_DBG("CLEAN NEEDED, CLEANING");
              EE_Clean(0,0);
           }
           else
           {
              /* Failed to write , an Erase shall be done */
              APP_DBG("APP_ZIGBEE_NVM_Write failed @ %d status %d", local_current_size,ee_status);
              break;
           }
        }
    }
    
 
    if(ee_status != EE_OK)
    {
       APP_DBG("WRITE STOPPED, need a FLASH ERASE");
       return false;
    }
    
    APP_DBG("WRITTEN PERSISTENT DATA LEN = %d",cache_persistent_data.U32_data[0]);
    return true;

} /* APP_ZIGBEE_NVM_Write */

/**
 * @brief  Erase the NVM
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NVM_Erase(void)
{
   EE_Init(1, HW_FLASH_ADDRESS + CFG_NVM_BASE_ADDRESS); /* Erase Flash */
} /* APP_ZIGBEE_NVM_Erase */

#endif /* CFG_NVM */


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

    ZbStackType = p_wireless_info->StackType; /* Memorize ZB stack type */
    switch (p_wireless_info->StackType) {
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_STATIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_FFD_STATIC");
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC:
      APP_DBG("FW Type : STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC");
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

  APP_DBG("==> SENDING TOGGLE TO GROUP 0x0001");
  if (ZbZclOnOffClientToggleReq(zigbee_app_info.onoff_client_1, &dst, NULL, NULL) != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)");
  }
}

/**
 * @brief SW2 button pushed Clean NVM
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process()
{
  APP_DBG("SW2 PUSHED : Clearing NVM");
//  HW_TS_Create(CFG_TIM_WAIT_BEOFRE_NVM_CLEAR, &TS_ID1, hw_ts_SingleShot, APP_ZIGBEE_persist_delete);
//  HW_TS_Start(TS_ID1, 1000);
  APP_ZIGBEE_persist_delete();
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
void APP_ZIGBEE_ProcessNotifyM0ToM4(void)
{
    if (CptReceiveNotifyFromM0 != 0) {
        /* If CptReceiveNotifyFromM0 is > 1. it means that we did not serve all the events from the radio */
        if (CptReceiveNotifyFromM0 > 1U) {
            APP_ZIGBEE_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
        }
        else {
            Zigbee_CallBackProcessing();
        }
        /* Reset counter */
        CptReceiveNotifyFromM0 = 0;
    }
}

/**
 * @brief Process the requests coming from the M0.
 * @param
 * @return
 */
void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
    if (CptReceiveRequestFromM0 != 0) {
        Zigbee_M0RequestProcessing();
        CptReceiveRequestFromM0 = 0;
    }
}


/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

WaitedSemStatus_t FD_WaitForSemAvailable(WaitedSemId_t WaitedSemId)
{
  if(WaitedSemId == WAIT_FOR_SEM_BLOCK_FLASH_REQ_BY_CPU1)
  {
    LL_HSEM_ClearFlag_C1ICR(HSEM, __HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID)); /* There is a bug in __HAL_HSEM_CLEAR_FLAG() */
    if(LL_HSEM_GetStatus(HSEM, CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID))
    {
      APP_DBG_MSG("\r\n\rWAIT UNTILL CPU1 ALLOWS FLASH OPERATION\n");

      HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID));
      UTIL_SEQ_WaitEvt( 1<< CFG_IDLEEVT_FLASH_OPER_ALLOWED);
      HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID));
    }
  }

  if(WaitedSemId == WAIT_FOR_SEM_BLOCK_FLASH_REQ_BY_CPU2)
  {
    LL_HSEM_ClearFlag_C1ICR(HSEM, __HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID)); /* There is a bug in __HAL_HSEM_CLEAR_FLAG() */
    if(LL_HSEM_GetStatus(HSEM, CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID))
    {
      APP_DBG_MSG("\r\n\rWAIT UNTILL CPU2 ALLOWS FLASH OPERATION\n");

      HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID));
      UTIL_SEQ_WaitEvt( 1<< CFG_IDLEEVT_FLASH_OPER_ALLOWED);
      HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID));
    }
  }

  return WAITED_SEM_FREE;
}

void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  if(SemMask == __HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID))
  {
    UTIL_SEQ_SetEvt(1<< CFG_IDLEEVT_FLASH_OPER_ALLOWED);
  }

  if(SemMask == __HAL_HSEM_SEMID_TO_MASK(CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID))
  {
    UTIL_SEQ_SetEvt(1<< CFG_IDLEEVT_FLASH_OPER_ALLOWED);
  }
}

/* USER CODE END FD_WRAP_FUNCTIONS */

void APP_ZIGBEE_WaitUntilNext_802154_Evt_cb(void){
  /** Set the EVENT_DYN_MODE_WAIT_NEXT_802154_EVT event. 
   *  Pending FLASH operation is going to be performed. 
   */
  UTIL_SEQ_SetEvt(EVENT_DYN_MODE_WAIT_NEXT_802154_EVT);
}

WaitedSemStatus_t DYN_CONCURRENT_MODE_WaitUntilNext_802154_Evt(uint32_t relative_time){  
  APP_DBG("Next BLE event in %dus. Waiting next 802.15.4 event.", relative_time);
  
  /* Activate the notification for the next 802.15.4 event */
  SHCI_C2_CONCURRENT_EnableNext_802154_EvtNotification();
  
  /* Wait on EVENT_DYN_MODE_WAIT_NEXT_802154_EVT event */
  UTIL_SEQ_WaitEvt(EVENT_DYN_MODE_WAIT_NEXT_802154_EVT);
  
  return WAITED_SEM_FREE;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
