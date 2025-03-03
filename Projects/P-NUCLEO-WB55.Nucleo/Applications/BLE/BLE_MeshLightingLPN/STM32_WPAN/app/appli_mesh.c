/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_mesh.c
  * @author  MCD Application Team
  * @brief   User Application file 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "ble_common.h"
#include "app_conf.h"
#include "hal_common.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "types.h"
#include "ble_hal_aci.h"
#include "ble_hci_le.h"
#include <string.h>
#include "models_if.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "light_lc.h"
#include "sensors.h"
#include "common.h"
#include "serial_if.h"
#include "appli_nvm.h"
#include "pal_nvm.h"
#include "appli_config_client.h"
#include "appli_generic_client.h"
#include "appli_light_client.h"
#include "appli_sensor.h"
#include "appli_sensors_client.h"

#include "stm32_seq.h"
#if (( CFG_LPM_SUPPORTED == 0) && (ENABLE_PWM_SUPPORT == 1))
#include "PWM_config.h"
#include "PWM_handlers.h"
#endif
#include "mesh_cfg_usr.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private define ------------------------------------------------------------*/
#define APPLI_OPTIM                     0

/*********** Macros to be modified according to the requirement *************/
#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U
#define MANUAL_UNPROVISION_TIMER        3000U
#define FLASH_ERASE_TIME                100U
//#define DISCOVER_TIMER                  10*60*1000 /* 10 minutes */
#define DISCOVER_TIMER_INTERVAL         10*60*(1000000/CFG_TS_TICK_VAL)  /* 10 minutes */
#if (APPLI_OPTIM == 1)
#define APPLI_MESH_TIMER_INTERVAL       1*(1000/CFG_TS_TICK_VAL)  /* 1 ms */
#endif
#define DEFAULT_DELAY_PACKET_FROM         500U
#define DEFAULT_DELAY_PACKET_RANDOM_TIME  500U
#define USER_OUTPUT_OOB_APPLI_PROCESS           0U
#define INPUT_OOB_TIMEOUT                       30*(1000000/CFG_TS_TICK_VAL) /* input Oob 30 Sec timeout*/
#define PBADV_UNPROV_DEV_BEACON_INTERVAL        100U /* 100 ms */

#define DEVICE_KEY_SIZE                         16U
#define APP_KEY_SIZE                            16U
#if (LOW_POWER_FEATURE == 1)
#define LPN_API_TIMER_INTERVAL           5*(1000000/CFG_TS_TICK_VAL)  /* 5 secondes */
#endif
/* Private macro -------------------------------------------------------------*/
#define MAX_APPLI_BUFF_SIZE             8 
#define MAX_PENDING_PACKETS_QUE_SIZE    2
#define DATA_BUFFER_LENGTH              8
#define MAX_NUMB_ELEMENTS               APPLICATION_NUMBER_OF_ELEMENTS
#define CUSTOM_BEACON_AD_TYPE           0x00
#define ENABLE_CUSTOM_BEACON            0
#define CUSTOM_BEACON_INTERVAL          2000U

/**********************Friendship callbacks macros ****************************/
#define FN_CLEARED_REPEAT_REQUEST   1
#define FN_CLEARED_POLL_TIMEOUT     2
#define FN_CLEARED_FRIEND_CLEAR     3
#define LPN_CLEARED_NO_RESPONSE     1

/* Private variables ---------------------------------------------------------*/


enum ButtonState
{
  BS_OFF,
  BS_DEBOUNCE,
  BS_SHORT_PRESS,
  BS_LONG_PRESS
};

enum ButtonState buttonState = BS_OFF;
tClockTime tBounce = 0;
/*Will be an array aligned with number of elements*/
MOBLEUINT8 Appli_LedState = 0;
MOBLEUINT16 IntensityValue = INTENSITY_LEVEL_ZERO;
MOBLEUINT8 IntensityFlag = FALSE;
MOBLEUINT8 ProxyFlag = 0;
MOBLEUINT8 ProvisionFlag = 0;
MOBLEUINT8 UnprovisionInProgress = 0; 

#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
static MOBLEUINT8 PrvngInProcess = 0;
static MOBLEUINT32 OutputOobData = 0;
static MOBLEUINT32 OutputOobBlinkCount = 0;
#endif

#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
MOBLEUINT8 InputOobData[8] = {0};
MOBLEUINT8 inputOOBDataReady = 0;
MOBLEUINT8 InputOOBTimeOut_Id;
#endif

/*Number Of Elements selected per Node. Maximum Elements supported = 3*/
MOBLEUINT8 NumberOfElements = APPLICATION_NUMBER_OF_ELEMENTS;

#ifdef ENABLE_AUTH_TYPE_STATIC_OOB
/* 16 octets Static OOB information to be input here. Used during provisioning by Library */
const MOBLEUINT8 StaticOobBuff[SIZE_STATIC_OOB] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
#else
const MOBLEUINT8 StaticOobBuff[] = {0};
#endif

#ifdef PUB_KEY_TYPE_OOB
/* 64 octets Public Key information to be input here. It is only required for Public Key OOB case.
Used during provisioning by Library */
const MOBLEUINT8 PubKeyBuff[64] =  {0xF4, 0x65, 0xE4, 0x3F, 0xF2, 0x3D, 0x3F, 0x1B, 0x9D, 0xC7, 
0xDF, 0xC0, 0x4D, 0xA8, 0x75, 0x81, 0x84, 0xDB, 0xC9, 0x66, 0x20, 0x47, 0x96, 0xEC, 0xCF, 
0x0D, 0x6C, 0xF5, 0xE1, 0x65, 0x00, 0xCC, 0x02, 0x01, 0xD0, 0x48, 0xBC, 0xBB, 0xD8, 0x99, 
0xEE, 0xEF, 0xC4, 0x24, 0x16, 0x4E, 0x33, 0xC2, 0x01, 0xC2, 0xB0, 0x10, 0xCA, 0x6B, 0x4D, 
0x43, 0xA8, 0xA1, 0x55, 0xCA, 0xD8, 0xEC, 0xB2, 0x79};

/* 32 octets Private Key information to be input here. It is only required for Public Key OOB case.
Used during provisioning by Library */
const MOBLEUINT8 PrivKeyBuff[32] =  {0x52, 0x9A, 0xA0, 0x67, 0x0D, 0x72, 0xCD, 0x64, 0x97, 0x50, 
0x2E, 0xD4, 0x73, 0x50, 0x2B, 0x03, 0x7E, 0x88, 0x03, 0xB5, 0xC6, 0x08, 0x29, 0xA5, 0xA3, 
0xCA, 0xA2, 0x19, 0x50, 0x55, 0x30, 0xBA};
#else
const MOBLEUINT8 PubKeyBuff[] = NULL;
const MOBLEUINT8 PrivKeyBuff[] = NULL;
#endif



/* Select Node as Sniffer, Means able to sniff all the packets 
*  0 - Filters are enabled and packets not targeted to node are filtered
*  1 - Filters are disabled and packets not targeted to node are not filtered
*/
MOBLEUINT8 DisableFilter = 0;

#if (LOW_POWER_FEATURE == 1)
MOBLEINT32 BLEMesh_sleepTime;
MOBLEUINT32 SysRefCount;
MOBLEBOOL LPN_scan_enabled;
#endif

MOBLEUINT8 nvm_operation;
MOBLEUINT8 nvm_flash_page;
MOBLEUINT8 provisioning_completion;

/* Timer to control unprovisioned device beacons */
#if PB_ADV_SUPPORTED
MOBLEUINT8 discoverTimer_Id;
tClockTime discoverTimerinterval = DISCOVER_TIMER_INTERVAL;
#endif

#if (APPLI_OPTIM == 1)
MOBLEUINT8 appliTaskTimer_Id;
#endif
#if (LOW_POWER_FEATURE == 1)
volatile uint8_t BleProcessInit = 0;
MOBLEUINT8 lowPowerNodeApiTimer_Id;
#endif

