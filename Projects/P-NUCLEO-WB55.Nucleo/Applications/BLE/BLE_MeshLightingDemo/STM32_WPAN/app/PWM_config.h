/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : PWM/PWM_config.c
* Author             : SNM Application Team
* Version            : V1.0.0
* Date               : September-2015
* Description        : Configuration file for PWM
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

//#include "BlueNRG1_mft.h"
#include "mesh_cfg_usr.h"

/* Macros defined for the number of PWM Provided */
#define PWM0   0 
#define PWM1   1  
#define PWM2   2  
#define PWM3   3
#define PWM4   4 

/* Setting for the Hardware PWM selection for BlueNRG-1 & BlueNRG-2 board.
if user want to change the hardware pwm according to his application then user
can select the given settings.

               SERIAL1_MODE        SERIAL2_MODE
PWM0_PIN       GPIO_2              GPIO_4
PWM1_PIN       GPIO_3              GPIO_5
*/

/*STEVAL_BlueNRG-1 
 *STEVAL_BlueNRG-2
 * This configuration given below is for the BlueNRG-1 and BlueNRG-2 board.
 * Here user has selected the PWM0 in SERIAL2_MODE and PWM1 in SERIAL1_MODE.
 * User can configure software PWM to any of the GPIO which are not used for 
 * hardware PWM.
*/
/* PWM pins for MFT1 and MFT2 respectively */
#if defined STEVAL_BLUENRG_1_BOARD_PWM_SELECTION || defined STEVAL_BLUENRG_2_BOARD_PWM_SELECTION

#define PWM0_PIN			GPIO_Pin_4 
#define PWM1_PIN			GPIO_Pin_3 

/* GPIOs pins (SOFTWARE PWM) used to output a PWM signal */
#define PWM2_PIN                        GPIO_Pin_1 
#define PWM3_PIN                        GPIO_Pin_2
#define PWM4_PIN                        GPIO_Pin_14 /*Blue Led*/
#endif
/**************************************************************************/

/* CUSTOM BOARD SETTING 
 * Uncomment the macros given below for custom board and comment the above macros 
 * which are given for BlueNRG board.
 * This configuration given below is for the custom board.
 * Here user has selected the PWM0 in SERIAL2_MODE and PWM1 in SERIAL2_MODE.
 * User can configure software PWM to any of the GPIO which are not used for 
 * hardware PWM.
NOTE - Do not enable the sensor model with this configuration.As GPIO_4 is used
       for sensor already.
*/
/* PWM pins for MFT1 and MFT2 respectively */
#ifdef CUSTOM_BOARD_PWM_SELECTION 

#define PWM0_PIN			GPIO_Pin_4 
#define PWM1_PIN			GPIO_Pin_5 

/* GPIOs pins (SOFTWARE PWM) used to output a PWM signal */
#define PWM2_PIN                        GPIO_Pin_0 /* Red Led*/
#define PWM3_PIN                        GPIO_Pin_2 /*Green Led*/ 
#define PWM4_PIN                        GPIO_Pin_3 /*Blue Led*/

#endif
/*************************************************************************/

/*
 * STEVAL-BLUEMIC-1 
 * This configuration given below is for the BlueNRG-1 and BlueNRG-2 board.
 * Here user has selected the PWM0 in SERIAL2_MODE and PWM1 in SERIAL1_MODE.
 * User can configure software PWM to any of the GPIO which are not used for 
 * hardware PWM.
NOTE - Do not enable the sensor model with this configuration.As GPIO_4 is used
       for sensor already.
*/
/* PWM pins for MFT1 and MFT2 respectively */
#ifdef STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION

#define PWM0_PIN			GPIO_Pin_4 
#define PWM1_PIN			GPIO_Pin_3 //blue_led

/* GPIOs pins (SOFTWARE PWM) used to output a PWM signal */
#define PWM2_PIN                        GPIO_Pin_8 
#define PWM3_PIN                        GPIO_Pin_6 
#define PWM4_PIN                        GPIO_Pin_14
         
#endif
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

