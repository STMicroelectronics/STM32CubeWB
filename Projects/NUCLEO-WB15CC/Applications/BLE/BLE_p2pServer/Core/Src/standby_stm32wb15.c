/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : standby_stm32wb15.c
  * Description        : Application configuration file for STM32WPAN Middleware.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
  Init_Exti();
  Init_Smps();
  HAL_Init();

  HW_IPCC_Init();
  HW_IPCC_Enable();
  WRITE_REG(IPCC->C1MR, backup_IPCC_C1MR);

  if( !LL_HSEM_1StepLock( HSEM, CFG_HW_RCC_SEMID ) )
  {
    LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);
    LL_HSEM_ReleaseLock( HSEM, CFG_HW_RCC_SEMID, 0 );
  }
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_PWR_STANDBY_SEMID, 0 );

  /* In this user section add MX init functions present in main.c , except MX_RTC_Init() */
  /* USER CODE BEGIN standby_hw_restore_2 */
  
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE END standby_hw_restore_2 */

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
  __HAL_RCC_HSEM_CLK_ENABLE();
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_PWR_STANDBY_SEMID ) );

  if( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET )
  {
    __disable_irq( );

    boot_after_standby = 1;
  }else{
    boot_after_standby = 0;

    LL_HSEM_ReleaseLock( HSEM, CFG_HW_PWR_STANDBY_SEMID, 0 );
  }
#else
  boot_after_standby = 0;
#endif

  return boot_after_standby;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
