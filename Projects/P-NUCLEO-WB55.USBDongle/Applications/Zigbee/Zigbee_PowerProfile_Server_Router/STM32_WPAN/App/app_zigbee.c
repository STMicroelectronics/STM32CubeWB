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
#include "zcl/general/zcl.power.profile.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT                                17
#define CHANNEL                                     19
#define HW_TS_SERVER_1S_NB_TICKS                    (1*1000*1000/CFG_TS_TICK_VAL) /* 1s */

/* Power Profile specific defines -------------------------------------------------*/
#define TOTAL_PROFILENUM                            0x01
#define MULTIPLE_SCHEDULING                         0 /* False */
#define ENERGY_FORMATING                            0x9B /* Suppress leading zero, 3 digits before and after decimal point */ 
#define ENERGY_REMOTE                               0 /* False */
#define SCHEDULE_MODE                               ZCL_PWR_PROF_SCHED_MODE_CHEAPEST

/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_App_Init(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_PowerProfile_Processing(void);

static void APP_ZIGBEE_PowerProfile_Server_Init(void);
static void APP_ZIGBEE_PowerProfile_Program_Step(void);
static void APP_ZIGBEE_PowerProfile_GetPrice_Step(void);
static void APP_ZIGBEE_PowerProfile_Start_Step(void);
static void APP_ZIGBEE_PowerProfile_Running_Loop(void);
static void APP_ZIGBEE_PowerProfile_Send_StateNotification(struct ZbApsAddrT* dst, struct ZbZclPowerProfSvrStateRsp* notify);
static void APP_ZIGBEE_PowerProfile_Notification_cb(struct ZbZclCommandRspT *rsp, void *arg);
static void APP_ZIGBEE_PowerProfile_Send_ProfileNotification(struct ZbApsAddrT* dst, struct ZbZclPowerProfSvrProfileRsp* notify);
static void APP_ZIGBEE_PowerProfile_GetPrice_cb(struct ZbZclCommandRspT *rsp, void *arg);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);
static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);

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
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;

  struct ZbZclClusterT *powerprofile_server_1;
};
static struct zigbee_app_info zigbee_app_info;

static uint8_t TS_ID1, TS_ID2;
static bool pending_operation = false;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  
  /* Tasks associated with Power Profile steps */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_1, UTIL_SEQ_RFU, APP_ZIGBEE_PowerProfile_Program_Step);
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_2, UTIL_SEQ_RFU, APP_ZIGBEE_PowerProfile_GetPrice_Step);
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_3, UTIL_SEQ_RFU, APP_ZIGBEE_PowerProfile_Start_Step);
  
  /* Timer associated with Power Profile steps */
  HW_TS_Create(CFG_TIM_ZIGBEE_APP_POWER_PROFILE_STEP_4, &TS_ID1, hw_ts_SingleShot, APP_ZIGBEE_PowerProfile_Running_Loop);
  HW_TS_Create(CFG_TIM_ZIGBEE_APP_ZIGBEE_PowerProfile_Processing, &TS_ID2, hw_ts_SingleShot, APP_ZIGBEE_PowerProfile_Processing);
 
  APP_ZIGBEE_PowerProfile_Server_Init();
} /* APP_ZIGBEE_App_Init */

