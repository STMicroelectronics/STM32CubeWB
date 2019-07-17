/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    BSP/BSP_Example/readme.txt 
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

This example describes how to use the bsp API. 

The 3 LEDs toggle one after the other in a forever loop. This shows the usage of
BSP_LED_Toggle, BSP_LED_On and BSP_LED_Off.

When the buttons SW1, SW2 or SW3 are pressed, the toggle delay is changed.
  - SW1 set the delay to 100ms.
  - SW2 set the delay to 500ms.
  - SW3 set the delay to 1000ms.

LED3 demonstrate BSP_LED_On and BSP_LED_Off in the same loop and so the toggle frequency is different from LED1 and LED2
which are demonstrating BSP_LED_Toggle.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
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
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
