/**
******************************************************************************
* @file    light_client.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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
#ifndef __LIGHT_CLIENT_H
#define __LIGHT_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/
/* Variable ------------------------------------------------------------------*/
#pragma pack(1)
typedef union  {
  Light_LightnessParam_t sLight_LightnessParam;
  MOBLEUINT8 a_Lightness_param[sizeof(Light_LightnessParam_t)]; 
} _Light_LightnessParam;
/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/


/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT LightModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT LightModelClient_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                   MOBLE_ADDRESS dst_peer, 
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response);

MOBLE_RESULT LightModelClient_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

MOBLE_RESULT LightClient_Lightness_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Light_LightnessParam *pLightness_param, 
                                     MOBLEUINT32 length);
MOBLE_RESULT Light_Client_Lightness_Status(MOBLEUINT8 const *pLightness_status, MOBLEUINT32 plength);

#endif /* __LIGHT_CLIENT_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

