/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Src/stm32wbxx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_aes1_in;

extern DMA_HandleTypeDef hdma_aes1_out;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief CRYP MSP Initialization
* This function configures the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==AES1)
  {
  /* USER CODE BEGIN AES1_MspInit 0 */

  /* USER CODE END AES1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_AES1_CLK_ENABLE();

    /* AES1 DMA Init */
    /* AES1_IN Init */
    hdma_aes1_in.Instance = DMA1_Channel1;
    hdma_aes1_in.Init.Request = DMA_REQUEST_AES1_IN;
    hdma_aes1_in.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_aes1_in.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_aes1_in.Init.MemInc = DMA_MINC_ENABLE;
    hdma_aes1_in.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_aes1_in.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_aes1_in.Init.Mode = DMA_NORMAL;
    hdma_aes1_in.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_aes1_in) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcryp,hdmain,hdma_aes1_in);

    /* AES1_OUT Init */
    hdma_aes1_out.Instance = DMA1_Channel2;
    hdma_aes1_out.Init.Request = DMA_REQUEST_AES1_OUT;
    hdma_aes1_out.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_aes1_out.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_aes1_out.Init.MemInc = DMA_MINC_ENABLE;
    hdma_aes1_out.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_aes1_out.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_aes1_out.Init.Mode = DMA_NORMAL;
    hdma_aes1_out.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_aes1_out) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcryp,hdmaout,hdma_aes1_out);

  /* USER CODE BEGIN AES1_MspInit 1 */
  HAL_NVIC_SetPriority(AES1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(AES1_IRQn); 
  /* USER CODE END AES1_MspInit 1 */
  }

}

/**
* @brief CRYP MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==AES1)
  {
  /* USER CODE BEGIN AES1_MspDeInit 0 */

  /* USER CODE END AES1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_AES1_CLK_DISABLE();

    /* AES1 DMA DeInit */
    HAL_DMA_DeInit(hcryp->hdmain);
    HAL_DMA_DeInit(hcryp->hdmaout);
  /* USER CODE BEGIN AES1_MspDeInit 1 */
  /* Disable CRYP Interrupt */
  HAL_NVIC_DisableIRQ(AES1_IRQn); 
  /* USER CODE END AES1_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
