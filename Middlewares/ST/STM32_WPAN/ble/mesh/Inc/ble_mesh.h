/**
******************************************************************************
* @file    ble_mesh.h
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
* @brief   Header file for the BLE-Mesh stack 
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
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_MESH_
#define _BLE_MESH_

#include "types.h"

#define BLE_MESH_APPLICATION_VERSION "1.09.000"
/**
* \mainpage ST BLE-Mesh Solutions Bluetooth LE Mesh Library
*
* \version 1.08.000
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
* - Call BLEMesh_Process() on each iteraction in application main loop
*
* \subsection brief_descr_sec API brief description
* 4) API brief description:
* ST BLE-Mesh Library sends and receives data from remote devices by itself when 
* required and provides data to the user applicaton by calling the appropriate callbacks.
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
//#include "types.h"

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
    uint8_t staticOobSize;  /* Size of Static OOB array */
    const uint8_t *staticOob; /* Pointer to array containing Static OOB info of the device */
} unprov_node_info_params_t;

/** \brief Callback map */
typedef struct
{
  /** \brief Write local data callback.
  * Called when the device gets a request to modify its data. Such a request is 
  * made via a call to \a BLEMesh_SetRemoteData on a remote device.
  * User is responsible for deserializing the data.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] offset Address of data in the data map.
  * \param[in] data Data buffer. Contains vendor-specific representation of data.
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*WriteLocalData)(MOBLE_ADDRESS peer, MOBLE_ADDRESS dst_peer, 
                                 MOBLEUINT8 offset, MOBLEUINT8 const *data, 
                                 MOBLEUINT32 length, MOBLEBOOL response);
  
  /** \brief Read local data callback.
  * Called when the device gets a request to provide its data. Such a request is 
  *         made via a call to \a _ReadRemoteData on a remote device.
  * User is responsible for serializing the data. After this callback 
  *              successfully returns, data is sent back to the requesting peer.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] offset Address of data in the data map.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*ReadLocalData)(MOBLE_ADDRESS peer, MOBLE_ADDRESS dst_peer, 
                                MOBLEUINT8 offset, MOBLEUINT8 const *data, 
                                MOBLEUINT32 length, MOBLEBOOL response);
  
  /** \brief On Response data callback.
  * Called when the device gets a request to provide its data. Such a request is 
  *         made via a call to \a Send response on a remote device.
  * User is responsible for serializing the data. After this callback 
  *              successfully returns, data is sent back to the requesting peer.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] offset Address of data in the data map.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*OnResponseData)(MOBLE_ADDRESS peer, MOBLEUINT8 elementIndex, 
                                 MOBLEUINT8 const * data, MOBLEUINT32 length); 

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

/** \brief of opcode table structure.
  * This is the structure of opcode of set, get and status message with the maximum 
  *              and maximum parameter value for set,get and status messages.
  * \param[in] opcode: opcode of message type.
  * \param[in] reliable :wheather mesaage is acknowledge or unacknowledge. 
  * \param[in] min_payload_size: minimum length of message.
  * \param[in] max_payload_size: maximum length of message.
  * \param[in] response_opcode: opcode for status message
  * \param[in] min_response_size: minimum length of status message.
  * \param[in] max_response_size: maximum length of status message.
 **/
typedef struct 
{
    MOBLEUINT32 opcode;
    MOBLEBOOL reliable;
    MOBLEUINT16 min_payload_size;
    MOBLEUINT16 max_payload_size;
    MOBLEUINT16 response_opcode;
    MOBLEUINT16 min_response_size;
    MOBLEUINT16 max_response_size;    
} MODEL_OpcodeTableParam_t;

/** \brief Callback map */
typedef struct
{
  /** \brief Get opcode table callback.
  * This function is a callback to get status opcode form the received opcode 
  * from client side.
  * \param[in] Pointer to get the opcode table address
  * \param[in] lenght of the opcode.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*GenericGetOpcodeTableCb)(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
   
  /** \brief get message/status message process callback
  * This function called when there will acknowleged message received or Get message is
  * is received to get the status of the message.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*GenericGetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *data, 
                                    MOBLEUINT32 *length, 
                                    MOBLEBOOL response); 
  
  /** \brief set message process callback
  * This function called when there will set message is received.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
 MOBLE_RESULT (*GenericSetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);

} MODEL_Generic_cb_t;

/** \brief Callback map */
typedef struct
{
  /** \brief Get opcode table callback.
  * This function is a callback to get status opcode form the received opcode 
  * from client side.
  * \param[in] Pointer to get the opcode table address
  * \param[in] lenght of the opcode.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  MOBLE_RESULT (*LightGetOpcodeTableCb)(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
  
  /** \brief get message/status message process callback
  * This function called when there will acknowleged message received or Get message is
  * is received to get the status of the message.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*LightGetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *data, 
                                    MOBLEUINT32 *length, 
                                    MOBLEBOOL response); 
  
  /** \brief set message process callback
  * This function called when there will set message is received.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
 MOBLE_RESULT (*LightSetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);

} MODEL_Light_cb_t;

/** \brief Callback map */
typedef struct
{
  /** \brief Get opcode table callback.
  * This function is a callback to get status opcode form the received opcode 
  * from client side.
  * \param[in] Pointer to get the opcode table address
  * \param[in] lenght of the opcode.
  * \return MOBLE_RESULT_SUCCESS on success.
  */
  
  MOBLE_RESULT (*SensorGetOpcodeTableCb)(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
  
  /** \brief get message/status message process callback
  * This function called when there will acknowleged message received or Get message is
  * is received to get the status of the message.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*SensorGetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *data, 
                                    MOBLEUINT32 *res_length,
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response); 
  
  /** \brief set message process callback
  * This function called when there will set message is received.
  * \param[in] peer Source network address.
  * \param[in] dst_peer Destination address set by peer.
  * \param[in] opcode to be processed
  * \param[in] data Data buffer. to be sent back in status
  * \param[in] length Data buffer length in bytes.
  * \param[in] response Flag if response is required.
  * \return MOBLE_RESULT_SUCCESS on success.
  */ 
  MOBLE_RESULT (*SensorSetRequestCb)(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);

} MODEL_Sensor_cb_t;

typedef struct
{
  MOBLEUINT8* pbuff_dyn ;
  const uint16_t dyn_buff_size;
  const uint16_t friend_lp_buff_size;
  const uint16_t max_appli_pkt_size;
} DynBufferParam_t;


typedef struct
{
  MOBLEUINT8* pbdaddr ;
  const tr_params_t* pTrParams;
  const fn_params_t* pFnParams;
  const lpn_params_t* pLpnParams;
  const uint16_t features;
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
*                          default MAC adress is used, i.e. it is not hanged.
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

/** \brief Set remote data on the given peer.
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
MOBLE_RESULT BLEMesh_SetRemoteData(MOBLE_ADDRESS peer, MOBLEUINT8 elementIndex,
                                       MOBLEUINT16 command, MOBLEUINT8 const * data, 
                                       MOBLEUINT32 length, MOBLEBOOL response, 
                                       MOBLEUINT8 isVendor);

/** \brief Read remote data on the given peer.
* User is responsible for serializing data into \a data buffer. Vendor_ReadLocalDataCb 
*                                  callback will be called on the remote device.
*                                  It is reliable command
* \param[in] peer Destination address. May be set to MOBLE_ADDRESS_ALL_NODES to broadcast data.
* \param[in] elementIndex index of the element
* \param[in] command vendor model commands 
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_ReadRemoteData(MOBLE_ADDRESS peer, MOBLEUINT8 elementIndex, MOBLEUINT16 command);

/** \brief Send response on received packet.
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] dst Source Address of Node
* \param[in] status Status of response.
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SendResponse(MOBLE_ADDRESS peer, MOBLE_ADDRESS dst, 
                                      MOBLEUINT8 status, MOBLEUINT8 const * data, 
                                      MOBLEUINT32 length);

/** \brief Generic Send response on received packet.
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT Generic_SendResponse(MOBLE_ADDRESS src_peer,MOBLE_ADDRESS dst_peer ,
                                          MOBLEUINT16 opcode, MOBLEUINT32 length);  

/** \brief Generic Send response on received packet.
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT Light_SendResponse(MOBLE_ADDRESS src_peer,MOBLE_ADDRESS dst_peer ,
                                          MOBLEUINT16 opcode, MOBLEUINT32 length);  

/** \brief Sensor Send response on received packet.
* \param[in] peer Destination address. Must be a device address (0b0xxx xxxx xxxx xxxx, but not 0).
* \param[in] data Data buffer.
* \param[in] length Length of data in bytes. Maximum accepted length is 8. 
*             If length is zero, no associated data is sent with the report.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT Sensor_SendResponse(MOBLE_ADDRESS src_peer,MOBLE_ADDRESS dst_peer ,
                                          MOBLEUINT16 opcode, MOBLEUINT32 length , MOBLEUINT8 const *pData);  

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

/** \brief Getting Info of unprovisioned node.
* \param None.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_UnprovisionedNodeInfo(const unprov_node_info_params_t *);

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
MOBLE_ADDRESS BLEMesh_GetPublishAddress(MOBLEUINT8 elementNumber);

/** \brief Get Subscription address of a node
*
* This function gets addresses of selected element. 
*
* \return MOBLE_RESULT_SUCCESS on success, MOBLE_RESULT_FAIL if failure code
*
*/
MOBLE_RESULT BLEMesh_GetSubscriptionAddress(MOBLE_ADDRESS *addressList, 
                                                     MOBLEUINT8 *sizeOfList, 
                                                     MOBLEUINT8 elementNumber);

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

/** \brief Set Netwrok Transmit Count value.
* When message is sent to mesh network, it is replicated NetworkTransmitCount 
* + 1 times. User shall call this function to set Netwrok Transmit value used 
* during message transmission.
* \param[in] count Network Transmit value. Supported values are 1-8.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetNetworkTransmitCount(MOBLEUINT8 count);

/** \brief Get Netwrok Transmit Count value.
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

/** \brief Provision callback
* Callback on Provision by provisioner
*
*/
void BLEMesh_ProvisionCallback(void);

/** \brief Set callback map.
* \param[in] map callback map. If NULL, nothing is done.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetModelGenericCbMap(MODEL_Generic_cb_t const * map);

/** \brief Set Lighting Model callback map.
* \param[in] map callback map. If NULL, nothing is done.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetModelLightCbMap (const MODEL_Light_cb_t* map);

/** \brief Set Sensor Model callback map.
* \param[in] map callback map. If NULL, nothing is done.
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_SetModelSensorCbMap (const MODEL_Sensor_cb_t* map);

/** \brief GenericModel_Add_Server 
* \param[in] model_id: 16bit Model Id
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT GenericModel_Add_Server( MOBLEUINT16 model_id);

/** \brief LightModel_Add_Server 
* \param[in] model_id: 16bit Model Id
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT LightModel_Add_Server( MOBLEUINT16 model_id);

/** \brief SensorModel_Add_Server 
* \param[in] model_id: 16bit Model Id
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT SensorModel_Add_Server (MOBLEUINT16 model_id);

/** \brief Returns sleep duration.
* going to sleep (or no call to BluenrgMesh_Process()) for this duration does not affect operation of mesh library
* \return Sleep duration.
*/
MOBLEUINT32 BLEMesh_GetSleepDuration(void);
/** \brief Upper tester data process.
* \param[in] testFunctionIndex: Test function indexes
* \param[in] testFunctionParm: Test function parameters
* \return MOBLE_RESULT_SUCCESS on success.
*/
MOBLE_RESULT BLEMesh_UpperTesterDataProcess(MOBLEUINT8 testFunctionIndex, MOBLEUINT8* testFunctionParm);
/** \brief Data print callback function.
* \param[in] message: To be printed data pointer 
* \return void.
*/
void BLEMesh_PrintStringCb(const char *message);


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
void BLEMesh_FnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS lpnAddress);

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
void BLEMesh_LpnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS fnAddress);

#endif /* __BLE_MESH_ */
/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

