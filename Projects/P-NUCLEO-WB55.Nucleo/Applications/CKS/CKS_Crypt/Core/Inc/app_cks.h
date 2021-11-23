/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_cks.h
  * @author  MCD Application Team
  * @brief   Header for cks application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
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
#ifndef APP_CKS_H
#define APP_CKS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "shci_tl.h"
#include "shci.h"

/* Exported functions ---------------------------------------------*/
HAL_StatusTypeDef CKS_Test (void);


#ifdef __cplusplus
}
#endif

#endif /*APP_CKS_H */
