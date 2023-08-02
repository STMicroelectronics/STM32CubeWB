/**
******************************************************************************
* @file    config_client.c
* @author  BLE Mesh Team
* @brief   Config model Client middleware file
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
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "config_client.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"
#include "appli_config_client.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "appli_nvm.h"

/** @addtogroup MODEL_CONFIG
*  @{
*/

/** @addtogroup Config_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)

/* ALIGN(4) */
__attribute__((aligned(4))) Composition_Header_Page0_t NodeCompositionPage0Header;

/* ALIGN(4) */
__attribute__((aligned(4)))Elements_Page0_t aNodeElements[CLIENT_MAX_ELEMENTS_PER_NODE];

/* ALIGN(4)*/
__attribute__((aligned(4)))NodeInfo_t NodeInfo;

const MODEL_OpcodeTableParam_t Config_Client_Opcodes_Table[] = 
{
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_CONFIG_MODEL_CLIENT

  /* 4.3.2.42 Config AppKey List, Opcode= 0x80 0x02
     The Config AppKey List is an unacknowledged message reporting all AppKeys 
     that are bound to the NetKey.*/
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_APPKEY_LIST,         MOBLE_FALSE,  5, 9, 0x8FFF , 0, 0},
  
  /* 4.3.2.40 Config AppKey Status, Opcode= 0x80 0x03
   The Config AppKey Status is an unacknowledged message used to report a status
   for the requesting message, based on the NetKey Index identifying the NetKey 
   on the NetKey List and on the AppKey Index identifying the AppKey on the 
   AppKey List. */  
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_APPKEY_STATUS,            MOBLE_FALSE,  4, 4, 0x8FFF , 0, 0},
  
  /* 4.3.2.3 Config Beacon Status, Opcode= 0x80 0x0B
    The Config Beacon Status is an unacknowledged message used to report the 
    current Secure Network Beacon state of a node (see Section 4.2.10). 

    Beacon : 1B : Secure Network Beacon state */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_BEACON_STATUS,  MOBLE_FALSE,  1, 1, 0x8FFF , 0, 0},

  /* 4.3.2.5 Config Composition Data Status, Opcode= 0x02
     The Config Composition Data Status is an unacknowledged message used to 
     report a single page of the Composition Data (see Section 4.2.1).
    This message uses a single octet opcode to maximize the size of a payload.
    Parameters: 
      Page : 1B : Page number of the Composition Data
      Data : variable : Composition Data for the identified page */
  {SIG_MODEL_ID_CONFIG_CLIENT, OPCODE_CONFIG_COMPOSITION_DATA_STATUS,  MOBLE_FALSE,  10, 100, 0x8FFF , 0, 0},
  
  /* 4.3.2.8 Config Default TTL Status, Opcode = 0x80 0x0E
     The Config Default TTL Status is an unacknowledged message used to report 
     the current Default TTL state of a node (see Section 4.2.7). 
     Parameter:
     TTL : 1B : Default TTL  */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_DEFAULT_TTL_STATUS,MOBLE_FALSE,  1, 1, 0x8FFF , 0, 0},
    
  /* 4.3.2.57 Config Friend Status, Opcode = 0x80 0x11
     The Config Friend Status is an unacknowledged message used to report the 
     current Friend state of a node (see Section 4.2.13).
     Parameter:
     Friend : 1B : Friend state */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_FRIEND_STATUS,MOBLE_FALSE,  1, 1, 0x8FFF , 0, 0},
  
  /* 4.3.2.11 Config GATT Proxy Status, Opcode = 0x80 0x14
     The Config GATT Proxy Status is an unacknowledged message used to report the 
     current GATT Proxy state of a node (see Section 4.2.11). 
     Parameter:
     GATTProxy : 1B : GATT Proxy state */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_GATT_PROXY_STATUS,MOBLE_FALSE,  1, 1, 0x8FFF , 0, 0},
  
  /* 4.3.2.63 Config Heartbeat Publication Status, Opcode = 0x06
      The Config Heartbeat Publication Status is an unacknowledged message used 
      to report the Heartbeat Publication state of a node (see Section 4.2.17).
     Parameter:
     Status : 1B : Status Code for the requesting message
     Destination : 2B : Destination address for Heartbeat messages
     CountLog : 1B : Number of Heartbeat messages remaining to be sent
     PeriodLog : 1B : Period for sending Heartbeat messages
     TTL : 1B : TTL to be used when sending Heartbeat messages
     Features : 2B : Bit field indicating features that trigger Heartbeat messages when changed
     NetKeyIndex : 2B : NetKey Index */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_HEARTBEAT_PUBLICATION_STATUS, MOBLE_FALSE,  10, 10, 0x8FFF , 0, 0},
  
  /* 4.3.2.66 Config Heartbeat Subscription Status, Opcode = 0x80 0x3C
     The Config Heartbeat Subscription Status is an unacknowledged message used 
     to report the Heartbeat Subscription state of a node (see Section 4.2.18) 
     Parameters:
      Status : 1B : Status Code for the requesting message
      Source : 2B : Source address for Heartbeat messages
      Destination : 2B : Destination address for Heartbeat messages
      PeriodLog : 1B : Remaining Period for processing Heartbeat messages
      CountLog : 1B : Number of Heartbeat messages received
      MinHops : 1B : Minimum hops when receiving Heartbeat messages
      MaxHops : 1B : Maximum hops when receiving Heartbeat messages */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_HEARTBEAT_SUBSCRIPTION_STATUS,MOBLE_FALSE,  9, 9, 0x8FFF , 0, 0},
  
  /* 4.3.2.60 Config Key Refresh Phase Status, Opcode = 0x80 0x17
     The Config Key Refresh Phase Status is an unacknowledged message used to 
     report the current Key Refresh Phase state of the identified network key 
     (see Section 4.2.14). 
    Parameters:
      Status : 1B : Status Code for the requesting message
      NetKeyIndex : 2B : NetKey Index
      Phase : 1B : Key Refresh Phase State */      
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_KEY_REFRESH_PHASE_STATUS,MOBLE_FALSE,  4, 4, 0x8FFF , 0, 0},
  
  /* 4.3.2.68 Config Low Power Node PollTimeout Status, Opcode = 0x80 0x2E
     The Config Low Power Node PollTimeout Status is an unacknowledged message 
     used to report the current value of the PollTimeout timer of the Low Power 
     node within a Friend node. 
    Parameters:
      LPNAddress: 2B : The unicast address of the Low Power node
      PollTimeout: 3B : The current value of the PollTimeout timer of the Low Power node */
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_STATUS, MOBLE_FALSE,  5, 5, 0x8FFF , 0, 0},
  
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_MODEL_SUBSCRIPTION_STATUS,MOBLE_FALSE,  7, 9, 0x8FFF , 0, 0},
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_MODEL_PUBLICATION_STATUS, MOBLE_FALSE,  12, 14, 0x8FFF , 0, 0},
  {SIG_MODEL_ID_CONFIG_CLIENT, OPCODE_CONFIG_MODEL_APP_STATUS,         MOBLE_FALSE,  7, 9, 0x8FFF , 0, 0},
  {SIG_MODEL_ID_CONFIG_CLIENT,   OPCODE_CONFIG_NODE_RESET_STATUS,        MOBLE_FALSE,  0, 0, 0x8FFF , 0, 0},
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
void PackNetkeyAppkeyInto3Bytes (MOBLEUINT16 netKeyIndex, 
                                 MOBLEUINT16 appKeyIndex,
                                 MOBLEUINT8* keysArray3B);
