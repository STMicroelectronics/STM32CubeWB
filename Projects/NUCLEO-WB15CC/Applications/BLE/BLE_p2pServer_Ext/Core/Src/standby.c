/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    standby.c
  * @author  MCD Application Team
  * @brief   Specific code for standby low power mode
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "standby.h"

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
uint32_t STBY_BackupMSP;
uint32_t STBY_BackupIpccC1MR;
uint32_t STBY_BackupVTOR;

uint32_t STBY_BootStatus;
uint16_t STBY_DebugGpioaPinList = UINT16_MAX;
uint16_t STBY_DebugGpiobPinList = UINT16_MAX;
uint16_t STBY_DebugGpiocPinList = UINT16_MAX;
uint16_t STBY_DtbGpioaPinList = UINT16_MAX;
uint16_t STBY_DtbGpiobPinList = UINT16_MAX;

extern RTC_HandleTypeDef hrtc;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void STBY_TriggerIT(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*******************************************************************************
 * This part may be updated by the user
 ******************************************************************************/

void STBY_AppHwSave(void)
{
/* In this function HW parameters lost in standby can de saved.
   All RAM memory is in retention */
  /* USER CODE BEGIN STBY_AppHwSave_1 */

  /* USER CODE END STBY_AppHwSave_1 */
  return;
}

void STBY_AppHwRestore(void)
{
  /* USER CODE BEGIN STBY_AppHwRestore_1 */

  /* USER CODE END STBY_AppHwRestore_1 */

  HAL_Init();
  HAL_ResumeTick();

  /* In this user section add MX init functions present in main.c, except MX_RTC_Init() */
  /* USER CODE BEGIN STBY_AppHwRestore_2 */

  MX_GPIO_Init();
  
  APPE_Led_Init();
  APPE_Button_Init();
  /* USER CODE END STBY_AppHwRestore_2 */

  STBY_TriggerIT();

  return;
}

/*******************************************************************************
 * Do not update code from this limit.
 ******************************************************************************/

void STBY_SysHwSave(void)
{
  STBY_BackupIpccC1MR = READ_REG(IPCC->C1MR);
  STBY_BackupVTOR = SCB->VTOR;

  /* USER CODE BEGIN STBY_SysHwSave_1 */

  /* USER CODE END STBY_SysHwSave_1 */
  return;
}

void STBY_SysHwRestore(void)
{
  /* USER CODE BEGIN STBY_SysHwRestore_1 */

  /* USER CODE END STBY_SysHwRestore_1 */
  APPD_Init();

  Init_Smps();
  Init_Exti();
  HAL_Init();

  WRITE_REG(SCB->VTOR, STBY_BackupVTOR);

  HW_IPCC_Init();
  WRITE_REG(IPCC->C1MR, STBY_BackupIpccC1MR);

  if( !LL_HSEM_1StepLock( HSEM, CFG_HW_RCC_SEMID ) )
  {
    LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);
    LL_HSEM_ReleaseLock( HSEM, CFG_HW_RCC_SEMID, 0 );
  }
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_PWR_STANDBY_SEMID, 0 );

  HW_TS_Init(hw_ts_InitMode_Limited, &hrtc);

  LL_PWR_EnableSRAM2Retention();

  /* USER CODE BEGIN STBY_SysHwRestore_2 */

  /* USER CODE END STBY_SysHwRestore_2 */

  return;
}

uint32_t STBY_BootManager(void)
{
#if ( CFG_LPM_STANDBY_SUPPORTED != 0 )
  __HAL_RCC_HSEM_CLK_ENABLE();
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_PWR_STANDBY_SEMID ) );

  /* If a reset other than standby occurs, perform a full reset cycle */
  if( (LL_RCC_IsActiveFlag_IWDGRST() != FALSE) ||
      (LL_RCC_IsActiveFlag_WWDGRST() != FALSE) ||
      (LL_RCC_IsActiveFlag_BORRST()  != FALSE) ||
      (LL_RCC_IsActiveFlag_LPWRRST() != FALSE) ||
      (LL_RCC_IsActiveFlag_OBLRST()  != FALSE) ||
      (LL_RCC_IsActiveFlag_PINRST()  != FALSE) ||
      (LL_RCC_IsActiveFlag_SFTRST()  != FALSE) )
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
  }

  if( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET )
  {
    __disable_irq( );

    STBY_BootStatus = 1;
  }else{
    STBY_BootStatus = 0;
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_C2SB);
    LL_HSEM_ReleaseLock( HSEM, CFG_HW_PWR_STANDBY_SEMID, 0 );
  }
#else
  STBY_BootStatus = 0;
#endif

  return STBY_BootStatus;
}

static void STBY_TriggerIT(void)
{
  /* If system has been woken-up by an RTC alarm,
   * a software event must be generated to trigger the IT.
   * NVIC and EXTI must be reconfigured properly before triggering the event.
   */
  if((__HAL_RTC_ALARM_GET_FLAG((RTC_HandleTypeDef *)&hrtc, RTC_FLAG_ALRAF) != 0) ||
     (__HAL_RTC_ALARM_GET_FLAG((RTC_HandleTypeDef *)&hrtc, RTC_FLAG_ALRBF) != 0) )
  {
    __HAL_RTC_ALARM_EXTI_GENERATE_SWIT();
  }

  /* If system has been woken-up by a wake up pin,
   * a software event must be generated to trigger the IT.
   * NVIC and EXTI must be reconfigured properly before triggering the event.
   */
  if(HAL_PWREx_GetWakeupFlag(PWR_FLAG_WUF1) != 0)
  {
    if( (LL_SYSCFG_GetEXTISource(LL_SYSCFG_EXTI_LINE0) == LL_SYSCFG_EXTI_PORTA) )
    {
      if(((READ_BIT(PWR->CR4, PWR_CR4_WP1) >> PWR_CR4_WP1_Pos) == 0) &&
         (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_0) == 1) )
      {
        LL_EXTI_GenerateSWI_0_31(LL_EXTI_LINE_0);
      }
      if(((READ_BIT(PWR->CR4, PWR_CR4_WP1) >> PWR_CR4_WP1_Pos) == 1) &&
         (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_0) == 1) )
      {
        LL_EXTI_GenerateSWI_0_31(LL_EXTI_LINE_0);
      }
    }
    HAL_PWREx_ClearWakeupFlag(PWR_FLAG_WUF1);
  }

  if(HAL_PWREx_GetWakeupFlag(PWR_FLAG_WUF4) != 0)
  {
    if( (LL_SYSCFG_GetEXTISource(LL_SYSCFG_EXTI_LINE2) == LL_SYSCFG_EXTI_PORTA) )
    {
      if(((READ_BIT(PWR->CR4, PWR_CR4_WP4) >> PWR_CR4_WP4_Pos) == 0) &&
         (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_2) == 1) )
      {
        LL_EXTI_GenerateSWI_0_31(LL_EXTI_LINE_2);
      }
      if(((READ_BIT(PWR->CR4, PWR_CR4_WP4) >> PWR_CR4_WP4_Pos) == 1) &&
         (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_2) == 1) )
      {
        LL_EXTI_GenerateSWI_0_31(LL_EXTI_LINE_2);
      }
    }
    HAL_PWREx_ClearWakeupFlag(PWR_FLAG_WUF4);
  }

  return;
}
