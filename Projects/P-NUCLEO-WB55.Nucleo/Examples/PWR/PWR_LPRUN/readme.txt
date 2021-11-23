/**
  @page PWR_LPRUN Low Power Run Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_LPRUN/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Low Power Run Mode example.
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
 
How to enter and exit the Low-power run mode.

In the associated software, the system clock is set to 64 MHz.
The SysTick is programmed to generate an interrupt each 1 ms.

The User push-button (SW1) can be pressed at any time to exit from Low Power Run. 
The software then comes back in Run mode for 5 sec. before automatically 
entering LP Run mode again. 

LED2 is used to monitor the system state as follows:
 - LED2 toggling : system in Run mode
 - LED2 off : system in LP Run mode
 - LED2 on : system in error

These steps are repeated in an infinite loop.

@note To measure the current consumption in LP SLEEP mode, remove JP2 jumper 
      and connect an amperemeter to JP2 to measure IDD current.

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

Power, PWR, Low Power, Run mode, Interrupt, EXTI, Wakeup, External reset

@par Directory contents 

  - PWR/PWR_LPRUN/Inc/stm32wbxx_conf.h         HAL Configuration file
  - PWR/PWR_LPRUN/Inc/stm32wbxx_it.h           Header for stm32wbxx_it.c
  - PWR/PWR_LPRUN/Inc/main.h                         Header file for main.c
  - PWR/PWR_LPRUN/Src/system_stm32wbxx.c       STM32WBxx system clock configuration file
  - PWR/PWR_LPRUN/Src/stm32wbxx_it.c           Interrupt handlers
  - PWR/PWR_LPRUN/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - PWR/PWR_LPRUN/Src/main.c                   Main program

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.

  - P-NUCLEO-WB55 set-up:
    - LED2 connected to PB.00 pin
    - Use the User push-button (SW1) connected to pin PC.04.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
