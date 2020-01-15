/**
******************************************************************************
* @file    serial_prvn.c
* @author  BLE Mesh Team
* @version V1.11.000
* @date    25-07-2019
* @brief   Embedded provisioner Serial Control file 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "serial_if.h"
#include "serial_ctrl.h"
#include "mesh_cfg.h"
#include "ble_mesh.h"

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

/* Private function prototypes -----------------------------------------------*/
static MOBLE_RESULT SerialPrvn_ProvisionDevice(char *text);
static MOBLE_RESULT SerialPrvn_ScanDevices(char *text);
/* Private functions ---------------------------------------------------------*/ 
/**
* @brief  This funcrion is used to parse the string given by the user
* @param  rcvdStringBuff: buffer to store input string 
* @param  rcvdStringSize: length of the input string 
* @retval void
*/ 
void SerialPrvn_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLE_RESULT result;
                 
  /* Command to scan the unprovisioned devices */
  if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "SCAN",4))
  {   
      result = SerialPrvn_ScanDevices(rcvdStringBuff+COMMAND_OFFSET);
  }
   /* Command to start the unprovisioned devices */
  else if (!strncmp(rcvdStringBuff+COMMAND_OFFSET, "PRVN-",4))
  {   
      result = SerialPrvn_ProvisionDevice(rcvdStringBuff+COMMAND_OFFSET);
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
* @param  text: recieved array
* @retval MOBLE_RESULT
*/  
static MOBLE_RESULT SerialPrvn_ProvisionDevice(char *text)
{
  MOBLEINT16 index = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  sscanf(text, "PRVN-%hd", &index);
  result = BLEMesh_ProvisionRemote(NeighborTable[index].uuid);
  
  return result;
}

/**
* @brief  This function scans and prints unprovisioned devices  
* @param  text: recieved array
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
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
