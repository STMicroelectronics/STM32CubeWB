/**
  @page BSP example
  
  @verbatim
  ******************************************************************************
  * @file    BSP/BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

How to use the bsp API of the NUCLEO-WB55.USBDongle board. 

The 3 LEDs toggle one after the other.
When the button is pressed, the toggle frequency is changed.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - BSP/BSP/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BSP/BSP/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BSP/BSP/Inc/main.h                  Header for main.c module  
  - BSP/BSP/Src/stm32wbxx_it.c          Interrupt handlers
  - BSP/BSP/Src/main.c                  Main program
  - BSP/BSP/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STM32WB55xx Usb Dongle board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
