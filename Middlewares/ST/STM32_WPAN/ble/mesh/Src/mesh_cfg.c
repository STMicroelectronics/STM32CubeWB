/**
 ******************************************************************************
 * @file    mesh_cfg.c
 * @author  BLE Mesh Team
 * @brief   User configurable settings for BLE-Mesh
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
#include "mesh_cfg.h"
#include "mesh_cfg_usr.h"
#include "types.h"
#include "ble_mesh.h"
#include "compiler.h"
#include "string.h"
#include "generic.h"
#include "light.h"
#include "sensors.h"
#include "vendor.h"
#include "config_client.h"
#include "light_lc.h"
#include "appli_mesh.h"
#include <string.h>
#include "time_scene.h"

/* Note: Please use Full Library configuration in project options to use the full 
         configuration of the C/C++ runtime library for printf and scanf functionality */
#if ((ENABLE_SERIAL_CONTROL) | (ENABLE_UT) | ((ENABLE_SERIAL_INTERFACE)))
  #if ((!ENABLE_USART) || (!ENABLE_SERIAL_INTERFACE))
    #error "Serial Interface also needed for Serial Control Application."
  #else
    #include "serial_if.c"
    #if ENABLE_SERIAL_CONTROL
      #include "serial_ctrl.c"
    #endif
    #if ENABLE_UT
      #include "serial_ut.c"
    #endif
    #if ENABLE_APPLI_TEST
      #include "appli_test.c"   
    #endif       
    #if ENABLE_SERIAL_PRVN
      #include "serial_prvn.c"   
    #endif    
  #endif
#endif

#if (LOW_POWER_FEATURE == 0)
MOBLE_RESULT BnrgmFrndLpnProcess(void* param);
MOBLE_RESULT BnrgmFrndLpnProcessFrndOffer(void* param);
MOBLE_RESULT BnrgmFrndLpnProcessFrndUpdate(void* param);
MOBLE_RESULT BnrgmFrndLpnProcessFrndRfu(void* param);
MOBLE_RESULT BnrgmFrndLpnProcessFrndSubscrListConf(void* param);
MOBLE_RESULT BnrgmFrndLpnSwitchKeys(void* param);
MOBLE_RESULT BnrgmFrndLpnTranspSegRecvd(void* param);
MOBLE_RESULT BnrgmFrndLpnUpdateKeys(void* param);
MOBLE_RESULT BnrgmFrndMgmtLpnInit(void* param);
#endif /* #if (LOW_POWER_FEATURE == 0) */

