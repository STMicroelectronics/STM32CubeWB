/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_bus.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32WB5MM-DK bus.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB5MM_DK_BUS_H
#define STM32WB5MM_DK_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */

/** @addtogroup STM32WB5MM_DK_BUS
  * @{
  */
   
/** @defgroup STM32WB5MM_DK_BUS_Exported_Types STM32WB5MM-DK BUS Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pSPI_CallbackTypeDef  pMspSpiInitCb;
  pSPI_CallbackTypeDef  pMspSpiDeInitCb;
}BSP_SPI1_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_BUS_Exported_Constants STM32WB5MM-DK BUS Exported Constants
  * @{
  */
#define BUS_I2C3                        I2C3
#define BUS_I2C3_CLK_ENABLE()           __HAL_RCC_I2C3_CLK_ENABLE()
#define BUS_I2C3_CLK_DISABLE()          __HAL_RCC_I2C3_CLK_DISABLE()
#define BUS_I2C3_FORCE_RESET()          __HAL_RCC_I2C3_FORCE_RESET()
#define BUS_I2C3_RELEASE_RESET()        __HAL_RCC_I2C3_RELEASE_RESET()
#define BUS_I2C3_SCL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C3_SCL_GPIO_PIN           GPIO_PIN_13
#define BUS_I2C3_SCL_GPIO_PORT          GPIOB
#define BUS_I2C3_SCL_GPIO_AF            GPIO_AF4_I2C3
#define BUS_I2C3_SDA_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C3_SDA_GPIO_PIN           GPIO_PIN_11
#define BUS_I2C3_SDA_GPIO_PORT          GPIOB
#define BUS_I2C3_SDA_GPIO_AF            GPIO_AF4_I2C3
#define BUS_I2C3_TIMEOUT                10000U
#if defined(HAL_SPI_MODULE_ENABLED)

/*##################### SPI1 ###################################*/
#define BUS_SPI1_INSTANCE                 SPI1
#define BUS_SPI1_CLOCK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define BUS_SPI1_CLOCK_DISABLE()          __HAL_RCC_SPI1_CLK_DISABLE()
#define BUS_SPI1_GPIO_PORT                GPIOB                      /* GPIOB */
  
#define BUS_SPI1_GPIO_PORTA               GPIOA
  
#define BUS_SPI1_AF                       GPIO_AF5_SPI1
#define BUS_SPI1_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI1_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()
  
#define BUS_SPI1_GPIO_CLKA_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_GPIO_CLKA_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()
  
#define BUS_SPI1_GPIO_FORCE_RESET()       __HAL_RCC_SPI1_FORCE_RESET()
#define BUS_SPI1_GPIO_RELEASE_RESET()     __HAL_RCC_SPI1_RELEASE_RESET()
#define BUS_SPI1_SCK_PIN                  GPIO_PIN_1                 /* PA.01*/
#define BUS_SPI1_MOSI_PIN                 GPIO_PIN_7                 /* PA.07 */

#define BUS_SPI1_TIMEOUT                  ((uint32_t)0x1000)

#ifndef BUS_SPI1_BAUDRATE
   #define BUS_SPI1_BAUDRATE  12500000    /* baud rate of SPIn = 12.5 Mbps*/
#endif

#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c3;
extern SPI_HandleTypeDef hbus_spi1;
/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_BUS_Exported_Functions
  * @{
  */
int32_t BSP_I2C3_Init(void);
int32_t BSP_I2C3_DeInit(void);
int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C3_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C3_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);

int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI1_RegisterMspCallbacks (BSP_SPI1_Cb_t *Callbacks);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */

HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef* phspi, uint32_t BaudratePrescaler);

int32_t BSP_GetTick(void);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32WB5MM_DK_BUS_H */
