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
#include "zcl/general/zcl.commission.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U

/* Commissioning specific defines -------------------------------------------------*/
#define ZR_NWK_ADDR                                  0xffff /* unspecified */
#define ZR_EXT_ADDR                                  (uint64_t)0x1111111111111111
#define ZR_NB_SCANS                                  1
#define ZR_RESTART_DEVICE_DELAY                      1 /* 1 second */
#define ZB_PANID_WILDCARD                            0xffff

#define NB_NWK_CONFIG                                1
#define CHANNEL_1                                    19
#define EXT_PANID_1                                  0x1122334455667788
#define ZR_COMMISSIONING_SERVER_INDEX_NWK_1          0
#define COMMISSIONING_DEST_ENDPOINT                  ZB_ENDPOINT_BCAST 
/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* NWK parameters handled by Commissioning client */
struct nwk_parameters{
  uint64_t extendedPanID;
  uint32_t channelMask;
  uint16_t nwk_addr;
  uint8_t nwkKeyType; 
  uint8_t stackProfile;
  uint8_t scan_count;
  uint8_t startupControl;
  bool useInsecureJoin;
};

/* ZC NWK configuration structure */
struct zigbee_nwk_config {
  uint8_t currentConfig;
  struct nwk_parameters config[NB_NWK_CONFIG];
};

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_App_Init(void);

static void APP_ZIGBEE_Commissioning_Client_Init(void);
static void APP_ZIGBEE_Commissioning_Cmd_Rsp_Mgmt(struct ZbZclCommandRspT *zcl_rsp);
static void APP_ZIGBEE_Generic_Cmd_Rsp_cb(struct ZbZclCommandRspT *zcl_rsp, void *arg);

static void APP_ZIGBEE_Commissioning_Client_RemoteWrite_cb(const ZbZclWriteRspT *writeResp, void *cb_arg);
static void APP_ZIGBEE_NwkInfo(void);
static void APP_ZIGBEE_Commissioning_Client_SetNwkCfg_cmd(uint8_t config_num, struct nwk_parameters *params);

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

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;

  struct ZbZclClusterT *commissioning_client;
};
static struct zigbee_app_info zigbee_app_info;

static struct zigbee_nwk_config nwk_config = {
  /* Starting with network configuration 1 */
  .currentConfig = ZR_COMMISSIONING_SERVER_INDEX_NWK_1,
  
  /* nwk 1 parameters */
  .config[0].extendedPanID = EXT_PANID_1,
  .config[0].channelMask = CHANNEL_1,
  .config[0].nwk_addr = ZR_NWK_ADDR,
  .config[0].nwkKeyType = ZB_SEC_KEYTYPE_STANDARD_NWK,
  .config[0].stackProfile = ZB_NWK_STACK_PROFILE_PRO,
  .config[0].scan_count = ZR_NB_SCANS,
  .config[0].startupControl = ZbStartTypeJoin,
  .config[0].useInsecureJoin = true,
};


/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Device generic command response callback
 * @param  zcl_rsp: ZCL response
 * @param  arg: passed arguments
 * @retval None
 */
static void APP_ZIGBEE_Generic_Cmd_Rsp_cb(struct ZbZclCommandRspT *zcl_rsp, void *arg){
  UNUSED(arg);
  
  /* Checking response consistency */
  assert(zcl_rsp != NULL);
  
  switch(zcl_rsp->hdr.frameCtrl.frameType){
    case ZCL_FRAMETYPE_PROFILE:
      /* general frame cmd response */   
      
      switch(zcl_rsp->hdr.cmdId){
        case ZCL_COMMAND_DEFAULT_RESPONSE:
          /* general default response */
          if (zcl_rsp->status != ZCL_STATUS_SUCCESS) {
            APP_DBG("Default response: the server returned an error: 0x%02x.\n", zcl_rsp->status);
          } else {
            APP_DBG("Default response: success!\n");
          }
         break;
         
         default:
          APP_DBG("Error: unsupported general cmd type.\n");
          break;
      }
      break;
      
    case ZCL_FRAMETYPE_CLUSTER:
      /* cluster specific frame cmd response */
      
      if(zcl_rsp->status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error: cmd not successfully received. 0x%02x.\n", zcl_rsp->status);
        return;
      }
      
      switch(zcl_rsp->clusterId){
        case ZCL_CLUSTER_COMMISSIONING:
          /* Commissioning cluster */
          APP_ZIGBEE_Commissioning_Cmd_Rsp_Mgmt(zcl_rsp);
          break;
         
         default:
          APP_DBG("Error: unsupported cluster.\n");
          break;
      }
      break;
      
    default:
      APP_DBG("Error: unsupported frame type.\n");
      break;
  }
} /* APP_ZIGBEE_Generic_Cmd_Rsp_cb */

