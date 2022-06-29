/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWM_handlers.c
  * @author  MCD Application Team
  * @brief   Handlers for PWM and other support functions.
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
#include "app_conf.h"
#include "PWM_handlers.h"
#include "mesh_cfg_usr.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

/* Global Variables */
uint16_t DUTY;
int NonZeroPWM0=0;
int NonZeroPWM1=0;
uint8_t Duty_flag_A = 0;
uint16_t PWM0_on, 
	 PWM1_on, 
	 PWM2_on = MFT1_TON_2,  /* Timer2, MFT1 */
	 PWM3_on = MFT2_TON_2,  /* Timer2, MFT2 */
	 PWM4_on = MFT2_TON_3;  /* Timer2, MFT2 */

uint32_t channel = PWM3_PIN;    /* It is the channel with initial lowest duty-cycle */
uint32_t period = TIME_PERIOD;  /* Period in ticks of the PWM signal */
uint8_t channel_number = 0;

uint32_t PWM_channels[] = {PWM3_PIN, PWM4_PIN};  /* Channels managed by MFT2 Timer 2 */                                                    
uint16_t Ton_values[] = {MFT2_TON_2, MFT2_TON_3}; /* Sorted array containing duty-cycle values
                                                    respectively for PWM3_PIN and PWM4_PIN */
uint16_t delta_1 = MFT2_TON_3 - MFT2_TON_2; /* Delta between the first two duty-cycles */
uint16_t delta = MFT2_TON_3 - MFT2_TON_2;

int number_of_channels = 2;  /* Number of channels managed by MFT2 Timer 2 */
int max_duty = MFT2_TON_3;  /* Maximum duty-cycle */

/**
  *@brief  PWM2 handler
  *@retval None
  */
void PWM2_handler() 
{
  if(__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1) > 0)
  {
    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC1); 
    if(Duty_flag_A == 1)
    {
      HAL_GPIO_WritePin(GPIOA, PWM2_PIN, GPIO_PIN_SET);
      Duty_flag_A = 0;
      htim1.Instance->CCR1 = PWM2_on;
    }
    else               
    {
      HAL_GPIO_WritePin(GPIOA, PWM2_PIN, GPIO_PIN_RESET);
      Duty_flag_A = 1;
      htim1.Instance->CCR1 = TIME_PERIOD - PWM2_on;
    }
  }
}

/**
  *@brief  PWM3 and PWM4 handler
  *@retval None
  */
void PWM3_PWM4_handler()
{
  /*STATE 0
  if delta != 0, there is a channel with a duty lower than the other one.
  So the channel with the lower duty is cleared. Delta is period - max_duty
  (the duty of the other channel)
  The machine passes to STATE 1.

  if delta == 0, the two channel are cleared together.
  The machine passes to STATE 2

  The delta is computed for the next timer event*/

  if (channel_number < number_of_channels - 1)
  { 
    if (delta != 0)
    {
//      HAL_GPIO_WritePin(GPIOA, channel, GPIO_PIN_RESET);
      htim2.Instance->CCR2 = delta - 1;
      htim2.Instance->CCR3 = delta - 1;
      channel_number ++;
      delta = period - max_duty; /*computed delta for the next reset of the counter*/
      channel = PWM_channels[channel_number];
    }
    else
    {
//      HAL_GPIO_WritePin(GPIOA, PWM3_PIN |PWM4_PIN, GPIO_PIN_RESET);
      delta = period - max_duty;
      htim2.Instance->CCR2 = delta - 1;
      htim2.Instance->CCR3 = delta - 1;
      channel_number = number_of_channels;
    }
  }
  /*STATE 1
  Here the channel with the max duty-cycle is cleared. The delta is that one 
  computed in STATE 0. The machine passes to STATE 2 */
  else if (channel_number == number_of_channels - 1)
  {
//    HAL_GPIO_WritePin(GPIOA, channel, GPIO_PIN_RESET);
    htim2.Instance->CCR2 = delta - 1;
    htim2.Instance->CCR3 = delta - 1;
    channel_number ++;
  }     
  /*STATE 2
  All the channels have been cleared and now are set again. The machine passes to STATE 0 */
  else
  { /* channel_number > number_of_channels - 1 */
    HAL_GPIO_WritePin(GPIOA, PWM3_PIN | PWM4_PIN, GPIO_PIN_SET);
    htim2.Instance->CCR2 = Ton_values[0] - 1;
    htim2.Instance->CCR3 = Ton_values[1] - 1;
    Ton_sorting();
    channel_number = 0;
    delta = Ton_values[1] - Ton_values[0];
    channel = PWM_channels[0];
  } 
}

