/**
******************************************************************************
* @file    ble_mesh.h
* @author  BLE Mesh Team
* @brief   Header file for the BLE-Mesh stack 
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
#ifndef _BLE_MESH_
#define _BLE_MESH_

#include "types.h"
//#include "hal_types.h"
#define BLE_MESH_APPLICATION_VERSION "1.13.007" 
/**
* \mainpage ST BLE-Mesh Solutions Bluetooth LE Mesh Library
*
* \version 1.13.007
*
* \subsection contents_sec Contents
*
* -# \ref overview_sec
* -# \ref supported_devices
* -# \ref install_sec
* -# \ref brief_descr_sec
* -# \ref other_info_sec
* -# \link ble_mesh.h ST BLE-Mesh Library User API \endlink
* -# \link types.h ST BLE-Mesh Library Data Types \endlink
*
* \subsection overview_sec Overview
* 1) Overview: 
* BLE-Mesh is a solution for connecting multiple BLE (Bluetooth Low Energy) 
* devices in Mesh networking for IoT (Internet of Things) solutions. 
* It enables the Bluetooth enabled devices into a powerful, integrated, 
* range-extending Mesh network with true two-way communication.
* The solution contains the core functionality required to form the secure 
* communication network and provide flexibility to the developer to develop applications.
* The solution is available for BLE product family. 
*
* \subsection supported_devices Supported devices
* 2) Supported devices:
* The solution is available for STM32WB BLE product.
*
* \subsection install_sec Installation
* 3) Installation:
* To use ST BLE-Mesh Library in any application the following should be done:
* - \a libBle_Mesh_CM4_IAR.a file should be linked to the application for ST32WB for IAR or Keil Compiler
*
*
* Proper operation of ST BLE-Mesh Library requires:
* - Call BLEMesh_Init() function before any calls to library functions
* - Call BLEMesh_Process() on each iteration in application main loop
*
* \subsection brief_descr_sec API brief description
* 4) API brief description:
* ST BLE-Mesh Library sends and receives data from remote devices by itself when 
* required and provides data to the user application by calling the appropriate callbacks.
*
* User application operation:
* - User application defines a callback map (\a MOBLE_VENDOR_CB_MAP)
* - The callback map is provided to ST BLE-Mesh Library in BLEMesh_SetVendorCbMap() 
*                                                          function
* - The callbacks from the map are invoked by the ST BLE-Mesh Library upon data 
*                                                          or request receival.
*
* \subsection other_info_sec Licensing and other Information
* 5) Licensing and other Information:
* BLE-Mesh is built over Motorola's Mesh over Bluetooth Low Energy Technology (MoBLE)
* ST has done suitable updates required for application and networking features
*/
/**
* \file ble_mesh.h
* \brief This file defines ST BLE-Mesh Solutions Bluetooth LE Mesh Library user API.
*
* This file defines ST BLE-Mesh Solutions Bluetooth LE Mesh Library user API. 
* Please refer to the desript
*/
#include <stdint.h>

/**
* \brief Output OOB Action values (provisioner)
*/
#define OUTPUT_OOB_ACTION_BIT_BLINK               (1 << 0) /**< Blink */
#define OUTPUT_OOB_ACTION_BIT_BEEP                (1 << 1) /**< Beep */
#define OUTPUT_OOB_ACTION_BIT_VIBRATE             (1 << 2) /**< Vibrate */
#define OUTPUT_OOB_ACTION_BIT_DISPLAY_NUM         (1 << 3) /**< Display Numeric */

/**
* \brief Input OOB Action values (unprovisioned node)
*/
#define INPUT_OOB_ACTION_BIT_PUSH                 (1 << 0) /**< Push */
#define INPUT_OOB_ACTION_BIT_TWIST                (1 << 1) /**< Twist */
#define INPUT_OOB_ACTION_BIT_ENTER_NUM            (1 << 2) /**< Enter Number */

#define ADDRESS_IS_UNICAST(a)   (((MOBLEINT16)(a)>0)&&((MOBLEUINT16)(a)<0x8000))
#define ADDRESS_IS_GROUP(a)     (((MOBLEINT16)(a) & 0xC000) == 0xC000)  
#define ADDRESS_IS_ALL_NODES(a) ((MOBLEUINT16)(a) == 0xFFFF)

/** \brief List of status values for responses. */
typedef enum _MOBLE_COMMAND_STATUS
{
  /** \brief Successful response
  * Returned when the packet is successively processed.
  */
  STATUS_SUCCESS = 0x00,
  
  /** \brief Invalid command response
  * Returned when the command in the packet is not supported.
  */
  STATUS_INVALID_COMMAND = 0x01,
  
  /** \brief Invalid address response
  * Returned when an address of a data element in the packet is not supported.
  */
  STATUS_INVALID_ADDRESS = 0x02,
  
  /** \brief Invalid data response
  * Returned when the data in the packet is invalid.
  */
  STATUS_INVALID_DATA = 0x03,
  
  /** \brief Device failure response
  * Returned when the device is unable to process packet.
  */
  STATUS_DEVICE_ERROR = 0x04
    
} MOBLE_COMMAND_STATUS;

/**
* This structure contains device name parameters
*/
typedef struct
{
    uint8_t deviceNameSize;/* size of device name */
    const uint8_t* pDeviceName;
} device_name_params_t;

/**
* This structure contains transmit receive parameters
*/ 
typedef struct
{
    uint16_t gapBetweenPktTransmission;/* Gap between successive packet transmission */
} tr_params_t;

/**
* This structure contains Low Power feature initialization parameters
*/ 
typedef struct
{
    uint8_t rssiFactor;
    uint8_t receiveWindowFactor;
    uint8_t minQueueSizeLog;
    uint8_t receiveDelay;
    uint32_t pollTimeout;
    uint8_t friendRequestFrequency;
    uint32_t friendPollFrequency;
    uint8_t receiveWindow;
    uint8_t subscriptionList;
    int8_t minRssi;
    uint8_t noOfRetry;
} lpn_params_t;

/**
* This structure contains Friend feature initialization parameters
*/ 
typedef struct
{   
    uint8_t noOfLpn;
} fn_params_t;

