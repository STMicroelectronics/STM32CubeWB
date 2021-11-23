/**
  @page GPIO_InfiniteLedToggling GPIO example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/GPIO/GPIO_InfiniteLedToggling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO example.
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

How to configure and use GPIOs to toggle the on-board user LEDs 
every 250 ms. This example is based on the STM32WBxx LL API. The peripheral 
is initialized with LL unitary service functions to optimize 
for performance and size.

PB.00 IO (configured in output pushpull mode) toggles in a forever loop.
On P-NUCLEO-WB55 board this IO is connected to LED2.

In this example, HCLK is configured at 16 MHz.

@par Keywords

System, GPIO, Output, No pull, Speed, Push-pull, Toggle


@par Directory contents 

  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - GPIO/GPIO_InfiniteLedToggling/Inc/main.h                  Header for main.c module
  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32_assert.h          Template file to include assert_failed function
  - GPIO/GPIO_InfiniteLedToggling/Src/stm32wbxx_it.c          Interrupt handlers
  - GPIO/GPIO_InfiniteLedToggling/Src/main.c                  Main program
  - GPIO/GPIO_InfiniteLedToggling/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