void NetkeyAppkeyUnpack (MOBLEUINT16 *pnetKeyIndex, 
                                MOBLEUINT16 *pappKeyIndex,
                                MOBLEUINT8* keysArray3B);
//MOBLE_RESULT ConfigClient_AppKeyAdd (MOBLEUINT16 netKeyIndex, 
//                                     MOBLEUINT16 appKeyIndex, 
//                                     MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyStatus(MOBLEUINT8 const *pSrcAppKeyStatus, 
                                                        MOBLEUINT32 length); 
MOBLE_RESULT ConfigClient_AppKeyUpdate (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyDelete (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyGet (MOBLEUINT8* appkey);
MOBLE_RESULT ConfigClient_AppKeyList (MOBLEUINT8* appkey);
MOBLE_RESULT _ConfigClient_SubscriptionAdd (configClientModelSubscriptionAdd_t *modelSubscription);
MOBLE_RESULT ConfigClient_SubscriptionDelete (void);
MOBLE_RESULT ConfigClient_SubscriptionDeleteAll (void);
MOBLE_RESULT ConfigClient_SubscriptionOverwrite (void);
MOBLE_RESULT ConfigClient_SubscriptionGet (void);
MOBLE_RESULT ConfigClient_SubscriptionList (void);
MOBLE_RESULT ConfigClient_ModelAppUnbind (void);
MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray);
MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray);
WEAK_FUNCTION (MOBLEUINT8* GetNewProvNodeDevKey(void));
MOBLE_RESULT ConfigClient_NodeResetStatus(MOBLEUINT8 const *pStatus, 
                                          MOBLEUINT32 length);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  ConfigClient_CompositionDataGet: This function is called to read the 
          composition data of the node 
* @param  None: No parameter for this function
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_CompositionDataGet(MOBLE_ADDRESS dst_peer) 
{
  
  /* 4.3.2.4 Config Composition Data Get
  The Config Composition Data Get is an acknowledged message used to read one 
  page of the Composition Data (see Section 4.2.1).
  The response to a Config Composition Data Get message is a 
  Config Composition Data Status message
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
    
  configClientGetCompositionMsg_t ccGetCompositionMsg;
  
  TRACE_M(TF_CONFIG_CLIENT_M, "Config CompositionDataGet Message of Node %d\r\n", dst_peer);  
  ccGetCompositionMsg.Opcode = OPCODE_CONFIG_COMPOSITION_DATA_GET;
  ccGetCompositionMsg.page = COMPOSITION_PAGE0;

  msg_opcode = OPCODE_CONFIG_COMPOSITION_DATA_GET;
  pConfigData = (MOBLEUINT8*) &(ccGetCompositionMsg.page);
  dataLength = sizeof(ccGetCompositionMsg.page);


  ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);
  
  return result;
}


/**
* @brief  ConfigClient_CompositionDataStatusResponse: This function is a call
           back when the response is received for Composition
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_CompositionDataStatusResponse(MOBLEUINT8 const *pSrcComposition, 
                                                        MOBLEUINT32 length)  
{
  MOBLEUINT8 *pSrcElements;
  MOBLEUINT32 parsedDataLength;
  MOBLEUINT8 elementIndex;
  MOBLEUINT8 numNodeSIGmodels;
  MOBLEUINT8 numNodeVendormodels;
  MOBLEUINT8 varModels;
  MOBLEUINT8 indexModels;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
       
  TRACE_M(TF_CONFIG_CLIENT_M, "Composition Status Cb \r\n");  

  parsedDataLength = 0; 
 
  if (length >= DEVICE_COMPOSITION_HEADER_SIZE)
  {
  /* Copy the header of the Composition page */ 
    NodeCompositionPage0Header.DataPage = *pSrcComposition; 
    NodeCompositionPage0Header.NodeCID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+1));
    NodeCompositionPage0Header.NodePID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+3));
    NodeCompositionPage0Header.NodeVID = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+5));
    NodeCompositionPage0Header.NodeCRPL = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+7));
    NodeCompositionPage0Header.NodeFeatures = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(pSrcComposition+9));
  }
  
  parsedDataLength += DEVICE_COMPOSITION_HEADER_SIZE; 
  
  if (length > parsedDataLength)
  {
  /* Point to the Start of Elements data from source  */
  /* Point after the Header and Loc , NumS, NumV */
  pSrcElements = (MOBLEUINT8*)(pSrcComposition+11);
  
  for (elementIndex =0; elementIndex < CLIENT_MAX_ELEMENTS_PER_NODE; elementIndex++ )
  { 
      
    NodeInfo.NbOfelements = elementIndex+1;   /* Save number of elements available in node for later use */
      
    /* Point to the destination address in Global Variable */
    /* Copy Loc, NumSIGmodels, NumVendorModels in Composition page */
    aNodeElements[elementIndex].Loc = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)pSrcElements);
    
    pSrcElements += 2;
    parsedDataLength += 2; 
      
    aNodeElements[elementIndex].NumSIGmodels = *(pSrcElements);
    
    pSrcElements++;
    parsedDataLength++;
      
    aNodeElements[elementIndex].NumVendorModels = *(pSrcElements);

    pSrcElements++;
    parsedDataLength++;

    /******************* Copy the SIG Models **********************************/    
    /* Prepare the variables to find the number of SIG Models, SInce header is already copied,
       it can use used directly for the comparison */
    /* This is to be used for running the loop for data copy */
      
    if (length >= (parsedDataLength+2))
    { /* Still length of data received is more than parsed and the remaining 
         data is >2Bytes for SIG Model ID */
        
      numNodeSIGmodels = aNodeElements[elementIndex].NumSIGmodels;
      varModels = numNodeSIGmodels;
    
      /* Point to the Elements array for the SIG Models  */

      if (numNodeSIGmodels > CLIENT_MAX_SIG_MODELS_PER_ELEMENT)
      { /* Number of models of Node is more than storage capacity */
          varModels = CLIENT_MAX_SIG_MODELS_PER_ELEMENT;
      }
 
      for (indexModels=0; indexModels< varModels; indexModels++)
      {
        aNodeElements[elementIndex].aSIGModels[indexModels] = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)pSrcElements);
        pSrcElements +=2;  /* Increment by 2 Bytes for next Model */
        parsedDataLength += 2;
      }
       
      /* If the source has more SIG Model IDs, then keep reading them till next address */
      if (numNodeSIGmodels > CLIENT_MAX_SIG_MODELS_PER_ELEMENT)
      {
        for (; indexModels< numNodeSIGmodels; indexModels++)
        {
          /* Increase the Source address pointer ONLY */
          pSrcElements +=2;
          parsedDataLength += 2;
        }
      }
    } /* if (length > parsedDataLength) */
    else
    {
      break;
    }
    
    /******************* Copy the Vendor Models *******************************/
    numNodeVendormodels = aNodeElements[elementIndex].NumVendorModels;
    varModels = numNodeVendormodels;
    
    if (length >= (parsedDataLength+4))
    { /* Still length of data received is more than parsed and the remaining 
         data is >4Bytes for Vendor Model ID */
        
      /* Point to the destination address in Global Variable */
      if (numNodeVendormodels > CLIENT_MAX_VENDOR_MODELS_PER_ELEMENT)
      {
        varModels = CLIENT_MAX_VENDOR_MODELS_PER_ELEMENT;
      }

      /* Start copying the Vendor Models */
      for (indexModels=0; indexModels < varModels; indexModels++)
      {
        aNodeElements[elementIndex].aVendorModels[indexModels] = CopyU8LittleEndienArrayToU32word(pSrcElements);
        pSrcElements +=4;
        parsedDataLength += 4;
      }
       
      /* If the source has more Vendor Model IDs, then keep reading them till next Element */
      if (numNodeVendormodels > CLIENT_MAX_VENDOR_MODELS_PER_ELEMENT)
      {
        for (; indexModels< numNodeVendormodels; indexModels++)
        {
          /* Increase the Source address pointer only */
          pSrcElements +=4;
        }
      }
    } /* if (length > (parsedDataLength+4)) */
    else
    {
      break;
    }
      
    if (length <= parsedDataLength)
    { /* No more data to parse */
      break;
    }
  
    } /*   for (elementIndex =0; elementIndex < CLIENT_MAX_ELEMENTS_PER_NODE; elementIndex++ ) */
  } /* if (length > parsedDataLength) */
  
  Appli_CompositionDataStatusCb(result);
  return result;
  
}


