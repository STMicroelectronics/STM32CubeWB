/**
******************************************************************************
* @file    config_client.h
* @author  BLE Mesh Team
* @version V1.11.000
* @date    25-07-2019
* @brief   Header file for the Config Model Client
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BLE-Mesh is built over Motorola�s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_CLIENT_H
#define __CONFIG_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/

#define CONFIG_CLIENT_UNICAST_ADDR   0x0001

#define MAX_SIG_MODELS_PER_ELEMENT    12   // Number of SIG Models to support 
#define MAX_VENDOR_MODELS_PER_ELEMENT  1   // Number of Vendor Models to support 
#define MAX_ELEMENTS_PER_NODE          1
#define CONFIG_COMPOSITION_DATA_GET_PAGE_SIZE 1
#define COMPOSITION_PAGE0              0

#define MAX_CONFIG_CLIENT_MODEL_TX_MSG_SIZE   20
#define NETKEY_SIZE  16
#define APPKEY_SIZE  16

#define CONFIGURATION_START_DELAY      2000  
#define CONFIGCLIENT_RESPONSE_TIMEOUT  10000   /* 10 sec Timeout */
#define CONFIGCLIENT_MAX_TRIALS        5       /* Attempt 5 times retries  */
#define CONFIGCLIENT_RE_TRIALS         3

#define CLIENT_TX_INPROGRESS  0
#define CLIENT_TX_TIMEOUT     1
#define CLIENT_TX_RETRY_ENDS  2


#define SIG_MODEL_ID_CONFIG_SERVER 0x0000
#define SIG_MODEL_ID_CONFIG_CLIENT 0x0001
#define SIG_MODEL_ID_HEALTH_SERVER 0x0002
#define SIG_MODEL_ID_HEALTH_CLIENT 0x0003

/* 4.3.4.1 Alphabetical summary of opcodes */
#define OPCODE_CONFIG_COMPOSITION_DATA_GET    0x8008
#define OPCODE_CONFIG_APPKEY_ADD             0x0000
#define OPCODE_CONFIG_APPKEY_DELETE           0x8000
#define OPCODE_CONFIG_MODEL_APP_BIND          0x803D

#define OPCODE_CONFIG_MODEL_APP_UNBIND        0x803F
#define OPCODE_CONFIG_MODEL_PUBLI_GET         0x8018
#define OPCODE_CONFIG_MODEL_PUBLI_SET           0x03
#define OPCODE_CONFIG_MODEL_SUBSCR_ADD          0x801B
#define OPCODE_CONFIG_MODEL_SUBSCR_DEL          0x801C
#define OPCODE_CONFIG_MODEL_SUBSCR_DEL_ALL      0x801D
#define OPCODE_CONFIG_MODEL_SUBSCR_OWR          0x801E

/* Status message list : These are required for Node configuration */
#define OPCODE_CONFIG_COMPOSITION_DATA_STATUS   0x02
#define OPCODE_CONFIG_APPKEY_STATUS             0x8003
#define OPCODE_CONFIG_SUBSCRIPTION_STATUS     0x801F
#define OPCODE_CONFIG_MODEL_PUBLI_STATUS        0x8019
#define OPCODE_CONFIG_MODEL_APP_STATUS        0x803E

/* Macros for usage in models ************************************************/

#define CHKSIGMODEL(a) (a&0xffff0000)==0
#define ADDRESS_IS_GROUP(a) (((MOBLEINT16)(a) & 0xC000) == 0xC000)  
#define ADDRESS_UNASSIGNED              0x0000U 
#define ADDRESS_IS_UNASSIGNED(a) ((MOBLEINT16)(a) == ADDRESS_UNASSIGNED)  


/* Exported structure --------------------------------------------------------*/

#pragma pack(1)
typedef struct {
  MOBLEUINT16 nodePrimaryAddress;
  MOBLEUINT8 NbOfelements;
  MOBLEUINT8 numberOfAttemptsTx;  
  MOBLEUINT32 Initial_time; /* Initial time for the Message */

  MOBLEUINT8 NbOfSIGModelsToConfigure;
  MOBLEUINT8 NbOfSIGModelsToAppBind;
  MOBLEUINT8 NbOfSIGModelsToPublish;
  MOBLEUINT8 NbOfSIGModelsToSubscribe;
  
  MOBLEUINT8 NbOfVendorModelsToConfigure;
  MOBLEUINT8 NbOfVendorModelsToAppBind;
  MOBLEUINT8 NbOfVendorModelsToPublish;
  MOBLEUINT8 NbOfVendorModelsToSubscribe;
} NodeInfo_t;

typedef struct {
  MOBLEUINT16 Loc;    /* Contains a location descriptor */
  MOBLEUINT8  NumSIGmodels;   /* Contains a count of SIG Model IDs in this element */
  MOBLEUINT8  NumVendorModels;   /* Contains a count of Vendor Model IDs in this element */
  MOBLEUINT16 aSIGModels[MAX_SIG_MODELS_PER_ELEMENT];
  MOBLEUINT32 aVendorModels[MAX_VENDOR_MODELS_PER_ELEMENT];
} Elements_Page0_t;

