/**
******************************************************************************
* @file    serial_prvn.c
* @author  MCD Application Team
* @brief   Embedded provisioner Serial Control file 
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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "serial_if.h"
#include "serial_ctrl.h"
#include "mesh_cfg.h"
#include "ble_mesh.h"
#include "appli_config_client.h"
#include "serial_prvn.h"
#include "appli_mesh.h"

/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup Middlewares_Serial_Interface
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static neighbor_params_t NeighborTable[5]; 
static MOBLEUINT8 NoOfNeighborPresent;
static MOBLEUINT8 PrvningInProcess = 0;
static MOBLEUINT16 PrvndNodeAddress = 0;
extern MOBLEUINT16 nodeAddressOffset;
/* Private function prototypes -----------------------------------------------*/
static MOBLE_RESULT SerialPrvn_ProvisionDevice(char *text);
static MOBLE_RESULT SerialPrvn_UnProvisionDevice(char *text);
static MOBLE_RESULT SerialPrvn_ScanDevices(char *text);
/* Private functions ---------------------------------------------------------*/ 
/**
* @brief  This function scans and prints unprovisioned devices  
* @param  unprovDeviceArray: Pointer of an array for filling unprovisioned device UUIDs
* @param  noOfUnprovDevices: Pointer to take total count of nearby unprovisioned devices
* @retval MOBLE_RESULT
*/  
__weak MOBLE_RESULT BLEMesh_ScanDevices(neighbor_params_t *unprovDeviceArray, MOBLEUINT8 *noOfUnprovDevices)
{
  return MOBLE_RESULT_NOTIMPL;
}
/**
* @brief  This function returns starts the provisioning of one of the devices
* @param  unprovDeviceArray: Pointer of an array having unprovisioned device UUIDs
* @param  index: Index of the device to be provisioned
* @retval MOBLE_RESULT
*/  
__weak MOBLE_RESULT BLEMesh_ProvisionDevice(neighbor_params_t *unprovDeviceArray, MOBLEUINT16 index)
{
    return MOBLE_RESULT_NOTIMPL;
}
/**
* @brief  This function is used to parse the string given by the user
* @param  rcvdStringBuff: buffer to store input string 
* @param  rcvdStringSize: length of the input string 
* @retval void
*/ 
void SerialPrvn_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLE_RESULT result = MOBLE_RESULT_INVALIDARG;

  /* Command to make a devices as Root node which creates Mesh network credentials */
  if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "ROOT",4))
  {   
#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)
      /* Initializes Mesh network parameters */
    Appli_StartProvisionerMode(1);
    result = MOBLE_RESULT_SUCCESS;
#endif      
  }
  /* Command to scan the unprovisioned devices */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "SCAN",4))
  {   
      result = SerialPrvn_ScanDevices(rcvdStringBuff+COMMAND_OFFSET);
  }
   /* Command to start the unprovisioned devices */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "PRVN-",4))
  {   
      if(!PrvningInProcess)
      {
          result = SerialPrvn_ProvisionDevice(rcvdStringBuff+COMMAND_OFFSET);
      }
      else
      {
          BLEMesh_PrintStringCb("Link opened already. Wait.\r\n");
          result = MOBLE_RESULT_FAIL;
      }
  }
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "UNPV",4))
  {
      
    result = SerialPrvn_UnProvisionDevice(rcvdStringBuff+COMMAND_OFFSET);
      
  }
     /* Command to start the unprovisioned devices */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "RESET",5))
  {
        BLEMesh_PrintStringCb("Resetting Board...\r\n");
        NVIC_SystemReset();
  }
  /* Command to scan the unprovisioned devices - Used By node only */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "NDSCAN",4))
  {   
      result = BLEMesh_ScanDevices(NeighborTable, &NoOfNeighborPresent);
  }
   /* Command to start the unprovisioned devices - Used By node only */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "NDPRVN-",4))
  {   
      MOBLEINT16 index = 0;  
      sscanf(rcvdStringBuff, "PRVN-%hd", &index);
      result = BLEMesh_ProvisionDevice(NeighborTable, index);
  }
  else
  {
      BLEMesh_PrintStringCb("Invalid Command\r\n");
      return;
  }
  
  /* Check the result of command processing */
  if(result == MOBLE_RESULT_SUCCESS)
  {
      BLEMesh_PrintStringCb("Test command executed successfully\r\n");
  }
  else if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
      BLEMesh_PrintStringCb("Test command Failed. Out of memory\r\n");  
  }
  else if(result == MOBLE_RESULT_INVALIDARG)
  {
      BLEMesh_PrintStringCb("Test command Failed. Invalid Argument\r\n");  
  }
  else
  {
       BLEMesh_PrintStringCb("Test command Failed.\r\n");   
  }
}

