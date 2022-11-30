/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_config_client.c
  * @author  MCD Application Team
  * @brief   Application interface for Config CLient Mesh Model
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "types.h"
#include "appli_generic.h"
#include "appli_light.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "mesh_cfg.h"
#include "appli_nvm.h"
#include "config_client.h"
#include "appli_config_client.h"
#include "appli_mesh.h"
#include "sensors.h"
#include "light_lc.h"
#include "vendor.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DEFAULT_GROUP_ADDR   0xC000
#define DEFAULT_PUBLISH_ADDR 0xC000
#define DEFAULT_NETKEY_INDEX 0x0000
#define DEFAULT_APPKEY_INDEX 0x0000
#define DEFAULT_CREDENTIAL_FLAG 0x00
#define DEFAULT_PUBLISH_TTL   0x08
#define DEFAULT_PUBLISH_PERIOD   0x00
#define DEFAULT_PUBLISH_RETRANSMIT_COUNT 0x00   
#define DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS 0x00

#define NUM_VENDOR_MODELS_TO_SUBSCRIBE 0
#define NUM_VENDOR_MODELS_TO_PUBLISH 0
#define NUM_VENDOR_MODELS_TO_BIND_APP 0

/******************************************************************************/
#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)
/******************************************************************************/

const MOBLEUINT8 aConfigAppKeyDefault[19]= 
                { /* NetKeyIndexAndAppKeyIndex : 3B
                Index of the NetKey and index of the AppKey*/
                (MOBLEUINT8)(DEFAULT_NETKEY_INDEX & 0x00ff), 
                (MOBLEUINT8)((DEFAULT_NETKEY_INDEX & 0x0f00) >> 8) | (MOBLEUINT8) ((DEFAULT_APPKEY_INDEX & 0x000f) << 4), 
                (MOBLEUINT8) ((DEFAULT_APPKEY_INDEX >>4) & 0xff), 
                /* AppKey is initialised as below = 16B */
                 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
                 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };


const MOBLEUINT8 aConfigAppKeyDeleteDefault[3]= 
                { /* NetKeyIndexAndAppKeyIndex : 3B
                Index of the NetKey and index of the AppKey*/
                (MOBLEUINT8)(DEFAULT_NETKEY_INDEX & 0x00ff), 
                (MOBLEUINT8)((DEFAULT_NETKEY_INDEX & 0x0f00) >> 8) | (MOBLEUINT8) ((DEFAULT_APPKEY_INDEX & 0x000f) << 4), 
                (MOBLEUINT8) ((DEFAULT_APPKEY_INDEX >>4) & 0xff)};


const MOBLEUINT8 aConfigAppKeyGetDefault[2]= 
                { /* NetKeyIndex : 2B
                Index of the NetKey */
                (MOBLEUINT8)(DEFAULT_NETKEY_INDEX & 0x00ff), 
                (MOBLEUINT8)((DEFAULT_NETKEY_INDEX & 0x0f00) >> 8) };

const MOBLEUINT8 aNoParamDefaultConfig;

const MOBLEUINT8 aNoInitParamDefault[MAX_CONFIG_PARAM_SIZE]= {0};

/* Private macro -------------------------------------------------------------*/