#if (FRIEND_FEATURE == 0)
void BnrgmFrndFnProcess(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndClear(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndClearConf(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndPoll(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndRequest(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndSubscrListAdd(void* param);
MOBLE_RESULT BnrgmFrndFnProcessFrndSubscrListRem(void* param);
MOBLE_RESULT BnrgmFrndFnSendFrndClear(void* param);
MOBLE_RESULT BnrgmFrndFnSwitchKeys(void* param);
MOBLE_RESULT BnrgmFrndFnUpdateKeys(void* param);
MOBLE_RESULT BnrgmFrndMgmtFnInit(void* param);
MOBLE_RESULT BnrgmFrndMgmtGetFriendshipStatus(void* param);
MOBLEUINT32 BnrgmFrndMgmtGetLpnPollTimeout(void* param);
MOBLE_RESULT BnrgmFrndMgmtGetSubscrStatus(void* param);
MOBLE_RESULT BnrgmFrndMgmtQueLpnPkt(void* param);
void BnrgmFrndMgmtSendSecurityUpdate(void* param);
#endif /* #if (FRIEND_FEATURE == 0) */


/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__attribute__((aligned(4))) 
MOBLEUINT8 dyn_buffer_m[DYNAMIC_MEMORY_SIZE + \
                        FRIEND_BUFF_DYNAMIC_MEMORY_SIZE + \
                        MAX_APPLICATION_PACKET_SIZE + \
                        SAR_BUFFER_SIZE + \
                        NEIGHBOR_TABLE_DYNAMIC_MEMORY_SIZE] = {0};


#define MODELDATA_SIZE (MAX_APPLICATION_PACKET_SIZE+16)
#define MODELS_BUFFER_SIZE (MODELDATA_SIZE)


#ifdef STATIC_MEMORY_ALLOCATION_IN_APPLI
ALIGN(4)
MOBLEUINT8 aModelsBuff[MODELS_BUFFER_SIZE]; 

ALIGN(4)
MOBLEUINT8 aModelsBuff_Response[MODELS_BUFFER_SIZE]; 

ALIGN(4)
MOBLEUINT8 aLowerTptBuff[MODELS_BUFFER_SIZE]; 

ALIGN(4)
MOBLEUINT8 aLowerTptFnBuff[16];  

ALIGN(4)
MOBLEUINT8 aLowerTptAppBuff[MAX_APPLICATION_PACKET_SIZE+16]; 

ALIGN(4)
MOBLEUINT8 aProvisionParamBuff[448];

ALIGN(4)
MOBLEUINT8 aAppli_SAR_Buffer[24+MAX_APPLICATION_PACKET_SIZE]; 

ALIGN(4)
MOBLEUINT8 aTrans_SegIn_Buffer[40*TPT_SEGMENT_COUNT]; 

ALIGN(4)
MOBLEUINT8 aTrans_Seq0_Buffer[40*TPT_SEGMENT_COUNT]; 

ALIGN(4)
MOBLEUINT8 aTrans_Out_Buffer[12*TPT_SEGMENT_COUNT];    

ALIGN(4)
MOBLEUINT8 aElementsCC[0x3b8];  
#endif

extern const MOBLEUINT8 StaticOobBuff[];
extern const MOBLEUINT8 PubKeyBuff[];
extern const MOBLEUINT8 PrivKeyBuff[];
extern MOBLEUINT8 NumberOfElements;
__attribute__((aligned(4)))const DynBufferParam_t DynBufferParam =
{
  dyn_buffer_m,
  (MOBLEUINT16) (DYNAMIC_MEMORY_SIZE + SAR_BUFFER_SIZE ),
  (MOBLEUINT16) FRIEND_BUFF_DYNAMIC_MEMORY_SIZE,
  (MOBLEUINT16) MAX_APPLICATION_PACKET_SIZE,
  (MOBLEUINT16) NEIGHBOR_TABLE_DYNAMIC_MEMORY_SIZE,
  (MOBLEUINT16) MODELS_BUFFER_SIZE
};




const tr_params_t TrParams = TRANSMIT_RECEIVE_PARAMS;
const lpn_params_t LpnParams = LOW_POWER_NODE_PARAMS;
const fn_params_t FnParams = FRIEND_NODE_PARAMS;
const prvn_params_t PrvnParams = UNPROV_NODE_INFO_PARAMS;
const neighbor_table_init_params_t NeighborTableParams = NEIGHBOR_TABLE_PARAMS;

/*ALIGN(4)*/
__attribute__((aligned(4))) MOBLEUINT16 Appli_SIG_Models[APPLICATION_NUMBER_OF_ELEMENTS][APPLICATION_SIG_MODELS_MAX_COUNT];
 
MOBLEUINT32 Appli_Vendor_Models[APPLICATION_NUMBER_OF_ELEMENTS][APPLICATION_VENDOR_MODELS_MAX_COUNT];

/* Private functions ---------------------------------------------------------*/

/** 
* @brief ApplicationInitVendorModel: This function initialize the list of the 
*                                     Vendor Models.
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLE_RESULT ApplicationInitVendorModelList(void)
{
  for(uint8_t index = 0; index < APPLICATION_NUMBER_OF_ELEMENTS; index++)
  {
    if(index == 0)
    {
      Appli_Vendor_Models[index][0] = VENDORMODEL_STMICRO_ID1;
      Appli_Vendor_Models[index][1] = NO_MODEL_AVLBL;
    }
    else
    {
      Appli_Vendor_Models[index][0] = NO_MODEL_AVLBL;
    }
  }
  return (MOBLE_RESULT_SUCCESS); 
}

/** 
* @brief ApplicationInitSigModelList: This function initialize the list of the 
*                                     SIG Models.
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLE_RESULT ApplicationInitSigModelList(void)
{
  uint8_t modelCount = 0;    
  
  for(uint8_t index = 0; index < APPLICATION_NUMBER_OF_ELEMENTS; index++)
  {
    uint8_t modelIndex = 0;
    
    Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
#ifdef ENABLE_CONFIG_MODEL_CLIENT
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_CONFIG_MODEL_CLIENT & (1 << index)) == 1) ||
       ((ENABLE_CONFIG_MODEL_CLIENT & (1 << index)) == 2) ||
       ((ENABLE_CONFIG_MODEL_CLIENT & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = SIG_MODEL_ID_CONFIG_CLIENT;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_ONOFF & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_ONOFF & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_ONOFF & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_ONOFF_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_LEVEL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_POWER_ONOFF_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_POWER_LEVEL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_BATTERY & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_BATTERY & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_BATTERY & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_BATTERY_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_LOCATION & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LOCATION & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LOCATION & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_LOCATION_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_LOCATION_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_ADMIN_PROPERTY_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_USER_PROPERTY_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_SERVER_CLIENT_PROPERTY_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places LIGHTING MODEL Enabled SERVER IDs    *********/
    /**************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID;       
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_LIGHTNESS_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_CTL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_CTL_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_CTL_TEMPERATURE_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_HSL & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_HSL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_HSL_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_HSL_HUE & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_HUE & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_HUE & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_HSL_HUE_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_HSL_SATURATION_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places Light XYL MODEL Enabled SERVER IDs   *********/
    /**************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_XYL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_XYL & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_XYL & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_XYL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_XYL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP
    if(modelIndex >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_XYL_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places Light LC MODEL Enabled SERVER IDs    *********/
    /**************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_LC_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_SERVER_LC_SETUP & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LC_SETUP & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_SERVER_LC_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places SENSOR MODEL Enabled SERVER IDs      *********/
    /**************************************************************************/
#ifdef ENABLE_SENSOR_MODEL_SERVER
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_SENSOR_MODEL_SERVER & (1 << index)) == 1) ||
       ((ENABLE_SENSOR_MODEL_SERVER & (1 << index)) == 2) ||
       ((ENABLE_SENSOR_MODEL_SERVER & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = SENSOR_SERVER_MODEL_ID;    
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_SENSOR_MODEL_SERVER_SETUP & (1 << index)) == 1) ||
       ((ENABLE_SENSOR_MODEL_SERVER_SETUP & (1 << index)) == 2) ||
       ((ENABLE_SENSOR_MODEL_SERVER_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = SENSOR_SETUP_SERVER_MODEL_ID; 
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places Time and Scene MODEL Enabled SERVER IDs    ***/
    /**************************************************************************/

#ifdef ENABLE_TIME_MODEL_SERVER
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_TIME_MODEL_SERVER & (1 << index)) == 1) ||
       ((ENABLE_TIME_MODEL_SERVER & (1 << index)) == 2) ||
       ((ENABLE_TIME_MODEL_SERVER & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = TIME_MODEL_SERVER_MODEL_ID;             
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_TIME_MODEL_SERVER_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_TIME_MODEL_SERVER_SETUP & (1 << index)) == 1) ||
       ((ENABLE_TIME_MODEL_SERVER_SETUP & (1 << index)) == 2) ||
       ((ENABLE_TIME_MODEL_SERVER_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = TIME_MODEL_SERVER_SETUP_MODEL_ID;       
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_SCENE_MODEL_SERVER & (1 << index)) == 1) ||
       ((ENABLE_SCENE_MODEL_SERVER & (1 << index)) == 2) ||
       ((ENABLE_SCENE_MODEL_SERVER & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = SCENE_MODEL_SERVER_MODEL_ID;            
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER_SETUP
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_SCENE_MODEL_SERVER_SETUP & (1 << index)) == 1) ||
       ((ENABLE_SCENE_MODEL_SERVER_SETUP & (1 << index)) == 2) ||
       ((ENABLE_SCENE_MODEL_SERVER_SETUP & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = SCENE_MODEL_SERVER_SETUP_MODEL_ID;      
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif

    /**************************************************************************/
    /** Following Section places Generic MODEL Enabled Client IDs    **********/
    /**************************************************************************/ 
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_CLIENT_ONOFF & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_CLIENT_ONOFF & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_CLIENT_ONOFF & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_CLIENT_ONOFF_MODEL_ID;   
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_GENERIC_MODEL_CLIENT_LEVEL & (1 << index)) == 1) ||
       ((ENABLE_GENERIC_MODEL_CLIENT_LEVEL & (1 << index)) == 2) ||
       ((ENABLE_GENERIC_MODEL_CLIENT_LEVEL & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = GENERIC_MODEL_CLIENT_LEVEL_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS
    if(modelCount >= APPLICATION_SIG_MODELS_MAX_COUNT)
      return(MOBLE_RESULT_FAIL);
    if(((ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS & (1 << index)) == 1) ||
       ((ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS & (1 << index)) == 2) ||
       ((ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS & (1 << index)) == 4))
    {
      Appli_SIG_Models[index][modelIndex++] = LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID;
      Appli_SIG_Models[index][modelIndex] = NO_MODEL_AVLBL;
      modelCount++;
    }
#endif
  }
  return (MOBLE_RESULT_SUCCESS); 
}

/** 
* @brief ApplicationGetSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetSigModelList(MOBLEUINT16* pModels_sig_ID, MOBLEUINT8 elementIndex)
{
  uint8_t count=0;
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  for(uint8_t index=0; index <= elementIndex; index++)
  {
    if(index == elementIndex)
    {
      for(uint8_t modelCount=0; modelCount < APPLICATION_SIG_MODELS_MAX_COUNT; modelCount++)
      {
        if(Appli_SIG_Models[index][modelCount] == NO_MODEL_AVLBL)
        {
          memcpy(pModels_sig_ID, Appli_SIG_Models+index, count*2);
          break;
        }
        
        else
        {
          count++;
        }
      }
      
      break;
    }
    
    else
    {
      /*do nothing*/
    }
  }

  return count; 
}


