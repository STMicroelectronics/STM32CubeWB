/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

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
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/**
  * @brief   APP_ZIGBEE_ThreadX_Low_Power_Setup
  * @param  count : TX timer count
  * @retval None
  */
void  APP_ZIGBEE_ThreadX_Low_Power_Setup(ULONG count)
{
  /* USER CODE BEGIN   APP_ZIGBEE_ThreadX_Low_Power_Setup */

  /* USER CODE END   APP_ZIGBEE_ThreadX_Low_Power_Setup */
}

/**
  * @brief  APP_ZIGBEE_ThreadX_EnterLowPower
  * @param  None
  * @retval None
  */
void APP_ZIGBEE_ThreadX_EnterLowPower(void)
{
  /* USER CODE BEGIN  APP_ZIGBEE_ThreadX_EnterLowPower */

  /* USER CODE END  APP_ZIGBEE_ThreadX_EnterLowPower */
}

/**
  * @brief  APP_ZIGBEE_ThreadX_ExitLowPower
  * @param  None
  * @retval None
  */
void APP_ZIGBEE_ThreadX_ExitLowPower(void)
{
  /* USER CODE BEGIN  APP_ZIGBEE_ThreadX_ExitLowPower */

  /* USER CODE END  APP_ZIGBEE_ThreadX_ExitLowPower */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
