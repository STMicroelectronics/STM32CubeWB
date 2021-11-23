/**
  @page EXTI_ToggleLedOnIT_Init EXTI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EXTI example.
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

This example describes how to configure the EXTI and use
GPIOs to toggle the user LEDs available on the board when
a user button is pressed. This example is based on the
STM32WBxx LL API. Peripheral initialization is done using LL
initialization function to demonstrate LL init usage.

In this example, one EXTI line (External line 0) is configured to generate
an interrupt on each falling edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - External line 0 is connected to PA.00 pin
      - when falling edge is detected on External line 0 by pressing User push-button (SW1), LED2 toggles

On NUCLEO-WB15CC:
    - External line 0 is connected to User push-button (SW1)

@par Keywords

System, GPIO, Output, Alternate function, EXTI, Toggle


@par Directory contents 

  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.
    
  - This example has been tested with NUCLEO-WB15CC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
