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
#include "zcl/general/zcl.time.h"
#include "zcl/general/zcl.alarm.h"
#include "zcl/general/zcl.doorlock.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT                                17
#define CHANNEL                                     19
#define HW_TS_SERVER_1S_NB_TICKS                    (1*1000*1000/CFG_TS_TICK_VAL)

/* Alarm specific defines -------------------------------------------------*/
#define ZCL_ALARM_LOG_ENTRY_NB                      4U   

/* DoorLock specific defines -------------------------------------------------*/
#define MAX_PIN_NB_USERS                            5
#define DOORLOCK_PASSAGE_MODE_DURATION              10*HW_TS_SERVER_1S_NB_TICKS
#define DOORLOCK_AUTO_RELOCK_TIME                   5

#define DOORLOCK_DOORSTATE_OPEN                     0x00
#define DOORLOCK_DOORSTATE_CLOSE                    0x01
#define DOORLOCK_DOORSTATE_ERROR_JAMMED             0x02
#define DOORLOCK_DOORSTATE_ERROR_FORCED_OPEN        0x03
#define DOORLOCK_DOORSTATE_ERROR_UNSPECIFIED        0x04
#define DOORLOCK_DOORSTATE_ERROR_UNDEFINED          0xFF

#define DOORLOCK_NORMAL_MODE                        0x00
#define DOORLOCK_VACATION_MODE                      0x01
#define DOORLOCK_PRIVACY_MODE                       0x02
#define DOORLOCK_NO_RF_LOCK_UNLOCK_MODE             0x03
#define DOORLOCK_PASSAGE_MODE                       0x04
#define DOORLOCK_NORMAL_MODE_SUPPORTED              1 << DOORLOCK_NORMAL_MODE
#define DOORLOCK_VACATION_MODE_SUPPORTED            1 << DOORLOCK_VACATION_MODE
#define DOORLOCK_PRIVACY_MODE_SUPPORTED             1 << DOORLOCK_PRIVACY_MODE
#define DOORLOCK_NO_RF_LOCK_UNLOCK_MODE_SUPPORTED   1 << DOORLOCK_NO_RF_LOCK_UNLOCK_MODE
#define DOORLOCK_PASSAGE_MODE_SUPPORTED             1 << DOORLOCK_PASSAGE_MODE

/* Deadbolt Jammed alarm */
#define DOORLOCK_ALARM_DEADBOLT_JAMMED              0x00
/* Lock Reset to Factory Defaults alarm */
#define DOORLOCK_ALARM_RESET                        0x01
/* RF Module Power Cycled alarm */
#define DOORLOCK_ALARM_RF_MODULE                    0x03
/* Tamper Alarm - wrong code entry limit */
#define DOORLOCK_ALARM_WRONG_CODE_LIMIT             0x04
/* Tamper Alarm - front escutcheon removed from main */
#define DOORLOCK_ALARM_FRONT                        0x05
/* Forced Door Open under Door Locked Condition alarm */
#define DOORLOCK_ALARM_FORCED_DOOR                  0x06

struct doorlock_info_t {
  uint8_t current_nb_users;
  struct ZbZclDoorLockSetPinReqT user_tab[MAX_PIN_NB_USERS];
};

/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_App_Init(void);

static void APP_ZIGBEE_BINDING_TABLE_insert(uint16_t ntw_addr, uint8_t endpoint, uint16_t cluster_id);

/* Time */
static uint32_t APP_ZIGBEE_Time_Server_GetTime_cb(struct ZbZclClusterT *clusterPtr, void *arg);
static void APP_ZIGBEE_Time_Server_SetTime_cb(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg);

