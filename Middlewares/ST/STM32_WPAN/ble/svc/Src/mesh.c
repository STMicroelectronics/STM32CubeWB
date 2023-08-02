/**
  ******************************************************************************
  * @file    mesh.c
  * @author  MCD Application Team
  * @brief   Mesh
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
#include "common_blesvc.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "models_if.h"
#include "mesh_cfg.h"
#include "appli_config.h"
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

const MOBLE_CONFIG_MODEL_CB_MAP config_model_cb = 
{
  Appli_GetAppKeyAddParamsCb,
  Appli_GetAppKeyDeleteParamsCb,
  Appli_GetAppKeyListParamsCb,
  Appli_GetAppKeyUpdateParamsCb,
  Appli_GetNetKeyAddParamsCb,
  Appli_GetNetKeyDeleteParamsCb,
  Appli_GetNetKeyListParamsCb,
  Appli_GetNetKeyUpdateParamsCb,
  Appli_GetAppKeyBindingParamsCb,
  Appli_GetAppKeyUnBindingParamsCb,
  Appli_GetSubAddParamsCb,
  Appli_GetSubDeleteParamsCb,
  Appli_GetSubOverwriteParamsCb,
  Appli_GetPublicationSetParamsCb,
  Appli_GetPublicationGetParamsCb
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
  
  /*Set BLE Config Model APIs callback*/
  BLEMesh_BleConfigModelInitCallBack(&config_model_cb);

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
  
  /*Check SIG & VENDOR models limit*/
  if(MOBLE_FAILED(AppliCheck_EnabledModelsList()))
  {
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
    5 Continuous cycles of OnOff with Ontime <2 sec will cause unprovisioning
  */
  Appli_Init(&PrvnDevKeyFlag);
  
/* Check if Node is a Provisioner */
#if PROVISIONER_FEATURE
  
  Appli_StartProvisionerMode(1);
  
#else
  /* Checks if the node is already provisioned or not */
  if (BLEMesh_IsUnprovisioned() == MOBLE_TRUE)
  {
    BLEMesh_InitUnprovisionedNode(); /* Initializes  Unprovisioned node */

    TRACE_I(TF_PROVISION,"Unprovisioned device \r\n");
    
#if PB_ADV_SUPPORTED
    BLEMesh_SetUnprovisionedDevBeaconInterval(100);
#endif    
  }
  else
  {
    BLEMesh_InitProvisionedNode();  /* Initializes  Provisioned node */
    TRACE_I(TF_PROVISION,"Provisioned node \r\n");
    TRACE_I(TF_INIT,"Provisioned Node Address: [%04x] \n\r", BLEMesh_GetAddress());       
  }
#endif  
  /* Initializes the Application */
  /* This function also checks for Power OnOff Cycles     
     Define the following Macro "ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE" 
     to check the Power-OnOff Cycles
    5 Continuous cycles of OnOff with Ontime <2 sec will cause unprovisioning
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

  /* Models initialization */  
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
  TRACE_I(TF_MISC,"[Features Supported] \r\n");

#ifdef ENABLE_RELAY_FEATURE
  TRACE_I(TF_MISC,"Relay = Enabled \r\n");
#endif 
  
#ifdef ENABLE_PROXY_FEATURE
  TRACE_I(TF_MISC,"Proxy = Enabled \r\n");
#endif
  
#ifdef ENABLE_FRIEND_FEATURE
  TRACE_I(TF_MISC,"Friend = Enabled \r\n");
#endif  
  
#ifdef ENABLE_LOW_POWER_FEATURE
  TRACE_I(TF_MISC,"Low Power = Enabled \r\n");
#endif  
  
#ifdef ENABLE_PROVISIONER_FEATURE
  TRACE_I(TF_MISC,"Provisioner = Enabled \r\n");
#endif  
  
#ifdef DYNAMIC_PROVISIONER
  TRACE_I(TF_MISC,"Dynamic Provisioner = Enabled \r\n");
#endif  
  
  TRACE_I(TF_MISC,"\n[Options] \r\n");
  
#ifdef ENABLE_PB_ADV
  TRACE_I(TF_MISC,"PB-ADV = Enabled \r\n");
#endif
  
#ifdef ENABLE_PB_GATT  
  TRACE_I(TF_MISC,"PB-GATT = Enabled \r\n");
#endif
  
#ifdef ENABLE_PUB_KEY_TYPE_OOB
  TRACE_I(TF_MISC,"Public Key OOB = Enabled \r\n");
#endif

#ifdef ENABLE_AUTH_TYPE_STATIC_OOB
  TRACE_I(TF_MISC,"Static OOB  = Enabled \r\n");
#endif
      
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  TRACE_I(TF_MISC,"Output OOB = Enabled \r\n");
#endif
  
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
  TRACE_I(TF_MISC,"Input OOB = Enabled \r\n");
#endif
  
  TRACE_I(TF_MISC,"\n[Library Capabilities] \r\n");
  
  TRACE_I(TF_MISC,"Net Keys = %d \r\n", BLEMesh_GetNetKeysCount());
  TRACE_I(TF_MISC,"App Keys = %d \r\n", BLEMesh_GetAppKeysCount());
  TRACE_I(TF_MISC,"Elements per Node = %d \r\n", BLEMesh_GetNumberOfElements());
  TRACE_I(TF_MISC,"Models per Element = %d \r\n", BLEMesh_GetTotalModelsCount());
  TRACE_I(TF_MISC,"Subscription per Model = %d \r\n", BLEMesh_GetSubscriptionCount());
  TRACE_I(TF_MISC,"Default TTL = %d \r\n", BLEMesh_GetTTL());
  TRACE_I(TF_MISC,"Default Network Transmit Count = %d \r\n", BLEMesh_GetNetworkTransmitCount());
  TRACE_I(TF_MISC,"Default Relay retransmit Count = %d \r\n", BLEMesh_GetRelayRetransmitCount());
  
  TRACE_I(TF_MISC,"\n[Enabled Models] \r\n");
  
  for(uint8_t elementCount=0; elementCount<APPLICATION_NUMBER_OF_ELEMENTS; elementCount++)  
  {
    TRACE_I(TF_MISC,"For Element Index = %d or Element Number = %d \r\n", elementCount, (elementCount+1));
#ifdef ENABLE_VENDOR_MODEL_SERVER    
    if ((ENABLE_VENDOR_MODEL_SERVER & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Vendor Server \r\n");
    }
#endif
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
    if ((ENABLE_GENERIC_MODEL_SERVER_ONOFF & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic On Off Server \r\n");
    }
#endif
 
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF
    if ((ENABLE_GENERIC_MODEL_CLIENT_ONOFF & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic On Off Client \r\n");
    }
#endif
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
    if ((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Level Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL
    if ((ENABLE_GENERIC_MODEL_CLIENT_LEVEL & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Level Client \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
    if ((ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Default Transition Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME
    if ((ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Default Transition Client \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
    if ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Power On Off Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
    if ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Power On Off Setup Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF
    if ((ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Power On Off Client \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL
    if((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL & (1 << elementCount)) == (1 << elementCount))
    { 
      TRACE_I(TF_MISC,"Generic Power Level Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY
    if((ENABLE_GENERIC_MODEL_SERVER_BATTERY & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Battery Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION
    if((ENABLE_GENERIC_MODEL_SERVER_LOCATION & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Location Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY
    if((ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Admin Property Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY
    if((ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic Manufacturer Property Server \r\n");
    }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY
    if((ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Generic User Property Server \r\n");
    }
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
    if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light Lightness Server \r\n");       
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
    if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light Lightness Setup Server \r\n");       
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS
    if((ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light Lightness Client \r\n");       
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
    if((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light CTL Server \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
    if((ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light CTL Setup Server \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL
    if((ENABLE_LIGHT_MODEL_CLIENT_CTL & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light CTL Client \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
    if((ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light CTL Temperature Server \r\n");
    } 
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
    if((ENABLE_LIGHT_MODEL_SERVER_HSL & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light HSL Server \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
    if((ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light HSL Setup Server \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL
    if((ENABLE_LIGHT_MODEL_CLIENT_HSL & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light HSL Client \r\n");
    }
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
    if((ENABLE_LIGHT_MODEL_SERVER_HSL_HUE & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light HSL Hue \r\n");
    } 
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
    if((ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light HSL Saturation Server \r\n");
    } 
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_XYL
    if((ENABLE_LIGHT_MODEL_SERVER_XYL & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Light XYL Server \r\n");
    } 
#endif 

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
    if((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light LC Server \r\n");
    } 
#endif 

#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC
    if((ENABLE_LIGHT_MODEL_CLIENT_LC & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Light LC Client \r\n");
    } 
#endif 

#ifdef ENABLE_SENSOR_MODEL_SERVER
    if((ENABLE_SENSOR_MODEL_SERVER & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Sensor Server \r\n");   
    }
#endif

#ifdef ENABLE_SENSOR_MODEL_CLIENT
    if((ENABLE_SENSOR_MODEL_CLIENT & (1 << elementCount)) == (1 << elementCount))
    {  
      TRACE_I(TF_MISC,"Sensor Client \r\n");   
    }
#endif

#ifdef ENABLE_TIME_MODEL_SERVER 
    if((ENABLE_TIME_MODEL_SERVER & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Time Server \r\n");            
    }
#endif
          
#ifdef ENABLE_SCENE_MODEL_SERVER 
    if((ENABLE_SCENE_MODEL_SERVER & (1 << elementCount)) == (1 << elementCount))
    {
      TRACE_I(TF_MISC,"Scene \r\n");           
    }
#endif
  }
  
TRACE_I(TF_MISC,"\n[Important Information] \r\n");
   
#ifdef ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE
   TRACE_I(TF_MISC,"To Unprovision : Do Power On-Off/Reset %d time \r\n", POWER_ON_OFF_CYCLES_FOR_UNPROVISIONING);
#endif
      
#ifdef ENABLE_SAVE_MODEL_STATE_NVM
  TRACE_I(TF_MISC,"Models data will be saved in Flash \r\n");
#endif
  
/* Only one Macro will be enable at one time */
#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES         
  TRACE_I(TF_MISC,"Models data for all messages will be saved \r\n");
#endif
  
#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION
  TRACE_I(TF_MISC,"Models data will be saved on Power failure \r\n");
#endif
  
#if SAVE_EMBD_PROVISION_DATA     
  TRACE_I(TF_MISC,"Embedded Provisioner data saving enabled \r\n");
#endif
  
#ifdef APPLICATION_NUMBER_OF_ELEMENTS     
  TRACE_I(TF_MISC,"Number of Elements enabled in Application: %d \r\n", APPLICATION_NUMBER_OF_ELEMENTS);  
#endif
  
#ifdef ENABLE_NEIGHBOR_TABLE
  TRACE_I(TF_MISC,"Neighbour Table is enabled \r\n");
#endif
  
  TRACE_I(TF_MISC,"********************\r\n\r\n");  

}


