/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : standby_stm32wb15.c
  * Description        : Application configuration file for STM32WPAN Middleware.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_entry.h"
#include "app_common.h"
#include "app_debug.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint32_t backup_MSP;
uint32_t backup_IPCC_C1MR;
uint32_t boot_after_standby;
extern RTC_HandleTypeDef hrtc;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
uint32_t standby_boot_mng(void);
void standby_hw_save(void);
void standby_hw_restore(void);
/* USER CODE BEGIN PFP */
void SystemClock_Config(void); // may be declared in main.h file ???
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*******************************************************************************
 * This part may be updated by the user
 ******************************************************************************/

  /**
  * @brief  standby_hw_save function, saves hardware context to restore
  * @param  None
  * @retval None
  */
void standby_hw_save(void)
{
  backup_IPCC_C1MR = READ_REG(IPCC->C1MR);

  /* USER CODE BEGIN standby_hw_save */

  /* USER CODE END standby_hw_save */
  return;
}

  /**
  * @brief  standby_hw_restore function, restore and reconfigure hardware context
  * @param  None
  * @retval None
  */
void standby_hw_restore(void)
{
  /* USER CODE BEGIN standby_hw_restore_1 */

  /* USER CODE END standby_hw_restore_1 */

  APPD_Init();

  SystemClock_Config();

  HAL_Init();

  /* In this user section add MX init functions present in main.c , except MX_RTC_Init() */
  /* USER CODE BEGIN standby_hw_restore_2 */
  
  Init_Exti();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  
  /* USER CODE END standby_hw_restore_2 */

  HW_IPCC_Init();
  HW_IPCC_Enable();
  WRITE_REG(IPCC->C1MR, backup_IPCC_C1MR);

  HW_TS_Init(hw_ts_InitMode_Limited, &hrtc);

  LL_PWR_EnableSRAM2Retention();

  /* USER CODE BEGIN standby_hw_restore_3 */
  APPE_Led_Init();
  APPE_Button_Init();
  /* USER CODE END standby_hw_restore_3 */

  return;
}

/*******************************************************************************
 * Do not update code from this limit.
 ******************************************************************************/

  /**
  * @brief  standby_boot_mng function, will restore MCU context if wakeup from standby
  * @param  None
  * @retval None
  */
uint32_t standby_boot_mng(void)
{
#if ( CFG_LPM_STANDBY_SUPPORTED != 0 )
  if( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET )
  {
    __disable_irq( );

    boot_after_standby = 1;
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
  }else{
    boot_after_standby = 0;
  }
#else
  boot_after_standby = 0;
#endif

  return boot_after_standby;
}

