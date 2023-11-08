/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWM_config.c
  * @author  MCD Application Team
  * @brief   Configuration file for PWM.
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

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "PWM_config.h"
 
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define PRESCALER_VALUE            (uint32_t)(((SystemCoreClock) / 1000000) - 1)

/**
  *@brief  Configure PWM sources and GPIO
  *@retval None
  */
void PWM_Init()
{
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  
  /* GPIO Configuration */
  GPIO_InitTypeDef GPIO_InitStructure = {0};
   

  /* Configure PWM pins */
  GPIO_InitStructure.Pin       = PWM0_PIN | PWM1_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* Configure PWM pins */
  GPIO_InitStructure.Pin       = PWM2_PIN | PWM3_PIN | PWM4_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* MFT Configuration */
  HAL_NVIC_SetPriority((IRQn_Type)(TIM1_CC_IRQn), 15, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(TIM1_CC_IRQn));
  HAL_NVIC_SetPriority((IRQn_Type)(TIM2_IRQn), 15, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(TIM2_IRQn));

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0/*PRESCALER_VALUE*/;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = TIME_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&htim1);

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0/*PRESCALER_VALUE*/;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIME_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterPeripheralMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.Pulse = MFT1_TON_1;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  sConfigOC.Pulse = MFT2_TON_1;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
  sConfigOC.Pulse = MFT1_TON_2;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
  sConfigOC.Pulse = MFT2_TON_2;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
  sConfigOC.Pulse = MFT2_TON_3;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);

  /* Start channel 1 */
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
  /* Start channel 2 */
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
  /* Start channel 1 */
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
  /* Start channel 2 */
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
  /* Start channel 3 */
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);
}
