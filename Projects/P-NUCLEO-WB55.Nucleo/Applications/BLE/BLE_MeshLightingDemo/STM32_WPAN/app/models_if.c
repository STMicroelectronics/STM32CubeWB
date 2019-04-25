/**
  ******************************************************************************
  * @file    models_if.c
  * @author  BLE Mesh Team
  * @brief   Mesh Modes interface file of the application
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
#include "hal_common.h"
#include "types.h"
#include "ble_mesh.h"
#include "models_if.h"
#include "appli_mesh.h"
#include "mesh_cfg.h" 
#include "generic.h"
#include "vendor.h"
#include "light.h"
#include "sensors.h"
#include "appli_generic.h"
#include "appli_vendor.h"
#include "appli_light.h"
#include "appli_sensor.h"
#include "ble_hci_le.h"

/** @addtogroup BLE_Mesh
*  @{
*/

/** @addtogroup models_BLE
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  MOBLE_ADDRESS peer;
  MOBLEUINT8 elementIndex;
  MOBLEUINT8 command;
  MOBLEUINT8 data[DATA_BUFFER_LENGTH]; /* 8-Bytes response packet */
  MOBLEUINT32 length;
} APPLI_SEND_RESPONSE_MODULE;


typedef struct
{
  MOBLEUINT8 packet_count;
  MOBLEUINT32 send_time;
  APPLI_SEND_RESPONSE_MODULE* head;
  MOBLEUINT8 head_index;
  APPLI_SEND_RESPONSE_MODULE packet[MAX_PENDING_PACKETS_QUE_SIZE];
} APPLI_PENDING_PACKETS;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
APPLI_PENDING_PACKETS Appli_PendingPackets = {0};

__attribute__((aligned(4))) 
const MOBLE_VENDOR_CB_MAP vendor_cb = 
{
  Vendor_WriteLocalDataCb,
  Vendor_ReadLocalDataCb,
  Vendor_OnResponseDataCb
};

__attribute__((aligned(4))) 
const Appli_Vendor_cb_t VendorAppli_cb = 
{
  /*Vendor Commads*/
  Appli_Vendor_LEDControl,
  Appli_Vendor_DeviceInfo,
  Appli_Vendor_Test,
  Appli_LedCtrl
};

__attribute__((aligned(4)))   
const Appli_Generic_cb_t GenericAppli_cb = 
{
  /* Generic OnOff callbacks */
  Appli_Generic_OnOff_Set,
  /* Generic Level callbacks */
  Appli_Generic_Level_Set,
  Appli_Generic_LevelDelta_Set,
  Appli_Generic_LevelMove_Set,
  
};

__attribute__((aligned(4))) 
const Appli_Generic_State_cb_t Appli_GenericState_cb =
{
  
  /* Generic Get On Off status */
  Appli_Generic_GetOnOffStatus,
  /* Generic Get level status */
  Appli_Generic_GetLevelStatus,
  
};

__attribute__((aligned(4))) 
const Appli_Light_GetStatus_cb_t Appli_Light_GetStatus_cb = 
{
  Appli_Light_GetLightnessStatus,
  Appli_Light_GetLightnessLinearStatus,
  Appli_Light_GetLightnessDefaultStatus,
  Appli_Light_GetLightnessRangeStatus,
  Appli_Light_GetCtlLightStatus,
  Appli_Light_GetCtlTemperatureStatus,
  Appli_Light_GetCtlTemperatureRange,
  Appli_Light_GetCtlDefaultStatus,
  Appli_Light_GetHslStatus,
  Appli_Light_GetHslHueStatus,
  Appli_Light_GetHslSaturationStatus,
  Appli_Light_GetHslHueRange,
  Appli_Light_GetHslSatRange
};

__attribute__((aligned(4))) 
const MODEL_Generic_cb_t Model_Generic_cb = 
{
  GenericModelServer_GetOpcodeTableCb,
  GenericModelServer_GetStatusRequestCb,
  GenericModelServer_ProcessMessageCb
};

__attribute__((aligned(4))) 
const Appli_Light_cb_t LightAppli_cb = 
{
  /* Light Lightness callbacks */
  Appli_Light_Lightness_Set,
  Appli_Light_Lightness_Linear_Set,
  Appli_Light_Lightness_Default_Set,
  Appli_Light_Lightness_Range_Set,
  Appli_Light_Ctl_Set,
  Appli_Light_CtlTemperature_Set,
  Appli_Light_CtlTemperature_Range_Set,
  Appli_Light_CtlDefault_Set,
  Appli_Light_Hsl_Set,
  Appli_Light_HslHue_Set,
  Appli_Light_HslSaturation_Set,
  Appli_Light_HslDefault_Set,
  Appli_Light_HslRange_Set
};

