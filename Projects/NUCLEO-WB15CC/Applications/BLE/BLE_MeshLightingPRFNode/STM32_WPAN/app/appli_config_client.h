/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_config_client.h
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
#ifndef APPLI_CONFIG_CLIENT_H
#define APPLI_CONFIG_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "generic.h"
#include "mesh_cfg.h"
#include "config_client.h"
#include "mesh_cfg_usr.h"

/* Exported macro ------------------------------------------------------------*/
#define MAX_CONFIG_PARAM_SIZE 22
#define CONFIGURE_AS_PER_COMPOSITION_DATA

/* Exported variables  ------------------------------------------------------- */
/* Application Variable-------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/

#if defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER)

MOBLE_RESULT Appli_ConfigClient_Init(void);
MOBLE_RESULT Appli_ConfigClient_Process(void);
MOBLE_RESULT Appli_ConfigClient_ConfigureNode(void);
void Appli_CompositionDataStatusCb(MOBLE_RESULT); 
void Appli_AppKeyStatusCb(MOBLEUINT8 status);
void Appli_AppBindModelStatusCb(MOBLEUINT8 status);
void Appli_PublicationStatusCb(MOBLEUINT8 status);
void Appli_SubscriptionAddStatusCb(MOBLEUINT8 status);
void Appli_NodeResetStatusCb(void);

void Appli_ConfigClientStartNodeConfiguration(MOBLEUINT8 prvState);
MOBLE_RESULT Appli_ConfigClient_GetCompositionData (void);
MOBLE_RESULT Appli_ConfigClient_DefaultAppKeyAdd (void);
MOBLE_RESULT Appli_ConfigClient_DefaultAppKeyBind (void);
MOBLE_RESULT AppliConfigClient_SubscriptionAddDefault (void);
MOBLE_RESULT AppliConfigClient_PublicationSetDefault (void);
MOBLE_RESULT AppliConfigClient_SelfPublicationSetDefault (void); 
MOBLE_RESULT AppliConfigClient_SelfSubscriptionSetDefault (void);
MOBLE_RESULT Appli_ConfigClient_SelfDefaultAppKeyBind (void);
Composition_Data_Page0_t* Appli_GetNodeCompositionBuff (void);

MOBLEUINT16 GetSIGModelToBindApp(MOBLEUINT8 elementIndex, 
                                 MOBLEUINT8 *pModelIndex, 
                                 MOBLEUINT8 numberOfModels); 

MOBLEUINT32 GetVendorModelToBindApp(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG);
MOBLEUINT8 GetCountSIGModelToBindApp(MOBLEUINT8 elementIdx);
MOBLEUINT8 GetCountVendorModelToBindApp(MOBLEUINT8 elementIdx);

MOBLEUINT16 GetSIGModelToSubscribe(MOBLEUINT8 elementIndex, 
                                 MOBLEUINT8 *pModelIndex, 
                                 MOBLEUINT8 numberOfModels); 


MOBLEUINT32 GetVendorModelToSubscribe(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG);
MOBLEUINT8 GetCountSIGModelToSubscribe(MOBLEUINT8);
MOBLEUINT8 GetCountVendorModelToSubscribe(MOBLEUINT8);

MOBLEUINT16 GetSIGModelToPublish(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pModelIndex, 
                                     MOBLEUINT8 numberOfModels);

MOBLEUINT32 GetVendorModelToPublish(MOBLEUINT8 elementIdx, MOBLEUINT8 idxSIG);
MOBLEUINT8 GetCountSIGModelToPublish(MOBLEUINT8 elementIdx);

MOBLEUINT8 GetCountVendorModelToPublish(MOBLEUINT8 elementIdx);

#endif /* defined (ENABLE_PROVISIONER_FEATURE) || defined(DYNAMIC_PROVISIONER) */

#endif /* APPLI_CONFIG_CLIENT_H */