/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetNodeElementAddress(void)
{
  return NodeInfo.nodePrimaryAddress; 
}

/**
* @brief  GetServerElementAddress: This function gets the element address 
          from elementIndex
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetServerElementAddress(MOBLEUINT8 elementIdx)
{
  MOBLEUINT16 elementAddr;
  
  elementAddr = NodeInfo.nodePrimaryAddress; 
  elementAddr += elementIdx;
  return elementAddr;
}

/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 ConfigClient_GetNodeElements(void)
{
  return NodeInfo.NbOfelements;  
}

/**
* @brief  GetSIGModelFromCompositionData: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT16 GetSIGModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG)
{
  MOBLEUINT16 model;

  model = aNodeElements[elementIdx].aSIGModels[idxSIG+2]; 
  return model;
}


/**
* @brief  GetNodeElementAddress: This function gets the element address 
          from last known address saved in the flash 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT32 GetVendorModelFromCompositionData(MOBLEUINT8 elementIdx, MOBLEUINT8 idxVendor)
{
  MOBLEUINT32 model;

  model = aNodeElements[elementIdx].aVendorModels[idxVendor]; 
  /* Maybe little endian conversion may be needed */
  
  return model;
}

/**
* @brief  GetTotalSIGModelsCount: This function gets the Total number of 
           SIG Models available in the node 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetTotalSIGModelsCount(MOBLEUINT8 elementIdx)
{
  MOBLEUINT8 sigModelsCount;

  sigModelsCount = aNodeElements[elementIdx].NumSIGmodels; 
  return sigModelsCount;
}

/**
* @brief  GetTotalSIGModelsCount: This function gets the Total number of 
           SIG Models available in the node 
* @param  None
* @retval MOBLE_RESULT
*/ 
MOBLEUINT8 GetTotalVendorModelsCount(MOBLEUINT8 elementIdx)
{
  return aNodeElements[elementIdx].NumVendorModels ; 
}

  
/**
* @brief  PackNetkeyAppkeyInto3Bytes: This function is called to pack the 
          2 key Index into 3Bytes
* @param  netKeyIndex: keyIndex to be packed
* @param  appKeyIndex: keyIndex to be packed
* @retval MOBLE_RESULT
*/ 
void PackNetkeyAppkeyInto3Bytes (MOBLEUINT16 netKeyIndex, 
                                 MOBLEUINT16 appKeyIndex,
                                 MOBLEUINT8* keysArray3B)
{
  /*
  4.3.1.1 Key indexes
  Global key indexes are 12 bits long. Some messages include 
    one, two or multiple key indexes. 
      To enable efficient packing, two key indexes are packed into three octets.  
  */
  
  /* 
  To pack two key indexes into three octets... 
  8 LSbs of first key index value are packed into the first octet
    placing the remaining 4 MSbs into 4 LSbs of the second octet. 
  The first 4 LSbs of the second 12-bit key index are packed into 
  the 4 MSbs of the second octet with the remaining 8 MSbs into the third octet.
  */
  keysArray3B[0] = (MOBLEUINT8) (netKeyIndex & 0x00ff);
  keysArray3B[1] = (MOBLEUINT8) ((netKeyIndex & 0x0f00) >> 8); /* Take 4bit Nibble to 4LSb nibble */
  keysArray3B[1] |= (MOBLEUINT8) ((appKeyIndex & 0x000f) << 4); /* Take 4LSb to upper Nibble */
  keysArray3B[2] = (MOBLEUINT8) ((appKeyIndex >>4) & 0xff);  /* Take 8MSb to a byte */
  
}


