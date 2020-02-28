/**
  @page I2S_AUDIO  How to use the Audio features with I2S peripheral
  
  @verbatim
  ******************************************************************************
  * @file    I2S/I2S_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2S Audio Example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to play an audio file through the I2S peripheral and DMA-based transfer
and using an external codec.

The I2S feature is part of STM32WB35xx. 

The NUCLEO-WB35CE board is not suitable to demonstrate this feature as it does not 
contain any audio codec driver.

The hal library for I2S driver is compatible between STM32F4xx_HAL_Driver and STM32WBxx_HAL_Driver.

You can retrieve the STM32F4xx_HAL_Driver under the stm32cubef4 package (https://www.st.com/stm32cubef4).

Inside the stm32cubef4 package, this I2S_Audio example is available with the following boards:
  STM32412G-DISCOVERY  (Projects\STM32F412G-Discovery\Examples\I2S\I2S_Audio)
  You can use those examples and boards schematics for your own implementation.
  
The required adaptation from STM32412G-DISCOVERY project, given as information:
  In all files, rename and adapt include files name to your situation.
  Copy the system_stm32wbxx.c from Drivers\CMSIS\Device\ST\STM32WBxx\Source\Templates to Src.
  Copy the stm32wbxx_hal_conf_template.h from Drivers\STM32WBxx_HAL_Driver\Inc to Inc\stm32wbxx_hal_conf.h.
  Adapt the main.c/SystemClock_Config(void) function to your board.
  Adapt the GPIO initialization to your board.
  Create the BSP (containing the initialisation of the audio codec driver) of your board.
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