const MODEL_CONFIG_CLIENT_OpcodeTableParam_t ConfigClient_MessageOpcodes_Table[] = {
  /*    MOBLEUINT16 opcode, 
        MOBLEUINT8 min_payload_size, 
        MOBLEUINT8 max_payload_size;
  Here in this array, Handler is not defined; */

  /* 4.3.2.37 Config AppKey Add, Opcode = 0x00
     The Config AppKey Add is an acknowledged message used to add an AppKey 
       to the AppKey List on a node and bind it to the NetKey identified by 
       NetKeyIndex. The added AppKey can be used by the node only as a pair with 
       the specified NetKey. 
    The AppKey is used to authenticate and decrypt messages it receives, as well 
      as authenticate and encrypt messages it sends.
    The response to a Config AppKey Add message is a Config AppKey Status message.

    message parameters:
    -------------------
    NetKeyIndexAndAppKeyIndex: 3B : Index of the NetKey and index of the AppKey
    AppKey 16B : AppKey value */
  { OPCODE_CONFIG_APPKEY_ADD,    19, 19, aConfigAppKeyDefault },  

  /* 4.3.2.39 Config AppKey Delete, Opcode = 0x80 0x00
     The Config AppKey Delete is an acknowledged message used to delete an 
       AppKey from the AppKey List on a node. 
     The response to a Config AppKey Delete message is a 
       Config AppKey Status message.

    message parameters:
    -------------------
    NetKeyIndexAndAppKeyIndex : 3B : Index of the NetKey and index of the AppKey
   */
  { OPCODE_CONFIG_APPKEY_DELETE,  3,  3, aConfigAppKeyDeleteDefault },
  
  /* 4.3.2.41 Config AppKey Get, Opcode = 0x80 0x01
     The AppKey Get is an acknowledged message used to report all AppKeys bound 
       to the NetKey.
     The response to a Config AppKey Get message is a Config AppKey List message */
  { OPCODE_CONFIG_APPKEY_GET,     2,  2, aConfigAppKeyGetDefault }, 
  
  /* 4.3.2.38 Config AppKey Update, Opcode = 0x01
     The Config AppKey Update is an acknowledged message used to update an 
     AppKey value on the AppKey List on a node. The updated AppKey is used by 
     the node to authenticate and decrypt messages it receives, as well as 
     authenticate and encrypt messages it sends, as defined by the Key Refresh procedure (see Section 3.10.4).
     The response to an Config AppKey Update message is an Config AppKey Status message. 

    message parameters:
    -------------------
    NetKeyIndexAndAppKeyIndex: 3B : Index of the NetKey and index of the AppKey
    AppKey 16B : AppKey value */
  { OPCODE_CONFIG_APPKEY_UPDATE, 19, 19, aConfigAppKeyDefault },

    /* 4.3.2.1 Config Beacon Get, Opcode = 0x80 0x09
     The Config Beacon Get is an acknowledged message used to get the current 
     Secure Network Beacon state of a node (see Section 4.2.10).
     The response to a Config Beacon Get message is a Config Beacon Status message.
     There are no Parameters for this message. */
  { OPCODE_CONFIG_BEACON_GET, 0, 0, aNoInitParamDefault }, 

  /* 4.3.2.2 Config Beacon Set, Opcode = 0x80 0x0A
    The Config Beacon Set is an acknowledged message used to set the 
    Secure Network Beacon state of a node (see Section 4.2.10).
    The response to a Config Beacon Set message is a Config Beacon Status message.
    Beacon : 1B: New Secure Network Beacon state*/
  { OPCODE_CONFIG_BEACON_SET, 1, 1, aNoInitParamDefault }, 

  /* 4.3.2.4 Config Composition Data Get, Opcode = 0x80 0x08
     The Config Composition Data Get is an acknowledged message used to read 
       one page of the Composition Data (see Section 4.2.1).
     The response to a Config Composition Data Get message is a 
        Config Composition Data Status message. 
     Page : 1B : Page number of the Composition Data   */
  { OPCODE_CONFIG_COMPOSITION_DATA_GET, 1, 1, aNoInitParamDefault },
  
  /* 4.3.2.16 Config Model Publication Set, Opcode = 0x03
     The Config Model Publication Set is an acknowledged message used to set the
     Model Publication state (see Section 4.2.2) of an outgoing message that 
     originates from a model.
     The response to a Config Model Publication Set message is a 
        Config Model Publication Status message.
     The Config Model Publication Set message uses a single octet opcode to 
         maximize the size of a payload.

      ElementAddress : 16b : Address of the element
      PublishAddress : 16b : Value of the publish address
      AppKeyIndex : 12b : Index of the application key
      CredentialFlag : 1b : Value of the Friendship Credential Flag
      RFU : 3b : Reserved for Future Use
      PublishTTL : 8b : Default TTL value for the outgoing messages
      PublishPeriod : 8b : Period for periodic status publishing
      PublishRetransmitCount : 3b : Number of retransmissions for each published message
      PublishRetransmitIntervalSteps : 5b: Number of 50-millisecond steps between retransmissions
      ModelIdentifier: 16 or 32b: SIG Model ID or Vendor Model ID
   */
  
  { OPCODE_CONFIG_CONFIG_MODEL_PUBLICATION_SET, 11, 13, }, 

  /* 4.3.2.6 Config Default TTL Get, Opcode = 0x80 0x0C
    The Config Default TTL Get is an acknowledged message used to get the 
    current Default TTL state of a node.
    The response to a Config Default TTL Get message is a Config Default TTL Status message.
    There are no Parameters for this message.
  */
  { OPCODE_CONFIG_DEFAULT_TTL_GET, 0, 0, aNoInitParamDefault },          
  
  /* 4.3.2.7 Config Default TTL Set, Opcode = 0x80 0x0D
     The Config Default TTL Set is an acknowledged message used to set the 
     Default TTL state of a node (see Section 4.2.7).
     The response to a Config Default TTL Set message is a 
       Config Default TTL Status message. 
     TTL : 1B : New Default TTL value*/
  { OPCODE_CONFIG_DEFAULT_TTL_SET, 1, 1, aNoInitParamDefault }, 

  /* 4.3.2.55 Config Friend Get, Opcode = 0x80 0x0F
     The Config Friend Get is an acknowledged message used to get the current 
       Friend state of a node (see Section 4.2.13).
     The response to a Config Friend Get message is a Config Friend Status message.
     There are no Parameters for this message. */
  { OPCODE_CONFIG_FRIEND_GET, 0, 0, aNoInitParamDefault }, 
  
  /* 4.3.2.56 Config Friend Set, Opcode = 0x80 0x10
     The Config Friend Set is an acknowledged message used to set the 
      Friend state of a node (see Section 4.2.13).
     The response to a Config Friend Set message is a Config Friend Status message. 
      Friend : 1B : New Friend state    */
  { OPCODE_CONFIG_FRIEND_SET, 1, 1, aNoInitParamDefault },

  /* 4.3.2.9 Config GATT Proxy Get, Opcode = 0x80 0x12
     The Config GATT Proxy Get is an acknowledged message used to get the 
     current GATT Proxy state of a node (see Section 4.2.11).
     The response to a Config GATT Proxy Get message is a 
        Config GATT Proxy Status message.
     There are no Parameters for this message. */
  { OPCODE_CONFIG_GATT_PROXY_GET, 0, 0, aNoInitParamDefault },          

  /* 4.3.2.10 Config GATT Proxy Set, Opcode = 0x80 0x12
     The Config GATT Proxy Set is an acknowledged message used to set the 
        GATT Proxy state of a node (see Section 4.2.11).
     The response to a Config GATT Proxy Set message is a Config GATT Proxy 
        Status message
     GATTProxy : 1B  : New GATT Proxy state */
  { OPCODE_CONFIG_GATT_PROXY_SET, 1, 1, aNoInitParamDefault },

    /* 4.3.2.61 Config Heartbeat Publication Get, Opcode = 0x80 0x38
    The Config Heartbeat Publication Get is an acknowledged message used to get 
    the current Heartbeat Publication state of an element (see Section 4.2.17).
    The response to a Config Heartbeat Publication Get message is a 
    Config Heartbeat Publication Status message.
    The message has no parameters.*/
  { OPCODE_CONFIG_HEARTBEAT_PUBLICATION_GET, 0, 0, aNoInitParamDefault },
  
  /* 4.3.2.62 Config Heartbeat Publication Set, Opcode = 0x80 0x39
    The Config Heartbeat Publication Set is an acknowledged message used to set 
    the current Heartbeat Publication state of an element (see Section 4.2.17).
    The response to a Config Heartbeat Publication Set message is a 
    Config Heartbeat Publication Status message. 

   Destination : 2B : Destination address for Heartbeat messages
   CountLog    : 1B : Number of Heartbeat messages to be sent
   PeriodLog   : 1B : Period for sending Heartbeat messages
   TTL         : 1B : TTL to be used when sending Heartbeat messages
   Features    : 2B : Bit field indicating features that trigger Heartbeat messages when changed
   NetKeyIndex : 2B : NetKey Index
  */
  { OPCODE_CONFIG_HEARTBEAT_PUBLICATION_SET, 9, 9, aNoInitParamDefault },
  
  /* 4.3.2.64 Config Heartbeat Subscription Get, Opcode = 0x80 0x3A
     The Config Heartbeat Subscription Get is an acknowledged message used to get  
     the current Heartbeat Subscription state of an element (see Section 4.2.18).
     The response to a Config Heartbeat Subscription Get message is a 
     Config Heartbeat Subscription Status message.
   The message has no parameters. */
  { OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_GET, 0, 0, aNoInitParamDefault },
  
  /* 4.3.2.65 Config Heartbeat Subscription Set, Opcode = 0x80 0x3B
     The Config Heartbeat Subscription Set is an acknowledged message used to 
     set the current Heartbeat Subscription state of an element (see Section 4.2.18).
      The response to a Config Heartbeat Subscription Set message is a 
      Config Heartbeat Subscription Status message.

      Source : 2B : Source address for Heartbeat messages
      Destination : 2B : Destination address for Heartbeat messages
      PeriodLog : 1B : Period for receiving Heartbeat messages */
  { OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_SET, 5, 5, aNoInitParamDefault },

  /* 4.3.2.58 Config Key Refresh Phase Get, Opcode = 0x80 0x15
     The Config Key Refresh Phase Get is an acknowledged message used to get the
       current Key Refresh Phase state of the identified network key.
     The response to a Config Key Refresh Phase Get message is a 
       Config Key Refresh Phase Status message. 
     NetKeyIndex : 2B : NetKey Index*/
  { OPCODE_CONFIG_KEY_REFRESH_PHASE_GET, 2, 2, aNoInitParamDefault },
  
  /* 4.3.2.59 Config Key Refresh Phase Set, Opcode = 0x80 0x16
    The Config Key Refresh Phase Set is an acknowledged message used to set the 
    Key Refresh Phase state of the identified network key (see Section 4.2.14).
   The response to a Config Key Refresh Phase Set message is a 
      Config Key Refresh Phase Status message. 
   
   NetKeyIndex : 2B : NetKey Index 
   Transition  : 1B : New Key Refresh Phase Transition */
  { OPCODE_CONFIG_KEY_REFRESH_PHASE_SET, 3, 3, aNoInitParamDefault },

  /* 4.3.2.67 Config Low Power Node PollTimeout Get, Opcode = 0x80 0x2D
     The Config Low Power Node PollTimeout Get is an acknowledged message used 
     to get the current value of PollTimeout timer of the Low Power node within 
     a Friend node (see Section 3.6.6.1). The message is sent to a Friend node 
    that has claimed to be handling messages by sending ACKs On Behalf Of (OBO) 
    the indicated Low Power node. This message should only be sent to a node 
    that has the Friend feature supported and enabled.
    The response to a Config Low Power Node PollTimeout Get message is a 
    Config Low Power Node PollTimeout Status message. 

    LPNAddress : 2B : The unicast address of the Low Power node */
  { OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_GET, 2, 2, aNoInitParamDefault },

  
  /* 4.3.3.12 Health Attention Get
     The Health Attention Get is an acknowledged message used to get the current
     Attention Timer state of an element (see Section 4.2.9).
     The response to a Health Attention Get message is an Attention Status message.
     There are no Parameters for this message.*/
  { OPCODE_HEALTH_ATTENTION_GET, 0, 0, aNoInitParamDefault }, 
  
  /* 4.3.3.13 Health Attention Set
     The Health Attention Set is an acknowledged message used to set the 
     Attention Timer state of an element (see Section 4.2.9).
     The response to a Health Attention Set message is a 
     Health Attention Status message 
    Attention: 1B: Value of the Attention Timer state*/
  { OPCODE_HEALTH_ATTENTION_SET, 1, 1, aNoInitParamDefault }, 
  
  /* 4.3.3.14 Health Attention Set Unacknowledged
     The Health Attention Set Unacknowledged is an unacknowledged message used 
     to set the Attention Timer state of an element (see Section 4.2.9). */
  { OPCODE_HEALTH_ATTENTION_SET_UNACKNOWLEDGED, 1, 1, aNoInitParamDefault }, 
  
  

  


  /* 4.3.2.15 Config Model Publication Get
     The Config Model Publication Get is an acknowledged message used to get the
     publish address and parameters of an outgoing message that originates 
     from a model.
     The response to a Config Model Publication Get message is a 
     Config Model Publication Status message.

     ElementAddress : 2B : Address of the element
     ModelIdentifier : 2 or 4B : SIG Model ID or Vendor Model ID
  */
  { OPCODE_CONFIG_MODEL_PUBLICATION_GET, 4, 6, aNoInitParamDefault },
  
  /* 4.3.2.17 Config Model Publication Virtual Address Set
     The Config Model Publication Virtual Address Set is an acknowledged message 
     used to set the model Publication state (see Section 4.2.2) of an 
     outgoing message that originates from a model.
     The response to a Config Model Publication Virtual Address Set message is 
        a Config Model Publication Status message.

      ElementAddress : 16b : Address of the element
      PublishAddress : 128b : Value of the Label UUID publish address
      AppKeyIndex : 12b : Index of the application key
      CredentialFlag : 1b : Value of the Friendship Credential Flag
      RFU : 3b : Reserved for Future Use
      PublishTTL : 8b : Default TTL value for the outgoing messages
      PublishPeriod : 8b : Period for periodic status publishing
      PublishRetransmitCount : 3b : Number of retransmissions for each published message
      PublishRetransmitIntervalSteps : 5b: Number of 50-millisecond steps between retransmissions
      ModelIdentifier: 16 or 32b: SIG Model ID or Vendor Model ID
  */
  { OPCODE_CONFIG_MODEL_PUBLICATION_VIRTUAL_ADDRESS_SET, 25, 27, aNoInitParamDefault },
  
  /* 4.3.2.19 Config Model Subscription Add
     The Config Model Subscription Add is an acknowledged message used to add an 
     address to a Subscription List of a model (see Section 4.2.3).

      The response to a Config Model Subscription Add message is a 
      Config Model Subscription Status message.

    ElementAddress  : 2B       : Address of the element
    address     : 2B           : Value of the address
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID
*/
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_ADD, 6, 8, aNoInitParamDefault },
  
  /* 4.3.2.21 Config Model Subscription Delete
     The Config Model Subscription Delete is an acknowledged message used to 
     delete a subscription address from the Subscription List of a model (see Section 4.2.3).
      The response to a Config Model Subscription Delete message is a 
      Config Model Subscription Status message.

    ElementAddress  : 2B       : Address of the element
    address     : 2B           : Value of the address
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID
  */
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE, 6, 8, aNoInitParamDefault },
  
  /* 4.3.2.25 Config Model Subscription Delete All
     The Config Model Subscription Delete All is an acknowledged message used to 
        discard the Subscription List of a model (see Section 4.2.3).
     The response to a Config Model Subscription Delete All message is a 
        Config Model Subscription Status message. 
    ElementAddress  : 2B       : Address of the element
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID */
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE_ALL, 4, 6, aNoInitParamDefault },

  /* 4.3.2.23 Config Model Subscription Overwrite
     The Config Model Subscription Overwrite is an acknowledged message used to 
     discard the Subscription List and add an address to the 
     cleared Subscription List of a model (see Section 4.2.3).
     
     The response to a Config Model Subscription Overwrite message is a 
     Config Model Subscription Status message. 

    ElementAddress  : 2B       : Address of the element
    address     : 2B           : Value of the address
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID */
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_OVERWRITE, 6, 8, aNoInitParamDefault },

  /* 4.3.2.20 Config Model Subscription Virtual Address Add
     The Config Model Subscription Virtual Address Add is an acknowledged message 
    used to add an address to a Subscription List of a model (see Section 4.2.3).
    The response to a Config Model Subscription Virtual Address Add message is a 
    Config Model Subscription Status message. 

    ElementAddress  : 2B       : Address of the element
    Label           : 16B      : Value of the Label UUID
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID */
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_ADD, 20, 22, aNoInitParamDefault },
  
  /* 4.3.2.22 Config Model Subscription Virtual Address Delete
     The Config Model Subscription Virtual Address Delete is an acknowledged 
       message used to delete a subscription address from the 
       Subscription List of a model (see Section 4.2.3).
     The response to a Config Model Subscription Virtual Address Delete message 
     is a Config Model Subscription Status message. 
    ElementAddress  : 2B       : Address of the element
    Address         : 16B      : Value of the Label UUID
    ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID */
  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_DELETE, 20, 22, aNoInitParamDefault },
  
    /* 4.3.2.24 Config Model Subscription Virtual Address Overwrite
       The Config Model Subscription Virtual Address Overwrite is an acknowledged 
       message used to discard the Subscription List and add an address to the 
       cleared Subscription List of a model (see Section 4.2.3).
       The response to a Config Model Subscription Virtual Address Overwrite 
       message is a Config Model Subscription Status message. 
       Element Address  : 2B       : Address of the element
       Address          : 16B      : Value of the Label UUID
       ModelIdentifier  : 2B or 4B : SIG Model ID or Vendor Model ID */

  { OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDRESS_OVERWRITE, 20, 22, aNoInitParamDefault },
  
  /* 4.3.2.69 Config Network Transmit Get
     The Config Network Transmit Get is an acknowledged message used to get the 
     current Network Transmit state of a node (see Section 4.2.19).
     The response to a Config Network Transmit Get message is a Config Network 
        Transmit Status message.
    There are no Parameters for this message. */
  { OPCODE_CONFIG_NETWORK_TRANSMIT_GET, 0, 0, aNoInitParamDefault },     
  
  /* 4.3.2.70 Config Network Transmit Set
     The Config Network Transmit Set is an acknowledged message used to set the 
        Network Transmit state of a node (see Section 4.2.19).
     The response to a Config Network Transmit Set message is a Config Network 
        Transmit Status message.
     
     NetworkTransmitCount : 3b : Number of transmissions for each Network PDU originating from the node
     NetworkTransmitIntervalSteps : 5b : Number of 10-millisecond steps between transmissions 
  */
  { OPCODE_CONFIG_NETWORK_TRANSMIT_SET, 1, 1, aNoInitParamDefault }, 

  /* 4.3.2.12 Config Relay Get
     The Config Relay Get is an acknowledged message used to get the 
       current Relay (see Section 4.2.8) and Relay Retransmit (see Section 4.2.20) 
       states of a node.
     The response to a Config Relay Get message is a Config Relay Status message.
     There are no Parameters for this message.*/
  { OPCODE_CONFIG_RELAY_GET, 0, 0, aNoInitParamDefault },      
  
  /* 4.3.2.13 Config Relay Set
     The Config Relay Set is an acknowledged message used to set the Relay 
    (see Section 4.2.8) and Relay Retransmit (see Section 4.2.20) states of a node.
    The response to a Config Relay Set message is a Config Relay Status message. 
    Relay : 8 bits : Relay
    RelayRetransmitCount  : 3b : Number of retransmissions on advertising bearer for each Network PDU relayed by the node
    RelayRetransmitIntervalSteps : 5b : Number of 10-millisecond steps between retransmissions
  */
  { OPCODE_CONFIG_RELAY_SET, 2, 2, aNoInitParamDefault },      

  /* 4.3.2.27 Config SIG Model Subscription Get
     The Config SIG Model Subscription Get is an acknowledged message used to 
     get the list of subscription addresses of a model within the element. 
     This message is only for SIG Models.
     The response to a Config SIG Model Subscription Get message is a Config SIG 
     Model Subscription List message.
       Element Address  : 2B       : Address of the element
       ModelIdentifier  : 2B       : SIG Model ID  */

  { OPCODE_CONFIG_SIG_MODEL_SUBSCRIPTION_GET, 4, 4, aNoInitParamDefault },
  
  /* 4.3.2.29 Config Vendor Model Subscription Get
     The Config Vendor Model Subscription Get is an acknowledged message used to
     get the list of subscription addresses of a model within the element. 
     This message is only for Vendor Models.
     The response to a Config Vendor Model Subscription Get message is a 
     Config Vendor Model Subscription List message.
       Element Address  : 2B       : Address of the element
       ModelIdentifier  : 4B       : Vendor Model ID */

  { OPCODE_CONFIG_VENDOR_MODEL_SUBSCRIPTION_GET, 6, 6, aNoInitParamDefault },
  

  /* 4.3.3.4 Health Fault Clear
     The Health Fault Clear is an acknowledged message used to clear the current
       Registered Fault state identified by Company ID of an element 
       (see Section 4.2.15.2).
     The response to a Health Fault Clear message is a Health Fault Status message */
  { OPCODE_HEALTH_FAULT_CLEAR, 2, 2, aNoInitParamDefault },

       /* 4.3.3.3 Health Fault Clear Unacknowledged
      The Health Fault Clear Unacknowledged is an unacknowledged message used to
      clear the current Registered Fault state identified by Company ID of an 
      element (see Section 4.2.15.2).
      Company ID : 2B : 16-bit Bluetooth assigned Company Identifier */

   { OPCODE_HEALTH_FAULT_CLEAR_UNACKNOWLEDGED, 2, 2, aNoInitParamDefault },
  
  /* 4.3.3.2 Health Fault Get
     The Health Fault Get is an acknowledged message used to get the current 
     Registered Fault state identified by Company ID of an element 
     (see Section 4.2.15.2).
     The response to a Health Fault Get message is a Health Fault Status message
     Company ID : 2B : 16-bit Bluetooth assigned Company Identifier */
   { OPCODE_HEALTH_FAULT_GET, 2, 2, aNoInitParamDefault },
   
   /* 4.3.3.5 Health Fault Test
      The Health Fault Test is an acknowledged message used to invoke a 
      self-test procedure of an element. 
      The procedure is implementation specific and may result in changing the 
       Health Fault state of an element (see Section 4.2.15).
     The response to a Health Fault Test message is a Health Fault Status message.

     Test ID : 1B : Identifier of a specific test to be performed 
    Company ID : 2B : 16-bit Bluetooth assigned Company Identifier */
  { OPCODE_HEALTH_FAULT_TEST, 3, 3, aNoInitParamDefault },
   
  /* 4.3.3.6 Health Fault Test Unacknowledged
     The Health Fault Test Unacknowledged is an unacknowledged message used to 
     invoke a self-test procedure of an element. The procedure is implementation
     specific and may result in changing the Health Fault state of an element 
    (see Section 4.2.15). 

    Test ID : 1B : Identifier of a specific test to be performed 
    Company ID : 2B : 16-bit Bluetooth assigned Company Identifier */
  { OPCODE_HEALTH_FAULT_TEST_UNACKNOWLEDGED, 3, 3, aNoInitParamDefault },
  
  /* 4.3.3.8 Health Period Get
    The Health Period Get is an acknowledged message used to get the 
    current Health Fast Period Divisor state of an element (see Section 4.2.16).
    The response to a Health Period Get message is a Health Period Status message.
    There are no parameters for this message. */
  { OPCODE_HEALTH_PERIOD_GET, 0, 0, aNoInitParamDefault },
  
  /* 4.3.3.10 Health Period Set
    The Health Period Set is an acknowledged message used to set the current 
    Health Fast Period Divisor state of an element (see Section 4.2.16).
    The response to a Health Period Set message is a Health Period Status message
   
    FastPeriodDivisor : 1B: Divider for the Publish Period. 
               Modified Publish Period is used for sending Current */
  { OPCODE_HEALTH_PERIOD_SET, 1, 1, aNoInitParamDefault },
  
  /* 4.3.3.9 Health Period Set Unacknowledged
     The Health Period Set Unacknowledged is an unacknowledged message used to 
     set the current Health Fast Period Divisor state of an element 
    (see Section 4.2.16). 

   FastPeriodDivisor : 1B: Divider for the Publish Period. 
               Modified Publish Period is used for sending Current 
           Health Status messages when there are active faults to communicate */
  { OPCODE_HEALTH_PERIOD_SET_UNACKNOWLEDGED, 1, 1, aNoInitParamDefault },
   /* { OPCODE_HEALTH_PERIOD_STATUS              0x8037  */



  /* 4.3.2.46 Config Model App Bind
     The Config Model App Bind is an acknowledged message used to bind an 
      AppKey to a model.
    The response to a Config Model App Bind message is a 
      Config Model App Status message.

  ElementAddress : 2B : Address of the element
  AppKeyIndex : 2B : Index of the AppKey
  ModelIdentifier : 2 or 4: SIG Model ID or Vendor Model ID */

  { OPCODE_CONFIG_MODEL_APP_BIND, 6, 8, aNoInitParamDefault },
  
  /* 4.3.2.47 Config Model App Unbind
     The Config Model App Unbind is an acknowledged message used to remove the 
         binding between an AppKey and a model.
     The response to a Config Model App Unbind message is a Config Model App Status message. 


  ElementAddress : 2B : Address of the element
  AppKeyIndex : 2B : Index of the AppKey
  ModelIdentifier : 2 or 4: SIG Model ID or Vendor Model ID */
  { OPCODE_CONFIG_MODEL_APP_UNBIND, 6, 8, aNoInitParamDefault },
  
  /* 4.3.2.31 Config NetKey Add
    The Config NetKey Add is an acknowledged message used to add a NetKey 
       to a NetKey List (see Section 4.2.4) on a node. 
    The added NetKey is then used by the node to authenticate and decrypt messages it receives, as well as authenticate and encrypt messages it sends.
    The response to a Config NetKey Add message is a Config NetKey Status message.
    NetKeyIndex  : 2B
    NetKey       : 16B NetKey  */
  { OPCODE_CONFIG_NETKEY_ADD, 18, 18, aNoInitParamDefault },
  
  /* 4.3.2.33 Config NetKey Delete
     The Config NetKey Delete is an acknowledged message used to delete a NetKey 
     on a NetKey List from a node.
     The response to a Config NetKey Delete message is a 
        Config NetKey Status message. 
    NetKeyIndex  : 2B  */
  { OPCODE_CONFIG_NETKEY_DELETE, 2, 2, aNoInitParamDefault },

  /* 4.3.2.35 Config NetKey Get
     The Config NetKey Get is an acknowledged message used to report all NetKeys
     known to the node.
    The response to a Config NetKey Get message is a Config NetKey List message.
    There are no Parameters for this message. */
  { OPCODE_CONFIG_NETKEY_GET, 0, 0, aNoInitParamDefault },

  /* 4.3.2.32 Config NetKey Update
     The Config NetKey Update is an acknowledged message used to update a NetKey 
     on a node. The updated NetKey is then used by the node to authenticate and 
     decrypt messages it receives, as well as authenticate and encrypt messages 
     it sends, as defined by the Key Refresh procedure (see Section 3.10.4).
     The response to a Config NetKey Update message is a Config NetKey Status message. 
    NetKeyIndex  : 2B  : Index of the NetKey
    NetKey       : 16B : NetKey  */
  { OPCODE_CONFIG_NETKEY_UPDATE, 18, 18, aNoInitParamDefault},
  
  /* 4.3.2.43 Config Node Identity Get
     The Config Node Identity Get is an acknowledged message used to get the 
           current Node Identity state for a subnet (see Section 4.2.12).
       The response to a Config Node Identity Get message is a 
            Config Node Identity Status message. 

    NetKeyIndex  : 2B : Index of the NetKey */
  { OPCODE_CONFIG_NODE_IDENTITY_GET, 2, 2, aNoInitParamDefault},
  
  /* 4.3.2.44 Config Node Identity Set
    The Config Node Identity Set is an acknowledged message used to set the 
        current Node Identity state for a subnet (see Section 4.2.12).
     The response to a Config Node Identity Set message is a 
         Config Node Identity Status message. 

    NetKeyIndex : 2B : Index of the NetKey
    Identity    : 1B : New Node Identity state */
  { OPCODE_CONFIG_NODE_IDENTITY_SET, 3, 3, aNoInitParamDefault},

  /* 4.3.2.53 Config Node Reset
     The Config Node Reset is an acknowledged message used to reset a node 
       (other than a Provisioner) and remove it from the network.
     The response to a Config Node Reset message is a Config Node Reset 
          Status message.
     There are no Parameters for this message.  */
  { OPCODE_CONFIG_NODE_RESET, 0, 0, aNoInitParamDefault },
  
  /* 4.3.2.49 Config SIG Model App Get
     The Config SIG Model App Get is an acknowledged message used to request 
       report of all AppKeys bound to the SIG Model.
     The response to a Config SIG Model App Get message is a Config SIG Model 
     App List message. 

     ElementAddress  : 2B : Address of the element
     ModelIdentifier : 2B : SIG Model ID */
  { OPCODE_CONFIG_SIG_MODEL_APP_GET, 4, 4, aNoInitParamDefault },

  /* 4.3.2.51 Config Vendor Model App Get
     The Config Vendor Model App Get is an acknowledged message used to request 
     report of all AppKeys bound to the model. This message is only for Vendor Models.
     The response to a Config Vendor Model App Get message is a Config Vendor Model App List message 

     ElementAddress  : 2B : Address of the element
     ModelIdentifier : 4B : Vendor Model ID */ 
  { OPCODE_CONFIG_VENDOR_MODEL_APP_GET, 6, 6, aNoInitParamDefault },

};
/* Private variables ---------------------------------------------------------*/