/** 
* @brief ApplicationGetCLIENTSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetCLIENTSigModelList(MOBLEUINT16* pModels_sig_ID, MOBLEUINT8 elementIndex)
{
  
  uint8_t count=0;
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  for(uint8_t index=0; index <= elementIndex; index++)
  {
    if(index == elementIndex)
  {
      for(uint8_t modelCount=0; modelCount < APPLICATION_SIG_MODELS_MAX_COUNT; modelCount++)
      {
        if(Appli_SIG_Models[index][modelCount] == NO_MODEL_AVLBL)
        {
          memcpy(pModels_sig_ID, Appli_SIG_Models+index, count*2);
          break;
        }
        
        else
        {
          count++;
        }
      }
      
      break;
    }
    
    else
    {
      /*do nothing*/
    }
  }
  
  return count; 
}


/** 
* @brief ApplicationGetCLIENTSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationSetNodeSigModelList(void)
{
  BLEMeshSetSelfModelList(NumberOfElements);
  
  return  1; //APPLI_SIG_MODELS_COUNT;  TBD
}


/** 
* @brief ApplicationGetVendorModelList: This function provides the list of the 
*           Vendor Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with Vendor Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the Vendor Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetVendorModelList(MOBLEUINT32* pModels_vendor_ID, MOBLEUINT8 elementIndex)
{
 
  uint8_t count=0;
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  for(uint8_t index=0; index <= elementIndex; index++)
  {
    if(index == elementIndex)
    {
      for(uint8_t modelCount=0; modelCount < APPLICATION_VENDOR_MODELS_MAX_COUNT; modelCount++)
      {
        if(Appli_Vendor_Models[index][modelCount] == NO_MODEL_AVLBL)
        {
          memcpy(pModels_vendor_ID, Appli_Vendor_Models+index, count*4);
          break;
        }
        
        else
        {
          count++;
        }
      }
      
      break;
    }
    
    else
    {
      /*do nothing*/
    }
  }
  
  return count; 
}


