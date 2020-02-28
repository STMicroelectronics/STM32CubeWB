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
#include "appli_config_client.h"
#include "appli_nvm.h"


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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* This structure contains Mesh library Initialisation info data */
const Mesh_Initialization_t BLEMeshlib_Init_params = 
{
  bdaddr,
  &TrParams,
  &FnParams,
  &LpnParams,
  &NeighborTableParams,
  BLUENRG_MESH_FEATURES,
  BLUENRG_MESH_PRVN_BEARER_INFO,
  &PrvnParams,
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
  MOBLEUINT8 uuid[16];
  MOBLEUINT8 PrvnDevKeyFlag = 0;
#if PROVISIONER_FEATURE
//   MOBLEUINT8 prvsnrDevKey[16];
#endif
  
  /* Check for valid Board Address */
  if (!Appli_CheckBdMacAddr())
  {
    TRACE_I(TF_INIT,"Bad BD_MAC ADDR!\r\n");
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
    TRACE_I(TF_INIT,"Could not initialize BLE-Mesh library!\r\n");   
    /* LED continuously blinks if library fails to initialize */
    while (1)
    {
      Appli_LedBlink();
    }
  }
  
/* Check if Node is a Provisioner */
//#if PROVISIONER_FEATURE
//  MOBLEUINT8 prvsnrDevKey[16];
//  /* Initializes Mesh network parameters */
//  BluenrgMesh_CreateNetwork(prvsnrDevKey);
//  
//  /* Following functions help to Configure the Provisioner to default settings*/
//  ApplicationSetNodeSigModelList();
//  AppliConfigClient_SelfPublicationSetDefault();
//  AppliConfigClient_SelfSubscriptionSetDefault();
//  Appli_ConfigClient_SelfDefaultAppKeyBind();
//
//  TRACE_I(TF_PROVISION,"Provisioner node \r\n");
//  TRACE_I(TF_PROVISION,"Provisioner Dev Key:");
//   for(MOBLEUINT8 i=0;i<16;i++)
//  {
//    TRACE_I(TF_INIT,"[%02x] ",prvsnrDevKey[i]);
//  }
//  TRACE_I(TF_INIT,"\r\n");  
  
//#else
  /* Checks if the node is already provisioned or not */
  if (BLEMesh_IsUnprovisioned() == MOBLE_TRUE)
  {
    BLEMesh_InitUnprovisionedNode(); /* Initalizes Unprovisioned node */

    TRACE_I(TF_PROVISION,"Unprovisioned device \r\n");
    
#if PB_ADV_SUPPORTED
    BLEMesh_SetUnprovisionedDevBeaconInterval(100);
#endif    
  }
  else
  {
    BLEMesh_InitProvisionedNode();  /* Initalizes Provisioned node */
    TRACE_I(TF_PROVISION,"Provisioned node \r\n");
    TRACE_I(TF_INIT,"Provisioned Node Address: [%04x] \n\r", BLEMesh_GetAddress());       
  }
//#endif  
  /* Initializes the Application */
  /* This function also checks for Power OnOff Cycles     
     Define the following Macro "ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE" 
     to check the Power-OnOff Cycles
    5 Continous cycles of OnOff with Ontime <2 sec will cause unprovisioning
  */
  Appli_Init(&PrvnDevKeyFlag);
#if PROVISIONER_FEATURE  
//  AppliNvm_saveProvisionerDevKey(&prvsnrDevKey[0],
//                                 sizeof(prvsnrDevKey),
//                                 &PrvnDevKeyFlag);  
#endif  
  
/* Check to manually unprovision the board */
  Appli_CheckForUnprovision();
  /* Set attention timer callback */
  BLEMesh_SetAttentionTimerCallback(Appli_BleAttentionTimerCb);

  /* Set uuid for the board*/
  Appli_BleSetUUIDCb(uuid);
  
  /* Prints the MAC Address of the board */ 
  TRACE_I(TF_INIT,"BLE-Mesh Lighting Demo v%s\n\r", BLE_MESH_APPLICATION_VERSION); 
  TRACE_I(TF_INIT,"BLE-Mesh Library v%s\n\r", BLEMesh_GetLibraryVersion()); 
  TRACE_I(TF_INIT,"BD_MAC Address = [%02x]:[%02x]:[%02x]:[%02x]:[%02x]:[%02x] \n\r",
          bdaddr[5],bdaddr[4],bdaddr[3],bdaddr[2],bdaddr[1],bdaddr[0]);
  TRACE_I(TF_INIT,"UUID Address = ");
  for(MOBLEUINT8 i=0;i<16;i++)
  {
    TRACE_I(TF_INIT,"[%02x] ",uuid[i]);
  }
  TRACE_I(TF_INIT,"\r\n");
  /* Models intialization */  
  BLEMesh_ModelsInit();

  /* Turn on Yellow LED */
#if (LOW_POWER_FEATURE == 1)
//  BSP_LED_On(LED_RED);
#endif
  
#ifdef CUSTOM_BOARD_PWM_SELECTION  
  Light_UpdatePWMValue((MOBLEUINT8)DEFAULT_STATE);
#endif
}

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
