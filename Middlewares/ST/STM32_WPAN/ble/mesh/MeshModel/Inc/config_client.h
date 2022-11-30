/**
******************************************************************************
* @file    config_client.h
* @author  BLE Mesh Team
* @brief   Header file for the Config Model Client
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_CLIENT_H
#define __CONFIG_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"
#include "mesh_cfg.h"
#include "mesh_cfg_usr.h"

/* Exported macro ------------------------------------------------------------*/

#define CONFIG_CLIENT_UNICAST_ADDR   0x0001
#define CONFIG_COMPOSITION_DATA_GET_PAGE_SIZE 1
#define COMPOSITION_PAGE0              0

#define MAX_CONFIG_CLIENT_MODEL_TX_MSG_SIZE   20
#define NETKEY_SIZE  16
#define APPKEY_SIZE  16

#define CONFIGURATION_START_DELAY      2000  
//#define CONFIGCLIENT_RESPONSE_TIMEOUT  20000    /* 20 sec Timeout */
#define CONFIGCLIENT_RESPONSE_TIMEOUT  2000    /* 2 sec Timeout */
#define CONFIGCLIENT_MAX_TRIALS        3    /* Attempt 3 times retries  */


#define CLIENT_TX_INPROGRESS  0
#define CLIENT_TX_TIMEOUT     1
#define CLIENT_TX_RETRY_ENDS  2


#define SIG_MODEL_ID_CONFIG_SERVER 0x0000
#define SIG_MODEL_ID_CONFIG_CLIENT 0x0001
#define SIG_MODEL_ID_HEALTH_SERVER 0x0002
#define SIG_MODEL_ID_HEALTH_CLIENT 0x0003

/* 4.3.4.2 Numerical summary of opcodes */
#define OPCODE_CONFIG_APPKEY_ADD                     0x00
#define OPCODE_CONFIG_APPKEY_UPDATE                  0x01
#define OPCODE_CONFIG_COMPOSITION_DATA_STATUS        0x02
#define OPCODE_CONFIG_CONFIG_MODEL_PUBLICATION_SET   0x03
#define OPCODE_HEALTH_CURRENT_STATUS                 0x04
#define OPCODE_HEALTH_FAULT_STATUS                   0x05
#define OPCODE_CONFIG_HEARTBEAT_PUBLICATION_STATUS   0x06
#define OPCODE_CONFIG_APPKEY_DELETE                0x8000
#define OPCODE_CONFIG_APPKEY_GET                   0x8001
#define OPCODE_CONFIG_APPKEY_LIST                  0x8002
#define OPCODE_CONFIG_APPKEY_STATUS                0x8003
#define OPCODE_HEALTH_ATTENTION_GET                0x8004
#define OPCODE_HEALTH_ATTENTION_SET                0x8005
#define OPCODE_HEALTH_ATTENTION_SET_UNACKNOWLEDGED 0x8006
#define OPCODE_HEALTH_ATTENTION_STATUS             0x8007
#define OPCODE_CONFIG_COMPOSITION_DATA_GET    0x8008
#define OPCODE_CONFIG_BEACON_GET                   0x8009
#define OPCODE_CONFIG_BEACON_SET                   0x800A
#define OPCODE_CONFIG_BEACON_STATUS                0x800B
#define OPCODE_CONFIG_DEFAULT_TTL_GET              0x800C
#define OPCODE_CONFIG_DEFAULT_TTL_SET              0x800D
#define OPCODE_CONFIG_DEFAULT_TTL_STATUS           0x800E
#define OPCODE_CONFIG_FRIEND_GET          0x800F
#define OPCODE_CONFIG_FRIEND_SET          0x8010
#define OPCODE_CONFIG_FRIEND_STATUS       0x8011
#define OPCODE_CONFIG_GATT_PROXY_GET      0x8012
#define OPCODE_CONFIG_GATT_PROXY_SET      0x8013
#define OPCODE_CONFIG_GATT_PROXY_STATUS   0x8014
#define OPCODE_CONFIG_KEY_REFRESH_PHASE_GET    0x8015
#define OPCODE_CONFIG_KEY_REFRESH_PHASE_SET    0x8016
#define OPCODE_CONFIG_KEY_REFRESH_PHASE_STATUS 0x8017
#define OPCODE_CONFIG_MODEL_PUBLICATION_GET    0x8018
#define OPCODE_CONFIG_MODEL_PUBLICATION_STATUS 0x8019
#define OPCODE_CONFIG_MODEL_PUBLICATION_VIRTUAL_ADDRESS_SET 0x801A
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_ADD    0x801B
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE 0x801C
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE_ALL 0x801D
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_OVERWRITE  0x801E
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_STATUS     0x801F
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_ADD       0x8020
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_DELETE    0x8021
#define OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_OVERWRITE 0x8022
#define OPCODE_CONFIG_NETWORK_TRANSMIT_GET      0x8023
#define OPCODE_CONFIG_NETWORK_TRANSMIT_SET      0x8024
#define OPCODE_CONFIG_NETWORK_TRANSMIT_STATUS   0x8025

