/**
******************************************************************************
* @file    light.c
* @author  BLE Mesh Team
* @brief   Light Model middleware implementation
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
#include "hal_common.h"
#include "mesh_cfg.h"
#include "light.h"
#include "generic.h"
#include "common.h"
#include "models_if.h"
#include "light_lc.h"
#include "math.h"
#include "compiler.h"
#include <string.h>
/** @addtogroup MODEL_Light
*  @{
*/

/** @addtogroup Light_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern Generic_ModelFlag_t Generic_ModelFlag[APPLICATION_NUMBER_OF_ELEMENTS];
static Light_TimeParam_t Light_TimeParam[APPLICATION_NUMBER_OF_ELEMENTS];
Light_ModelFlag_t Light_ModelFlag[APPLICATION_NUMBER_OF_ELEMENTS];
Publication1SecFlag_t Publication1SecFlag;
static Light_TemporaryStatus_t Light_TemporaryStatus[APPLICATION_NUMBER_OF_ELEMENTS];

/* Light Lightness */
static Light_LightnessParam_t Light_LightnessParam;
static Light_LightnessStatus_t Light_LightnessStatus[APPLICATION_NUMBER_OF_ELEMENTS];
static Light_LightnessDefaultParam_t Light_LightnessDefaultParam[APPLICATION_NUMBER_OF_ELEMENTS];
/* Initialize the light lighness range with maximum and minimuwm value as pr the spec */
static Light_LightnessRangeParam_t Light_LightnessRangeParam[APPLICATION_NUMBER_OF_ELEMENTS];																																																																																																																																			

/* Light CTL */
static Light_CtlParam_t Light_CtlParam;
/* Initialize the parameter with minimum value of temperature and delta */
static Light_CtlStatus_t Light_CtlStatus[APPLICATION_NUMBER_OF_ELEMENTS] ;
static Light_CtlDefaultParam_t Light_CtlDefaultParam[APPLICATION_NUMBER_OF_ELEMENTS];

/* Initialize the Ctl tepmerature range with minimum and maximum value as per the spec */
static Light_CtlTemperatureRangeParam_t CtlTemperatureRangeParam[APPLICATION_NUMBER_OF_ELEMENTS];
static Light_HslParam_t Light_HslParam;
static Light_HslStatus_t Light_HslStatus[APPLICATION_NUMBER_OF_ELEMENTS];
static Light_HslDefaultParam_t Light_HslDefaultParam[APPLICATION_NUMBER_OF_ELEMENTS];

/* Initialize the HSL parameter range with minimum and maximum value as per the spec */
static Light_HslRangeParam_t Light_HslRangeParam[APPLICATION_NUMBER_OF_ELEMENTS];

Light_PublishOpcodeList_t Light_PublishOpcodeList[APPLICATION_NUMBER_OF_ELEMENTS];

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
extern Generic_DefaultTransitionParam_t Generic_DefaultTransitionParam;
#endif
MOBLEUINT8 LightUpdateFlag = 0;
extern Model_Binding_Var_t Model_Binding_Var;

const MODEL_OpcodeTableParam_t Light_Opcodes_Table[] = 
{    
/* model_id                               opcode                                 reliable     min_payload_size max_payload_size response_opcode                      min_response_size max_response_size */
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS                                                                                                                                 
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_GET,                   MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_SET,                   MOBLE_TRUE,  3,               5,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_SET_UNACK,             MOBLE_FALSE, 3,               5,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_GET,            MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_SET,            MOBLE_TRUE,  3,               5,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_SET_UNACK,      MOBLE_FALSE, 3,               5,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_LAST_GET,              MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_LAST_STATUS,        2,                2},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_DEFAULT_GET,           MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_DEFAULT_STATUS,     2,                2},
  {LIGHT_LIGHTNESS_SERVER_MODEL_ID,       LIGHT_LIGHTNESS_RANGE_GET,             MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_RANGE_STATUS,       5,                5},
#endif                                                                                                                                                                     
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP                                                                                                                           
  {LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID, LIGHT_LIGHTNESS_DEFAULT_SET,           MOBLE_TRUE,  2,               2,                LIGHT_LIGHTNESS_DEFAULT_STATUS,     2,                2},                 
  {LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID, LIGHT_LIGHTNESS_DEFAULT_SET_UNACK,     MOBLE_FALSE, 2,               2,                0,                                  2,                2}, 
  {LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID, LIGHT_LIGHTNESS_RANGE_SET,             MOBLE_TRUE,  4,               4,                LIGHT_LIGHTNESS_RANGE_STATUS,       5,                5},    
  {LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID, LIGHT_LIGHTNESS_RANGE_SET_UNACK,       MOBLE_FALSE, 4,               4,                0,                                  5,                5}, 
#endif                                                                                                                                                                     
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL                                                                                                                                       
  {LIGHT_CTL_SERVER_MODEL_ID,             LIGHT_CTL_GET,                         MOBLE_TRUE,  0,               0,                LIGHT_CTL_STATUS,                   4,                9},     
  {LIGHT_CTL_SERVER_MODEL_ID,             LIGHT_CTL_SET,                         MOBLE_TRUE,  7,               9,                LIGHT_CTL_STATUS,                   4,                9},     
  {LIGHT_CTL_SERVER_MODEL_ID,             LIGHT_CTL_SET_UNACK,                   MOBLE_FALSE, 7,               9,                LIGHT_CTL_STATUS,                   4,                9},
  {LIGHT_CTL_SERVER_MODEL_ID,             LIGHT_CTL_TEMPERATURE_RANGE_GET,       MOBLE_TRUE,  0,               0,                LIGHT_CTL_TEMPERATURE_RANGE_STATUS, 5,                5},
  {LIGHT_CTL_SERVER_MODEL_ID,             LIGHT_CTL_DEFAULT_GET,                 MOBLE_TRUE,  0,               0,                LIGHT_CTL_DEFAULT_STATUS,           6,                6},  
#endif                                                                                                                                                                     
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP                                                                                                                                 
  {LIGHT_CTL_SETUP_SERVER_MODEL_ID,       LIGHT_CTL_DEFAULT_SET,                 MOBLE_TRUE,  6,               6,                LIGHT_CTL_DEFAULT_STATUS,           6,                6},   
  {LIGHT_CTL_SETUP_SERVER_MODEL_ID,       LIGHT_CTL_DEFAULT_SET_UNACK,           MOBLE_FALSE, 6,               6,                LIGHT_CTL_DEFAULT_STATUS,           6,                6}, 
  {LIGHT_CTL_SETUP_SERVER_MODEL_ID,       LIGHT_CTL_TEMPERATURE_RANGE_SET,       MOBLE_TRUE,  4,               4,                LIGHT_CTL_TEMPERATURE_RANGE_STATUS, 5,                5},   
  {LIGHT_CTL_SETUP_SERVER_MODEL_ID,       LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK, MOBLE_FALSE, 4,               4,                0,                                  5,                5},
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE  
  {LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID, LIGHT_CTL_TEMPERATURE_GET,             MOBLE_TRUE,  0,               0,                LIGHT_CTL_TEMPERATURE_STATUS,       4,                9},
  {LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID, LIGHT_CTL_TEMPERATURE_SET,             MOBLE_TRUE,  5,               7,                LIGHT_CTL_TEMPERATURE_STATUS,       4,                9},
  {LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID, LIGHT_CTL_TEMPERATURE_SET_UNACK,       MOBLE_FALSE, 5,               7,                LIGHT_CTL_TEMPERATURE_STATUS,       4,                9},
#endif       
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_GET,                         MOBLE_TRUE,  0,               0,                LIGHT_HSL_STATUS,                   6,                7}, 
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_SET,                         MOBLE_TRUE,  7,               9,                LIGHT_HSL_STATUS,                   6,                7},   
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_SET_UNACK,                   MOBLE_FALSE, 7,               9,                LIGHT_HSL_STATUS,                   6,                7},    
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_TARGET_GET,                  MOBLE_TRUE,  0,               0,                LIGHT_HSL_TARGET_STATUS,            6,                7}, 
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_DEFAULT_GET,                 MOBLE_TRUE,  0,               0,                LIGHT_HSL_DEFAULT_STATUS,           6,                6},  
  {LIGHT_HSL_SERVER_MODEL_ID,             LIGHT_HSL_RANGE_GET,                   MOBLE_TRUE,  0,               0,                LIGHT_HSL_RANGE_STATUS,             9,                9},
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP   
  {LIGHT_HSL_SETUP_SERVER_MODEL_ID,       LIGHT_HSL_DEFAULT_SET,                 MOBLE_TRUE,  6,               6,                LIGHT_HSL_DEFAULT_STATUS,            6,               6},   
  {LIGHT_HSL_SETUP_SERVER_MODEL_ID,       LIGHT_HSL_DEFAULT_SET_UNACK,           MOBLE_FALSE, 6,               6,                LIGHT_HSL_DEFAULT_STATUS,            6,               6},
  {LIGHT_HSL_SETUP_SERVER_MODEL_ID,       LIGHT_HSL_RANGE_SET,                   MOBLE_TRUE,  8,               8,                LIGHT_HSL_RANGE_STATUS,              9,               9},   
  {LIGHT_HSL_SETUP_SERVER_MODEL_ID,       LIGHT_HSL_RANGE_SET_UNACK,             MOBLE_FALSE, 8,               8,                0,                                   9,               9},  
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE    
  {LIGHT_HSL_HUE_SERVER_MODEL_ID,         LIGHT_HSL_HUE_GET,                     MOBLE_TRUE,  0,               0,                LIGHT_HSL_HUE_STATUS,                2,               5},   
  {LIGHT_HSL_HUE_SERVER_MODEL_ID,         LIGHT_HSL_HUE_SET,                     MOBLE_TRUE,  3,               5,                LIGHT_HSL_HUE_STATUS,                2,               5},   
  {LIGHT_HSL_HUE_SERVER_MODEL_ID,         LIGHT_HSL_HUE_SET_UNACK,               MOBLE_FALSE, 3,               5,                LIGHT_HSL_HUE_STATUS,                2,               5}, 
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION    
  {LIGHT_HSL_SATURATION_SERVER_MODEL_ID,  LIGHT_HSL_SATURATION_GET,             MOBLE_TRUE,   0,               0,                LIGHT_HSL_SATURATION_STATUS,         2,               5},   
  {LIGHT_HSL_SATURATION_SERVER_MODEL_ID,  LIGHT_HSL_SATURATION_SET,             MOBLE_TRUE,   3,               5,                LIGHT_HSL_SATURATION_STATUS,         2,               5},    
  {LIGHT_HSL_SATURATION_SERVER_MODEL_ID,  LIGHT_HSL_SATURATION_SET_UNACK,       MOBLE_FALSE,  3,               5,                LIGHT_HSL_SATURATION_STATUS,         2,               5},    
#endif   
  {0}		
};

/* Private function prototypes -----------------------------------------------*/
MOBLE_RESULT Light_Ctl_Status(MOBLEUINT8* pLightCtl_status, 
                              MOBLEUINT32 *pLength,
                              MODEL_MessageHeader_t *pmsgParam);
MOBLE_RESULT Light_CtlTemperature_Status(MOBLEUINT8* pLightCtlTemp_status, 
                                         MOBLEUINT32 *pLength,
                                         MODEL_MessageHeader_t *pmsgParam);