/* Door Lock */
static void APP_ZIGBEE_DoorLock_Server_Init(void);
static void APP_ZIGBEE_DoorLock_Server_PassageMode(void);
static void APP_ZIGBEE_DoorLock_Server_PassageMode_exec(void);
static void APP_ZIGBEE_DoorLock_Server_PassageMode_Ended(void);
static void APP_ZIGBEE_DoorLock_Server_PassageMode_Ended_exec(void);
static void APP_ZIGBEE_DoorLock_Server_AutoRelockTime(void);
static void APP_ZIGBEE_DoorLock_Server_AutoRelockTime_exec(void);
static int8_t APP_ZIGBEE_DoorLock_Server_search_userID(struct doorlock_info_t* info, uint16_t user_id);
static int8_t APP_ZIGBEE_DoorLock_Server_search_PIN(struct doorlock_info_t* info, uint8_t pin[], uint8_t pin_len);
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Attr_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *cb);
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_AttrNotify_cb(struct ZbZclClusterT *clusterPtr, struct ZbApsAddrT *src, 
                                                                   uint16_t attributeId, const uint8_t *inputData, unsigned int inputMaxLen, 
                                                                   void *attrData, ZclWriteModeT mode, void *app_cb_arg);
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Lock_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockLockDoorReqT *cmd_req, 
                                                              struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Unlock_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockUnlockDoorReqT *cmd_req, 
                                                                struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_SetPin_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetPinReqT *cmd_req, 
                                                                struct ZbZclAddrInfoT *srcInfo, void *arg);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);

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

/* Adding required optional attributes */
static const struct ZbZclAttrT optionalAttrList[] = {
    {
        ZCL_DRLK_ATTR_DOORSTATE, ZCL_DATATYPE_UNSIGNED_8BIT,
        ZCL_ATTR_FLAG_CB_NOTIFY, 0, APP_ZIGBEE_DoorLock_Server_Attr_cb, {0, 0}, {0, 0}
    },
};

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;

  struct ZbZclClusterT *doorlock_server;
  struct ZbZclClusterT *time_server;
  struct ZbZclClusterT *alarm_server;
};
static struct zigbee_app_info zigbee_app_info;

/* Time server custom callbacks */
struct ZbZclTimeServerCallbacks ZbZclTimeServerCallbacks = {
  .get_time = APP_ZIGBEE_Time_Server_GetTime_cb,
  .set_time = APP_ZIGBEE_Time_Server_SetTime_cb,
};

/* DoorLock server custom callbacks */
static struct ZbZclDoorLockServerCallbacksT DoorLockServerCallbacks = {
  .lock = APP_ZIGBEE_DoorLock_Server_Lock_cb,
  .unlock = APP_ZIGBEE_DoorLock_Server_Unlock_cb,
  .set_pin = APP_ZIGBEE_DoorLock_Server_SetPin_cb,
};

static struct doorlock_info_t doorlock_info;
static uint8_t TS_ID1, TS_ID2; 

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Adding entry in binding table
 * @param  ntw_addr: Network address
 * @param  endpoint: Endpoint
 * @param  cluster_id: ZCL cluster ID
 * @retval None
 */
static void APP_ZIGBEE_BINDING_TABLE_insert(uint16_t ntw_addr, uint8_t endpoint, uint16_t cluster_id){
  enum ZbStatusCodeT status;
  struct ZbApsmeBindT current_bind;
  struct ZbApsmeBindReqT bindreq;
  struct ZbApsmeBindConfT bindconf;
  uint32_t bind_index = 0;
  bool insert_new = false;
  
  /* check if this ntw address with this particular endpoint is in the BT */
  
  /* Searching over the BT */
  for(bind_index=0;;bind_index++){
    
    /* Get the next entry in the binding table. */
    status = ZbApsGetIndex(zigbee_app_info.zb, ZB_APS_IB_ID_BINDING_TABLE, &current_bind, sizeof(current_bind), bind_index);
    if (status != ZB_APS_STATUS_SUCCESS) {
         /* End of table, no matching found */
         insert_new = true;
         break;
    }
    
    if(current_bind.srcExtAddr != 0){
      if((current_bind.clusterId == cluster_id) 
         && (current_bind.dst.nwkAddr == ntw_addr)
         && (current_bind.dst.endpoint == endpoint)){
           /* matching found, exit */
           break;
       }
    }
  }
  
  /* if new, insert it */
  if(insert_new){
    /* Binding request */
    memset(&bindreq, 0, sizeof(bindreq));
    memset(&bindconf, 0, sizeof(bindconf));
    bindreq.srcExtAddr = ZbExtendedAddress(zigbee_app_info.zb);
    bindreq.srcEndpt = SW1_ENDPOINT;
    bindreq.clusterId = cluster_id;
    bindreq.dst.mode = ZB_APSDE_ADDRMODE_EXT;
    bindreq.dst.endpoint = endpoint;
    bindreq.dst.extAddr = ZbNwkAddrLookupExt(zigbee_app_info.zb, ntw_addr);
    
    ZbApsmeBindReq(zigbee_app_info.zb,&bindreq,&bindconf);
      
    if(bindconf.status != ZB_WPAN_STATUS_SUCCESS){
        APP_DBG("[DOORLOCK] Local Bind failed on DoorLock Cluster.\n");
    }
    APP_DBG("[DOORLOCK] Binding the client with ext_addr = %#08llx dest endpoint: %d.\n", bindreq.dst.extAddr, bindreq.dst.endpoint);
  }
}

