/**
  ******************************************************************************
  * @file    PWM/PWM_handlers.c
* @author  SNM Application Team
* @version V1.0.0
* @date    September-2015
  * @brief   Handlers for PWM and other support functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */
	
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