/**
  *@brief  Sorting on basis of duty cycle
  *@retval None
  */

void Ton_sorting(void)
{
  if (PWM3_on > PWM4_on)
  {
    max_duty = PWM3_on; 
    Ton_values[1] = PWM3_on;
    Ton_values[0] = PWM4_on;
    PWM_channels[1] = PWM3_PIN;
    PWM_channels[0] = PWM4_PIN;
  }
  else
  {
    max_duty = PWM4_on;
    Ton_values[0] = PWM3_on;
    Ton_values[1] = PWM4_on;
    PWM_channels[0] = PWM3_PIN;
    PWM_channels[1] = PWM4_PIN;  
  }
}


/**
  *@brief  PWM modification
  *@param  PWM_ID: PWM number
  *@param  duty_cycle: Duty cycle at output 
  *@retval None
  */
void Modify_PWM(uint8_t PWM_ID, uint16_t duty_cycle) 
{  
#if (( CFG_LPM_SUPPORTED == 0) && (ENABLE_PWM_SUPPORT == 1))
  GPIO_InitTypeDef GPIO_InitStructure1 = {0};
   
  if (PWM_ID == 0)     /* PD14 */
  {
    if (duty_cycle == 0 && NonZeroPWM0 == 0)   
    {
      GPIO_InitStructure1.Pin       = PWM0_PIN;
      GPIO_InitStructure1.Mode      = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure1.Pull      = GPIO_NOPULL;
      GPIO_InitStructure1.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStructure1);
      HAL_GPIO_WritePin(GPIOD, PWM0_PIN, GPIO_PIN_RESET);

      NonZeroPWM0=1;
    }
    else if ((duty_cycle != 0 && NonZeroPWM0 == 1))
    {
      GPIO_InitStructure1.Pin       = PWM0_PIN;
      GPIO_InitStructure1.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStructure1.Pull      = GPIO_PULLUP;
      GPIO_InitStructure1.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStructure1.Alternate = GPIO_AF1_TIM1;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStructure1);
      
      NonZeroPWM0=0;
    }
  }
  else if((PWM_ID == 1))   /* GPIO3 */
  {
    if (duty_cycle == 0 && NonZeroPWM1 == 0)
    {
      GPIO_InitStructure1.Pin       = PWM1_PIN;
      GPIO_InitStructure1.Mode      = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure1.Pull      = GPIO_NOPULL;
      GPIO_InitStructure1.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStructure1);
      HAL_GPIO_WritePin(GPIOD, PWM1_PIN, GPIO_PIN_RESET);
      
      NonZeroPWM1=1;
    }
    else if ((duty_cycle != 0 && NonZeroPWM1 == 1))
    {
      GPIO_InitStructure1.Pin       = PWM1_PIN;
      GPIO_InitStructure1.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStructure1.Pull      = GPIO_PULLUP;
      GPIO_InitStructure1.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStructure1.Alternate = GPIO_AF1_TIM1;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStructure1);
      
      NonZeroPWM1=0;
    }
  }
  else
  {
  }
  
  DUTY = duty_cycle;
  switch (PWM_ID) 
  {
    case 0: /* PWM0 */ 
      {                  
        htim1.Instance->CCR1 = DUTY;
      }
      break;
    case 1: /* PWM1 */
      {
        htim1.Instance->CCR2 = DUTY;
      }
      break;
    case 2: /* PWM2 */
      {
        PWM2_on = DUTY;
        htim2.Instance->CCR1 = PWM2_on;
      }
      break;
    case 3: /* PWM3 */
      {
        PWM3_on = DUTY;
        htim2.Instance->CCR2 = PWM3_on;
      }
      break;
    case 4: /* PWM4 */
      {
        PWM4_on = DUTY;
        htim2.Instance->CCR3 = PWM4_on;
      }
      break;
  }
#endif
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
#if 0
  if(htim->Instance == TIM1)
    PWM2_handler();
  else
    PWM3_PWM4_handler();
#endif
}