/**
 * @brief  Door Lock server attribute callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  cb: Callback info
 * @param  cluster_id: ZCL cluster ID
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Attr_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *cb){
  if (cb->type == ZCL_ATTR_CB_TYPE_NOTIFY) {
    return APP_ZIGBEE_DoorLock_Server_AttrNotify_cb(clusterPtr, (struct ZbApsAddrT *)cb->src, cb->info->attributeId, 
                                                    cb->zcl_data, cb->zcl_len, cb->attr_data, cb->write_mode, 
                                                    cb->app_cb_arg);
  } else {
    return ZCL_STATUS_FAILURE;
  }
}

/**
 * @brief  Door Lock server attribute notify callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  src: Source of the command, if not locally generated. May be NULL
 * @param  attributeId: ZCL attribute ID
 * @param  inputData: Incoming or outgoing ZCL attribute payload data
 * @param  inputMaxLen: Incoming or outgoing ZCL attribute payload data length
 * @param  attrData: Local attribute storage (if application is letting the cluster maintain the attribute data)
 * @param  mode: ZCL write mode
 * @param  app_cb_arg: Application's defined callback argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_AttrNotify_cb(struct ZbZclClusterT *clusterPtr, struct ZbApsAddrT *src, 
                                                                   uint16_t attributeId, const uint8_t *inputData, unsigned int inputMaxLen, 
                                                                   void *attrData, ZclWriteModeT mode, void *app_cb_arg){
  enum ZclStatusCodeT status;                                                                   
  uint16_t alarmMask;                                                                   
  uint8_t currentDoorState;
  
  if(clusterPtr != zigbee_app_info.doorlock_server){
    return ZCL_STATUS_FAILURE;
  }
  
  switch(attributeId){
    case ZCL_DRLK_ATTR_DOORSTATE:  
      
      /* getting the door state */
      currentDoorState = (uint8_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_DOORSTATE, NULL, &status);
      if (status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error reading local attribute.");
        return ZCL_STATUS_FAILURE;
      }
      
      APP_DBG("[DOORLOCK] Door state changed to 0x%02x.", currentDoorState);
      
      /* getting the alarm mask */
      alarmMask = (uint16_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_ALARM_MASK, NULL, &status);
      if (status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error reading local attribute.");
        return ZCL_STATUS_FAILURE;
      }
      
      /* check the door state */
      if((currentDoorState == DOORLOCK_DOORSTATE_ERROR_FORCED_OPEN)
         && (1<<DOORLOCK_ALARM_FORCED_DOOR & alarmMask)){
        APP_DBG("[DOORLOCK] Problematic value detected for the door state.");
        ZbZclClusterSendAlarm(zigbee_app_info.doorlock_server, ZbZclClusterGetEndpoint(zigbee_app_info.doorlock_server), currentDoorState);
        APP_DBG("[ALARM] Alarm sent for DoorLock cluster clients that are registered in the BT.");
      }
      
      /* setting door state to closed */
      status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_DOORSTATE, DOORLOCK_DOORSTATE_CLOSE);
      if(status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error writing local attribute.");
        assert(0);
      }
      return ZCL_STATUS_SUCCESS;
      
    default:
      return ZCL_STATUS_FAILURE;
  }
}