#define OPCODE_CONFIG_RELAY_GET    0x8026
#define OPCODE_CONFIG_RELAY_SET    0x8027
#define OPCODE_CONFIG_RELAY_STATUS 0x8028

#define OPCODE_CONFIG_SIG_MODEL_SUBSCRIPTION_GET     0x8029
#define OPCODE_CONFIG_SIG_MODEL_SUBSCRIPTION_LIST    0x802A
#define OPCODE_CONFIG_VENDOR_MODEL_SUBSCRIPTION_GET  0x802B
#define OPCODE_CONFIG_VENDOR_MODEL_SUBSCRIPTION_LIST 0x802C
#define OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_GET 0x802D
#define OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_STATUS 0x802E

#define OPCODE_HEALTH_FAULT_CLEAR                0x802F
#define OPCODE_HEALTH_FAULT_CLEAR_UNACKNOWLEDGED 0x8030
#define OPCODE_HEALTH_FAULT_GET                  0x8031
#define OPCODE_HEALTH_FAULT_TEST                 0x8032
#define OPCODE_HEALTH_FAULT_TEST_UNACKNOWLEDGED  0x8033
#define OPCODE_HEALTH_PERIOD_GET                 0x8034
#define OPCODE_HEALTH_PERIOD_SET                 0x8035
#define OPCODE_HEALTH_PERIOD_SET_UNACKNOWLEDGED  0x8036
#define OPCODE_HEALTH_PERIOD_STATUS              0x8037
#define OPCODE_CONFIG_HEARTBEAT_PUBLICATION_GET  0x8038
#define OPCODE_CONFIG_HEARTBEAT_PUBLICATION_SET  0x8039
#define OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_GET 0x803A
#define OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_SET 0x803B
#define OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_STATUS 0x803C

#define OPCODE_CONFIG_MODEL_APP_BIND          0x803D
#define OPCODE_CONFIG_MODEL_APP_STATUS 0x803E
#define OPCODE_CONFIG_MODEL_APP_UNBIND 0x803F
#define OPCODE_CONFIG_NETKEY_ADD       0x8040
#define OPCODE_CONFIG_NETKEY_DELETE    0x8041
#define OPCODE_CONFIG_NETKEY_GET       0x8042
#define OPCODE_CONFIG_NETKEY_LIST      0x8043
#define OPCODE_CONFIG_NETKEY_STATUS    0x8044
#define OPCODE_CONFIG_NETKEY_UPDATE    0x8045
#define OPCODE_CONFIG_NODE_IDENTITY_GET 0x8046
#define OPCODE_CONFIG_NODE_IDENTITY_SET 0x8047

#define OPCODE_CONFIG_NODE_IDENTITY_STATUS 0x8048
#define OPCODE_CONFIG_NODE_RESET           0x8049
#define OPCODE_CONFIG_NODE_RESET_STATUS    0x804A
#define OPCODE_CONFIG_SIG_MODEL_APP_GET    0x804B
#define OPCODE_CONFIG_SIG_MODEL_APP_LIST   0x804C
#define OPCODE_CONFIG_VENDOR_MODEL_APP_GET 0x804D
#define OPCODE_CONFIG_VENDOR_MODEL_APP_LIST 0x804E



