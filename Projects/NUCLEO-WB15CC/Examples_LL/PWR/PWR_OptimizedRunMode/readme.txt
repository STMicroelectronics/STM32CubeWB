/**
  @page PWR_OptimizedRunMode PWR PWR_OptimizedRunMode
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_OptimizedRunMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Optimized Run Mode example.
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

How to increase/decrease frequency and VCORE and how to enter/exit the
Low-power run mode.

In the associated software, the system clock is set to 48MHz, an EXTI line
is connected to the User push-button (SW1) through PA.00 and configured to generate an 
interrupt on falling edge upon key press.

After start-up LED2 is toggling FAST (100ms blinking period), indicates that device 
is running at 48MHz.

LED2 toggling speed is controlled by variable "uhLedBlinkSpeed".

The User push-button (SW1) can be pressed at any time to change Frequency, VCore(VOS) 
and Low Power Run mode. 

Initial STATE: 
--> Freq: 48MHz, VCore 1.2V, Core in Run Mode
--> LED2 toggling FAST (100ms)   - wait User push-button (SW1) action

STATE 2: 
User push-button (SW1) pressed:
--> Freq: 16MHz, VCore 1.0V, Core in Run Mode
--> LED2 toggling MEDIUM (200ms) - wait User push-button (SW1) action

STATE 3: 
User push-button (SW1) pressed:
--> Freq:  100KHz, VCore 1.0V, Core in Low Power Run Mode
--> LED2 toggling SLOW (400ms)   - wait User push-button (SW1) action

STATE 4: 
User push-button (SW1) pressed:
--> Freq: 16MHz, VCore 1.0V, Core in Run Mode
--> LED2 toggling MEDIUM (200ms) - wait User push-button (SW1) action

Final STATE: 
User push-button (SW1) pressed:
--> Freq: 48MHz, VCore 1.2V, Core in Run Mode
--> LED2 toggling FAST (100ms) in infinite loop

@note This example may not be used in debug mode depending on IDE and debugger
      configuration selected, due to stsem low frequency and low power mode
      constraints.

@par Keywords

Power, PWR, Low-power run mode, Interrupt, VCORE, Low Power

@par Directory contents 

  - PWR/PWR_OptimizedRunMode/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - PWR/PWR_OptimizedRunMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_OptimizedRunMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_OptimizedRunMode/Src/stm32wbxx_it.c          Interrupt handlers
  - PWR/PWR_OptimizedRunMode/Src/main.c                  Main program
  - PWR/PWR_OptimizedRunMode/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-WB15CC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-WB15CC Set-up
    - LED2 connected to pin PB.00
    - User push-button connected to pin PA.00

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