/**
 * @brief  DoorLock server lock command callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  cmd_req: ZCL DoorLock cluster Lock Door request
 * @param  srcInfo: Src address information
 * @param  arg: passed arguments
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Lock_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockLockDoorReqT *cmd_req, 
                                                              struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  enum ZclStatusCodeT status;
  struct ZbZclDoorLockLockDoorRspT rsp = {.status = ZCL_DRLK_STATUS_FAIL};
  struct doorlock_info_t* info = (struct doorlock_info_t*) arg;  
  long long res;
  uint8_t currentMode = 0;
  int8_t user_pos = 0;
  
  /* Get the lock operating mode */
  currentMode = (uint8_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, NULL, &status);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reading local attribute.");
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  APP_DBG("Lock requested.");
  
  /* Read the lock state */
  res = ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, NULL, &status);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reqding local attribute: cannot read lock state.\n");
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  if(res == ZCL_DRLK_LOCKSTATE_LOCKED){
    /* Lock already locked */
    APP_DBG("Already locked!\n");
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Passage mode behaviour */
  if(currentMode == DOORLOCK_PASSAGE_MODE){
    APP_DBG("Passage mode is activated.");
    
    /* Lock is unlocked -> locked it  */
    status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_LOCKED);
    if(status != ZCL_STATUS_SUCCESS){
      APP_DBG("Error writing local attribute: cannot set lock state to 'locked'.\n");
      (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
      return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
    }
        
    APP_DBG("Door locked!");
    rsp.status = ZCL_DRLK_STATUS_SUCCESS;
    
    BSP_LED_On(LED_GREEN);
    APP_DBG("LED_GREEN ON\n");
    
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Check pin length */
  if(cmd_req->pin_len>ZCL_DRLK_MAX_PIN_LEN
     || cmd_req->pin_len<ZCL_DRLK_MIN_PIN_LEN){ 
    APP_DBG("Error: Wrong pin format.\n");
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  } 
  
  /* Check if the PIN is valid */
  user_pos = APP_ZIGBEE_DoorLock_Server_search_PIN(info, cmd_req->pin, cmd_req->pin_len);
  if(user_pos < 0){
    APP_DBG("Error: Pin doesn't match.\n");
   (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
   return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  APP_DBG("Request from user %d.", info->user_tab[user_pos].user_id);
  
  /* Lock is unlocked -> locked it  */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_LOCKED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set lock state to 'unlocked'.\n");
    (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
      
  APP_DBG("Door locked!");
  rsp.status = ZCL_DRLK_STATUS_SUCCESS;
  
  BSP_LED_On(LED_GREEN);
  APP_DBG("LED_GREEN ON\n");
  
  /* Stop the Auto Relock Time associated timer */
  HW_TS_Stop(TS_ID2);
  
  (void) ZbZclDoorLockServerSendLockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
} /* APP_ZIGBEE_DoorLock_Server_Lock_cb */

/**
 * @brief  DoorLock server lock command callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  cmd_req: ZCL DoorLock cluster Unlock Door request
 * @param  srcInfo: Src address information
 * @param  arg: passed arguments
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_Unlock_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockUnlockDoorReqT *cmd_req, 
                                                                struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  enum ZclStatusCodeT status;
  struct ZbZclDoorLockUnlockDoorRspT rsp = {.status = ZCL_DRLK_STATUS_FAIL};
  struct doorlock_info_t* info = (struct doorlock_info_t*) arg;  
  long long res;
  uint32_t autoRelockTime = 0;
  uint16_t currentMode = 0;
  int8_t user_pos = 0;
  
  /* Get the lock operating mode */
  currentMode = (uint8_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, NULL, &status);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reading local attribute.");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  APP_DBG("Unlock requested.");
  
  /* Read the lock state */
  res = ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, NULL, &status);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reqding local attribute: cannot reqd lock state.\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  if(res == ZCL_DRLK_LOCKSTATE_UNLOCKED){
    /* Lock already unlocked */
    APP_DBG("Already unlocked!\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Passage mode behaviour */
  if(currentMode == DOORLOCK_PASSAGE_MODE){
    APP_DBG("Passage mode is activated.");
    
  /* Lock is locked -> unlocked it  */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_UNLOCKED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set lock state to 'unlocked'.\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
    
    APP_DBG("Door unlocked!");
    rsp.status = ZCL_DRLK_STATUS_SUCCESS;
    
    BSP_LED_Off(LED_GREEN);
    APP_DBG("LED_GREEN OFF\n");
  
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Check pin length */
  if(cmd_req->pin_len>ZCL_DRLK_MAX_PIN_LEN
     || cmd_req->pin_len<ZCL_DRLK_MIN_PIN_LEN){ 
    APP_DBG("Error: Wrong pin format.\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  } 
      
  /* Check if the PIN is valid */
  user_pos = APP_ZIGBEE_DoorLock_Server_search_PIN(info, cmd_req->pin, cmd_req->pin_len);
  if(user_pos < 0){
    APP_DBG("Error: Pin doesn't match.\n");
   (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
   return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  APP_DBG("Request from user %d.", info->user_tab[user_pos].user_id);
  
  /* Lock is locked -> unlocked it  */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_UNLOCKED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set lock state to 'unlocked'.\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Read the auto relock time value */
  autoRelockTime = ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_AUTO_RELOCK, NULL, &status);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reqding local attribute: cannot reqd lock state.\n");
    (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
    
  /* Start Auto Relock timer with auto relock time attribute value */
  HW_TS_Start(TS_ID2, autoRelockTime*HW_TS_SERVER_1S_NB_TICKS); 
    
  APP_DBG("Door unlocked for %d seconds !", autoRelockTime);
  rsp.status = ZCL_DRLK_STATUS_SUCCESS;
  
  BSP_LED_Off(LED_GREEN);
  APP_DBG("LED_GREEN OFF\n");
    
  (void) ZbZclDoorLockServerSendUnlockRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
} /* APP_ZIGBEE_DoorLock_Server_Unlock_cb */

/**
 * @brief  DoorLock server set pin command callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  cmd_req: ZCL DoorLock cluster Unlock Door request
 * @param  srcInfo: Src address information
 * @param  arg: passed arguments
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_DoorLock_Server_SetPin_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetPinReqT *cmd_req, 
                                                                struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  struct ZbZclDoorLockSetPinRspT rsp = {.status = ZCL_DRLK_STATUS_FAIL};
  struct doorlock_info_t* info = (struct doorlock_info_t*) arg;  
  APP_DBG("Set PIN requested.");
  
  /* Check if we can add a new PIN user */
  if(info->current_nb_users >= MAX_PIN_NB_USERS){
    APP_DBG("Error: PIN users limit reached.\n");
    rsp.status = ZCL_DRLK_STATUS_MEM_FULL;
    (void) ZbZclDoorLockServerSendSetPinRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  }
  
  /* Check if the user id is unique */
  if(APP_ZIGBEE_DoorLock_Server_search_userID(info, cmd_req->user_id) < 0){
    APP_DBG("Error: User with ID %d already registered.\n", cmd_req->user_id);
  }
  
  /* PIN length check */
  if(cmd_req->pin_len>ZCL_DRLK_MAX_PIN_LEN
     || cmd_req->pin_len<ZCL_DRLK_MIN_PIN_LEN){ 
    APP_DBG("Error: Wrong pin format.\n");
    (void) ZbZclDoorLockServerSendSetPinRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
    return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
  } 
  
  /* Check the unicity of the PIN */
  if(APP_ZIGBEE_DoorLock_Server_search_PIN(info, cmd_req->pin, cmd_req->pin_len) < 0){
    APP_DBG("Error: User with this PIN already registered.\n");
  }
  
  /* create the user */
  info->user_tab[info->current_nb_users].user_id = cmd_req->user_id;
  info->user_tab[info->current_nb_users].user_status = cmd_req->user_status;
  info->user_tab[info->current_nb_users].user_type = cmd_req->user_type;
  memcpy(info->user_tab[info->current_nb_users].pin, cmd_req->pin, cmd_req->pin_len);
  info->user_tab[info->current_nb_users].pin_len = cmd_req->pin_len;
  info->current_nb_users++;
  APP_DBG("PIN set for user %d!\n", cmd_req->user_id);
  
  /* Add Alarm client in the server binding table if not present for receiving Alarm notifications */
  /* DoorLock and Alarm cluster are mapped in the same endpoint */
  APP_ZIGBEE_BINDING_TABLE_insert(srcInfo->addr.nwkAddr, srcInfo->addr.endpoint, ZCL_CLUSTER_ALARMS);
  
  rsp.status = ZCL_DRLK_STATUS_SUCCESS;
  (void) ZbZclDoorLockServerSendSetPinRsp(clusterPtr, srcInfo, &rsp, NULL, NULL);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
}

/**
 * @brief  DoorLock server Passage mode task launcher wrapper
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_PassageMode_exec(void){
  /* Start the task */
  UTIL_SEQ_SetTask(1U << CFG_ZIGBEE_APP_DOORLOCK_PASSAGE_MODE, CFG_SCH_PRIO_1);
}

/**
 * @brief  DoorLock server enable Passage mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_PassageMode(void){
  enum ZclStatusCodeT status = ZCL_STATUS_FAILURE;
  uint16_t supportedModes = 0;
  uint8_t currentMode = 0;
  
  /* Get current operating mode */
  currentMode = (uint8_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, NULL, &status);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reading local attribute.");
    return;
  }
  if(currentMode == DOORLOCK_PASSAGE_MODE){
    /* Already in passage mode*/
    return;
  }
  
  /* Get available operating modes */
  supportedModes = (uint16_t)ZbZclAttrIntegerRead(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_SUPPORTED_MODES, NULL, &status);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error reading local attribute.");
    return;
  }
  
  /* Check if passage mode is available */
  if(!(supportedModes >> DOORLOCK_PASSAGE_MODE)){
    APP_DBG("Error passage mode not available.");
    return;
  }
  
  /* Activate passage mode */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, DOORLOCK_PASSAGE_MODE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set operating mode.");
  }
  
  /* Start passage mode duration timer */
  HW_TS_Start(TS_ID1, DOORLOCK_PASSAGE_MODE_DURATION); 
  
  APP_DBG("Passage mode activated.");
  BSP_LED_On(LED_RED);
  APP_DBG("LED_RED ON\n");
}

/**
 * @brief  DoorLock server Passage mode end task launcher wrapper
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_PassageMode_Ended_exec(void){
  /* Start the task */
  UTIL_SEQ_SetTask(1U << CFG_ZIGBEE_APP_DOORLOCK_PASSAGE_MODE_DURATION, CFG_SCH_PRIO_1);
}

/**
 * @brief  DoorLock server stop Passage mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_PassageMode_Ended(void){
  enum ZclStatusCodeT status = ZCL_STATUS_FAILURE;
  
  /* Operating mode set to normal */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, DOORLOCK_NORMAL_MODE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set operating mode.");
  }
  
  APP_DBG("Passage mode ended.");
  BSP_LED_Off(LED_RED);
  APP_DBG("LED_RED OFF\n");
}