MOBLEUINT8 pPropertyId[4];

/********************* Application configuration **************************/
#if defined(__GNUC__) || defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM)
MOBLEUINT8 bdaddr[8];

#ifdef INTERNAL_UNIQUE_NUMBER_MAC
static void Appli_GetMACfromUniqueNumber(void);
#endif /* INTERNAL_UNIQUE_NUMBER_MAC */

const void *mobleNvmBase; 
const void *appNvmBase;
const void *prvsnr_data;
#else
#error "Unknown compiler"
#endif /* __GNUC__ || defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM) */

#if ENABLE_SERIAL_INTERFACE
extern uint8_t button_emulation;
#endif

/* Private function prototypes -----------------------------------------------*/
static void Appli_LongButtonPress(void);
static void Appli_ShortButtonPress(void);
#if USER_OUTPUT_OOB_APPLI_PROCESS
void Appli_OobAuthenticationProcess(void);
#endif
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
static void InputOOBTimeOutTask(void);
#endif
void BLEMesh_UnprovisionCallback(MOBLEUINT8 reason);
void Appli_LowPowerProcess(void);
#if (APPLI_OPTIM == 1)
static void AppliMeshTask(void);
#endif
void Appli_GetPublicationParamsCb(model_publicationparams_t* pPubParameters);
WEAK_FUNCTION (void SerialPrvn_ProvisioningStatusUpdateCb(uint8_t flagPrvningInProcess, 
                                                          MOBLEUINT16 nodeAddress));

/* Private functions ---------------------------------------------------------*/

/************************* Button Control functions ********************/
/**
* @brief  Function calls when a button is pressed for short duration  
* @param  void
* @retval void
*/ 
static void Appli_ShortButtonPress(void)
{
  BLEMesh_ModelsCommand();
}
  

/**
* @brief  Function calls when a button is pressed for Long duration  
* @param  void
* @retval void
*/ 
static void Appli_LongButtonPress(void)
{
  /** GENERIC ONOFF **/  
//  TRACE_I(TF_GENERIC_CLIENT_M, "----------- Generic API ONOFF GET ------------- \r\n");
//  Appli_GenericClient_API(0, GENERIC_ON_OFF_GET, NULL);  
  
  /** GENERIC LEVEL **/  
//  TRACE_I(TF_GENERIC_CLIENT_M, "----------- Generic API LEVEL GET ------------- \r\n");
//  Appli_GenericClient_API(0, GENERIC_LEVEL_GET, NULL);  
  
  /** GENERIC POWER ONOFF **/  
//  TRACE_I(TF_GENERIC_CLIENT_M, "----------- Generic API POWER ON OFF GET ------------- \r\n");
//  Appli_GenericClient_API(0, GENERIC_POWER_ON_OFF_GET, NULL);  
  
  /** GENERIC TRANSITION TIME **/ 
//  TRACE_I(TF_GENERIC_CLIENT_M, "----------- Generic API DEFAULT TRANSITION TIME GET ------------- \r\n");
//  Appli_GenericClient_API(0, GENERIC_DEFAULT_TRANSITION_TIME_GET, NULL);  
  
  /** LIGHT LIGHTNESS **/   
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LIGHTNESS GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LIGHTNESS_GET, NULL);  
  
  /** LIGHT LIGHTNESS LINEAR **/ 
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LIGHTNESS LINEAR GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LIGHTNESS_LINEAR_GET, NULL);  
  
  /** LIGHT LIGHTNESS DEFAULT **/ 
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LIGHTNESS DEFAULT GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LIGHTNESS_DEFAULT_GET, NULL);  
  
  /** LIGHT LIGHTNESS RANGE **/ 
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LIGHTNESS RANGE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LIGHTNESS_RANGE_GET, NULL); 
  
  /** LIGHT LIGHTNESS CTL **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT CTL GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_CTL_GET, NULL);  
  
  /** LIGHT LIGHTNESS CTL TEMPERATURE **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT CTL TEMPERATURE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_CTL_TEMPERATURE_GET, NULL);  
  
  /** LIGHT LIGHTNESS CTL TEMPERATURE RANGE **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT CTL TEMPERATURE RANGE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_CTL_TEMPERATURE_RANGE_GET, NULL);  
  
  /** LIGHT LIGHTNESS CTL DEFAULT **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT CTL DEFAULT GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_CTL_DEFAULT_GET, NULL);  
  
  /** LIGHT LIGHTNESS HSL **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT HSL GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_HSL_GET, NULL);  

  /** LIGHT LIGHTNESS HSL DEFAULT **/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT HSL DEFAULT GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_HSL_DEFAULT_GET, NULL);  

  /** LIGHT LIGHTNESS HSL RANGE**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT HSL RANGE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_HSL_RANGE_GET, NULL);  
  
  /** LIGHT LIGHTNESS HSL HUE**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT HSL HUE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_HSL_HUE_GET, NULL);
  
  /** LIGHT LIGHTNESS HSL SATURATION**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT HSL SATURATION GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_HSL_SATURATION_GET, NULL);

  /** LIGHT LC MODE**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LC MODE GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LC_MODE_GET, NULL);
  
  /** LIGHT LC OM**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LC OM GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LC_OM_GET, NULL);
  
  /** LIGHT LC ON OFF**/
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LC ON OFF GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LC_ON_OFF_GET, NULL);
  
  /** LIGHT LC PROPERTY **/
//LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID  : 0x002B
  pPropertyId[0]= 0x2B;           // Property ID byte 0 : Property ID identifying a Light LC Property.
  pPropertyId[1]= 0x00;           // Property ID byte 1 : Property ID identifying a Light LC Property.
//  TRACE_I(TF_LIGHT_CLIENT_M, "----------- API LIGHT LC PROPERTY GET ------------- \r\n");
//  Appli_LightClient_API(0, LIGHT_LC_PROPERTY_GET, pPropertyId);
  

  /* SENSOR APIs */
  //PRESENT_AMBIENT_TEMPERATURE_PID : 0x004F
  pPropertyId[0]= 0x4F;           // Property ID byte 0 : Property ID for the sensor
  pPropertyId[1]= 0x00;           // Property ID byte 1 : Property ID for the sensor
  
  /** SENSOR DESCRIPTOR**/
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR DESCRIPTOR GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_DESCRIPTOR_GET, pPropertyId);
  
  /** SENSOR CADENCE**/
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR CADENCE GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_CADENCE_GET, pPropertyId);
  
    /** SENSOR SETTINGS **/
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR SETTINGS GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_SETTINGS_GET, pPropertyId);
  
    /** SENSOR SETTING **/
  pPropertyId[2]= 0xAD;           // Sensor Setting Property ID byte 0 : Property ID for the sensor setting
  pPropertyId[3]= 0x00;           // Sensor Setting Property ID byte 1 : Property ID for the sensor setting
 
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR SETTING GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_SETTING_GET, pPropertyId);
  
  /** SENSOR GET **/
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_GET, pPropertyId);
  
  /** SENSOR COLUMN **/
//  pPropertyId[2]= 0x01;           // Raw Value X byte 0 : Raw value identifying a column
//  pPropertyId[3]= 0x00;           // Raw Value X byte 1 : Raw value identifying a column
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR COLUMN GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_COLUMN_GET, pPropertyId);
  
  /** SENSOR SERIES **/
//  pPropertyId[2]= 0x01;           // Raw Value X1 byte 0 : Raw value identifying a starting column.
//  pPropertyId[3]= 0x02;           // Raw Value X2 byte 0 : Raw value identifying an ending column.
//  TRACE_I(TF_SENSOR_CLIENT_M, "----------- API SENSOR SERIES GET ------------- \r\n");
//  Appli_SensorsClient_API(0, SENSOR_SERIES_GET, pPropertyId);
  
  IntensityPublish();
  
}

