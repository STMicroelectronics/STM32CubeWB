/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    standby.h
  * @author  MCD Application Team
  * @brief   Header for standby.c module (specific code for standby low power mode)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef STANDBY_H
#define STANDBY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported variables --------------------------------------------------------*/
extern uint32_t STBY_BootStatus;
extern uint16_t STBY_DebugGpioaPinList;
extern uint16_t STBY_DebugGpiobPinList;
extern uint16_t STBY_DebugGpiocPinList;
extern uint16_t STBY_DtbGpioaPinList;
extern uint16_t STBY_DtbGpiobPinList;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
/**
  * @brief Save necessary system HW data before entering into standby mode.
  * @param none
  * @retval none
  */
void STBY_SysHwSave(void);

/**
  * @brief Save application HW data before entering into standby mode.
  * @param none
  * @retval none
  */
void STBY_AppHwSave(void);

/**
  * @brief Restore necessary system HW data when exiting standby mode.
  * @param none
  * @retval none
  */
void STBY_SysHwRestore(void);

/**
  * @brief Restore application HW data when exiting standby mode.
  * @param none
  * @retval none
  */
void STBY_AppHwRestore(void);

/**
  * @brief  Return the standby boot to perform.
  * @param  None
  * @retval 1 if system can be restored after standby, else 0.
  */
uint32_t STBY_BootManager(void);

/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STANDBY_H */