WEAK_FUNCTION (MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t* pLight_LightnessParam,
                                                      MOBLEUINT8 OptionalValid, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                                            MOBLEUINT8 OptionalValid, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                                             MOBLEUINT8 OptionalValid, 
                                                             uint16_t dstPeer, 
                                                             uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                                           MOBLEUINT8 OptionalValid, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                               MOBLEUINT8 OptionalValid, 
                                               uint16_t dstPeer, 
                                               uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                                          MOBLEUINT8 OptionalValid, 
                                                          uint16_t dstPeer, 
                                                          uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                                                MOBLEUINT8 OptionalValid, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                                      MOBLEUINT8 OptionalValid,
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                               MOBLEUINT8 OptionalValid, 
                                               uint16_t dstPeer, 
                                               uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                                  MOBLEUINT8 OptionalValid, 
                                                  uint16_t dstPeer,
                                                  uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                                         MOBLEUINT8 OptionalValid, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslDefaultParam_t* pLight_HslDefaultParam,
                                                      MOBLEUINT8 OptionalValid, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslRangeParam,
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Status(MOBLEUINT8 const *pLightness_status, 
                                                        MOBLEUINT32 pLength, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, 
                                                               MOBLEUINT32 pLength, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, 
                                                                MOBLEUINT32 pLength, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                                             MOBLEUINT32 pLength, 
                                                             MOBLEUINT16 dstPeer, 
                                                             MOBLEUINT8 elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, 
                                                              MOBLEUINT32 pLength, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, 
                                                  MOBLEUINT32 pLength, 
                                                  uint16_t dstPeer, 
                                                  uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, 
                                                             MOBLEUINT32 pLength, 
                                                             uint16_t dstPeer, 
                                                             uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, 
                                                                   MOBLEUINT32 pLength, 
                                                                   uint16_t dstPeer, 
                                                                   uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, 
                                                         MOBLEUINT32 pLength, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Hsl_Status(MOBLEUINT8 const *pHsl_status, 
                                                  MOBLEUINT32 pLength, 
                                                  uint16_t dstPeer, 
                                                  uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslHue_Status(MOBLEUINT8 const *pHslHue_status, 
                                                     MOBLEUINT32 pLength, 
                                                     uint16_t dstPeer, 
                                                     uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, 
                                                            MOBLEUINT32 pLength, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, 
                                                         MOBLEUINT32 pLength, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslRange_Status(MOBLEUINT8 const *pHslRange_status, 
                                                       MOBLEUINT32 pLength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState, 
                                                          uint16_t dstPeer, 
                                                          uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState, 
                                                                 uint16_t dstPeer, 
                                                                 uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslDefaultStatus(MOBLEUINT8* lHslDefaultState, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex));
WEAK_FUNCTION(void HSL2RGB_Conversion(MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void RgbF_Create(MOBLEUINT16 value1, 
                               MOBLEUINT16 value2, 
                               MOBLEUINT16 value3, 
                               MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Light_UpdatePWMValue(MOBLEUINT8 state));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessLastStatus(MOBLEUINT8* lDefaultState, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTargetStatus(MOBLEUINT8* lCtlLightState, 
                                                          MOBLEUINT16 dstPeer, 
                                                          MOBLEUINT8 elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslTargetStatus(MOBLEUINT8* lHslState,
                                                          MOBLEUINT16 dstPeer, 
                                                          MOBLEUINT8 elementIndex));
MOBLE_RESULT Light_LightnessStateUpdate_Process(MOBLEUINT8 elementIndex);
#if 0
MOBLE_RESULT Light_LC_LightnessStateUpdate_Process(Light_LC_TemporaryStatus_t *lc_Temp_Value, 
                                                   MOBLEUINT16 dstPeer, 
                                                   MOBLEUINT8 elementIndex);
#endif
MOBLE_RESULT LightLightness_ActualUpdate(MOBLEUINT16 lightActual, 
                                         MOBLEUINT8 elementIndex);
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Last_Set(Light_LightnessStatus_t* pLight_LightnessParam,
                                                          MOBLEUINT8 elementIndex));

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Light_Lightness_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  plightness_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Lightness_Set(const MOBLEUINT8* plightness_param, 
                                 MOBLEUINT32 length,
                                 MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.2 Light Lightness Set
  
  Lightness             2B The target value of the Light Lightness Actual state. 
  TID                   1B Transaction Identifier
  Transition Time       1B Format as defined in Section 3.1.3. (Optional)
  Delay                 1B Message execution delay in 5 millisecond steps (C.1)
  */
  TRACE_M(TF_LIGHT_M, "Callback Received \r\n");
  
  Light_LightnessParam.TargetLightnessStatus = (plightness_param[0] | plightness_param[1] << 8);
  Light_LightnessParam.TID = plightness_param[2];
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP  
  /* Binding of actual lightness value with lightness range */
  Light_Actual_RangeBinding(&Light_LightnessParam, pmsgParam->elementIndex);
#endif  
  /* Check for Optional Parameters */ 
  if ((length > 3) && (plightness_param[3] !=0))
  {
    /* Transition_Time & Delay_Time Present */
    Light_LightnessParam.TransitionTime = plightness_param[3];
    Light_LightnessParam.Delay_Time = plightness_param[4]; 
    
    /* Copy the received data in status message which needs
    to be set in application messages */  
    Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16 = Light_LightnessParam.TargetLightnessStatus;
    Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime = Light_LightnessParam.TransitionTime;
    
    /* copy status parameters in Temporary parameters for transition 
    process.
    */
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = 
                   Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16;    
    Light_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = 
                   Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime;
    
    /* Function to calculate time parameters, step resolution
    step size for transition state machine.
    */
    Light_GetStepValue(plightness_param[3],pmsgParam->elementIndex);
    /*option parameter flag, enable to sent all required parameter in status.*/
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_LIGHTNESS_TRANSITION_START;  
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
    Light_LightnessDefaultTransitionValue(&Light_LightnessParam,
                                          pmsgParam->elementIndex);
#else
    /* When no optional parameter received, target value will
    be set as present value in application.
    */
    Light_TransitionParameterReset(pmsgParam->elementIndex);
		
    Light_LightnessStatus[pmsgParam->elementIndex].LightnessPresentValue16 = 
                                     Light_LightnessParam.TargetLightnessStatus; 
#endif   
     
    if(Light_LightnessStatus[pmsgParam->elementIndex].LightnessPresentValue16 > 0x00)
    {
      Light_LightnessStatus[pmsgParam->elementIndex].LightnessLastStatus = 
        Light_LightnessStatus[pmsgParam->elementIndex].LightnessPresentValue16;
      
      (LightAppli_cb.Lightness_Last_Set_cb)(&Light_LightnessStatus[pmsgParam->elementIndex],                                     
                                            pmsgParam->elementIndex);
    }
  }
   
  /* Application Callback */
  (LightAppli_cb.Lightness_Set_cb)(&Light_LightnessStatus[pmsgParam->elementIndex], 
                                   0,pmsgParam->dst_peer, 
                                   pmsgParam->elementIndex);
  
#ifdef ENABLE_MODEL_BINDING    
  /* Binding of actual light lightness with other models */
  Light_Lightness_Binding(BINDING_LIGHT_LIGHTNESS_ACTUAL_SET , length,pmsgParam->elementIndex,
              Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);  
#endif  
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light Lightness Status
* @param  pLightness_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Lightness_Status(MOBLEUINT8* pLightness_status, MOBLEUINT32* pLength,\
                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 
  Following is the status message:
  Present Lightness     1B The present value of the Generic OnOff state. 
  Target Lightness      1B The target value of the Generic OnOff state (optional).
  Remaining Time        1B Time to complete the transition to the target Light Lightness
  
  */ 
  
  TRACE_M(TF_LIGHT_M, "Light_Lightness_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#824B! \n\r");
  
  *(pLightness_status) = Light_LightnessStatus[pmsgParam->elementIndex].LightnessPresentValue16;
  *(pLightness_status+1) = Light_LightnessStatus[pmsgParam->elementIndex].LightnessPresentValue16 >> 8;
  
  /* This condition will be processed when message received will have
  transition time and delay parameter.
  or the transition will be in process
  */
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    *(pLightness_status+2) = Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16;
    *(pLightness_status+3) = Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16 >>8;
    *(pLightness_status+4) = Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 5;     
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0;    
    TRACE_M(TF_LIGHT_M, "Light_Lightness_Status sent with transition \r\n");
  }
  else
  {    
    /* When no optional parameter received then, target value will
    be sent in status message.
    */    
    *pLength = 2;          
     TRACE_M(TF_LIGHT_M, "Light_Lightness_Status sent without transition \r\n");
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LightnessLinear_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  plightnessLinear_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessLinear_Set(const MOBLEUINT8* plightnessLinear_param,\
                                        MOBLEUINT32 length,\
                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.6 Light Lightness Linear Set
  
  Lightness             2B The target value of the Light Lightness Actual state. 
  TID                   1B Transaction Identifier
  Transition Time       1B Format as defined in Section 3.1.3. (Optional)
  Delay                 1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessLinear_Set callback received \r\n");
  
  Light_LightnessParam.TargetLightnessLinearStatus = ((plightnessLinear_param[0])
                                                      | (plightnessLinear_param[1] << 8));
  Light_LightnessParam.TID = plightnessLinear_param[2];
  
  /* Check for Optional Parameters */ 
  if ((length > 3) && (plightnessLinear_param[3] !=0))
  {
    /* Transition_Time & Delay_Time Present */
    Light_LightnessParam.TransitionTime = plightnessLinear_param[3];
    Light_LightnessParam.Delay_Time = plightnessLinear_param[4]; 
    
    /* Copy the received data in status message which needs
    to be set in application messages */
    Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16 = Light_LightnessParam.TargetLightnessLinearStatus;
    Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime = Light_LightnessParam.TransitionTime;
    /* copy status parameters in Temporary parameters for transition 
    process.
    */
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16;    
    Light_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime;
    /* Function to calculate time parameters, step resolution
    step size for transition state machine.
    */
    
    Light_GetStepValue(plightnessLinear_param[3],pmsgParam->elementIndex);
    /*transition process enable flag. */
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_LINEAR_TRANSITION_START; 
    /*option parameter flag, enable to sent all required parameter in status.*/
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
    Light_LightnessLinearDefaultTransitionValue(&Light_LightnessParam,
                                          pmsgParam->elementIndex);
#else
    /* When no optional parameter received, target value will
    be set as present value in application.
    */
    Light_TransitionParameterReset(pmsgParam->elementIndex);
		
    Light_LightnessStatus[pmsgParam->elementIndex].LinearPresentValue16 = 
      Light_LightnessParam.TargetLightnessLinearStatus; 
#endif  
  }
  
  /* Application Callback */
  (LightAppli_cb.Lightness_Linear_Set_cb)(&Light_LightnessStatus[pmsgParam->elementIndex], OptionalValid, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
#ifdef ENABLE_MODEL_BINDING   
  /* Binding of data of light lightness with light linear */
  Light_Lightness_Linear_Binding(BINDING_LIGHT_LIGHTNESS_LINEAR_SET ,length, pmsgParam->elementIndex,
                  Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
                                 
#endif   

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC  
  Light_Linear_ActualBinding(length, pmsgParam->elementIndex);
#endif    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light Lightness Linear Status
* @param  pLightnessLinear_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessLinear_Status(MOBLEUINT8* pLightnessLinear_status,
                                           MOBLEUINT32 *pLength,\
                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 
  Following is the status message:
  Present Lightness     2B The present value of the Light Lightness Linear state. 
  Target Lightness      2B The target value of the Light Lightness Linear state (Optional).
  Remaining Time        1B Time to complete the transition to the target Light Lightness Li
  */
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessLinear_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#824F! \n\r");
  
  *(pLightnessLinear_status) = Light_LightnessStatus[pmsgParam->elementIndex].LinearPresentValue16;
  *(pLightnessLinear_status+1) = Light_LightnessStatus[pmsgParam->elementIndex].LinearPresentValue16 >> 8;
    
  /* This condition will be processed when message received will have
  transition time and delay parameter.
  or the transition will be in process
  */
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    *(pLightnessLinear_status+2) = Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16;
    *(pLightnessLinear_status+3) = Light_LightnessStatus[pmsgParam->elementIndex].TargetValue16 >>8;
    *(pLightnessLinear_status+4) = Light_LightnessStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 5;
    
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0U;          
  }
  else
  { 
    /* When no optional parameter received then, target value will
    be sent in status message.
    */
    *pLength = 2;
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LightnessLast_Set: This function is called for both Acknowledged and 
*         unacknowledged message. The Acknowledgement is taken care by the Library
* @param  plightnessLast_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT

*/ 
MOBLE_RESULT Light_LightnessLast_Set(const MOBLEUINT8* plightnessLast_param, 
                                      MOBLEUINT32 length,\
                                      MODEL_MessageHeader_t *pmsgParam)
{
  Light_LightnessStatus[pmsgParam->elementIndex].LightnessLastStatus = plightnessLast_param[1] << 8;
  Light_LightnessStatus[pmsgParam->elementIndex].LightnessLastStatus |= plightnessLast_param[0];
  
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Light Lightness Last Status
* @param  pLightnessLast_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessLast_Status(MOBLEUINT8* pLightnessLast_status, 
                                         MOBLEUINT32 *pLength,\
                                         MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.10 Light Lightness Last State
  Following is the status message:
  Lightness     2B The value of the Light Lightness Last. 
  */
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessLast_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8253! \n\r");
  
  *pLightnessLast_status = Light_LightnessStatus[pmsgParam->elementIndex].LightnessLastStatus ;
  *(pLightnessLast_status+1) = Light_LightnessStatus[pmsgParam->elementIndex].LightnessLastStatus >> 8;
  *pLength = 2;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_LightnessDefault_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  plightnessDefault_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessDefault_Set(const MOBLEUINT8* plightnessDefault_param, 
                                         MOBLEUINT32 length,\
                                         MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.12 Light Default Lightness Set
  Lightness   2B The target value of the Light Lightness Actual state. 
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessDefault_Set callback received \r\n");  
  
  Light_LightnessDefaultParam[pmsgParam->elementIndex].LightnessDefaultStatus = plightnessDefault_param[1] << 8;
  Light_LightnessDefaultParam[pmsgParam->elementIndex].LightnessDefaultStatus |= plightnessDefault_param[0];
  
  /* Application Callback */
  (LightAppli_cb.Lightness_Default_Set_cb)(&Light_LightnessDefaultParam[pmsgParam->elementIndex], 
                                   OptionalValid, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light Lightness Default Status
* @param  pLightnessDefault_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessDefault_Status(MOBLEUINT8* pLightnessDefault_status,
                                            MOBLEUINT32 *pLength,\
                                            MODEL_MessageHeader_t *pmsgParam)
{
  /* 
  Following is the status message:
  Lightness     2B The value of the Light Lightness Default state. 
  */
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessDefault_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8255! \n\r");
  
  /* Function call back to get the values from application*/
  *pLightnessDefault_status = Light_LightnessDefaultParam[pmsgParam->elementIndex].LightnessDefaultStatus ;
  *(pLightnessDefault_status+1) = Light_LightnessDefaultParam[pmsgParam->elementIndex].LightnessDefaultStatus >>8;
  *pLength = 2;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_LightnessRange_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  plightnessRange_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessRange_Set(const MOBLEUINT8* plightnessRange_param, 
                                       MOBLEUINT32 length,\
                                       MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.16 Light Lightness Range Set 
  Min Range    2B The Minimum range value of the Light Lightness state. 
  Max Range    2B The Maximum range value of the Light Lightness state         
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessRange_Set callback received \r\n");
  
  Light_LightnessRangeParam[pmsgParam->elementIndex].MinRangeStatus = plightnessRange_param[1] << 8;
  Light_LightnessRangeParam[pmsgParam->elementIndex].MinRangeStatus |= plightnessRange_param[0];
  
  Light_LightnessRangeParam[pmsgParam->elementIndex].MaxRangeStatus = plightnessRange_param[3] << 8;
  Light_LightnessRangeParam[pmsgParam->elementIndex].MaxRangeStatus |= plightnessRange_param[2];
  Light_LightnessRangeParam[pmsgParam->elementIndex].StatusCode = SUCCESS_STATUS_CODE;
  
  /* condition to check the minimum range value according to the specification*/
  /* set the status code for status message according to value of range*/  
  
  /* Application Callback */
  (LightAppli_cb.Lightness_Range_Set_cb)(&Light_LightnessRangeParam[pmsgParam->elementIndex], OptionalValid,pmsgParam->dst_peer,pmsgParam->elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_LightnessRange_Status
* @param  pLightnessRange_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LightnessRange_Status(MOBLEUINT8* pLightnessRange_status,
                                          MOBLEUINT32 *pLength,\
                                          MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.1.18 Light Lightness Range Status  
  Min Range      2B The Minimum range value of the Light Lightness state. 
  Max Range      2B The Maximum range value of the Light Lightness state         
  */
  
  TRACE_M(TF_LIGHT_M, "Light_LightnessRange_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8257! \n\r");
  
  *(pLightnessRange_status) = Light_LightnessRangeParam[pmsgParam->elementIndex].StatusCode;
  *(pLightnessRange_status+1) = Light_LightnessRangeParam[pmsgParam->elementIndex].MinRangeStatus;
  *(pLightnessRange_status+2) = Light_LightnessRangeParam[pmsgParam->elementIndex].MinRangeStatus >> 8;
  *(pLightnessRange_status+3) = Light_LightnessRangeParam[pmsgParam->elementIndex].MaxRangeStatus;
  *(pLightnessRange_status+4) = Light_LightnessRangeParam[pmsgParam->elementIndex].MaxRangeStatus >> 8;
  *pLength = 5;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CTL_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pLightCtl_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Ctl_Set(const MOBLEUINT8* pLightCtl_param, MOBLEUINT32 length,\
                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.2 Light Lightness Ctl Set
  
  Ctl Lightness         2B The target value of the Light Lightness Ctl state.
  Ctl Temperature       2B The target value of the Light Lightness Ctl state.
  Ctl Delta Uv          2B The target value of the Light Lightness Ctl state.
  TID                   1B Transaction Identifier
  Transition Time       1B Format as defined in Section 3.1.3. (Optional)
  Delay                 1B Message execution delay in 5 millisecond steps (C.1)
  */
  
  TRACE_M(TF_LIGHT_M, "Light_Ctl_Set callback received \r\n");
  
  /* Received parameters from client */
  Light_CtlParam.CTL_Lightness    = pLightCtl_param[1] << 8;
  Light_CtlParam.CTL_Lightness   |= pLightCtl_param[0];
  Light_CtlParam.CTL_Temperature  = pLightCtl_param[3] << 8;
  Light_CtlParam.CTL_Temperature |= pLightCtl_param[2];
  Light_CtlParam.CTL_DeltaUv      = pLightCtl_param[5] << 8;
  Light_CtlParam.CTL_DeltaUv     |= pLightCtl_param[4];
  Light_CtlParam.CTL_TID          = pLightCtl_param[6];
  
  if((Light_CtlParam.Last_Lightness == Light_CtlParam.CTL_Lightness)
     && (Light_CtlParam.Last_Temperature == Light_CtlParam.CTL_Temperature))
  {
    TRACE_M(TF_LIGHT_M, "Received Value is same to last values received \r\n");
  }
  
  Light_CtlParam.Last_Lightness = Light_CtlParam.CTL_Lightness;
  Light_CtlParam.Last_Temperature = Light_CtlParam.CTL_Temperature;
  Light_CtlStatus[pmsgParam->elementIndex].PresentCtlDelta16 = Light_CtlParam.CTL_DeltaUv;
  
  /* Check if the optional parameters are available for the transition */
  if((length > 7) && (pLightCtl_param[7] !=0)) 
  {
    Light_CtlParam.CTL_TransitionTime = pLightCtl_param[7];    
    Light_CtlParam.CTL_Delay          = pLightCtl_param[8];
    
    Light_CtlStatus[pmsgParam->elementIndex].TargetCtlLightness16 = Light_CtlParam.CTL_Lightness;
    Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16 = Light_CtlParam.CTL_Temperature;
    Light_CtlStatus[pmsgParam->elementIndex].TargetCtlDeltaUv16 = Light_CtlParam.CTL_DeltaUv;
    Light_CtlStatus[pmsgParam->elementIndex].RemainingTime = Light_CtlParam.CTL_TransitionTime;
    
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlLightness16;
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_2 = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16;
    Light_GetStepValue(pLightCtl_param[7],pmsgParam->elementIndex);  
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_CTL_TRANSITION_START;
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
     Light_CTLDefaultTransitionValue(&Light_CtlParam, pmsgParam->elementIndex);
#else
    /* When no optional parameter received, target value will
    be set as present value in application.
    */
    Light_TransitionParameterReset(pmsgParam->elementIndex);
    Light_CtlStatus[pmsgParam->elementIndex].PresentCtlLightness16 = Light_CtlParam.CTL_Lightness;
    Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16 = Light_CtlParam.CTL_Temperature;
#endif    
  }
  /* Application Callback */
  (LightAppli_cb.Light_Ctl_Set_cb)(&Light_CtlStatus[pmsgParam->elementIndex], 0,
                                   pmsgParam->dst_peer,pmsgParam->elementIndex);  
  
#ifdef ENABLE_MODEL_BINDING   
  /* binding of light ctl with actual lightness */
  Light_Ctl_LightActual_Binding(BINDING_LIGHT_CTL_SET,pmsgParam->elementIndex,
           Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
#endif 
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light CTL Status
* @param  pLightCtl_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Ctl_Status(MOBLEUINT8* pLightCtl_status, MOBLEUINT32 *pLength,
                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.4 Light Lightness Ctl Status 
  Present Ctl Lightness           2B The Present ctl Lightness value of the model state. 
  Present Ctl Temperature         2B The Present ctl Temperature value of the model state. 
  Target Ctl Lightness            2B The Target ctl Lightness value of the model state.
  Target Ctl Temeperature         2B The Target ctl Temperature value of the model state.
  Remaining Time                  1B The Remaining Time value of the model state        
  */
  
  TRACE_M(TF_LIGHT_M,"Light_Ctl_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#825D! \n\r");
  
  *(pLightCtl_status) =  Light_CtlStatus[pmsgParam->elementIndex].PresentCtlLightness16 ; 
  *(pLightCtl_status+1) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlLightness16 >> 8;
  *(pLightCtl_status+2) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16;
  *(pLightCtl_status+3) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16 >> 8;
  
  /* Check if optional parameters are available */
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    *(pLightCtl_status+4) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlLightness16;
    *(pLightCtl_status+5) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlLightness16 >>8;
    *(pLightCtl_status+6) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16;
    *(pLightCtl_status+7) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16 >>8;
    *(pLightCtl_status+8) = Light_CtlStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 9;
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0;    
    TRACE_M(TF_LIGHT_M, "Light_Ctl_Status sent with transition \r\n");
  }
  else
  {     
    *pLength = 4;         
    TRACE_M(TF_LIGHT_M, "Light_Ctl_Status sent without transition \r\n");
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CTLTemperature_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pLightCtlTemp_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlTemperature_Set(const MOBLEUINT8* pLightCtlTemp_param,
                                      MOBLEUINT32 length,\
                                      MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.6 Light Ctl Temperature Set 
  Ctl Temperature       2B The target value of the Light Temperature Ctl state.
  Ctl Delta Uv          2B The target value of the Light Temperature Ctl state.
  TID                   1B Transaction Identifier
  Transition Time       1B Format as defined in Section 3.1.3. (Optional)
  Delay                 1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLEUINT8 OptionalValid = 0;   
  
  TRACE_M(TF_LIGHT_M,"Light_CtlTemperature_Set callback received \r\n");
  
  /* Received parameters from client */
  Light_CtlParam.CTL_Temperature   = pLightCtlTemp_param[1] << 8;
  Light_CtlParam.CTL_Temperature  |= pLightCtlTemp_param[0];
  Light_CtlParam.CTL_DeltaUv  = pLightCtlTemp_param[3] << 8;
  Light_CtlParam.CTL_DeltaUv |= pLightCtlTemp_param[2];
  Light_CtlParam.CTL_TID     = pLightCtlTemp_param[4] << 8;
  
  /* Binding with ctl temperature range value */
  Light_CtlTemperature_TempRangeBinding(&Light_CtlParam, pmsgParam->elementIndex);
  
  /* Check if optional parameters are available For transition */
  if((length > 5) && (pLightCtlTemp_param[5] !=0))
  {
    Light_CtlParam.CTL_TransitionTime = pLightCtlTemp_param[5];
    Light_CtlParam.CTL_Delay          = pLightCtlTemp_param[6];
    
    Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16 = Light_CtlParam.CTL_Temperature;
    Light_CtlStatus[pmsgParam->elementIndex].TargetCtlDeltaUv16 = Light_CtlParam.CTL_DeltaUv;
    Light_CtlStatus[pmsgParam->elementIndex].RemainingTime = Light_CtlParam.CTL_TransitionTime;
    
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16;
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_2 = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlDeltaUv16;
    Light_GetStepValue(pLightCtlTemp_param[5],pmsgParam->elementIndex); 
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_TEMPERATURE_TRANSITION_START;
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME       
    Light_CTLTemperatureDefaultTransitionValue(&Light_CtlParam, pmsgParam->elementIndex);
#else
    /* When no optional parameter received, target value will
    be set as present value in application.
    */
    Light_TransitionParameterReset(pmsgParam->elementIndex);
    Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16 = Light_CtlParam.CTL_Temperature;
    Light_CtlStatus[pmsgParam->elementIndex].PresentCtlDelta16 = Light_CtlParam.CTL_DeltaUv;
#endif    
  }
  
  /* Application Callback */
  (LightAppli_cb.Light_CtlTemperature_Set_cb)(&Light_CtlStatus[pmsgParam->elementIndex], OptionalValid,pmsgParam->dst_peer,pmsgParam->elementIndex);
#ifdef ENABLE_MODEL_BINDING  
  /* Binding of data Ctl Temperature  with Generic Level */
  Light_CtlTemperature_Binding(pmsgParam->elementIndex,Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,
                                        Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
#endif   
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CtlTemperature_Status
* @param  pLightCtlTemp_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlTemperature_Status(MOBLEUINT8* pLightCtlTemp_status, 
                                         MOBLEUINT32 *pLength,
                                         MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.4 Light Lightness Ctl Status
  
  Present Ctl Temperature         2B The Present ctl Temperature value of the model state. 
  Present Ctl Delta Uv            2B The Present ctl Delta Uv value of the model state. 
  Target Ctl Temperature          2B The Target ctl Tepmerature value of the model state.
  Target Ctl Delta Uv             2B The Target ctl Delta Uv value of the model state.
  Remaining Time                  1B The Remaining Time value of the model state
  */
  
  TRACE_M(TF_LIGHT_M,"Light_CtlTemperature_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8261! \n\r");
  
  *(pLightCtlTemp_status) =  Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16; 
  *(pLightCtlTemp_status+1) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlTemperature16  >> 8;
  *(pLightCtlTemp_status+2) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlDelta16;
  *(pLightCtlTemp_status+3) = Light_CtlStatus[pmsgParam->elementIndex].PresentCtlDelta16 >>8;
  
  /* Check if optional parameters are available For transition */
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    *(pLightCtlTemp_status+4) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16;
    *(pLightCtlTemp_status+5) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlTemperature16 >>8;
    *(pLightCtlTemp_status+6) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlDeltaUv16;
    *(pLightCtlTemp_status+7) = Light_CtlStatus[pmsgParam->elementIndex].TargetCtlDeltaUv16 >>8;
    *(pLightCtlTemp_status+8) = Light_CtlStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 9;
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0; 
    TRACE_M(TF_LIGHT_M, "Light_CtlTemperature_Status sent with transition \r\n");
  }
  else
  {   
    /* When no optional parameter received then, target value will
    be sent in status message.
    */
    *pLength = 4;             
    TRACE_M(TF_LIGHT_M, "Light_CtlTemperature_Status sent without transition \r\n");
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CtlTemperatureRange_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library.
* @param  plightCtlTempRange_param : Pointer to the parameters received for message.
* @param  length : Length of the parameters received for message.
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlTemperatureRange_Set(const MOBLEUINT8* plightCtlTempRange_param,
                                            MOBLEUINT32 length,\
                                            MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.10 Light Ctl Temperature Set
  
  Min Range          2B The minimum value for Ctl temperature state.
  Max Range          2B The maximum value for Ctl temperature state.
  */  
  MOBLEUINT8 OptionalValid = 0;
  TRACE_M(TF_LIGHT_M, "Light_CtlTemperatureRange_Set callback received \r\n"); 
  
  CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus = plightCtlTempRange_param[1] << 8;
  CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus |= plightCtlTempRange_param[0];
  
  CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus = plightCtlTempRange_param[3] << 8;
  CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus |= plightCtlTempRange_param[2];
  CtlTemperatureRangeParam[pmsgParam->elementIndex].StatusCode = SUCCESS_STATUS_CODE;
  
  /* Condition to check the minimum range value according to the specification */
  /* Set the status code for status message according to value of range*/  
  if((CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus < MIN_CTL_TEMP_RANGE) ||
     (CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus >MAX_CTL_TEMP_RANGE)  ||     
     (CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus < MIN_CTL_TEMP_RANGE) ||
     (CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus >MAX_CTL_TEMP_RANGE))
  {
    CtlTemperatureRangeParam[pmsgParam->elementIndex].StatusCode = MIN_RANGE_INVALID_STATUS_CODE;
  }
  
  if((CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus == UNKNOWN_CTL_TEMP_VALUE) ||
     (CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus < CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus))
  {
    CtlTemperatureRangeParam[pmsgParam->elementIndex].StatusCode = MAX_RANGE_INVALID_STATUS_CODE;
  } 
  /* Application Callback */
  (LightAppli_cb.Light_CtlTemperature_Range_Set_cb)(&CtlTemperatureRangeParam[pmsgParam->elementIndex], OptionalValid,pmsgParam->dst_peer,pmsgParam->elementIndex);
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CtlTemperatureRange_Status
* @param  pCtlTempRange_status: Pointer to the status message, which needs to be updated.
* @param  pLength: Pointer to the Length of the Status message.
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlTemperatureRange_Status(MOBLEUINT8* pCtlTempRange_status, 
                                               MOBLEUINT32 *pLength,\
                                               MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.12 Light Ctl Temperature status 
  Status Code        1B The Status code for the temperature range status.
  Min Range          2B The minimum value for Ctl temperature state.
  Max Range          2B The maximum value for Ctl temperature state.
  */
  
  TRACE_M(TF_LIGHT_M, "Light_CtlTemperatureRange_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8262! \n\r");
  
  *pCtlTempRange_status = CtlTemperatureRangeParam[pmsgParam->elementIndex].StatusCode;
  *(pCtlTempRange_status+1) = CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus ;
  *(pCtlTempRange_status+2) = CtlTemperatureRangeParam[pmsgParam->elementIndex].MinRangeStatus >> 8;
  *(pCtlTempRange_status+3) = CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus ;
  *(pCtlTempRange_status+4) = CtlTemperatureRangeParam[pmsgParam->elementIndex].MaxRangeStatus >> 8;
  
  *pLength = 5;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CtlDefault_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library.
* @param  pCtlDefault_param : Pointer to the parameters received for message.
* @param  length : Length of the parameters received for message.
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlDefault_Set(const MOBLEUINT8* pCtlDefault_param,
                                  MOBLEUINT32 length,\
                                  MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.6 Light Ctl Default Set  
  Ctl Lightness         2B The Default value of the Ctl Lightness.
  Ctl Temperature       2B The Default value of the Ctl Temperature..
  Ctl Delta Uv          2B The Default value of the Ctl Delta Uv..  
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M,"Light_CtlDefault_Set callback received \r\n");
  
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultLightness16 = pCtlDefault_param[1] << 8;
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultLightness16 |= pCtlDefault_param[0];
  
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultTemperature16 = pCtlDefault_param[3] << 8;
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultTemperature16 |= pCtlDefault_param[2];
  
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultDeltaUv = pCtlDefault_param[5] << 8;
  Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultDeltaUv |= pCtlDefault_param[4];              
  
  /* Application Callback */
  (LightAppli_cb.Light_CtlDefault_Set_cb)(&Light_CtlDefaultParam[pmsgParam->elementIndex] , OptionalValid,
                                          pmsgParam->dst_peer,
                                          pmsgParam->elementIndex);
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_CtlDefault_Status
* @param  pCtlDefault_status: Pointer to the status message, which needs to be updated.
* @param  pLength: Pointer to the Length of the Status message.
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_CtlDefault_Status(MOBLEUINT8* pCtlDefault_status, 
                                     MOBLEUINT32 *pLength,\
                                     MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.2.16 Light Ctl Default Status 
  Ctl Lightness         2B The Default value of the Ctl Lightness.
  Ctl Temperature       2B The Default value of the Ctl Temperature..
  Ctl Delta Uv          2B The Default value of the Ctl Delta Uv.. 
  */
  
  TRACE_M(TF_LIGHT_M, "Light_Ctl_DefaultStatus callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8267! \n\r");
  
  *pCtlDefault_status = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultLightness16 ;
  *(pCtlDefault_status +1) = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultLightness16 >> 8;
  *(pCtlDefault_status +2) = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultTemperature16;
  *(pCtlDefault_status +3) = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultTemperature16 >> 8;
  *(pCtlDefault_status +4) = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultDeltaUv;
  *(pCtlDefault_status +5) = Light_CtlDefaultParam[pmsgParam->elementIndex].CtlDefaultDeltaUv >> 8;
  
  *pLength = 6;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_Hsl_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pHsl_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Hsl_Set(const MOBLEUINT8* pHsl_param, 
                           MOBLEUINT32 length,\
                           MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.1.4.1 Light Hsl Set  
  HSL Lightness           2B The target value of the Light HSL Lightness state.
  HSL Hue                 2B The target value of the Light HSL Hue state.
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  TID                     1B Transaction Identifier
  Transition Time         1B Format as defined in Section 3.1.3. (Optional)
  Delay                   1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M,"Light_Hsl_Set callback received \r\n");
  
  Light_HslParam.HslLightness16 = (MOBLEUINT16)pHsl_param[1] << 8;
  Light_HslParam.HslLightness16 |= (MOBLEUINT16)pHsl_param[0];
  
  Light_HslParam.HslHueLightness16 = (MOBLEUINT16)pHsl_param[3] << 8;
  Light_HslParam.HslHueLightness16 |= (MOBLEUINT16)pHsl_param[2];
  
  Light_HslParam.HslSaturation16 = (MOBLEUINT16)pHsl_param[5] << 8;
  Light_HslParam.HslSaturation16 |= (MOBLEUINT16)pHsl_param[4];
  
  Light_HslParam.Hsl_TID = pHsl_param[6];
  
  /* Check if optional parameters are available For transition */
  if((length > 7U) && (pHsl_param[7] !=0))
  {
    Light_HslParam.Hsl_TransitionTime = pHsl_param[7];
    Light_HslParam.Hsl_Delay = pHsl_param[8];
    
    Light_HslStatus[pmsgParam->elementIndex].TargetHslLightness16 = Light_HslParam.HslLightness16;
    Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16 = Light_HslParam.HslHueLightness16;
    Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16 = Light_HslParam.HslSaturation16;
    Light_HslStatus[pmsgParam->elementIndex].RemainingTime = Light_HslParam.Hsl_TransitionTime;
    
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_HslStatus[pmsgParam->elementIndex].TargetHslLightness16;
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_2 = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16;
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_3 = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16;
    
    Light_GetStepValue(pHsl_param[7],pmsgParam->elementIndex);
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_HSL_TRANSITION_START;    
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
     Light_HSLDefaultTransitionValue(&Light_HslParam, pmsgParam->elementIndex);
#else
    Light_TransitionParameterReset(pmsgParam->elementIndex);
    Light_HslStatus[pmsgParam->elementIndex].PresentHslLightness16 = Light_HslParam.HslLightness16;
    Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16 = Light_HslParam.HslHueLightness16;
    Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16 = Light_HslParam.HslSaturation16;
#endif
  }
  /* Application Callback */
  (LightAppli_cb.Light_Hsl_Set_cb)(&Light_HslStatus[pmsgParam->elementIndex] , OptionalValid,pmsgParam->dst_peer,pmsgParam->elementIndex);
  
#ifdef ENABLE_MODEL_BINDING   
  /* Light Hsl Lightness binding with lightness Actual */
  Light_HslLightness_LightnessActualBinding(pmsgParam->elementIndex,Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,
                                                    Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
#endif  
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_Hsl_Status
* @param  pHsl_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_Hsl_Status(MOBLEUINT8* pHsl_status, 
                              MOBLEUINT32 *pLength,\
                              MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.4 Light Hsl Status 
  HSL Lightness           2B The present value of the Light HSL Lightness state.
  HSL Hue                 2B The present value of the Light HSL Hue state.
  HSL Saturation          2B The present value of the Light HSL Saturation state.
  Remaining Time          1B The remaining time value for transition of state.
  */
  
  TRACE_M(TF_LIGHT_M,"Light_Hsl_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#826D! \n\r");
  
  *(pHsl_status) = Light_HslStatus[pmsgParam->elementIndex].PresentHslLightness16 ; 
  *(pHsl_status+1) = Light_HslStatus[pmsgParam->elementIndex].PresentHslLightness16 >> 8;
  *(pHsl_status+2) = Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16;
  *(pHsl_status+3) = Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16 >> 8;
  *(pHsl_status+4) = Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16;
  *(pHsl_status+5) = Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16 >> 8;

  /* Check if optional parameters are available For transition */ 
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    *(pHsl_status+6) = Light_HslStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 7; /* Total packet length with remaining time */
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0;     
  }
  else
  {            
    *pLength = 6;         
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslHue_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pHslHue_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslHue_Set(const MOBLEUINT8* pHslHue_param, 
                              MOBLEUINT32 length,\
                              MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.8 Light Hsl Hue Set  
  HSL Lightness           2B The target value of the Light HSL Lightness state.
  HSL Hue                 2B The target value of the Light HSL Hue state.
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  TID                     1B Transaction Identifier
  Transition Time         1B Format as defined in Section 3.1.3. (Optional)
  Delay                   1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLEUINT8 OptionalValid = 0; 
  
  TRACE_M(TF_LIGHT_M,"Light_HslHue_Set callback received \r\n");
  
  Light_HslParam.HslHueLightness16 = (MOBLEUINT16)pHslHue_param[1] << 8;
  Light_HslParam.HslHueLightness16 |= (MOBLEUINT16)pHslHue_param[0];
  
  Light_HslParam.Hsl_TID = pHslHue_param[2];
  Light_HslHue_RangeBinding(&Light_HslParam, pmsgParam->elementIndex);
  
  /* Check if optional parameters are available For transition */
  if((length > 3U) && (pHslHue_param[3] !=0))
  {
    Light_HslParam.Hsl_TransitionTime = pHslHue_param[3];
    Light_HslParam.Hsl_Delay = pHslHue_param[4]; 
    
    Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16 = Light_HslParam.HslHueLightness16;
    Light_HslStatus[pmsgParam->elementIndex].RemainingTime = Light_HslParam.Hsl_TransitionTime;   
    
    /* Copy status parameters in Temporary parameters for transition process. */
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16;    
    
    /* Function to calculate time parameters, step resolution step size for transition state machine. */
    Light_GetStepValue(pHslHue_param[3],pmsgParam->elementIndex);
    
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_HSL_HUE_TRANSITION_START;    
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
     Light_HSLHueDefaultTransitionValue(&Light_HslParam, pmsgParam->elementIndex);
#else
    Light_TransitionParameterReset(pmsgParam->elementIndex);
    Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16 = Light_HslParam.HslHueLightness16;
#endif
  }
  /* Application Callback */
  (LightAppli_cb.Light_HslHue_Set_cb)(&Light_HslStatus[pmsgParam->elementIndex], OptionalValid, 
                                      pmsgParam->dst_peer, pmsgParam->elementIndex);
  
#ifdef ENABLE_MODEL_BINDING   
   /* Light Hsl Hue  binding with Generic level */
  Light_Hsl_Hue_Binding(pmsgParam->elementIndex,Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,
                           Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
#endif    
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslHue_Status
* @param  pHslHue_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslHue_Status(MOBLEUINT8* pHslHue_status, 
                                 MOBLEUINT32 *pLength,\
                                 MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.10 Light Hsl Hue Status 
  Present HSL Hue         2B The Present value of the Light HSL Hue state.
  Target HSL Hue          2B The target value of the Light HSL Hue state.
  Remaining Time          1B The remaining time value for transition of state.
  */
  
  TRACE_M(TF_LIGHT_M,"Light_HslHue_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#826E! \n\r");
  
  *(pHslHue_status) = Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16;
  *(pHslHue_status+1) = Light_HslStatus[pmsgParam->elementIndex].PresentHslHueLightness16 >> 8;
  
  /* Check if optional parameters are available For transition */
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {   
    *(pHslHue_status+2) = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16;
    *(pHslHue_status+3) = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16 >> 8;
    *(pHslHue_status+4) = Light_HslStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 5;
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 5;     
  }
  else
  {            
    *pLength = 2;         
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslSaturation_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pHslSaturation_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslSaturation_Set(const MOBLEUINT8* pHslSaturation_param,
                                     MOBLEUINT32 length,\
                                     MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.12 Light Hsl Saturation Set  
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  TID                     1B Transaction Identifier
  Transition Time         1B Format as defined in Section 3.1.3. (Optional)
  Delay                   1B Message execution delay in 5 millisecond steps (C.1)
  */
  
  TRACE_M(TF_LIGHT_M,"Light_HslSaturation_Set callback received \r\n");
  
  MOBLEUINT8 OptionalValid = 0;
  
  Light_HslParam.HslSaturation16 = (MOBLEUINT16)pHslSaturation_param[1] << 8;
  Light_HslParam.HslSaturation16 |= (MOBLEUINT16)pHslSaturation_param[0];
  
  Light_HslParam.Hsl_TID = pHslSaturation_param[2];
  Light_HslSaturation_RangeBinding(&Light_HslParam, pmsgParam->elementIndex);
  if((length > 3U) && (pHslSaturation_param[3] != 0))
  {
    Light_HslParam.Hsl_TransitionTime = pHslSaturation_param[3];
    Light_HslParam.Hsl_Delay = pHslSaturation_param[4];
    
    Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16 = Light_HslParam.HslSaturation16;
    Light_HslStatus[pmsgParam->elementIndex].RemainingTime = Light_HslParam.Hsl_TransitionTime;
    
    /* copy status parameters in Temporary parameters for transition process. */
    Light_TemporaryStatus[pmsgParam->elementIndex].TargetParam_1 = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16;    
    
    /* Function to calculate time parameters, step resolution
    step size for transition state machine.
    */
    Light_GetStepValue(pHslSaturation_param[3],pmsgParam->elementIndex);
    
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 1;
    Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag = LIGHT_HSL_SATURATION_TRANSITION_START; 
  }
  else
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME   
     Light_HSLSaturationDefaultTransitionValue(&Light_HslParam, pmsgParam->elementIndex);
#else
    Light_TransitionParameterReset(pmsgParam->elementIndex);
    Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16= Light_HslParam.HslSaturation16; 
#endif
  }
  /* Application Callback */
  (LightAppli_cb.Light_HslSaturation_Set_cb)(&Light_HslStatus[pmsgParam->elementIndex], 
                            OptionalValid, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
#ifdef ENABLE_MODEL_BINDING   
  /* Light Hsl Saturation binding with Generic level model*/
  Light_Hsl_Saturation_Binding(pmsgParam->elementIndex,Light_ModelFlag[pmsgParam->elementIndex].LightTransitionFlag,
                                       Light_ModelFlag[pmsgParam->elementIndex].Light_Trnsn_Cmplt);
#endif    
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslSaturation_Status
* @param  pHslSaturation_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslSaturation_Status(MOBLEUINT8* pHslSaturation_status, 
                                        MOBLEUINT32 *pLength,\
                                        MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.14 Light Hsl Saturation Status 
  Present HSL Saturation         2B The Present value of the Light HSL Hue state.
  Target HSL Saturation          2B The target value of the Light HSL Hue state.
  Remaining Time          1B The remaining time value for transition of state.
  */
  
  TRACE_M(TF_LIGHT_M,"Light_HslSaturation_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8272! \n\r");
  
  *(pHslSaturation_status) = Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16;
  *(pHslSaturation_status+1) = Light_HslStatus[pmsgParam->elementIndex].PresentHslSaturation16 >> 8;
  
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {          
    *(pHslSaturation_status+2) = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16;
    *(pHslSaturation_status+3) = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16 >> 8;
    *(pHslSaturation_status+4) = Light_HslStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 5;
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0;         
  }
  else
  {            
    *pLength = 2;         
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslDefault_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pHslDefault_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslDefault_Set(const MOBLEUINT8* pHslDefault_param,
                                  MOBLEUINT32 length,\
                                  MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.16 Light Hsl Saturation Set  
  HSL Lightness           2B The target value of the Light HSL Lightness state.
  HSL Hue                 2B The target value of the Light HSL Hue state.
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  */
  MOBLEUINT8 OptionalValid = 0;
  
  TRACE_M(TF_LIGHT_M,"Light_HslDefault_Set callback received \r\n"); 
  
  Light_HslDefaultParam[pmsgParam->elementIndex].HslLightnessDefault16= (MOBLEUINT16)pHslDefault_param[1] << 8;
  Light_HslDefaultParam[pmsgParam->elementIndex].HslLightnessDefault16 |= (MOBLEUINT16)pHslDefault_param[0];
  
  Light_HslDefaultParam[pmsgParam->elementIndex].HslHueDefault16 = (MOBLEUINT16)pHslDefault_param[3] << 8;
  Light_HslDefaultParam[pmsgParam->elementIndex].HslHueDefault16 |= (MOBLEUINT16)pHslDefault_param[2];
  
  Light_HslDefaultParam[pmsgParam->elementIndex].HslSaturationDefault16 = (MOBLEUINT16)pHslDefault_param[5] << 8;
  Light_HslDefaultParam[pmsgParam->elementIndex].HslSaturationDefault16 |= (MOBLEUINT16)pHslDefault_param[4];
  
  /* Application Callback */
  (LightAppli_cb.Light_HslDefault_Set_cb)(&Light_HslDefaultParam[pmsgParam->elementIndex] , OptionalValid,
                                          pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslDefault_Status
* @param  pHslDefault_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslDefault_Status(MOBLEUINT8* pHslDefault_status,
                                     MOBLEUINT32 *pLength,\
                                     MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.18 Light Hsl Default Status 
  HSL Lightness           2B The target value of the Light HSL Lightness state.
  HSL Hue                 2B The target value of the Light HSL Hue state.
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  */
  
  TRACE_M(TF_LIGHT_M,"Light_HslDefault_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#827B! \n\r");
  
  *pHslDefault_status = Light_HslDefaultParam[pmsgParam->elementIndex].HslLightnessDefault16;
  *(pHslDefault_status+1) = Light_HslDefaultParam[pmsgParam->elementIndex].HslLightnessDefault16 >> 8;
  *(pHslDefault_status+2) = Light_HslDefaultParam[pmsgParam->elementIndex].HslHueDefault16;
  *(pHslDefault_status+3) = Light_HslDefaultParam[pmsgParam->elementIndex].HslHueDefault16 >> 8;
  *(pHslDefault_status+4) = Light_HslDefaultParam[pmsgParam->elementIndex].HslSaturationDefault16;
  *(pHslDefault_status+5) = Light_HslDefaultParam[pmsgParam->elementIndex].HslSaturationDefault16 >> 8;
  
  *pLength = 6; 
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslRange_Set: This function is called for both Acknowledged and 
* unacknowledged message. The Acknowledgement is taken care by the Library
* @param  pHslRange_param : Pointer to the parameters received for message
* @param  length : Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslRange_Set(const MOBLEUINT8* pHslRange_param,
                                MOBLEUINT32 length,\
                                MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.20 Light Hsl Range Set  
  Hue Range Min          The value of the Hue Range Min field of the Light HSL Hue Range state
  Hue Range Max          The value of the Hue Range Max field of the Light HSL Hue Range state
  Saturation Range Min   The value of the Saturation Range Min field of the Light HSL Saturation Range state
  Saturation Range Max   The value of the Saturation Range Max field of the Light HSL Saturation Range state
  */
  MOBLEUINT8 OptionalValid = 0;
  TRACE_M(TF_LIGHT_M,"Light_HslRange_Set callback received \r\n");
  
  Light_HslRangeParam[pmsgParam->elementIndex].HslHueMinRange16 = (MOBLEUINT16)pHslRange_param[1] << 8;
  Light_HslRangeParam[pmsgParam->elementIndex].HslHueMinRange16 |= (MOBLEUINT16)pHslRange_param[0];
  
  Light_HslRangeParam[pmsgParam->elementIndex].HslHueMaxRange16 = (MOBLEUINT16)pHslRange_param[3] << 8;
  Light_HslRangeParam[pmsgParam->elementIndex].HslHueMaxRange16 |= (MOBLEUINT16)pHslRange_param[2];
  
  Light_HslRangeParam[pmsgParam->elementIndex].HslMinSaturation16 = (MOBLEUINT16)pHslRange_param[5] << 8;
  Light_HslRangeParam[pmsgParam->elementIndex].HslMinSaturation16 |= (MOBLEUINT16)pHslRange_param[4];
  
  Light_HslRangeParam[pmsgParam->elementIndex].HslMaxSaturation16 = (MOBLEUINT16)pHslRange_param[7] << 8;
  Light_HslRangeParam[pmsgParam->elementIndex].HslMaxSaturation16 |= (MOBLEUINT16)pHslRange_param[6];
  
  /* Application Callback */
  (LightAppli_cb.Light_HslRange_Set_cb)(&Light_HslRangeParam[pmsgParam->elementIndex] , OptionalValid,pmsgParam->dst_peer,pmsgParam->elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslRange_Status
* @param  pHslRange_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslRange_Status(MOBLEUINT8* pHslRange_status, 
                                   MOBLEUINT32 *pLength,\
                                   MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.22 Light Hsl Range Set 
  Hue Range Min          The value of the Hue Range Min field of the Light HSL Hue Range state
  Hue Range Max          The value of the Hue Range Max field of the Light HSL Hue Range state
  Saturation Range Min   The value of the Saturation Range Min field of the Light HSL Saturation Range state
  Saturation Range Max   The value of the Saturation Range Max field of the Light HSL Saturation Range state
  */   
  
  TRACE_M(TF_LIGHT_M,"Light_HslRange_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#827D! \n\r");
  
  /* Function call back to get the values from application*/   
  *pHslRange_status = Light_HslRangeParam[pmsgParam->elementIndex].Status_Code;  
  *(pHslRange_status+1) = Light_HslRangeParam[pmsgParam->elementIndex].HslHueMinRange16;
  *(pHslRange_status+2) = Light_HslRangeParam[pmsgParam->elementIndex].HslHueMinRange16 >> 8;
  *(pHslRange_status+3) = Light_HslRangeParam[pmsgParam->elementIndex].HslHueMaxRange16;
  *(pHslRange_status+4) = Light_HslRangeParam[pmsgParam->elementIndex].HslHueMaxRange16 >> 8;
  *(pHslRange_status+5) = Light_HslRangeParam[pmsgParam->elementIndex].HslMinSaturation16;
  *(pHslRange_status+6) = Light_HslRangeParam[pmsgParam->elementIndex].HslMinSaturation16 >> 8;
  *(pHslRange_status+7) = Light_HslRangeParam[pmsgParam->elementIndex].HslMaxSaturation16;
  *(pHslRange_status+8) = Light_HslRangeParam[pmsgParam->elementIndex].HslMaxSaturation16 >> 8;
  
  *pLength = 9;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Light_HslTarget_Status
* @param  pHslTarget_status: Pointer to the status message, which needs to be updated
* @param  pLength: Pointer to the Length of the Status message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_HslTarget_Status(MOBLEUINT8* pHslTarget_status, 
                                    MOBLEUINT32 *pLength,\
                                    MODEL_MessageHeader_t *pmsgParam)
{
  /* 6.3.3.5 Light HSL Target Get
  HSL Lightness           2B The target value of the Light HSL Lightness state.
  HSL Hue                 2B The target value of the Light HSL Hue state.
  HSL Saturation          2B The target value of the Light HSL Saturation state.
  */
  
  TRACE_M(TF_LIGHT_M,"Light_HslTarget_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8279! \n\r");
  /* Function call back to get the values from application*/
  
  *(pHslTarget_status) = Light_HslStatus[pmsgParam->elementIndex].TargetHslLightness16 ; 
  *(pHslTarget_status+1) = Light_HslStatus[pmsgParam->elementIndex].TargetHslLightness16 >> 8;
  *(pHslTarget_status+2) = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16;
  *(pHslTarget_status+3) = Light_HslStatus[pmsgParam->elementIndex].TargetHslHueLightness16 >> 8;
  *(pHslTarget_status+4) = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16;
  *(pHslTarget_status+5) = Light_HslStatus[pmsgParam->elementIndex].TargetHslSaturation16 >> 8; 
  
  if((Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam == 1) || (Light_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {          
    *(pHslTarget_status+6) = Light_HslStatus[pmsgParam->elementIndex].RemainingTime;
    *pLength = 7;
    Light_ModelFlag[pmsgParam->elementIndex].LightOptionalParam = 0;     
  }
  else
  {            
    *pLength = 6;         
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  LightModelServer_GetLightOpcodeTableCb: This function is call-back 
from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Light Model opcode array 
* @param  length: Pointer to the Length of Light Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT LightModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                               MOBLEUINT16 *length)
{
  *data = Light_Opcodes_Table;
  *length = sizeof(Light_Opcodes_Table)/sizeof(Light_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  LightModelServer_GetStatusRequestCb : This function is call-back 
from the library to send response to the message from peer
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT LightModelServer_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)
{
  TRACE_M(TF_LIGHT_M,"response status enable \n\r");
  switch(opcode)
  {
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
    
  case LIGHT_LIGHTNESS_STATUS:
    {
      Light_Lightness_Status(pResponsedata, plength , pmsgParam);
      break;
    }    
  case LIGHT_LIGHTNESS_LINEAR_STATUS:
    {
      Light_LightnessLinear_Status(pResponsedata,plength, pmsgParam);
      break;
    }     
  case LIGHT_LIGHTNESS_LAST_STATUS:
    {
      Light_LightnessLast_Status(pResponsedata,plength, pmsgParam);
      break;
    }   
  case LIGHT_LIGHTNESS_DEFAULT_STATUS:
    {
      Light_LightnessDefault_Status(pResponsedata,plength, pmsgParam);
      break;
    }  
  case LIGHT_LIGHTNESS_RANGE_STATUS:
    {
      Light_LightnessRange_Status(pResponsedata,plength, pmsgParam);
      break;
    } 
#endif  
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL 
    
  case LIGHT_CTL_STATUS:
    {      
      Light_Ctl_Status(pResponsedata ,plength, pmsgParam);    
      break;
    }    
  case LIGHT_CTL_TEMPERATURE_RANGE_STATUS:
    {    
      Light_CtlTemperatureRange_Status(pResponsedata ,plength, pmsgParam);
      break;
    }
  case LIGHT_CTL_DEFAULT_STATUS:
    {
      Light_CtlDefault_Status(pResponsedata ,plength, pmsgParam);         
      break;
    } 
#endif
    
#if defined ENABLE_LIGHT_MODEL_SERVER_CTL || defined ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE 
    
  case LIGHT_CTL_TEMPERATURE_STATUS:    
    {
      Light_CtlTemperature_Status(pResponsedata ,plength, pmsgParam);
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL    
  case LIGHT_HSL_STATUS:
    {
      Light_Hsl_Status(pResponsedata ,plength, pmsgParam);         
      break;
    } 
  case LIGHT_HSL_DEFAULT_STATUS:
    {
      Light_HslDefault_Status(pResponsedata ,plength, pmsgParam);         
      break;
    }    
  case LIGHT_HSL_RANGE_STATUS:
    {
      Light_HslRange_Status(pResponsedata ,plength, pmsgParam);         
      break;
    } 
  case LIGHT_HSL_TARGET_STATUS:
    {
      Light_HslTarget_Status(pResponsedata ,plength, pmsgParam);
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE    
  case LIGHT_HSL_HUE_STATUS:
    {
      Light_HslHue_Status(pResponsedata ,plength, pmsgParam);          
      break;
    } 
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION    
  case LIGHT_HSL_SATURATION_STATUS:
    {
      Light_HslSaturation_Status(pResponsedata ,plength, pmsgParam);        
      break;
    } 
#endif
  default:
    {
      break;
    }
  }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  LightModelServer_ProcessMessageCb: This is a callback function from
*         the library whenever a Light Model message is received
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message 
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT LightModelServer_ProcessMessageCb(MODEL_MessageHeader_t* pmsgParam, 
                                               MOBLEUINT16 opcode, 
                                               MOBLEUINT8 const *pRxData, 
                                               MOBLEUINT32 dataLength, 
                                               MOBLEBOOL response
                                                 )
{
  Model_Binding_Var.Dst_Peer = pmsgParam->dst_peer;	
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 modelStateChangeFlag = MOBLE_FALSE;
  
  TRACE_M(TF_LIGHT_M, "elementIndex %.2x dst_peer %.2X peer_add %.2X opcode %.2X response %.2X\r\n",
          pmsgParam->elementIndex, pmsgParam->dst_peer, pmsgParam->peer_addr, opcode, response);
  
  switch(opcode)
  {
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
    
  case LIGHT_LIGHTNESS_SET:
  case LIGHT_LIGHTNESS_SET_UNACK:
    {       
      result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE ,pRxData , MAX_VALID_RANGE );
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr, pmsgParam->dst_peer,pRxData[2])))
        {
            Light_Lightness_Set(pRxData, dataLength,pmsgParam);
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_LIGHTNESS_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE; 
            Publication1SecFlag.count = 0;
        }
      }
      break;
    }
  case LIGHT_LIGHTNESS_LINEAR_SET:
  case LIGHT_LIGHTNESS_LINEAR_SET_UNACK: 
    {
      result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE , pRxData , MAX_VALID_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr, 
                                                  pmsgParam->dst_peer,pRxData[2])))
        {
            Light_LightnessLinear_Set(pRxData, dataLength,pmsgParam);
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_LIGHTNESS_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE;
            Publication1SecFlag.count = 0;
        }
      }
      break;
    }

#endif 
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
    
  case LIGHT_LIGHTNESS_DEFAULT_SET:
  case LIGHT_LIGHTNESS_DEFAULT_SET_UNACK:
    {      
      result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE , pRxData , MAX_VALID_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          Light_LightnessDefault_Set(pRxData, dataLength, pmsgParam);       
      }
      break;
    }
  case LIGHT_LIGHTNESS_RANGE_SET:
  case LIGHT_LIGHTNESS_RANGE_SET_UNACK:
    {
      result = Chk_RangeValidity(MIN_VALID_RANGE , pRxData , MAX_VALID_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          Light_LightnessRange_Set(pRxData, dataLength,pmsgParam);     
      }
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
    
  case LIGHT_CTL_SET:
  case LIGHT_CTL_SET_UNACK:
    {       
      result = Chk_TwoParamValidity(MIN_VALID_RANGE ,MAX_VALID_RANGE ,MIN_CTL_TEMP_RANGE ,MAX_CTL_TEMP_RANGE ,pRxData);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,pmsgParam->dst_peer,pRxData[6])))
        {
            Light_Ctl_Set(pRxData, dataLength, pmsgParam); 
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_CTL_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE; 
            Publication1SecFlag.count = 0;
        }        
      }        
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE  
    
  case LIGHT_CTL_TEMPERATURE_SET:
  case LIGHT_CTL_TEMPERATURE_SET_UNACK:
    {
      result = Chk_ParamMinMaxValidity(MIN_CTL_TEMP_RANGE , pRxData , MAX_CTL_TEMP_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,pmsgParam->dst_peer,pRxData[4])))
        {
            Light_CtlTemperature_Set(pRxData, dataLength, pmsgParam);
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE;
            Publication1SecFlag.count = 0;
        }
      }
      break;
    }

#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
    
  case LIGHT_CTL_TEMPERATURE_RANGE_SET:
  case LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK:
    {
      result = Chk_RangeValidity(MIN_CTL_TEMP_RANGE , pRxData , MAX_CTL_TEMP_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          Light_CtlTemperatureRange_Set(pRxData, dataLength,pmsgParam);
      }
      break;
    }
  case LIGHT_CTL_DEFAULT_SET:
  case LIGHT_CTL_DEFAULT_SET_UNACK:
    { 
      result = Chk_MultiParamValidity(MIN_VALID_RANGE ,MAX_VALID_RANGE ,
                                                 MIN_CTL_TEMP_RANGE ,MAX_CTL_TEMP_RANGE ,
                                                  MIN_CTL_DELTA_VALUE ,MAX_CTL_DELTA_VALUE ,pRxData);
      
      if(result == MOBLE_RESULT_SUCCESS) 
      {
          Light_CtlDefault_Set(pRxData, dataLength,pmsgParam);       
      }
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
    
  case LIGHT_HSL_SET:                           
  case LIGHT_HSL_SET_UNACK:
    {
      result = Chk_MultiParamValidityAllUnsigned(MIN_VALID_RANGE ,MAX_VALID_RANGE ,
                                                    MIN_VALID_RANGE ,MAX_VALID_RANGE ,
                                                      MIN_VALID_RANGE ,MAX_VALID_RANGE ,pRxData);
      
      if(result == MOBLE_RESULT_SUCCESS) 
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,pmsgParam->dst_peer,pRxData[6])))
        {
            Light_Hsl_Set(pRxData, dataLength, pmsgParam);   
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_HSL_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE;
            Publication1SecFlag.count = 0;
        }
      }
      break;
    }

#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
    
  case LIGHT_HSL_HUE_SET:                       
  case LIGHT_HSL_HUE_SET_UNACK: 
    {
      result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE , pRxData , MAX_VALID_RANGE);
      
      if(result == MOBLE_RESULT_SUCCESS)
      {
          if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,pmsgParam->dst_peer,pRxData[2])))
        {
            Light_HslHue_Set(pRxData, dataLength, pmsgParam);  
            Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_HSL_HUE_SERVER_MODEL_ID;
            Model_Binding_Var.Model_Rx_Opcode = opcode;
            modelStateChangeFlag = MOBLE_TRUE;
            Publication1SecFlag.count = 0;
        }
      }
      break;
    }

#endif 
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
    
  case LIGHT_HSL_DEFAULT_SET:                   
  case LIGHT_HSL_DEFAULT_SET_UNACK: 
    {       
        Light_HslDefault_Set(pRxData, dataLength, pmsgParam);     
      break;
    }
  case LIGHT_HSL_RANGE_SET:                     
  case LIGHT_HSL_RANGE_SET_UNACK:
    {
      result = Chk_HslRangeValidity(pRxData ,MIN_HUE_RANGE,MAX_HUE_RANGE,
                                  MIN_SATURATION_RANGE,MAX_SATURATION_RANGE);
      if(result == MOBLE_RESULT_SUCCESS)
      {
          Light_HslRange_Set(pRxData , dataLength,pmsgParam);
      }
      break;
    }
#endif
    
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
    
  case LIGHT_HSL_SATURATION_SET:                     
  case LIGHT_HSL_SATURATION_SET_UNACK:
    {
        if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,pmsgParam->dst_peer,pRxData[2])))
      {
          Light_HslSaturation_Set(pRxData, dataLength, pmsgParam); 
          Model_Binding_Var.Model_ID = (MOBLEUINT16)LIGHT_HSL_SATURATION_SERVER_MODEL_ID;
          Model_Binding_Var.Model_Rx_Opcode = opcode;
          modelStateChangeFlag = MOBLE_TRUE;
          Publication1SecFlag.count = 0;
      }
      break;
    }

#endif  
  default:
    {
      break;
    }    
  } /* Switch ends */
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    if(ADDRESS_IS_UNICAST(pmsgParam->dst_peer))
    {
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);                                           
    }
    else{

      pmsgParam->dst_peer = BLEMesh_GetAddress();       // Replace group address by the single node address for response
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);   
    }
  }
  
  /*
     Publish the status to publish address if the publication is set by client 
     publication is independent of the response of the message.if the condition 
     for publication is full filled as per specification then the status will be 
     published.
  */
  publishAddress = BLEMesh_GetPublishAddress(pmsgParam->elementIndex, Model_Binding_Var.Model_ID);
  
  if((result == MOBLE_RESULT_SUCCESS) && (publishAddress != 0x0000) && (modelStateChangeFlag == MOBLE_TRUE))
  {
    if(ADDRESS_IS_UNICAST(pmsgParam->dst_peer))
    {
      pmsgParam->peer_addr = publishAddress;
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
    else{

      pmsgParam->peer_addr = publishAddress;
      pmsgParam->dst_peer = BLEMesh_GetAddress(); // Replace group address by the single node address for response
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
    
    modelStateChangeFlag = MOBLE_FALSE;  
    TRACE_I(TF_LIGHT_M,"Publishing state to the address  %.2X \r\n",publishAddress);
  }

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Light_TransitionBehaviourSingle_Param function is used for the Light Lightness model
*         when transition time is  received in message.This function is used for 
*         single parameter transition.
* @param GetValue: Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Light_TransitionBehaviourSingle_Param(MOBLEUINT8 *GetValue,
                                                   MOBLEUINT8 elementIndex)
{
  
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  MOBLEUINT16 targetRange;
  MOBLEUINT16 targetSlot;
  
  if(Clockflag == 0)
  {
    Check_time = Clock_Time();
    Clockflag = 1;
  }
  
  /* Publication after 1 second if total transition time is greater than 2 sec.*/
  if(Light_TimeParam[elementIndex].TotalTime >2000)
  {
    Punblication_OneSecTimer();
  }
  
  /* Values from application are copied into temporary variables for processing */    
  Light_TemporaryStatus[elementIndex].PresentParam_1 = GetValue[1] << 8;
  Light_TemporaryStatus[elementIndex].PresentParam_1 |= GetValue[0];   
  /*if condition to wait until the time is equal to the given resolution time */  
  if(((Clock_Time()- Check_time) >= Light_TimeParam[elementIndex].Res_Value))
  {
    if(Light_TimeParam[elementIndex].StepValue == 0)
    {
      Light_TimeParam[elementIndex].StepValue = 1;
    }
    
    if(Light_TemporaryStatus[elementIndex].TargetParam_1 > Light_TemporaryStatus[elementIndex].PresentParam_1)
    {
      /* calculating the target range to be achieved which is target value
      which is target value minus present value if target value is greater 
      than present value.   
      */
      targetRange = Light_TemporaryStatus[elementIndex].TargetParam_1 - Light_TemporaryStatus[elementIndex].PresentParam_1;  
      /*target slot = time to cover in single step */
      targetSlot = targetRange/Light_TimeParam[elementIndex].StepValue;
      /* target slot added to present value to achieve target value */
      Light_TemporaryStatus[elementIndex].PresentParam_1 += targetSlot;             
    }              
    else
    { 
      /* if present value is greater than target value, this condition executes */
      targetRange = Light_TemporaryStatus[elementIndex].PresentParam_1 - Light_TemporaryStatus[elementIndex].TargetParam_1;
      targetSlot = targetRange/Light_TimeParam[elementIndex].StepValue;          
      Light_TemporaryStatus[elementIndex].PresentParam_1 -= targetSlot;
    } 
    
    Light_TimeParam[elementIndex].StepValue=Light_TimeParam[elementIndex].StepValue-1;         
    /* updating the remaining time after each step covered*/
    Light_TemporaryStatus[elementIndex].RemainingTime =  Light_TimeParam[elementIndex].StepValue  | (Light_TimeParam[elementIndex].ResBitValue << 6) ;
    LightUpdateFlag = VALUE_UPDATE_SET;
    Check_time = 0;
    Clockflag = 0;
    
    /* when transition is completed, disable the transition by disabling 
    transition flag
    */
    if(Light_TimeParam[elementIndex].StepValue == 0)
    {              
      Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
      Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt = 1;
     
    }
    
 TRACE_M(TF_LIGHT_M, "Inside light lightness transition time at %ld, Current state 0x%.2x ,target state 0x%.2x , Remaining Time 0x%.2x\n\r",
            Clock_Time(),Light_TemporaryStatus[elementIndex].PresentParam_1,Light_TemporaryStatus[elementIndex].TargetParam_1,\
                                                                Light_TemporaryStatus[elementIndex].RemainingTime);
    
    
  }
  
  return MOBLE_RESULT_SUCCESS;         
} 


/**
* @brief Light_TransitionBehaviourMulti_Param function is used for the Light Lightness model
*         when transition time is  received in message.This function is used for
*         the multiple parameters in transition.
* @param GetValue: Pointer of the array
* @param 
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Light_TransitionBehaviourMulti_Param(MOBLEUINT8 *GetValue , 
                                                  MOBLEUINT8 param_Count,
                                                  MOBLEUINT8 elementIndex)
{
  
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  MOBLEUINT16 targetRangeLightness;
  MOBLEUINT16 targetRangeTemperature;
  MOBLEUINT16 targetSlotParam_1;
  MOBLEUINT16 targetSlotParam_2;
  MOBLEUINT16 targetSlotParam_3;
  
  if(Clockflag == 0)
  {
    Check_time = Clock_Time();
    Clockflag = 1;
  }
  
  /* Publication after 1 second if total transition time is greater than 2 sec.*/
  if(Light_TimeParam[elementIndex].TotalTime >2000)
  {
    Punblication_OneSecTimer();
  }
  
  /* Values from application are copied into Temporary variables for processing */
  Light_TemporaryStatus[elementIndex].PresentParam_1 = GetValue[1] << 8;
  Light_TemporaryStatus[elementIndex].PresentParam_1 |= GetValue[0];
  Light_TemporaryStatus[elementIndex].PresentParam_2 = GetValue[3] << 8;
  Light_TemporaryStatus[elementIndex].PresentParam_2 |= GetValue[2];
  Light_TemporaryStatus[elementIndex].PresentParam_3 = GetValue[5] << 8;
  Light_TemporaryStatus[elementIndex].PresentParam_3 |= GetValue[4];
  /* if condition to wait until the time is equal to the given resolution time */   
  if(((Clock_Time()- Check_time) >= Light_TimeParam[elementIndex].Res_Value))
  {
    if(Light_TimeParam[elementIndex].StepValue == 0)
    {
      Light_TimeParam[elementIndex].StepValue = 1;
    }
    
    if(Light_TemporaryStatus[elementIndex].TargetParam_1 > Light_TemporaryStatus[elementIndex].PresentParam_1)
    {
      /* calculating the target range to be achieved which is target value
      which is target value minus present value if target value is greater 
      than present value.   
      */
      targetRangeLightness =  Light_TemporaryStatus[elementIndex].TargetParam_1 - Light_TemporaryStatus[elementIndex].PresentParam_1;            
      /*target slot = time to cover in single step */
      targetSlotParam_1 = targetRangeLightness/Light_TimeParam[elementIndex].StepValue;
      /* target slot added to present value to achieve target value */
      Light_TemporaryStatus[elementIndex].PresentParam_1 += targetSlotParam_1;             
    }              
    else
    { 
      /* condition execute when transition is negative */
      /* target range = total range to be covered */ 
      targetRangeLightness = Light_TemporaryStatus[elementIndex].PresentParam_1 - Light_TemporaryStatus[elementIndex].TargetParam_1; 
      /*target slot = time to cover in single step */
      targetSlotParam_1 = targetRangeLightness/Light_TimeParam[elementIndex].StepValue;
      /*target slot = time to cover in single step */
      Light_TemporaryStatus[elementIndex].PresentParam_1 -= targetSlotParam_1;
    } 
    
    if(Light_TemporaryStatus[elementIndex].TargetParam_2 > Light_TemporaryStatus[elementIndex].PresentParam_2 )
    {
      targetRangeTemperature = Light_TemporaryStatus[elementIndex].TargetParam_2 - Light_TemporaryStatus[elementIndex].PresentParam_2;
      targetSlotParam_2 = targetRangeTemperature/Light_TimeParam[elementIndex].StepValue; 
      Light_TemporaryStatus[elementIndex].PresentParam_2 += targetSlotParam_2;
    }
    else
    {
      targetRangeTemperature = Light_TemporaryStatus[elementIndex].PresentParam_2 - Light_TemporaryStatus[elementIndex].TargetParam_2;
      targetSlotParam_2 = targetRangeTemperature/Light_TimeParam[elementIndex].StepValue; 
      Light_TemporaryStatus[elementIndex].PresentParam_2 -= targetSlotParam_2;
    }
    
    if(param_Count == 3)
    {
      if(Light_TemporaryStatus[elementIndex].TargetParam_3 > Light_TemporaryStatus[elementIndex].PresentParam_3 )
      {
        targetRangeTemperature = Light_TemporaryStatus[elementIndex].TargetParam_3 - Light_TemporaryStatus[elementIndex].PresentParam_3;
        targetSlotParam_3 = targetRangeTemperature/Light_TimeParam[elementIndex].StepValue; 
        Light_TemporaryStatus[elementIndex].PresentParam_3 += targetSlotParam_3;
      }
      else
      {
        targetRangeTemperature = Light_TemporaryStatus[elementIndex].PresentParam_3 - Light_TemporaryStatus[elementIndex].TargetParam_3;
        targetSlotParam_3 = targetRangeTemperature/Light_TimeParam[elementIndex].StepValue; 
        Light_TemporaryStatus[elementIndex].PresentParam_3 -= targetSlotParam_3;
      }
    }
    
    Light_TimeParam[elementIndex].StepValue--;                           
    /* updating the remaining time after each step covered*/
    Light_TemporaryStatus[elementIndex].RemainingTime = Light_TimeParam[elementIndex].StepValue | (Light_TimeParam[elementIndex].ResBitValue << 6) ;
    LightUpdateFlag = VALUE_UPDATE_SET;
    Check_time = 0;
    Clockflag = 0;
    /* when transition is completed, disable the transition by disabling 
    transition flag
    */
    
    if(Light_TimeParam[elementIndex].StepValue <= 0)
    {
      Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;  
      Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt = 1;      
    }   
    TRACE_M(TF_LIGHT_M, "Inside light transition %ld,PV1 0x%.2x ,TV1 0x%.2x ,PV2 0x%.2x , TV2 0x%.2x,PV3 0x%.2x ,TV3 0x%.2x, RT 0x%.2x \n\r",
            Clock_Time(),Light_TemporaryStatus[elementIndex].PresentParam_1,Light_TemporaryStatus[elementIndex].TargetParam_1, 
                Light_TemporaryStatus[elementIndex].PresentParam_2,Light_TemporaryStatus[elementIndex].TargetParam_2,
                 Light_TemporaryStatus[elementIndex].PresentParam_3, Light_TemporaryStatus[elementIndex].TargetParam_3,
            Light_TemporaryStatus[elementIndex].RemainingTime);    
    
    
  }
  return MOBLE_RESULT_SUCCESS;         
} 

/**
* @brief  Function to publish the status of the node to the publish address.
*         The Publishing of all the model states will depend on the model message received 
          and supporting binding with other models.
* @param  void
* @param  void
*/ 
void Model_BindingPublishStatus(void)
{
  MOBLE_ADDRESS publishAddress;
  static MOBLEUINT8 elementIndex;
  MODEL_MessageHeader_t msgParam;
  MOBLEUINT16 opcode;
  MOBLEUINT8 const pRxData[12] = {0};
  MOBLEUINT32 dataLength = 0;
  MOBLE_ADDRESS my_Address;

  if((Light_PublishOpcodeList[elementIndex].BindedStateCount > 0) &&(ADDRESS_IS_UNICAST(Model_Binding_Var.Dst_Peer))) 
  { 
    if((Wait_RandomTime() == MOBLE_TRUE)) 
    {   
      my_Address = BLEMesh_GetAddress(); 
      publishAddress = BLEMesh_GetPublishAddress(elementIndex, Light_PublishOpcodeList[elementIndex].Model_ID[Publication1SecFlag.count]);
      opcode = (MOBLEUINT16)Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Publication1SecFlag.count];

      /*Initialize the Parameters*/
      msgParam.dst_peer = my_Address;
      msgParam.peer_addr = publishAddress;
      msgParam.elementIndex = elementIndex;
      msgParam.rcvdAppKeyOffset = 0;
      msgParam.rcvdNetKeyOffset = 0;
      msgParam.rssi = 0;
      msgParam.ttl = 0;
        
      if(publishAddress != 0x0000) 
      {
        /*Using the same API used for Acknowledgement, where src & dst getting interchanged.
        Therefore PublishAddress is copied in peer_addr & vice-versa*/  
        Model_SendResponse(&msgParam, opcode, pRxData, dataLength);
        TRACE_M(TF_LIGHT_M, "Binded publishing address %.2x opcode %.2x model id %.2x \r\n",
                publishAddress,Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Publication1SecFlag.count],
                Light_PublishOpcodeList[elementIndex].Model_ID[Publication1SecFlag.count]);
      }     
       
      Publication1SecFlag.count++;
      if(Publication1SecFlag.count > Light_PublishOpcodeList[elementIndex].BindedStateCount)
      {
        Publication1SecFlag.count = 0;
        Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
        Publication1SecFlag.Transition1SecFlag = MOBLE_FALSE;
        elementIndex++;
        if(elementIndex == APPLICATION_NUMBER_OF_ELEMENTS)
        {
          elementIndex = 0;
        }       
      }
    }
  }
  else
  {
    Publication1SecFlag.count = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    if((Generic_ModelFlag[elementIndex].GenericTransitionFlag == MODEL_TRANSITION_STOP) &&
       (Light_ModelFlag[elementIndex].LightTransitionFlag == MODEL_TRANSITION_STOP))
    {
      for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
      {
        Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
      }		
    }  
    elementIndex++;
    if(elementIndex == APPLICATION_NUMBER_OF_ELEMENTS)
    {
      elementIndex = 0;
    }
  }		
}

/**
* @brief  Light_GetStepValue:Function calculates parameters for transition time 
*         for light model.
* @param stepParam is transition time set value.
* retval void.
*/
void Light_GetStepValue(MOBLEUINT8 stepParam,MOBLEUINT8 elementIndex)
{
  /*
  Two MSB bit of transition time is dedicated to resolution.
  00 = resolution is 100 ms.
  01 = resolution is 1000 ms. 
  10 = resolution is 10000 ms.
  11 = resolution is 600000 ms. 
  Last bits from 0 to 5th index is step number.
  */
  
  Light_TimeParam[elementIndex].ResBitValue = stepParam >> 6 ;
  Light_TimeParam[elementIndex].Res_Value = Get_StepResolutionValue(Light_TimeParam[elementIndex].ResBitValue);
  Light_TimeParam[elementIndex].StepValue = stepParam & 0x3F ;
  
  Light_TimeParam[elementIndex].TotalTime = (Light_TimeParam[elementIndex].StepValue * Light_TimeParam[elementIndex].Res_Value);
  
  if(Light_TimeParam[elementIndex].Res_Value >= 100)
  {
    Light_TimeParam[elementIndex].Res_Value = Light_TimeParam[elementIndex].Res_Value/TRANSITION_SCALER;
    Light_TimeParam[elementIndex].StepValue = (Light_TimeParam[elementIndex].StepValue * TRANSITION_SCALER);
  }
  
  TRACE_M(TF_LIGHT_M, "step resolution 0x%.2lx, number of step 0x%.2x \r\n",Light_TimeParam[elementIndex].Res_Value ,
          Light_TimeParam[elementIndex].StepValue  );   
}


/**
* @brief  Function to execute the transition state machine for particular Light Model
* @param  void
* @retval void
*/ 
void Lighting_Process(void)
{ 
  MOBLE_ADDRESS publishAddress;
  MOBLE_ADDRESS my_Address = BLEMesh_GetAddress();
  MODEL_MessageHeader_t msgParam;
  MOBLEUINT8 elementIndex;
  MOBLEUINT8 const pRxData[12] = {0};
  MOBLEUINT32 dataLength = 0;
  
  for(elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++)
  {
    my_Address = my_Address+elementIndex;
    
#if defined ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS || defined ENABLE_LIGHT_MODEL_SERVER_CTL \
  || defined ENABLE_LIGHT_MODEL_SERVER_HSL
    
    MOBLEUINT8 Light_GetBuff[8];
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_LIGHTNESS_TRANSITION_START)
  {  
     /* Fetching the state value from the application layer to middle layer */
    (Appli_Light_GetStatus_cb.GetLightLightness_cb)(Light_GetBuff, my_Address, elementIndex);
    /* Transition function called to change the state in transition */
    Light_TransitionBehaviourSingle_Param(Light_GetBuff, elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      /* updating the state value after the each transition step complete */
      Light_LightnessStateUpdate_Process(elementIndex);
      /*Application Callback */
      (LightAppli_cb.Lightness_Set_cb)(&Light_LightnessStatus[elementIndex],0, my_Address, elementIndex); 
      /* Binding of actual light lightness with other models */
      Light_Lightness_Binding(BINDING_LIGHT_LIGHTNESS_ACTUAL_SET , 0,elementIndex,
                              Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }  
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_LINEAR_TRANSITION_START)
  {
    /* Fetching the state value from the application layer to middle layer */
    (Appli_Light_GetStatus_cb.GetLightLightnessLinear_cb)(Light_GetBuff,my_Address, elementIndex);
    /* Transition function called to change the state in transition */
    Light_TransitionBehaviourSingle_Param(Light_GetBuff,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      /* updating the state value after the each transition step complete */
      Light_LinearStateUpdate_Process(elementIndex);
      /*Application Callback */
      (LightAppli_cb.Lightness_Linear_Set_cb)(&Light_LightnessStatus[elementIndex], 0, my_Address, elementIndex);
      /* Binding of data of light lightness with light linear */
      Light_Lightness_Binding(BINDING_LIGHT_LIGHTNESS_LINEAR_SET , 0,elementIndex,
                              Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL   
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_CTL_TRANSITION_START)
  {
    (Appli_Light_GetStatus_cb.GetLightCtl_cb)(Light_GetBuff,my_Address, elementIndex);
    Light_TransitionBehaviourMulti_Param(Light_GetBuff , 0,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      Light_CtlStateUpdate_Process(elementIndex);
      (LightAppli_cb.Light_Ctl_Set_cb)(&Light_CtlStatus[elementIndex], 0, my_Address, elementIndex);
      /* binding of light ctl with actual lightness */
      Light_Ctl_LightActual_Binding(BINDING_LIGHT_CTL_SET,elementIndex,
                    Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);                                 
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
  
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_TEMPERATURE_TRANSITION_START)
  {
    (Appli_Light_GetStatus_cb.GetLightCtlTemp_cb)(Light_GetBuff, my_Address, elementIndex);
    Light_TransitionBehaviourMulti_Param(Light_GetBuff , 0,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      Light_CtlTemperatureStateUpdate_Process(elementIndex);
      (LightAppli_cb.Light_CtlTemperature_Set_cb)(&Light_CtlStatus[elementIndex], 0, my_Address, elementIndex);
      /* Binding of data Ctl Temperature  with Generic Level */
      Light_CtlTemperature_Binding(elementIndex,Light_ModelFlag[elementIndex].LightTransitionFlag,
                                      Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL   
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_HSL_TRANSITION_START)
  {
    (Appli_Light_GetStatus_cb.GetLightHsl_cb)(Light_GetBuff,my_Address, elementIndex);
    Light_TransitionBehaviourMulti_Param(Light_GetBuff , 3,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      Light_HslStateUpdate_Process(elementIndex);
      (LightAppli_cb.Light_Hsl_Set_cb)(&Light_HslStatus[elementIndex], 0, my_Address, elementIndex);
      /* Light Hsl Lightness binding with lightness Actual */
      Light_HslLightness_LightnessActualBinding(elementIndex,Light_ModelFlag[elementIndex].LightTransitionFlag,
                                                            Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
  
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_HSL_HUE_TRANSITION_START)
  {
    (Appli_Light_GetStatus_cb.GetLightHslHue_cb)(Light_GetBuff,my_Address, elementIndex);
    Light_TransitionBehaviourSingle_Param(Light_GetBuff,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      Light_HslHueStateUpdate_Process(elementIndex);
      (LightAppli_cb.Light_HslHue_Set_cb)(&Light_HslStatus[elementIndex], 0, my_Address, elementIndex);
      /* Light Hsl Hue  binding with Generic level */
      Light_Hsl_Hue_Binding(elementIndex,Light_ModelFlag[elementIndex].LightTransitionFlag,
                                     Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
  
  if(Light_ModelFlag[elementIndex].LightTransitionFlag == LIGHT_HSL_SATURATION_TRANSITION_START)
  {
    (Appli_Light_GetStatus_cb.GetLightHslSaturation_cb)(Light_GetBuff,my_Address, elementIndex);
    Light_TransitionBehaviourSingle_Param(Light_GetBuff,elementIndex);
    if(LightUpdateFlag == VALUE_UPDATE_SET)
    {
      Light_HslSaturationStateUpdate_Process(elementIndex);
      (LightAppli_cb.Light_HslSaturation_Set_cb)(&Light_HslStatus[elementIndex], 0, my_Address, elementIndex);
      /* Light Hsl Saturation binding with Generic level model*/
      Light_Hsl_Saturation_Binding(elementIndex,Light_ModelFlag[elementIndex].LightTransitionFlag,
                                   Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      LightUpdateFlag = VALUE_UPDATE_RESET;
    }
  }
#endif  
  /* Check if Transition is completed */
  if(Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt == MOBLE_TRUE)
  {         
    my_Address = BLEMesh_GetAddress();
    publishAddress = BLEMesh_GetPublishAddress(elementIndex, Model_Binding_Var.Model_ID);
    
    /* Initialize the messageParam*/
    msgParam.dst_peer = my_Address;
    msgParam.peer_addr = publishAddress;
    msgParam.elementIndex = elementIndex;
    msgParam.rcvdAppKeyOffset = 0;
    msgParam.rcvdNetKeyOffset = 0;
    msgParam.rssi = 0;
    msgParam.ttl = 0;
  
    if(publishAddress != 0x00)
    {
      /*Using the same API used for Acknowledgement, where src & dst getting interchanged.
        Therefore PublishAddress is copied in peer_addr & vice-versa*/ 
      Model_SendResponse(&msgParam, Model_Binding_Var.Model_Rx_Opcode, pRxData, dataLength);
      TRACE_M(TF_LIGHT_M, "Publishing the status after transition complete opcode %.2x model id %.2x \r\n",
       Model_Binding_Var.Model_Rx_Opcode,Model_Binding_Var.Model_ID);
    }
    Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt = MOBLE_FALSE;
  }
#ifdef  ENABLE_MODEL_BINDING 
    /* publish the status of the node to the publish address */
  Model_BindingPublishStatus();
#endif  
}
}


/**
* @brief Light_LightnessStateUpdate_Process:Function to update the parameters of light
*         lightness model in application file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_LightnessStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_LightnessStatus[elementIndex].LightnessPresentValue16 = Light_TemporaryStatus[elementIndex].PresentParam_1;
  Light_LightnessStatus[elementIndex].TargetValue16 = Light_TemporaryStatus[elementIndex].TargetParam_1;
  Light_LightnessStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime ;
	
  if((Light_LightnessStatus[elementIndex].LightnessPresentValue16 == Light_LightnessStatus[elementIndex].TargetValue16) && 
     (Light_LightnessStatus[elementIndex].LightnessPresentValue16 > 0))
  {
    Light_LightnessStatus[elementIndex].LightnessLastStatus = Light_LightnessStatus[elementIndex].LightnessPresentValue16;
		  
    (LightAppli_cb.Lightness_Last_Set_cb)(&Light_LightnessStatus[elementIndex],elementIndex);                                       
  }
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Light_ModelRangeInit: Function called to initialise the minimum and maximum range of model state.
* @param void
* return void.
*/
void Light_ModelRangeInit(void)
{
	
  for(uint8_t elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++)
  {
    Light_LightnessRangeParam[elementIndex].StatusCode = 0x00;
    Light_LightnessRangeParam[elementIndex].MinRangeStatus = MIN_VALID_RANGE;
    Light_LightnessRangeParam[elementIndex].MaxRangeStatus = MAX_VALID_RANGE;
			
    CtlTemperatureRangeParam[elementIndex].StatusCode = 0x00;
    CtlTemperatureRangeParam[elementIndex].MinRangeStatus = MIN_CTL_TEMP_RANGE;
    CtlTemperatureRangeParam[elementIndex].MaxRangeStatus = MAX_CTL_TEMP_RANGE;
			
    Light_HslRangeParam[elementIndex].Status_Code = 0x00;
    Light_HslRangeParam[elementIndex].HslHueMinRange16 = MIN_HUE_RANGE;
    Light_HslRangeParam[elementIndex].HslHueMaxRange16 = MAX_HUE_RANGE;
    Light_HslRangeParam[elementIndex].HslMinSaturation16 = MIN_SATURATION_RANGE;
    Light_HslRangeParam[elementIndex].HslMaxSaturation16 = MAX_SATURATION_RANGE;
			
    Light_ModelFlag[elementIndex].LightOptionalParam = 0x00;
    Light_ModelFlag[elementIndex].LightTransitionFlag = 0x0;
    Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt = 0x00;  
  }
}

/**
* @brief Light_LightnessActualUpdate updates lightness actual state
*        It can be called from LC power up sequence
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_LightnessActualUpdate(MOBLEUINT16 lightActual, MOBLEUINT8 elementIndex)
{
#ifdef ENABLE_MODEL_BINDING    
  /* Binding of actual light lightness with other models */
#endif
  return MOBLE_RESULT_SUCCESS;
}

/** 
  * @brief Update Light Lightness Linear
  *        Can be used to update binded light lightness linear state
  * @param
  * @retval
  */
void Light_LightnessLinearUpdate(MOBLEUINT8 elementIndex, MOBLEUINT16 linear)
{
  Light_LightnessStatus[elementIndex].LinearPresentValue16 = linear;
  Light_LightnessStatus[elementIndex].LightnessPresentValue16 = (MOBLEUINT16)(sqrt((double)linear * (double)65535));
  
  /* Status publish pending */
  
  if(LightAppli_cb.Lightness_Set_cb != NULL)
  {
    /* Application Callback */
    (LightAppli_cb.Lightness_Set_cb)(&Light_LightnessStatus[elementIndex], 0, 0, elementIndex);
  }
}


#if 0
/**
* @brief Light_LC_LightnessStateUpdate_Process:Function to update the parameters of light
*        lightness model in application file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_LC_LightnessStateUpdate_Process(Light_LC_TemporaryStatus_t *lc_Temp_Value, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex)
{
  Light_LightnessStatus[elementIndex].LightnessPresentValue16 = lc_Temp_Value->PresentParam_1;
  Light_LightnessStatus[elementIndex].TargetValue16 = lc_Temp_Value->TargetParam_1;
  Light_LightnessStatus[elementIndex].RemainingTime = lc_Temp_Value->RemainingTime;
  
  (LightAppli_cb.Lightness_Set_cb)(&Light_LightnessStatus[elementIndex], 0, dstPeer, elementIndex); 

  return MOBLE_RESULT_SUCCESS;
}
#endif


/**
* @brief LightLightness_ActualUpdate updates lightness actual state
*        It can be called from LC power up sequence
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT LightLightness_ActualUpdate(MOBLEUINT16 lightActual, MOBLEUINT8 elementIndex)
{
#ifdef ENABLE_MODEL_BINDING    
  /* Binding of actual light lightness with other models */
#endif
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_LinearStateUpdate_Process:function to update the parameters of light 
*        lightness linear model in application file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_LinearStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_LightnessStatus[elementIndex].LinearPresentValue16 = Light_TemporaryStatus[elementIndex].PresentParam_1;
  Light_LightnessStatus[elementIndex].TargetValue16 =  Light_TemporaryStatus[elementIndex].TargetParam_1;
  Light_LightnessStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime ;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_CtlStateUpdate_Process:Function to update the parameters of light  
*        CTL model in application file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_CtlStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_CtlStatus[elementIndex].PresentCtlLightness16 =Light_TemporaryStatus[elementIndex].PresentParam_1; 
  Light_CtlStatus[elementIndex].PresentCtlTemperature16 = Light_TemporaryStatus[elementIndex].PresentParam_2;
  Light_CtlStatus[elementIndex].PresentCtlDelta16 = Light_TemporaryStatus[elementIndex].PresentParam_3;
  Light_CtlStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_CtlTemperatureStateUpdate_Process:Function to update the parameters of 
*        light CTL Temperature model in application  file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_CtlTemperatureStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_CtlStatus[elementIndex].PresentCtlTemperature16 = Light_TemporaryStatus[elementIndex].PresentParam_1; 
  Light_CtlStatus[elementIndex].PresentCtlDelta16 = Light_TemporaryStatus[elementIndex].PresentParam_2;
  Light_CtlStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_HslStateUpdate_Process:Function to update the parameters of 
*        light HSL model in application  file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_HslStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_HslStatus[elementIndex].PresentHslLightness16= Light_TemporaryStatus[elementIndex].PresentParam_1; 
  Light_HslStatus[elementIndex].PresentHslHueLightness16 = Light_TemporaryStatus[elementIndex].PresentParam_2;
  Light_HslStatus[elementIndex].PresentHslSaturation16 = Light_TemporaryStatus[elementIndex].PresentParam_3; 
  Light_HslStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_HslHueStateUpdate_Process:Function to update the parameters of 
*        light HSL Hue model in application  file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_HslHueStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_HslStatus[elementIndex].PresentHslHueLightness16= Light_TemporaryStatus[elementIndex].PresentParam_1; 
  Light_HslStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_HslSaturationStateUpdate_Process:Function to update the parameters of 
*        light HSL Saturation model in application  file from Temporary parameter in model file.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT.
*/
MOBLE_RESULT Light_HslSaturationStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Light_HslStatus[elementIndex].PresentHslSaturation16 = Light_TemporaryStatus[elementIndex].PresentParam_1; 
  Light_HslStatus[elementIndex].RemainingTime = Light_TemporaryStatus[elementIndex].RemainingTime;
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief Light_Ctl_LightActual_Binding:Function for binding the data of actual 
*        lightness and light Ctl set.
* @param bindingFlag: value pass to function from light lightness
* @param elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Ctl_LightActual_Binding(MOBLEUINT8 bindingFlag, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{ 
#if defined ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS || defined ENABLE_LIGHT_MODEL_SERVER_CTL
  MOBLE_RESULT result;
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
  
  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag == BINDING_LIGHT_CTL_SET)
    {
      Light_LightnessStatus[elementIndex].LightnessPresentValue16 = Light_CtlParam.CTL_Lightness; 
    
      Light_Lightness_Binding(BINDING_LIGHT_CTL_SET ,0,elementIndex,
                              Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {  
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
      
      TRACE_M(TF_LIGHT_M, "Light CTL binding with Light Actual 0x%.2x \r\n", 
            Light_LightnessStatus[elementIndex].LightnessPresentValue16);
    }
    /* Ligtness Linear updated on its own (unsolicited change) */
    BindingDisable_LcLinearOut_LightLightnessLinear(elementIndex);
  }
#endif 
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL  
  
  if((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag == BINDING_LIGHT_LIGHTNESS_ACTUAL_SET)
    {
      Light_CtlStatus[elementIndex].PresentCtlLightness16 = Light_LightnessParam.TargetLightnessStatus;    
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_CTL_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {     
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_CTL_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_CTL_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++; 
      }
    
    TRACE_M(TF_LIGHT_M, "Light Actual binding with Light CTL 0x%.2x \r\n", Light_CtlStatus[elementIndex].PresentCtlLightness16);
  }
}
#endif 
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  }
}

/**
* @brief Light_Lightness_Binding:Function for binding the data of actual 
lightness and Linear Lightness is implicit binding with  generic on off state.              
* @param bindingFlag: flag value passed in function 
* @param length:length of data
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Lightness_Linear_Binding(MOBLEUINT8 bindingFlag ,MOBLEUINT32 length, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
  MOBLE_RESULT result;
  
  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag == BINDING_LIGHT_LIGHTNESS_LINEAR_SET)
    {
      Light_Linear_ActualBinding(length, elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {     
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;    
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
  Light_Lightness_Binding(bindingFlag , 0,elementIndex,
                          Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
#endif  
}

/**
* @brief Light_Lightness_Binding:Function for binding the data of actual 
*        lightness and Linear Lightness is implicit binding with  generic on off state.              
* @param bindingFlag: flag value passed in function 
* @param length:length of data
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Lightness_Binding(MOBLEUINT8 bindingFlag , MOBLEUINT32 length, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{ 
  /*
  6.1.2.1.1 - actual lightness = 655354 * squareroot(linear lightness/ 655354).
  */
#if defined (ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS) || defined (ENABLE_GENERIC_MODEL_SERVER_ONOFF)\
	  || defined (ENABLE_GENERIC_MODEL_SERVER_LEVEL) || defined (ENABLE_LIGHT_MODEL_SERVER_CTL)\
	   || defined (ENABLE_LIGHT_MODEL_SERVER_HSL)
  MOBLE_RESULT result;
#endif 	
 
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag != BINDING_LIGHT_LIGHTNESS_LINEAR_SET)
    {
      Light_Actual_LinearBinding(elementIndex); 
      TRACE_M(TF_LIGHT_M, "Light Lightness binding with Light Linear \r\n"); 
                  
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_LINEAR_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {            
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;    
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif	
    /* Implicit binding of linear lightness with generic on off.
    linear lightnes set -> actual lightness set -> generic on off set.
    As linear lightness changes, it changes the value of actual lightness
    which in turn changes the state of generic on off state.
    */
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF  
    /* Binding of data b/w light lightness actual and Generic on off */
  if((ENABLE_GENERIC_MODEL_SERVER_ONOFF & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag != BINDING_GENERIC_ON_OFF_SET)
    {
      LightActual_GenericOnOffBinding(&Light_LightnessStatus[elementIndex],elementIndex);
       
      result = Chk_OpcodePresent((MOBLEUINT16)GENERIC_ON_OFF_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      { 
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= GENERIC_ON_OFF_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = GENERIC_ONOFF_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif 
  
    /* Implicit binding of linear lightness with generic level.
    linear lightnes set -> actual lightness set -> generic Level set.
    As linear lightness changes, it changes the value of actual lightness
    which in turn changes the state of generic level state.
    */
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
    /* Binding of data b/w light lightness actual and Generic Level */
  if((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag != BINDING_GENERIC_LEVEL_SET)
    {
      LightActual_GenericLevelBinding(&Light_LightnessStatus[elementIndex],elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)GENERIC_LEVEL_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= GENERIC_LEVEL_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = GENERIC_LEVEL_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif   
     
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL  
  /* Binding of Light Ctl with Actual Light Lightness */
  if((ENABLE_LIGHT_MODEL_SERVER_CTL & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag != BINDING_LIGHT_CTL_SET)
    {
      Light_Ctl_LightActual_Binding(BINDING_LIGHT_LIGHTNESS_ACTUAL_SET,elementIndex,trnsnFlag,trnsnCmplt);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_CTL_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {      
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_CTL_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_CTL_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL  
  /* Binding of data b/W Actual Lightness and Hsl Lightness.*/
  if((ENABLE_LIGHT_MODEL_SERVER_HSL & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(bindingFlag != BINDING_LIGHT_HSL_SET)
    {
      Light_ActualLightness_HslLightnessBinding(&Light_LightnessStatus[elementIndex], elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_HSL_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_HSL_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_HSL_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif     
   
  /* Ligtness Linear updated on its own (unsolicited change) */
  BindingDisable_LcLinearOut_LightLightnessLinear(elementIndex);
  
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  }
}

/**
* @brief  GenericOnOff_LightActualBinding: Data binding b/w Generic On Off and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic on off set. 
* @param onOff_param: Pointer to the data which needs to be checked.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void GenericOnOff_LightActualBinding(Generic_OnOffStatus_t* onOff_param, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{
  
  /*
  6.1.2.2.3 - Binding of actual light lightness with Generic on off.
  As generic on off state changes, the actual lightness value will
  change.
  */
  /* condition is depends on the generic on off state */  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 	
  MOBLE_RESULT result;
  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  {       
    if(onOff_param->Present_OnOff_State == 0x00)
    {
      Light_LightnessStatus[elementIndex].LightnessPresentValue16 = 0x00;    
    }
    else if((onOff_param->Present_OnOff_State == 0x01) && 
            (Light_LightnessDefaultParam[elementIndex].LightnessDefaultStatus == 0x00))
    {
      Light_LightnessStatus[elementIndex].LightnessPresentValue16 = Light_LightnessStatus[elementIndex].LightnessLastStatus;
    }
    else if((onOff_param->Present_OnOff_State == 0x01) && 
            (Light_LightnessDefaultParam[elementIndex].LightnessDefaultStatus != 0x000))
    {
      Light_LightnessStatus[elementIndex].LightnessPresentValue16 = Light_LightnessDefaultParam[elementIndex].LightnessDefaultStatus;
    }
    else
    {
      /* no condition to Execute */
    }
    TRACE_M(TF_LIGHT_M, "Generic On Off binding with Light Actual \r\n"); 

    result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_SET_UNACK ,elementIndex); 
  
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    {
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  }
#endif
  
  /*As Generic on off changes, it will change the value of the light linear due to change in the light lightness 
    value due to binding of the states.
 */
   
  Light_Lightness_Binding(BINDING_GENERIC_ON_OFF_SET, 0, elementIndex,trnsnFlag,trnsnCmplt);
}

/*
* @Brief  GenericLevel_LightBinding: Data binding b/w Generic Level and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic Level set. 
* @param gLevel_param: Pointer to the data which needs to be checked.
* @param flag: flag for the binding
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void GenericLevel_LightBinding(Generic_LevelStatus_t* gLevel_param , MOBLEUINT8 flag, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{ 
#if defined (ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS) || (defined ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE) \
    	|| (defined ENABLE_LIGHT_MODEL_SERVER_HSL_HUE) || (defined ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION)
        	
    	
  MOBLE_RESULT result; 
#endif	
  /*
  6.1.2.2.2 - Binding of actual light lightness with generic level
  As generic Level changes, the actual lightness value will
  change.
  */
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS    

  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  {
    Light_LightnessStatus[elementIndex].LightnessPresentValue16 = gLevel_param->Present_Level16 + 32768;   
  
    result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_SET_UNACK ,elementIndex); 
    
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    {
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  
    /*As Generic Level changes, it will change the value of the light linear due to change in the light lightness 
      value due to binding of the states.
    */
    Light_Actual_LinearBinding(elementIndex);
  
    result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_LINEAR_SET_UNACK ,elementIndex); 
    
    if(result == MOBLE_RESULT_SUCCESS || (trnsnCmplt == MOBLE_TRUE))
    {
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  }
  
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
  if((ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(flag != BINDING_LIGHT_CTL_TEMP_SET)
    {
      GenericLevel_CtlTempBinding(gLevel_param,elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_CTL_TEMPERATURE_SET_UNACK ,elementIndex); 
      
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_CTL_TEMPERATURE_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE  
  if((ENABLE_LIGHT_MODEL_SERVER_HSL_HUE & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(flag != BINDING_LIGHT_HSL_HUE_SET)
    {
      GenericLevel_HslHueBinding(gLevel_param,elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_HSL_HUE_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_HSL_HUE_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_HSL_HUE_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION  
  if((ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION & (1 << elementIndex)) == (1 << elementIndex))
  {
    if(flag != BINDING_LIGHT_HSL_SATURATION_SET)
    {
      GenericLevel_HslSaturationBinding(gLevel_param,elementIndex);
      
      result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_HSL_SATURATION_SET_UNACK ,elementIndex); 
    
      if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
      {
        Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_HSL_SATURATION_SET_UNACK;
        Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_HSL_SATURATION_SERVER_MODEL_ID;
        Light_PublishOpcodeList[elementIndex].BindedStateCount++;
      }
    }
  }
#endif    
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  } 
}

/**
* @brief Light_Actual_LinearBinding:Light_Actual_LinearBinding:Function used for 
*        binding the data of actual lightness and lineaer lightness.this function
*        changes the value of linear lightness as actual lightness value is set.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLEUINT16
*/
MOBLEUINT16 Light_Actual_LinearBinding(MOBLEUINT8 elementIndex)
{
  float productValue;
  
  productValue = pow((((float)Light_LightnessStatus[elementIndex].LightnessPresentValue16)/65535) , 2);
  Light_LightnessStatus[elementIndex].LinearPresentValue16 = (MOBLEUINT16)(65535 * productValue);
 
  return Light_LightnessStatus[elementIndex].LinearPresentValue16; 
}


/**
* @brief Light_Linear_ActualBinding:Function used for binding the data of actual 
*        lightness and linear lightness. this function changes the value of Actual 
*        lightness as Linear lightness value is set.
* @param length: length of the data 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLEUINT16
*/
MOBLEUINT16 Light_Linear_ActualBinding(MOBLEUINT32 length, MOBLEUINT8 elementIndex)
{
  /*
  6.1.2.1.1 - actual lightness = 655354 * squareroot(linear lightness/ 655354).
  */
  float productValue;
  
  if(length <= 3)
  {
    productValue = sqrt((((float)Light_LightnessStatus[elementIndex].LinearPresentValue16) / 65535));
    Light_LightnessStatus[elementIndex].LightnessPresentValue16 = (MOBLEUINT16)(65535 * productValue);      
    
    return Light_LightnessStatus[elementIndex].LightnessPresentValue16;  
  }
  else
  {
    productValue = sqrt((((float)Light_LightnessStatus[elementIndex].TargetValue16) / 65535));
    Light_LightnessStatus[elementIndex].TargetValue16 = (MOBLEUINT16)(65535 * productValue);
    Light_TemporaryStatus[elementIndex].TargetParam_1 = Light_LightnessStatus[elementIndex].TargetValue16;   
    
    return Light_LightnessStatus[elementIndex].TargetValue16;  
  }
}


/**
* @brief Light_Actual_RangeBinding:Function used for binding the data of actual 
*        lightness and lightness range this function set the value of Actual 
*        lightness according to the min range  and max range value.
* @param lightActual: Pointer which needs to be set. 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Actual_RangeBinding(Light_LightnessParam_t* lightActual, MOBLEUINT8 elementIndex)
{
  /*
  6.1.2.2.5 - actual lightness = range minimum if actual lighteness is less than range min
  value.
  actual lightness = range maximum if actual lighteness is greater than range max
  value.
  */
  if((lightActual->TargetLightnessStatus < Light_LightnessRangeParam[elementIndex].MinRangeStatus)
     && (lightActual->TargetLightnessStatus != 0))
  {
    lightActual->TargetLightnessStatus = Light_LightnessRangeParam[elementIndex].MinRangeStatus;
  }
  else if((lightActual->TargetLightnessStatus > Light_LightnessRangeParam[elementIndex].MaxRangeStatus)
          && (lightActual->TargetLightnessStatus != 0))
  {
    lightActual->TargetLightnessStatus = Light_LightnessRangeParam[elementIndex].MaxRangeStatus;
  }
  else
  {
    /* no condition to execute */
  }
}

/**
* @brief Light_CtlTemperature_Binding: Function used for binding the data of 
*        Ctl temperature with other models.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_CtlTemperature_Binding(MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  MOBLE_RESULT result;
  
  if((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << elementIndex)) == (1 << elementIndex))
  {	
    Light_CtlTemp_GenericLevelBinding(&Light_CtlStatus[elementIndex], elementIndex,
                                      Light_ModelFlag[elementIndex].LightTransitionFlag,Light_ModelFlag[elementIndex].Light_Trnsn_Cmplt);
  
    result = Chk_OpcodePresent((MOBLEUINT16)GENERIC_LEVEL_SET_UNACK ,elementIndex); 
    
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    { 
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= GENERIC_LEVEL_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = GENERIC_LEVEL_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  }
#endif  
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  }
}

/**
* @brief Light_CtlTemperature_TempRangeBinding: Function used for binding the data of 
*        Ctl temperature and Ctl temperature range this function changes the value of 
*        Ctl temperature according to the min range  and max range value.
* @param ctlTempemerature: Pointer which needs to be set.  
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_CtlTemperature_TempRangeBinding(Light_CtlParam_t* ctlTemperature, MOBLEUINT8 elementIndex)
{
  /*
  6.1.2.2.5 - actual lightness = range minimum if actual lighteness is less than range min
  value.
  actual lightness = range maximum if actual lighteness is greater than range max
  value.
  */
  
  if(ctlTemperature->CTL_Temperature < CtlTemperatureRangeParam[elementIndex].MinRangeStatus)        
  {
    ctlTemperature->CTL_Temperature = CtlTemperatureRangeParam[elementIndex].MinRangeStatus;
  }
  else if(ctlTemperature->CTL_Temperature > CtlTemperatureRangeParam[elementIndex].MaxRangeStatus)           
  {
    ctlTemperature->CTL_Temperature = CtlTemperatureRangeParam[elementIndex].MaxRangeStatus;
  }
  else
  {
    /* no condition to execute */
  }
}

/**
* @brief GenericLevel_CtlTempBinding: Data binding b/w Generic level and Ctl
*        Temperature set.
* @param bLevelParam: pointer to the structure, which should be set.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void GenericLevel_CtlTempBinding(Generic_LevelStatus_t * bLevelParam, MOBLEUINT8 elementIndex)
{
  /* 6.1.3.1.1 Binding with the Generic Level state.
  Light CTL Temperature = T_MIN + (Generic Level + 32768) * (T_MAX - T_MIN) / 65535
  T_MIN = minimum range of ctl temperature
  T_MAX = maximum range of ctl temperature
  */
  MOBLEUINT32 productValue;
  
  productValue = (bLevelParam->Present_Level16 + 32768) * (MAX_CTL_TEMP_RANGE - MIN_CTL_TEMP_RANGE) ;
  Light_CtlStatus[elementIndex].PresentCtlTemperature16 = (MOBLEUINT16)(MIN_CTL_TEMP_RANGE + round(((productValue) / (float)65535)));
  
  TRACE_M(TF_LIGHT_M, "PresentGeneric = %d,\r\n", bLevelParam->Present_Level16);
  TRACE_M(TF_LIGHT_M, "Present Level value %d\r\n", Light_CtlStatus[elementIndex].PresentCtlTemperature16); 
}

/**
* @brief  Light_HslLightness_LightnessActualBinding: Data binding b/w Hsl lightness and 
*         Lightness Atual . this function will set the Lightness Actual
*         value at the time of  Hsl Lightness value set. 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_HslLightness_LightnessActualBinding(MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{
  /* 6.1.4.7.1 Binding with the Light Lightness Actual state
  Light Lightness Actual = Light HSL Lightness
  */
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
  MOBLE_RESULT result;
   
  if((ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS & (1 << elementIndex)) == (1 << elementIndex))
  { 
    Light_LightnessStatus[elementIndex].LightnessPresentValue16= Light_HslStatus[elementIndex].PresentHslLightness16;
  
    result = Chk_OpcodePresent((MOBLEUINT16)LIGHT_LIGHTNESS_SET_UNACK ,elementIndex); 
    
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    { 
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= LIGHT_LIGHTNESS_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
    
    Light_Lightness_Binding(BINDING_LIGHT_HSL_SET ,0,elementIndex,trnsnFlag,trnsnCmplt);
  }
#endif  
}

/**
* @brief Light_HSL_HUE_Binding: Function used for binding the data of 
*        hsl hue parameter with other models.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Hsl_Hue_Binding(MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)     
{
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  MOBLE_RESULT result;
  /* Hsl Hue binding with Generic Level */
  if((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << elementIndex)) == (1 << elementIndex))
  {       
    Light_HslHue_GenericLevelBinding(&Light_HslStatus[elementIndex], elementIndex);
    
    result = Chk_OpcodePresent((MOBLEUINT16)GENERIC_LEVEL_SET_UNACK ,elementIndex); 
    
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    { 
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= GENERIC_LEVEL_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = GENERIC_LEVEL_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  }
#endif  
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  }
}

/**
* @brief Light_HSL_Saturation_Binding: Function used for binding the data of 
*        hsl saturation parameter with other models.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_Hsl_Saturation_Binding(MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)     
{
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  MOBLE_RESULT result;
  /* Hsl Hue binding with Generic Level */
  if((ENABLE_GENERIC_MODEL_SERVER_LEVEL & (1 << elementIndex)) == (1 << elementIndex))
  {
    Light_HslSaturation_GenericLevelBinding(&Light_HslStatus[elementIndex], elementIndex);
    
    result = Chk_OpcodePresent((MOBLEUINT16)GENERIC_LEVEL_SET_UNACK ,elementIndex); 
    
    if((result == MOBLE_RESULT_SUCCESS) || (trnsnCmplt == MOBLE_TRUE))
    { 
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= GENERIC_LEVEL_SET_UNACK;
      Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = GENERIC_LEVEL_SERVER_MODEL_ID;
      Light_PublishOpcodeList[elementIndex].BindedStateCount++;
    }
  }
#endif  
  if((trnsnFlag != MODEL_TRANSITION_STOP) && (trnsnCmplt != MOBLE_TRUE) && (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
    Light_PublishOpcodeList[elementIndex].PublishStateOpcode[Light_PublishOpcodeList[elementIndex].BindedStateCount]= 0;
    Light_PublishOpcodeList[elementIndex].Model_ID[Light_PublishOpcodeList[elementIndex].BindedStateCount] = 0;
    Light_PublishOpcodeList[elementIndex].BindedStateCount = 0;
    for(MOBLEUINT8 idx = 0; idx < MAX_NUM_BINDED_STATE; idx++)
    {
      Light_PublishOpcodeList[elementIndex].Model_ID[idx] = 0;
      Light_PublishOpcodeList[elementIndex].PublishStateOpcode[idx] = 0;
    }
  }
}

/**
* @brief GenericLevel_HslSaturationBinding: Data binding b/w Generic level and Hsl
*        Hue set.
* @param bLevelParam: pointer to the structure, which should be set.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void GenericLevel_HslSaturationBinding(Generic_LevelStatus_t * bLevelParam, MOBLEUINT8 elementIndex)
{
  /* 6.1.4.1.1 Binding with the Generic Level state
  Light HSL Hue = Generic Level + 32768
  */
  
  Light_HslStatus[elementIndex].PresentHslSaturation16 = bLevelParam->Present_Level16 + 32768;   
}

/*
* @brief GenericLevel_HslHueBinding: Data binding b/w Generic level and Hsl
*        Hue set.
* @param bLevelParam: pointer to the structure, which should be set.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void GenericLevel_HslHueBinding(Generic_LevelStatus_t * bLevelParam, MOBLEUINT8 elementIndex)
{ 
  /*  6.1.4.1.1 Binding with the Generic Level state
  Light HSL Hue = Generic Level + 32768
  */  
  Light_HslStatus[elementIndex].PresentHslHueLightness16 = bLevelParam->Present_Level16 + 32768;   
}

/**
* @brief Light_HslHue_RangeBinding: Function used for binding the data of 
*        Hsl Hue value and Hsl Hue range. this function changes the value of 
*        Hsl Hue according to the min range  and max range value.
* @param bHslHueParam: Pointer which needs to be set.  
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void
*/
void Light_HslHue_RangeBinding(Light_HslParam_t* bHslHueParam, MOBLEUINT8 elementIndex)
{
  /*
  6.1.4.1.3 Binding with the HSL Hue Range state
  Light HSL Hue = Light HSL Hue Range Min->for (Light HSL Hue < Light HSL Hue Range Min)
  Light HSL Hue = Light HSL Hue Range Max->for (Light HSL Hue > Light HSL Hue Range Min)
  */
  
  if(bHslHueParam->HslHueLightness16< Light_HslRangeParam[elementIndex].HslHueMinRange16)        
  {
    bHslHueParam->HslHueLightness16 = Light_HslRangeParam[elementIndex].HslHueMinRange16;
  }
  else if(bHslHueParam->HslHueLightness16 > Light_HslRangeParam[elementIndex].HslHueMaxRange16)           
  {
    bHslHueParam->HslHueLightness16 = Light_HslRangeParam[elementIndex].HslHueMaxRange16;
  }
  else
  {
    /* no condition to execute */
  }
}

/**
* @brief Light_HslSaturation_RangeBinding: Function used for binding the data of 
*        Hsl saturation value and Hsl saturation range. this function changes the value of 
*        Hsl saturation according to the min range  and max range value.
* @param bHslSatParam: Pointer which needs to be set.  
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1 
* return void
*/
void Light_HslSaturation_RangeBinding(Light_HslParam_t* bHslSatParam, MOBLEUINT8 elementIndex)
{
  /*
  6.1.4.4.3 Binding with the HSL Saturation Range state
  Light HSL Saturation = Light HSL Saturation Range Min Min->for (Light HSL Saturation < Light HSL Saturation Min)
  Light HSL Saturation = Light HSL Saturation Range Max->for (Light HSL Saturation > Light HSL Saturation Max)
  */
  
  if(bHslSatParam->HslSaturation16 < Light_HslRangeParam[elementIndex].HslMinSaturation16)        
  {
    bHslSatParam->HslSaturation16 = Light_HslRangeParam[elementIndex].HslMinSaturation16;
  }
  else if(bHslSatParam->HslSaturation16 > Light_HslRangeParam[elementIndex].HslMaxSaturation16)           
  {
    bHslSatParam->HslSaturation16 = Light_HslRangeParam[elementIndex].HslMaxSaturation16;
  }
  else
  {
    /* no condition to execute */
  }
}

/**
* @brief  Light_ActualLightness_HslLightnessBinding: Data binding b/w Actual Lightness and 
*         Hsl lightness . this function will set the Hsl Lightness value
*         at the time of  Lightness Actual value set. 
* @param bActualLightParam: Pointer to the data which needs to be checked.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_ActualLightness_HslLightnessBinding(Light_LightnessStatus_t* bActualLightParam, MOBLEUINT8 elementIndex)
{
  /* 6.1.4.7.1 Binding with the Light Lightness Actual state
  Light HSL Lightness = Light Lightness Actual
  */
  Light_HslStatus[elementIndex].PresentHslLightness16= bActualLightParam->LightnessPresentValue16;
  
  TRACE_M(TF_LIGHT_M, "Light_ActualLightness_HslLightnessBinding Function - Present HSL lightness value 0x%.2x \r\n" 
         , Light_HslStatus[elementIndex].PresentHslLightness16); 
}

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
/**
* @brief function called in Light lightness when the default transition time is enabled.
* @param pLightnessValue: pointer to the function
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_LightnessDefaultTransitionValue(Light_LightnessParam_t* pLightnessValue, MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "Light_LightnessDefaultTransition StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  
  /* Check if the Transition Time is immediate or not */
    if(Light_TimeParam[elementIndex].StepValue != 0x00){
      
  Light_TemporaryStatus[elementIndex].TargetParam_1 = pLightnessValue->TargetLightnessStatus;   
  Light_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime; 
      Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_LIGHTNESS_TRANSITION_START;   
      Light_ModelFlag[elementIndex].LightOptionalParam = 1;     
    }
    else{
      Light_ModelFlag[elementIndex].LightOptionalParam = 0;
      Light_LightnessStatus[elementIndex].LightnessPresentValue16 = 
                                     Light_LightnessParam.TargetLightnessStatus; 
	  Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
    }
}


/**
* @brief function called in Light lightness linear when the l transition time is enabled.
* @param pLightnessValue: pointer to the function
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_LightnessLinearDefaultTransitionValue(Light_LightnessParam_t* pLightnessValue, MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "Light_LightnessLinearDefaultTransition StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  /* Check if the Transition Time is immediate or not */
    if(Light_TimeParam[elementIndex].StepValue != 0x00){
      
      Light_TemporaryStatus[elementIndex].TargetParam_1 = Light_LightnessParam.TargetLightnessLinearStatus;
      Light_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime; 
      Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_LINEAR_TRANSITION_START;   
  Light_ModelFlag[elementIndex].LightOptionalParam = 1;
}
    else{
      Light_ModelFlag[elementIndex].LightOptionalParam = 0;
      Light_LightnessStatus[elementIndex].LinearPresentValue16 = 
      Light_LightnessParam.TargetLightnessLinearStatus; 
	  Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
    }
}

/**
* @brief function called in Light CTL set when the default transition time is enabled.
* @param pCTLValue: pointer to the function
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_CTLDefaultTransitionValue(Light_CtlParam_t* pCTLValue,MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "Light_LightnessDefaultTransition StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  
  /* Check if the Transition Time is immediate or not */
  if(Light_TimeParam[elementIndex].StepValue != 0x00)
  {
  Light_TemporaryStatus[elementIndex].TargetParam_1 = pCTLValue->CTL_Lightness;
  Light_TemporaryStatus[elementIndex].TargetParam_2 = pCTLValue->CTL_Temperature;
  Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_CTL_TRANSITION_START;
  Light_ModelFlag[elementIndex].LightOptionalParam = 1;
}
  else
  {
    Light_ModelFlag[elementIndex].LightOptionalParam = 0;
    Light_CtlStatus[elementIndex].PresentCtlLightness16 = pCTLValue->CTL_Lightness;
    Light_CtlStatus[elementIndex].PresentCtlTemperature16 = pCTLValue->CTL_Temperature;
	Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  }                                                        
  TRACE_M(TF_LIGHT_M, "PresentCtlTemperature16: %d\r\n", Light_CtlStatus[elementIndex].PresentCtlTemperature16);
}

/**
* @brief function called in Light CTL temperature set when the default transition time is enabled.
* @param pCTLValue:pointer to the function
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Light_CTLTemperatureDefaultTransitionValue(Light_CtlParam_t* pCTLValue,
                                                MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "Light_LightnessDefaultTransition StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  
  /* Check if the Transition Time is immediate or not */
  if(Light_TimeParam[elementIndex].StepValue != 0x00)
{
  Light_TemporaryStatus[elementIndex].TargetParam_1 = pCTLValue->CTL_Temperature;
  Light_TemporaryStatus[elementIndex].TargetParam_2 = pCTLValue->CTL_DeltaUv;
    Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_TEMPERATURE_TRANSITION_START;
    Light_ModelFlag[elementIndex].LightOptionalParam = 1;
  }
  else
  {
    Light_ModelFlag[elementIndex].LightOptionalParam = 0;
    Light_CtlStatus[elementIndex].PresentCtlTemperature16 = pCTLValue->CTL_Temperature;
    Light_CtlStatus[elementIndex].PresentCtlDelta16 = pCTLValue->CTL_DeltaUv;
	Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  }
  TRACE_M(TF_LIGHT_M, "PresentCtlTemperature16: %d\r\n", Light_CtlStatus[elementIndex].PresentCtlTemperature16);
}

/**
* @brief function called in Light HSL set when the default transition time is enabled.
* @param pCTLValue: pointer to the function
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/      
void Light_HSLDefaultTransitionValue(Light_HslParam_t* pHSLValue,
                                     MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  
  /* Check if the Transition Time is immediate or not */
  if(Light_TimeParam[elementIndex].StepValue != 0x00)
  {
     Light_TemporaryStatus[elementIndex].TargetParam_1 = pHSLValue->HslLightness16;
     Light_TemporaryStatus[elementIndex].TargetParam_2 = pHSLValue->HslHueLightness16;
     Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_HSL_TRANSITION_START;
     Light_ModelFlag[elementIndex].LightOptionalParam = 1;
  }
  else
  {
    Light_ModelFlag[elementIndex].LightOptionalParam = 0;
    Light_HslStatus[elementIndex].PresentHslLightness16 = pHSLValue->HslLightness16;
    Light_HslStatus[elementIndex].PresentHslHueLightness16 = pHSLValue->HslHueLightness16;
    Light_HslStatus[elementIndex].PresentHslSaturation16 = pHSLValue->HslSaturation16;
    Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  }                                                        
  TRACE_M(TF_LIGHT_M, "PresentHSLLightness: %d \r\n", Light_HslStatus[elementIndex].PresentHslLightness16);
}
/**
* @brief function called in Light HSL Hue set when the default transition time is enabled.
* @param pCTLValue: pointer to the HSL Parameters
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/      
void Light_HSLHueDefaultTransitionValue(Light_HslParam_t* pHSLHueValue,
                                     MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 
  
  /* Check if the Transition Time is immediate or not */
  if(Light_TimeParam[elementIndex].StepValue != 0x00)
  {
     Light_TemporaryStatus[elementIndex].TargetParam_1 = pHSLHueValue->HslHueLightness16;
     Light_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
     
     /* Load status register with updated value for Status response */
     Light_HslStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
     Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_HSL_HUE_TRANSITION_START;
  Light_ModelFlag[elementIndex].LightOptionalParam = 1;
}
  else
  {
    Light_ModelFlag[elementIndex].LightOptionalParam = 0;
    Light_HslStatus[elementIndex].PresentHslHueLightness16 = pHSLHueValue->HslHueLightness16;
    Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  }                                                        
  TRACE_M(TF_LIGHT_M, "PresentHSLLightness: %d\r\n", Light_HslStatus[elementIndex].PresentHslLightness16);
}
/**
* @brief function called in Light HSL Saturation set when the default transition time is enabled.
* @param pHSLSaturationValue: pointer to the HSL Parameters
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/      
void Light_HSLSaturationDefaultTransitionValue(Light_HslParam_t* pHSLSaturationValue,
                                     MOBLEUINT8 elementIndex)
{
  Light_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime,elementIndex);
  TRACE_M(TF_LIGHT_M, "StepValue is =%.2x \r\n", Light_TimeParam[elementIndex].StepValue); 

  /* Check if the Transition Time is immediate or not */
  if(Light_TimeParam[elementIndex].StepValue != 0x00)
  {
     Light_TemporaryStatus[elementIndex].TargetParam_1 = pHSLSaturationValue->HslSaturation16;
     Light_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
     /* Load status register with updated value for Status response */
     Light_HslStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
     Light_ModelFlag[elementIndex].LightTransitionFlag = LIGHT_HSL_SATURATION_TRANSITION_START;
     Light_ModelFlag[elementIndex].LightOptionalParam = 1;
  }
  else
  {
    Light_ModelFlag[elementIndex].LightOptionalParam = 0;
    Light_HslStatus[elementIndex].PresentHslSaturation16 = pHSLSaturationValue->HslSaturation16;
    Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  }                                                        
  TRACE_M(TF_LIGHT_M, "PresentHSLSaturation: %d\r\n", Light_HslStatus[elementIndex].PresentHslSaturation16);
}
#endif

/**
* @brief Function called when message received without optional parameter, stop the running transition.
* @param void
* @param void
*/
void Light_TransitionParameterReset(MOBLEUINT8 elementIndex)
{
  Light_ModelFlag[elementIndex].LightTransitionFlag = MODEL_TRANSITION_STOP;
  Light_TimeParam[elementIndex].Res_Value = 0x00;
  Light_TimeParam[elementIndex].StepValue = 0x00;
}

/**
* @brief Function is to check the opcode present in the buffer or not
* @param opcode: opcode of the model
* @param elementIndex:element index of the model
* @return : MOBLE_RESULT
*/
MOBLE_RESULT Chk_OpcodePresent(MOBLEUINT16 opcode ,MOBLEUINT8 elementIndex)
{
 
  for(MOBLEUINT8 index=0; index < MAX_NUM_BINDED_STATE; index++)
  {
    if((Light_PublishOpcodeList[elementIndex].PublishStateOpcode[index]) == opcode)
    {
      return  MOBLE_RESULT_FALSE;
    }
  }
   return MOBLE_RESULT_SUCCESS;     
}

/**
* @brief  Function used to calculate the particular time delay only once.
* @param  MOBLEUINT16
* @retval MOBLEUINT8
*/
void Punblication_OneSecTimer(void)
{
  static MOBLEUINT32 Check_time;
  
  /* Taking the time stamp for particular time */
  if(Publication1SecFlag.TimeStampFlag == MOBLE_FALSE)
  {
    Check_time = Clock_Time();
    Publication1SecFlag.TimeStampFlag = MOBLE_TRUE;
  }
  
  if(((Clock_Time()- Check_time) >= PUBLISH_1SEC_TIME) &&
                  (Publication1SecFlag.Transition1SecFlag == MOBLE_FALSE))
  {
      Publication1SecFlag.Transition1SecFlag = MOBLE_TRUE;
  }
}

/**
* @brief function used to restore the states of the generic model.
* @param pointer to the array
* return void.
*/
void Light_RestoreModelStates(void *model_State , MOBLEUINT8 elementIndex)
{
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
  Light_LightnessStatus[elementIndex].LightnessPresentValue16 = ((Models_ParamNvm_t *)model_State)->LightLightness; 
  Light_LightnessStatus[elementIndex].TargetValue16 = ((Models_ParamNvm_t *)model_State)->LightTarget;
  Light_LightnessStatus[elementIndex].LightnessLastStatus = ((Models_ParamNvm_t *)model_State)->LightLast;
  Light_LightnessDefaultParam[elementIndex].LightnessDefaultStatus = ((Models_ParamNvm_t *)model_State)->LightDefault;
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL   
  Light_CtlStatus[elementIndex].PresentCtlLightness16 = ((Models_ParamNvm_t *)model_State)->CtlLightness;
  Light_CtlStatus[elementIndex].PresentCtlTemperature16 = ((Models_ParamNvm_t *)model_State)->CtlTemperature;
  Light_CtlStatus[elementIndex].PresentCtlDelta16 = ((Models_ParamNvm_t *)model_State)->CtlDelta;
  
  Light_CtlDefaultParam[elementIndex].CtlDefaultLightness16 = ((Models_ParamNvm_t *)model_State)->CtlDefaultLightness;
  Light_CtlDefaultParam[elementIndex].CtlDefaultTemperature16 = ((Models_ParamNvm_t *)model_State)->CtlDefaultTemperature;
  Light_CtlDefaultParam[elementIndex].CtlDefaultDeltaUv = ((Models_ParamNvm_t *)model_State)->CtlDefaultDelta;
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL     
  Light_HslStatus[elementIndex].PresentHslLightness16 = ((Models_ParamNvm_t *)model_State)->HslLightness;
  Light_HslStatus[elementIndex].PresentHslHueLightness16 = ((Models_ParamNvm_t *)model_State)->HslHue;
  Light_HslStatus[elementIndex].PresentHslSaturation16 = ((Models_ParamNvm_t *)model_State)->HslSaturation;
  Light_HslDefaultParam[elementIndex].HslLightnessDefault16 = ((Models_ParamNvm_t *)model_State)->HslLightnessDefault;
  Light_HslDefaultParam[elementIndex].HslHueDefault16 = ((Models_ParamNvm_t *)model_State)->HslHueDefault;
  Light_HslDefaultParam[elementIndex].HslSaturationDefault16 = ((Models_ParamNvm_t *)model_State)->HslSaturationDefault;  
#endif   
}

/**
* @brief function used to save the states of the generic model.
* @param pointer to the array
* return void.
*/
void Light_SaveModelStates(void *model_State,MOBLEUINT8 elementIndex)
{
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS    
  ((Models_ParamNvm_t *)model_State)->LightLightness = Light_LightnessStatus[elementIndex].LightnessPresentValue16;
  ((Models_ParamNvm_t *)model_State)->LightTarget = Light_LightnessStatus[elementIndex].TargetValue16;
  ((Models_ParamNvm_t *)model_State)->LightDefault = Light_LightnessDefaultParam[elementIndex].LightnessDefaultStatus;
  ((Models_ParamNvm_t *)model_State)->LightLast = Light_LightnessStatus[elementIndex].LightnessLastStatus;
  
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL  
  ((Models_ParamNvm_t *)model_State)->CtlLightness = Light_CtlStatus[elementIndex].PresentCtlLightness16;
  ((Models_ParamNvm_t *)model_State)->CtlTemperature = Light_CtlStatus[elementIndex].PresentCtlTemperature16;
  ((Models_ParamNvm_t *)model_State)->CtlDelta = Light_CtlStatus[elementIndex].PresentCtlDelta16;
  
  ((Models_ParamNvm_t *)model_State)->CtlDefaultLightness = Light_CtlDefaultParam[elementIndex].CtlDefaultLightness16;
  ((Models_ParamNvm_t *)model_State)->CtlDefaultTemperature = Light_CtlDefaultParam[elementIndex].CtlDefaultTemperature16;
  ((Models_ParamNvm_t *)model_State)->CtlDefaultDelta = Light_CtlDefaultParam[elementIndex].CtlDefaultDeltaUv;
  
  ((Models_ParamNvm_t *)model_State)->CtlLightTarget = Light_CtlStatus[elementIndex].TargetCtlLightness16;
  ((Models_ParamNvm_t *)model_State)->CtlTempTarget = Light_CtlStatus[elementIndex].TargetCtlTemperature16;
  ((Models_ParamNvm_t *)model_State)->CtlDeltaTarget = Light_CtlStatus[elementIndex].TargetCtlDeltaUv16;
#endif 
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL   
  ((Models_ParamNvm_t *)model_State)->HslLightness = Light_HslStatus[elementIndex].PresentHslLightness16;
  ((Models_ParamNvm_t *)model_State)->HslHue = Light_HslStatus[elementIndex].PresentHslHueLightness16;
  ((Models_ParamNvm_t *)model_State)->HslSaturation = Light_HslStatus[elementIndex].PresentHslSaturation16;
  
  ((Models_ParamNvm_t *)model_State)->HslLightnessDefault = Light_HslDefaultParam[elementIndex].HslLightnessDefault16;
  ((Models_ParamNvm_t *)model_State)->HslHueDefault = Light_HslDefaultParam[elementIndex].HslHueDefault16; 
  ((Models_ParamNvm_t *)model_State)->HslSaturationDefault = Light_HslDefaultParam[elementIndex].HslSaturationDefault16;
    
  ((Models_ParamNvm_t *)model_State)->HslLightTarget = Light_HslStatus[elementIndex].TargetHslLightness16;
  ((Models_ParamNvm_t *)model_State)->HslHueTarget = Light_HslStatus[elementIndex].TargetHslHueLightness16;
  ((Models_ParamNvm_t *)model_State)->HslSatTarget = Light_HslStatus[elementIndex].TargetHslSaturation16;
#endif
  ((Models_ParamNvm_t *)model_State)->LightTransitionStatus = Light_ModelFlag[elementIndex].LightTransitionFlag;

}

/**
Weak function are defined to support the original function if they are not
included in firmware.
There is no use of this function for application development purpose.
*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t* pLight_LightnessParam,
                                                      MOBLEUINT8 OptionalValid, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex))
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                                            MOBLEUINT8 OptionalValid, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                                             MOBLEUINT8 OptionalValid, 
                                                             uint16_t dstPeer, 
                                                             uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Last_Set(Light_LightnessStatus_t* pLight_LightnessParam,\
                                            MOBLEUINT8 elementIndex))
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                                           MOBLEUINT8 OptionalValid, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                               MOBLEUINT8 OptionalValid, 
                                               uint16_t dstPeer, 
                                               uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                                          MOBLEUINT8 OptionalValid, 
                                                          uint16_t dstPeer, 
                                                          uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                                                MOBLEUINT8 OptionalValid, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                                      MOBLEUINT8 OptionalValid, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                               MOBLEUINT8 OptionalValid, 
                                               uint16_t dstPeer, 
                                               uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                                  MOBLEUINT8 OptionalValid, 
                                                  uint16_t dstPeer, 
                                                  uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                                         MOBLEUINT8 OptionalValid, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslDefaultParam_t* pLight_HslDefaultParam,
                                                      MOBLEUINT8 OptionalValid, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslRangeParam,
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex))   
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Status(MOBLEUINT8 const *pLightness_status, 
                                                        MOBLEUINT32 pLength, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, 
                                                               MOBLEUINT32 pLength, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, 
                                                                MOBLEUINT32 pLength, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                                             MOBLEUINT32 pLength, 
                                                             MOBLEUINT16 dstPeer, 
                                                             MOBLEUINT8 elementIndex))
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, 
                                                              MOBLEUINT32 pLength, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, 
                                                  MOBLEUINT32 pLength, 
                                                  uint16_t dstPeer, 
                                                  uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, 
                                                             MOBLEUINT32 pLength, 
                                                             uint16_t dstPeer, 
                                                             uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, 
                                                                   MOBLEUINT32 pLength, 
                                                                   uint16_t dstPeer, 
                                                                   uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, 
                                                         MOBLEUINT32 pLength, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_Hsl_Status(MOBLEUINT8 const *pHsl_status, 
                                                  MOBLEUINT32 pLength, 
                                                  uint16_t dstPeer, 
                                                  uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslHue_Status(MOBLEUINT8 const *pHslHue_status, 
                                                     MOBLEUINT32 pLength, 
                                                     uint16_t dstPeer, 
                                                     uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, 
                                                            MOBLEUINT32 pLength, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, 
                                                         MOBLEUINT32 pLength, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_HslRange_Status(MOBLEUINT8 const *pHslRange_status, 
                                                       MOBLEUINT32 pLength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex))  
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState, 
                                                          uint16_t dstPeer, 
                                                          uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState, 
                                                                uint16_t dstPeer, 
                                                                uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState, 
                                                                 uint16_t dstPeer, 
                                                                 uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessLastStatus(MOBLEUINT8* lDefaultState, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTargetStatus(MOBLEUINT8* lCtlLightState, 
                                                          MOBLEUINT16 dstPeer, 
                                                          MOBLEUINT8 elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState, 
                                                               uint16_t dstPeer, 
                                                               uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslTargetStatus(MOBLEUINT8* lHslState, 
                                                          MOBLEUINT16 dstPeer, 
                                                          MOBLEUINT8 elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange, 
                                                      uint16_t dstPeer, 
                                                      uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(MOBLE_RESULT Appli_Light_GetHslDefaultStatus(MOBLEUINT8* lHslDefaultState, 
                                                           uint16_t dstPeer, 
                                                           uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS; }
WEAK_FUNCTION(void HSL2RGB_Conversion(MOBLEUINT8 elementIndex) {})
WEAK_FUNCTION(void RgbF_Create(MOBLEUINT16 value1, 
                               MOBLEUINT16 value2, 
                               MOBLEUINT16 value3, 
                               MOBLEUINT8 elementIndex) {})
WEAK_FUNCTION(void Light_UpdatePWMValue(MOBLEUINT8 state) {})
/**
* @}
*/

/**
* @}
*/