/**
* @brief  Updates the button status  
* @param  int isPressed
* @retval void
*/ 
static void Appli_UpdateButtonState(int isPressed)
{
  uint32_t t0 = 0,t1 = 1;

  t0 = Clock_Time(); /* SW1 press timing */
  
  while(BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
  t1 = Clock_Time(); /* SW1 release timing */
  
  if((t1 - t0) > LONG_PRESS_THRESHOLD)
  {
    Appli_LongButtonPress();
  }
  else if((t1 - t0) > BOUNCE_THRESHOLD)
  {
    /* Button 1 short press action */
    Appli_ShortButtonPress();
  }
}

/**
* @brief  task for the BLE MESH, the MESH Models and the Appli processes  
* @param  void
* @retval void
*/ 
static void Mesh_Task()
{
  BLEMesh_Process();
  BLEMesh_ModelsProcess(); /* Models Processing */
  
  if((UnprovisionInProgress > 0) &&
     (!LL_FLASH_IsActiveFlag_OperationSuspended()))
  {
    UnprovisionInProgress = 0;
    AppliNvm_ClearModelState();
    PalNvmErase(PRVN_NVM_BASE_OFFSET, 4);
    TRACE_I(TF_PROVISION,"NVM erased\r\n");      
    TRACE_I(TF_PROVISION,"Device is unprovisioned by application \r\n");      
  }
    
#if (APPLI_OPTIM == 0)
  /* Set the task in the scheduler for the next execution */
#if (LOW_POWER_FEATURE == 0)
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);
#endif
#endif
}

/**
* @brief  task for the BLE MESH, the MESH Models and the Appli processes  
* @param  void
* @retval void
*/ 
static void Appli_Task()
{
  Appli_Process();
  
#if (APPLI_OPTIM == 0)
  /* Set the task in the scheduler for the next execution */
#if (LOW_POWER_FEATURE == 0)
  UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
#endif
}

/************************* LED Control functions ********************/
/**
* @brief  Controls the state of on board LED
* @param  void
* @retval void
*/ 
void Appli_LedCtrl(void)
{
  if(Appli_LedState)
  {
    BSP_LED_On(LED_BLUE);

  }
  else
  {
    BSP_LED_Off(LED_BLUE);

  }
}

/************* BLE-Mesh library callback functions ***************/

/**
* @brief  Blinks the on board LED  
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_LedBlink(void)
{
  /* Switch On the LED */
  Appli_LedState = 1;
  Appli_LedCtrl();
  Clock_Wait(500);
  
  /* Switch Off the LED */
  Appli_LedState = 0;
  Appli_LedCtrl();
  Clock_Wait(500);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Callback function Sets the state of the bulb 
