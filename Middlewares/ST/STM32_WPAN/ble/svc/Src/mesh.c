/**
  ******************************************************************************
  * @file    mesh.c
  * @author  MCD Application Team
  * @brief   Mesh
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
#include "common_blesvc.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "models_if.h"
#include "mesh_cfg.h"
#include "appli_config_client.h"
#include "appli_nvm.h"
#include "shci.h"


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
  &DeviceNameParams,
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
void Appli_ConfigurationInfoDump(void);
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
//#if PROVISIONER_FEATURE
//   MOBLEUINT8 prvsnrDevKey[16];
//#endif
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t *p_wireless_info = &wireless_info_instance;  
  
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
  
  /* Initialization of the  SIG Models list */
  if(MOBLE_FAILED(ApplicationInitSigModelList()))
  {
    TRACE_I(TF_INIT,"The number of Models enabled exceed the limit of %d !\r\n",
            APPLICATION_SIG_MODELS_MAX_COUNT);   
    /* LED continuously blinks if library fails to initialize */
    while (1)
    {
      Appli_LedBlink();
    }
  }
  
  /* Initialization of the  Vendor Models list */
  if(MOBLE_FAILED(ApplicationInitVendorModelList()))
  {
    TRACE_I(TF_INIT,"The number of Models enabled exceed the limit of %d !\r\n",
            APPLICATION_SIG_MODELS_MAX_COUNT);   
    /* LED continuously blinks if library fails to initialize */
    while (1)
    {
      Appli_LedBlink();
    }
  }
  
  /* Initializes the Application */
  /* This function also checks for Power OnOff Cycles     
     Define the following Macro "ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE" 
     to check the Power-OnOff Cycles
    5 Continous cycles of OnOff with Ontime <2 sec will cause unprovisioning
  */
  Appli_Init(&PrvnDevKeyFlag);
  
/* Check if Node is a Provisioner */
#if PROVISIONER_FEATURE
  
  Appli_StartProvisionerMode(1);
  
#else
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
#endif  
  /* Initializes the Application */
  /* This function also checks for Power OnOff Cycles     
     Define the following Macro "ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE" 
     to check the Power-OnOff Cycles
    5 Continous cycles of OnOff with Ontime <2 sec will cause unprovisioning
  */
//  Appli_Init(&PrvnDevKeyFlag);
//#if PROVISIONER_FEATURE  
//  AppliPrvnNvm_SaveProvisionerDevKey(&prvsnrDevKey[0],
//                                     PROVISIONER_ROOT_ADDR_DEFAULT,
//                                     &PrvnDevKeyFlag, 
//                                     APPLICATION_NUMBER_OF_ELEMENTS);  
//#endif  
/* Check to manually unprovision the board */
  Appli_CheckForUnprovision();
  
  /* Set attention timer callback */
  BLEMesh_SetAttentionTimerCallback(Appli_BleAttentionTimerCb);

#if TF_MISC
  Appli_ConfigurationInfoDump();
#endif  

/* Set uuid for the board*/
  Appli_BleSetUUIDCb(uuid);
  
  /* Prints the MAC Address of the board */ 
  TRACE_I(TF_INIT,"BLE-Mesh Lighting Demo v%s\n\r", BLE_MESH_APPLICATION_VERSION); 
  TRACE_I(TF_INIT,"BLE-Mesh Library v%s\n\r", BLEMesh_GetLibraryVersion()); 
  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    // Error
  }
  else
  {
    TRACE_I(TF_INIT,"BLE Stack v%d.%d.%d Branch=%d Type=%d\n\r", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub, p_wireless_info->VersionBranch, p_wireless_info->VersionReleaseType);
    TRACE_I(TF_INIT,"FUS v%d.%d.%d\n\r", p_wireless_info->FusVersionMajor, p_wireless_info->FusVersionMinor, p_wireless_info->FusVersionSub);
  }
  
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