/** 
* @brief ApplicationChkSigModelActive: This function checks if a specific 
*          Model Server is active in the Model Server list
* @param modelID: Model Server ID received for the checking function
* @param elementIndex : index of element for which active model checking is needed
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkSigModelActive(MOBLEUINT16 modelID, MOBLEUINT8 elementIndex)
{
  
  MOBLEBOOL result = MOBLE_FALSE;
    
  for(uint8_t index = 0; index <= elementIndex; index++)
  {
    if(index == elementIndex)
    {
      for(uint8_t count = 0; count < APPLICATION_SIG_MODELS_MAX_COUNT; count++)
      {
        if (modelID == Appli_SIG_Models[index][count])      
        {
         result = MOBLE_TRUE;
           break;
        }
        
        else
        {
           /*do nothing*/ 
        }
      }
      break;
    }
    
    else
    {
      /*do nothing*/  
    }
  }
  
  return result;
}


/** 
* @brief ApplicationChkVendorModelActive: This function checks if a specific 
*          Model Server is active in the Vendor Model Server list
* @param modelID: Model Server ID received for the checking function
* @param elementIndex : index of element for which active model checking is needed
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkVendorModelActive(MOBLEUINT32 modelID, MOBLEUINT8 elementIndex)
{
  MOBLEBOOL result = MOBLE_FALSE;
    
  for(uint8_t index = 0; index <= elementIndex; index++)
  {
    if(index == elementIndex)
    {
      for(uint8_t count = 0; count < APPLICATION_VENDOR_MODELS_MAX_COUNT; count++)
      {
        if (modelID == Appli_Vendor_Models[index][count])      
        {
         result = MOBLE_TRUE;
         break;
        }

        else
        {
           /*do nothing*/ 
        }
      }
      break;
    }

    else
    {
      /*do nothing*/  
    }
  }
  
  return result;  
    
}