/**
 * @brief  Commissioning client command response management
 * @param  zcl_rsp: ZCL response
 * @retval None
 */
static void APP_ZIGBEE_Commissioning_Cmd_Rsp_Mgmt(struct ZbZclCommandRspT *zcl_rsp){
  uint8_t rsp_status = 0;
  
  /* Checking response consistency */
  assert(zcl_rsp != NULL && zcl_rsp->payload != NULL);
  
  memcpy(&rsp_status, zcl_rsp->payload, sizeof(rsp_status));
  switch(zcl_rsp->hdr.cmdId){
    case ZCL_COMMISSION_SVR_CMD_RESTART_DEVICE_RSP:
      /* Commissioning restart device cmd response */
      APP_DBG("RestartDevice response received from server.");
      if(rsp_status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error, server did not handle the request successfully. Error code 0x%2x", rsp_status);
        return;
      }
      break;
      
    case ZCL_COMMISSION_SVR_CMD_SAVE_STARTUP_RSP:
      /* Commissioning save startup cmd response */
      APP_DBG("SaveStartup response received from server.");
      if(rsp_status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error, server did not handle the request successfully. Error code 0x%2x", rsp_status);
        return;
      }
      break;
      
    case ZCL_COMMISSION_SVR_CMD_RESTORE_STARTUP_RSP:
      /* Commissioning restore startup cmd response */
      APP_DBG("RestoreStartup response received from server.");
      if(rsp_status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error, server did not handle the request successfully. Error code 0x%2x", rsp_status);
        return;
      }
      break;
      
    case ZCL_COMMISSION_SVR_CMD_RESET_STARTUP_RSP:
      /* Commissioning reset startup cmd response */
      APP_DBG("ResetStartup response received from server.");
      if(rsp_status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error, server did not handle the request successfully. Error code 0x%2x", rsp_status);
        return;
      }
      break;
             
    default:
      APP_DBG("Error: unsupported general cmd type.\n");
      return;
    }
  
  APP_DBG("Response is OK, continue Commissioning process.\n");

  /* Continue the Commissioning process */
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_CONTINUE_INIT);
} /* APP_ZIGBEE_Commissioning_Cmd_Rsp_Mgmt */

/**
 * @brief  Zigbee application helper to configure Commissioning server
 * @param  config_num: Current NWK commissioning client NWK configuration
 * @param  params: Corresponding NWK parameters
 * @retval None
 */