/**
* @brief This function is the Information dump function
* @param  None
* @retval None
*/
void Appli_ConfigurationInfoDump(void)
{
  TRACE_I(TF_MISC,"\r\n********************\r\n");
  
#ifdef ENABLE_PB_ADV
  TRACE_I(TF_MISC,"PB-ADV Enabled \r\n");
#endif   

#ifdef ENABLE_PB_GATT  
  TRACE_I(TF_MISC,"PB-GATT Enabled \r\n");
#endif  

#ifdef ENABLE_RELAY_FEATURE
  TRACE_I(TF_MISC,"Feature: Relay Enabled \r\n");
#endif 
  
#ifdef ENABLE_PROXY_FEATURE
  TRACE_I(TF_MISC,"Feature: Proxy Enabled \r\n");
#endif
  
#ifdef ENABLE_FRIEND_FEATURE
  TRACE_I(TF_MISC,"Feature: Friend Enabled \r\n");
#endif  
  
#ifdef ENABLE_PUB_KEY_TYPE_OOB
  TRACE_I(TF_MISC,"Public Key OOB Enabled \r\n");
#endif
  
#ifdef ENABLE_AUTH_TYPE_STATIC_OOB
  TRACE_I(TF_MISC,"Static OOB Enabled \r\n");
#endif
  
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  TRACE_I(TF_MISC,"Output OOB Enabled \r\n");
#endif
  
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
  TRACE_I(TF_MISC,"Input OOB Enabled \r\n");
#endif

#ifdef ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE
  TRACE_I(TF_MISC,"To Unprovision: Do Power On-Off/Reset %d time \r\n", POWER_ON_OFF_CYCLES_FOR_UNPROVISIONING);
#endif
      
#ifdef ENABLE_SAVE_MODEL_STATE_NVM
  TRACE_I(TF_MISC,"Models data will be saved in Flash \r\n");
#endif
  
/* Only one Macro will be enable at one time */
#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES         
  TRACE_I(TF_MISC,"Models data for all messages wll be saved \r\n");
#endif
  
#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION
  TRACE_I(TF_MISC,"Models data will be saved on Power failure \r\n");
#endif
  
#if SAVE_EMBD_PROVISION_DATA     
  TRACE_I(TF_MISC,"Embedded Provisioner data saving enabled \r\n");
#endif
  
#ifdef APPLICATION_NUMBER_OF_ELEMENTS     
  TRACE_I(TF_MISC,"Number of Elements enabled: %d \r\n", APPLICATION_NUMBER_OF_ELEMENTS);  
#endif
  
#ifdef ENABLE_NEIGHBOR_TABLE
  TRACE_I(TF_MISC,"Neighbour Table is enabled \r\n");
#endif
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
  TRACE_I(TF_MISC,"Generic On Off Server Model enabled \r\n");
#endif
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
  TRACE_I(TF_MISC,"Generic Level Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
  TRACE_I(TF_MISC,"Generic Default Transition Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
  TRACE_I(TF_MISC,"Generic Power On Off Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL
  TRACE_I(TF_MISC,"Generic Power Level Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY
  TRACE_I(TF_MISC,"Generic Battery Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION
  TRACE_I(TF_MISC,"Generic Location Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY
  TRACE_I(TF_MISC,"Generic Admin Property Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY
  TRACE_I(TF_MISC,"Generic Manufacturer Property Server Model enabled \r\n");
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY
  TRACE_I(TF_MISC,"Generic User Property Server Model enabled \r\n");
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
  TRACE_I(TF_MISC,"Light Lightness Server Model enabled \r\n");       
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
  TRACE_I(TF_MISC,"Light CTL Server Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
  TRACE_I(TF_MISC,"Light CTL Temperature Server Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
  TRACE_I(TF_MISC,"Light HSL Server Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
  TRACE_I(TF_MISC,"Light HSL Hue Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
  TRACE_I(TF_MISC,"Light HSL Saturation Server Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_XYL
  TRACE_I(TF_MISC,"Light XYL Server Model enabled \r\n");
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
  TRACE_I(TF_MISC,"Light LC Server Model enabled \r\n");
#endif 

#ifdef ENABLE_SENSOR_MODEL_SERVER
  TRACE_I(TF_MISC,"Sensor Server Model enabled \r\n");   
#endif

#ifdef ENABLE_TIME_MODEL_SERVER 
  TRACE_I(TF_MISC,"Time Server Model enabled \r\n");            
#endif
          
#ifdef ENABLE_SCENE_MODEL_SERVER 
  TRACE_I(TF_MISC,"Scene Model enabled \r\n")           
#endif
  TRACE_I(TF_MISC,"********************\r\n\r\n");  

}

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
