/**
  ******************************************************************************
  * @file    gpio_lld.c
  * @author  MCD Application Team
  * @brief   This file contains the init of all the GPIOs used by LLD tests.
  *          It is to be used on both M0 and M4.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) STMicroelectronics</center></h2>
(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
// Be carrefull with the .h included as this file must be compilable on both M0 and M4 environments
#include "app_conf.h"
#include "gpio_lld.h"

/* Initialize GPIOs fused by PHY valid CLI */
void gpio_lld_phy_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable clock(s) for GPIOs */
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
#ifdef STM32WB35xx
#else
  __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
#ifdef USE_SIMU
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#ifdef STM32WB35xx
#else
  __HAL_RCC_C2GPIOC_CLK_ENABLE();
#endif
#ifdef USE_SIMU
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
  __HAL_RCC_C2GPIOC_CLK_ENABLE();
  __HAL_RCC_C2GPIOD_CLK_ENABLE();
#endif
#endif

#if defined (USE_SIMU)
  // Initialize GPIO used to detect if current STM32WB5x is master or slave
  // 4 GPIOs are needed while only one is enough but this must be kept as it is 
  // because this is also used by tests which are not managed by MDG-RF
  GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  // Initialize GPIOs used to have both STM32WB5x communicating over
  // 13 bits of the port C and D which are cross connected in SIMU test bench
  GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif
  
  // configure the GPIO to be set to '1' during frame TX
  GPIO_InitStruct.Pin = (GPIO_TX_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIO_TX_PORT, &GPIO_InitStruct);
  gpio_lld_phy_gpioTx_down();
  
  // configure the GPIO to be set to '1' during HardFault'
  GPIO_InitStruct.Pin = (GPIO_HARD_FAULT_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIO_HARD_FAULT_PORT, &GPIO_InitStruct);
  gpio_lld_phy_gpioHardFault_down();
  
  // configure 2 GPIOs that can be used for debug purposes
  GPIO_InitStruct.Pin = (GPIO_1_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIO_1_PORT, &GPIO_InitStruct);
  gpio_lld_phy_gpio1_down();
#if !defined (USE_SIMU) && !defined (USE_FPGA)
  GPIO_InitStruct.Pin = (GPIO_2_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIO_2_PORT, &GPIO_InitStruct);
  gpio_lld_phy_gpio2_down();
#endif
}

/* Set PHY GPIO_TX to '1' */
void gpio_lld_phy_gpioTx_up(void) {
  HAL_GPIO_WritePin(GPIO_TX_PORT, GPIO_TX_PIN, GPIO_PIN_SET);
}

/* Set PHY GPIO_TX to '0' */
void gpio_lld_phy_gpioTx_down(void) {
  HAL_GPIO_WritePin(GPIO_TX_PORT, GPIO_TX_PIN, GPIO_PIN_RESET);
}

/* Set PHY GPIO_HARD_FAULT to '1' */
void gpio_lld_phy_gpioHardFault_up(void) {
  HAL_GPIO_WritePin(GPIO_HARD_FAULT_PORT, GPIO_HARD_FAULT_PIN, GPIO_PIN_SET);
}

/* Set PHY GPIO_HARD_FAULT to '0' */
void gpio_lld_phy_gpioHardFault_down(void) {
  HAL_GPIO_WritePin(GPIO_HARD_FAULT_PORT, GPIO_HARD_FAULT_PIN, GPIO_PIN_RESET);
}

/* Set PHY GPIO_1 to '1' */
void gpio_lld_phy_gpio1_up(void) {
  HAL_GPIO_WritePin(GPIO_1_PORT, GPIO_1_PIN, GPIO_PIN_SET);
}

/* Set PHY GPIO_1 to '0' */
void gpio_lld_phy_gpio1_down(void) {
  HAL_GPIO_WritePin(GPIO_1_PORT, GPIO_1_PIN, GPIO_PIN_RESET);
}

/* Set PHY GPIO_2 to '1' */
void gpio_lld_phy_gpio2_up(void) {
  HAL_GPIO_WritePin(GPIO_2_PORT, GPIO_2_PIN, GPIO_PIN_SET);
}

/* Set PHY GPIO_2 to '0' */
void gpio_lld_phy_gpio2_down(void) {
  HAL_GPIO_WritePin(GPIO_2_PORT, GPIO_2_PIN, GPIO_PIN_RESET);
}

