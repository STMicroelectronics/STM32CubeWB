/**
  ******************************************************************************
 * @file    hw_conf.h
 * @author  MCD Application Team
 * @brief   Configuration of hardware interface
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HW_CONF_H
#define HW_CONF_H

/******************************************************************************
* Semaphores
* THIS SHALL NO BE CHANGED AS THESE SEMAPHORES ARE USED AS WELL ON THE CM0+
*****************************************************************************/
/**
*  Index of the semaphore used by CPU2 to prevent the CPU1 to either write or erase data in flash
*  The CPU1 shall not either write or erase in flash when this semaphore is taken by the CPU2
*  When the CPU1 needs to either write or erase in flash, it shall first get the semaphore and release it just
*  after writing a raw (64bits data) or erasing one sector.
*  On v1.4.0 and older CPU2 wireless firmware, this semaphore is unused and CPU2 is using PES bit.
*  By default, CPU2 is using the PES bit to protect its timing. The CPU1 may request the CPU2 to use the semaphore
*  instead of the PES bit by sending the system command SHCI_C2_SetFlashActivityControl()
*/
#define CFG_HW_BLOCK_FLASH_REQ_BY_CPU2_SEMID                    7

/**
*  Index of the semaphore used by CPU1 to prevent the CPU2 to either write or erase data in flash
*  In order to protect its timing, the CPU1 may get this semaphore to prevent the  CPU2 to either
*  write or erase in flash (as this will stall both CPUs)
*  The PES bit shall not be used as this may stall the CPU2 in some cases.
*/
#define CFG_HW_BLOCK_FLASH_REQ_BY_CPU1_SEMID                    6

/**
*  Index of the semaphore used to manage the CLK48 clock configuration
*  When the USB is required, this semaphore shall be taken before configuring te CLK48 for USB
*  and should be released after the application switch OFF the clock when the USB is not used anymore
*  When using the RNG, it is good enough to use CFG_HW_RNG_SEMID to control CLK48.
*  More details in AN5289
*/
#define CFG_HW_CLK48_CONFIG_SEMID                               5

/* Index of the semaphore used to manage the entry Stop Mode procedure */
#define CFG_HW_ENTRY_STOP_MODE_SEMID                            4

/* Index of the semaphore used to access the RCC */
#define CFG_HW_RCC_SEMID                                        3

/* Index of the semaphore used to access the FLASH */
#define CFG_HW_FLASH_SEMID                                      2

/* Index of the semaphore used to access the PKA */
#define CFG_HW_PKA_SEMID                                        1

/* Index of the semaphore used to access the RNG */
#define CFG_HW_RNG_SEMID                                        0

/******************************************************************************
 * HW UART
 *****************************************************************************/

/* For release: set to 1  CFG_HW_LPUART1_ENABLED and CFG_HW_LPUART1_DMA_TX_SUPPORTED */
/* CLI = LPUART1
 * TRACES = USART1
 */
#define CFG_HW_LPUART1_ENABLED                 1
#define CFG_HW_LPUART1_DMA_TX_SUPPORTED        1

#define CFG_HW_USART1_ENABLED                  1
#define CFG_HW_USART1_DMA_TX_SUPPORTED         1

#if(CFG_HW_LPUART1_ENABLED == 1)
/**
 * LPUART1
 */
#define CFG_HW_LPUART1_PREEMPTPRIORITY         0x0F
#define CFG_HW_LPUART1_SUBPRIORITY             0

#define CFG_HW_LPUART1_BAUDRATE                115200
#define CFG_HW_LPUART1_WORDLENGTH              UART_WORDLENGTH_8B
#define CFG_HW_LPUART1_STOPBITS                UART_STOPBITS_1
#define CFG_HW_LPUART1_PARITY                  UART_PARITY_NONE
#define CFG_HW_LPUART1_HWFLOWCTL               UART_HWCONTROL_NONE
#define CFG_HW_LPUART1_MODE                    UART_MODE_TX_RX
#define CFG_HW_LPUART1_ADVFEATUREINIT          UART_ADVFEATURE_NO_INIT
#define CFG_HW_LPUART1_OVERSAMPLING            UART_OVERSAMPLING_8

#define CFG_HW_LPUART1_TX_PORT_CLK_ENABLE      __HAL_RCC_GPIOA_CLK_ENABLE
#define CFG_HW_LPUART1_TX_PORT                 GPIOA
#define CFG_HW_LPUART1_TX_PIN                  GPIO_PIN_2
#define CFG_HW_LPUART1_TX_MODE                 GPIO_MODE_AF_PP
#define CFG_HW_LPUART1_TX_PULL                 GPIO_NOPULL
#define CFG_HW_LPUART1_TX_SPEED                GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_LPUART1_TX_ALTERNATE            GPIO_AF8_LPUART1

#define CFG_HW_LPUART1_RX_PORT_CLK_ENABLE      __HAL_RCC_GPIOA_CLK_ENABLE
#define CFG_HW_LPUART1_RX_PORT                 GPIOA
#define CFG_HW_LPUART1_RX_PIN                  GPIO_PIN_3
#define CFG_HW_LPUART1_RX_MODE                 GPIO_MODE_AF_PP
#define CFG_HW_LPUART1_RX_PULL                 GPIO_NOPULL
#define CFG_HW_LPUART1_RX_SPEED                GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_LPUART1_RX_ALTERNATE            GPIO_AF8_LPUART1

