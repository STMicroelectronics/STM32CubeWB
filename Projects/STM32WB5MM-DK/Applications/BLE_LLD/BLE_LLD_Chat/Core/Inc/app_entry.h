/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_entry.h
  * @author  MCD Application Team
  * @brief   Interface to the application
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
#ifndef APP_ENTRY_H
#define APP_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32wb5mm_dk.h"
#include "stm32wb5mm_dk_bus.h"
#include "stm32wb5mm_dk_lcd.h"
#include "stm32_lcd.h"
/* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
  void APPE_Init( void );
/* USER CODE BEGIN EF */
  void LED_Deinit(void);
  void LED_On(aPwmLedGsData_TypeDef aPwmLedGsData);
  void LED_Off(void);
/* USER CODE END EF */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*APP_ENTRY_H */
