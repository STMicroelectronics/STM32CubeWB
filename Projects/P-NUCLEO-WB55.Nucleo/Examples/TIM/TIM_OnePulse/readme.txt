/**
  @page TIM_OnePulse TIM One Pulse example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

Use of the TIM peripheral to generate a single pulse when 
an external signal rising edge is received on the timer input pin.

Clock setup for TIM2
================================

  TIM2CLK = SystemCoreClock = 64 MHz.
  
  Prescaler = (TIM2CLK /TIM2 counter clock) - 1
  
  The prescaler value is computed in order to have TIM2 counter clock 
  set at 1000000 Hz.
  
  The Autoreload value is 65535 (TIM2->ARR), so the maximum frequency value to 
  trigger the TIM2 input is 1000000/65535 [Hz].
 
Configuration of TIM2 in One Pulse Mode
===================================================
 
  - The external signal is connected to TIM2_CH2 pin (PA.01), 
    and a rising edge on this input is used to trigger the Timer.
  - The One Pulse signal is output on TIM2_CH1 (PA.00).

  The delay value is fixed to:
   - Delay =  CCR1/TIM2 counter clock 
           = 16383 / 1000000 [sec]
           
  The pulse value is fixed to : 
   - Pulse value = (TIM_Period - TIM_Pulse)/TIM2 counter clock  
                 = (65535 - 16383) / 1000000 [sec]

  The one pulse waveform can be displayed using an oscilloscope and it looks
  like this.
  
                               ____
                               |   |
  CH2 _________________________|   |__________________________________________
 
                                             ___________________________
                                            |                           |
  CH1 ______________________________________|                           |_____
                               <---Delay----><------Pulse--------------->
  
@par Keywords

Timer, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

@par Directory contents 

  - TIM/TIM_OnePulse/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - TIM/TIM_OnePulse/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OnePulse/Src/stm32wbxx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - TIM/TIM_OnePulse/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

   - This example runs on STM32WB55xx devices.
   - In this example, the clock is set to 64 MHz.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

  - P-NUCLEO-WB55 Set-up
   - Connect the external signal to the TIM2_CH2 pin (PA.01) (pin 32 in CN7 connector)
   - Connect the TIM2_CH1 pin(PA.00) (pin 34 in CN7 connector) to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