/* Macros for usage in models ************************************************/

#define CHKSIGMODEL(a) (a&0xffff0000)==0
#define ADDRESS_IS_GROUP(a) (((MOBLEINT16)(a) & 0xC000) == 0xC000)  
#define ADDRESS_UNASSIGNED              0x0000U 
#define ADDRESS_IS_UNASSIGNED(a) ((MOBLEINT16)(a) == ADDRESS_UNASSIGNED)  
#define MODEL_IS_FOUNDATION_MODEL(a) ( ((MOBLEINT16)(a) & 0xFFF0) == 0)  

/* Exported structure --------------------------------------------------------*/

#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)


#pragma pack(push, 1)

typedef struct {
  MOBLEUINT16 nodePrimaryAddress;
  MOBLEUINT8 NbOfelements;
  MOBLEUINT8 numberOfAttemptsTx;  
  MOBLEUINT32 Initial_time; /* Initial time for the Message */

} NodeInfo_t;


typedef struct {
  MOBLEUINT16 Loc;    /* Contains a location descriptor */
  MOBLEUINT8  NumSIGmodels;   /* Contains a count of SIG Model IDs in this element */
  MOBLEUINT8  NumVendorModels;   /* Contains a count of Vendor Model IDs in this element */
  MOBLEUINT16 aSIGModels[CLIENT_MAX_SIG_MODELS_PER_ELEMENT];
  MOBLEUINT32 aVendorModels[CLIENT_MAX_VENDOR_MODELS_PER_ELEMENT];
} Elements_Page0_t;


typedef struct {
  MOBLEUINT8  DataPage;
  MOBLEUINT16 NodeCID;
  MOBLEUINT16 NodePID;
  MOBLEUINT16 NodeVID;
  MOBLEUINT16 NodeCRPL;
  MOBLEUINT16 NodeFeatures;
} Composition_Header_Page0_t;


#define DEVICE_COMPOSITION_HEADER_SIZE   sizeof(Composition_Header_Page0_t)
#define DEVICE_COMPOSITION_ELEMENTS_DESC sizeof(Elements_Page0_t)
#define DEVICE_COMPOSITION_MAX_SIZE (DEVICE_COMPOSITION_HEADER_SIZE) 
//+ (MAX_ELEMENTS_PER_NODE*DEVICE_COMPOSITION_ELEMENTS_DESC))


typedef struct {
  Composition_Header_Page0_t sheader; 
} _Composition_Data_Page0_t;



typedef union {
  _Composition_Data_Page0_t sComposition_Data_Page0;
  MOBLEUINT8 aComposition_Data_Page0[DEVICE_COMPOSITION_MAX_SIZE];
} Composition_Data_Page0_t;


typedef struct {
  MOBLEUINT16 Opcode;
  MOBLEUINT8 page;
} configClientGetCompositionMsg_t;


typedef struct {
  MOBLEUINT16 elementAddr;
  MOBLEUINT16 publishAddr;
  MOBLEUINT16 appKeyIndex:12;
  MOBLEUINT8 credentialFlag:1;
  MOBLEUINT8 rfu:3;
  MOBLEUINT8 publishTTL;
  MOBLEUINT8 publishPeriod;
  MOBLEUINT8 publishRetransmitCount:3;
  MOBLEUINT8 publishRetransmitIntervalSteps:5;
  MOBLEUINT32 modelIdentifier;
} configClientModelPublication_t;


typedef struct {
  MOBLEUINT8 Status;
  MOBLEUINT16 elementAddr;
  MOBLEUINT16 publishAddr;
  MOBLEUINT16 appKeyIndex:12;
  MOBLEUINT8 credentialFlag:1;
  MOBLEUINT8 rfu:3;
  MOBLEUINT8 publishTTL;
  MOBLEUINT8 publishPeriod;
  MOBLEUINT8 publishRetransmitCount:3;
  MOBLEUINT8 publishRetransmitIntervalSteps:5;
  MOBLEUINT32 modelIdentifier;
} configClientPublicationStatus_t;


