/**
  @page PWR_EnterStopMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStopMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Stop 2 mode example.
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

How to enter the Stop 2 mode.

After start-up LED2 is toggling during 5 seconds, then the system automatically 
enter in Stop 2 mode (Final state).

LED2 is used to monitor the system state as follows:
 - LED2 toggling : system in RUN mode
 - LED2 off : system in Stop 2 mode

@note To measure MCU current consumption on board STM32WB Nucleo,
      board configuration must be applied:
      - remove all jumpers on connector JP5 to avoid leakages between ST-Link circuitry and STM32WB device.
      - remove jumper JP2 and connect an amperemeter to measure current between the 2 connectors of JP2.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note On STM32WB, both CPU1 and CPU2 must be in stop mode to set the entire system in stop mode.
      In this example, CPU2 is not started-up and CPU1 configures the CPU2 stop mode.
      If this example would be ported to another application, user must manage CPU2 entering in stop mode.

@par Keywords

Power, PWR, stop mode, Interrupt, Low Power

@par Directory contents 

  - PWR/PWR_EnterStopMode/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStopMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStopMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStopMode/Src/stm32wbxx_it.c          Interrupt handlers
  - PWR/PWR_EnterStopMode/Src/main.c                  Main program
  - PWR/PWR_EnterStopMode/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.

  - P-NUCLEO-WB55 Set-up
    - LED2 connected to PB.00 pin

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
