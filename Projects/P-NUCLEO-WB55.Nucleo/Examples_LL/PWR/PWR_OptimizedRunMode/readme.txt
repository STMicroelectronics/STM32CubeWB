/**
  @page PWR_OptimizedRunMode PWR standby example
  
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

In the associated software, the system clock is set to 64 MHz, an EXTI line
is connected to the User push-button (SW1) through PC.04 and configured to generate an 
interrupt on falling edge upon key press.

After start-up LED2 is toggling FAST(100-ms blinking period), indicates that device 
is running at 64MHz.

LED2 toggling speed is controlled by variable "uhLedBlinkSpeed".

The User push-button (SW1) can be pressed at any time to change Frequency, VCore(VOS) 
and Low Power Run mode. 

Initial STATE: 
--> Freq: 64MHz, VCore 1.2V, Core in Run Mode
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
--> Freq: 64MHz, VCore 1.2V, Core in Run Mode
--> LED2 toggling FAST (100ms) in infinite loop


@note To measure MCU current consumption on board STM32WB Nucleo,
      board configuration must be applied:
      - remove all jumpers on connector JP5 to avoid leakages between ST-Link circuitry and STM32WB device.
      - remove jumper JP2 and connect an amperemeter to measure current between the 2 connectors of JP2.
      NB: LED2 have an impact on power consumption. 
          Remove LED2 blinking to have a stable power consumption, 
          comment line  "#define USE_LED" in main.c file  
		  
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

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.

  - P-NUCLEO-WB55 Set-up
    - LED2 connected to PB.00 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
