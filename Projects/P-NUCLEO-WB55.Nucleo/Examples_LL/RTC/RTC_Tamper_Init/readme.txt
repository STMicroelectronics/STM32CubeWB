/**
  @page RTC_Tamper_Init RTC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Tamper_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
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

Configuration of the Tamper using the RTC LL API. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
MSI as clock source.

The RTC peripheral configuration is ensured by the Configure_RTC() function 
(configure of the needed RTC resources according to the used hardware CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

The associated firmware performs the following:
1. It configures the Tamper pin to be falling edge, and enables the Tamper 
   interrupt.
2. It writes the data to all RTC Backup data registers, then check whether the
   data were correctly written. If yes, LED2 toggles with a fast period,
   otherwise LED2 toggles with a period of 1s.
3. Applying a high level on the tamper 1 pin (PC13),
   the RTC backup data registers are reset and the Tamper interrupt is generated. 
   The corresponding ISR then checks whether the RTC Backup data registers are cleared.
   If yes LED2 turns on, otherwise LED2 toggles with a period of 1s.

@Note: On this board, tamper is not connected by default to User push-button (SW1) (pin PC.04).
       Therefore, an external connection is needed to use the selected tamper pin (PC.13):
       - to generate a tamper event using User push-button (SW1): connect PC.13 to PC.04.
       - to generate a tamper event using external signal: connect PC.13 to voltage level 0V or 3.3V.

@par Keywords

System, RTC, Tamper, Reset, LSE, LSI

@par Directory contents 

  - RTC/RTC_Tamper_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - RTC/RTC_Tamper_Init/Inc/main.h                  Header for main.c module
  - RTC/RTC_Tamper_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_Tamper_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - RTC/RTC_Tamper_Init/Src/main.c                  Main program
  - RTC/RTC_Tamper_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


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
