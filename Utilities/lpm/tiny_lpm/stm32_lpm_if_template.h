/**
 ******************************************************************************
 * @file    stm32_lpm_if.h
 * @brief   Header for stm32_lpm_f.c module (device specific LP management)
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_TINY_LPM_IF_H
#define STM32_TINY_LPM_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @defgroup TINY_LPM_IF TINY LPM IF
  * @{
  */

/* Exported Functions ------------------------------------------------------------------*/

/** @defgroup TINY_LPM_IF_Exported_functions TINY LPM IF Exported functions
 * @{
 */

/**
  * @brief Enters Low Power Off Mode
  */
void PWR_EnterOffMode( void );
/**
  * @brief Exits Low Power Off Mode
  */
void PWR_ExitOffMode( void );

/**
  * @brief Enters Low Power Stop Mode
  */
void PWR_EnterStopMode( void );
/**
  * @brief Exits Low Power Stop Mode
  */
void PWR_ExitStopMode( void );

/**
  * @brief Enters Low Power Sleep Mode
  */
void PWR_EnterSleepMode( void );

/**
  * @brief Exits Low Power Sleep Mode
  */
void PWR_ExitSleepMode( void );

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* STM32_TINY_LPM_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