/**
* @brief  PackNetkeyAppkeyInto3Bytes: This function is called to pack the 
          2 key Index into 3Bytes
* @param  netKeyIndex: keyIndex to be packed
* @param  appKeyIndex: keyIndex to be packed
* @retval MOBLE_RESULT
*/ 
void NetkeyAppkeyUnpack (MOBLEUINT16 *pnetKeyIndex, 
                                MOBLEUINT16 *pappKeyIndex,
                                MOBLEUINT8* keysArray3B)
{
  /*
  4.3.1.1 Key indexes
  Global key indexes are 12 bits long. Some messages include 
    one, two or multiple key indexes. 
      To enable efficient packing, two key indexes are packed into three octets.  
  */
  MOBLEUINT16 netKeyIndex; 
  MOBLEUINT16 appKeyIndex;
  /* 
  To pack two key indexes into three octets... 
  8 LSbs of first key index value are packed into the first octet
    placing the remaining 4 MSbs into 4 LSbs of the second octet. 
  The first 4 LSbs of the second 12-bit key index are packed into 
  the 4 MSbs of the second octet with the remaining 8 MSbs into the third octet.
  */
   netKeyIndex = keysArray3B[1] & 0x0f;  /* Take 4MSb from 2nd octet */
   netKeyIndex <<= 8;
   netKeyIndex |= keysArray3B[0];
   
   appKeyIndex = keysArray3B[1] & 0xf0;  /* Take LSb from 2nd octet */
   appKeyIndex >>= 4;
   appKeyIndex |= (keysArray3B[2] << 0x04);
   
   *pnetKeyIndex = netKeyIndex;
   *pappKeyIndex = appKeyIndex;
}


/**
* @brief  ConfigClient_AppKeyAdd: This function is called to 
          add the default AppKeys and net keys to a node under configuration
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyAdd (  MOBLE_ADDRESS dst_peer,
                                       MOBLEUINT16 netKeyIndex, 
                                     MOBLEUINT16 appKeyIndex, 
                                     MOBLEUINT8* appkey)
{
  /*
4.3.2.37 Config AppKey Add
The Config AppKey Add is an acknowledged message used to add an AppKey to 
the AppKey List on a node and bind it to the NetKey identified by NetKeyIndex. 
The added AppKey can be used by the node only as a pair with the specified NetKey. The AppKey is used to authenticate and decrypt messages it receives, as well as authenticate and encrypt messages it sends.
The response to a Config AppKey Add message is a Config AppKey Status message.

NetKeyIndexAndAppKeyIndex: 3B : Index of the NetKey and index of the AppKey
AppKey 16B : AppKey value
  */
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  configClientAppKeyAdd_t configClientAppKeyAdd;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
  
  configClientAppKeyAdd.netKeyIndex = netKeyIndex; 
  configClientAppKeyAdd.appKeyIndex = appKeyIndex;
  memcpy (configClientAppKeyAdd.a_Appkeybuffer, appkey, APPKEY_SIZE );

  msg_opcode = OPCODE_CONFIG_APPKEY_ADD;
  pConfigData = (MOBLEUINT8*) &(configClientAppKeyAdd);
  dataLength = sizeof(configClientAppKeyAdd_t);
  
  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client App Key Add of Node %d\r\n", dst_peer);  
  ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);

  return result;
}

/**
* @brief  ConfigClient_AppKeyStatus: This function is a call
           back when the response is received for AppKey Add
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyStatus(MOBLEUINT8 const *pSrcAppKeyStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.40 Config AppKey Status
  The Config AppKey Status is an unacknowledged message used to report a status 
  for the requesting message, based on the 
     NetKey Index identifying the NetKey on the NetKey List and on the 
     AppKey Index identifying the AppKey on the AppKey List.
  */
  
  /*
Status : 1B : Status Code for the requesting message
NetKeyIndexAndAppKeyIndex : 3B : Index of the NetKey and index of the AppKey
  */
  configClientAppKeyStatus_t configClientAppKeyStatus;
  MOBLEUINT16 netKeyIndex; 
  MOBLEUINT16 appKeyIndex;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* Config AppKey Status 0x80 0x03*/
  configClientAppKeyStatus.Status = pSrcAppKeyStatus[0];  /* Ignoring the OpCode */
  
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientAppKeyStatus.Status)
  {
    /* Status returned is an error */
  }
  
  NetkeyAppkeyUnpack (&netKeyIndex, &appKeyIndex, (MOBLEUINT8*) (pSrcAppKeyStatus+1));
  configClientAppKeyStatus.appKeyIndex = appKeyIndex;
  configClientAppKeyStatus.netKeyIndex = netKeyIndex;
  Appli_AppKeyStatusCb(configClientAppKeyStatus.Status);
  /* The Netkey and AppKey can be compared with what was issued */

  return result;
}


/**
* @brief  ConfigClient_AppKeyUpdate: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyUpdate (MOBLEUINT8* appkey)
{
  /*
4.3.2.38 Config AppKey Update
The Config AppKey Update is an acknowledged message used to update an AppKey value on the AppKey List on a node. The updated AppKey is used by the node to authenticate and decrypt messages it receives, as well as authenticate and encrypt messages it sends, as defined by the Key Refresh procedure (see Section 3.10.4).
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_AppKeyDelete: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyDelete (MOBLEUINT8* appkey)
{
/*
4.3.2.39 Config AppKey Delete
The Config AppKey Delete is an acknowledged message used to delete an AppKey from the AppKey List on a node.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_AppKeyGet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyGet (MOBLEUINT8* appkey)
{
/*
4.3.2.41 Config AppKey Get
The AppKey Get is an acknowledged message used to report all AppKeys bound to the NetKey.
The response to a Config AppKey Get message is a Config AppKey List message.
*/
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}

