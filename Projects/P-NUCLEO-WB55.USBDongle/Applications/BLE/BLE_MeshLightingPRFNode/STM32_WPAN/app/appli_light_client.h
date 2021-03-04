/**
******************************************************************************
* @file    appli_light_client.h
* @author  BLE Mesh Team
* @brief   Application interface for Generic Mesh Models  
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_LIGHT_CLIENT_H
#define __APPLI_LIGHT_CLIENT_H

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
 
#endif /* __APPLI_LIGHT_CLIENT_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

