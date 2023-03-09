/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ModularExponentiation/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use PKA peripheral to generate an
  *          ECDSA signature using the STM32WBxx PKA LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

__IO uint32_t endOfProcess = 0;
uint8_t buffer[256] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_PKA_Init(void);
/* USER CODE BEGIN PFP */

void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
static uint32_t Buffercmp(const uint8_t* pBuffer1,const uint8_t* pBuffer2, uint32_t BufferLength);
__IO uint32_t *PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], uint32_t n);
uint8_t *PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], uint32_t n);
void PKA_load_ciphering_parameter(void);
void PKA_load_unciphering_parameter(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  uint32_t result = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_PKA_Init();
  /* USER CODE BEGIN 2 */
  
  /* Set mode to ECDSA signature generation in interrupt mode */
  LL_PKA_SetMode(PKA, LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP);
  LL_PKA_EnableIT_ADDRERR(PKA);
  LL_PKA_EnableIT_RAMERR(PKA);
  LL_PKA_EnableIT_PROCEND(PKA);
  
  /*   FROM PLAINTEXT TO CIPHERTEXT   */
  
  /* Loads the input buffers to PKA RAM */
  PKA_load_ciphering_parameter();
  
  /* Launch the computation in interrupt mode */
  LL_PKA_Start(PKA);
  
  /* Wait for the interrupt callback */
  while(endOfProcess != 1);
  endOfProcess = 0;
  
  /* Retrieve the result and output buffer */
  PKA_Memcpy_u32_to_u8(buffer, &PKA->RAM[PKA_MODULAR_EXP_OUT_SM_ALGO_ACC1], rsa_pub_2048_modulus_len / 4);
  
  /* Compare to expected results */
  result = Buffercmp(buffer, ciphertext_bin, ciphertext_bin_len);
  if (result != 0)
  {
    LED_Blinking(LED_BLINK_ERROR);
  } 
  
  /*   FROM CIPHERTEXT TO PLAINTEXT   */
  
  /* Loads the input buffers to PKA RAM */  
  PKA_load_unciphering_parameter();
  
  /* Launch the computation in interrupt mode */
  LL_PKA_Start(PKA);
  
  /* Wait for the interrupt callback */
  while(endOfProcess != 1);
  endOfProcess = 0;
  
  /* Retrieve the result and output buffer */
  PKA_Memcpy_u32_to_u8(buffer, &PKA->RAM[PKA_MODULAR_EXP_OUT_SM_ALGO_ACC1], rsa_pub_2048_modulus_len / 4);
  
  /* Compare to expected results */
  result = Buffercmp(buffer, plaintext_bin, plaintext_bin_len);
  if (result != 0)
  {
    LED_Blinking(LED_BLINK_ERROR);
  }  
  
  LED_On();
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Sysclk activation on the HSI */
  /* Set CPU1 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set CPU2 prescaler*/
  LL_C2_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set AHB SHARED prescaler*/
  LL_RCC_SetAHB4Prescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set APB2 prescaler*/
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(16000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/**
  * @brief PKA Initialization Function
  * @param None
  * @retval None
  */
static void MX_PKA_Init(void)
{

  /* USER CODE BEGIN PKA_Init 0 */

  /* USER CODE END PKA_Init 0 */

  /* Peripheral clock enable */
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PKA);

  /* USER CODE BEGIN PKA_Init 1 */

  /* USER CODE END PKA_Init 1 */
  LL_PKA_Enable(PKA);
  /* USER CODE BEGIN PKA_Init 2 */

    /* Configure NVIC for PKA interrupts */
    /*   Set priority for PKA_IRQn */
    /*   Enable PKA_IRQn */
    NVIC_SetPriority(PKA_IRQn, 0);  
    NVIC_EnableIRQ(PKA_IRQn);
  
  /* USER CODE END PKA_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

  /**/
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Load into PKA RAM the ciphering parameters.
  * @param  None
  * @retval None
  */
void PKA_load_ciphering_parameter(void)
{
  /* Get the number of bit per operand */
  PKA->RAM[PKA_MODULAR_EXP_IN_OP_NB_BITS] = rsa_pub_2048_modulus_len*8;

  /* Get the number of bit of the exponent */
  PKA->RAM[PKA_MODULAR_EXP_IN_EXP_NB_BITS] = rsa_pub_2048_publicExponent_len*8;

  /* Move the input parameters pOp1 to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT_BASE], plaintext_bin, rsa_pub_2048_modulus_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT_BASE + rsa_pub_2048_modulus_len / 4] = 0;

  /* Move the exponent to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT], rsa_pub_2048_publicExponent, rsa_pub_2048_publicExponent_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT + rsa_pub_2048_publicExponent_len / 4] = 0;

  /* Move the modulus to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_MODULUS], rsa_pub_2048_modulus, rsa_pub_2048_modulus_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_MODULUS + rsa_pub_2048_modulus_len / 4] = 0;
}

/**
  * @brief  Load into PKA RAM the unciphering parameters.
  * @param  None
  * @retval None
  */
void PKA_load_unciphering_parameter(void)
{
  /* Get the number of bit per operand */
  PKA->RAM[PKA_MODULAR_EXP_IN_OP_NB_BITS] = rsa_pub_2048_modulus_len*8;

  /* Get the number of bit of the exponent */
  PKA->RAM[PKA_MODULAR_EXP_IN_EXP_NB_BITS] = rsa_priv_2048_privateExponent_len*8;

  /* Move the input parameters pOp1 to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT_BASE], ciphertext_bin, rsa_pub_2048_modulus_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT_BASE + rsa_pub_2048_modulus_len / 4] = 0;

  /* Move the exponent to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT], rsa_priv_2048_privateExponent, rsa_priv_2048_privateExponent_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_EXPONENT + rsa_priv_2048_privateExponent_len / 4] = 0;

  /* Move the modulus to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_MODULAR_EXP_IN_MODULUS], rsa_priv_2048_modulus, rsa_pub_2048_modulus_len);
  PKA->RAM[PKA_MODULAR_EXP_IN_MODULUS + rsa_pub_2048_modulus_len / 4] = 0;
  
}


void PKA_ERROR_callback(void)
{
  LED_Blinking(LED_BLINK_ERROR);
}

void PKA_PROCEND_callback(void)
{
  endOfProcess = 1;
}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint32_t Buffercmp(const uint8_t* pBuffer1,const uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Copy uint8_t array to uint32_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of u32 to be handled
  * @retval dst
  */
__IO uint32_t *PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], uint32_t n)
{
  const uint32_t *ptrSrc = (const uint32_t *) src;

  if (dst != 0)
  {
    for (uint32_t index = 0; index < n / 4; index++)
    {
      dst[index] = __REV(ptrSrc[n / 4 - index - 1]);
    }
  }
  return dst;
}

/**
  * @brief  Copy uint32_t array to uint8_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of u8 to be handled (must be multiple of 4)
  * @retval dst
  */
uint8_t *PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], uint32_t n)
{
  uint32_t *ptrDst = (uint32_t *) dst;
  if (dst != 0)
  {
    for (uint32_t index = 0; index < n; index++)
    {
      ptrDst[n - index - 1] = __REV(src[index]);
    }
  }
  return dst;
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin); 
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle LED2 in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); 
    LL_mDelay(Period);
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