eClientSendMsgState_t eClientSendMsgState; /* Keeps the state of Sent Message */
eServerRespRecdState_t eServerRespRecdState; /* Keeps the state of Received Message */

/* Private function prototypes -----------------------------------------------*/
MOBLEUINT8 AppliConfigClient_SendMessageDefault(MOBLEUINT8 elementIdx);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  This function is callback from library after the Provisioning is 
          completed by embedded Provisioner
* @param  prvState: Provisioning State of the Node. Expecting "1" when the
                   provisioning is completed
* @retval None
*/ 
void Appli_ConfigClientStartNodeConfiguration(MOBLEUINT8 prvState)
{
  if (prvState==1 )
  {
    Appli_ConfigClient_Init();
    eClientSendMsgState = ProvisioningDone_State;
    NodeInfo.nodePrimaryAddress = GetAddressToConfigure();
  }
}


/**
* @brief  This function is Init function for the state machine of the 
          Configuration Client. The Function shall be called every time a new 
          node is provisioned 
* @param  None
* @retval None
*/ 
MOBLE_RESULT Appli_ConfigClient_Init(void) 
{
  eClientSendMsgState = ClientIdle_State; /* Init the value of state machine  */ 
  eServerRespRecdState = NodeIdle_State;  /* Init the value of state machine  */
  ConfigClient_ResetTrials();
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_ConfigClient_Process: This function is Process function and 
          shall be called from while(1) loop 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_Process(void) 
{
    
  Appli_ConfigClient_ConfigureNode();
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  This function is used to configure the Node after provisioning
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_ConfigureNode(void) 
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 nowClockTime;
    
    /* 
    State                  response                   called function
    --------------------------------------------------------------------------------
    ClientIdle_State       X                               None
    ProvisioningDone_State X                               Start the configuration
    CompositionGet_State   ConfigRespInit_State       GetComposition
    CompositionGet_State   CompositionRecd_State      ChangeThe State to next 
    AppKeyAdd_State        X                          Issue AppKey
    AppKeyAdd_State        AppkeyAck_State            ChangeThe State to next 
    AppBindModel_State     X                          Issue AppKetBind
    AppBindModel_State     AppBindModelAck_State      ChangeThe State to next 
    AddSubscription_State  X                          Issue the Subscription 
    AddSubscription_State  SubscriptionAck_State      ChangeThe State to next 
    SetPublication_State   PublicationStatus_State    Issue the Subscription 
    SetPublication_State   PublicationStatus_State    ChangeThe State to next 

  */

  /* If the Node is already configured, return from here  */
  if (eClientSendMsgState == ConfigurationDone_State) 
  {
    return result;
  }
  
  if (eClientSendMsgState == ClientIdle_State) 
  {
    /* Waiting for the Provisioning to be done before to Start the 
       node configuration procedure */
      return result;
  }
  
  if (eServerRespRecdState == NodeNoResponse_State) 
  {
    /* No Response received from Node under Provisioning for some config 
       messages. So, no need to do the trials  */
      return MOBLE_RESULT_FAIL;
  }
  
  if (eClientSendMsgState == ProvisioningDone_State) 
  {
    /* Start the node configuration procedure */
    eClientSendMsgState = CompositionGet_State;
    ConfigClient_SaveMsgSendingTime();
  }
  
  else if (eClientSendMsgState == CompositionGet_State)
  {
     nowClockTime = Clock_Time();
     if( (nowClockTime - NodeInfo.Initial_time) < CONFIGURATION_START_DELAY)
     {
       return result;
     }
     /*------------- Add the delay before to start the configuration messages */
    
    if (eServerRespRecdState == CompositionRecdCompleted_State)
    {
      eClientSendMsgState = AppKeyAdd_State;  /* Change the state to Next */
      eServerRespRecdState = NodeIdle_State;
    }
    else 
    {
      /* Continue the GetComposition servicing */
      Appli_ConfigClient_GetCompositionData();
    }
  }
  
  else if (eClientSendMsgState == AppKeyAdd_State)
  {
    if (eServerRespRecdState == AppkeyAckCompleted_State)
    {
      eClientSendMsgState = AppBindModel_State;  /* Change the send state */
      eServerRespRecdState = NodeIdle_State;
    }
    else
    {
      /* Continue the AppKeyAdd servicing */
    Appli_ConfigClient_DefaultAppKeyAdd();
  }
  }  
  
  else if (eClientSendMsgState == AppBindModel_State)
  {
    if (eServerRespRecdState == AppBindModelAckCompleted_State)
    {
      eClientSendMsgState = AddSubscription_State;  /* Change the send state */
      eServerRespRecdState = NodeIdle_State;
    }
    else
    {
       /* Continue the AppKeyBIND servicing */
    Appli_ConfigClient_DefaultAppKeyBind();
  }
  }
  
  else if (eClientSendMsgState == AddSubscription_State)
  {
    if (eServerRespRecdState == SubscriptionAckCompleted_State)
    {
      eClientSendMsgState = SetPublication_State;  /* Change the send state */
      eServerRespRecdState = NodeIdle_State;
    }
    else 
    {
      /* Continue the Subscription add servicing */
    AppliConfigClient_SubscriptionAddDefault();
    }    
  }

  else if (eClientSendMsgState == SetPublication_State)
  {
    if (eServerRespRecdState == PublicationStatusCompleted_State)
    {
      eClientSendMsgState = ConfigurationDone_State;  /* Change the send state */
      eServerRespRecdState = NodeIdle_State;
      TRACE_M(TF_CONFIG_CLIENT,"**Node is configured** \r\n");  
    }
    else 
    {
      /* Continue the Publication add servicing */
    AppliConfigClient_PublicationSetDefault();
    }
  }
  
  return result;
}


/**
* @brief  This function is called to Get the Composition data from the 
*         a node under configuration
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_GetCompositionData (void)
{
    MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    MOBLEUINT8 retry;
    MOBLEUINT16 dst_peer;
    
    switch(eServerRespRecdState)
    {
    case NodeIdle_State:

      ConfigClient_SaveMsgSendingTime();
      
      dst_peer = GetAddressToConfigure(); 
      
      /* Start the Get Composition Message */
      ConfigClient_CompositionDataGet(dst_peer);
      
      /* Switch to InProgress_State */
      eServerRespRecdState = InProgress_State;
      break;

    case CompositionRecd_State:
      /* Switch the state to next state AddAppKey_State */
      ConfigClient_ResetTrials();
      eServerRespRecdState = CompositionRecdCompleted_State;
      break;
      
    case InProgress_State:
      /* Just wait and let the messages be completed 
         or look for timeout */

      retry = ConfigClient_ChkRetries();
      
      if (retry == CLIENT_TX_RETRY_ENDS)
      {
        eServerRespRecdState = NodeNoResponse_State;
      }
      else if (retry == CLIENT_TX_TIMEOUT)
      {
        eServerRespRecdState = NodeIdle_State; /* Run next re-trial cycle again */
      }
      
      break;
      
    default:
      /* Error State */
      break;
    }
    
  return result;
}


