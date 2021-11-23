/**
  @page TouchSensing 1 touchkey sensor example
 
  @verbatim
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_1touchkey\readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TouchSensing 1 touchkey sensor example.
  ******************************************************************************
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

Use of the STMTouch driver with 1 touchkey sensor.

The TSC feature is part of STM32WB55xx. 
The STM32_TouchSensing_Library is provided under Middlewares\ST\STM32_TouchSensing_Library.

The P-NUCLEO-WB55 kit is not suitable to demonstrate this feature as it does not contains 
any touch-sensing button.

The hal library for TSC driver is compatible between STM32L4xx_HAL_Driver and STM32WBxx_HAL_Driver.
The STM32_TouchSensing_Library is compatible between STM32cubel4 and STM32cubewb.

You can retrieve the STM32L4xx_HAL_Driver under the stm32cubel4 package (https://www.st.com/stm32cubel4).

Inside the stm32cubel4 package, this TouchSensing_1touchkey example is available with the following boards:
  STM32L476G-EVAL  (Projects\STM32L476G-EVAL\Applications\TouchSensing\TouchSensing_1touchKey)
  STM32L4R9I-EVAL  (Projects\STM32L4R9I-EVAL\Applications\TouchSensing\TouchSensing_1touchKey)
  You can use those examples and boards schematics for your own implementation.

The required adaptation from STM32L476G-EVAL project, given as information:
  In all files, rename and adapt include files name to your situation.
  Copy the system_stm32wbxx.c from Drivers\CMSIS\Device\ST\STM32WBxx\Source\Templates to Src.
  Copy the stm32wbxx_hal_conf_template.h from Drivers\STM32WBxx_HAL_Driver\Inc to Inc\stm32wbxx_hal_conf.h.
  Adapt the main.c/SystemClock_Config(void) function to your board.
  Adapt the GPIO initialization to your board.
 
@par Keywords

  Touch sensing, TouchSensing, TSC

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
