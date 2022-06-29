/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWM_config.h
  * @author  MCD Application Team
  * @brief   Header Configuration file for PWM.
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

#include "mesh_cfg_usr.h"

/* Macros defined for the number of PWM Provided */
#define PWM0   0 
#define PWM1   1  
#define PWM2   2  
#define PWM3   3
#define PWM4   4 

/************************************************************************/

/*STM32WB
 * This configuration given below is for the STM32WB board.
 * Here user has selected the PWM0 in SERIAL2_MODE and PWM1 in SERIAL1_MODE.
 * User can configure software PWM to any of the GPIO which are not used for 
 * hardware PWM.
*/
/* PWM pins for MFT1 and MFT2 respectively */
#if defined USE_STM32WBXX_NUCLEO || defined USE_STM32WBXX_USB_DONGLE

#define PWM0_PIN			GPIO_PIN_14 /* TIM1_CH1 AF1 */
#define PWM1_PIN			GPIO_PIN_15 /* TIM1_CH2 AF1 */

/* GPIOs pins (SOFTWARE PWM) used to output a PWM signal */
#define PWM2_PIN                        GPIO_PIN_0 /* TIM2_CH1 AF1 */
#define PWM3_PIN                        GPIO_PIN_1 /* TIM2_CH2 AF1 */
#define PWM4_PIN                        GPIO_PIN_2 /* TIM2_CH3 AF1 */
#endif
/************************************************************************/

#define TIME_PERIOD 32000

/* TIM1 Channel 1 Timer Ton and T0ff (ticks number) PWM0*/
#define MFT1_TON_1  (12000 - 1)
#define MFT1_TOFF_1 (20000 - 1)

/* TIM1 Channel 2 Timer 1 Ton and T0ff (ticks number) PWM1*/
#define MFT2_TON_1 (8000 - 1 )
#define MFT2_TOFF_1 (24000 - 1)

/* TIM2 Channel 1 Timer 2 Ton and T0ff (ticks number) PWM2*/
#define MFT1_TON_2 (16000 - 1)
#define MFT1_TOFF_2 (16000 -1)

/* TIM2 Channel 2 Timer 2 Ton and T0ff (ticks number) PWM3*/
#define MFT2_TON_2 (4000 - 1)
#define MFT2_TOFF_2 (28000 - 1)

/* TIM2 Channel 3 Timer 2 Ton and T0ff (ticks number) PWM4*/
#define MFT2_TON_3 (4000 - 1)
#define MFT2_TOFF_3 (28000 - 1)

void PWM_Init(void);