/**
* @brief  Appli_ConfigClient_DefaultAppKeyAdd: This function is called to 
          add the default AppKeys and NetKeys to a node under configuration
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_DefaultAppKeyAdd (void)
{
   MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 retry; 
   MOBLEUINT8 *pAppKey;  
   MOBLEUINT16 netKeyIndex = DEFAULT_NETKEY_INDEX;
   MOBLEUINT16 appKeyIndex = DEFAULT_APPKEY_INDEX;
  MOBLE_ADDRESS dst_addr;

   pAppKey = GetNewProvNodeAppKey();
   
    switch(eServerRespRecdState)
    {
    case NodeIdle_State:
      ConfigClient_SaveMsgSendingTime();
      
      dst_addr = GetAddressToConfigure(); 
      
      /* Start the Set Appkey message  */
      ConfigClient_AppKeyAdd ( dst_addr,
                               netKeyIndex, 
                                  appKeyIndex,
                                  pAppKey);
      
      /* Switch to InProgress_State */
      eServerRespRecdState = InProgress_State; 
      break;

    case AppkeyAck_State:
      ConfigClient_ResetTrials();
      eServerRespRecdState = AppkeyAckCompleted_State;
      break;
      
    case InProgress_State:
      /* Just wait and let the messages be completed 
         or look for timeout */

      retry = ConfigClient_ChkRetries();
      
      if (retry == CLIENT_TX_RETRY_ENDS)
      {
        eServerRespRecdState = NodeNoResponse_State;
      }
      else if (retry == CLIENT_TX_TIMEOUT)
      {
        eServerRespRecdState = NodeIdle_State; /* Run next re-trial cycle again */
      }

      break;

    default:
      /* Error State */
      break;
}

  return result;
}


