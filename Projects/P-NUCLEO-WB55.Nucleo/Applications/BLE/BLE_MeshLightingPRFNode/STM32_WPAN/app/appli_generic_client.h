/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_generic_client.h
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
#ifndef APPLI_GENERIC_CLIENT_H
#define APPLI_GENERIC_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "generic.h"
#include "mesh_cfg.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
/* Application Variable-------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT Appli_ConfigClient_Set(void); 
MOBLE_RESULT Appli_GenericClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params);

#endif /* APPLI_GENERIC_CLIENT_H */