* @param  MOBLEUINT16 ctrl which sets the lighting state of LED
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_LedStateCtrlCb(MOBLEUINT16 ctrl)
{
  /* Switch On the LED */
  if(ctrl > 0)
  {
    Appli_LedState = 1;
  }
  /* Switch Off the LED */
  else
  {
    Appli_LedState = 0;
  }
  
  Appli_LedCtrl();
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  initializes BLE Stack    
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleStackInitCb()
{
  /* BLE Stack initialization */
  /* Not Required in BLE */
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Sets transmission power of RF 
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleSetTxPowerCb(void)
{
  tBleStatus status;
  /* API to change the Transmission power of BLE Device */
  /* uint8_t en_high_power, uint8_t pa_level
  en_high_power Can be only 0 or 1. Set high power bit on or off.
  pa_level Can be from 0 to 7. Set the PA level value
  */ 
  status = aci_hal_set_tx_power_level(1, CFG_TX_POWER);
  if (status)
    return MOBLE_RESULT_FAIL;
  else
    return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  SET UUID value 
* @param  uuid_prefix_data : Pointer of UUID buffer data
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleSetUUIDCb(MOBLEUINT8 *uuid_prefix_data)
{
  /* UUID is 128 bits (16 bytes) and can guarantee uniqueness across space and time.
     It can be "Time-based" UUID or "truly-random or pseudo-random numbers"
 
     In this buffer user need to fill 10 bytes of UUID data. 
     Remaining 6 bytes, library fill this data with BDaddress.

    For example :
        F81D4FAE-7DEC-XBC4-Y12F-17D1AD07A961 (16 bytes)
        <any number> |_|  |_|   <BDAddress>

      X = 1 i.e. "Time Base UUID"
      X = 4 i.e. "Random Number"
      Y = 4 i.e. Conforming to the current spec 
    For UUID information refer RFC4122
  */
  
  /* copy UUID prefix data*/
  uint8_t uuid_buff[10] = {0xF8,0x1D,0x4F,0xAE,0x7D,0xEC};/*random value by user */

   uuid_buff[6] = 0x4B;  /* 0x4B => 4 : Random */
   uuid_buff[7] = 'S' ;  /* User define value */
   uuid_buff[8] = 0xA1;  /* 0xA1 => A : Conforming to the current Spec */
   uuid_buff[9] = 'T' ;  /* User define value */  

   memcpy(uuid_prefix_data,uuid_buff,sizeof(uuid_buff));
   memcpy(&uuid_prefix_data[10],bdaddr,6);  /* Copy bdaddr to last 6 bytes of UUID */
   
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  SET CID , PID and VID values 
* @param  company_product_info : vendor fill product information in this buffer
* @retval MOBLE_RESULT status of result
*/
MOBLE_RESULT Appli_BleSetProductInfoCB(MOBLEUINT8 *company_product_info)
{
  /* CID : provide 16-bit company identifier assigned by the Bluetooth SIG */
  uint16_t company_id                   = COMPANY_ID;
  
  /* PID : Provide 16-bit vendor-assigned product identifier */
  uint16_t product_id                   = PRODUCT_ID;
  
  /* VID : Provide 16-bit vendor-assigned product version identifier*/
  uint16_t product_version_id           = PRODUCT_VERSION_ID;
  
  memcpy(company_product_info + 0,(void *)&company_id,sizeof(company_id));
  memcpy(company_product_info + 2 ,(void *)&product_id,sizeof(product_id));
  memcpy(company_product_info + 4 ,(void *)&product_version_id,sizeof(product_version_id));
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Call back function called when GATT Connection Created  
* @param  none
* @retval none
*/ 
void Appli_BleGattConnectionCompleteCb(void)
{ 
  ProvisionFlag = 1;
  /* Proxy Node, will be called whenever Gatt connection is established */
  /* Turn ON Red LED*/
  ProxyFlag = 1;
  BSP_LED_On(LED_GREEN);
}

/**
* @brief  Call back function called when GATT Disconnection Complete  
* @param  none
* @retval none
*/ 
void Appli_BleGattDisconnectionCompleteCb(void)
{ 
  ProvisionFlag = 0;
  /* Proxy Node, will be called whenever Gatt disconnected */
  /* Turn Off Red LED*/
  ProxyFlag = 0;
  BSP_LED_Off(LED_GREEN);
}

/**
* @brief Unprovisioned Node Identification
* @param MOBLEUINT8 data 
* @retval none
*/
void Appli_BleUnprovisionedIdentifyCb(MOBLEUINT8 data)
{
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  PrvngInProcess = 1;  
#endif   
  TRACE_I(TF_PROVISION,"Unprovisioned Node Identifier received: %02x\n\r", data);    
}

/**
* @brief  Set Number of Elements in a Node
* @param  none
* @retval Sending the value to BLE Mesh Library
*/ 
MOBLEUINT8 Appli_BleSetNumberOfElementsCb(void)
{
  if(NumberOfElements > BLEMesh_GetNumberOfElements())
  {
    TRACE_I(TF_MISC, "Number of Elements enabled in application exceeding from Library Capability!\r\n"); 
    return BLEMesh_GetNumberOfElements();
  }
  
  else if(NumberOfElements == 0)
  {
    TRACE_I(TF_MISC,"Number Of Elements must be 1 or greater than 1!\r\n"); 
    return 1;
  }
  
  else
  {
    return NumberOfElements;
  }
}

/**
* @brief  Sets the Attention Timer Callback function
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleAttentionTimerCb(void)
{
/* avoid printf, if low power feature is supported */  
#if (LOW_POWER_FEATURE == 0)
/*  TRACE_I(TF_MISC, " \r\n"); */
#endif /* LOW_POWER_FEATURE == 0 */
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Call back function to give Output OOB information 
* @param  MOBLEUINT8* output_oob
* @param  MOBLEUINT8 size
* @retval void
*/ 
void Appli_BleOutputOOBAuthCb(MOBLEUINT8* output_oob, MOBLEUINT8 size)
{
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  uint32_t ooBData = 0;    
    for (MOBLEUINT8 i = 0; i < size; i++)
    {
        ooBData |= (output_oob[i] << 8*i);
    }
    OutputOobData = ooBData;
    TRACE_I(TF_PROVISION,"Output OOB information for provisioner: %ld\n\r", ooBData);
  #endif
}
  

#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
/**
* @brief  Process for both output and Input OOB handling
* @param  void
* @retval void
*/ 
void Appli_OobAuthenticationProcess(void)
{
    if(OutputOobBlinkCount < OutputOobData)
    {
      OutputOobBlinkCount++;
      Appli_LedBlink();
    }
    else
    {
        OutputOobBlinkCount++;
        Clock_Wait(1000);
        if(OutputOobBlinkCount > OutputOobData+3)
        {
            OutputOobBlinkCount = 0;
        }
    }
}
#endif


#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
/**
* @brief  Time out call back function for Input OOB information providing
* @param  None
* @retval None 
*/ 
static void InputOOBTimeOutTask(void)
{
  inputOOBDataReady = 0;
  /* Set event for Input OOB information in Terminal */
  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_INPUT_OOB_RSP_ID);
}
#endif

/**
* @brief  Call back function to provide Input OOB information
* @param  MOBLEUINT8 size
* @retval MOBLEUINT8*  
*/ 
MOBLEUINT8* Appli_BleInputOOBAuthCb(MOBLEUINT8 size)
{
#if defined (ENABLE_AUTH_TYPE_INPUT_OOB) && (ENABLE_SERIAL_INTERFACE)
#if 0
  MOBLEUINT16 inputTimer = 0; 
  TRACE_I(TF_PROVISION,"Input OOB information for provisioner-Size: %d\n\r", size);   
  while(1)
  {
      Serial_InterfaceProcess();
      /* Enable Mesh process working in while loop */
      BTLE_StackTick();
      /* Check if input is completed or timeout */
      if((inputOOBDataReady != 0x00) | (inputTimer > INPUT_OOB_TIMEOUT))
      {
          inputOOBDataReady = 0;
          break;
      }
      Clock_Wait(100);
      inputTimer++;
  }
  return InputOobData; 
#else
  TRACE_I(TF_PROVISION,"Input OOB information for provisioner-Size: %d\n\r", size);
  
  /* Start the timer for Input OOB information timeout: 30s */
  HW_TS_Start(InputOOBTimeOut_Id, INPUT_OOB_TIMEOUT);
  /* Wait for Input OOB information in Terminal */
  UTIL_SEQ_ClrEvt( 1 << CFG_IDLEEVT_INPUT_OOB_RSP_ID );
  UTIL_SEQ_WaitEvt( 1<< CFG_IDLEEVT_INPUT_OOB_RSP_ID );
  
  inputOOBDataReady = 0;
  return InputOobData; 
#endif
#else
  return NULL;
#endif
}


/**
* @brief  Call back function to provide Input OOB information
* @param  rcvdStringBuff: buffer to store input string 
* @param  rcvdStringSize: length of the input string 
* @retval void  
*/ 
void Appli_BleSerialInputOOBValue(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
#ifdef  ENABLE_AUTH_TYPE_INPUT_OOB
  unsigned long long InputOobDatatemp;
    sscanf(rcvdStringBuff + 5, "%lld", &InputOobDatatemp);
    memmove(&InputOobData, &InputOobDatatemp, sizeof(InputOobDatatemp));
    inputOOBDataReady = 1;

  /* Stop the timer for Input OOB information timeout */
  HW_TS_Stop(InputOOBTimeOut_Id);
  /* Set event for Input OOB information in Terminal */
  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_INPUT_OOB_RSP_ID);
#endif
}


/**
* @brief  Application BLE Disable Filter 
* @param  none
* @retval Status is replied
*/
MOBLEUINT8 Appli_BleDisableFilterCb(void)
{
    return DisableFilter;
}

/**
* @brief  Checks and do Manual Unprovision of board  
* @param  void
* @retval void
*/ 
void Appli_CheckForUnprovision(void)
{
  /* Checks if the User button is pressed or not at the startup */
  if (BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED)
  {
    BSP_LED_On(LED_BLUE);
    tClockTime t = Clock_Time();
    int interrupted = 0;
    
    /*Wait to check if user is pressing the button persistently*/
    while ((Clock_Time() - t) < MANUAL_UNPROVISION_TIMER)
    {
      if (BSP_PB_GetState(BUTTON_SW1) != BUTTON_PRESSED)
      {
        interrupted = 1;
        break;
      }
    }
    /* Unprovision, show that unprovisioning is completed, and 
    wait until user releases button*/
    if (!interrupted)
    {
      /* No GATT connection */
      BLEMesh_StopAdvScan();
      HAL_Delay(10);

      PalNvmErase(PRVN_NVM_BASE_OFFSET, 4);
      TRACE_I(TF_PROVISION,"NVM erased\r\n");      
      
      BLEMesh_Unprovision();
      AppliNvm_ClearModelState();     
      TRACE_I(TF_PROVISION,"Device is unprovisioned by application \r\n");      
      t = Clock_Time();
      while ((Clock_Time() - t) < FLASH_ERASE_TIME)
      {
        BLEMesh_Process();
      }
      
      while (BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED)
      {
        Appli_LedBlink();
      }
    }
    BSP_LED_Off(LED_BLUE);
  }
  
  /* Register the task for all MESH dedicated processes */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_REQ_ID, UTIL_SEQ_RFU, Mesh_Task );
  /* Set the task in the scheduler for the next scheduling */
#if (LOW_POWER_FEATURE == 0)
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);  
#else
  BleProcessInit = 1;
#endif
  
  /* Register the task for all MESH dedicated processes */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_APPLI_REQ_ID, UTIL_SEQ_RFU, Appli_Task );
  /* Set the task in the scheduler for the next scheduling */
#if (LOW_POWER_FEATURE == 0)
  UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif  

#if (APPLI_OPTIM == 1)
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &appliTaskTimer_Id, hw_ts_Repeated, AppliMeshTask);
  
  HW_TS_Start(appliTaskTimer_Id, APPLI_MESH_TIMER_INTERVAL);
#endif
}

void Appli_Unprovision(void)
{
  if(!ProxyFlag)
  {
    /* No GATT connection */
    BLEMesh_StopAdvScan();
    HAL_Delay(10);
      
    PalNvmErase(PRVN_NVM_BASE_OFFSET, 4);
    TRACE_I(TF_PROVISION,"NVM erased\r\n");      
  
    TRACE_I(TF_PROVISION,"Device is unprovisioned by application \r\n");      

    BLEMesh_Process();
    NVIC_SystemReset();
  }
}