/**
* @brief  Appli_ConfigClient_DefaultAppKeyBind: This function is application used for 
          function to Bind the element(node) with AppKeyIndex and Models
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_DefaultAppKeyBind (void)
{
   static MOBLEUINT32 modelIdentifier;
   static MOBLEUINT8 elementIndex;
   static MOBLEUINT8  indexSIGmodels;
   static MOBLEUINT8  indexVendormodels;
   MOBLEUINT16 elementAddress;
   MOBLEUINT16 appKeyIndex = DEFAULT_APPKEY_INDEX;

   MOBLEUINT8  numSIGmodels;
   MOBLEUINT8  numVendorModels;
   MOBLEUINT8  numofElements;        
   MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
   MOBLEUINT8 retry;  
  
    switch(eServerRespRecdState)
    {
    case NodeIdle_State:
      {
        /* Start the AppBindModel_State message  */
        elementIndex = 0; /* Initialize it for the complete loop */
        indexSIGmodels = 0; /* Initialize it for the complete loop */
        indexVendormodels = 0;
      }

    case NodeNextSigModel_State:
      {
        numSIGmodels = GetCountSIGModelToBindApp(elementIndex);
        if(numSIGmodels > 0)
        {
          modelIdentifier = GetSIGModelToBindApp(elementIndex,
                                                 &indexSIGmodels, 
                                                 numSIGmodels);
        
          if(modelIdentifier != 0xffff)
          {
            /* Switch to NodeSendMessage_State */
            eServerRespRecdState = NodeSendMessage_State;
          }
          else
          {
            /*No SIG Models, do binding for Vendor Model */
            eServerRespRecdState = NodeNextVendorModel_State;
          }
        }
        else
        {
          /*No SIG Models, do binding for Vendor Model */
          eServerRespRecdState = NodeNextVendorModel_State;
        }
      }
      break;

    case NodeNextVendorModel_State:
      {
        numVendorModels = GetCountVendorModelToBindApp(elementIndex);
        if(numVendorModels > 0)
        {
          modelIdentifier = GetVendorModelToBindApp(elementIndex,indexVendormodels );
          /* Switch to NodeSendMessage_State */
          eServerRespRecdState = NodeSendMessage_State;
        }
        else
        {
          /* Change the received state for application  */
          eServerRespRecdState = AppBindModelAck_State;
        }
      }
      break;
      
     case NodeSendMessage_State:
       {
         /* Start the AppBindModel_State message  */
         elementAddress = GetServerElementAddress(elementIndex);

         /* Switch to InProgress_State */
         eServerRespRecdState = InProgress_State;

         ConfigClient_SaveMsgSendingTime();
        
         /* Send the Message to the server */
         ConfigClient_ModelAppBind (elementAddress, appKeyIndex, modelIdentifier);
       }
      break;

    case AppBindModelAck_State:
      {
        /* Need to check if all SIG Models are binded ? */

        ConfigClient_ResetTrials();
            
        numSIGmodels = GetCountSIGModelToBindApp(elementIndex);
        numVendorModels = GetCountVendorModelToBindApp(elementIndex);
      
        if (indexSIGmodels < numSIGmodels )
        { /* Even when all SIG Models are serviced, we need to start for Vendor Models */
          indexSIGmodels++; 
          indexVendormodels =0;  /* Reset back, bcoz, we are still process the SIG Models */
        }
        else if (indexVendormodels < numVendorModels)
        {
          indexVendormodels++; /* When SIG Models and Vendor Models are processed
                                  the loop condition will become true */
        }
      
        if (indexSIGmodels < numSIGmodels )
        {/* if index is still less, then we have scope of reading 1 more index */
       
          eServerRespRecdState = NodeNextSigModel_State;
          /* Switch to InProgress_State */
        }
        else if (indexVendormodels < numVendorModels)
        {
          /*Now, do binding for Vendor Model */
          eServerRespRecdState = NodeNextVendorModel_State;
        }
        else
        {
          /* Now, the element index is handled, change the element index */
          elementIndex++;
          numofElements = ConfigClient_GetNodeElements();  
          if (elementIndex >=  numofElements)
          {/* we are comparing Index whose counting started from 0, becomes equal, 
              then exit the loop */
          
             eServerRespRecdState = AppBindModelAckCompleted_State; 
          }
          else if (elementIndex < numofElements)
          { /* When the Element Index is still less than the total number of 
               elements in the Node: So, Restart the cycle */
            indexSIGmodels = 0; /* Initialize it for the complete loop */
            indexVendormodels = 0;
        
            eServerRespRecdState = NodeNextSigModel_State; 
          }
        }
      }
      break;
      
    case InProgress_State:
      {
        /* Just wait and let the messages be completed 
           or look for timeout */
        retry = ConfigClient_ChkRetries();
      
        if (retry == CLIENT_TX_RETRY_ENDS)
        {
          eServerRespRecdState = NodeNoResponse_State;
        }
        else if (retry == CLIENT_TX_TIMEOUT)
        {
          eServerRespRecdState = NodeSendMessage_State; /* Run next re-trial cycle again */;
        }
      }
      break;
     
    default:
      /* Error State */
      break;
    }
    
  return result;
}


