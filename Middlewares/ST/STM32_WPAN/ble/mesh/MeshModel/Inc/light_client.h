/**
******************************************************************************
* @file    light_client.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIGHT_CLIENT_H
#define __LIGHT_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/

/* Variable ------------------------------------------------------------------*/
  
/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
/* Light Client Structures */
/* Light Ctl Temperature parameters*/
typedef struct
{
  MOBLEUINT16 CTL_Temperature;
  MOBLEINT16 CTL_DeltaUv;
  MOBLEUINT8  CTL_TID;
  MOBLEUINT8  CTL_TransitionTime;
  MOBLEUINT8  CTL_Delay;
}Light_CtlTemperatureParam_t;

typedef struct
{
  MOBLEUINT16 HslHueLightness16;
  MOBLEUINT8  Hsl_TID;
  MOBLEUINT8  Hsl_TransitionTime;
  MOBLEUINT8  Hsl_Delay;
}Light_HslHueParam_t;

typedef struct
{
  MOBLEUINT16 HslSaturation16;
  MOBLEUINT8  Hsl_TID;
  MOBLEUINT8  Hsl_TransitionTime;
  MOBLEUINT8  Hsl_Delay;
}Light_HslSaturationParam_t;

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT LightModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                               MOBLEUINT16 *length);

MOBLE_RESULT LightModelClient_GetStatusRequestCb(MODEL_MessageHeader_t* pmsgParam, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 *pResponsedata, 
                                                 MOBLEUINT32 *plength, 
                                                 MOBLEUINT8 const *pRxData,
                                                 MOBLEUINT32 dataLength,
                                                 MOBLEBOOL response);

MOBLE_RESULT LightModelClient_ProcessMessageCb(MODEL_MessageHeader_t* pmsgParam, 
                                               MOBLEUINT16 opcode, 
                                               MOBLEUINT8 const *pRxData, 
                                               MOBLEUINT32 dataLength, 
                                               MOBLEBOOL response);

MOBLE_RESULT LightClient_Lightness_Get(MOBLEUINT8 elementIndex); 
MOBLE_RESULT LightClient_Lightness_Set_Ack(MOBLEUINT8 elementIndex,  
                                           MOBLEUINT8 *pLightness_param, 
                                           MOBLEUINT32 length);
MOBLE_RESULT LightClient_Lightness_Set_Unack(MOBLEUINT8 elementIndex,  
                                             MOBLEUINT8 *pLightness_param, 
                                             MOBLEUINT32 length);

MOBLE_RESULT LightClient_Lightness_Linear_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Lightness_Linear_Set_Ack(MOBLEUINT8 elementIndex, 
                                                  MOBLEUINT8 *pLightnessLinear_param, 
                                                  MOBLEUINT32 length);
MOBLE_RESULT LightClient_Lightness_Linear_Set_Unack(MOBLEUINT8 elementIndex, 
                                                    MOBLEUINT8 *pLightnessLinear_param, 
                                                    MOBLEUINT32 length);

MOBLE_RESULT LightClient_Lightness_Default_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Lightness_Default_Set_Ack(MOBLEUINT8 elementIndex, 
                                                   MOBLEUINT8 *pLightnessDefault_param, 
                                                   MOBLEUINT32 length);
MOBLE_RESULT LightClient_Lightness_Default_Set_Unack(MOBLEUINT8 elementIndex, 
                                                     MOBLEUINT8 *pLightnessDefault_param, 
                                                     MOBLEUINT32 length);

MOBLE_RESULT LightClient_Lightness_Range_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Lightness_Range_Set_Ack(MOBLEUINT8 elementIndex, 
                                                 MOBLEUINT8 *pLightnessRange_param, 
                                                 MOBLEUINT32 length);
MOBLE_RESULT LightClient_Lightness_Range_Set_Unack(MOBLEUINT8 elementIndex, 
                                                   MOBLEUINT8 *pLightnessRange_param, 
                                                   MOBLEUINT32 length);

MOBLE_RESULT LightClient_Ctl_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Ctl_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pLightCtl_param, 
                                     MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Ctl_Set_Unack(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pLightCtl_param,
                                       MOBLEUINT32 length);