typedef struct {
  MOBLEUINT8  DataPage;
  MOBLEUINT16 NodeCID;
  MOBLEUINT16 NodePID;
  MOBLEUINT16 NodeVID;
  MOBLEUINT16 NodeCRPL;
  MOBLEUINT16 NodeFeatures;
} Composition_Header_Page0_t;
#pragma pack(4)

/* 
#define MAX_SIG_MODELS_PER_ELEMENT    10   // Number of SIG Models to support 
#define MAX_VENDOR_MODELS_PER_ELEMENT  1   // Number of Vendor Models to support 
#define MAX_ELEMENTS_PER_NODE          1
*/

#define DEVICE_COMPOSITION_HEADER_SIZE   sizeof(Composition_Header_Page0_t)
#define DEVICE_COMPOSITION_ELEMENTS_DESC sizeof(Elements_Page0_t)
#define DEVICE_COMPOSITION_MAX_SIZE (DEVICE_COMPOSITION_HEADER_SIZE + (MAX_ELEMENTS_PER_NODE*DEVICE_COMPOSITION_ELEMENTS_DESC))

#pragma pack(1)
typedef struct {
  Composition_Header_Page0_t sheader; 
  //Elements_Page0_t aNodeElements[MAX_ELEMENTS_PER_NODE];
} _Composition_Data_Page0_t;
#pragma pack(4)

//#pragma pack(1)
typedef union {
  _Composition_Data_Page0_t sComposition_Data_Page0;
  MOBLEUINT8 aComposition_Data_Page0[DEVICE_COMPOSITION_MAX_SIZE];
} Composition_Data_Page0_t;

#pragma pack(1)
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
#pragma pack(4)

typedef struct {
  MOBLEUINT8 Status;
  MOBLEUINT16 netKeyIndex:12;
  MOBLEUINT16 appKeyIndex:12;
} configClientAppKeyStatus_t;

#pragma pack(1)
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
#pragma pack(4)

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
 NodeNoResponse_State
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

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/

extern NodeInfo_t NodeInfo;

/* Exported Functions Prototypes ---------------------------------------------*/
void CopyU8LittleEndienArray_fromU16word (MOBLEUINT8* pArray, MOBLEUINT16 inputWord);
void CopyU8LittleEndienArray_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord);
void CopyU8LittleEndienArray_2B_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord);
MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray);
MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray);

MOBLE_RESULT ConfigClientModel_SendMessage(MOBLE_ADDRESS dst_peer ,
                                     MOBLEUINT16 opcode, MOBLEUINT8 *pData,
                                     MOBLEUINT32 length); 


MOBLE_RESULT ConfigClient_CompositionDataGet(void); 

MOBLE_RESULT ConfigClient_CompositionDataStatusResponse(MOBLEUINT8 const *pCompositionData, 
                                                        MOBLEUINT32 length);  

MOBLEUINT8 ConfigClient_GetNodeElements(void);
MOBLEUINT16 GetSIGModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG);
MOBLEUINT32 GetVendorModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxVendor);
MOBLEUINT16 GetNodeElementAddress(void);
MOBLEUINT8 GetTotalSIGModelsCount(MOBLEUINT8 elementIdx);
MOBLEUINT8 GetTotalVendorModelsCount(MOBLEUINT8 elementIdx);
MOBLEUINT8 GetNumberofSIGModels(MOBLEUINT8 elementIdx);
MOBLEUINT8 GetNumberofVendorModels(MOBLEUINT8 elementIdx);
void SetSIGModelCountToConfigure(MOBLEUINT8 count);
void SetVendorModelCountToConfigure(MOBLEUINT8 count);
MOBLEUINT8 ConfigClient_ChkRetrialState (eServerRespRecdState_t* eRespRecdState);
void ConfigClient_SaveMsgSendingTime (void);
void ConfigClient_ResetTrials (void);
void ConfigClient_ErrorState (void);

MOBLE_RESULT ConfigClient_AppKeyAdd (MOBLEUINT16 netKeyIndex, MOBLEUINT16 appKeyIndex, 
                                     MOBLEUINT8* appkey);
MOBLE_RESULT _ConfigClient_AppKeyAdd (configClientAppKeyAdd_t* pClientAppKey);

MOBLE_RESULT ConfigClient_ModelAppBind (MOBLEUINT16 elementAddress,
                                        MOBLEUINT16 appKeyIndex,
                                        MOBLEUINT32 modelIdentifier);

MOBLE_RESULT _ConfigClient_ModelAppBind (configClientModelAppBind_t* modelAppBind);

MOBLE_RESULT ConfigClient_ModelAppStatus(MOBLEUINT8 const *pSrcModelAppStatus, 
                                                        MOBLEUINT32 length);


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

MOBLE_RESULT ConfigClientModel_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response);
                                    
MOBLE_RESULT ConfigClientModel_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response);                                    
#endif /* __CONFIG_CLIENT_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