/**
 * @brief  Power Profile server initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Server_Init(void){
  enum ZclStatusCodeT status;
  
  /* ZCL_POWER_PROF_SVR_ATTR_TOTAL_PROFILENUM attribute init */
  APP_DBG("[POWER PROFILE] Writing Total Profile Number attribute.");
  status = ZbZclAttrIntegerWrite(zigbee_app_info.powerprofile_server_1, ZCL_POWER_PROF_SVR_ATTR_TOTAL_PROFILENUM, TOTAL_PROFILENUM);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error writing local attribute.");
    assert(0);
  }
  
  /* ZCL_POWER_PROF_SVR_ATTR_MULTIPLE_SCHED attribute init */
  APP_DBG("[POWER PROFILE] Writing Multiple Scheduling attribute.");
  status = ZbZclAttrIntegerWrite(zigbee_app_info.powerprofile_server_1, ZCL_POWER_PROF_SVR_ATTR_MULTIPLE_SCHED, MULTIPLE_SCHEDULING);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error writing local attribute.");
    assert(0);
  }
  
  /* ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT attribute init */
  APP_DBG("[POWER PROFILE] Writing Energy Formatting attribute.");
  status = ZbZclAttrIntegerWrite(zigbee_app_info.powerprofile_server_1, ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT, ENERGY_FORMATING);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error writing local attribute.");
    assert(0);
  }
  
  /* ZCL_POWER_PROF_SVR_ATTR_ENERGY_REMOTE attribute init */
  APP_DBG("[POWER PROFILE] Writing Energy Remote attribute.");
  status = ZbZclAttrIntegerWrite(zigbee_app_info.powerprofile_server_1, ZCL_POWER_PROF_SVR_ATTR_ENERGY_REMOTE, ENERGY_REMOTE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error writing local attribute.");
    assert(0);
  }
  
  /* ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE attribute init */
  APP_DBG("[POWER PROFILE] Writing Schedule Mode attribute.");
  status = ZbZclAttrIntegerWrite(zigbee_app_info.powerprofile_server_1, ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE, SCHEDULE_MODE);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error writing local attribute.");
    assert(0);
  }
  
  APP_DBG("[POWER PROFILE] Power Profile server init done!\n");  
} /* APP_ZIGBEE_PowerProfile_Server_Init */

/**
 * @brief  Power Profile Program Step (Step 1)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Program_Step(void){
  struct ZbApsAddrT dst;
  struct ZbZclPowerProfSvrStateRsp state_notify;
  struct ZbZclPowerProfSvrProfileRsp profile_notify;
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Point 1: Power Profile State Notification (Idle, no remote control) is sent by Server */ 
  memset(&state_notify, 0, sizeof(state_notify));
  state_notify.profile_count = TOTAL_PROFILENUM;
  state_notify.record_list[0].remote_control = ENERGY_REMOTE; /* no remote control */
  state_notify.record_list[0].state = ZCL_PWR_PROF_STATE_PROFILE_IDLE;
  
  APP_DBG("[POWER PROFILE] Sending Power Profile state notification: Idle.");    
  APP_ZIGBEE_PowerProfile_Send_StateNotification(&dst, &state_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  /* Point 2: Power Profile Notification is sent from Server to Client */
  memset(&profile_notify, 0, sizeof(profile_notify));
  profile_notify.profile_id = 0x01;
  profile_notify.num_transferred_phases = 0x01;  /* single energy phase */
  profile_notify.phase_list[0].energy_phase_id = 0x01;  /* first energy phase */
  profile_notify.phase_list[0].expect_duration = 90; /* expected duration = 90 min */
  profile_notify.phase_list[0].max_activation_delay = 0xFFFF;  /* first energy phase */
  
  APP_DBG("[POWER PROFILE] Sending Power Profile profile notification.");   
  APP_ZIGBEE_PowerProfile_Send_ProfileNotification(&dst, &profile_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  /* Point 3: Power Profile State Notification (Programmed, no remote control) is sent by Server */
  state_notify.record_list[0].state = ZCL_PWR_PROF_STATE_PROFILE_PROGRAMMED;

  APP_DBG("[POWER PROFILE] Sending Power Profile state notification: Programmed.");
  APP_ZIGBEE_PowerProfile_Send_StateNotification(&dst, &state_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_2, CFG_SCH_PRIO_0);
}

/**
 * @brief  Power Profile GetPrice Step (Step 2)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_GetPrice_Step(void){
  enum ZclStatusCodeT status;
  struct ZbApsAddrT dst;
  struct ZbZclPowerProfCliProfileReq req;
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Point 4: Server ask for the price */
  memset(&req, 0, sizeof(req));
  req.profile_id = 0x01;
  
  APP_DBG("[POWER PROFILE] Sending Power Profile GetPrice request.\n");
  status = ZbZclPowerProfServerGetPriceReq(zigbee_app_info.powerprofile_server_1, &dst, &req, APP_ZIGBEE_PowerProfile_GetPrice_cb, NULL);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error sending Power Profile GetPrice request.");
    assert(0);
  }
}

