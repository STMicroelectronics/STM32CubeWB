/**
******************************************************************************
* @file    appli_light_lc.h
* @author  BLE Mesh Team
* @brief   Application interface for Light Control Mesh Models  
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
#ifndef __APPLI_LIGHT_LC_H
#define __APPLI_LIGHT_LC_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "light_lc.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
/*******************************************************************************
Following Variables are used for the Light LC Mode MODEL 
*******************************************************************************/
void Appli_LightLCs_ModeGet(MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_ModeSet(MOBLEUINT8 const* pRxData, 
                            MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_ModeSetUnack(MOBLEUINT8 const* pRxData, 
                                 MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_ModeStatus(MOBLEUINT8 const* pLightLCMode_status,
                               MOBLEUINT32 plength, 
                               MOBLEUINT16 dstPeer, 
                               MOBLEUINT8 elementIndex);
void Appli_LightLCs_OmGet(MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OmSet(MOBLEUINT8 const* pRxData, 
                          MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OmSetUnack(MOBLEUINT8 const* pRxData, 
                               MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OmStatus(MOBLEUINT8 const* pLightLCOccupancyMode_status,
                             MOBLEUINT32 plength, 
                             MOBLEUINT16 dstPeer, 
                             MOBLEUINT8 elementIndex);
void Appli_LightLCs_OnOffGet(MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OnOffSet(MOBLEUINT8 const* pRxData, 
                             MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OnOffSetUnack(MOBLEUINT8 const* pRxData, 
                                  MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_OnOffStatus(MOBLEUINT8 const* pLightLCOnOff_status,
                                MOBLEUINT32 plength, 
                                MOBLEUINT16 dstPeer, 
                                MOBLEUINT8 elementIndex);
void Appli_LightLCs_PropertyGet(MOBLEUINT8 const* pRxData, 
                                MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_PropertySet(MOBLEUINT8 const* pRxData, 
                                MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_PropertySetUnack(MOBLEUINT8 const* pRxData, 
                                     MODEL_MessageHeader_t *pmsgParam);
void Appli_LightLCs_PropertyStatus(MOBLEUINT8 const* pLightLCOccupancyMode_status,
                                   MOBLEUINT32 plength, 
                                   MOBLEUINT16 dstPeer, 
                                   MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Light_LCs_Init(void);
MOBLE_RESULT Appli_Light_LC_SensorPropertyUpdate(MOBLEUINT8 elementIndex,
                                                 MOBLEUINT16 propertyId,
                                                 MOBLEUINT32 value);
void Appli_Light_LC_SerialCmd(char *rcvdStringBuff, 
                              uint16_t rcvdStringSize);
#endif /* __APPLI_LIGHT_LC_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