/**
* @brief  Checks and updates Mac address to generate MAC Address
* @param  void
* @retval MOBLEUINT8 sum return the sum calculated mac
*/ 
int Appli_CheckBdMacAddr(void)
{
  MOBLEUINT8 sum = 239;
  MOBLEUINT8 result = 0;
  tBleStatus status = BLE_STATUS_SUCCESS;
  
#ifdef EXTERNAL_MAC_ADDR_MGMT
  memcpy(bdaddr, (MOBLEUINT8 *)CFG_ADV_BD_ADDRESS, 7);
  bdaddr[7] = (EXTERNAL_MAC_ADDR_MGMT | EXTERNAL_MAC_TYPE);
#endif
  
#ifdef INTERNAL_UNIQUE_NUMBER_MAC
  status = aci_hal_read_config_data(CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, 
                                    &bdaddr[7],
                                    bdaddr); 
  if(status != BLE_STATUS_SUCCESS) 
  {
    TRACE_I(TF_PROVISION, "Failed to read Public Address %d", status);
  }

  Appli_GetMACfromUniqueNumber();
  bdaddr[7] = INTERNAL_UNIQUE_NUMBER_MAC;
#endif

  for (int i = 0; i < 6; ++i)
  {
    sum = (sum << 1) + bdaddr[i];
  }

  if (sum == bdaddr[6])
  {
    result = 1;
  }
  
#if defined(EXTERNAL_MAC_ADDR_MGMT) && defined(EXTERNAL_MAC_IS_PUBLIC_ADDR)
   /* Do nothing for modification of 2 MSb */
#else
  bdaddr[5] |= 0xC0;    /* 2 Msb bits shall be 11b for Static Random Address */
#endif
  
#ifdef GENERATE_STATIC_RANDOM_MAC
  /* 6bytes of MAC will be filled, baddr_tmp[7] will contain the length  */
  status = aci_hal_read_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET, 
                                    &bdaddr[7], 
                                    bdaddr); 
  if(status != BLE_STATUS_SUCCESS) 
  {
    TRACE_I(TF_PROVISION, "Failed to read Random Address %d", status);
  }

  bdaddr[7] = GENERATE_STATIC_RANDOM_MAC;   
                      /* Do nothing for bdaddr, just pass the identification */
  result = 1;         /* This will overwrite the above for loop result, 
                          which is redundant for this case */
#endif  
  
  return result;
}


/**
* @brief  Reads the unique serial number of the device
* @param  void
* @retval void
*/
#ifdef INTERNAL_UNIQUE_NUMBER_MAC
static void Appli_GetMACfromUniqueNumber(void)
{
#ifdef STM32L152xE
  /************************STM32L152RE 96 bit Unique ID addresses***************/
#define DEVICE_ID_REG0 (0x1FF800D0)
#define DEVICE_ID_REG1 (0x1FF800D4)
#define DEVICE_ID_REG2 (0x1FF800E4)
#endif
  
  /*
  The unique serial number is a six bytes value generated from STM32 UID 
  stored at DEVICE_ID_REG0, DEVICE_ID_REG1 and DEVICE_ID_REG2. It is generated 
  cancating 4bytes from DEVICE_ID_REG2 and first bytes of DEVICE_ID_REG0 and 
  DEVICE_ID_REG1. Since the UID at DEVICE_ID_REG0 and 
  DEVICE_ID_REG1 could be identical for same batch samples, 
  4 bytes have been taken from DEVICE_ID_REG2
  */
  
  uint8_t i;
  uint8_t sum = 239;
#if 0
  uint8_t *ls_addr;
  
  ls_addr = (uint8_t*)DEVICE_ID_REG2;
  
  for(i = 0; i < 4; i += 1) 
  {
    bdaddr[i] = *ls_addr;
    ls_addr++;
  }
  
  bdaddr[4] = *(uint8_t*)(DEVICE_ID_REG0);
  bdaddr[5] = (*(uint8_t*)(DEVICE_ID_REG1)) | 0xC0; 
#endif
  /* Static Device Address: 
  The two most significant bits 
  of the address shall be equal to 1, 
  hence ORed with 0xC0)*/
  
  for (i = 0; i < 6; ++i)
  {
    sum = (sum << 1) + bdaddr[i];
  }
  bdaddr[6] = sum;
}
#endif


#if 0 
/**
* @brief  provides the information of the power saving mode
* @param  sleepMode currently unused, to be used in future 
* @retval SleepModes returns the mode of the controller
*/
SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
  if (buttonState == BS_OFF)
  {
    return SLEEPMODE_WAKETIMER;
  }
  else
  {
    return SLEEPMODE_RUNNING;
  }
}
#endif

/**
* @brief  callback for unprovision the node by provisioner.
* @param  status reserved for future 
* @retval void
*/
void BLEMesh_UnprovisionCallback(MOBLEUINT8 reason)
{
  ProvisionFlag = 0; 
  
  BLEMesh_Process();
  HAL_Delay(100);
#if PB_ADV_SUPPORTED
  BLEMesh_SetUnprovisionedDevBeaconInterval(PBADV_UNPROV_DEV_BEACON_INTERVAL);
#endif
  BLEMesh_StopAdvScan();
  HAL_Delay(100);
    
  TRACE_I(TF_PROVISION,"Device is unprovisioned by application \r\n");      
    
  PalNvmErase(PRVN_NVM_BASE_OFFSET, 4);
  TRACE_I(TF_PROVISION,"NVM erased\r\n");      
  TRACE_I(TF_PROVISION,"Reset Device\r\n");      
  
  HAL_Delay(100);

  NVIC_SystemReset();
}

/**
* @brief  This function scans and prints unprovisioned devices  
* @param  unprovDeviceArray: Pointer of an array for filling unprovisioned device UUIDs
* @param  noOfUnprovDevices: Pointer to take total count of nearby unprovisioned devices
* @retval MOBLE_RESULT
*/  
MOBLE_RESULT BLEMesh_ScanDevices(neighbor_params_t *unprovDeviceArray, MOBLEUINT8 *noOfUnprovDevices)
{
  MOBLE_RESULT result;
  
    result = BLEMesh_GetNeighborState(unprovDeviceArray,noOfUnprovDevices);
    /* Array print for testing */
    /* Check if any unprovisioned device is available */
    if(!(*noOfUnprovDevices))
    {
        TRACE_I(TF_PROVISION,"No Unprovisioned Device Nearby\r\n");  
    }
    else
    {
        for(MOBLEINT8 count=0; count < *noOfUnprovDevices; count++)
        {
          BLEMesh_PrintStringCb("");  
          TRACE_I(TF_PROVISION,"Device-%d -> ", count);  
          BLEMesh_PrintDataCb(unprovDeviceArray[count].uuid, 16);
        }
    }
  return result;
}
/**
* @brief  This function returns starts the provisioning of one of the devices
* @param  unprovDeviceArray: Pointer of an array having unprovisioned device UUIDs
* @param  index: Index of the device to be provisioned
* @retval MOBLE_RESULT
*/  
MOBLE_RESULT BLEMesh_ProvisionDevice(neighbor_params_t *unprovDeviceArray, MOBLEUINT16 index)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    
  result = BLEMesh_ProvisionRemote((unprovDeviceArray+index)->uuid);

  return result;
}

/* Customized implementation for provisioning a device from mesh node - End */

/**
* @brief  Call back function called when PB-ADV link Opened  
* @param  none
* @retval none
*/ 
void BLEMesh_PbAdvLinkOpenCb(void)
{ 
  ProvisionFlag = 0;
  TRACE_I(TF_PROVISION,"PB-ADV Link opened successfully \n\r");    
  /* Turn ON Red LED*/
#if (LOW_POWER_FEATURE == 1)
  /* do nothing */
#else
  BSP_LED_On(LED_RED);
#endif
}


/**
* @brief  Call back function called when PB-ADV link Closed   
* @param  none
* @retval none
*/ 
void BLEMesh_PbAdvLinkCloseCb(void)
{
  TRACE_I(TF_PROVISION,"PB-ADV Link Closed successfully \n\r");   
  /* Turn Off Red LED*/
#if (LOW_POWER_FEATURE == 1)
  /* do nothing */
#else
  BSP_LED_Off(LED_RED);
#endif
}


/**
* @brief  callback for provision the node by provisioner.
* @param  void
* @retval void
*/
void BLEMesh_ProvisionCallback(void)
{
  ProvisionFlag = 1;
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  PrvngInProcess = 0;
#endif
  TRACE_I(TF_PROVISION,"Device is provisioned by provisioner \r\n");
  
#if (LOW_POWER_FEATURE == 1)
  /* Call API LPN_API_TIMER_INTERVAL after LPN provisioning */
//  HW_TS_Start(lowPowerNodeApiTimer_Id, LPN_API_TIMER_INTERVAL);
#endif
}