static void APP_ZIGBEE_Commissioning_Client_SetNwkCfg_cmd(uint8_t config_num, struct nwk_parameters *params){
  enum ZclStatusCodeT status;  
  struct ZbZclCommissionClientResetStartup ResetStartupReq; 
  struct ZbZclCommissionClientSaveStartup SaveStartupReq;
  struct ZbZclCommissionClientRestoreStartup RestoreStartup;
  struct ZbZclCommissionClientRestartDev RestartDeviceReq;
  ZbZclWriteReqT RemoteWriteReq;
  uint64_t epid;
  uint32_t channelMask;
  uint16_t nwk_addr;
  
  /* Resetting Commissioning client nwk configuration */
  APP_DBG("Sending ResetStartup request."); 
  memset(&ResetStartupReq, 0, sizeof(ResetStartupReq));
  status = ZbZclCommissionClientSendResetStartup(zigbee_app_info.commissioning_client, ZR_EXT_ADDR, COMMISSIONING_DEST_ENDPOINT ,&ResetStartupReq,
                                                 APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclCommissionClientSendResetStartup failed");
    assert(0);
  }
  
  /* Waiting the successful server response */
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Remotely writing Commissioning client nwk configuration (request 1) */
  memset(&RemoteWriteReq, 0, sizeof(RemoteWriteReq));
  RemoteWriteReq.dst.mode = ZB_APSDE_ADDRMODE_EXT;
  /* We want to address any device with INTERPAN, 
   * even if not connected to ZC's network */
  RemoteWriteReq.dst.panId = ZB_PANID_WILDCARD; /* wildcard value */
  RemoteWriteReq.dst.extAddr = ZR_EXT_ADDR;
  RemoteWriteReq.dst.endpoint = ZB_ENDPOINT_INTERPAN;
  RemoteWriteReq.count = 4;
  
  APP_DBG("Remotely writing Commissioning server attributes.");
  
  /* NWK address attribute */
  APP_DBG("Modifying NKK address.");
  RemoteWriteReq.attr[0].attrId = ZCL_COMMISSION_SVR_ATTR_SHORT_ADDR;
  RemoteWriteReq.attr[0].type = ZCL_DATATYPE_UNSIGNED_16BIT;
  putle16((uint8_t*)&nwk_addr, params->nwk_addr);
  RemoteWriteReq.attr[0].value = (uint8_t const*)&nwk_addr;
  RemoteWriteReq.attr[0].length = 2;
  
  /* Extended PAN ID attribute */
  APP_DBG("Modifying extended PAN ID.");
  RemoteWriteReq.attr[1].attrId = ZCL_COMMISSION_SVR_ATTR_EPID;
  RemoteWriteReq.attr[1].type = ZCL_DATATYPE_BITMAP_64BIT;
  putle64((uint8_t*)&epid, params->extendedPanID);
  RemoteWriteReq.attr[1].value = (uint8_t const*)&epid;
  RemoteWriteReq.attr[1].length = 8;
  
  /* Stack profile attribute */
  APP_DBG("Modifying stack profile.");
  RemoteWriteReq.attr[2].attrId = ZCL_COMMISSION_SVR_ATTR_STACKPROFILE;
  RemoteWriteReq.attr[2].type = ZCL_DATATYPE_UNSIGNED_8BIT;
  RemoteWriteReq.attr[2].value = (uint8_t const*)&params->stackProfile;
  RemoteWriteReq.attr[2].length = 1;
  
  /* Channel mask attribute */
  APP_DBG("Modifying channel mask.");
  RemoteWriteReq.attr[3].attrId = ZCL_COMMISSION_SVR_ATTR_CHANNELMASK;
  RemoteWriteReq.attr[3].type = ZCL_DATATYPE_BITMAP_32BIT;
  putle32((uint8_t*)&channelMask, 1 << params->channelMask);
  RemoteWriteReq.attr[3].value = (uint8_t const*)&channelMask;
  RemoteWriteReq.attr[3].length = 4;
    
  status = ZbZclWriteReq(zigbee_app_info.commissioning_client, &RemoteWriteReq, APP_ZIGBEE_Commissioning_Client_RemoteWrite_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclWriteReq failed");
    assert(0);
  }
  
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  /* Remotely writing Commissioning client nwk configuration (request 1) */
  memset(&RemoteWriteReq, 0, sizeof(RemoteWriteReq));
  RemoteWriteReq.dst.mode = ZB_APSDE_ADDRMODE_EXT;
  /* We want to address any device with INTERPAN, 
   * even if not connected to ZC's network */
  RemoteWriteReq.dst.panId = ZB_PANID_WILDCARD; /* wildcard value */
  RemoteWriteReq.dst.extAddr = ZR_EXT_ADDR;
  RemoteWriteReq.dst.endpoint = ZB_ENDPOINT_INTERPAN;
  RemoteWriteReq.count = 4;
  
  APP_DBG("Remotely writing Commissioning server attributes.");
  
  /* Use Insecure Join attribute */
  APP_DBG("Modifying insecure join.");
  RemoteWriteReq.attr[0].attrId = ZCL_COMMISSION_SVR_ATTR_USEINSECJOIN;
  RemoteWriteReq.attr[0].type = ZCL_DATATYPE_BOOLEAN;
  RemoteWriteReq.attr[0].value = (uint8_t const*)&params->useInsecureJoin;
  RemoteWriteReq.attr[0].length = 1;
  
  /* Preconfigured Link Key attribute */
  APP_DBG("Modifying preconfigured link key.");
  RemoteWriteReq.attr[1].attrId = ZCL_COMMISSION_SVR_ATTR_PRECONFLINKKEY;
  RemoteWriteReq.attr[1].type = ZCL_DATATYPE_SECURITY_KEY128;
  RemoteWriteReq.attr[1].value = (uint8_t const*)&sec_key_ha;
  RemoteWriteReq.attr[1].length = 16;
  
  /* Network Key type attribute */
  APP_DBG("Modifying NWK key type.");
  RemoteWriteReq.attr[2].attrId = ZCL_COMMISSION_SVR_ATTR_NWKKEYTYPE;
  RemoteWriteReq.attr[2].type = ZCL_DATATYPE_ENUMERATION_8BIT;
  RemoteWriteReq.attr[2].value = (uint8_t const*)&params->nwkKeyType;
  RemoteWriteReq.attr[2].length = 1;
  
  /* Startup Control attribute */
  APP_DBG("Modifying startup control.");
  RemoteWriteReq.attr[3].attrId = ZCL_COMMISSION_SVR_ATTR_STARTUPCONTROL;
  RemoteWriteReq.attr[3].type = ZCL_DATATYPE_ENUMERATION_8BIT;
  RemoteWriteReq.attr[3].value = (uint8_t const*)&params->startupControl;
  RemoteWriteReq.attr[3].length = 1;
    
  status = ZbZclWriteReq(zigbee_app_info.commissioning_client, &RemoteWriteReq, APP_ZIGBEE_Commissioning_Client_RemoteWrite_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclWriteReq failed");
    assert(0);
  }
  
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Remotely writing Commissioning client nwk configuration (request 2) */
  memset(&RemoteWriteReq, 0, sizeof(RemoteWriteReq));
  RemoteWriteReq.dst.mode = ZB_APSDE_ADDRMODE_EXT;
  /* We want to address any device with INTERPAN, 
   * even if not connected to ZC's network */
  RemoteWriteReq.dst.panId = ZB_PANID_WILDCARD; /* wildcard value */
  RemoteWriteReq.dst.extAddr = ZR_EXT_ADDR;
  RemoteWriteReq.dst.endpoint = ZB_ENDPOINT_INTERPAN;
  RemoteWriteReq.count = 1;
  
  APP_DBG("Remotely writing Commissioning server attributes.");
  
  /* Scan attempts attribute */
  APP_DBG("Modifying scan attempts.");
  RemoteWriteReq.attr[0].attrId = ZCL_COMMISSION_SVR_ATTR_SCANATTEMPTS;
  RemoteWriteReq.attr[0].type = ZCL_DATATYPE_UNSIGNED_8BIT;
  RemoteWriteReq.attr[0].value = (uint8_t const*)&params->scan_count;
  RemoteWriteReq.attr[0].length = 1;
  
  status =   ZbZclWriteReq(zigbee_app_info.commissioning_client, &RemoteWriteReq, APP_ZIGBEE_Commissioning_Client_RemoteWrite_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclWriteReq failed");
    assert(0);
  }
  
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Saving Commissioning client nwk configuration */
  APP_DBG("Sending SaveStartup request."); 
  memset(&SaveStartupReq, 0, sizeof(SaveStartupReq));
  SaveStartupReq.index = config_num;
  status = ZbZclCommissionClientSendSaveStartup(zigbee_app_info.commissioning_client, ZR_EXT_ADDR, COMMISSIONING_DEST_ENDPOINT,&SaveStartupReq,
                                                APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclCommissionClientSendSaveStartup failed");
    assert(0);
  }
  
  /* Waiting the successful server response */
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Resetting Commissioning client nwk configuration */
  APP_DBG("Sending ResetStartup request."); 
  memset(&ResetStartupReq, 0, sizeof(ResetStartupReq));
  status = ZbZclCommissionClientSendResetStartup(zigbee_app_info.commissioning_client, ZR_EXT_ADDR, COMMISSIONING_DEST_ENDPOINT,&ResetStartupReq, 
                                                 APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclCommissionClientSendResetStartup failed");
    assert(0);
  }
  
  /* Waiting the successful server response */
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Sending Restore Startup command */
  APP_DBG("Sending RestoreStartup request."); 
  memset(&RestoreStartup, 0, sizeof(RestoreStartup));
  RestoreStartup.index = config_num;
  status = ZbZclCommissionClientSendRestoreStartup(zigbee_app_info.commissioning_client, ZR_EXT_ADDR, COMMISSIONING_DEST_ENDPOINT,&RestoreStartup,
                                                   APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclCommissionClientSendResetStartup failed");
    assert(0);
  }
  
  /* Waiting the successful server response */
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  /* Sending RestartDevice command */
  APP_DBG("Sending RestartDevice request."); 
  memset(&RestartDeviceReq, 0, sizeof(RestartDeviceReq));
  
  /* We want the receiver to install the current set of startup parameters before restarting */
  RestartDeviceReq.options = ZCL_COMMISS_RESTART_OPTS_MODE_USE_STARTUP;
  RestartDeviceReq.delay = ZR_RESTART_DEVICE_DELAY;
  status = ZbZclCommissionClientSendRestart(zigbee_app_info.commissioning_client, ZR_EXT_ADDR, COMMISSIONING_DEST_ENDPOINT,&RestartDeviceReq,
                                            APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("Error, ZbZclCommissionClientSendRestart failed");
    assert(0);
  }
  
  /* Waiting the successful server response */
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_CONTINUE_INIT);
  
  APP_DBG("Commissioning process successfully ended.\n");
}

