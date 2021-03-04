/**
******************************************************************************
* @file    appli_config.h
* @author  BLE Mesh Team
* @version V1.13.000
* @date    20-10-2020
* @brief   Header file for the application config file 
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
#ifndef __APPLI_CONFIG_H
#define __APPLI_CONFIG_H

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

#endif /* __APPLI_CONFIG_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
