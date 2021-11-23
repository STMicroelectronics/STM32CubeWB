/**
  @page TSC_BasicAcquisition_Interrupt Touch-Sensing basic acquisition using interrupt

  @verbatim
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Interrupt/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TSC basic acquisition interrupt example.
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

@par Example Description

Use of the TSC HAL API to perform continuous acquisitions of one channel in Interrupt mode.

The TSC feature is part of STM32WB55xx. 

The P-NUCLEO-WB55 kit is not suitable to demonstrate this feature as it does not contains 
any touch-sensing button.

The hal library for TSC driver is compatible between STM32L4xx_HAL_Driver and STM32WBxx_HAL_Driver.

You can retrieve the STM32L4xx_HAL_Driver under the stm32cubel4 package (https://www.st.com/stm32cubel4).

Inside the stm32cubel4 package, this TSC_BasicAcquisition_Interrupt example is available with the following boards:
  STM32L476G-EVAL  (Projects\STM32L476G-EVAL\Examples\TSC\TSC_BasicAcquisition_Interrupt)
  You can use those examples and boards schematics for your own implementation.

The required adaptation from STM32L476G-EVAL project, given as information:
  In all files, rename and adapt include files name to your situation.
  Copy the system_stm32wbxx.c from Drivers\CMSIS\Device\ST\STM32WBxx\Source\Templates to Src.
  Copy the stm32wbxx_hal_conf_template.h from Drivers\STM32WBxx_HAL_Driver\Inc to Inc\stm32wbxx_hal_conf.h.
  Adapt the main.c/SystemClock_Config(void) function to your board.
  Adapt the GPIO initialization to your board.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
