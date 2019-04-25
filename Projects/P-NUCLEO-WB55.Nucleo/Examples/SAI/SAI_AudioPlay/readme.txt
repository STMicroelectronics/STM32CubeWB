/**
  @page SAI_AudioPlay  Description of the SAI audio play example
  
  @verbatim
  ******************************************************************************
  * @file    SAI/SAI_AudioPlay/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SAI audio play example.
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

Use of the SAI HAL API to play an audio file in DMA circular mode and handle the buffer update.

The SAI feature is part of STM32WB55xx. 

The P-NUCLEO-WB55 kit is not suitable to demonstrate this feature as it does not contains any audio codec
like wm8994. 

The hal library for SAI driver is compatible between STM32L4xx_HAL_Driver and STM32WBxx_HAL_Driver.

You can retrieve the STM32L4xx_HAL_Driver under the stm32cubel4 package (https://www.st.com/stm32cubel4).

Inside the stm32cubel4 package, this SAI_AudioPlay example is available with the following boards:
  32L476GDISCOVERY (Projects\32L476GDISCOVERY\Examples\SAI\SAI_AudioPlay)
  32L4R9IDISCOVERY (Projects\32L4R9IDISCOVERY\Examples\SAI\SAI_AudioPlay)
  STM32L476G-EVAL  (Projects\STM32L476G-EVAL\Examples\SAI\SAI_AudioPlay)
  STM32L4R9I-EVAL  (Projects\STM32L4R9I-EVAL\Examples\SAI\SAI_AudioPlay)
  You can use those examples and boards schematics for your own implementation.
  
The required adaptation from STM32L476G-EVAL project, given as information:
  In all files, rename and adapt include files name to your situation.
  Copy the system_stm32wbxx.c from Drivers\CMSIS\Device\ST\STM32WBxx\Source\Templates to Src.
  Copy the stm32wbxx_hal_conf_template.h from Drivers\STM32WBxx_HAL_Driver\Inc to Inc\stm32wbxx_hal_conf.h.
  Adapt the main.c/SystemClock_Config(void) function to your board.
  In main.c/Playback_Init(void) function, adapt the configuration of the SAI peripheral to your board.
  In main.c/HAL_SAI_MspInit function:
    Adapt the GPIO initialization to your board.
    Adapt the DMA configuration: Request parameter can be DMA_REQUEST_SAI1_A or DMA_REQUEST_SAI1_B.
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
