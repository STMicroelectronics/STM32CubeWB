/**
  ******************************************************************************
  * @file    mesh.c
  * @author  MCD Application Team
  * @brief   Mesh
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright c 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "models_if.h"
#include "mesh_cfg.h"


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
const MOBLE_USER_BLE_CB_MAP user_ble_cb =
{  
  Appli_BleStackInitCb,
  Appli_BleSetTxPowerCb,
  Appli_BleGattConnectionCompleteCb,
  Appli_BleGattDisconnectionCompleteCb,
  Appli_BleUnprovisionedIdentifyCb,
  Appli_BleSetUUIDCb,
  Appli_BleSetProductInfoCB,
  Appli_BleSetNumberOfElementsCb,
  Appli_BleDisableFilterCb
};

/* This structure contains Mesh library Initialisation info data */
const Mesh_Initialization_t BLEMeshlib_Init_params = 
{
  bdaddr,
  &TrParams,
  &FnParams,
  &LpnParams,
  MESH_FEATURES,
  &DynBufferParam
};

extern MOBLEUINT8 bdaddr[]; 

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* This structure contains Mesh library Initialisation info data */
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Mesh initialization
 * @param  None
 * @retval None
 */
void MESH_Init(void)
{
  /* Check for valid Board Address */
  if (!Appli_CheckBdMacAddr())
  {

#ifdef TEST
    PrintCrt("Bad BD_MAC ADDR!\r\n");
#else
    printf("Bad BD_MAC ADDR!\r\n");
#endif
    /* LED Blinks if BDAddr is not appropriate */
    while (1)
    {
      Appli_LedBlink();
    }
  }
  
  /* Set BLE configuration function callbacks */
  BLEMesh_BleHardwareInitCallBack(&user_ble_cb);  
  
  /* Initializes BLE-Mesh Library */
  if (MOBLE_FAILED(BLEMesh_Init(&BLEMeshlib_Init_params)))
  {
    BLE_DBG_SVCCTL_MSG("Could not initialize BLE-Mesh library!\r\n");
    /* LED continuously blinks if library fails to initialize */
    while (1)
    {
      Appli_LedBlink();
    }
  }
  
  /* Checks if the node is already provisioned or not */
  if (BLEMesh_IsUnprovisioned() == MOBLE_TRUE)
  {
    BLEMesh_UnprovisionedNodeInfo(&UnprovNodeInfoParams);
    BLEMesh_InitUnprovisionedNode(); /* Initalizes Unprovisioned node */
  }
  else
  {
    BLEMesh_InitProvisionedNode();  /* Initalizes Provisioned node */
#ifdef TEST
    PrintCrt("Node is already provisioned \r\n");
#else
    /* printf("Node is already provisioned \r\n"); */
#endif
  }
  
  /* Models intialization */  
  BLEMesh_ModelsInit();

  /* Set attention timer callback */
  BLEMesh_SetAttentionTimerCallback(Appli_BleAttentionTimerCb);

  /* Prints the MAC Address of the board */
#ifdef TEST
  PrintCrt("BLE-Mesh Lighting Demo v%s\n\r", BLE_MESH_APPLICATION_VERSION); 
  PrintCrt("BLE-Mesh Library v%s\n\r", BLEMesh_GetLibraryVersion()); 
  PrintCrt("MAC Address = [%02x]:[%02x]:[%02x]:[%02x]:[%02x]:[%02x] \n\r",
           bdaddr[5],bdaddr[4],bdaddr[3],
           bdaddr[2],bdaddr[1],bdaddr[0] );
#else
/*	
  printf("BLE-Mesh Lighting Demo v%s\n\r", BLE_MESH_APPLICATION_VERSION); 
  printf("BLE-Mesh Library v%s\n\r", BLEMesh_GetLibraryVersion()); 
  printf("MAC Address = [%02x]:[%02x]:[%02x]:[%02x]:[%02x]:[%02x] \n\r",
           bdaddr[5],bdaddr[4],bdaddr[3],
           bdaddr[2],bdaddr[1],bdaddr[0] );
*/					 
#endif
  
  Appli_CheckForUnprovision();/* Check to unprovision the board manually */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