/**
* @brief  AppliConfigClient_SubscriptionAddDefault: This function is application
          used for adding subscription to the element(node) for default settings
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT AppliConfigClient_SubscriptionAddDefault (void) 
{
   static MOBLEUINT32 modelIdentifier;
   static MOBLEUINT16 elementAddress;
   static MOBLEUINT8 elementIndex;
   static MOBLEUINT8  indexSIGmodels;
   static MOBLEUINT8  indexVendormodels;
   MOBLEUINT8  numSIGmodels;
   MOBLEUINT8  numVendorModels;
   MOBLEUINT8  numofElements;        
   MOBLEUINT16 address = DEFAULT_GROUP_ADDR;
   MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
   MOBLEUINT8 retry; 
      
    switch(eServerRespRecdState)
    {
    case NodeIdle_State:
      /* Start the SubscriptionAdd message  */

        elementIndex = 0; /* Initialize it for the complete loop */
        indexSIGmodels = 0; /* Initialize it for the complete loop */
        indexVendormodels = 0;

    case NodeNextSigModel_State:
        numSIGmodels = GetCountSIGModelToSubscribe(elementIndex);
        modelIdentifier = GetSIGModelToSubscribe(elementIndex,
                                               &indexSIGmodels, 
                                               numSIGmodels);

        if(modelIdentifier != 0xffff)
        {
          /* Switch to NodeSendMessage_State */
          eServerRespRecdState = NodeSendMessage_State;
        }
        else
        {
          /*No SIG Models, do binding for Vendor Model */
          eServerRespRecdState = NodeNextVendorModel_State;
        }
      break;

    case NodeNextVendorModel_State:
        numVendorModels = GetCountVendorModelToBindApp(elementIndex);
        if(numVendorModels > 0)
        {
          modelIdentifier = GetVendorModelToSubscribe(elementIndex,indexVendormodels );
          /* Switch to NodeSendMessage_State */
          eServerRespRecdState = NodeSendMessage_State;
        }
        else
        {
          /* Change the received state for application  */
          eServerRespRecdState = SubscriptionAck_State;
        }
      break;
      
    case NodeSendMessage_State:
        elementAddress = GetServerElementAddress(elementIndex);
        ConfigClient_SaveMsgSendingTime();

         /* Switch to InProgress_State */
        eServerRespRecdState = InProgress_State;        
        ConfigClient_SubscriptionAdd (elementAddress, address, modelIdentifier);
      
      break;

       
    case SubscriptionAck_State:
       /* Need to check if all SIG Models are subscribed ? */
      ConfigClient_ResetTrials();

      numSIGmodels = GetCountSIGModelToSubscribe(elementIndex);
      numVendorModels = GetCountVendorModelToSubscribe(elementIndex);
      elementAddress = GetServerElementAddress(elementIndex);
      
      if (indexSIGmodels < numSIGmodels )
      { /* Even when all SIG Models are serviced, we need to start for Vendor Models */
        indexSIGmodels++; 
        indexVendormodels =0;  /* Reset back, bcoz, we are still process the SIG Models */
      }
      else if (indexVendormodels < numVendorModels)
      {
        indexVendormodels++; /* When SIG Models and Vendor Models are processed
                                the loop condition will become true */
      }
      
      if (indexSIGmodels < numSIGmodels )
      {/* if index is still less, then we have scope of reading 1 more index */
       
        /* Get the Next Model and Bind it again till all SIG Models are binded */
        eServerRespRecdState = NodeNextSigModel_State;
        
      }
      else if (indexVendormodels < numVendorModels)
{
        eServerRespRecdState = NodeNextVendorModel_State;
      }
      else
      {
        /* Now, the element index is handled, change the element index */
        elementIndex++;
        numofElements = ConfigClient_GetNodeElements();  
        if (elementIndex ==  numofElements)
        {/* we are comparing Index whose counting started from 0, becomes equal, 
            then exit the loop */
           eServerRespRecdState = SubscriptionAckCompleted_State; 
        }
        else if (elementIndex < numofElements)
        { /* When the Element Index is still less than the total number of 
             elements in the Node: So, Restart the cycle */
        
          eServerRespRecdState = NodeNextSigModel_State; 
          indexSIGmodels =0; /* Reset the variable again for the next element */
        indexVendormodels = 0;
      }
      }
      break;
      
    case InProgress_State:
      /* Just wait and let the messages be completed 
         or look for timeout */
      retry = ConfigClient_ChkRetries();
      
      if (retry == CLIENT_TX_RETRY_ENDS)
      {
        eServerRespRecdState = NodeNoResponse_State;
      }
      else if (retry == CLIENT_TX_TIMEOUT)
      {
        eServerRespRecdState = NodeSendMessage_State; /* Run next re-trial cycle again */;
      }
      break;
      
    default:
      /* Error State */
      break;
    }
    
  return result;
}


