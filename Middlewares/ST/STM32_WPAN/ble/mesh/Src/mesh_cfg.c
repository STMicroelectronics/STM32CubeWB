/**
******************************************************************************
* @file    mesh_cfg.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   User configurable settings for BLE-Mesh
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
* Initial BLE-Mesh is built over Motorola's Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
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
//#if defined(BLUENRG1_DEVICE) || defined(BLUENRG2_DEVICE) /* BLUENRG1 or BLUENRG2 */
const lpn_params_t LpnParams = LOW_POWER_NODE_PARAMS;
//#else
//const lpn_params_t LpnParams;
//#endif
const fn_params_t FnParams = FRIEND_NODE_PARAMS;
const prvn_params_t PrvnParams = UNPROV_NODE_INFO_PARAMS;
const neighbor_table_init_params_t NeighborTableParams = NEIGHBOR_TABLE_PARAMS;

/*ALIGN(4)*/
__attribute__((aligned(4))) const MOBLEUINT16 Appli_SIG_Models[]=
{
 /*****************************************************************************/
/** Following Section places Generic MODEL Enabled Client IDs    ************/
/*****************************************************************************/ 

#ifdef ENABLE_CONFIG_MODEL_CLIENT
 SIG_MODEL_ID_CONFIG_CLIENT,
#endif
 /*****************************************************************************/
 /** Following Section places GENERIC MODEL Enabled SERVER IDs     ************/
 /*****************************************************************************/
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
 GENERIC_MODEL_SERVER_ONOFF_MODEL_ID,
#endif
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
 GENERIC_MODEL_SERVER_LEVEL_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
 GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
 GENERIC_MODEL_SERVER_POWER_ONOFF_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
 GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL
 GENERIC_MODEL_SERVER_POWER_LEVEL_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP
 GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY
 GENERIC_MODEL_SERVER_BATTERY_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION
 GENERIC_MODEL_SERVER_LOCATION_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP
 GENERIC_MODEL_SERVER_LOCATION_SETUP_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY
 GENERIC_MODEL_SERVER_ADMIN_PROPERTY_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY
 GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY
 GENERIC_MODEL_SERVER_USER_PROPERTY_MODEL_ID,
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY
 GENERIC_MODEL_SERVER_CLIENT_PROPERTY_MODEL_ID,
#endif

 /*****************************************************************************/
 /** Following Section places LIGHTING MODEL Enabled SERVER IDs    ************/
 /*****************************************************************************/

 #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
 LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID,       
#endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
 LIGHT_MODEL_SERVER_LIGHTNESS_SETUP_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
 LIGHT_MODEL_SERVER_CTL_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
 LIGHT_MODEL_SERVER_CTL_SETUP_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
 LIGHT_MODEL_SERVER_CTL_TEMPERATURE_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
 LIGHT_MODEL_SERVER_HSL_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
 LIGHT_MODEL_SERVER_HSL_SETUP_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
 LIGHT_MODEL_SERVER_HSL_HUE_MODEL_ID,
 #endif 
 
 #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
 LIGHT_MODEL_SERVER_HSL_SATURATION_MODEL_ID,
 #endif 
/*****************************************************************************/
/** Following Section places Light XYL MODEL Enabled SERVER IDs    ************/
/*****************************************************************************/
 #ifdef ENABLE_LIGHT_MODEL_SERVER_XYL
 LIGHT_MODEL_SERVER_XYL_MODEL_ID,
 #endif 

 #ifdef ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP
 LIGHT_MODEL_SERVER_XYL_SETUP_MODEL_ID,
 #endif 

/*****************************************************************************/
/** Following Section places Light LC MODEL Enabled SERVER IDs    ************/
/*****************************************************************************/
 #ifdef ENABLE_LIGHT_MODEL_SERVER_LC
 LIGHT_MODEL_SERVER_LC_MODEL_ID,
 #endif 
 
 #ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP
 LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID,
 #endif 

 /*****************************************************************************/
 /** Following Section places SENSOR MODEL Enabled SERVER IDs    ************/
 /*****************************************************************************/
 #ifdef ENABLE_SENSOR_MODEL_SERVER
 SENSOR_SERVER_MODEL_ID,    
 #endif
 
 #ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP 
 SENSOR_SETUP_SERVER_MODEL_ID, 
 #endif

/*****************************************************************************/
/** Following Section places Time and Scene MODEL Enabled SERVER IDs    ************/
/*****************************************************************************/

#ifdef ENABLE_TIME_MODEL_SERVER 
TIME_MODEL_SERVER_MODEL_ID,             
#endif
#ifdef ENABLE_TIME_MODEL_SERVER_SETUP 
TIME_MODEL_SERVER_SETUP_MODEL_ID,       
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER 
SCENE_MODEL_SERVER_MODEL_ID,            
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER_SETUP 
SCENE_MODEL_SERVER_SETUP_MODEL_ID,      
#endif

/*****************************************************************************/
/** Following Section places Generic MODEL Enabled Client IDs    ************/
/*****************************************************************************/ 
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF 
 GENERIC_MODEL_CLIENT_ONOFF_MODEL_ID,   
#endif

#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL
 GENERIC_MODEL_CLIENT_LEVEL_MODEL_ID,
#endif

#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS
 LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID,
#endif 
 
 
 /*****************************************************************************/
 /** End of the SIG Server Model IDs                               ************/
 /*****************************************************************************/
 
 0      /* Default entry, do NOT remove */
};

