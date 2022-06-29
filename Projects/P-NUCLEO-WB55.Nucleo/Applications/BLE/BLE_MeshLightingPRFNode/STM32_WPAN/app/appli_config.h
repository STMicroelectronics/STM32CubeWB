/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_config.h
  * @author  MCD Application Team
  * @brief   Header file for the application config file 
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
#ifndef APPLI_CONFIG_H
#define APPLI_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "mesh_cfg.h"
#include "mesh_cfg_usr.h"
#include "sensors.h"
#include "common.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */

/* Exported Functions Prototypes ---------------------------------------------*/
void Appli_GetAppKeyAddParamsCb(model_securityKeyParams_t*);
void Appli_GetAppKeyDeleteParamsCb(model_securityKeyParams_t*);
void Appli_GetAppKeyListParamsCb(model_appKeyListParams_t*);
void Appli_GetAppKeyUpdateParamsCb(model_securityKeyParams_t*);
void Appli_GetNetKeyAddParamsCb(model_securityKeyParams_t*);
void Appli_GetNetKeyDeleteParamsCb(model_securityKeyParams_t*);
void Appli_GetNetKeyListParamsCb(model_netKeyListParams_t*);
void Appli_GetNetKeyUpdateParamsCb(model_securityKeyParams_t*);
void Appli_GetAppKeyBindingParamsCb(model_appKeyBindingParams_t*);
void Appli_GetAppKeyUnBindingParamsCb(model_appKeyBindingParams_t*);
void Appli_GetSubAddParamsCb(model_subParams_t*);
void Appli_GetSubDeleteParamsCb(model_subParams_t*);
void Appli_GetSubOverwriteParamsCb(model_subParams_t*);
void Appli_GetPublicationSetParamsCb(model_publicationparams_t*);
void Appli_GetPublicationGetParamsCb(model_publicationparams_t*);

#endif /* APPLI_CONFIG_H */

