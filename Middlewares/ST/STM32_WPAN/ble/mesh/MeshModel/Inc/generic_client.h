/**
******************************************************************************
* @file    generic_client.h
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
#ifndef __GENERIC_CLIENT_H
#define __GENERIC_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/
/* Variable-------------------------------------------------------------------*/
#pragma pack(1)
typedef union  
{
  Generic_OnOffParam_t sGeneric_OnOffParam;
  MOBLEUINT8 a_OnOff_param[sizeof(Generic_OnOffParam_t)]; 
} _Generic_OnOffParam;

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/


/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT GenericModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT GenericModelClient_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                   MOBLE_ADDRESS dst_peer, 
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response);

MOBLE_RESULT GenericModelClient_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

MOBLE_RESULT GenericClient_OnOff_Set(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
                                     MOBLEUINT32 length); 

MOBLE_RESULT GenericClient_OnOff_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
                                     MOBLEUINT32 length); 
MOBLE_RESULT GenericClient_Level_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_LevelParam *plevel_param, 
                                     MOBLEUINT32 length);


MOBLE_RESULT Generic_Client_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_Level_Status(MOBLEUINT8 const *plevel_status, MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength);

#endif /* __GENERIC_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