/**
* @brief  callback for configuration the node by provisioner.
* @param  void
* @retval void
*/
void BLEMesh_ConfigurationCallback(void)
{
#if (LOW_POWER_FEATURE == 1)
//  /* Set the task in the scheduler for the next execution */
//  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_LPN_REQ_ID, CFG_SCH_PRIO_0);
  /* Call API LPN_API_TIMER_INTERVAL after LPN provisioning */
  if(LPN_scan_enabled == MOBLE_FALSE)
  {
    HW_TS_Start(lowPowerNodeApiTimer_Id, LPN_API_TIMER_INTERVAL);
    LPN_scan_enabled = MOBLE_TRUE;
  }
#endif
}


/**
* @brief  callback for friendship established by friend node
* @param  address of corresponding low power node
* @param  receive delay of low power node (unit ms)
* @param  poll timeout of low power node (unit 100ms)
* @param  number of elements of low power node
* @param  previous friend address of low power node (can be invalid address)
* @retval void
*/
void BLEMesh_FnFriendshipEstablishedCallback(MOBLE_ADDRESS lpnAddress,
                                             MOBLEUINT8 lpnReceiveDelay,
                                             MOBLEUINT32 lpnPollTimeout,
                                             MOBLEUINT8 lpnNumElements,
                                             MOBLE_ADDRESS lpnPrevFriendAddress)
{ 
  TRACE_I(TF_LPN_FRND,"Friendship established. Low power node address 0x%.4X \r\n", lpnAddress);
  TRACE_I(TF_LPN_FRND,"Low power node receive delay %dms \r\n", lpnReceiveDelay);
  TRACE_I(TF_LPN_FRND,"Low power node poll timeout %ldms \r\n", lpnPollTimeout*100);
  TRACE_I(TF_LPN_FRND,"Low power node number of elements %d \r\n", lpnNumElements);
  if (lpnPrevFriendAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_I(TF_LPN_FRND,"Low power node previous friend address 0x%.4X \r\n", lpnPrevFriendAddress);
  }
}

/**
* @brief  callback for friendship clear by friend node
* @param  reason of friendship clear
*         0: reserved,
*         1: friend request received from existing low power node (friend)
*         2: low power node poll timeout occurred
*         3: friend clear received
* @param  previous friend address of low power node (can be invalid address)
* @retval void
*/
void BLEMesh_FnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS lpnAddress)
{
  TRACE_I(TF_LPN_FRND,"Friendship cleared. Low power node address 0x%.4X \r\n", lpnAddress);
  
  switch(reason)
  {
  case FN_CLEARED_REPEAT_REQUEST: 
    TRACE_I(TF_LPN_FRND,"Reason: New friend request received from existing low power node \r\n");
    break;
  case FN_CLEARED_POLL_TIMEOUT:
    TRACE_I(TF_LPN_FRND,"Reason: Low power node poll timeout occurred \r\n");
    break;
  case FN_CLEARED_FRIEND_CLEAR:
    TRACE_I(TF_LPN_FRND,"Reason: Friend clear received \r\n");
    break;
  default:
    TRACE_I(TF_LPN_FRND,"Reason: Invalid \r\n");
    break;
  }
}

/**
* @brief  callback for friendship established by low power node
* @param  address of corresponding friend node
* @retval void
*/
void BLEMesh_LpnFriendshipEstablishedCallback(MOBLE_ADDRESS fnAddress)
{
  /* Friendship established */
  TRACE_I(TF_LPN_FRND,"Friend node responding, friendship established.\r\n");
}

/**
* @brief  callback for friendship clear by low power node
* @param  reason of friendship clear.
*         0: reserved
*         1: No response received from friend node
* @retval void
*/
void BLEMesh_LpnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS fnAddress)
{ 
  TRACE_I(TF_LPN_FRND,"Friendship cleared. Friend node address 0x%.4x \r\n", fnAddress);
  
  if (reason == LPN_CLEARED_NO_RESPONSE)
  {
    TRACE_I(TF_LPN_FRND,"Reason: Friend node not responding \r\n");
  }
  else
  {
    TRACE_I(TF_LPN_FRND,"Reason: Invalid \r\n");
  } 
}

/** 
* @brief  New neighbor appeared callback in neighbor table.
          If MAC address of neighbor changes it appears as new neighbor.
* @param  MAC address of neighbor.
*         is neighbor provisioned or unprovisioned device.
*         uuid of neighbor. NULL if not available
*         network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
*         last updated rssi value.
* @retval void
*/
void BLEMesh_NeighborAppearedCallback(const MOBLEUINT8* bdAddr,
                                          MOBLEBOOL provisioned,
                                          const MOBLEUINT8* uuid,
                                          MOBLE_ADDRESS networkAddress,
                                          MOBLEINT8 rssi)
{
  TRACE_I(TF_NEIGHBOUR,"New neighbor appeared. Neighbor MAC address:");
  
  for (MOBLEUINT8 count=0 ; count<6; count++)
  {
    TRACE_I(TF_NEIGHBOUR,"%.2x ", bdAddr[count]);
  }
  
  if (provisioned == MOBLE_TRUE)
  {
    TRACE_I(TF_NEIGHBOUR,"-> Provisioned node. \n\r");
  }
  else
  {
    TRACE_I(TF_NEIGHBOUR,"-> Unprovisioned device. \n\r");
  }
  
  TRACE_I(TF_NEIGHBOUR,"rssi: %d. ", rssi);
  
  if (networkAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_I(TF_NEIGHBOUR,"Network address: 0x%.4x\n\r", networkAddress);
  }
  else
  {
    TRACE_I(TF_NEIGHBOUR,"Network address not available\n\r");
  }

  for (MOBLEUINT8 i=0; i<16; i++)
  {
    if (uuid[i] == 0x00)
    {
      continue;
    }
    else
    {
      TRACE_I(TF_NEIGHBOUR,"UUID: ");
      
      for (MOBLEUINT8 count=0 ; count<16; count++)
      {
        TRACE_I(TF_NEIGHBOUR,"%.2x ", uuid[count]);
      }
      
      break;
    }
  }
  
  TRACE_I(TF_NEIGHBOUR,"\n\r");
}

/** 
* @brief  Existing neighbor refreshed callback in neighbor table.
          If MAC address of neighbor changes it appears as new neighbor.
* @param  MAC address of neighbor.
*         is neighbor provisioned or unprovisioned device.
*         uuid of neighbor. NULL if not available
*         network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
*         last updated rssi value.
* @retval void
*/
void BLEMesh_NeighborRefreshedCallback(const MOBLEUINT8* bdAddr,
                                          MOBLEBOOL provisioned,
                                          const MOBLEUINT8* uuid,
                                          MOBLE_ADDRESS networkAddress,
                                          MOBLEINT8 rssi)
{
  TRACE_I(TF_NEIGHBOUR,"Existing neighbor refreshed. Neighbor MAC address:");
  
  for (MOBLEUINT8 count=0 ; count<6; count++)
  {
    TRACE_I(TF_NEIGHBOUR,"%.2x ", bdAddr[count]);
  }
  
  if (provisioned == MOBLE_TRUE)
  {
    TRACE_I(TF_NEIGHBOUR,"-> Provisioned node. \n\r");
  }
  else
  {
    TRACE_I(TF_NEIGHBOUR,"-> Unprovisioned device. \n\r");
  }
  
  TRACE_I(TF_NEIGHBOUR,"rssi: %d. ", rssi);
  
  if (networkAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_I(TF_NEIGHBOUR,"Network address: 0x%.4x\n\r", networkAddress);
  }
  else
  {
    TRACE_I(TF_NEIGHBOUR,"Network address not available\n\r");
  }
  
  for (MOBLEUINT8 i=0; i<16; i++)
  {
    if (uuid[i] == 0x00)
    {
      continue;
    }
    else
    {
      TRACE_I(TF_NEIGHBOUR,"UUID: ");
      
      for (MOBLEUINT8 count=0 ; count<16; count++)
      {
        TRACE_I(TF_NEIGHBOUR,"%.2x ", uuid[count]);
      }
      
      break;
    }
  }
  
  TRACE_I(TF_NEIGHBOUR,"\n\r");
}

