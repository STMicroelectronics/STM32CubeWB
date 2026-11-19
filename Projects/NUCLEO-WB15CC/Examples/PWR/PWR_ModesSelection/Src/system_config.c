/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/system_config.c
  * @author  MCD Application Team
  * @brief   This file provides system configuration.
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
/* USER CODE BEGIN 1 */
/* Includes ------------------------------------------------------------------*/
#include "system_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static System_StatusTypeDef SystemClock_RunLow(void);
static System_StatusTypeDef RTC_Enable(void);
static System_StatusTypeDef RTC_Disable(void);

/* System clock helpers implemented in main.c, reused here to avoid
   duplicating the RCC configuration sequences. */
extern void SystemClock_Config(void);   /* Run - 64 MHz (PLL from MSI)         */
extern void SystemClock_Decrease(void); /* Sleep/Stop - ~2 MHz (MSI, PLL off)  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Optimize system power consumption configuration.
  * @param  None.
  * @retval System Status.
  */
void system_config(System_LowPowerModeScenarioTypeDef scenario)
{
  /* *** Common pre-setup for all modes *** */

  /* Enforcing true STOP and STANDBY modes, by disabling their emulation. */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

  /* Disable unused VDDs */
  HAL_PWREx_DisableBatteryCharging();

  if (scenario >= SYSTEM_LOWPOWER_S2)
  {
    if ((LL_PWR_IsActiveFlag_C1SB() == 0U) || (LL_PWR_IsActiveFlag_C2SB() == 0U))
    {
      LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);
    }
  }
}

/**
  * @brief  Configure rtc state.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_rtc_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (scenario)
  {
     /* Standby - 8kb SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S5:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }

    /* Standby - No SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S6:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }

    /* Shutdown w/ RTC */
    case SYSTEM_LOWPOWER_S7:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      break;
    }

    /* Disable RTC */
    default:
    {
      if (RTC_Disable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system clock frequency.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_clock_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (scenario)
  {
    /* Run - 64 MHz */
    case SYSTEM_LOWPOWER_S0:
    {
      SystemClock_Config();
      break;
    }

    /* Run - 16 MHz */
    case SYSTEM_LOWPOWER_S1:
    {
      if (SystemClock_RunLow() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }

    /* Sleep / Stop0 / Stop1 / Standby / Shutdown: lower the clock beforehand */
    default:
    {
      SystemClock_Decrease();
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure SRAM retention.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_ram_retention_config(System_LowPowerModeScenarioTypeDef scenario)
{
  switch (scenario)
  {
    /* Standby - SRAM retention + RTC */
    case SYSTEM_LOWPOWER_S5:
    {
      HAL_PWREx_EnableSRAMRetention();
      break;
    }

    default:
    {
      HAL_PWREx_DisableSRAMRetention();
      break;
    }
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 16 MHz (HSI16).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_RunLow(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSI16 without touching the PLL: it is still the active SYSCLK source here */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Switch SYSCLK to HSI16: the PLL cannot be disabled while still selected as SYSCLK source */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Now safe to disable the PLL and its MSI source, both unused from this point */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_OFF;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure the RTC peripheral and Initialize it.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Enable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Enable Backup domain access, PWR_CR1_DBP enable */
  HAL_PWR_EnableBkUpAccess();

  /* Enable LSE oscillator if not already running */
  if (READ_BIT(RCC->BDCR, RCC_BDCR_LSEON) == 0U)
  {
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
    {
    }
  }

  /* Select LSE as RTC clock source and enable the RTC clock */
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
  __HAL_RCC_RTC_ENABLE();

  /* Set parameter to be configured */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = 0x7FU;
  RTCHandle.Init.SynchPrediv    = 0xFFU;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  /* Initialize the RTC peripheral */
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Disable Backup access */
  HAL_PWR_DisableBkUpAccess();

  return SYSTEM_OK;
}

/**
  * @brief  De-Initialize the RTC peripheral.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Disable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Check RTC state */
  if (HAL_RTC_GetState(&RTCHandle) != HAL_RTC_STATE_RESET)
  {
    /* De-Initialize the RTC peripheral */
    if (HAL_RTC_DeInit(&RTCHandle) != HAL_OK)
    {
      return SYSTEM_ERROR;
    }
  }

  /* Enable Backup domain access, PWR_CR1_DBP enable */
  HAL_PWR_EnableBkUpAccess();

  /* Disable RTC clock and LSE oscillator */
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);

  /* Disable backup access */
  HAL_PWR_DisableBkUpAccess();

  return SYSTEM_OK;
}

/**
  * @brief  Configure system low power mode.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_lowpower_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Suspend Systick before entering any low power mode */
  HAL_SuspendTick();

  /* Empty buffers before reaching the final mode */
  __DSB();

  switch (scenario)
  {
    /* Sleep - 2 MHz */
    case SYSTEM_LOWPOWER_S2:
    {
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 0 mode */
    case SYSTEM_LOWPOWER_S3:
    {
      HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
      break;
    }

    /* Stop 1 mode */
    case SYSTEM_LOWPOWER_S4:
    {
      HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
      break;
    }

    /* Standby - SRAM retention + RTC */
    case SYSTEM_LOWPOWER_S5:
    /* Standby - no SRAM retention + RTC */
    case SYSTEM_LOWPOWER_S6:
    /* Standby - no SRAM retention + no RTC */
    case SYSTEM_LOWPOWER_S7:
    {
      /* MCU resets on wake-up from Standby: this call never returns */
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Shutdown w/ RTC */
    case SYSTEM_LOWPOWER_S8:
    /* Shutdown wo/ RTC */
    case SYSTEM_LOWPOWER_S9:
    {
      /* MCU resets on wake-up from Shutdown: this call never returns */
      HAL_PWREx_EnterSHUTDOWNMode();
      break;
    }

    /* Run modes: nothing more to do */
    default:
    {
      break;
    }
  }

  return status;
}

/* USER CODE END 1 */