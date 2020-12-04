 /*******************************************************************************
  * @file    stm32_lpm_if.c
  * @author  MCD Application Team
  * @brief   Low layer function to enter/exit low power modes (stop, sleep)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/  
#include "stm32_lpm_if.h"
#include "stm32_lpm.h"

/** @addtogroup TINY_LPM_IF
  * @{
  */

/* USER CODE BEGIN include */

/* USER CODE END include */

/* Exported variables --------------------------------------------------------*/
/** @defgroup TINY_LPM_IF_Exported_varaibles TINY LPM IF exported variables
  * @{
  */

/**
 * @brief variable to provide all the functions corresponding to the different low power modes.
 */
const struct UTIL_LPM_Driver_s UTIL_PowerDriver = 
{
  PWR_EnterSleepMode,
  PWR_ExitSleepMode,
  
  PWR_EnterStopMode,
  PWR_ExitStopMode, 
  
  PWR_EnterOffMode,
  PWR_ExitOffMode,
};

/**
 * @}
 */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN Private_Function_Prototypes */

/* USER CODE END Private_Function_Prototypes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Private_Variables */

/* USER CODE END Private_Variables */

/** @addtogroup TINY_LPM_IF_Exported_functions
 * @{
 */

void PWR_EnterOffMode( void )
{
/* USER CODE BEGIN PWR_EnterOffMode */

/* USER CODE END PWR_EnterOffMode */
}

void PWR_ExitOffMode( void )
{
/* USER CODE BEGIN PWR_ExitOffMode */

/* USER CODE END PWR_ExitOffMode */
}

void PWR_EnterStopMode( void )
{
/* USER CODE BEGIN PWR_EnterStopMode */

/* USER CODE END PWR_EnterStopMode */
}

void PWR_ExitStopMode( void )
{
/* USER CODE BEGIN PWR_ExitStopMode */

/* USER CODE END PWR_ExitStopMode */
}

void PWR_EnterSleepMode( void )
{
/* USER CODE BEGIN PWR_EnterSleepMode */

/* USER CODE END PWR_EnterSleepMode */
}

void PWR_ExitSleepMode( void )
{
/* USER CODE BEGIN PWR_ExitSleepMode */

/* USER CODE END PWR_ExitSleepMode */
}

/* USER CODE BEGIN Private_Functions */

/* USER CODE END Private_Functions */

/**
 * @}
 */

/**
 * @}
 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