/**
 * @brief  DoorLock server Auto Relock Time reached task launcher wrapper
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_AutoRelockTime_exec(void){
  /* Start the task */
  UTIL_SEQ_SetTask(1U << CFG_ZIGBEE_APP_DOORLOCK_AUTO_RELOCK_TIME, CFG_SCH_PRIO_1);
}

/**
 * @brief  DoorLock server Auto Relock Time reached
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_AutoRelockTime(void){
  enum ZclStatusCodeT status = ZCL_STATUS_FAILURE;
  
  APP_DBG("Auto Relock Time reached.");
  
  /* Lock the door */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_LOCKED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set lock state to 'unlocked'.\n");
    return;
  }
  
  APP_DBG("Door Locked.");
  BSP_LED_On(LED_GREEN);
  APP_DBG("LED_GREEN ON\n");
}

/**
 * @brief  DoorLock server search user by ID
 * @param  info: Door Lock server info
 * @param  user_id: ZCL Door Lock user ID
 * @retval User index in the table if found (-1 otherwise)
 */
static int8_t APP_ZIGBEE_DoorLock_Server_search_userID(struct doorlock_info_t* info, uint16_t user_id){
  int8_t pos = -1, cpt = 0;
  
  if(info->current_nb_users == 0){
    /* no user registered */
    return 0;
  }
  
  while(cpt <= info->current_nb_users){
    if(info->user_tab[cpt].user_id == user_id){
      /* user id found */
      pos = cpt;
      break;
    }
    cpt++;
  }
  
  return pos;
}

