/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWM_handlers.h
  * @author  MCD Application Team
  * @brief   Header Handlers for PWM and other support functions.
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
	
#include "PWM_config.h"
//#include "SDK_EVAL_Config.h"	
	
/* PWM2 handler */
void PWM2_handler(void);
 
/* PWM3 and PWM4 handler */
void PWM3_PWM4_handler(void);

/* Here the channel with the lower duty-cycle is established*/
void Ton_sorting(void);

/* Handle modifications in duty cycle */
void Modify_PWM(uint8_t PWM_ID, uint16_t duty_cycle);