/**
* @brief  ConfigClient_AppKeyList: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_AppKeyList (MOBLEUINT8* appkey)
{
/*
4.3.2.42 Config AppKey List
The Config AppKey List is an unacknowledged message reporting all AppKeys that are bound to the NetKey.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  return result;
}


/**
* @brief  ConfigClient_PublicationSet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_PublicationSet (MOBLEUINT16 elementAddress,
                                          MOBLEUINT16 publishAddress,
                                          MOBLEUINT16 appKeyIndex,
                                          MOBLEUINT8 credentialFlag,
                                          MOBLEUINT8 publishTTL,
                                          MOBLEUINT8 publishPeriod,
                                          MOBLEUINT8 publishRetransmitCount,
                                          MOBLEUINT8 publishRetransmitIntervalSteps,
                                          MOBLEUINT32 modelIdentifier)
{
/* 
  4.3.2.16 Config Model Publication Set
  The Config Model Publication Set is an acknowledged message used to set the Model
  Publication state (see Section 4.2.2) of an outgoing message that originates 
  from a model.

  The response to a Config Model Publication Set message is a Config Model 
  Publication Status message.
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
  
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLEUINT32 dataLength;
  MOBLE_ADDRESS self_addr;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  configClientModelPublication_t configClientModelPublication;

  
  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client Publication Add Message \r\n");  
  dataLength = sizeof(configClientModelPublication_t);

  
  if ( (ADDRESS_IS_GROUP(elementAddress)) || 
      (ADDRESS_IS_UNASSIGNED(elementAddress)) )
  {
    /* The ElementAddress field is the unicast address of the element, 
       all other address types are Prohibited. */
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
      
    configClientModelPublication.elementAddr = elementAddress;
    configClientModelPublication.publishAddr = publishAddress;
    configClientModelPublication.appKeyIndex = appKeyIndex;
    configClientModelPublication.credentialFlag = credentialFlag;
    configClientModelPublication.rfu = 0;
    configClientModelPublication.publishTTL=publishTTL;
    configClientModelPublication.publishPeriod=publishPeriod;
    configClientModelPublication.publishRetransmitCount=publishRetransmitCount;
    configClientModelPublication.publishRetransmitIntervalSteps=publishRetransmitIntervalSteps;
    configClientModelPublication.modelIdentifier=modelIdentifier;
  
    
    if(CHKSIGMODEL(modelIdentifier))
    {
      /* if upper 16b are 0, then it's a SIG Model */
      dataLength -= 2;
    }
  }
  

  msg_opcode = OPCODE_CONFIG_CONFIG_MODEL_PUBLICATION_SET;
  pConfigData = (MOBLEUINT8*) &(configClientModelPublication);
  
  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client Publication Add  \r\n");  
  TRACE_M(TF_CONFIG_CLIENT_M, "elementAddr = [%04x]\r\n", elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT_M, "publishAddress = [%04x]\r\n", publishAddress); 
  TRACE_M(TF_CONFIG_CLIENT_M, "modelIdentifier = [%08lx]\r\n", modelIdentifier);
  
  TRACE_I(TF_CONFIG_CLIENT_M, "Publication Set buffer \r\n");
  
  for (MOBLEUINT8 count=0 ; count<dataLength; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT_M, "%.2x ", pConfigData[count]);
  } 
  TRACE_I(TF_CONFIG_CLIENT_M, "\r\n");
  
  self_addr = BLEMesh_GetAddress();
  
  if ((elementAddress >= self_addr) && (elementAddress < (self_addr+APPLICATION_NUMBER_OF_ELEMENTS)) )
  {
    /* Provisioner needs to be configured */
    ConfigModel_SelfPublishConfig(elementAddress,
                                         msg_opcode,pConfigData,dataLength);
  }
  else
  {
    /* Node address to be configured */
    ConfigClientModel_SendMessage(elementAddress,msg_opcode,pConfigData,dataLength);
  }

  return result;
}


/**
* @brief  ConfigClient_PublicationStatus: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_PublicationStatus(MOBLEUINT8 const *pPublicationStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.18 Config Model Publication Status
  The Config Model Publication Status is an unacknowledged message used to report 
  the model Publication state (see Section 4.2.2) of an outgoing message that is 
  published by the model.
 */
  
  /*
     
  Status : 8b : Status Code for the requesting message
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
  configClientPublicationStatus_t configClientPublicationStatus;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; 
  MOBLEUINT8 temp_var=0;
  MOBLEUINT16 u16temp_var=0;
  MOBLEUINT32 modelIdentifier=0;
  MOBLEUINT8* pSrc;
  
  pSrc = (MOBLEUINT8*)pPublicationStatus;
  configClientPublicationStatus.Status = *pPublicationStatus; 
  configClientPublicationStatus.elementAddr = CopyU8LittleEndienArrayToU16word(pSrc+1); 
  configClientPublicationStatus.publishAddr = CopyU8LittleEndienArrayToU16word(pSrc+3); 
  u16temp_var = CopyU8LittleEndienArrayToU16word(pSrc+5); 
  u16temp_var &= 0x0FFF;  /* Take 12b only*/
  configClientPublicationStatus.appKeyIndex = u16temp_var; 
  
  temp_var = *(pSrc+6) & 0x10;
  configClientPublicationStatus.credentialFlag = temp_var >> 4; 
  configClientPublicationStatus.publishTTL = *(pSrc+7); 
  configClientPublicationStatus.publishPeriod = *(pSrc+8); 
  configClientPublicationStatus.publishRetransmitCount = *(pSrc+9) & 0x07; 
  temp_var = *(pSrc+9) & 0xf8; 
  temp_var>>= 3;            
  configClientPublicationStatus.publishRetransmitIntervalSteps = temp_var; 

  if (length == 12 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrc+10);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrc+10);
  }
    
  configClientPublicationStatus.modelIdentifier = modelIdentifier;   
    
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientPublicationStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT_M, "\r\n Config Client Publication Status Recd \r\n");  
  TRACE_I(TF_CONFIG_CLIENT_M, "Publication Status buffer: ");
  for (MOBLEUINT8 count=0 ; count<length; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT_M, "%.2x ", pPublicationStatus[count]);
  }  
  TRACE_M(TF_CONFIG_CLIENT_M, "elementAddr = [%04x]\r\n", configClientPublicationStatus.elementAddr);  
  TRACE_M(TF_CONFIG_CLIENT_M, "publishAddress = [%04x]\r\n", configClientPublicationStatus.publishAddr); 
  TRACE_M(TF_CONFIG_CLIENT_M, "modelIdentifier = [%08lx]\r\n", configClientPublicationStatus.modelIdentifier);
  TRACE_M(TF_CONFIG_CLIENT_M, "status = [%02x]\r\n", configClientPublicationStatus.Status);  

  Appli_PublicationStatusCb(configClientPublicationStatus.Status);

  return result;
}