typedef struct {
  MOBLEUINT16 netKeyIndex:12;
  MOBLEUINT16 appKeyIndex:12;
  MOBLEUINT8 a_Appkeybuffer[APPKEY_SIZE];
} configClientAppKeyAdd_t;

typedef struct {
  MOBLEUINT8 Status;
  MOBLEUINT16 netKeyIndex:12;
  MOBLEUINT16 appKeyIndex:12;
} configClientAppKeyStatus_t;


typedef struct {
  MOBLEUINT16 elementAddress;
  MOBLEUINT16 appKeyIndex;
  MOBLEUINT32 modelIdentifier;
} configClientModelAppBind_t;

/* 4.3.2.48 Config Model App Status */

typedef struct {
  MOBLEUINT8 Status;
  MOBLEUINT16 elementAddress;
  MOBLEUINT16 appKeyIndex;
  MOBLEUINT32 modelIdentifier;
} configClientModelAppStatus_t;

typedef struct {
  MOBLEUINT16 elementAddress;
  MOBLEUINT16 address;
  MOBLEUINT32 modelIdentifier;
} configClientModelSubscriptionAdd_t;

/* 4.3.2.26 Config Model Subscription Status */
typedef struct {
  MOBLEUINT8 Status;
  MOBLEUINT16 elementAddress;
  MOBLEUINT16 address;
  MOBLEUINT32 modelIdentifier;
} configClientSubscriptionStatus_t;

#pragma pack(pop)

typedef union ConfigClientTxMsg_s {
  configClientAppKeyAdd_t sConfigClientAppKeyAdd;
  configClientModelPublication_t sConfigClientModelPublication;
  MOBLEUINT8 a_ConfigClientSendPacket[MAX_CONFIG_CLIENT_MODEL_TX_MSG_SIZE]; 
} uConfigClientTxMsg_t;


/* These states are used for configuration messages sending after Provisioner */
typedef enum
{
 ClientIdle_State,
 ProvisioningDone_State,
 CompositionGet_State,
 AppKeyAdd_State,
 AppBindModel_State,
 AddSubscription_State,
 SetPublication_State,  
 ConfigurationDone_State,
}eClientSendMsgState_t;

/* These states are used for configuration messages responses after Provisioner */
typedef enum
{
 NodeIdle_State,
 NodeNextSigModel_State,
 NodeNextVendorModel_State,
 NodeSendMessage_State,
 InProgress_State,
 CompositionRecd_State,
 CompositionRecdCompleted_State,
 AppkeyAck_State,
 AppkeyAckCompleted_State,
 AppBindModelAck_State,
 AppBindModelAckCompleted_State,
 SubscriptionAck_State,
 SubscriptionAckCompleted_State,
 PublicationStatus_State,
 PublicationStatusCompleted_State,
 NodeNoResponse_State,
 NodeResetStatus_State
}eServerRespRecdState_t;


/* This states are used for checking individual message sending and 
   response synchronisation */

typedef enum
{
  SuccessStatus = 0x00,
  InvalidAddressStatus = 0x01,
  InvalidModelStatus = 0x02,
  InvalidAppKeyIndexStatus = 0x03,
  InvalidNetKeyIndexStatus = 0x04,
  InsufficientResourcesStatus =0x05,
  KeyIndexAlreadyStoredStatus =0x06,
  InvalidPublishParametersStatus =0x07,
  NotSubscribeModelStatus = 0x08,
  StorageFailureStatus = 0x09,
  FeatureNotSupportedStatus = 0x0A,
  CannotUpdateStatus = 0x0B,
  CannotRemoveStatus = 0x0C,
  CannotBindStatus = 0x0D,
  TemporarilyUnabletoChangeStateStatus = 0x0E,
  CannotSetStatus = 0x0F,
  UnspecifiedErrorStatus = 0x10,
  InvalidBindingStatus = 0x11,
} ConfigModelStatusCode_t;

typedef struct 
{   
    MOBLEUINT16 opcode;
    MOBLEUINT8 min_payload_size;
    MOBLEUINT8 max_payload_size;
    const MOBLEUINT8 *pDefaultParam;
} MODEL_CONFIG_CLIENT_OpcodeTableParam_t;

