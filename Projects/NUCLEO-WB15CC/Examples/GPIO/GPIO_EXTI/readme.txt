/**
  @page GPIO_EXTI GPIO EXTI example
  
  @verbatim
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO EXTI example.
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

How to configure external interrupt lines.

In this example, one EXTI line (External line 0) is configured to generate
an interrupt on each falling edge.
In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - External line 0 is connected to PA.00 pin
      - when falling edge is detected on External line 0 by pressing User push-button (SW1), LED2 toggles once

On NUCLEO-WB15CC:
    - External line 0 is connected to User push-button (SW1)

In this example, HCLK is configured at 64 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, GPIO, EXTI, Output, Alternate function, Push-pull, Toggle

@par Directory contents 

  - GPIO/GPIO_EXTI/Inc/nucleo_wb15cc_conf.h     BSP configuration file
  - GPIO/GPIO_EXTI/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_EXTI/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - GPIO/GPIO_EXTI/Inc/main.h                  Header for main.c module  
  - GPIO/GPIO_EXTI/Src/stm32wbxx_it.c          Interrupt handlers
  - GPIO/GPIO_EXTI/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - GPIO/GPIO_EXTI/Src/main.c                  Main program
  - GPIO/GPIO_EXTI/Src/system_stm32wbxx.c      STM32WBxx system source file

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