/**
 * @brief  Zigbee application helper to show network information
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkInfo(void){
  struct ZbNlmeGetInterfaceReqT ZbNlmeReq;
  struct ZbNlmeGetInterfaceConfT ZbNlmeConf;
  uint64_t epid = 0;
  uint32_t channelInUse = 0 ;
  
  /* Extended Pan ID */
  if(ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS){
    APP_DBG("Error, ZbNwkGet failed.");
    return; 
  }
  APP_DBG("Extended PAN ID: 0x%" PRIx64 "\n.", epid);
  
  /* Channel in use */
  
  /* NLME-GET-INTERFACE.request */
  memset(&ZbNlmeReq, 0, sizeof(ZbNlmeReq));
  memset(&ZbNlmeConf, 0, sizeof(ZbNlmeConf));
  ZbNlmeReq.ifcIndex = 0; /* interface 0 */
  
  ZbNlmeGetInterface(zigbee_app_info.zb, &ZbNlmeReq, &ZbNlmeConf);
  if (ZbNlmeConf.status != ZB_STATUS_SUCCESS) {
    APP_DBG("Error, ZbNlmeGetInterface failed.");
    return;
  }
  
  /* Current channel */
  while (!(ZbNlmeConf.channelInUse & 1)) {  
    /* Iterate through bits until we find the only set bit */
    ZbNlmeConf.channelInUse = ZbNlmeConf.channelInUse >> 1;
    ++channelInUse; 
  } 
  
  APP_DBG("Channel in use: %d\n.", channelInUse);
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Initialize Zigbee Commissioning Client parameters */
  APP_ZIGBEE_Commissioning_Client_Init();
} /* APP_ZIGBEE_App_Init */