extern Elements_Page0_t aNodeElements[CLIENT_MAX_ELEMENTS_PER_NODE];

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/

extern NodeInfo_t NodeInfo;



/* Exported Functions Prototypes ---------------------------------------------*/


MOBLE_RESULT ConfigClientModel_SendMessage(MOBLE_ADDRESS dst_peer ,
                                     MOBLEUINT16 opcode, MOBLEUINT8 *pData,
                                     MOBLEUINT32 length); 


MOBLE_RESULT ConfigClient_CompositionDataGet(MOBLE_ADDRESS dst_peer); 

MOBLE_RESULT ConfigClient_CompositionDataStatusResponse(MOBLEUINT8 const *pCompositionData, 
                                                        MOBLEUINT32 length);  

MOBLEUINT8 ConfigClient_GetNodeElements(void);
MOBLEUINT16 GetSIGModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG);
MOBLEUINT32 GetVendorModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxVendor);
MOBLEUINT16 GetNodeElementAddress(void);
MOBLEUINT16 GetServerElementAddress(MOBLEUINT8 elementIndex);
MOBLEUINT8 GetTotalSIGModelsCount(MOBLEUINT8 elementIdx);
MOBLEUINT8 GetTotalVendorModelsCount(MOBLEUINT8 elementIdx);
MOBLEUINT8 ConfigClient_ChkRetrialState (eServerRespRecdState_t* eRespRecdState);
MOBLEUINT8 ConfigClient_ChkRetries (void);
void ConfigClient_SaveMsgSendingTime (void);
void ConfigClient_ResetTrials (void);
void ConfigClient_ErrorState (void);

MOBLE_RESULT ConfigClient_AppKeyAdd (MOBLE_ADDRESS dst_peer,
                                     MOBLEUINT16 netKeyIndex, 
                                     MOBLEUINT16 appKeyIndex, 
                                     MOBLEUINT8* appkey);

MOBLE_RESULT _ConfigClient_AppKeyAdd (configClientAppKeyAdd_t* pClientAppKey);

MOBLE_RESULT ConfigClient_ModelAppBind (MOBLEUINT16 elementAddress,
                                        MOBLEUINT16 appKeyIndex,
                                        MOBLEUINT32 modelIdentifier);

MOBLE_RESULT _ConfigClient_ModelAppBind (configClientModelAppBind_t* modelAppBind);

MOBLE_RESULT ConfigClient_ModelAppStatus(MOBLEUINT8 const *pSrcModelAppStatus, 
                                                        MOBLEUINT32 length);

MOBLE_RESULT ConfigClient_NodeReset (MOBLEUINT16 elementAddress);
void Appli_NodeResetStatusCb(void);
MOBLE_RESULT ConfigClient_SubscriptionAdd (MOBLEUINT16 elementAddress,
                                           MOBLEUINT16 appKeyIndex,
                                           MOBLEUINT32 modelIdentifier);

MOBLE_RESULT ConfigClient_SubscriptionStatus(MOBLEUINT8 const *pSrcSubscriptionStatus, 
                                                        MOBLEUINT32 length);  



MOBLE_RESULT _ConfigClient_PublicationSet (configClientModelPublication_t *pClientPublicationMsg);

MOBLE_RESULT ConfigClient_PublicationSet (MOBLEUINT16 elementAddress,
                                          MOBLEUINT16 publishAddress,
                                          MOBLEUINT16 appKeyIndex,
                                          MOBLEUINT8 credentialFlag,
                                          MOBLEUINT8 publishTTL,
                                          MOBLEUINT8 publishPeriod,
                                          MOBLEUINT8 publishRetransmitCount,
                                          MOBLEUINT8 publishRetransmitIntervalSteps,
                                          MOBLEUINT32 modelIdentifier);

MOBLE_RESULT ConfigClient_PublicationStatus(MOBLEUINT8 const *pPublicationStatus, 
                                                        MOBLEUINT32 length);                                          

MOBLE_RESULT ConfigClientModel_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT ConfigClientModel_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response);
                                    
MOBLE_RESULT ConfigClientModel_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam,
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response);                                    

#endif /* defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER) */

#endif /* __CONFIG_CLIENT_H */



