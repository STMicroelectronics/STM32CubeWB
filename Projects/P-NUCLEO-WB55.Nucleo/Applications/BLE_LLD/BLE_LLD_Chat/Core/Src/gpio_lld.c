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

/* Set PHY GPIO_HARD_FAULT to '1' */
void gpio_lld_phy_gpioHardFault_up(void) {
  HAL_GPIO_WritePin(GPIO_HARD_FAULT_PORT, GPIO_HARD_FAULT_PIN, GPIO_PIN_SET);
}

/* Set PHY GPIO_HARD_FAULT to '0' */
void gpio_lld_phy_gpioHardFault_down(void) {
  HAL_GPIO_WritePin(GPIO_HARD_FAULT_PORT, GPIO_HARD_FAULT_PIN, GPIO_PIN_RESET);
}

/* Initialize GPIOs used by USART */
void gpio_lld_usart_init(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  USART_TX_GPIO_CLK_ENABLE();
  USART_RX_GPIO_CLK_ENABLE();

  /* Common configuration to Tx and Rx */
  gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull       = GPIO_NOPULL;
  gpioinitstruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;

  /* Configure USART Tx */
  gpioinitstruct.Pin        = USART_TX_PIN;
  gpioinitstruct.Alternate  = USART_TX_AF;
  HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpioinitstruct);

  /* Configure USART Rx */
  gpioinitstruct.Pin        = USART_RX_PIN;
  gpioinitstruct.Alternate  = USART_RX_AF;
  HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpioinitstruct);

  /*** Configure the USART peripheral ***/
  /* Enable USART clock */
  USART_CLK_ENABLE();
}

/* De-initialize GPIOs used by USART */
void gpio_lld_usart_deInit(void) {
  HAL_GPIO_DeInit(USART_TX_GPIO_PORT, USART_TX_PIN);
  HAL_GPIO_DeInit(USART_RX_GPIO_PORT, USART_RX_PIN);
  /* Do not disable clocks as they could be used by others GPIOs and it seems
  to not need power in STOP mode */
}

/* Initialize GPIOs used by LPUART */
void gpio_lld_lpuart_init(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  LPUART_TX_GPIO_CLK_ENABLE();
  LPUART_RX_GPIO_CLK_ENABLE();

  /* Common configuration to Tx and Rx */
  gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull       = GPIO_NOPULL;
  gpioinitstruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;

  /* Configure LPUART Tx */
  gpioinitstruct.Pin        = LPUART_TX_PIN;
  gpioinitstruct.Alternate  = LPUART_TX_AF;
  HAL_GPIO_Init(LPUART_TX_GPIO_PORT, &gpioinitstruct);

  /* Configure LPUART Rx */
  gpioinitstruct.Pin        = LPUART_RX_PIN;
  gpioinitstruct.Alternate  = LPUART_RX_AF;
  HAL_GPIO_Init(LPUART_RX_GPIO_PORT, &gpioinitstruct);

  /*** Configure the LPUART peripheral ***/
  /* Enable LPUART clock */
  LPUART_CLK_ENABLE();
}

/* De-initialize GPIOs used by LPUART */
void gpio_lld_lpuart_deInit(void) {
  HAL_GPIO_DeInit(LPUART_TX_GPIO_PORT, LPUART_TX_PIN);
  HAL_GPIO_DeInit(LPUART_RX_GPIO_PORT, LPUART_RX_PIN);
  /* Do not disable clocks as they could be used by others GPIOs and it seems
  to not need power in STOP mode */
}