MOBLE_RESULT LightClient_Ctl_Temperature_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Ctl_Temperature_Set_Ack(MOBLEUINT8 elementIndex, 
                                                 MOBLEUINT8 *pLightCtlTemperature_param, 
                                                 MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Ctl_Temperature_Set_Unack(MOBLEUINT8 elementIndex, 
                                                   MOBLEUINT8 *pLightCtlTemperature_param,
                                                   MOBLEUINT32 length); 

MOBLE_RESULT LightClient_Ctl_Temperature_Range_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Ctl_Temperature_Range_Set_Ack(MOBLEUINT8 elementIndex, 
                                                       MOBLEUINT8 *pLightCtlTemperatureRange_param, 
                                                       MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Ctl_Temperature_Range_Set_Unack(MOBLEUINT8 elementIndex, 
                                                         MOBLEUINT8 *pLightCtlTemperatureRange_param,
                                                         MOBLEUINT32 length); 

MOBLE_RESULT LightClient_Ctl_Default_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Ctl_Default_Set_Ack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pLightCtlDefault_param, 
                                             MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Ctl_Default_Set_Unack(MOBLEUINT8 elementIndex, 
                                               MOBLEUINT8 *pLightCtlDefault_param,
                                               MOBLEUINT32 length); 

MOBLE_RESULT LightClient_Hsl_Get(MOBLEUINT8 elementIndex) ;
MOBLE_RESULT LightClient_Hsl_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pLightHsl_param, 
                                     MOBLEUINT32 length); 

MOBLE_RESULT LightClient_Hsl_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pLightHsl_param,
                                     MOBLEUINT32 length); 

MOBLE_RESULT LightClient_Hsl_Default_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Hsl_Default_Set_Ack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pLightHslDefault_param, 
                                             MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Hsl_Default_Set_Unack(MOBLEUINT8 elementIndex, 
                                               MOBLEUINT8 *pLightHslDefault_param,
                                               MOBLEUINT32 length); 
                                             
MOBLE_RESULT LightClient_Hsl_Range_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Hsl_Range_Set_Ack(MOBLEUINT8 elementIndex, 
                                           MOBLEUINT8 *pLightHslRange_param, 
                                           MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Hsl_Range_Set_Unack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pLightHslRange_param,
                                             MOBLEUINT32 length); 
 
MOBLE_RESULT LightClient_Hsl_Hue_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Hsl_Hue_Set_Ack(MOBLEUINT8 elementIndex, 
                                         MOBLEUINT8 *pLightHslHue_param, 
                                         MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Hsl_Hue_Set_Unack(MOBLEUINT8 elementIndex, 
                                           MOBLEUINT8 *pLightHslHue_param,
                                           MOBLEUINT32 length);

MOBLE_RESULT LightClient_Hsl_Saturation_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_Hsl_Saturation_Set_Ack(MOBLEUINT8 elementIndex, 
                                         MOBLEUINT8 *pLightHslSaturation_param, 
                                         MOBLEUINT32 length); 
MOBLE_RESULT LightClient_Hsl_Saturation_Set_Unack(MOBLEUINT8 elementIndex, 
                                           MOBLEUINT8 *pLightHslSaturation_param,
                                           MOBLEUINT32 length);

#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC  
MOBLE_RESULT LightClient_LC_Mode_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_LC_Mode_Set_Ack(MOBLEUINT8 elementIndex, 
                                         MOBLEUINT8 *pLightLCMode_param, 
                                         MOBLEUINT32 length); 
MOBLE_RESULT LightClient_LC_Mode_Set_Unack(MOBLEUINT8 elementIndex, 
                                           MOBLEUINT8 *pLightLCMode_param,
                                           MOBLEUINT32 length);

MOBLE_RESULT LightClient_LC_OM_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_LC_OM_Set_Ack(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pLightLCOccupancyMode_param, 
                                       MOBLEUINT32 length); 
MOBLE_RESULT LightClient_LC_OM_Set_Unack(MOBLEUINT8 elementIndex, 
                                         MOBLEUINT8 *pLightLCOccupancyMode_param,
                                         MOBLEUINT32 length);

MOBLE_RESULT LightClient_LC_OnOff_Get(MOBLEUINT8 elementIndex);
MOBLE_RESULT LightClient_LC_OnOff_Set_Ack(MOBLEUINT8 elementIndex, 
                                          MOBLEUINT8 *pLightLCOnOff_param, 
                                          MOBLEUINT32 length); 
MOBLE_RESULT LightClient_LC_OnOff_Set_Unack(MOBLEUINT8 elementIndex, 
                                            MOBLEUINT8 *pLightLCOnOff_param,
                                            MOBLEUINT32 length);

MOBLE_RESULT LightClient_LC_Property_Get(MOBLEUINT8 elementIndex,
                                         MOBLEUINT8 *pLightLCProperty_param);
MOBLE_RESULT LightClient_LC_Property_Set_Ack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pLightLCProperty_param, 
                                             MOBLEUINT32 length); 
