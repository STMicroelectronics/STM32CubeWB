/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nucleo_wb35ce.h"
#include "stm32wbxx_ll_i2c.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_utils.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx_MASTER's NVIC */
#define I2Cx_MASTER_EV_IRQn                     I2C3_EV_IRQn
#define I2Cx_MASTER_ER_IRQn                     I2C3_ER_IRQn
#define I2Cx_MASTER_EV_IRQHandler               I2C3_EV_IRQHandler
#define I2Cx_MASTER_ER_IRQHandler               I2C3_ER_IRQHandler

/* Definition for I2Cx_SLAVE's NVIC */
#define I2Cx_SLAVE_EV_IRQn                      I2C1_EV_IRQn
#define I2Cx_SLAVE_ER_IRQn                      I2C1_ER_IRQn
#define I2Cx_SLAVE_EV_IRQHandler                I2C1_EV_IRQHandler
#define I2Cx_SLAVE_ER_IRQHandler                I2C1_ER_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                            (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                            TXBUFFERSIZE

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS1 0x13E
#define I2C_SLAVE_ADDRESS2 0x64
#define I2C_TIMING 0x10B07DB7
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
