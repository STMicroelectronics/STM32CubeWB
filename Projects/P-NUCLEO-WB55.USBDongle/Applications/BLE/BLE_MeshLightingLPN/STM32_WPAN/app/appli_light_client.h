/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_light_client.h
  * @author  MCD Application Team
  * @brief   Application interface for Generic Mesh Models  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APPLI_LIGHT_CLIENT_H
#define APPLI_LIGHT_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "generic.h"
#include "mesh_cfg.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
/* Application Variable-------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT Appli_Light_Lightness_Status(MOBLEUINT8 const *pLightness_status, 
                                          MOBLEUINT32 pLength,
                                          MOBLEUINT16 dstPeer, 
                                          MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, 
                                                 MOBLEUINT32 pLength,
                                                 MOBLEUINT16 dstPeer, 
                                                 MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                               MOBLEUINT32 pLength, 
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, 
                                                  MOBLEUINT32 pLength,
                                                  MOBLEUINT16 dstPeer, 
                                                  MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, 
                                                MOBLEUINT32 pLength,
                                                MOBLEUINT16 dstPeer, 
                                                MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, 
                                    MOBLEUINT32 pLength,
                                    MOBLEUINT16 dstPeer, 
                                    MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, 
                                               MOBLEUINT32 pLength,
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, 
                                                     MOBLEUINT32 pLength,
                                                     MOBLEUINT16 dstPeer, 
                                                     MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, 
                                           MOBLEUINT32 pLength,
                                           MOBLEUINT16 dstPeer, 
                                           MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Hsl_Status(MOBLEUINT8 const *pHsl_status, 
                                    MOBLEUINT32 pLength,
                                    MOBLEUINT16 dstPeer, 
                                    MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, 
                                              MOBLEUINT32 pLength,
                                              MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslHue_Status(MOBLEUINT8 const *pHslHue_status, 
                                       MOBLEUINT32 pLength,
                                       MOBLEUINT16 dstPeer, 
                                       MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslRange_Status(MOBLEUINT8 const *pHslRange_status, 
                                         MOBLEUINT32 pLength,
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, 
                                           MOBLEUINT32 pLength,
                                           MOBLEUINT16 dstPeer, 
                                           MOBLEUINT8 elementIndex);  

MOBLE_RESULT Appli_LightClient_API(MOBLEUINT8 elementIndex, 
                                   MOBLEUINT16 msg_opcode, 
                                   MOBLEUINT8* msg_params);
 
#endif /* APPLI_LIGHT_CLIENT_H */


