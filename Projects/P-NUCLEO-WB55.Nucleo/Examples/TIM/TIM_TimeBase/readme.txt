/**
  @page TIM_TimeBase Time Base example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Time Base example
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

This example shows how to configure the TIM peripheral to generate a time base of 
one second with the corresponding Interrupt request.
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32WB55RGVx Devices :
The CPU at 64 MHz 

    In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock
    To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM2CLK / TIM2 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 64 MHz for STM32WBxx Devices.

The TIM2 ARR register value is equal to 10000 - 1, 
Update rate = TIM2 counter clock / (Period + 1) = 1 Hz,
So the TIM2 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM update 
interrupt is generated and, in the handler routine, pin PB.00 (connected to LED2 on board P-NUCLEO-WB55)
is toggled. So led blinks at the following frequency: 0.5Hz.

In case of error, LED3 is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, TIM, time base , Interrupt, Clock source

@par Directory contents

  - TIM/TIM_TimeBase/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - TIM/TIM_TimeBase/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase/Inc/main.h                  Header for main.c module  
  - TIM/TIM_TimeBase/Src/stm32wbxx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase/Src/main.c                  Main program
  - TIM/TIM_TimeBase/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - TIM/TIM_TimeBase/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  - In this example, the clock is set to 64 MHz.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.      

  - P-NUCLEO-WB55 Set-up
    - Use LED2 connected to PB.00 pin (pin 22 in CN10 connector) and connect it on an oscilloscope 
      to show the time base signal.  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