#define CFG_HW_LPUART1_CTS_PORT_CLK_ENABLE     __HAL_RCC_GPIOA_CLK_ENABLE
#define CFG_HW_LPUART1_CTS_PORT                GPIOA
#define CFG_HW_LPUART1_CTS_PIN                 GPIO_PIN_6
#define CFG_HW_LPUART1_CTS_MODE                GPIO_MODE_AF_PP
#define CFG_HW_LPUART1_CTS_PULL                GPIO_PULLDOWN
#define CFG_HW_LPUART1_CTS_SPEED               GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_LPUART1_CTS_ALTERNATE           GPIO_AF8_LPUART1

#if(CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
#define CFG_HW_LPUART1_DMA_TX_PREEMPTPRIORITY  0x0F
#define CFG_HW_LPUART1_DMA_TX_SUBPRIORITY      0

#define CFG_HW_LPUART1_DMAMUX_CLK_ENABLE       __HAL_RCC_DMAMUX1_CLK_ENABLE
#define CFG_HW_LPUART1_DMA_CLK_ENABLE          __HAL_RCC_DMA1_CLK_ENABLE
#define CFG_HW_LPUART1_TX_DMA_REQ              DMA_REQUEST_LPUART1_TX
#define CFG_HW_LPUART1_TX_DMA_CHANNEL          DMA1_Channel4
#define CFG_HW_LPUART1_TX_DMA_IRQn             DMA1_Channel4_IRQn
#define CFG_HW_LPUART1_DMA_TX_IRQHandler       DMA1_Channel4_IRQHandler
#endif
#endif

#if(CFG_HW_USART1_ENABLED == 1)
/**
 * UART1
 */
#define CFG_HW_USART1_PREEMPTPRIORITY         0x0F
#define CFG_HW_USART1_SUBPRIORITY             0

#define CFG_HW_USART1_BAUDRATE                115200
#define CFG_HW_USART1_WORDLENGTH              UART_WORDLENGTH_8B
#define CFG_HW_USART1_STOPBITS                UART_STOPBITS_1
#define CFG_HW_USART1_PARITY                  UART_PARITY_NONE
#define CFG_HW_USART1_HWFLOWCTL               UART_HWCONTROL_NONE
#define CFG_HW_USART1_MODE                    UART_MODE_TX_RX
#define CFG_HW_USART1_ADVFEATUREINIT          UART_ADVFEATURE_NO_INIT
#define CFG_HW_USART1_OVERSAMPLING            UART_OVERSAMPLING_8

#define CFG_HW_USART1_TX_PORT_CLK_ENABLE      __HAL_RCC_GPIOB_CLK_ENABLE
#define CFG_HW_USART1_TX_PORT                 GPIOB
#define CFG_HW_USART1_TX_PIN                  GPIO_PIN_6
#define CFG_HW_USART1_TX_MODE                 GPIO_MODE_AF_PP
#define CFG_HW_USART1_TX_PULL                 GPIO_NOPULL
#define CFG_HW_USART1_TX_SPEED                GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_USART1_TX_ALTERNATE            GPIO_AF7_USART1

#define CFG_HW_USART1_RX_PORT_CLK_ENABLE      __HAL_RCC_GPIOB_CLK_ENABLE
#define CFG_HW_USART1_RX_PORT                 GPIOB
#define CFG_HW_USART1_RX_PIN                  GPIO_PIN_7
#define CFG_HW_USART1_RX_MODE                 GPIO_MODE_AF_PP
#define CFG_HW_USART1_RX_PULL                 GPIO_NOPULL
#define CFG_HW_USART1_RX_SPEED                GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_USART1_RX_ALTERNATE            GPIO_AF7_USART1

#define CFG_HW_USART1_CTS_PORT_CLK_ENABLE     __HAL_RCC_GPIOA_CLK_ENABLE
#define CFG_HW_USART1_CTS_PORT                GPIOA
#define CFG_HW_USART1_CTS_PIN                 GPIO_PIN_11
#define CFG_HW_USART1_CTS_MODE                GPIO_MODE_AF_PP
#define CFG_HW_USART1_CTS_PULL                GPIO_PULLDOWN
#define CFG_HW_USART1_CTS_SPEED               GPIO_SPEED_FREQ_VERY_HIGH
#define CFG_HW_USART1_CTS_ALTERNATE           GPIO_AF7_USART1

#if(CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
#define CFG_HW_USART1_DMA_TX_PREEMPTPRIORITY  0x0F
#define CFG_HW_USART1_DMA_TX_SUBPRIORITY      0

#define CFG_HW_USART1_DMAMUX_CLK_ENABLE       __HAL_RCC_DMAMUX1_CLK_ENABLE
#define CFG_HW_USART1_DMA_CLK_ENABLE          __HAL_RCC_DMA2_CLK_ENABLE
#define CFG_HW_USART1_TX_DMA_REQ              DMA_REQUEST_USART1_TX
#define CFG_HW_USART1_TX_DMA_CHANNEL          DMA2_Channel4
#define CFG_HW_USART1_TX_DMA_IRQn             DMA2_Channel4_IRQn
#define CFG_HW_USART1_DMA_TX_IRQHandler       DMA2_Channel4_IRQHandler
#endif
#endif


#endif /* HW_CONF_H */