/**
* @brief  This function returns starts the provisioning of one of the devices
* @param  text: received array
* @retval MOBLE_RESULT
*/  
static MOBLE_RESULT SerialPrvn_ProvisionDevice(char *text)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    
#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)  
  MOBLEINT16 index = 0;
//  MOBLEINT16 na = 0;

  
//  sscanf(text, "PRVN-%hd %hd", &index, &na);  
  sscanf(text, "PRVN-%hd", &index);  
//  if(na>1)
  { /* Address 1 is reserved for the Provisioner */
    
      result = BLEMesh_ProvisionRemote(NeighborTable[index].uuid);
      
      /* Make copy of the UUID */
      memcpy (NodeUnderProvisionParam.NewProvNodeUUID, 
              NeighborTable[index].uuid,
              UUID_SIZE);
  }
//  else 
//  {
//      result = MOBLE_RESULT_INVALIDARG;
//  }
  
#endif  
  return result;
}

/**
* @brief  This function Un-Provision one of the devices
* @param  text: received array
* @retval MOBLE_RESULT
*/  
static MOBLE_RESULT SerialPrvn_UnProvisionDevice(char *text)
{
  MOBLEINT16 na = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  sscanf(text, "UNPV %hd", &na);  
  if(na>1)
  {
    result = ConfigClient_NodeReset(na);
  }
  else if(na == 1)
  {
    if(!BLEMesh_IsUnprovisioned())
    {
      Appli_Unprovision();
    }
    else
    {
      BLEMesh_PrintStringCb("Device is already unprovisioned !\r\n");
    }
  }
  else 
  {
    result = MOBLE_RESULT_INVALIDARG;
  }

  return result;
}

/**
* @brief  This function scans and prints unprovisioned devices  
* @param  text: received array
* @retval MOBLE_RESULT
*/  
static MOBLE_RESULT SerialPrvn_ScanDevices(char *text)
{
  MOBLE_RESULT result;
  
    result = BLEMesh_GetNeighborState(NeighborTable,&NoOfNeighborPresent);
    /* Check if any unprovisioned device is available */
    if(!NoOfNeighborPresent)
    {
      TRACE_I(TF_PROVISION,"No Unprovisioned Device Nearby\r\n");  
    }
    else
    {
      for(MOBLEINT8 count=0; count < NoOfNeighborPresent; count++)
      {
        BLEMesh_PrintStringCb("");  
        TRACE_I(TF_PROVISION,"Device-%d -> ", count);  
        BLEMesh_PrintDataCb(NeighborTable[count].uuid, 16);
      }
    }
  return result;
  }
/**
* @brief  This funcrion is used to update the status of the provisioning
* @retval void
*/ 
void SerialPrvn_ProvisioningStatusUpdateCb(uint8_t flagPrvningInProcess, MOBLEUINT16 nodeAddress)
{
    PrvningInProcess = flagPrvningInProcess;
    PrvndNodeAddress = nodeAddress;
}
/**
* @}
*/

/**
* @}
*/