/* De-initialize GPIOs fused by PHY valid CLI */
void gpio_lld_phy_deInit(void) {
  HAL_GPIO_DeInit(GPIO_TX_PORT, GPIO_TX_PIN);
  HAL_GPIO_DeInit(GPIO_HARD_FAULT_PORT, GPIO_HARD_FAULT_PIN);
  HAL_GPIO_DeInit(GPIO_1_PORT, GPIO_1_PIN);
  HAL_GPIO_DeInit(GPIO_2_PORT, GPIO_2_PIN);
}

/* Initialize GPIOs for MCO use */
void gpio_lld_mco_init(uint32_t mcoSource, uint32_t mcoDiv) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  //HAL_RCC_MCOConfig(RCC_MCO3, mcoSource, mcoDiv);
  LL_RCC_ConfigMCO(mcoSource, mcoDiv);
  
  /* Enable clock(s) for GPIOs */
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
#endif
  
  // configure the GPIO PA15 in AF6 to be used as MCO
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_MCO;
  GPIO_InitStruct.Pin       = GPIO_MCO_PIN;
  HAL_GPIO_Init(GPIO_MCO_PORT, &GPIO_InitStruct);
}

/* De-initialize GPIOs for external PA use */
void gpio_lld_mco_deInit(void) {
  HAL_GPIO_DeInit(GPIO_MCO_PORT, GPIO_MCO_PIN);
  //HAL_RCC_MCOConfig(RCC_MCO3, RCC_MCO1SOURCE_NOCLOCK, RCC_MCODIV_1);
  LL_RCC_ConfigMCO(RCC_MCO1SOURCE_NOCLOCK, RCC_MCODIV_1);
}

/* Initialize GPIOs for external PA use */
void gpio_lld_extPa_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable clock(s) for GPIOs */
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOB_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
  
  // configure the GPIO PB0 in AF6 to be used as RF_TX_MOD_EXT_PA
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_RF_DTB0;
  GPIO_InitStruct.Pin       = GPIO_EXT_PA_TX_PIN;
  HAL_GPIO_Init(GPIO_EXT_PA_TX_PORT, &GPIO_InitStruct);
  
  // configure the GPIO which will be managed by M0 stack to enable Ext PA
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pin   = GPIO_EXT_PA_EN_PIN;
  HAL_GPIO_Init(GPIO_EXT_PA_EN_PORT, &GPIO_InitStruct);
}

/* De-initialize GPIOs for external PA use */
void gpio_lld_extPa_deInit(void) {
  HAL_GPIO_DeInit(GPIO_EXT_PA_TX_PORT, GPIO_EXT_PA_TX_PIN);
  HAL_GPIO_DeInit(GPIO_EXT_PA_EN_PORT, GPIO_EXT_PA_EN_PIN);
}

/* Set EXT PA EN to '1' */
void gpio_lld_phy_extPaEn_up(void) {
  HAL_GPIO_WritePin(GPIO_EXT_PA_EN_PORT, GPIO_EXT_PA_EN_PIN, GPIO_PIN_SET);
}

/* Set EXT PA EN to '0' */
void gpio_lld_phy_extPaEn_down(void) {
  HAL_GPIO_WritePin(GPIO_EXT_PA_EN_PORT, GPIO_EXT_PA_EN_PIN, GPIO_PIN_RESET);
}