/**
 * @brief  Power Profile Start Step (Step 3)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Start_Step(void){
  struct ZbApsAddrT dst;
  struct ZbZclPowerProfSvrStateRsp state_notify;
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Point 5: Power Profile State Notification (Waiting to start, no remote control) is sent by Server */ 
  memset(&state_notify, 0, sizeof(state_notify));
  state_notify.profile_count = TOTAL_PROFILENUM;
  state_notify.record_list[0].remote_control = ENERGY_REMOTE; /* no remote control */
  state_notify.record_list[0].state = ZCL_PWR_PROF_STATE_PHASE_WAITING_START;
  
  APP_DBG("[POWER PROFILE] Sending Power Profile state notification: Waiting to start.");    
  APP_ZIGBEE_PowerProfile_Send_StateNotification(&dst, &state_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  /* Point 6: Wait 2 seconds */
  HW_TS_Start(TS_ID1, 2*HW_TS_SERVER_1S_NB_TICKS);
}

/**
 * @brief  Power Profile Running Loop (Step 4)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Running_Loop(void){
  struct ZbApsAddrT dst;
  struct ZbZclPowerProfSvrStateRsp state_notify;
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Point 7: Power Profile State Notification (Running, no remote control) is sent by Server */ 
  state_notify.profile_count = TOTAL_PROFILENUM;
  state_notify.record_list[0].remote_control = ENERGY_REMOTE; /* no remote control */
  state_notify.record_list[0].state = ZCL_PWR_PROF_STATE_PHASE_RUNNING;
  
  APP_DBG("[POWER PROFILE] Sending Power Profile state notification: Running.");    
  APP_ZIGBEE_PowerProfile_Send_StateNotification(&dst, &state_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  HW_TS_Start(TS_ID2, HW_TS_SERVER_1S_NB_TICKS/100);
}

/**
 * @brief  Power Profile Processing (Step 5)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Processing(void){
  struct ZbApsAddrT dst;
  struct ZbZclPowerProfSvrStateRsp state_notify;
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Point 8: Wait 10 seconds while LED_GREEN is blinking */
  for(unsigned int cpt=0 ; cpt<10 ; cpt++){
    BSP_LED_On(LED_GREEN);
    HAL_Delay(500);
    BSP_LED_Off(LED_GREEN);
    HAL_Delay(500);
  }
  
  /* Point 9: Power Profile State Notification (Ended, no remote control) is sent by Server */ 
  memset(&state_notify, 0, sizeof(state_notify));
  state_notify.profile_count = TOTAL_PROFILENUM;
  state_notify.record_list[0].remote_control = ENERGY_REMOTE; /* no remote control */
  state_notify.record_list[0].state = ZCL_PWR_PROF_STATE_PROFILE_ENDED;
  
  APP_DBG("[POWER PROFILE] Sending Power Profile state notification: Ended.\n");    
  APP_ZIGBEE_PowerProfile_Send_StateNotification(&dst, &state_notify);
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  UTIL_SEQ_ClrEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
  
  HW_TS_Stop(TS_ID1);
  HW_TS_Stop(TS_ID2);
  pending_operation = false;
}

