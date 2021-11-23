/**
  @page TIM_OCToggle TIM_OCToggle example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four different 
  *          signals with four different frequencies.
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

Configuration of the TIM peripheral to generate four different 
signals at four different frequencies.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32WB55RGVx Devices :
  The CPU at 64 MHz 
  The TIM2 frequency is set to SystemCoreClock, and the objective is
  to get TIM2 counter clock at 1 MHz so the Prescaler is computed as following:
     - Prescaler = (TIM2CLK /TIM2 counter clock) - 1
   
  SystemCoreClock is set to 64 MHz for STM32WBxx Devices.

  The TIM2 CCR1 register value is equal to 625:
  CC1 update rate = TIM2 counter clock / CCR1_Val = 1600 Hz,
  so the TIM2 Channel 1 generates a periodic signal with a frequency equal to 800 Hz.

  The TIM2 CCR2 register value is equal to 1250:
  CC2 update rate = TIM2 counter clock / CCR2_Val = 800 Hz,
  so the TIM2 channel 2 generates a periodic signal with a frequency equal to 400 Hz.

  The TIM2 CCR3 register value is equal to 2500:
  CC3 update rate = TIM2 counter clock / CCR3_Val = 400 Hz,
  so the TIM2 channel 3 generates a periodic signal with a frequency equal to 200 Hz.

  The TIM2 CCR4 register value is equal to 5000:
  CC4 update rate = TIM2 counter clock / CCR4_Val =  200 Hz,
  so the TIM2 channel 4 generates a periodic signal with a frequency equal to 100 Hz.

@note PWM signal frequency values mentioned above are theoretical (obtained when the system clock frequency
      is exactly 64 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

LED3 is ON when there are an error.
@par Keywords

Timer, Output, signals, Output compare toggle, PWM, Oscilloscope

@par Directory contents

  - TIM/TIM_OCToggle/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCToggle/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - TIM/TIM_OCToggle/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCToggle/Src/stm32wbxx_it.c          Interrupt handlers
  - TIM/TIM_OCToggle/Src/main.c                  Main program
  - TIM/TIM_OCToggle/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCToggle/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55 
    board and can be easily tailored to any other supported device 
    and development board.      

  - P-NUCLEO-WB55 Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - PA0: (TIM2_CH1) (pin 34 in CN7 connector)
      - PA1: (TIM2_CH2) (pin 32 in CN7 connector)
      - PA2: (TIM2_CH3) (pin 35 in CN10 connector)
      - PA3: (TIM2_CH4) (pin 37 in CN10 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