#ifdef USE_SIMU
/* Initialize GPIOs for master STM32WB5x of the SIMU */
void gpio_lld_SimuMaster_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable clock(s) for GPIOs */
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
  
  // In STM32WB5x master only : initialize GPIO port A pin 0, 1 and, 2 to send CRC result to simulator
  GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // In STM32WB5x master only : initialize GPIO port B pin 8 and pin 9 to be used for debug purposes
  GPIO_InitStruct.Pin = (GPIO_TX_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH  ;
  HAL_GPIO_Init(GPIO_TX_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = (GPIO_1_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH  ;
  HAL_GPIO_Init(GPIO_1_PORT, &GPIO_InitStruct);
}

/* Initialize GPIOs for slave STM32WB5x of the SIMU */
void gpio_lld_SimuSlave_init(void) {
// No GPIO to initialize in STM32WB5x slave
}
#endif

/* Initialize GPIOs used by USART */
void gpio_lld_usart_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable clock(s) for GPIOs used by USART */
#if USE_NEW_SET_OF_GPIO_FOR_USART
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
#endif
#else
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOB_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
#endif
  
  /* USART1 GPIO Configuration
       USART1_TX : PB6
       USART1_RX : PB7
  */
#if USE_NEW_SET_OF_GPIO_FOR_USART
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#else
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
}

/* De-initialize GPIOs used by USART */
void gpio_lld_usart_deInit(void) {
  /* USART1 GPIO Configuration
       USART1_TX : PB6
       USART1_RX : PB7
  */
#if USE_NEW_SET_OF_GPIO_FOR_USART
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
#else
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
#endif
  
  /* Do not disable clocks as they could be used by others GPIOs and it seems to not need power in STOP mode */
}

/* Initialize GPIOs used by LPUART */
void gpio_lld_lpuart_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
#ifdef STM32WB35xx
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
#else
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOC_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOC_CLK_ENABLE();
#endif
#endif
  
  /* LPUART1 GPIO Configuration
       LPUART1_TX : PB5 on STM32WB3x or PC1 on STM32WB5x
       LPUART1_RX : PA3 on STM32WB3x or PC0 on STM32WB5x
  */
#ifdef STM32WB35xx
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#else
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

/* De-initialize GPIOs used by LPUART */
void gpio_lld_lpuart_deInit(void) {
  /* LPUART1 GPIO Configuration
       LPUART1_TX : PB5 on STM32WB3x PC1 on STM32WB5x
       LPUART1_RX : PA3 on STM32WB3x PC0 on STM32WB5x
  */
#ifdef STM32WB35xx
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
#else
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_1);
#endif
  /* Do not disable clocks as they could be used by others GPIOs and it seems to not need power in STOP mode */
}

#if !defined (USE_SIMU) && !defined(USE_FPGA)
/* Initialize GPIO PA2 (for debug use only) */
void gpio_lld_pa2_init(uint8_t mode, uint8_t speed) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable clock(s) for GPIOs */
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
#endif
  
  if (mode == 0) {
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  } else if (mode == 1) {
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF6_RF_DTB7;
  } else if (mode == 2) {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
  } else if (mode == 3) {
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
  } else {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
  }
  
  if (speed <= GPIO_SPEED_FREQ_VERY_HIGH) {
    GPIO_InitStruct.Speed = speed;
  } else {
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  }
  
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* De-initialize GPIO PA2 (for debug use only) */
void gpio_lld_pa2_deInit(void) {
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
}

/* 
 * Initialize GPIOs needed by DTB mode chosen (for debug use only).
 * Only DTB0 and DTB7 configurations are coded until now.
 * Do not forget to program the RF SPI register addr 0x30 () with the DTB cfg and DTB enable.
 */