/** 
* @brief GetModel Data buffer to use by the library 
* @param none
* retval void* Pointer to the used buffer 
*/
#ifdef STATIC_MEMORY_ALLOCATION_IN_APPLI

void* GetMemoryDataBuffer(MOBLEUINT8 buffer_type, MOBLEUINT32 u32length)
{
  void* ptr = NULL;
  MOBLEUINT32 allocated_len;

  switch (buffer_type)
  {
    case MESH_MODEL_BUFFER:
    case HEALTH_MODEL_NEW_BUFFER:
    case HEALTH_MODEL_PUBLISH_BUFFER:
      ptr = &aModelsBuff[0];  
      allocated_len = sizeof(aModelsBuff); 
      break;
    
    case MESH_MODEL_RESPONSE_BUFFER:
    case VENDOR_MODEL_WRITE_BUFFER:
    case VENDOR_MODEL_WRITE_PUBLISHBUFFER:
    case VENDOR_MODEL_RESPONSE_BUFFER:
    case GENERIC_MODEL_REPLY_BUFFER:
    case GENERIC_MODEL_SENDREMOTE_BUFFER:
    case GENERIC_MODEL_SENDDATA_BUFFER:
    case CONFIG_MODEL_PUBLISH_BUFFER:
      ptr = &aModelsBuff_Response[0];  
      allocated_len = sizeof(aModelsBuff_Response);
      break;
    
    case MESH_LOWER_TPT_BUFFER:
      ptr = &aLowerTptBuff[0]; 
      allocated_len = sizeof(aLowerTptBuff);
      break;
      
    case MESH_LOWER_TPT_FN_BUFFER:
      ptr = &aLowerTptFnBuff[0];  
      allocated_len = sizeof(aLowerTptFnBuff);
      break;
      
    case MESH_LOWER_TPT_APP_BUFFER:
      ptr = &aLowerTptAppBuff[0];    
      allocated_len = sizeof(aLowerTptAppBuff);
      break;
      
    case PROVISIONER_BUFFER:
    case PROVISION_NODE_BUFFER:
      ptr = &aProvisionParamBuff[0];    
      allocated_len = sizeof(aProvisionParamBuff);
      break;
      
    case ACCESS_APPLI_BUFFER:
      ptr = &aAppli_SAR_Buffer[0];    
      allocated_len = sizeof(aAppli_SAR_Buffer);
      
    case MESH_LOWER_TPT_INSEG:
      ptr = &aTrans_SegIn_Buffer[0];    
      allocated_len = sizeof(aTrans_SegIn_Buffer);
      break;
      
    case MESH_LOWER_TPT_INSEQ0:
      ptr = &aTrans_Seq0_Buffer[0];    
      allocated_len = sizeof(aTrans_Seq0_Buffer);
      break;
      
    case MESH_LOWER_TPT_OUTMSG:
      ptr = &aTrans_Out_Buffer[0];    
      allocated_len = sizeof(aTrans_Out_Buffer);
      
    default:
      break;
  }

  TRACE_M(TF_MEMORY, "BuffType= %d, RequiredLength=%d, Allocated= %d \r\n", buffer_type, u32length, allocated_len);
  if (u32length > allocated_len)
  {
    TRACE_M(TF_MEMORY, "***<<<ERROR>>> Short buffer Allocated \r\n");
  }
  
  return ptr;
}


