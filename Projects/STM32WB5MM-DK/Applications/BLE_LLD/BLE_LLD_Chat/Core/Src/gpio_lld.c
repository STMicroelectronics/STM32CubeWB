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