__attribute__((aligned(4))) 
const MODEL_Light_cb_t Model_Light_cb = 
{
  LightModelServer_GetLightOpcodeTableCb,
  LightModelServer_GetStatusRequestCb,
  LightModelServer_ProcessMessageCb
};

__attribute__((aligned(4))) 
const Appli_Sensor_cb_t SensorAppli_cb = 
{
  /* Sensor Model callbacks */
  Appli_Sensor_Data_Status,
  Appli_Sensor_Descriptor_Status  
};

__attribute__((aligned(4))) 
const MODEL_Sensor_cb_t Model_Sensor_cb = 
{
  SensorModelServer_GetOpcodeTableCb,
  SensorModelServer_GetStatusRequestCb,
  SensorModelServer_ProcessMessageCb
};

extern MOBLEUINT8 NumberOfElements;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
* @brief  Initialization Commands for Models
* @param  void
* @retval void
*/    
void BLEMesh_ModelsInit(void)
{
  /* Callbacks used by BLE-Mesh library */
  BLEMesh_SetVendorCbMap(&vendor_cb);
  
  /* Callbacks used by BLE-Mesh Generic Models */
  BLEMesh_SetModelGenericCbMap(&Model_Generic_cb);
  
  BLEMesh_AddGenericModels();
  
  /* Callbacks used by BLE-Mesh Generic Models */
  BLEMesh_SetModelLightCbMap(&Model_Light_cb);
  
  BLEMesh_AddLightingModels();
  
  /* Callbacks used by BlueNRG-Mesh Sensor Models */
  BLEMesh_SetModelSensorCbMap(&Model_Sensor_cb);
  
  BLEMesh_AddSensorModels();
}

/**
* @brief  Process Commands for Models
* @param  void
* @retval void
*/    
void BLEMesh_ModelsProcess(void)
{
  Generic_Process();
  Lighting_Process();
  Vendor_Process();
  Sensor_Process();
}

/**
* @brief  Publish Command for Models
* @param  void
* @retval void
*/    
void BLEMesh_ModelsCommand(void)
{
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 elementIndex;
  
  /*Select the Element Number for which publication address is required*/
  
  if (NumberOfElements == 1)
  {
    elementNumber = 0x01; 
  }
  
  else if(NumberOfElements == 2)
  { 
    elementNumber = 0x02; /*Element 2 is configured as switch*/
  }
  
  else if(NumberOfElements == 3)
  {
    elementNumber = 0x03; /*Element 3 is configured as switch*/
  }
  
  publishAddress = BLEMesh_GetPublishAddress(elementNumber);
  elementIndex = elementNumber-1;
 
  if(publishAddress)
  {
#if !defined(DISABLE_TRACES)
    printf("Published Address is= 0x%2x \n\r", publishAddress); 
#endif
   
  }
  else
  {
#if !defined(DISABLE_TRACES)
    printf("Publish Address is unassigned!\r\n"); 
#endif
  }
  
#ifndef VENDOR_MODEL_PUBLISH    
  Generic_Publish(publishAddress, elementIndex);
#endif
  
#ifdef VENDOR_MODEL_PUBLISH   
  Vendor_Publish(publishAddress, elementIndex);
#endif  
}

/**
* @brief  Get the Element Number for selected Model 
* @param  dst_peer : Destination Address received
* @retval MOBLEUINT8 : elementIndex
*/ 
MOBLEUINT8 BLEMesh_ModelsGetElementNumber(MOBLE_ADDRESS dst_peer)
{
  
  MOBLE_ADDRESS nodeAddress;
  MOBLEUINT8 elementNumber;
  
  nodeAddress = BLEMesh_GetAddress();
  elementNumber = ((dst_peer - nodeAddress)+1);
  
  return elementNumber;
}