/**
* @brief  AppliConfigClient_PublicationSetDefault: This function is application
          used for adding publication settings to the element(node) 
          for default settings
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT AppliConfigClient_PublicationSetDefault (void) 
{
  MOBLEUINT16 publishAddress = DEFAULT_PUBLISH_ADDR;
  MOBLEUINT16 appKeyIndex = DEFAULT_APPKEY_INDEX;
  MOBLEUINT8 credentialFlag = DEFAULT_CREDENTIAL_FLAG;
  MOBLEUINT8 publishTTL = DEFAULT_PUBLISH_TTL;
  MOBLEUINT8 publishPeriod = DEFAULT_PUBLISH_PERIOD;
  MOBLEUINT8 publishRetransmitCount = DEFAULT_PUBLISH_RETRANSMIT_COUNT;
  MOBLEUINT8 publishRetransmitIntervalSteps= DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS;
  
   static MOBLEUINT16 elementAddress;
   static MOBLEUINT32 modelIdentifier;
   static MOBLEUINT8 elementIndex;
   static MOBLEUINT8  indexSIGmodels;
   static MOBLEUINT8  indexVendormodels;
   MOBLEUINT8  numSIGmodels;
   MOBLEUINT8  numVendorModels;
   MOBLEUINT8  numofElements;        
   MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
   MOBLEUINT8 retry; 
      
    switch(eServerRespRecdState)
    {
    case NodeIdle_State:
      /* Start the Publication Add message  */

        elementIndex = 0; /* Initialize it for the complete loop */
        indexSIGmodels = 0; /* Initialize it for the complete loop */
        indexVendormodels = 0;

    case NodeNextSigModel_State:
        numSIGmodels = GetCountSIGModelToPublish(elementIndex);
        modelIdentifier = GetSIGModelToPublish(elementIndex,
                                               &indexSIGmodels, 
                                               numSIGmodels);
        
        if(modelIdentifier != 0xffff)
        {
          /* Switch to NodeSendMessage_State */
          eServerRespRecdState = NodeSendMessage_State;        
        }
        else
        {
          /*No SIG Models, do binding for Vendor Model */
          eServerRespRecdState = NodeNextVendorModel_State;
        }
      break;
        
    case NodeNextVendorModel_State:
        numVendorModels = GetCountVendorModelToBindApp(elementIndex);
        if(numVendorModels > 0)
        {
          modelIdentifier = GetVendorModelToPublish(elementIndex,indexVendormodels);
          /* Switch to NodeSendMessage_State */
          eServerRespRecdState = NodeSendMessage_State;
        }
        else
        {
          /* Change the received state for application  */
          eServerRespRecdState = PublicationStatus_State;
        }        
      break;

      
    case NodeSendMessage_State:
        elementAddress = GetServerElementAddress(elementIndex);

        ConfigClient_SaveMsgSendingTime();

         /* Switch to InProgress_State */
        eServerRespRecdState = InProgress_State;        
        ConfigClient_PublicationSet(elementAddress,
                              publishAddress,
                              appKeyIndex,
                              credentialFlag,
                              publishTTL,
                              publishPeriod,
                              publishRetransmitCount,
                              publishRetransmitIntervalSteps,
                              modelIdentifier);

      break;

      
    case PublicationStatus_State:
       /* Need to check if all SIG Models are subscribed ? */
      ConfigClient_ResetTrials();

      numSIGmodels = GetCountSIGModelToPublish(elementIndex);
      numVendorModels = GetCountVendorModelToPublish(elementIndex);
      elementAddress = GetServerElementAddress(elementIndex);
      
      if (indexSIGmodels < numSIGmodels )
      { /* Even when all SIG Models are serviced, we need to start for Vendor Models */
        indexSIGmodels++; 
        indexVendormodels =0;  /* Reset back, bcoz, we are still process the SIG Models */
      }
      else if (indexVendormodels < numVendorModels)
      {
        indexVendormodels++; /* When SIG Models and Vendor Models are processed
                                the loop condition will become true */
      }

      
      if (indexSIGmodels < numSIGmodels )
      {/* if index is still less, then we have scope of reading 1 more index */
        eServerRespRecdState = NodeNextSigModel_State;        
      }
      else if (indexVendormodels < numVendorModels)
      {
        eServerRespRecdState = NodeNextVendorModel_State;        
      }
      else
      {
        /* Now, the element index is handled, change the element index */
        elementIndex++;
        numofElements = ConfigClient_GetNodeElements();  

        if (elementIndex ==  numofElements)
        {/* we are comparing Index whose counting started from 0, becomes equal, 
            then exit the loop */
           eServerRespRecdState = PublicationStatusCompleted_State; 
        }
        else if (elementIndex < numofElements)
        { /* When the Element Index is still less than the total number of 
             elements in the Node: So, Restart the cycle */
        
          eServerRespRecdState = NodeNextSigModel_State; 
          indexSIGmodels =0; /* Reset the variable again for the next element */
        indexVendormodels = 0;
      }
      }
      break;
      
    case InProgress_State:
      /* Just wait and let the messages be completed 
         or look for timeout */
      retry = ConfigClient_ChkRetries();
      
      if (retry == CLIENT_TX_RETRY_ENDS)
      {
        eServerRespRecdState = NodeNoResponse_State;
      }
      else if (retry == CLIENT_TX_TIMEOUT)
      {
        eServerRespRecdState = NodeSendMessage_State; /* Run next re-trial cycle again */;
      }
      break;
      
    default:
      /* Error State */
      break;
    }

  return result;
}


/**
* @brief  AppliConfigClient_SelfPublicationSetDefault: This function is application
          used for adding publication settings to the element(node) 
          for default settings
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT AppliConfigClient_SelfPublicationSetDefault (void) 
{
  MOBLEUINT16 publishAddress = DEFAULT_PUBLISH_ADDR;
  MOBLEUINT16 appKeyIndex = DEFAULT_APPKEY_INDEX;
  MOBLEUINT8 credentialFlag = DEFAULT_CREDENTIAL_FLAG;
  MOBLEUINT8 publishTTL = DEFAULT_PUBLISH_TTL;
  MOBLEUINT8 publishPeriod = DEFAULT_PUBLISH_PERIOD;
  MOBLEUINT8 publishRetransmitCount = DEFAULT_PUBLISH_RETRANSMIT_COUNT;
  MOBLEUINT8 publishRetransmitIntervalSteps= DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS;
 
  MOBLEUINT16 elementAddress;
  MOBLEUINT32 modelIdentifier=0;
  MOBLEUINT8  elementIndex;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  for (elementIndex=0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++ )
  { 
    /*Checking for SIG Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_SIG_MODELS_MAX_COUNT;  index++)
    {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;

      modelIdentifier = (MOBLEUINT16) Appli_SIG_Models[elementIndex][index]; 
  
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else if (MODEL_IS_FOUNDATION_MODEL(modelIdentifier)) 
      {
        /* If Model is Foundation Model, then it does not need to be 
            added for Publishing */  
        
        /* Do NOTHING, let the next Model be picked */
      }

      else
      {
        ConfigClient_PublicationSet(elementAddress,
                                    publishAddress,
                                    appKeyIndex,
                                    credentialFlag,
                                    publishTTL,
                                    publishPeriod,
                                    publishRetransmitCount,
                                    publishRetransmitIntervalSteps,
                                    modelIdentifier);
      }
    }
    
#ifdef ENABLE_VENDOR_MODEL_SERVER  
    /*Checking for VENDOR Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_VENDOR_MODELS_MAX_COUNT;  index++)
  {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;

      modelIdentifier = (MOBLEUINT32) Appli_Vendor_Models[elementIndex][index]; 
    
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else
      {
    ConfigClient_PublicationSet(elementAddress,
                                publishAddress,
                                appKeyIndex,
                                credentialFlag,
                                publishTTL,
                                publishPeriod,
                                publishRetransmitCount,
                                publishRetransmitIntervalSteps,
                                modelIdentifier);
  }
    }
#endif  
  }
  return result;
}


/**
* @brief  AppliConfigClient_SelfSubscriptionSetDefault: This function is application
          used for adding Subscription settings to the element(node) 
          for default settings
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT AppliConfigClient_SelfSubscriptionSetDefault (void) 
{
  MOBLEUINT32 modelIdentifier=0;
  MOBLEUINT16 elementAddress;
  MOBLEUINT8  elementIndex;
  MOBLEUINT16 address = DEFAULT_GROUP_ADDR;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  for (elementIndex=0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++ )
  {
    /*Checking for SIG Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_SIG_MODELS_MAX_COUNT;  index++)
    {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;

      modelIdentifier = (MOBLEUINT16) Appli_SIG_Models[elementIndex][index]; 
      
      /*If limit is reaching to no models*/
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else if (MODEL_IS_FOUNDATION_MODEL(modelIdentifier)) 
      {
        /* If Model is Foundation Model, then it does not need to be 
            Subscribed */  
        
        /* Do NOTHING, let the next Model be picked */
      }
      else
      {
        ConfigClient_SubscriptionAdd (elementAddress, address, modelIdentifier);
      }
    }
  