/** 
* @brief  Beacon received callback
*         Beacons are received only if received beacon ad type is not 
*         Mesh Message, Mesh Beacon or PB-ADV
* @param  MAC address
*         data
*         length of beacon
*         rssi value of beacon
* @retval void
*/
#if (ENABLE_CUSTOM_BEACON == 1)
/* BLEMesh_CustomBeaconReceivedCallback high frequency callback */
void BLEMesh_CustomBeaconReceivedCallback(const MOBLEUINT8* bdAddr,
                                              const MOBLEUINT8* data,
                                              MOBLEUINT8 length,
                                              MOBLEINT8 rssi)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (length < 2)
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_I(TF_BEACON, "Message is too small \r\n");
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if (data[1] == CUSTOM_BEACON_AD_TYPE)
    {
      TRACE_I(TF_BEACON, "Message length(%d), rssi(%d) \r\n", length, rssi);
  
      if (TF_BEACON == 1)
      {
        TRACE_I(TF_BEACON, "Data: ");
        for (MOBLEUINT8 count=0; count<length; count++)
        {
          TRACE_I(TF_BEACON, "%.2x ", data[count]);
        }
        TRACE_I(TF_BEACON, " \r\n");
      }
    }
  }
}
#endif /* BLEMesh_CustomBeaconReceivedCallback high frequency callback */

/** 
* @brief  Custom beacon generator
*         If size set to > 31 bytes, beacon is rejected
*         BLEMesh_SetCustomBeaconInterval should be set to get this callback
* @param  beacon data buffer. It includes length and AD type fields
*         buffer size
* @retval void
*/
void BLEMesh_CustomBeaconGeneratorCallback(void* buffer, MOBLEUINT8* size)
{
  MOBLEUINT8 adType = CUSTOM_BEACON_AD_TYPE;
  MOBLEUINT8 dataLength = 5;
  MOBLEUINT8 data[5] = {0x00,0x01,0x02,0x03,0x04};
  MOBLEUINT8* buf = (MOBLEUINT8*)buffer;
  
  buf[0] = dataLength+1;
  buf[1] = adType;
  memcpy(buf+2, data, dataLength);
  
  *size = dataLength+2;  
}

/**
* @brief  Low Power mode process 
*         If low power feature is supported, device enables low power mode
* @param  void
* @retval void
*/
void Appli_LowPowerProcess(void)
{
#if 0
#if LOW_POWER_FEATURE
  BluenrgMesh_sleepTime = (MOBLEINT32)BluenrgMesh_GetSleepDuration();
  
  if (BluenrgMesh_sleepTime > 0)
  {
    /* Timer 0 used for low power */
    HAL_VTimerStart_ms(0, BluenrgMesh_sleepTime);
    
    /* To start virtual timer */
    BTLE_StackTick();

    /* save virtual timer current count */
    SysRefCount = HAL_VTimerGetCurrentTime_sysT32();
    
    /* wakeup either from io or virtual timer */
    BlueNRG_Sleep(SLEEPMODE_WAKETIMER, WAKEUP_IO13, WAKEUP_IOx_LOW);
    
    /* update systick count based on updated virtual timer count */
    Set_Clock_Time(Clock_Time() + 
                   HAL_VTimerDiff_ms_sysT32(HAL_VTimerGetCurrentTime_sysT32(), SysRefCount));
    
    /* Stop Timer 0 */
    HAL_VTimer_Stop(0);
  }
#endif /* LOW_POWER_FEATURE */
#endif
}

/**
* @brief  Appli_IntensityControlPublishing:Function is used to set the intensity value.
*          Publishing the value.
* @param  void
* @retval void
*/
void Appli_IntensityControlPublishing(MOBLEUINT8* value)
{
      
  if(IntensityFlag == FALSE)
  {
      
    IntensityValue = IntensityValue + (INTENSITY_LEVEL_FULL/5);
    value[0] = (MOBLEUINT8)IntensityValue;
    value[1] = (MOBLEUINT8)(IntensityValue >> 8) ;
    
    if(IntensityValue >= INTENSITY_LEVEL_FULL)
    {
      IntensityFlag = TRUE;
    }
     
  }
  else
  {
     
    IntensityValue = IntensityValue - (INTENSITY_LEVEL_FULL/5);
    value[0] = (MOBLEUINT8)IntensityValue;
    value[1] = (MOBLEUINT8)(IntensityValue >> 8) ;    
    
    if(IntensityValue <= INTENSITY_LEVEL_ZERO)
    {
      IntensityFlag = FALSE;
    }
  
  }
}


/**
* @brief  Publish the intensity value for generic level or light lightness 
*         This function should be called in main loop
* @param  void
* @retval void
*/
void IntensityPublish(void)
{  
  
#ifdef LIGHT_CLIENT_MODEL_PUBLISH 
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS  
  MOBLEUINT8 elementIndex = 0; 
  MOBLEUINT8 pLightnessParam[3];
  Appli_IntensityControlPublishing(pLightnessParam);
  Appli_LightClient_API(elementIndex, LIGHT_LIGHTNESS_SET, pLightnessParam); 
#endif
  
#else  
#ifdef GENERIC_CLIENT_MODEL_PUBLISH
  MOBLEUINT8 elementIndex = 0; 
  MOBLEUINT8 pGeneric_LevelParam[3];
  
  Appli_IntensityControlPublishing(pGeneric_LevelParam);
  Appli_GenericClient_API(elementIndex, GENERIC_LEVEL_SET_ACK, pGeneric_LevelParam); 
#endif  
#endif
}

/**
* @brief  Application processing 
*         This function should be called in main loop
* @param  void
* @retval void
*/
void Appli_Process(void)
{
#ifdef ENABLE_SAVE_MODEL_STATE_NVM  
  AppliNvm_Process();
#endif
  
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
  if(PrvngInProcess)
  {
#if USER_OUTPUT_OOB_APPLI_PROCESS
    Appli_OobAuthenticationProcess();
#endif
  }
#endif

#ifdef TPT_LAYER_BUSY_TEST      
  if(BLEMesh_TrsptIsBusyState())
  {
    //SdkEvalLedOn(LED1);  
  }
  else
  {
    //SdkEvalLedOff(LED1);
  }
#endif      
}

#if PB_ADV_SUPPORTED
static void UnprovisionedDeviceBeaconApp(void)
{
  BLEMesh_SetUnprovisionedDevBeaconInterval(0);
}

static void UnprovisionedDeviceBeaconTask(void)
{
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_BEACON_REQ_ID, CFG_SCH_PRIO_0);

  return;
}
#endif

#if (APPLI_OPTIM == 1)
static void AppliMeshTask(void)
{
  /* Set the task in the scheduler for the next execution */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);

  return;
}
#endif