/**
* @brief  Check Subscription of Elements for Group Address for selected Model 
* @param  dst_peer : Destination Address received
* @param  elementNumber : Number of element to check Subscription
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT BLEMesh_ModelsCheckSubscription(MOBLE_ADDRESS dst_peer, \
  MOBLEUINT8 elementNumber)
{
  MOBLE_RESULT status = MOBLE_RESULT_FAIL;
  MOBLE_ADDRESS subscriptionList[10] = {0};
  MOBLEUINT8 length;
  
  BLEMesh_GetSubscriptionAddress(subscriptionList,&length,elementNumber);
  
  
  for(uint8_t list=0; list<length; list++)
  {
    if(dst_peer == subscriptionList[list])
    {
      status = MOBLE_RESULT_SUCCESS;    
      break;
    }
  }
  
  return status;
}

/**
* @brief  Schedule a packet to be sent with randomized send timestamp  
*         If a que is empty, random timestamp is calculated
*         Subsequent packets are sent in sequence
* @param  peer:    Address of the peer
* @param elementIndex : index of the element
* @param  status:  Command status
* @param  data:    Data buffer.
* @param  length:  Length of data in bytes.
* @retval None
*/ 
void BLEMesh_ModelsDelayPacket(MOBLE_ADDRESS peer, 
                               MOBLEUINT8 elementIndex,
                               MOBLEUINT8 command, 
                               MOBLEUINT8 const * data, 
                               MOBLEUINT32 length)
{
  MOBLEUINT8 random_time[8];
  
  if (Appli_PendingPackets.packet_count == 0)
  {
    Appli_PendingPackets.packet_count = 1;
    hci_le_rand(random_time);
    Appli_PendingPackets.send_time = Clock_Time() + 
      DEFAULT_DELAY_PACKET_FROM + 
        (random_time[0] + random_time[1]*256)\
          %DEFAULT_DELAY_PACKET_RANDOM_TIME;    
    Appli_PendingPackets.head = Appli_PendingPackets.packet;
    Appli_PendingPackets.head_index = 0;
#if !defined(DISABLE_TRACES)	
    printf("Randomized time: %d\n\r", Appli_PendingPackets.send_time - Clock_Time());
#endif	
  }
  else 
  {
    Appli_PendingPackets.packet_count += 1;
    Appli_PendingPackets.packet_count = (Appli_PendingPackets.packet_count)%\
      (MAX_PENDING_PACKETS_QUE_SIZE+1);
    
    if (Appli_PendingPackets.head != (Appli_PendingPackets.packet + \
      MAX_PENDING_PACKETS_QUE_SIZE - 1))
    {
      Appli_PendingPackets.head = Appli_PendingPackets.head +1;
      Appli_PendingPackets.head_index = Appli_PendingPackets.head_index+1;
    }
    else
    {
      Appli_PendingPackets.head = Appli_PendingPackets.packet;
      Appli_PendingPackets.head_index = 0;
    }
  }  
  
  Appli_PendingPackets.head->peer = peer;
  Appli_PendingPackets.head->elementIndex = elementIndex;
  Appli_PendingPackets.head->command = command;
  Appli_PendingPackets.head->length = length;
  for (MOBLEUINT8 count=0; count<length; count++)
    Appli_PendingPackets.head->data[count] = data[count];
}   


/**
* @brief  If send timestamp is reached and que is not empty, send all packets
* @param  None
* @retval None
*/
void BLEMesh_ModelsSendDelayedPacket(void)
{
  APPLI_SEND_RESPONSE_MODULE* ptr;
  MOBLEUINT8 temp_index;
  
  if ((Appli_PendingPackets.packet_count != 0) && 
      (Appli_PendingPackets.send_time <= Clock_Time()))
  {
    for (MOBLEUINT8 count=Appli_PendingPackets.packet_count; count!=0; count--)
    {
#if !defined(DISABLE_TRACES)		
      printf("Sending randomized packets. Packet count: %d \n\r",\
        Appli_PendingPackets.packet_count - count + 1);
#endif      
      temp_index = ((Appli_PendingPackets.head_index+MAX_PENDING_PACKETS_QUE_SIZE+1)\
        -count)%MAX_PENDING_PACKETS_QUE_SIZE;
      ptr = Appli_PendingPackets.packet + temp_index;
      
      BLEMesh_SendResponse(ptr->peer,
                           ptr->elementIndex,
                           ptr->command,
                           ptr->data,
                           ptr->length);
    }
    
    Appli_PendingPackets.packet_count = 0;
  }
}


/**
* @brief  Convert ASCII value into Character
* @param  tempValue : 8bit value for conversion
* @retval MOBLEUINT8 
*/         
MOBLEUINT8 BLEMesh_ModelsASCII_To_Char(MOBLEUINT8 tempValue)
{
  tempValue = tempValue - 0x30;
  return tempValue;
} 

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