/**
 * @brief  DoorLock server search user by PIN
 * @param  info: Door Lock server info
 * @param  pin: PIN code to search
 * @param  pin_len: PIN code length
 * @retval User index in the table if found (-1 otherwise)
 */
static int8_t APP_ZIGBEE_DoorLock_Server_search_PIN(struct doorlock_info_t* info, uint8_t pin[], uint8_t pin_len){
  int8_t pos = -1, cpt = 0;
  
  if(info->current_nb_users == 0){
    /* no user registered */
    return 0;
  }
  
  while(cpt <= info->current_nb_users){
    if(info->user_tab[cpt].pin_len == pin_len 
       && memcmp(info->user_tab[cpt].pin, pin, pin_len) == 0){
      /* pin found */
      pos = cpt;
      break;
    }
    cpt++;
  }
  
  return pos;
}

/**
 * @brief  Time server GetTime callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  arg: Passed argument
 * @retval None
 */
static uint32_t APP_ZIGBEE_Time_Server_GetTime_cb(struct ZbZclClusterT *clusterPtr, void *arg){
  uint32_t time = HAL_GetTick()*HW_TS_SERVER_1S_NB_TICKS;
  return time;
}

/**
 * @brief  Time server SetTime callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  time_val: time value to set
 * @param  arg: Passed argument
 * @retval None
 */