const MOBLEUINT32 Appli_Vendor_Models[]=
{
  VENDORMODEL_STMICRO_ID1,
  
 /*****************************************************************************/
 /** End of the Vendor Server Model IDs                               ************/
 /*****************************************************************************/

 0      /* Default entry, do NOT remove */
};


//ALIGN(4)
__attribute__((aligned(4))) const MOBLEUINT16 Appli_CLIENT_Support_SIG_Models[]=
{
  /* Array to define the Models on which subscription and Publication may work 
     for the Provisioner itself */
 GENERIC_MODEL_SERVER_ONOFF_MODEL_ID,
// GENERIC_MODEL_SERVER_LEVEL_MODEL_ID,
// GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID,
// SENSOR_SERVER_MODEL_ID,
// LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID,
// LIGHT_MODEL_SERVER_LC_MODEL_ID,
 0
};


#define APPLI_SIG_MODELS_COUNT ( (sizeof(Appli_SIG_Models) / sizeof(Appli_SIG_Models[0]) ) -1  )
/* Subtracted 1 because of last default entry of 0, since users can comment all models */

#define APPLI_VENDOR_MODELS_COUNT ( ( sizeof(Appli_Vendor_Models) / sizeof(Appli_Vendor_Models[0]) ) -1 )
/* Subtracted 1 because of last default entry of 0, since users can comment all models */

#define APPLI_NODE_SIG_MODELS_COUNT ( (sizeof(Appli_CLIENT_Support_SIG_Models) / sizeof(Appli_CLIENT_Support_SIG_Models[0]) ) -1  )
/* Subtracted 1 because of last default entry of 0, since users can comment all models */

#define APPLI_NODE_VENDOR_MODELS_COUNT ( ( sizeof(Appli_Vendor_Models) / sizeof(Appli_Vendor_Models[0]) ) -1 )
/* Subtracted 1 because of last default entry of 0, since users can comment all models */

/* Private function prototypes -----------------------------------------------*/
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

#if (NEIGHBOR_TABLE_SUPPORTED == 0)
MOBLE_RESULT BLEMesh_NeighborTableInit(void* param);
void BLEMesh_UpdateNeighbors(void* param);
#endif /* #if (NEIGHBOR_TABLE_SUPPORTED == 0) */