/**
* @brief  ConfigClient_SubscriptionAdd: This function is called for issuing
         add subscribe message to an element(node) for Models and AppKey binding
* @param  elementAddress: Element address of node for model binding
* @param  appKeyIndex: Index of App key 
* @param  MOBLEUINT32: Model to be subscribed 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionAdd (MOBLEUINT16 elementAddress,
                                           MOBLEUINT16 address, 
                                           MOBLEUINT32 modelIdentifier)
{
/*
4.3.2.19 Config Model Subscription Add
The Config Model Subscription Add is an acknowledged message used to add an 
address to a Subscription List of a model (see Section 4.2.3).

The response to a Config Model Subscription Add message is a 
Config Model Subscription Status message.

  ElementAddress  : 2B       : Address of the element
  address         : 2B       : Value of the address
  ModelIdentifier : 2B or 4B : SIG Model ID or Vendor Model ID
*/

  MOBLEUINT32 dataLength = 0;
  MOBLE_ADDRESS self_addr;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  configClientModelSubscriptionAdd_t modelSubscription;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
   
  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client Subscription Add Message  \r\n");  
  
  /* The ElementAddress field is the unicast address of the element, 
     all other address types are Prohibited.
     The Address field shall contain the new address to be added to the 
     Subscription List. 
     The value of the Address field shall not be an unassigned address, 
     unicast address, all-nodes address or virtual address.
   */

  if ( (ADDRESS_IS_GROUP(elementAddress)) || (ADDRESS_IS_UNASSIGNED(elementAddress)) )
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    modelSubscription.elementAddress = elementAddress;    
    modelSubscription.address = address;
    modelSubscription.modelIdentifier = modelIdentifier;
    
    /*  
         The ModelIdentifier field is either a SIG Model ID or a Vendor Model ID 
     that shall identify the model within the element.
    */
    
    if(CHKSIGMODEL(modelIdentifier))
    {
      /* if upper 16b are 0, then it's a SIG Model */
     dataLength = sizeof(configClientModelSubscriptionAdd_t) - 2;
    }
    else
    {
      dataLength = sizeof(configClientModelSubscriptionAdd_t);
    }
  } /* else: address is valid */
  
   msg_opcode = OPCODE_CONFIG_MODEL_SUBSCRIPTION_ADD;
   pConfigData = (MOBLEUINT8*) &(modelSubscription);
  
  TRACE_I(TF_CONFIG_CLIENT_M, "Subscription Set buffer ");
  
  for (MOBLEUINT8 count=0 ; count<dataLength; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT_M, "%.2x ", pConfigData[count]);
  } 
   
  TRACE_M(TF_CONFIG_CLIENT_M, "elementAddr = [%04x]\r\n", elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT_M, "SubscriptionAddress = [%04x]\r\n", address); 
  TRACE_M(TF_CONFIG_CLIENT_M, "modelIdentifier = [%08lx]\r\n", modelIdentifier);
   
  self_addr = BLEMesh_GetAddress();

  if ((elementAddress >= self_addr) && (elementAddress < (self_addr+APPLICATION_NUMBER_OF_ELEMENTS)) )
  {
    /* Provisioner needs to be configured */
    ConfigModel_SelfSubscriptionConfig(elementAddress,
                                              msg_opcode,pConfigData,dataLength);
  }
  else
  {
    ConfigClientModel_SendMessage(elementAddress,msg_opcode,pConfigData,dataLength);  
  }
  
  return result;
  
}

/**
* @brief  ConfigClient_SubscriptionDelete: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionDelete (void)
{
/*
4.3.2.21 Config Model Subscription Delete
The Config Model Subscription Delete is an acknowledged message used to delete a subscription address from the Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Delete message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;
}

/**
* @brief  ConfigClient_SubscriptionDeleteAll: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionDeleteAll (void)
{
/*
4.3.2.25 Config Model Subscription Delete All
The Config Model Subscription Delete All is an acknowledged message used to discard the Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Delete All message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;

}

/**
* @brief  ConfigClient_SubscriptionOverwrite: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionOverwrite (void)
{
  /*
4.3.2.23 Config Model Subscription Overwrite
The Config Model Subscription Overwrite is an acknowledged message used to discard the Subscription List and add an address to the cleared Subscription List of a model (see Section 4.2.3).
The response to a Config Model Subscription Overwrite message is a Config Model Subscription Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  return result;
  
}


/**
* @brief  ConfigClient_SubscriptionStatus: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionStatus(MOBLEUINT8 const *pSrcSubscriptionStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
  4.3.2.26 Config Model Subscription Status
  The Config Model Subscription Status is an unacknowledged message used to report
  a status of the operation on the Subscription List (see Section 4.2.3).
  */
  
  /*
     Status : 1B : Status Code for the requesting message
     ElementAddress : 2B : Address of the element
     Address        : 2B : Value of the address
     ModelIdentifier : 2B or 4B: SIG Model ID or Vendor Model ID
  */
  configClientSubscriptionStatus_t configClientSubscriptionStatus;
  MOBLEUINT8 *pSrc;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 modelIdentifier=0;
    
  pSrc = (MOBLEUINT8*) pSrcSubscriptionStatus;
  configClientSubscriptionStatus.Status = *pSrc;
  configClientSubscriptionStatus.elementAddress = CopyU8LittleEndienArrayToU16word(pSrc+1); 
  configClientSubscriptionStatus.address = CopyU8LittleEndienArrayToU16word(pSrc+3); 
    
  if (length == 7 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrc+5);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrc+5);
  }
  
  configClientSubscriptionStatus.modelIdentifier = modelIdentifier;   
    
  
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientSubscriptionStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT_M, "ConfigClient_SubscriptionStatus  \r\n"); 
  TRACE_I(TF_CONFIG_CLIENT_M, "SubscriptionStatus buffer ");
  
  for (MOBLEUINT8 count=0 ; count<length; count++)
  {
    TRACE_I(TF_CONFIG_CLIENT_M, "%.2x ", pSrcSubscriptionStatus[count]);
  }
  
  TRACE_M(TF_CONFIG_CLIENT_M, "elementAddr = [%04x]\r\n", configClientSubscriptionStatus.elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT_M, "SubscriptionAddress = [%04x]\r\n", configClientSubscriptionStatus.address); 
  TRACE_M(TF_CONFIG_CLIENT_M, "modelIdentifier = [%08lx]\r\n", configClientSubscriptionStatus.modelIdentifier);
  TRACE_M(TF_CONFIG_CLIENT_M, "subscription status = [%02x]\r\n", configClientSubscriptionStatus.Status);
  
  Appli_SubscriptionAddStatusCb(configClientSubscriptionStatus.Status);

  return result;


}


/**
* @brief  ConfigClient_SubscriptionGet: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionGet (void)
{
/*
4.3.2.27 Config SIG Model Subscription Get
The Config SIG Model Subscription Get is an acknowledged message used to get the list of subscription addresses of a model within the element. This message is only for SIG Models.
The response to a Config SIG Model Subscription Get message is a Config SIG Model Subscription List message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
}


/**
* @brief  ConfigClient_SubscriptionList: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_SubscriptionList (void)
{
/*
4.3.2.28 Config SIG Model Subscription List
The Config SIG Model Subscription List is an unacknowledged message used to 
report all addresses from the Subscription List of the model (see Section 4.2.3). This message is only for SIG Models.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
  
}


/**
* @brief  ConfigClient_ModelAppBind: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppBind (MOBLEUINT16 elementAddress,
                                        MOBLEUINT16 appKeyIndex,
                                        MOBLEUINT32 modelIdentifier)
{
/*
4.3.2.46 Config Model App Bind
The Config Model App Bind is an acknowledged message used to bind an AppKey to a model.
The response to a Config Model App Bind message is a Config Model App Status message.

  ElementAddress : 2B : Address of the element
  AppKeyIndex : 2B : Index of the AppKey
  ModelIdentifier : 2 or 4: SIG Model ID or Vendor Model ID
*/

  MOBLEUINT32 dataLength;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  configClientModelAppBind_t modelAppBind;
  MOBLE_ADDRESS self_addr;
  
  modelAppBind.appKeyIndex = appKeyIndex;
  modelAppBind.elementAddress = elementAddress; /* Will be converted to address inside lib */
  modelAppBind.modelIdentifier = modelIdentifier;
  dataLength = sizeof(configClientModelAppBind_t);

  if(CHKSIGMODEL(modelIdentifier))
  {
    /* if upper 16b are 0, then it's a SIG Model */
    dataLength -= 2;  /* reduce 2 bytes when it's SIG Model */
  }
  
  msg_opcode = OPCODE_CONFIG_MODEL_APP_BIND;
  pConfigData = (MOBLEUINT8*) &(modelAppBind);

  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client App Key Bind message  \r\n");   
  TRACE_M(TF_CONFIG_CLIENT_M, "elementAddr = [%04x]\r\n", elementAddress);  
  TRACE_M(TF_CONFIG_CLIENT_M, "modelIdentifier = [%08lx]\r\n", modelIdentifier);
  
  if(ADDRESS_IS_UNASSIGNED(elementAddress))
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  
  self_addr = BLEMesh_GetAddress();
  if ((elementAddress >= self_addr) && (elementAddress < (self_addr+APPLICATION_NUMBER_OF_ELEMENTS)) )
  {
    /* Provisioner needs to be configured */
    ConfigClient_SelfModelAppBindConfig(self_addr,
                                        msg_opcode,pConfigData,dataLength);
  }
  else
  {
    /* Node address to be configured */
    ConfigClientModel_SendMessage(elementAddress,msg_opcode,pConfigData,dataLength);
  }
  
  return result;
}


