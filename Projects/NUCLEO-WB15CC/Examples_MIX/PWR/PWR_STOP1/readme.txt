/**
  @page PWR_STOP1 PWR Example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_MIX/PWR/PWR_STOP1/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR_STOP1 example.
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
 
How to enter the STOP 1 mode and wake up from this mode by using external 
reset or wakeup interrupt (all the RCC function calls use RCC LL API 
for minimizing footprint and maximizing performance).

In the associated software, the system clock is set to 64 MHz, an EXTI line
is connected to the user button through PA.0 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED2 is toggled in order to indicate whether the MCU is in STOP 1 mode 
or RUN mode.

5 seconds after start-up, the system automatically enters STOP 1 mode and 
LED2 stops toggling.

The User push-button (SW1) can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering STOP 1 mode again. 

Two leds LED2 and LED1 are used to monitor the system state as following:
 - LED2 toggling: system in RUN mode
 - LED2 off : system in STOP 1 mode
 - LED2 and LED1 ON: configuration failed (system will go to an infinite loop)

These steps are repeated in an infinite loop.

@note To measure MCU current consumption on board STM32WB Nucleo,
      board configuration must be applied:
      - remove all jumpers on connector JP5 to avoid leakages between ST-Link circuitry and STM32WB device.
      - remove jumper JP2 and connect an amperemeter to measure current between the 2 connectors of JP2.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
  
@par Keywords

Power, PWR, stop mode, wake-up, external reset, Interrupt, low power mode    

@par Directory contents 

  - Examples_MIX/PWR/PWR_STOP1/Inc/stm32wbxx_conf.h         HAL Configuration file
  - Examples_MIX/PWR/PWR_STOP1/Inc/stm32wbxx_it.h           Header for stm32wbxx_it.c
  - Examples_MIX/PWR/PWR_STOP1/Inc/main.h                   Header file for main.c
  - Examples_MIX/PWR/PWR_STOP1/Src/system_stm32wbxx.c       STM32WBxx system clock configuration file
  - Examples_MIX/PWR/PWR_STOP1/Src/stm32wbxx_it.c           Interrupt handlers
  - Examples_MIX/PWR/PWR_STOP1/Src/main.c                   Main program
  - Examples_MIX/PWR/PWR_STOP1/Src/stm32wbxx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32WBxx devices
    

  - This example has been tested with STMicroelectronics NUCLEO-WB15CC
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-WB15CC set-up:
    - Use LED2 and LED1 connected respectively to PB.00 and PB.05 pins
    - User push-button (SW1) connected to pin PA.00 (External line 0)
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