#if (PB_ADV_SUPPORTED == 0)
MOBLE_RESULT MoblePBADVInit(void* param);
MOBLE_RESULT MoblePBADVProcessData(void* param);
MOBLE_RESULT MoblePBADVStartProvisioning(void* param);
MOBLE_RESULT MoblePBADVStopProvisioning(void* param);
#endif

MOBLEUINT8 ApplicationSetNodeSigModelList(void);

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

void* GetModelsResponseBuffer(void);

/* Private functions ---------------------------------------------------------*/

/** 
* @brief ApplicationGetSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetSigModelList(MOBLEUINT16* pModels_sig_ID, MOBLEUINT8 elementIndex)
{
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  if (APPLI_SIG_MODELS_COUNT != 0)
  {
    memcpy(pModels_sig_ID, Appli_SIG_Models, APPLI_SIG_MODELS_COUNT*2);
  }
  return  APPLI_SIG_MODELS_COUNT; 
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
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  if (APPLI_NODE_SIG_MODELS_COUNT != 0)
  {
    memcpy(pModels_sig_ID, Appli_CLIENT_Support_SIG_Models, APPLI_NODE_SIG_MODELS_COUNT*2);
  }
  return  APPLI_SIG_MODELS_COUNT; 
}

#ifdef ENABLE_PROVISIONER_FEATURE
/** 
* @brief ApplicationGetCLIENTSigModelList: This function provides the list of the 
*           SIG Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with SIG Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the SIG Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationSetNodeSigModelList(void)
{
  /* Since the SIG Models are 2 bytes, copy 2*size for memcpy */
  if (APPLI_SIG_MODELS_COUNT != 0)
  {
    BLEMeshSetSelfModelList(NumberOfElements);
  }
  return  APPLI_SIG_MODELS_COUNT; 
}

#endif
/** 
* @brief ApplicationGetVendorModelList: This function provides the list of the 
*           Vendor Models to the calling function
* @param pModels_sig_ID: Pointer of the array to be filled with Vendor Models list
* @param elementIndex: Index of the element for Model List
* retval Count of the Vendor Model Servers enabled in the Application
*/
MOBLEUINT8 ApplicationGetVendorModelList(MOBLEUINT32* pModels_vendor_ID, MOBLEUINT8 elementIndex)
{
  /* Since the Vendor Models are 4 bytes, copy 4*size for memcpy */
  if (APPLI_VENDOR_MODELS_COUNT != 0)
  {
    memcpy(pModels_vendor_ID, Appli_Vendor_Models, APPLI_VENDOR_MODELS_COUNT*4);
  }
  return  APPLI_VENDOR_MODELS_COUNT; 
}


/** 
* @brief ApplicationChkSigModelActive: This function checks if a specific 
*          Model Server is active in the Model Server list
* @param modelID: Model Server ID received for the checking function
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkSigModelActive(MOBLEUINT16 modelID)
{
  MOBLEINT8 index = 0;
  MOBLEBOOL result = MOBLE_FALSE;
    
  for(index = 0; index < APPLI_SIG_MODELS_COUNT; index++)
  {
   if ( modelID == Appli_SIG_Models[index] )      
   {
     result = MOBLE_TRUE;
       break;
   }
  }
  
  return result;
}


/** 
* @brief ApplicationChkVendorModelActive: This function checks if a specific 
*          Model Server is active in the Vendor Model Server list
* @param modelID: Model Server ID received for the checking function
* retval Bool: True or False, if the Server ID matches with the list 
*/
MOBLEBOOL ApplicationChkVendorModelActive(MOBLEUINT32 modelID)
{
  MOBLEUINT8 index;
  MOBLEBOOL result = MOBLE_FALSE;
    
  for(index = 0; index < APPLI_VENDOR_MODELS_COUNT; index++)
  {
     if ( modelID == Appli_Vendor_Models[index] )      
     {
       result = MOBLE_TRUE;
       break;
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