/**
* @brief  ConfigClient_ModelAppUnbind: This function is called for both Acknowledged and 
unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppUnbind (void)
{
/*
4.3.2.47 Config Model App Unbind
The Config Model App Unbind is an acknowledged message used to remove the binding between an AppKey and a model.
The response to a Config Model App Unbind message is a Config Model App Status message.
*/
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  return result;
}



/**
* @brief  ConfigClient_ModelAppStatus: This function is a call
           back when the response is received for AppKey Add
* @param  configClientAppKeyAdd_t: Structure of the AppKey add message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_ModelAppStatus(MOBLEUINT8 const *pSrcModelAppStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
    4.3.2.48 Config Model App Status
    The Config Model App Status is an unacknowledged message used to report a status
    for the requesting message, based on the element address, 
    the AppKeyIndex identifying the AppKey on the AppKey List, 
                                                       and the ModelIdentifier.
  */
  
  /*
     Status : 1B : Status Code for the requesting message
     ElementAddress : 2B : Address of the element
     AppKeyIndex : 2B : Index of the AppKey
     ModelIdentifier : 2B or 4B: SIG Model ID or Vendor Model ID
  */
  configClientModelAppStatus_t configClientModelAppStatus;
  MOBLEUINT8* pSrcAppStatus;
  MOBLEUINT32 modelIdentifier;
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  pSrcAppStatus = (MOBLEUINT8*)pSrcModelAppStatus;
  
  configClientModelAppStatus.Status = *pSrcAppStatus; 
  configClientModelAppStatus.elementAddress = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+1);
  configClientModelAppStatus.appKeyIndex = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+3);

  if (length == 7 )
  {
    modelIdentifier = CopyU8LittleEndienArrayToU16word(pSrcAppStatus+5);
  }
  else
  {
    modelIdentifier = CopyU8LittleEndienArrayToU32word(pSrcAppStatus+5);
  }
  
  configClientModelAppStatus.modelIdentifier = modelIdentifier;  
    
  if ((ConfigModelStatusCode_t)SuccessStatus != configClientModelAppStatus.Status)
  {
    /* Status returned is an error */
  }
  
  TRACE_M(TF_CONFIG_CLIENT_M, "ConfigClient_ModelAppStatus = %d \r\n", configClientModelAppStatus.Status);    
  Appli_AppBindModelStatusCb(configClientModelAppStatus.Status);

  return result;
}


/**
* @brief  ConfigClient_ModelNodeReset: This function is called for reset of the 
           Node 
* @param  elementAddress: Element address to be reset / Un-provisioned
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_NodeReset (MOBLEUINT16 elementAddress)
{
/*
4.3.2.53 Config Node Reset
The Config Node Reset is an acknowledged message used to reset a node (other than a Provisioner) and remove it from the network.
The response to a Config Node Reset message is a Config Node Reset Status message.
There are no Parameters for this message.
*/
  MOBLEUINT32 dataLength;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8* pConfigData;
  MOBLE_ADDRESS dst_peer;
  MOBLE_ADDRESS self_addr;

  dataLength = 0;
  msg_opcode = OPCODE_CONFIG_NODE_RESET;
  pConfigData = (MOBLEUINT8*)(NULL);

  dst_peer = elementAddress; 

  TRACE_M(TF_CONFIG_CLIENT_M, "Config Client Node Reset message  \r\n");   
  if(ADDRESS_IS_UNASSIGNED(elementAddress))
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  
  self_addr = BLEMesh_GetAddress();
  if (elementAddress == self_addr)
  {

  }
  else
  {
    /* Node address to be configured */
    result = ConfigClientModel_SendMessage(dst_peer,msg_opcode,pConfigData,dataLength);
  }
  
  return result;
}


/**
* @brief  ConfigClient_NodeResetStatus: This function is a call
           back when the response is received for Node Reset
* @param  pStatus: Pointer to the Status message parameters
* @param  length: Length of the status parameter message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClient_NodeResetStatus(MOBLEUINT8 const *pStatus, 
                                                        MOBLEUINT32 length)  
{
  /*
   4.3.2.54 Config Node Reset Status
   The Config Node Reset Status is an unacknowledged message used to acknowledge that an element has received a Config Node Reset message.
   There are no Parameters for this message.  */
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  TRACE_M(TF_CONFIG_CLIENT_M, "ConfigClient_NodeResetStatus Received \r\n");    
  Appli_NodeResetStatusCb();

  return result;
}


/**
* @brief   GenericModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_SendMessage(MOBLE_ADDRESS dst_peer ,
                                     MOBLEUINT16 opcode, 
                                     MOBLEUINT8 *pData,
                                     MOBLEUINT32 dataLength)
{
  MOBLE_ADDRESS peer_addr;  
  peer_addr = BLEMesh_GetAddress(); 
  MOBLEUINT8 *pTargetDevKey;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  
  pTargetDevKey = (MOBLEUINT8 *)AppliPrvnNvm_GetNodeDevKey(dst_peer);
  
  result = ConfigModel_SendMessage(peer_addr, dst_peer, opcode, 
                                   pData, dataLength, pTargetDevKey); 
  return result;
}

/**
* @brief   ApplicationGetConfigServerDeviceKey: This function is call-back 
*          from the library to Get the Device Keys from the application
* @param  MOBLE_ADDRESS src: Source address of the Config Server for which 
           device key is required
* @param  pkeyTbUse: Pointer to the Device key to be updated
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src, 
                                                 const MOBLEUINT8 **ppkeyTbUse)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;  
  *ppkeyTbUse= GetNewProvNodeDevKey();
  
  return result;  
}



/**
* @brief   ConfigClient_SaveMsgSendingTime: This function is used by application to save 
           the Initial time of message issue
* @param  None
* @retval None
*/ 
void ConfigClient_SaveMsgSendingTime (void)
{
   NodeInfo.Initial_time = Clock_Time();  /* Get the current time to see the 
                                                Timeout later */
}