static void APP_ZIGBEE_Time_Server_SetTime_cb(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg){
  
  /* Unused function */
  
  UNUSED(clusterPtr);
  UNUSED(time_val);
  UNUSED(arg);
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Timers associated with Door Lock passage operating mode */
  HW_TS_Create(1U << CFG_TIM_ZIGBEE_APP_DOORLOCK_PASSAGE_MODE_DURATION, &TS_ID1, hw_ts_SingleShot, APP_ZIGBEE_DoorLock_Server_PassageMode_Ended_exec);
  HW_TS_Create(1U << CFG_TIM_ZIGBEE_APP_DOORLOCK_AUTO_RELOCK_TIME, &TS_ID2, hw_ts_SingleShot, APP_ZIGBEE_DoorLock_Server_AutoRelockTime_exec);
  
  /* Task associated with Door Lock passage mode */
  UTIL_SEQ_RegTask(1U << CFG_ZIGBEE_APP_DOORLOCK_PASSAGE_MODE, UTIL_SEQ_RFU, APP_ZIGBEE_DoorLock_Server_PassageMode);
  UTIL_SEQ_RegTask(1U << CFG_ZIGBEE_APP_DOORLOCK_PASSAGE_MODE_DURATION, UTIL_SEQ_RFU, APP_ZIGBEE_DoorLock_Server_PassageMode_Ended);
  UTIL_SEQ_RegTask(1U << CFG_ZIGBEE_APP_DOORLOCK_AUTO_RELOCK_TIME, UTIL_SEQ_RFU, APP_ZIGBEE_DoorLock_Server_AutoRelockTime);
  
  /* Initialize Zigbee Meter Identification Server parameters */
  APP_ZIGBEE_DoorLock_Server_Init();
}