#endif

#if (FRIEND_FEATURE == 0)
void BnrgmFrndFnProcess(void* param)
{
  
}

MOBLE_RESULT BnrgmFrndFnProcessFrndClear(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnProcessFrndClearConf(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnProcessFrndPoll(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnProcessFrndRequest(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnProcessFrndSubscrListAdd(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnProcessFrndSubscrListRem(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnSendFrndClear(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnSwitchKeys(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndFnUpdateKeys(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndMgmtFnInit(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndMgmtGetFriendshipStatus(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLEUINT32 BnrgmFrndMgmtGetLpnPollTimeout(void* param)
{
  return 0;
}

MOBLE_RESULT BnrgmFrndMgmtGetSubscrStatus(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndMgmtQueLpnPkt(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

void BnrgmFrndMgmtSendSecurityUpdate(void* param)
{
  
}
#endif /* #if (FRIEND_FEATURE == 0) */

#if (LOW_POWER_FEATURE == 0)
MOBLE_RESULT BnrgmFrndLpnProcess(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnProcessFrndOffer(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnProcessFrndUpdate(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnProcessFrndRfu(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnProcessFrndSubscrListConf(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnSwitchKeys(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnTranspSegRecvd(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndLpnUpdateKeys(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}

MOBLE_RESULT BnrgmFrndMgmtLpnInit(void* param)
{
  return MOBLE_RESULT_NOTIMPL;
}
#endif /* #if (LOW_POWER_FEATURE == 0) */

#if (NEIGHBOR_TABLE_SUPPORTED == 0)
MOBLE_RESULT BLEMesh_NeighborTableInit(void* param)
{
  return MOBLE_RESULT_SUCCESS;
}

void BLEMesh_UpdateNeighbors(void* param)
{
  /* empty function */
}
#endif /* #if (NEIGHBOR_TABLE_SUPPORTED == 0) */

/* Empty functions to reduce code size in case of PB-ADV functionality not in use */

#if (PB_ADV_SUPPORTED == 0)
MOBLE_RESULT MoblePBADVInit(void* param)
{
  return MOBLE_RESULT_SUCCESS;
}
MOBLE_RESULT MoblePBADVProcessData(void* param)
{
  return MOBLE_RESULT_SUCCESS;
}
MOBLE_RESULT MoblePBADVStartProvisioning(void* param)
{
  return MOBLE_RESULT_SUCCESS;
}
MOBLE_RESULT MoblePBADVStopProvisioning(void* param)
{
  return MOBLE_RESULT_SUCCESS;
}
#endif 

/* PLEASE REFER TO THE .h file for different settings */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