#ifdef ENABLE_VENDOR_MODEL_SERVER  
    /*Checking for Vendor Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_VENDOR_MODELS_MAX_COUNT;  index++)
  {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;
    
      modelIdentifier = (MOBLEUINT32) Appli_Vendor_Models[elementIndex][index]; 
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else
      {
    ConfigClient_SubscriptionAdd (elementAddress, address, modelIdentifier);
  }
    }
#endif  
  }
  return result; 
}


/**
* @brief  Appli_ConfigClient_DefaultAppKeyBind: This function is application used for 
          function to Bind the element(node) with AppKeyIndex and Models
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_ConfigClient_SelfDefaultAppKeyBind (void)
{
  
  MOBLEUINT32 modelIdentifier=0;
  MOBLEUINT16 appKeyIndex = DEFAULT_APPKEY_INDEX;
  MOBLEUINT16 elementAddress;
  MOBLEUINT8  elementIndex;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  appKeyIndex = DEFAULT_APPKEY_INDEX;
  
  for (elementIndex=0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++ )
  {
    /*Checking for SIG Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_SIG_MODELS_MAX_COUNT;  index++)
    {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;
      
      modelIdentifier = (MOBLEUINT16) Appli_SIG_Models[elementIndex][index];  
 
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else if (MODEL_IS_FOUNDATION_MODEL(modelIdentifier)) 
      {
        /* If Model is Foundation Model, then it does not need to be binded 
           with AppKey */  
        
        /* Do NOTHING, let the next Model be picked */
      }
      else
  {
        ConfigClient_ModelAppBind (elementAddress, appKeyIndex, modelIdentifier);
      }
    }
  
#ifdef ENABLE_VENDOR_MODEL_SERVER  
    /*Checking for VENDOR Models*/
    for (MOBLEUINT8 index=0; index < APPLICATION_VENDOR_MODELS_MAX_COUNT;  index++)
  {
      elementAddress = BLEMesh_GetAddress();
      elementAddress += elementIndex;
      
      modelIdentifier = (MOBLEUINT32) Appli_Vendor_Models[elementIndex][index]; 
      
      if(modelIdentifier == NO_MODEL_AVLBL)
      {
        break;
      }
      else
      {
    ConfigClient_ModelAppBind (elementAddress, appKeyIndex, modelIdentifier);
  }      
  }
#endif    
  }
  return result;
}

/**
* @brief  Appli_CompositionDataStatusCb: This function is callback from config 
         client middleware on reception of the response
* @param  None
* @retval MOBLE_RESULT
*/ 
void Appli_CompositionDataStatusCb(MOBLE_RESULT status)
{
   eServerRespRecdState = CompositionRecd_State;
}


/**
* @brief  Appli_AppKeyStatusCb: This function is callback from config 
         client middleware on reception of the response
* @param  None
* @retval MOBLE_RESULT
*/ 
void Appli_AppKeyStatusCb(MOBLEUINT8 status)
{
   /* Change the received state for application  */
   eServerRespRecdState = AppkeyAck_State;
}


/**
* @brief  Appli_AppBindModelStatusCb: This function is callback from config 
         client middleware on reception of the response
* @param  None
* @retval MOBLE_RESULT
*/ 
void Appli_AppBindModelStatusCb(MOBLEUINT8 status)
{
   /* Change the received state for application  */
   eServerRespRecdState = AppBindModelAck_State;
}


/**
* @brief  Appli_SubscriptionAddStatusCb: This function is callback from config 
         client middleware on reception of the response
* @param  None
* @retval MOBLE_RESULT
*/ 
void Appli_SubscriptionAddStatusCb(MOBLEUINT8 status)
{
   /* Change the received state for application  */
   eServerRespRecdState = SubscriptionAck_State;
}


/**
* @brief  Appli_PublicationStatusCb: This function is callback from config 
         client middleware on reception of the response
* @param  None
* @retval MOBLE_RESULT
*/ 
void Appli_PublicationStatusCb(MOBLEUINT8 status)
{
   /* Change the received state for application  */
   eServerRespRecdState = PublicationStatus_State;
}


/**
* @brief  Appli_NodeResetStatusCb: This function is callback from config 
         client middleware on reception of the Node Reset response
* @param  None
* @retval None
*/ 
void Appli_NodeResetStatusCb(void)
{
   /* Change the received state for application  */
   eServerRespRecdState = NodeResetStatus_State;
}


/**
* @brief  GetSIGModelToBindApp: This function gets the SIG Model to Bind 
* @param  elementIndex Index of the Element
* @param  pModelIndex  Pointer to Index of the Element to be updated 
* @param  numberOfModels Total number of Models to be scanned
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetSIGModelToBindApp(MOBLEUINT8 elementIndex, 
                                 MOBLEUINT8 *pModelIndex, 
                                 MOBLEUINT8 numberOfModels) 
{
  
  MOBLEUINT16 model_id;
  MOBLEUINT8 index;
  MOBLEUINT8 idxSIG = *pModelIndex;
  
  for (index = idxSIG; index < numberOfModels ; index++ )
  {
    model_id = aNodeElements[elementIndex].aSIGModels[index];
    if (MODEL_IS_FOUNDATION_MODEL(model_id)) 
    {
      /* Do nothing, let the next model_if be scanned */
      model_id = 0xffff;  /* Invalid Model ID */
    }
    else
    {
      break;
    }
  }
  
  *pModelIndex = index;
  return model_id;

}


/**
* @brief  GetVendorModelToBindApp: This function gets the Vendor Model to Bind 
* @param  elementIndex Index of the Element
* @param  pModelIndex  Pointer to Index of the Element to be updated 
* @param  numberOfModels Total number of Models to be scanned
* @retval MOBLE_RESULT
*/ 
MOBLEUINT32 GetVendorModelToBindApp(MOBLEUINT8 elementIndex, MOBLEUINT8 indexModels)
{
  return aNodeElements[elementIndex].aVendorModels[indexModels];
}

/**
* @brief  GetCountSIGModelToBindApp: This function gets the NUMBER of SIG Model 
              to Bind 
* @param  elementIndex Index of the Element for which the Count is needed
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountSIGModelToBindApp(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumSIGmodels;
}


/**
* @brief  GetCountVendorModelToBindApp: This function gets the Vendor Model to Bind 
* @param  elementIndex Index of the Element for which the Count is needed
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountVendorModelToBindApp(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumVendorModels;
}


/**
* @brief  GetSIGModelToPublish: This function gets the SIG Model to Publish 
* @param  elementIndex Index of the Element
* @param  pModelIndex  Pointer to Index of the Element to be updated 
* @param  numberOfModels Total number of Models to be scanned
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetSIGModelToPublish(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pModelIndex, 
                                     MOBLEUINT8 numberOfModels)

{
  return GetSIGModelToBindApp(elementIndex, 
                              pModelIndex, 
                              numberOfModels);
}


/**
* @brief  GetVendorModelToPublish: This function gets the Vendor Model to Publish
* @param  elementIndex Index of the Element
* @param  idxSIG  Pointer to Index of the Model
* @retval MOBLE_RESULT
*/ 
MOBLEUINT32 GetVendorModelToPublish(MOBLEUINT8 elementIndex, MOBLEUINT8 idxSIG)
{
  return aNodeElements[elementIndex].aVendorModels[idxSIG];
}


/**
* @brief  GetCountSIGModelToPublish: This function gets the SIG Model to Bind 
* @param  elementIndex Index of the Element
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountSIGModelToPublish(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumSIGmodels;
}


/**
* @brief  GetCountVendorModelToPublish: This function gets the Vendor Model to Publish
* @param  elementIndex Index of the Element
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountVendorModelToPublish(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumVendorModels;
}


/**
* @brief  GetSIGModelToSubscribe: This function gets the SIG Model to Bind 
* @param  elementIndex Index of the Element
* @param  pModelIndex  Pointer to Index of the Element to be updated 
* @param  numberOfModels Total number of Models to be scanned
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetSIGModelToSubscribe(MOBLEUINT8 elementIndex, 
                                 MOBLEUINT8 *pModelIndex, 
                                 MOBLEUINT8 numberOfModels) 
{
  return GetSIGModelToBindApp(elementIndex, 
                              pModelIndex, 
                              numberOfModels);
}


/**
* @brief  GetVendorModelToSubscribe: This function Gets the Vendor Model ID for 
          Subscription
* @param  elementIndex Index of the Element
* @param  idxSIG  Index to the Model ID to Subscribe
* @retval MOBLE_RESULT
*/ 
MOBLEUINT32 GetVendorModelToSubscribe(MOBLEUINT8 elementIndex, 
                                      MOBLEUINT8 idxSIG)
{
  return aNodeElements[elementIndex].aVendorModels[idxSIG];
}


/**
* @brief  GetCountSIGModelToSubscribe: This function gets the SIG Model to Subscribe 
* @param  elementIndex Index of the Element
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountSIGModelToSubscribe(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumSIGmodels;
}


/**
* @brief  GetCountVendorModelToPublish: This function gets the Count of the 
           Vendor Model to Subscribe
* @param  elementIndex Index of the Element
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetCountVendorModelToSubscribe(MOBLEUINT8 elementIndex)
{
  return aNodeElements[elementIndex].NumVendorModels;
}


/**
* @brief  AppliConfigClient_SendMessageDefault: This function is used for sending
          a message. 
          ** The Function is not yet written fully
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 AppliConfigClient_SendMessageDefault(MOBLEUINT8 elementIdx)
{
  return NUM_VENDOR_MODELS_TO_SUBSCRIBE; 
}

/******************************************************************************/
#endif /* defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER) */
/******************************************************************************/

/**
* @}
*/

/**
* @}
*/