/**
* @brief  ConfigClient_ChkRetrialState: This function is used by application 
          to check if there is a timeout of the Config Client Message sending
* @param  None
* @retval None
*/ 
MOBLEUINT8 ConfigClient_ChkRetrialState (eServerRespRecdState_t* peRespRecdState)
{
  MOBLEUINT8 retry_state = CLIENT_TX_INPROGRESS;
  MOBLEUINT32 nowClockTime;
  
  nowClockTime = Clock_Time();
  if(( (nowClockTime - NodeInfo.Initial_time) >= CONFIGCLIENT_RESPONSE_TIMEOUT))
  {
    /* Timeout occurred, Do retry or enter the error state  */
    NodeInfo.numberOfAttemptsTx++;
    
    if (NodeInfo.numberOfAttemptsTx >= CONFIGCLIENT_MAX_TRIALS)
    {
      NodeInfo.numberOfAttemptsTx = 0;
      retry_state = CLIENT_TX_RETRY_ENDS; /* re-trial cycle ends, no response */
      *peRespRecdState = NodeNoResponse_State;
      ConfigClient_ErrorState();
    }
    else 
    {
       retry_state = CLIENT_TX_TIMEOUT;   
       *peRespRecdState = NodeIdle_State;    /* Run next re-trial cycle again */
       TRACE_M(TF_CONFIG_CLIENT_M, "Retry started \n\r");       
    }
    
    ConfigClient_SaveMsgSendingTime(); /* Save the time again for next loop */
  }

  return retry_state;
}

/**
* @brief  ConfigClient_ChkRetries: This function is used by application 
          to check if there is a timeout of the Config Client Message sending
* @param  None
* @retval None
*/ 
MOBLEUINT8 ConfigClient_ChkRetries (void)
{
  MOBLEUINT8 retry_state = CLIENT_TX_INPROGRESS;
  MOBLEUINT32 nowClockTime;
  
  nowClockTime = Clock_Time();
  if(( (nowClockTime - NodeInfo.Initial_time) >= CONFIGCLIENT_RESPONSE_TIMEOUT))
  {
    /* Timeout occurred, Do retry or enter the error state  */
    NodeInfo.numberOfAttemptsTx++;
    
    if (NodeInfo.numberOfAttemptsTx >= CONFIGCLIENT_MAX_TRIALS)
    {
      NodeInfo.numberOfAttemptsTx = 0;
      retry_state = CLIENT_TX_RETRY_ENDS; /* re-trial cycle ends, no response */
      ConfigClient_ErrorState();
    }
    else 
    {
       retry_state = CLIENT_TX_TIMEOUT;   
       TRACE_M(TF_CONFIG_CLIENT_M, "Retry started \n\r");       
    }
    
    ConfigClient_SaveMsgSendingTime(); /* Save the time again for next loop */
  }

  return retry_state;
}

/**
* @brief   ConfigClient_ErrorState: This function is used by application to save 
           the Initial time of message issue
* @param  None
* @retval None
*/ 
void ConfigClient_ErrorState (void)
{
   /* No Response from the Node under provisioning after trials */
  TRACE_M(TF_CONFIG_CLIENT_M, "No response from Node \n\r"); 
}

/**
* @brief   ConfigClient_ResetTrials: This function is used by application to 
            Reset the Number of attempts of transmissions
* @param  None
* @retval None
*/ 
void ConfigClient_ResetTrials (void)
{
  NodeInfo.numberOfAttemptsTx = 0;
}
             

/**
* @brief   ConfigClientModel_GetOpcodeTableCb: This function is call-back 
*          from the library to process the Config Client Status messages 
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Config_Client_Opcodes_Table;
  *length = sizeof(Config_Client_Opcodes_Table)/sizeof(Config_Client_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  ConfigClientModel_GetStatusRequestCb : This function is call-back 
from the library to send response to the message from peer
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam,
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)

{
  TRACE_M(TF_CONFIG_CLIENT_M, "Response **Should Never enter here  \n\r");
  switch(opcode)
  {
 
  case 0:
    {     
      break;
    }

  default:
    {
      break;
    }
  }
  return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  GenericModelServer_ProcessMessageCb: This is a callback function from
the library whenever a Generic Model message is received
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  modelID: Received modelID of the message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT ConfigClientModel_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
//  tClockTime delay_t = Clock_Time();
  
  TRACE_M(TF_CONFIG_CLIENT_M, "dst_peer = [%04x] , peer_add = [%04x], opcode= [%.2X] ,response= [%.2X] \r\n  ",
                                                      pmsgParam->dst_peer, pmsgParam->peer_addr, opcode , response);

  switch(opcode)
  {
    
  case OPCODE_CONFIG_COMPOSITION_DATA_STATUS: 
    {     
      ConfigClient_CompositionDataStatusResponse(pRxData, dataLength); 
      if(result == MOBLE_RESULT_SUCCESS)
      {
        /*
        when device is working as proxy and is a part of node
        delay will be included in the toggelinf of led.
        */         
      }
      
      break;
    }
    
  case OPCODE_CONFIG_APPKEY_STATUS:
    {
      ConfigClient_AppKeyStatus(pRxData, dataLength); 
      break;
    }

  case OPCODE_CONFIG_MODEL_SUBSCRIPTION_STATUS:
    {
      ConfigClient_SubscriptionStatus(pRxData, dataLength); 
        
      break;
    }
    
   case OPCODE_CONFIG_MODEL_PUBLICATION_STATUS:
    {
      ConfigClient_PublicationStatus(pRxData, dataLength); 
      break;
    }

   case OPCODE_CONFIG_MODEL_APP_STATUS:
    {
      ConfigClient_ModelAppStatus(pRxData, dataLength); 
      break;
    }

   case OPCODE_CONFIG_NODE_RESET_STATUS:
    {
      ConfigClient_NodeResetStatus(pRxData, dataLength); 
      break;
    }

  default:
    {
      break;
    }          
  } /* Switch ends */
  
    
  return MOBLE_RESULT_SUCCESS;
}

#endif /* defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER) */

WEAK_FUNCTION (MOBLEUINT8* GetNewProvNodeDevKey(void))
{
  return 0;
}

/**
* @}
*/

/**
* @}
*/



