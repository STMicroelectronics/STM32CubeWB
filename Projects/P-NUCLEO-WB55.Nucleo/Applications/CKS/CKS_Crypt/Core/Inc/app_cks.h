/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_cks.h
  * @author  MCD Application Team
  * @brief   Header for cks application
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