static void AppliMeshSW1Task(void)
{
  /* Check if button action is emulated with SW1 command on USART1 */
#if ENABLE_SERIAL_INTERFACE
  if(!button_emulation)
  {
    Appli_UpdateButtonState(BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
  }
  else
  {
    /* Button 1 short press action */
    Appli_ShortButtonPress();
    button_emulation = 0;
  }
#else
  Appli_UpdateButtonState(BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
#endif  
  return;
}

#if (ENABLE_SENSOR_MODEL_SERVER != 0)
static void AppliMeshSW3Task(void)
{
  Sensor_UpdatePublishState(0, 1);
  
  Appli_Sensor_Update(0, 1);
  
#if ENABLE_SERIAL_INTERFACE
  /* Reset button_emulation state in case of emulation with SW3 command on USART1 */
  button_emulation = 0;
#endif  
  return;
}
#endif

#if (LOW_POWER_FEATURE == 1)
static void LowPowerNodeApiApp(void)
{
  TRACE_I(TF_PROVISION,"Scan disabled \r\n");
  BLEMesh_LpnDisableScan();
    
  return;
}

static void LowPowerNodeApiTask(void)
{
  /* Set the task in the scheduler for the next execution */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_LPN_REQ_ID, CFG_SCH_PRIO_0);
  
  return;
}
#endif

#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION       
/**
* @brief function to inintiallise the GPIO interrupt for Power down 
* @param  void
* @retval void
*/
static void GPIO_InitNVICPowerOff(void)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Enable the PowerOff GPIO Clock */
  POWEROFF_GPIO_CLK_ENABLE();
  
  /* Configure PowerOff pin as input with External interrupt */
  gpioinitstruct.Pin  = POWEROFF_PIN;
  gpioinitstruct.Pull = GPIO_PULLUP;
  gpioinitstruct.Mode = GPIO_MODE_IT_FALLING; 
  HAL_GPIO_Init(POWEROFF_GPIO_PORT, &gpioinitstruct);
  
  /* Enable and set PowerOff EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(POWEROFF_EXTI_IRQn), 0x0F, 0x00);
  HAL_NVIC_EnableIRQ((IRQn_Type)(POWEROFF_EXTI_IRQn));
}
#endif

/**
* @brief  Initializes the Application  
* @param  void
* @retval void
*/
void Appli_Init(MOBLEUINT8 *flag)
{
#if 0
#ifdef ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE  
/* Hardware Factory Restore implementation */
  AppliNvm_FactorySettingReset();
#endif
#endif
  
#if PB_ADV_SUPPORTED
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_BEACON_REQ_ID, UTIL_SEQ_RFU, UnprovisionedDeviceBeaconApp );
  /**
   * Create Timer to control unprovisioned device beacons
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &discoverTimer_Id, hw_ts_SingleShot, UnprovisionedDeviceBeaconTask);
  
  HW_TS_Start(discoverTimer_Id, DISCOVER_TIMER_INTERVAL);
#endif
  
#if ENABLE_SERIAL_INTERFACE
  Serial_Init();
#endif
  
#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION       
  GPIO_InitNVICPowerOff();
#endif 
  
#if 0
#if (( CFG_LPM_SUPPORTED == 0) && (ENABLE_PWM_SUPPORT == 1))
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();
  PWM_Init();
#ifdef USER_BOARD_1LED
  Modify_PWM(SINGLE_LED, 1);     
#endif
#ifdef  USER_BOARD_COOL_WHITE_LED
  Modify_PWM(COOL_LED, 0); 
  Modify_PWM(WARM_LED, 0); 
#endif
#ifdef  USER_BOARD_RGB_LED
  Modify_PWM(RED_LED, 1); 
  Modify_PWM(GREEN_LED, 1); 
  Modify_PWM(BLUE_LED, 1);
#endif
#endif
#endif
  
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_SW1_REQ_ID, UTIL_SEQ_RFU, AppliMeshSW1Task );
  
#if (ENABLE_CUSTOM_BEACON == 1)
  BLEMesh_SetCustomBeaconInterval(CUSTOM_BEACON_INTERVAL);
#endif

#if (LOW_POWER_FEATURE == 1)  
  /**
  * Create Timer to control unprovisioned device beacons
  */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &lowPowerNodeApiTimer_Id, hw_ts_SingleShot, LowPowerNodeApiTask);
  
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_LPN_REQ_ID, UTIL_SEQ_RFU, LowPowerNodeApiApp );
  LPN_scan_enabled = MOBLE_FALSE;
#endif
#if (ENABLE_SENSOR_MODEL_SERVER != 0)
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_SW3_REQ_ID, UTIL_SEQ_RFU, AppliMeshSW3Task );
#endif
  
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
  /**
  * Create Timer to control Input OOB information
  */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &InputOOBTimeOut_Id, hw_ts_SingleShot, InputOOBTimeOutTask);
#endif

}

/*****************************Config Model Callbacks***************************/
/**
* @brief  Callback from Config Model to receive the Publication Parameters
* @param  Structure Pointer having parameters - modelID, elementAddress,
*         Publish Address, PublishPeriod, PublishTTL, Credential Flag, 
*         Publish AppKeyIndex, Retransmit Count & Restransmit Interval Steps
* @retval none
*/ 
void Appli_GetPublicationParamsCb(model_publicationparams_t* pPubParameters)
{
  if(pPubParameters->modelID == SENSOR_SERVER_MODEL_ID)
  {
    Sensor_ModelPublishSet(pPubParameters);
  }
}


/**
* @brief  PWM_CoolValue: This function is used to calculate the value for the 
          cool temperature for the light.
* @param  colourValue: Temperature value ratio
* @param  brightValue: Lightness value ratio.
* @retval duty: duty for the PWM
*/ 
MOBLEUINT16 PWM_CoolValue(float colourValue ,float brightValue)
{
  float colourDuty; 
  MOBLEUINT16 duty;
  
  colourDuty = colourValue * brightValue;    
  duty = (MOBLEUINT16)(colourDuty * PWM_TIME_PERIOD);
  
  return duty;
}


/**
* @brief  PWM_WarmValue: This function is used to calculate the value for the 
          warm temperature for the light.
* @param  colourValue: Temperature value ratio
* @param  brightValue: Lightness value ratio.
* @retval duty: duty for the PWM
*/ 
MOBLEUINT16 PWM_WarmValue(float colourValue ,float brightValue)
{
  float warmDuty;    
  MOBLEUINT16 duty;
  
  warmDuty = (1-colourValue)* brightValue;
  duty = (MOBLEUINT16)(warmDuty * PWM_TIME_PERIOD);
  
  return duty;
}

/**
* @brief  PwmValueMapping: This function takes the value as parameter from the 
*         model and return the value according to .
* @param  setValue: value to be set from model
* @param  maxRange: Max range of the parameter
* @param  minRange: Min range of the parameter
* @retval MOBLEUINT8
*/
MOBLEUINT16 PwmValueMapping(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange)
{
  MOBLEUINT16 percentValue;
  MOBLEUINT16 duty;
  
#ifdef SMART_PLUG
   percentValue =  (setValue * 100)/ (maxRange - minRange);
        
   if(percentValue < 2)
   {
     duty = 0;
   }   
   else if((percentValue > 2) && (percentValue <= 10))
   {
     duty = 1;
   }
   else if((percentValue > 10) && (percentValue <= 20))
   {
     duty = 2;
   }
   else if((percentValue > 20) && (percentValue <= 30))
   {
     duty = 3;
   }
   else if((percentValue > 30) && (percentValue <= 40))
   {
     duty = 4;
   }
   else if((percentValue > 40) && (percentValue <= 50))
   {
     duty = 5;
   }
   else if((percentValue > 50) && (percentValue <= 60))
   {
     duty = 6;
   }
   else if((percentValue > 60) && (percentValue <= 70))
   {
     duty = 7;
   }
   else if((percentValue > 70) && (percentValue <= 80))
   {
     duty = 8;
   }
   else if((percentValue > 80) && (percentValue <= 90))
   {
     duty = 9;
   }
   else if(percentValue > 90)
   {
     duty = 10;
   }
#else
   if(minRange > 0x00)
   {
     percentValue =  (setValue - 800)/ (maxRange - minRange);
     percentValue = (percentValue*100);
   }
   else
   {
     percentValue =  (setValue * 100)/ (maxRange - minRange);
   }
   
   duty = percentValue*320 ;
   
   if(duty >= 32000)
   {
     duty = 31999;
   }
   if(duty == 0)
   {
     duty = 1;
   }
#endif
     
   return duty;
}


/************************************ Weak linking ***********************************
If implemented in application, linker would replace weak linking in library */
WEAK_FUNCTION (void SerialPrvn_ProvisioningStatusUpdateCb(uint8_t flagPrvningInProcess, MOBLEUINT16 nodeAddress))
{
    
}


/**
* @}
*/

/**
* @}
*/