/**
* This structure contains static OOB parameters
*/ 
typedef struct
{
    uint8_t pubKeyTypeOob;  /* Used Public Key: OOB / No OOB */   
    const uint8_t *pubKey;    /* Pointer to array containing Public Key of the device */
    const uint8_t *privKey;   /* Pointer to array containing Private Key of the device*/ 
    uint8_t noOobAuthEnable;  /* No OOB Authentication Enable */  
    uint8_t staticOobSize;  /* Size of Static OOB array */
    const uint8_t *staticOob; /* Pointer to array containing Static OOB info of the device */
    uint8_t OutputOobSize;    /* Size of Output OOB value */
    uint8_t OutputOobAction;  /* Size of Output OOB Action */
    void (*OutputOOBAuthCb)(MOBLEUINT8* oobData, MOBLEUINT8 size); /* Callback to Output OOB data */
    uint8_t InputOobSize;    /* Size of Input OOB value */
    uint8_t InputOobAction;  /* Size of Input OOB Action */
    MOBLEUINT8* (*InputOOBAuthCb)(MOBLEUINT8 size); /* Callback to Input OOB data */
} prvn_params_t;

/**
* Structure contains neighbor table initialization parameters
*/ 
typedef struct
{
    uint8_t count;
    uint8_t aliveTime;
    uint8_t unprvnd_dev_beacon_ntu;
    uint8_t secure_net_beacon_ntu;
    uint8_t msg_ttlx_ntu;
} neighbor_table_init_params_t;

/**
* Structure contains neighbor parameters
*/
typedef struct
{
  MOBLEUINT8 bdAddr[6];
  MOBLEBOOL   provisioned; 
  MOBLEUINT8 uuid[16];
  MOBLE_ADDRESS networkAddress;
  MOBLEUINT8 rssi;
} neighbor_params_t;

/**
* Structure contains NetKey List
*/
typedef struct
{
  MOBLEUINT16 *netKeyIndexList;
  MOBLEUINT16 netKeyIndexList_size;
  MOBLEUINT8 status; 
  MOBLEUINT8 reserved[3];
  
}model_netKeyListParams_t;

/**
* Structure contains AppKey & NetKey Add/Delete & Update parameters
*/
typedef struct
{
 MOBLEUINT16 netKeyIndex;
 MOBLEUINT16 appKeyIndex;
 MOBLEUINT8 *netKey;
 MOBLEUINT8 *appKey;
 MOBLEUINT8 keySize;
 MOBLEUINT8 status;
 
}model_securityKeyParams_t;

/**
* Structure contains AppKey List
*/
typedef struct
{
  MOBLEUINT16 netKeyIndex;
  MOBLEUINT16 *appKeyIndexList;
  MOBLEUINT16 appKeyIndexList_size;
  MOBLEUINT8 status; 
  MOBLEUINT8 reserved;
  
}model_appKeyListParams_t;

/**
* Structure contains AppKey Bind/Unbind parameters
*/
typedef struct
{
 MOBLEUINT8 elementIndex;
 MOBLEUINT16 appKeyIndex;
 MOBLEUINT32 modelID;
 MOBLEUINT8 status;
  
}model_appKeyBindingParams_t;

/**
* Structure contains Subscription related parameters
*/
typedef struct
{
 MOBLEUINT8 elementIndex;
 MOBLEUINT32 modelID;
 MOBLE_ADDRESS subAddress;
 MOBLEUINT8 status;
 MOBLEBOOL isVirtual;
 MOBLEBOOL allSubDeleted;
 MOBLEUINT8 reserved[2]; 
}model_subParams_t;

/**
* Structure contains publication parameters of required model
*/
typedef struct
{
  MOBLEUINT8 elementAddress;
  MOBLEUINT32 modelID;
  MOBLEUINT16 publishAddress;
  MOBLEUINT16 publishAppKeyIndex;
  MOBLEUINT8 credentialFlag;
  MOBLEUINT8 publishTTL;
  MOBLEUINT8 publishPeriod;
  MOBLEUINT8 publishRetransmitCount;
  MOBLEUINT8 publishRetransmitIntervalSteps;    
  MOBLEUINT8 status;
  MOBLEUINT8 reserved;
} model_publicationparams_t;

/** \brief of Message Header structure.
  * This is the structure of message header for elementIndex, src-dst addresses,
  * TTL, RSSI, NetKey & AppKey Offset
  * \param[in] elementIndex: index of the element received from peer which is 
  *            elementNumber-1
  * \param[in] peer_addr: peer Source network address.
  * \param[in] dst_peer : Destination address set by peer.
  * \param[in] ttl: ttl of the received message.
  * \param[in] rssi: rssi of the received message.
  * \param[in] rcvdAppKeyOffset: AppKey Offset corresponding to received index.
  * \param[in] rcvdNetKeyOffset: NetKey Offset corresponding to received index.
 **/
typedef struct 
{   
    MOBLEUINT8 elementIndex;
    MOBLEUINT8 reserved[3];
    MOBLE_ADDRESS peer_addr;
    MOBLE_ADDRESS dst_peer;
    MOBLEUINT8 ttl;
    MOBLEUINT8 rssi;
    MOBLEUINT8 rcvdAppKeyOffset;
    MOBLEUINT8 rcvdNetKeyOffset;
} MODEL_MessageHeader_t;