/**
 * @brief  DoorLock server initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Server_Init(void){
  enum ZclStatusCodeT status;

  /* Init the PIN user struct */
  memset(&doorlock_info, 0, sizeof(doorlock_info));
  
  /* At startup, the lock is locked */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_LOCKSTATE, ZCL_DRLK_LOCKSTATE_LOCKED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set lock state to 'locked'.");
    assert(0);
  }
  
  /* Door state is closed */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_DOORSTATE, DOORLOCK_DOORSTATE_CLOSE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute.");
    assert(0);
  }
  
  /* Setting up supported mode bitmap */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_SUPPORTED_MODES, 
                                 (uint16_t)DOORLOCK_NORMAL_MODE_SUPPORTED|DOORLOCK_PASSAGE_MODE_SUPPORTED);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set supported mode.");
    assert(0);
  }
  
  /* Setting up operating mode attribute */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_MODE, DOORLOCK_NORMAL_MODE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set operating mode.");
    assert(0);
  }
  
  /* Setting up auto relock time attribute */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_AUTO_RELOCK, DOORLOCK_AUTO_RELOCK_TIME);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set operating mode.");
    assert(0);
  }
  
  /* Setting up alarm bitmap */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_ALARM_MASK, 1<<DOORLOCK_ALARM_FORCED_DOOR);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute: cannot set operating mode.");
    assert(0);
  }
 
  BSP_LED_On(LED_GREEN);
  APP_DBG("LED_GREEN ON");
  APP_DBG("Lock state initialized to 'locked'");
  APP_DBG("DoorLock server init done !\n");
} /* APP_ZIGBEE_DoorLock_Server_Init */

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
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

  /* Register task */
  /* Create the different tasks */

  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessNotifyM0ToM4);
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessRequestM0ToM4);

  /* Task associated with network creation process */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, UTIL_SEQ_RFU, APP_ZIGBEE_NwkForm);
  
  /* Task associated with push button SW1 and SW2 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);

 /* Task associated with application init */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_START, UTIL_SEQ_RFU, APP_ZIGBEE_App_Init);
  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();
  
 

} /* APP_ZIGBEE_Init */

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

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  /* run the task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_StackLayersInit */

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  enum ZclStatusCodeT status;
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_METER_INTERFACE;

  /* Endpoint: SW1_ENDPOINT */
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);
  
  /* Time server */
  zigbee_app_info.time_server = ZbZclTimeServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &ZbZclTimeServerCallbacks, NULL);
  assert(zigbee_app_info.time_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.time_server);
  
  /* Alarm server */
  zigbee_app_info.alarm_server = ZbZclAlarmServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, ZCL_ALARM_LOG_ENTRY_NB, zigbee_app_info.time_server);
  assert(zigbee_app_info.alarm_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.alarm_server);

  /* DoorLock Server */
  zigbee_app_info.doorlock_server = ZbZclDoorLockServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &DoorLockServerCallbacks, &doorlock_info);
  assert(zigbee_app_info.doorlock_server != NULL);
  
  /* Registering attributes */
  status = ZbZclAttrAppendList(zigbee_app_info.doorlock_server, optionalAttrList, ZCL_ATTR_LIST_LEN(optionalAttrList));
  assert(status == ZCL_STATUS_SUCCESS);
  
  ZbZclClusterEndpointRegister(zigbee_app_info.doorlock_server); 
} /* APP_ZIGBEE_ConfigEndpoints */

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

    /* Set the centralized network */
    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_COORD");
    config.startupControl = ZbStartTypeForm;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);

    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /*Channel in use */

    /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
     * a blocking call on the M4. */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS) {
      zigbee_app_info.join_delay = 0U;
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      APP_DBG("Startup failed, re-attempting to form a network after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  else
  {

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
  }
  
  /* Starting application init task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_NwkForm */

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
     // print the application name
    char* __PathProject__ =(strstr(__FILE__, "Zigbee") ? strstr(__FILE__, "Zigbee") + 7 : __FILE__);
    char *del;
    if ( (strchr(__FILE__, '/')) == NULL)
        {del = strchr(__PathProject__, '\\');}
    else 
        {del = strchr(__PathProject__, '/');}
    
        int index = (int) (del - __PathProject__);
        APP_DBG("Application flashed: %*.*s",index,index,__PathProject__);
    
    //print channel
    APP_DBG("Channel used: %d", CHANNEL);
    //print Link Key
    APP_DBG("Link Key: %.16s", sec_key_ha);
    //print Link Key value hex   
    char Z09_LL_string[ZB_SEC_KEYSIZE*3+1];
    Z09_LL_string[0]=0;
    for(int str_index=0; str_index < ZB_SEC_KEYSIZE; str_index++)
      {           
        sprintf(&Z09_LL_string[str_index*3],"%02x ",sec_key_ha[str_index]);
      }
  
    APP_DBG("Link Key value: %s",Z09_LL_string);
    //print clusters allocated
    APP_DBG("Clusters allocated are:");  
    APP_DBG("Door lock Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("Alarm Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("Time Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

/**
 * @brief  Button 1 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process(void){
  enum ZclStatusCodeT status;
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  /* Door state is forced opened */
  status = ZbZclAttrIntegerWrite(zigbee_app_info.doorlock_server, ZCL_DRLK_ATTR_DOORSTATE, DOORLOCK_DOORSTATE_ERROR_FORCED_OPEN);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("Error writing local attribute.");
    assert(0);
  }
} /* APP_ZIGBEE_SW1_Process */


/**
 * @brief  Button 2 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process(void){
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  /* Start passage mode */
  APP_ZIGBEE_DoorLock_Server_PassageMode_exec();
} /* APP_ZIGBEE_SW2_Process */


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