MOBLE_RESULT LightClient_LC_Property_Set_Unack(MOBLEUINT8 elementIndex, 
                                               MOBLEUINT8 *pLightLCProperty_param,
                                               MOBLEUINT32 length);
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LC */

// Status
MOBLE_RESULT Light_Client_Lightness_Status(MOBLEUINT8 const *pLightness_status, 
                                           MOBLEUINT32 plength, 
                                           MOBLEUINT16 dstPeer, 
                                           MOBLEUINT8 elementIndex);

MOBLE_RESULT Light_Client_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, 
                                                  MOBLEUINT32 pLength,
                                                  MOBLEUINT16 dstPeer,
                                                  MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                                MOBLEUINT32 pLength,
                                                MOBLEUINT16 dstPeer,
                                                MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, 
                                                   MOBLEUINT32 pLength,
                                                   MOBLEUINT16 dstPeer,
                                                   MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, 
                                                 MOBLEUINT32 pLength,
                                                 MOBLEUINT16 dstPeer,
                                                 MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, 
                                     MOBLEUINT32 pLength,
                                     MOBLEUINT16 dstPeer,
                                     MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, 
                                                      MOBLEUINT32 pLength,
                                                      MOBLEUINT16 dstPeer,
                                                      MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, 
                                            MOBLEUINT32 pLength,
                                            MOBLEUINT16 dstPeer,
                                            MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, 
                                                MOBLEUINT32 pLength,
                                                MOBLEUINT16 dstPeer,
                                                MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_Hsl_Status(MOBLEUINT8 const *pHsl_status, 
                                     MOBLEUINT32 pLength,
                                     MOBLEUINT16 dstPeer,
                                     MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, 
                                            MOBLEUINT32 pLength,
                                            MOBLEUINT16 dstPeer,
                                            MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_HslRange_Status(MOBLEUINT8 const *pHslRange_status, 
                                          MOBLEUINT32 pLength,
                                          MOBLEUINT16 dstPeer,
                                          MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_HslTarget_Status(MOBLEUINT8 const *pHslTarget_status, 
                                           MOBLEUINT32 pLength,
                                           MOBLEUINT16 dstPeer,
                                           MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_HslHue_Status(MOBLEUINT8 const *pHslHue_status, 
                                        MOBLEUINT32 pLength,
                                        MOBLEUINT16 dstPeer,
                                        MOBLEUINT8 elementIndex);
MOBLE_RESULT Light_Client_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, 
                                               MOBLEUINT32 pLength,
                                               MOBLEUINT16 dstPeer,
                                               MOBLEUINT8 elementIndex);
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC  
MOBLE_RESULT LightLC_Client_Mode_Status(MOBLEUINT8 const *pLCMode_status, 
                                        MOBLEUINT32 plength, 
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);
MOBLE_RESULT LightLC_Client_OM_Status(MOBLEUINT8 const *pLCOccupancyMode_status, 
                                      MOBLEUINT32 plength, 
                                      MOBLEUINT16 dstPeer, 
                                      MOBLEUINT8 elementIndex);
MOBLE_RESULT LightLC_Client_OnOff_Status(MOBLEUINT8 const *pLCOnOff_status, 
                                         MOBLEUINT32 plength, 
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);
MOBLE_RESULT LightLC_Client_Property_Status(MOBLEUINT8 const *pLCProperty_status, 
                                            MOBLEUINT32 plength, 
                                            MOBLEUINT16 dstPeer, 
                                            MOBLEUINT8 elementIndex);
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LC */

#endif /* __LIGHT_CLIENT_H */