/** \brief Callback map */
typedef struct
{
  /** \brief Write local data callback.
  * Called when the device gets a request to modify its data. Such a request is 
  * made via a call to \a BluenrgMesh_SetRemotePublication
  * on a remote device.
  * User is responsible for deserializing the data.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] offset Address of data in the data map.
  * \param[in] data Data buffer. Contains vendor-specific representation of data.
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*WriteLocalData)(MODEL_MessageHeader_t *pmsgParams,   
                                 MOBLEUINT8 offset, MOBLEUINT8 const *data, 
                                 MOBLEUINT32 length, MOBLEBOOL response);
  
  /** \brief Read local data callback.
  * Called when the device gets a request to provide its data. Such a request is 
  *         made via a call to \a _ReadRemoteData on a remote device.
  * User is responsible for serializing the data. After this callback 
  *              successfully returns, data is sent back to the requesting peer.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] offset Address of data in the data map.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*ReadLocalData)(MODEL_MessageHeader_t *pmsgParams,   
                                MOBLEUINT8 offset, MOBLEUINT8 const *data, 
                                MOBLEUINT32 length, MOBLEBOOL response);
  
  /** \brief On Response data callback.
  * Called when the device gets a request to provide its data. Such a request is 
  *         made via a call to \a Send response on a remote device.
  * User is responsible for serializing the data. After this callback 
  *              successfully returns, data is sent back to the requesting peer.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] offset Address of data in the data map.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*OnResponseData)(MODEL_MessageHeader_t *pmsgParams,  
                                 MOBLEUINT8 command, MOBLEUINT8 const *pRxData, 
                                 MOBLEUINT32 dataLength, MOBLEBOOL response);

} MOBLE_VENDOR_CB_MAP;

/** \brief Hardware function Callback map */
typedef struct 
{
  /* Structure for setting the hardware configuration by the user */
  MOBLE_RESULT (*BLE_Stack_Initialization)(void);
  
  /* Structure for setting the Tx Power by the user */
  MOBLE_RESULT (*BLE_Set_Tx_Power)(void);
  
  /*This event indicates that a new connection has been created.*/
  void (*GATT_Connection_Complete)(void);
  
  /*This event occurs when a connection is terminated*/
  void (*GATT_Disconnection_Complete)(void);
  
  /*This event occurs for a Unprovisioned Node Identification*/
  void (*Unprovision_Identify_Cb)(MOBLEUINT8 data);
  
  /* Call back function for setting UUID value by the user  
     when BLEMesh_Init() function called*/
  MOBLE_RESULT (*Set_Device_UUID) (MOBLEUINT8 *data); 
  
  /* Call back function for setting CID and PID by the user */ 
  MOBLE_RESULT (*Set_Product_Info) (MOBLEUINT8 *data); 
  
  /*This event sets the number of elements in a Node*/
  MOBLEUINT8 (*Number_Of_Capabilities_Element)(void);

  /*This event disables network layer filter to sniff all the packets*/
  MOBLEUINT8 (*Disable_Filter)(void);

} MOBLE_USER_BLE_CB_MAP;


/** \brief User Application function Callback map */
typedef struct 
{
  /* Call back function to switch on or off the LED*/
  MOBLE_RESULT (*LedStateCtrl)(MOBLEUINT16 control);  
  
} MOBLE_USER_INTF_CB_MAP;

/** \brief Config Model function Callback map */
typedef struct 
{ 
   /* Call back function to get the AppKey Add command received on Config Model*/
  void(*GetAppKeyAddParamsCb)(model_securityKeyParams_t*);
   /* Call back function to get the AppKey Delete command received on Config Model*/
  void(*GetAppKeyDeleteParamsCb)(model_securityKeyParams_t*);
   /* Call back function to get the AppKey List from Config Model*/
  void(*GetAppKeyListParamsCb)(model_appKeyListParams_t*);
   /* Call back function to get the updated AppKey from Config Model*/
  void(*GetAppKeyUpdateParamsCb)(model_securityKeyParams_t*);
  /* Call back function to get the NetKey Add command received on Config Model*/
  void(*GetNetKeyAddParamsCb)(model_securityKeyParams_t*);
  /* Call back function to get the NetKey Delete command received on Config Model*/
  void(*GetNetKeyDeleteParamsCb)(model_securityKeyParams_t*);
   /* Call back function to get the NetKey List from Config Model*/
  void(*GetNetKeyListParamsCb)(model_netKeyListParams_t*);
   /* Call back function to get the updated NetKey from Config Model*/
  void(*GetNetKeyUpdateParamsCb)(model_securityKeyParams_t*);
  /* Call back function to get the appKey binding parameters*/
  void(*GetAppKeyBindingParamsCb)(model_appKeyBindingParams_t*);
    /* Call back function to get the appKey Unbinding parameters*/
  void(*GetAppKeyUnBindingParamsCb)(model_appKeyBindingParams_t*);
    /* Call back function to get the subscription add parameters*/
  void(*GetSubAddParamsCb)(model_subParams_t*);
    /* Call back function to get the subscription delete parameters*/
  void(*GetSubDeleteParamsCb)(model_subParams_t*);
    /* Call back function to get the subscription overwrite parameters*/
  void(*GetSubOverwriteParamsCb)(model_subParams_t*);
  /* Call back function to get the Publication Parameters when Publication Set
     command received on Config Model*/
  void (*GetPublicationSetParamsCb)(model_publicationparams_t*);  
  /* Call back function to get the Publication Parameters when Publication Get
     command received on Config Model*/
  void (*GetPublicationGetParamsCb)(model_publicationparams_t*);
  
} MOBLE_CONFIG_MODEL_CB_MAP;

/** \brief of opcode table structure.
  * This is the structure of opcode of set, get and status message with the maximum 
  *              and maximum parameter value for set,get and status messages.
  * \param[in] opcode: opcode of message type.
  * \param[in] reliable :whether mesaage is acknowledge or unacknowledge. 
  * \param[in] min_payload_size: minimum length of message.
  * \param[in] max_payload_size: maximum length of message.
  * \param[in] response_opcode: opcode for status message
  * \param[in] min_response_size: minimum length of status message.
  * \param[in] max_response_size: maximum length of status message.
 **/
typedef struct 
{
    MOBLEUINT16 model_id;
    MOBLEUINT16 opcode;
    MOBLEBOOL reliable;
    MOBLEUINT8 min_payload_size;
    MOBLEUINT8 max_payload_size;
    MOBLEUINT16 response_opcode;
    MOBLEUINT8 min_response_size;
    MOBLEUINT8 max_response_size;    
} MODEL_OpcodeTableParam_t;

/** \brief Callback map */
typedef struct
{
  /** \brief Get opcode table callback.
  * This function is a callback to get status opcode form the received opcode 
  * from client side.
  * \param[in] Pointer to get the opcode table address
  * \param[in] length of the opcode.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  
  MOBLE_RESULT (*ModelSIG_GetOpcodeTableCb)(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
  
  /** \brief get message/status message process callback
  * This function called when there will acknowledged message received or Get message is
  * is received to get the status of the message.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] opcode to be processed
  * \param[in] modelID to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*ModelSIG_GetRequestCb)(MODEL_MessageHeader_t *pmsgParams, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *data, 
                                    MOBLEUINT32 *res_length,
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response); 
  
  /** \brief set message process callback
  * This function called when there will set message is received.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*ModelSIG_SetRequestCb)(MODEL_MessageHeader_t *pmsgParams, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);

} MODEL_SIG_cb_t;


/** \brief Callback map */
typedef struct
{  
  /** \brief Get opcode table callback.
  * This function is a callback to get status opcode form the received opcode 
  * from client side.
  * \param[in] Pointer to get the opcode table address
  * \param[in] length of the opcode.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
   
  MOBLE_RESULT (*ModelVendor_GetOpcodeTableCb)(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
  
  /** \brief get message/status message process callback
  * This function called when there will acknowledged message received or Get message is
  * is received to get the status of the message.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*ModelVendor_GetRequestCb)(MODEL_MessageHeader_t *pmsgParams, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *data, 
                                    MOBLEUINT32 *res_length,
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response); 
  
  /** \brief set message process callback
  * This function called when there will set message is received.
  * \param[in] *pmsgParam Pointer to structure of message header for parameters:
  *             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*ModelVendor_SetRequestCb)(MODEL_MessageHeader_t *pmsgParams, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);

} MODEL_Vendor_cb_t;

typedef struct
{
  MOBLEUINT8* pbuff_dyn ;
  const uint16_t dyn_buff_size;
  const uint16_t friend_lp_buff_size;
  const uint16_t max_appli_pkt_size;
  const uint16_t neighbor_table_buff_size;
  const uint16_t models_buff_size;
} DynBufferParam_t;


typedef struct
{
  const device_name_params_t* pDeviceNameParams;
  MOBLEUINT8* pbdaddr ;
  const tr_params_t* pTrParams;
  const fn_params_t* pFnParams;
  const lpn_params_t* pLpnParams;
  const neighbor_table_init_params_t* pNeighborTableParams;
  const uint16_t features;
  const uint8_t prvnBearer;
  const prvn_params_t* pPrvnParams;
  const DynBufferParam_t* pDynBufferParam;
} Mesh_Initialization_t;

/******************************************************************************/
/*                         BLE-Mesh stack functions                           */
/******************************************************************************/

/** \brief ST BLE-Mesh Library initialization
*
* This function should be called to initialize ST BLE-Mesh Library.
* Other ST BLE-Mesh Library functions should NOT be called until the library is initialized
*
* \param[in] bdaddr A pointer to the array with MAC address. If equals NULL then 
*                          default MAC address is used, i.e. it is not hanged.
* \param[in] features Features to be supported by library
*                     Bit0 Relay feature
*                     Bit1 Proxy feature
*                     Bit2 Friend feature
*                     Bit3 Low power feature
* \param[in] LpnParams Init values corresponding to low power node performance
* \return MOBLE_RESULT_SUCCESS on success.
*
*/
MOBLE_RESULT BLEMesh_Init(const Mesh_Initialization_t* pInit_params);

/** \brief ST BLE-Mesh Library Version
*
* This function can be called to get the latest library version.
* \return Pointer to string.
*
*/
char* BLEMesh_GetLibraryVersion(void);

/** \brief ST BLE-Mesh Library Sub Version
*
* This function can be called to get the latest library sub version.
* \return Pointer to string.
*
*/
char* BLEMesh_GetLibrarySubVersion(void);

/** \brief ST BLE-Mesh Library main task processing function
*
* This function should be called in user application main loop.
* \return MOBLE_RESULT_SUCCESS on success.
*
*/
MOBLE_RESULT BLEMesh_Process(void);

/* brief set Generic model
*\returnMOBLE_RESULT_SUCCESS
*/

/** \brief Set callback map.
* \param[in] map callback map. If NULL, nothing is done.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetVendorCbMap(MOBLE_VENDOR_CB_MAP const * map);

/** \brief Set remote data on the given peer. The usage of this API is deprecated and replaced with
*                                                     BluenrgMesh_SetRemotePublication
* User is responsible for serializing data into \a data buffer. Vendor_WriteLocalDataCb 
*                                  callback will be called on the remote device.
* \param[in] peer Destination address. May be set to MOBLE_ADDRESS_ALL_NODES to broadcast data.
* \param[in] elementIndex index of the element
* \param[in] command vendor model commands 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes.
* \param[in] response If 'MOBLE_TRUE', used to get the response. If 'MOBLE_FALSE', no response 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetRemoteData(MOBLE_ADDRESS peer, 
                                   MOBLEUINT8 elementIndex,
                                   MOBLEUINT16 command, 
                                   MOBLEUINT8 const * data, 
                                   MOBLEUINT32 length, 
                                   MOBLEBOOL response, 
                                   MOBLEUINT8 isVendor);


/** \brief Set remote publication for the given Model ID to publish Address
* User is responsible for serializing data into \a data buffer. Vendor_WriteLocalDataCb 
*                                  callback will be called on the remote device.
* \param[in] modelId ID of the model. 
* \param[in] srcAddress element Address of the Node
* \param[in] command vendor model commands 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes.
* \param[in] response If 'MOBLE_TRUE', used to get the response. If 'MOBLE_FALSE', no response 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetRemotePublication(MOBLEUINT32 modelId, 
                                          MOBLE_ADDRESS srcAddress,
                                          MOBLEUINT16 command, 
                                          MOBLEUINT8 const * data, 
                                          MOBLEUINT32 length, 
                                          MOBLEBOOL response, 
                                          MOBLEUINT8 isVendor);

/** \brief Send message for given Model ID & node Address
* User is responsible for serializing data into \a data buffer
* \param[in] srcAddress element Address of the Node
* \param[in] peerAddress address of targeted node(s)
* \param[in] opcode of message 
* \param[in] modelId ID of the model. 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes.
* \param[in] response If 'MOBLE_TRUE', used to get the response. If 'MOBLE_FALSE', no response 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_ModelSendMessage(MOBLE_ADDRESS srcAddress,
                                      MOBLE_ADDRESS peerAddress,
                                      MOBLEUINT32 modelId,
                                      MOBLEUINT16 opcode, 
                                      const MOBLEUINT8 *pData,
                                      MOBLEUINT32 length);

/** \brief Vendor Model Set remote data on the given peer.
* User is responsible for serializing data into a data buffer. 
* \param[in] peer Destination address. May be set to MOBLE_ADDRESS_ALL_NODES to broadcast data.
* \param[in] src_addr index of the element which is generating the data
* \param[in] command vendor model commands 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes.
* \param[in] response If 'MOBLE_TRUE', used to get the response. If 'MOBLE_FALSE', no response 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT Vendor_WriteRemoteData (MOBLEUINT32 vendorModelId,
                                     MOBLE_ADDRESS src_addr, 
                                     MOBLE_ADDRESS dst_peer, 
                                     MOBLEUINT8 command, 
                                     MOBLEUINT8 const *data, 
                                     MOBLEUINT32 length, 
                                     MOBLEBOOL response);
  
/** \brief Read remote data on the given peer.
* User is responsible for serializing data into \a data buffer. Vendor_ReadLocalDataCb 
*                                  callback will be called on the remote device.
*                                  It is reliable command
* \param[in] *pmsgParam Pointer to structure of message header for parameters:
*             elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* \param[in] command vendor model commands 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_ReadRemoteData(MODEL_MessageHeader_t *pmsgParam,
                                        MOBLEUINT16 command,
                                        MOBLEUINT8 const * data, 
                                        MOBLEUINT32 length);

/** \brief Send response on received packet.The usage of this API is deprecated and replaced with VendorModel_SendResponse
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] dst Source Address of Node
* \param[in] status Status of response.
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SendResponse(MOBLE_ADDRESS peer, 
                                  MOBLE_ADDRESS dst, 
                                  MOBLEUINT8 status,
                                  MOBLEUINT8 const * data, 
                                  MOBLEUINT32 length);

/** \brief Send response on received packet.
* \param[in] *pmsgParam Pointer to structure of message header for parameters:
*             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* \param[in] status Status of response.
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT VendorModel_SendResponse(MOBLEUINT16 vendorModelId, 
                                      MODEL_MessageHeader_t *pmsgParams,  
                                      MOBLEUINT8 status, 
                                      MOBLEUINT8 const * data, 
                                      MOBLEUINT32 length);

/** \brief Sensor Send response on received packet.
* \param[in] *pmsgParam Pointer to structure of message header for parameters:
*             src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* \param[in] opcode to be send
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT Model_SendResponse(MODEL_MessageHeader_t *pmsgParams,
                                MOBLEUINT16 opcode,
                                MOBLEUINT8 const *pData,
                                MOBLEUINT32 length); 

/** \brief Config Model Send message to the remote
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT ConfigModel_SendMessage(MOBLE_ADDRESS src_peer,
                                     MOBLE_ADDRESS dst_peer,
                                     MOBLEUINT16 opcode, 
                                     MOBLEUINT8 *pData,
                                     MOBLEUINT32 length,
                                     MOBLEUINT8 *pTargetDevKey); 

/** \brief Publish Send to the provisioner
* \param[in] peer Destination address is Provisioner address 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT ConfigModel_SelfPublishConfig (MOBLE_ADDRESS dst_peer,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pData,
                                                   MOBLEUINT32 length); 

/** \brief Subscription Send to the provisioner
* \param[in] peer Destination address is Provisioner address 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT ConfigModel_SelfSubscriptionConfig (MOBLE_ADDRESS dst_peer,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pData,
                                                   MOBLEUINT32 length); 

/** \brief App binding Send to the provisioner
* \param[in] peer Destination address is Provisioner address 
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/

MOBLE_RESULT ConfigClient_SelfModelAppBindConfig  (MOBLE_ADDRESS dst_peer,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pData,
                                                   MOBLEUINT32 length); 

/** \brief initialize unprovisioned node to be provisioned.
* \param None
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_InitUnprovisionedNode(void);

/** \brief Enable provision procedure for provisioned node.
* \param None.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_InitProvisionedNode(void);

/** \brief Check if node configures as Unprovisioned node.
* \return MOBLE_TRUE if node configured as Unprovisioned node. MOBLE_FALSE otherwise.
*/
MOBLEBOOL BLEMesh_IsUnprovisioned(void);

/** \brief Unprovisions the node if it is provisioned.
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FALSE if node us already 
*                               unprovisioned, and failure code in other cases.
*/
MOBLE_RESULT BLEMesh_Unprovision(void);

/** \brief Set BLE Hardware init callback
* \param _cb callback
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FAIL if failure code.
*/
MOBLE_RESULT BLEMesh_BleHardwareInitCallBack(MOBLE_USER_BLE_CB_MAP const * _cb);

/** \brief Set Config Model init callback
* \param _cb callback
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FAIL if failure code.
*/
MOBLE_RESULT BLEMesh_BleConfigModelInitCallBack(MOBLE_CONFIG_MODEL_CB_MAP const * _cb);

/** \brief Get provisioning process state
* \return 0,1,2,3,4,5,6 during provisioning, else 7.
*/
MOBLEUINT8 BLEMesh_GetUnprovisionState(void);

/** \brief Get mesh address of a node
*
* This function gets address of a node. If node is unprovisioned then 
*                                         MOBLE_ADDRESS_UNASSIGNED is returned.
*
* \return mesh address of a node.
*
*/
MOBLE_ADDRESS BLEMesh_GetAddress(void);

/** \brief Get Publish address of a node
*
* This function gets address of a node. 
*
* \return mesh address of a node.
*
*/
MOBLE_ADDRESS BLEMesh_GetPublishAddress(MOBLEUINT8 elementIndex, 
                                        MOBLEUINT32 modelId);

/** \brief Get Publish Parameters of a node
*
* This function gets publication parameters of required model of a element
*
* \return status of the command
*
*/
MOBLE_RESULT BLEMesh_Get_PublicationParameters(model_publicationparams_t *pPublicationParams, 
                                                   MOBLEUINT8 elementIndex, MOBLEUINT32 modelId);

/** \brief Get Subscription address of a node
*
* This function gets addresses of selected element. 
*
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FAIL if failure code
*
*/
MOBLE_RESULT BLEMesh_GetSubscriptionAddress(MOBLE_ADDRESS *addressList, 
                                                     MOBLEUINT8 *sizeOfList, 
                                                     MOBLEUINT8 elementIndex,
                                                     MOBLEUINT32 modelId);


/** \brief Check Subscription address of a element for a model
*
* This function checks subscription of selected element. 
*
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FAIL if failure code
*
*/
MOBLE_RESULT BLEMesh_ModelsCheckSubscription(MOBLE_ADDRESS dst_peer,
                                             MOBLEUINT32 modelID, 
                                             MOBLEUINT8 elementIndex);

/** \brief Set default TTL value.
* When message is sent to mesh network, it contains TTL field. User shall call 
* this function to set TTL value used during message transmission.
* \param[in] ttl TTL value. Supported values are 0-127.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetTTL(MOBLEUINT8 ttl);

/** \brief Get default TTL value.
* \return Default TTL value.
*/
MOBLEUINT8 BLEMesh_GetTTL(void);

/** \brief Set Network Transmit Count value.
* When message is sent to mesh network, it is replicated NetworkTransmitCount 
* + 1 times. User shall call this function to set Network Transmit value used 
* during message transmission.
* \param[in] count Network Transmit value. Supported values are 1-8.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetNetworkTransmitCount(MOBLEUINT8 count);

/** \brief Get Network Transmit Count value.
* \return Default Network Transmit Count value.
*/
MOBLEUINT8 BLEMesh_GetNetworkTransmitCount(void);

/** \brief Set Relay Retransmit Count value.
* When message is relayed by mesh network relay, it is replicated 
* RelayRetransmitCount + 1 times. User shall call this function to set Relay 
* Retransmit value used during message transmission.
* \param[in] count Relay Retransmit value. Supported values are 1-8.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetRelayRetransmitCount(MOBLEUINT8 count);

/** \brief Get Relay Retransmit Count value.
* \return Default Relay retransmit Count value.
*/
MOBLEUINT8 BLEMesh_GetRelayRetransmitCount(void);

/** \brief Enable or disable relay feature. Feature can be changed only if it is supported 
*          0 - disable, 1 - enable
*          Results in state disparity between Server and Client
* \return MOBLE_RESULT_FALSE if no change occur
*         MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT BLEMesh_SetRelayFeatureState(MOBLEUINT8 state);

/** \brief Enable or disable proxy feature. Feature can be changed only if it is supported 
*          0 - disable, 1 - enable
*          Results in state disparity between Server and Client
* \return MOBLE_RESULT_FALSE if no change occur
*         MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT BLEMesh_SetProxyFeatureState(MOBLEUINT8 state);

/** \brief Enable or disable friend feature. Feature can be changed only if it is supported 
*          0 - disable, 1 - enable
*          Results in state disparity between Server and Client
* \return MOBLE_RESULT_FALSE if no change occur
*         MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT BLEMesh_SetFriendFeatureState(MOBLEUINT8 state);

/** \brief Disable low power feature only if it is supported and enabled
*          0 - disable, low power feature can't be enabled using BluenrgMesh_SetLowPowerFeatureState
*          Results in state disparity between Server and Client
* \return MOBLE_RESULT_FALSE if no change occur
*         MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT BLEMesh_SetLowPowerFeatureState(MOBLEUINT8 state);

/** \brief Get features state
*          Bit0: Relay feature. 0 - disabled, 1 - enabled
*          Bit1: Proxy feature. 0 - disabled, 1 - enabled
*          Bit2: Friend feature. 0 - disabled, 1 - enabled
*          Bit3: Low Power feature. 0 - disabled, 1 - enabled
* \return Features state
*/
MOBLEUINT16 BLEMesh_GetFeatures(void);

/** \brief Get status of transmission in process state.
* \return  Status of the packet transmission.
*/
MOBLEBOOL BLEMesh_TrsptIsBusyState(void);

/** \brief Set callback for handling heartbeat messages.
*
* \param[in] cb Callback
* \return MOBLE_RESULT_SUCCESS on success.
*
*/
MOBLE_RESULT BLEMesh_SetHeartbeatCallback(MOBLE_HEARTBEAT_CB cb);

/** \brief Set callback for attention timer.
* To be used for attention during provisioning and for health model
* For devices who want to implement actions corresponding to attention timer, set callback else do not set callback
* \param[in] cb Callback
* \return MOBLE_RESULT_SUCCESS on success.
*
*/
MOBLE_RESULT BLEMesh_SetAttentionTimerCallback(MOBLE_ATTENTION_TIMER_CB cb);

/** \brief Unprovision callback
* Callback on unprovision by provisioner
* \param[in] Unprovisioning reason
*
*/
void BLEMesh_UnprovisionCallback(MOBLEUINT8 reason);

/* EME: configuration complete callback to application */
/** \brief Provision callback
* Callback on Provision by provisioner
*
*/
void BLEMesh_ProvisionCallback(void);

/** \brief Configuration callback
* Callback on Configuration by provisioner
*
*/
void BLEMesh_ConfigurationCallback(void);

/** \brief Call back function called when PB-ADV link Opened 
* Callback on Provision by provisioner
*
*/
void BLEMesh_PbAdvLinkOpenCb(void);

/** \brief Call back function called when PB-ADV link Closed   
* Callback on Provision by provisioner
*
*/
void BLEMesh_PbAdvLinkCloseCb(void);

/** \brief Provisioning of a node from Provisioner 
* \param[in] UUID of the Unprovisioned node
*
*/
MOBLE_RESULT BLEMesh_ProvisionRemote(MOBLEUINT8 uuid[16]);

/** \brief Creates credentials for Provisioner 
*
*/
MOBLE_RESULT BLEMesh_CreateNetwork(MOBLEUINT8 *devKey);

/** \brief Set SIG Model callback map.
* \param[in] map callback map. If NULL, nothing is done.
* \count[in] count of the number of models defined in Application
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetSIGModelsCbMap(const MODEL_SIG_cb_t* pSig_cb, 
                                       MOBLEUINT32 count);

/** \brief GetApplicationVendorModels
* \param[in] map callback map. If NULL, nothing is done.
* \count[in] count of the number of models defined in Application
* \return MOBLE_RESULT_SUCCESS on success.
*/
void GetApplicationVendorModels(const MODEL_Vendor_cb_t** pModelsTable, 
                                MOBLEUINT32* VendorModelscount);

/** \brief Returns sleep duration.
* going to sleep (or no call to BLEMesh_Process()) for this duration does not affect operation of mesh library
* \return Sleep duration.
*/
MOBLEUINT32 BLEMesh_GetSleepDuration(void);
/** \brief Upper tester data process.
* \param[in] testFunctionIndex: Test function indexes
* \param[in] testFunctionParm: Test function parameters
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_UpperTesterDataProcess(MOBLEUINT8 testFunctionIndex, 
                                            MOBLEUINT8* testFunctionParm);

/** \brief String print callback function.
* \param[in] message: To be printed string pointer 
* \return void.
*/
void BLEMesh_PrintStringCb(const char *message);
/** \brief Data print callback function.
* \param[in] data: To be printed data pointer 
* \param[in] size: Size of data to be printed data pointer 
* \return void.
*/
void BLEMesh_PrintDataCb(MOBLEUINT8* data, MOBLEUINT16 size);


/** \brief Friend node callback corresponding to friendship established with low power node.
* \param[out] address of corresponding low power node.
* \param[out] receive delay of low power node (unit ms).
* \param[out] poll timeout of low power node (unit 100ms).
* \param[out] number of elements of low power node.
* \param[out] previous friend address of low power node (can be invalid address).
*/
void BLEMesh_FnFriendshipEstablishedCallback(MOBLE_ADDRESS lpnAddress,
                                                 MOBLEUINT8 lpnReceiveDelay,
                                                 MOBLEUINT32 lpnPollTimeout,
                                                 MOBLEUINT8 lpnNumElements,
                                                 MOBLE_ADDRESS lpnPrevFriendAddress);

/** \brief Friend node callback corresponding to friendship cleared with low power node.
* \param[out] reason of friendship clear.
*             0: reserved
*             1: friend request received from existing low power node (friend)
*             2: low power node poll timeout occurred
*             3: friend clear received
* \param[out] address of corresponding low power node.
*/
void BLEMesh_FnFriendshipClearedCallback(MOBLEUINT8 reason, 
                                         MOBLE_ADDRESS lpnAddress);

/** \brief Low Power node callback corresponding to friendship established with friend node.
* \param[out] address of corresponding friend node.
*/
void BLEMesh_LpnFriendshipEstablishedCallback(MOBLE_ADDRESS fnAddress);

/** \brief Low Power node callback corresponding to friendship cleared with friend node.
* \param[out] reason of friendship clear.
*             0: reserved
*             1: No response received from friend node
* \param[out] address of corresponding friend node.
*/
void BLEMesh_LpnFriendshipClearedCallback(MOBLEUINT8 reason, 
                                          MOBLE_ADDRESS fnAddress);

/** \brief Disable continuous scan
* Applicable only to provisioned Low Power feature enabled node
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_LpnDisableScan(void);

/** \brief To synchronize flash erase with sufficient available time w.r.t. next connection event.
* \return MOBLE_TRUE if no connection exists or sufficient time is available for flash erase operation.
*/
MOBLEBOOL BLEMesh_IsFlashReadyToErase(void);

/** \brief Stop ongoing scan (if in scan mode) and advertisement (if in adv mode).
*/
void BLEMesh_StopAdvScan(void);

/** \brief Stop ongoing scan (if in scan mode) and advertisement (if in adv mode).
*/
void BLEMesh_SuspendAdvScan(void);

/** \brief Stop ongoing scan (if in scan mode) and advertisement (if in adv mode).
*/
void BLEMesh_ResumeAdvScan(void);

/** \brief Set adv interval of provisioning service, 0 value results in stop.
*          Default value: 1000 ms
*          Actual value -> interval + random(16)
* \param[in] adv interval (ms), min interval value is 100 ms
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetProvisioningServAdvInterval(MOBLEUINT16 interval);

/** \brief Set interval of unprovisioned device beacon, 0 value results in stop.
*          Default value: 1000 ms
*          Actual value -> interval + random(16)
* \param[in] interval (ms) of beacons, min interval value is 100 ms
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetUnprovisionedDevBeaconInterval(MOBLEUINT16 interval);

/** \brief Set adv interval of proxy service.
*          Default value: 1000 ms
*          Actual value -> interval + random(128)
* \param[in] adv interval (ms), min interval value is 1000 ms
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetProxyServAdvInterval(MOBLEUINT16 interval);

/** \brief Set interval of secure network beacon.
*          Default value: 10000 ms
*          Actual value -> interval + random(128)
* \param[in] interval (ms) of beacons, min interval value is 10000 ms
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetSecureBeaconInterval(MOBLEUINT16 interval);

/** \brief Set interval of custom beacon, 0 value results in stop.
*          Actual value -> interval + random(128)
* \param[in] interval (ms) of beacons, min interval value is 1000 ms
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetCustomBeaconInterval(MOBLEUINT16 interval);

/** \brief Set custom beacon data.
*          If size > 31 bytes, beacon is rejected
* \param[out] beacon data buffer. Includes length, adtype, data
* \param[out] size of buffer
*/
void BLEMesh_CustomBeaconGeneratorCallback(void* buffer, MOBLEUINT8* size);

/** \brief Callback to receive non-mesh beacons
*
* Beacons are received only if received beacon ad type is not Mesh Message, 
* Mesh Beacon or PB-ADV
* \param[out] MAC address
* \param[out] data
* \param[out] length
* \param[out] rssi
*/
void BLEMesh_CustomBeaconReceivedCallback(const MOBLEUINT8* bdAddr,
                                          const MOBLEUINT8* data,
                                          MOBLEUINT8 length,
                                          MOBLEINT8 rssi);

/** 
* @brief ApplicationGetSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetSigModelList(MOBLEUINT16* pModels_sig_ID,
                                      MOBLEUINT8 elementIndex);

/** 
* @brief ApplicationGetCLIENTSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetCLIENTSigModelList(MOBLEUINT16* pModels_sig_ID, 
                                            MOBLEUINT8 elementIndex);

/** 
* @brief BLEMeshSetSelfModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param Node: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 BLEMeshSetSelfModelList(MOBLEUINT8 numberOfElements);

/** 
* @brief ApplicationGetVendorModelList: This function provides the list of the 
*           Vendor Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with Vendor Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the Vendor Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetVendorModelList(MOBLEUINT32* pModels_vendor_ID,
                                         MOBLEUINT8 elementIndex);

/** 
* @brief ApplicationChkSigModelActive: This function checks if a specific 
*          Model Server is active in the Model Server list
* @param modelID: Model Server ID received for the checking function
* @param elementIndex : index of element for which active model checking is needed
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkSigModelActive(MOBLEUINT16 modelID, 
                                       MOBLEUINT8 elementIndex);


/** 
* @brief ApplicationChkVendorModelActive: This function checks if a specific 
*          Model Server is active in the Vendor Model Server list
* @param modelID: Model Server ID received for the checking function
* @param elementIndex : index of element for which active model checking is needed
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkVendorModelActive(MOBLEUINT32 modelID, 
                                          MOBLEUINT8 elementIndex);

/** 
* @brief ApplicationGetConfigServerDeviceKey: This function provides the 
         device key to the node from Application 
* @param modelID: Model Server ID received for the checking function
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src, 
                                                 const MOBLEUINT8 **ppkeyTbUse);

/** \brief New neighbor appeared callback in neighbor table.
* \param[out] MAC address of neighbor.
* \param[out] is neighbor provisioned or unprovisioned device.
* \param[out] uuid of neighbor. NULL if not available
* \param[out] network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
* \param[out] last updated rssi value.
*/
void BLEMesh_NeighborAppearedCallback(const MOBLEUINT8* bdAddr,
                                      MOBLEBOOL provisioned,
                                      const MOBLEUINT8* uuid,
                                      MOBLE_ADDRESS networkAddress,
                                      MOBLEINT8 rssi);

/** \brief Existing neighbor refreshed callback in neighbor table.
* \param[out] MAC address of neighbor.
* \param[out] is neighbor provisioned or unprovisioned device.
* \param[out] uuid of neighbor. NULL if not available
* \param[out] network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
* \param[out] last updated rssi value.
*/
void BLEMesh_NeighborRefreshedCallback(const MOBLEUINT8* bdAddr,
                                       MOBLEBOOL provisioned,
                                       const MOBLEUINT8* uuid,
                                       MOBLE_ADDRESS networkAddress,
                                       MOBLEINT8 rssi);

/** \brief Get neighbor table status.
* \param[in] pointer to application buff, it will be updated with neighbor table parameters.
* \param[in] reference to a variable which will be updated according to number of entries in neighbor table.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_GetNeighborState(neighbor_params_t* pNeighborTable, 
                                      MOBLEUINT8* pNoOfNeighborPresent);

/** \brief Set system faults. Will be used by Health Model. Supporting All Bluetooth assigned FaultValues. 
* \param[in] pFaultArray FaultValue Array pointer. (FaultValue Range: 0x01–0x32)   
* \param[in] faultArraySize Size of the fault array. Max supported array size is 5. 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetFault(MOBLEUINT8 *pFaultArray, 
                              MOBLEUINT8 faultArraySize);

/** \brief Clears already set system faults. Will be used by Health Model.  
* \param[in] pFaultArray Fault Array pointer  
* \param[in] faultArraySize Size of the fault array. Max supported array size is 5.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_ClearFault(MOBLEUINT8 *pFaultArray, 
                                MOBLEUINT8 faultArraySize);

/** \brief Get the Number Of Elements.  
* \param[in] none
* \return Number of Elements supported by Library
*/
MOBLEUINT8 BLEMesh_GetNumberOfElements(void);

/** \brief Get the Models Count
* \param[in] none
* \return Number Of Models per element supported by Node
*/
MOBLEUINT8 BLEMesh_GetTotalModelsCount(void);

/** \brief Get the Subscription Count per Model
* \param[in] none
* \return Number Of Subscription supported per Model of an Element
*/
MOBLEUINT8 BLEMesh_GetSubscriptionCount(void);

/** \brief Get the App Keys Count  
* \param[in] none
* \return Number of App Keys supported by Node
*/
MOBLEUINT8 BLEMesh_GetAppKeysCount(void);

/** \brief Get Net Keys Count  
* \param[in] none
* \return Number of Net Keys supported by Node
*/
MOBLEUINT8 BLEMesh_GetNetKeysCount(void);

/** \brief Bluetooth LE Mesh Library shutdown
*
* This function should be called to shutdown Bluetooth LE Mesh Library
* To resume the operation, \a BluenrgMesh_Resume should be called.
* \return MOBLE_RESULT_FAIL if already shut down, MOBLE_RESULT_SUCCESS otherwise.
*/
MOBLE_RESULT BLEMesh_Shutdown(void);

/** \brief Restore Bluetooth LE Mesh Library after shutdown
*
* This function should be called to restore previously shutdown Bluetooth LE Mesh Library
* in order to resume library operation.
* \return MOBLE_RESULT_FAIL if already up and running, MOBLE_RESULT_SUCCESS otherwise.
*/
MOBLE_RESULT BLEMesh_Resume(void);

/** \brief Get the Buffer for Mesh Model data Received
*
* This function should be called to Get the buffer from the Mesh Models
* \return MOBLE_RESULT_FAIL if NOT available, MOBLE_RESULT_SUCCESS if available
*/

#ifdef STATIC_MEMORY_ALLOCATION_IN_APPLI
void* GetMemoryDataBuffer(MOBLEUINT8 type, MOBLEUINT32 len);
#endif




#define MESH_MODEL_BUFFER 1
#define MESH_MODEL_RESPONSE_BUFFER 2
#define MESH_LOWER_TPT_BUFFER 3
#define MESH_LOWER_TPT_FN_BUFFER 4
#define MESH_LOWER_TPT_APP_BUFFER 5
#define VENDOR_MODEL_WRITE_BUFFER 6
#define VENDOR_MODEL_WRITE_PUBLISHBUFFER 7
#define VENDOR_MODEL_RESPONSE_BUFFER 8
#define GENERIC_MODEL_REPLY_BUFFER 9
#define GENERIC_MODEL_PUBLISH_BUFFER 10
#define GENERIC_MODEL_SENDREMOTE_BUFFER 11
#define GENERIC_MODEL_SENDDATA_BUFFER 12
#define HEALTH_MODEL_PUBLISH_BUFFER 13
#define HEALTH_MODEL_NEW_BUFFER 14
#define CONFIG_MODEL_PUBLISH_BUFFER 15
#define PROVISIONER_BUFFER 16
#define PROVISION_NODE_BUFFER 17
#define ACCESS_APPLI_BUFFER 18
#define MESH_LOWER_TPT_INSEG 19
#define MESH_LOWER_TPT_INSEQ0 20
#define MESH_LOWER_TPT_OUTMSG 21



#endif /* __BLE_MESH_ */