/**
 * @brief  Power Profile State notification sending wrapper
 * @param  dst: APS address
 * @param  notify: Power Profile State notification
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Send_StateNotification(struct ZbApsAddrT* dst, struct ZbZclPowerProfSvrStateRsp* notify){
  enum ZclStatusCodeT status;
  
  status = ZbZclPowerProfServerStateNotify(zigbee_app_info.powerprofile_server_1, dst, notify, &APP_ZIGBEE_PowerProfile_Notification_cb, NULL);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error sending Power Profile state notification.");
    assert(0);
  }
}

/**
 * @brief  Power Profile Profile notification sending wrapper
 * @param  dst: APS address
 * @param  notify: Power Profile Profile notification
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Send_ProfileNotification(struct ZbApsAddrT* dst, struct ZbZclPowerProfSvrProfileRsp* notify){
  enum ZclStatusCodeT status;
  
  status = ZbZclPowerProfServerProfileNotify(zigbee_app_info.powerprofile_server_1, dst, notify, &APP_ZIGBEE_PowerProfile_Notification_cb, NULL);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error sending Power Profile profile notification.");
    assert(0);
  }
}

/**
 * @brief  Power Profile State and Profile notification response callback
 * @param  rsp: Sender status response
 * @param  arg: Passed argument
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_Notification_cb(struct ZbZclCommandRspT *rsp, void *arg){
  UNUSED(arg);
  
  /* Unlock the waiting event */
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_APP_POWER_PROFILE_NOTIFY_RSP_RECEIVED);
}

/**
 * @brief  Power Profile GetPrice callback
 * @param  rsp: ZCL generic response
 * @param  arg: passed argument
 * @retval None
 */
static void APP_ZIGBEE_PowerProfile_GetPrice_cb(struct ZbZclCommandRspT *rsp, void *arg){
  struct ZbZclPowerProfCliPriceRsp price_rsp;
  unsigned int len = 0;
  
  UNUSED(arg);
  
  /* Checking response consistency */
  assert(rsp != NULL && rsp->payload != NULL);
  
  if(rsp->hdr.frameCtrl.frameType != ZCL_FRAMETYPE_CLUSTER){ 
    APP_DBG("[POWER PROFILE] Error Power Profile GetPrice response: wrong frame type.");
    assert(0);
  }
  
  if(rsp->status != ZCL_STATUS_SUCCESS){
    APP_DBG("[POWER PROFILE] Error: cmd not successfully received. 0x%02x.\n", rsp->status);
    assert(0);
  }
  
  if(rsp->clusterId != ZCL_CLUSTER_POWER_PROFILE){
    APP_DBG("[POWER PROFILE] Error Power Profile GetPrice response: wrong cluster.");
    assert(0);
  }
  
  if(rsp->hdr.cmdId != ZCL_PWR_PROF_CLI_PRICE_RSP){
    APP_DBG("[POWER PROFILE] Error Power Profile GetPrice response: wrong command.");
    assert(0);
  }
  
  APP_DBG("[POWER PROFILE] Power Profile GetPrice response received.");
  
  /* Parse the payload */
  price_rsp.profile_id = rsp->payload[len++];
  price_rsp.currency = pletoh16(&rsp->payload[len]);
  len += 2;
  price_rsp.price = pletoh32(&rsp->payload[len]);
  len += 4;
  price_rsp.trailing_digit = rsp->payload[len++];
  
  APP_DBG("[POWER PROFILE] Power Profile ID %d.", price_rsp.profile_id);
  APP_DBG("[POWER PROFILE] The price is %d.\n", price_rsp.price);
  
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_3, CFG_SCH_PRIO_0);
}

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
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_WHITE_GOODS;

  /* Endpoint: SW1_ENDPOINT */
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  /* Power Profile Server */
  zigbee_app_info.powerprofile_server_1 = ZbZclPowerProfServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, NULL, NULL);
  assert(zigbee_app_info.powerprofile_server_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.powerprofile_server_1); 
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

    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    config.startupControl = ZbStartTypeJoin;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);   
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/

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
      APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
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
    
    /* Starting application init task */
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
  }
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
    APP_DBG("Power profile Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

/**
 * @brief  Button 1 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process(void)
{
  if(!pending_operation) {
    pending_operation = true;
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_POWER_PROFILE_STEP_1, CFG_SCH_PRIO_0);
  }
} /* APP_ZIGBEE_SW1_Process */

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

