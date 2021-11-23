/**
  @page IWDG_RefreshUntilUserEvent_Init IWDG example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG_RefreshUntilUserEvent_Init example.
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

How to configure the IWDG peripheral to ensure periodical counter update and 
generate an MCU IWDG reset when a User push-button (SW1) is pressed. The peripheral 
is initialized with LL unitary service functions to optimize 
for performance and size.

Example Configuration:
Configure the IWDG (prescaler, counter) and enable it.
Infinite refresh of the IWDG down-counter done in the main loop.
LED2 is blinking fast & continuously.

Example Execution:
When User push-button (SW1) is pressed, the down-counter automatic refresh mechanism is 
disable and thus, reset will occur. After a reset, when re-entering in the main, 
RCC IWDG Reset Flag will be checked and if we are back from a IWDG reset the LED2
will be switch ON.

Waiting a new User push-button (SW1) pressed to re-activate the IWDG

@par Keywords

System, IWDG, reload counter, MCU Reset

@par Directory contents 

  - IWDG/IWDG_RefreshUntilUserEvent_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - IWDG/IWDG_RefreshUntilUserEvent_Init/Inc/main.h                        Header for main.c module
  - IWDG/IWDG_RefreshUntilUserEvent_Init/Inc/stm32_assert.h                Template file to include assert_failed function
  - IWDG/IWDG_RefreshUntilUserEvent_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - IWDG/IWDG_RefreshUntilUserEvent_Init/Src/main.c                        Main program
  - IWDG/IWDG_RefreshUntilUserEvent_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


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