/**
 * @brief  Commissioning client initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_Commissioning_Client_Init(void){
  /* Show NWK info */
  APP_ZIGBEE_NwkInfo();
  
  APP_DBG("Commissioning client init done!\n");  
} /* APP_ZIGBEE_Commissioning_Client_Init */

/**
 * @brief  Device generic remote write response callback
 * @param  writeResp: ZCL write response
 * @param  cb_arg: passed arguments
 * @retval None
 */
static void APP_ZIGBEE_Commissioning_Client_RemoteWrite_cb(const ZbZclWriteRspT *writeResp, void *cb_arg){
   UNUSED(cb_arg);
   
   if(writeResp->status != ZCL_STATUS_SUCCESS){
     APP_DBG("Error, remote write failed.\n");
     return;
   }
   
   UTIL_SEQ_SetEvt(EVENT_ZIGBEE_CONTINUE_INIT);
   APP_DBG("Remote write OK.\n"); 
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
  
  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  
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
  zigbee_app_info.startupControl = ZbStartTypeJoin;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_StackLayersInit */

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  /* Commissioning Client */
  zigbee_app_info.commissioning_client = ZbZclCommissionClientAlloc(zigbee_app_info.zb, COMMISSIONING_DEST_ENDPOINT,ZCL_PROFILE_HOME_AUTOMATION, false);
  assert(zigbee_app_info.commissioning_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.commissioning_client);

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
    config.channelList.list[0].channelMask = 1 << nwk_config.config[nwk_config.currentConfig].channelMask; /* Channel in use */
    config.extendedPanId = nwk_config.config[nwk_config.currentConfig].extendedPanID; /* PAN ID in use */

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
    APP_DBG("Channel used: %d", CHANNEL_1);
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
    APP_DBG("Commissioning Client on Endpoint %d",COMMISSIONING_DEST_ENDPOINT);
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
  
  /* Set NWK configuration 1 on the commissioning server */
  APP_DBG("Setting up NWK configuration for the ZR.");
  APP_ZIGBEE_Commissioning_Client_SetNwkCfg_cmd(nwk_config.currentConfig, &nwk_config.config[nwk_config.currentConfig]);
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