void gpio_lld_dtb_init(uint8_t dtbMode) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  uint32_t usePA = 0, pinPA = 0;
  uint32_t usePB = 0, pinPB = 0;
  uint32_t usePC = 0, pinPC = 0;
  
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  if (dtbMode == 0) {
    GPIO_InitStruct.Alternate = GPIO_AF6_RF_DTB0;
    
    usePA = 1;
    pinPA = (GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3| GPIO_PIN_4 |
             GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    
    usePC = 1;
    pinPC = (GPIO_PIN_14 | GPIO_PIN_15);
  }
  
  if (dtbMode == 7) {
    GPIO_InitStruct.Alternate = GPIO_AF6_RF_DTB7;
    
    usePA = 1;
#ifdef STM32WB35xx
    pinPA = (GPIO_PIN_8 | GPIO_PIN_9| GPIO_PIN_10| GPIO_PIN_11| GPIO_PIN_13| GPIO_PIN_14);
#else
    pinPA = (GPIO_PIN_8 | GPIO_PIN_9| GPIO_PIN_10| GPIO_PIN_11);
#endif
    
    usePB = 1;
#ifdef STM32WB35xx
    //pinPB = (GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8| GPIO_PIN_10| GPIO_PIN_11);
    // PB 7 is in conflict with USART
    pinPB = (GPIO_PIN_2 | GPIO_PIN_8);
#else
    //pinPB = (GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8| GPIO_PIN_10| GPIO_PIN_11);
    // PB 7 is in conflict with USART
    pinPB = (GPIO_PIN_2 | GPIO_PIN_8| GPIO_PIN_10| GPIO_PIN_11);
#endif
  }
  
  if (usePA == 1) {
#ifdef CORE_CM4
    // Enable GPIO PA clock for M4 use
    __HAL_RCC_GPIOA_CLK_ENABLE();
#else
    // Enable GPIO PA clock for M0 use
    __HAL_RCC_C2GPIOA_CLK_ENABLE();
#endif
    
    GPIO_InitStruct.Pin = pinPA;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  
  if (usePB == 1) {
#ifdef CORE_CM4
    // Enable GPIO PB clock for M4 use
    __HAL_RCC_GPIOB_CLK_ENABLE();
#else
    // Enable GPIO PB clock for M0 use
    __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
    
    GPIO_InitStruct.Pin = pinPB;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  
  if (usePC == 1) {
#ifdef CORE_CM4
    // Enable GPIO PC clock for M4 use
    __HAL_RCC_GPIOC_CLK_ENABLE();
#else
    // Enable GPIO PC clock for M0 use
    __HAL_RCC_C2GPIOC_CLK_ENABLE();
#endif
    
    GPIO_InitStruct.Pin = pinPC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
}

void gpio_lld_dtb_deInit(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
#ifdef CORE_CM4
  // Enable GPIO PB clock for M4 use
  __HAL_RCC_GPIOB_CLK_ENABLE();
#else
  // Enable GPIO PB clock for M0 use
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
#endif
  
  // configure the GPIO to be set to '1' during frame TX
  GPIO_InitStruct.Pin = (GPIO_PIN_8);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH  ;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}
#endif /* ! USE_SIMU and ! USE_FPGA */

// Do not initialize LED GPIOs as they are already initialized by BSP if possible (depending on board and possible GPIOs conflicts).
// So just offer the toggle possibility for debug purposes
/* Toggle LED1 */
void gpio_lld_led1_toggle(void) {
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}

/* Toggle LED2 */
void gpio_lld_led2_toggle(void) {
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

/* Toggle LED3 */
void gpio_lld_led3_toggle(void) {
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}

/* Initialize GPIOs fused by PHY valid CLI */
void gpio_lld_phy_dbg_init(void) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  
#ifdef CORE_CM4
  // Enable GPIO clocks for M4 use
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
#else
  // Enable GPIO clocks for M0 use
  __HAL_RCC_C2GPIOA_CLK_ENABLE();
  __HAL_RCC_C2GPIOB_CLK_ENABLE();
  __HAL_RCC_C2GPIOC_CLK_ENABLE();
#endif
  
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#ifndef STM32WB35xx
  GPIO_InitStruct.Pin   = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin   = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

/* Set GPIO PA0 to '1' */
void gpio_lld_phy_pa0_up(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}

/* Set GPIO PA0 to '0' */
void gpio_lld_phy_pa0_down(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

/* Set GPIO PA1 to '1' */
void gpio_lld_phy_pa1_up(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
}

/* Set GPIO PA1 to '0' */
void gpio_lld_phy_pa1_down(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* Set GPIO PA5 to '1' */
void gpio_lld_phy_pa5_up(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

/* Set GPIO PA5 to '0' */
void gpio_lld_phy_pa5_down(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

/* Set GPIO PA7 to '1' */
void gpio_lld_phy_pa7_up(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

/* Set GPIO PA7 to '0' */
void gpio_lld_phy_pa7_down(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

#ifndef STM32WB35xx
/* Set GPIO PB12 to '1' */
void gpio_lld_phy_pb12_up(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

/* Set GPIO PB12 to '0' */
void gpio_lld_phy_pb12_down(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

/* Set GPIO PB13 to '1' */
void gpio_lld_phy_pb13_up(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
}

/* Set GPIO PB13 to '0' */
void gpio_lld_phy_pb13_down(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}

/* Set GPIO PB14 to '1' */
void gpio_lld_phy_pb14_up(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}

/* Set GPIO PB14 to '0' */
void gpio_lld_phy_pb14_down(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}

/* Set GPIO PB15 to '1' */
void gpio_lld_phy_pb15_up(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

/* Set GPIO PB15 to '0' */
void gpio_lld_phy_pb15_down(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* Set GPIO PC5 to '1' */
void gpio_lld_phy_pc5_up(void) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}

/* Set GPIO PC5 to '0' */
void gpio_lld_phy_pc5_down(void) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
}
#endif
