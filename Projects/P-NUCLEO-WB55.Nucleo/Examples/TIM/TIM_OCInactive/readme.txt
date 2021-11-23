/**
  @page TIM_OCInactive TIM_OCInactive example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OCInactive/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four 
  *          delayed signals.
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

Configuration of the TIM peripheral in Output Compare Inactive mode 
with the corresponding Interrupt requests for each channel.

  The TIM2 frequency is set to SystemCoreClock, and the objective is
  to get TIM2 counter clock at 10 kHz so the Prescaler is computed as following:
     - Prescaler = (TIM2CLK /TIM2 counter clock) - 1
   
  SystemCoreClock is set to 64 MHz for STM32WBxx Devices.

  The TIM2 CCR1 register value is equal to 10000:
  TIM2_CH1 delay = CCR1_Val/TIM2 counter clock  = 1s
  so the TIM2 Channel 1 generates a signal with a delay equal to 1s.

  The TIM2 CCR2 register value is equal to 5000:
  TIM2_CH2 delay = CCR2_Val/TIM2 counter clock = 500 ms
  so the TIM2 Channel 2 generates a signal with a delay equal to 500 ms.

  The TIM2 CCR3 register value is equal to 2500:
  TIM2_CH3 delay = CCR3_Val/TIM2 counter clock = 250 ms
  so the TIM2 Channel 3 generates a signal with a delay equal to 250 ms.

  The TIM2 CCR4 register value is equal to 1250:
  TIM2_CH4 delay = CCR4_Val/TIM2 counter clock = 125 ms
  so the TIM2 Channel 4 generates a signal with a delay equal to 125 ms.

  While the counter is lower than the Output compare registers values, which 
  determines the Output delay, the PC0, PC1, PC2 and PC3 pin are turned ON. 

  When the counter value reaches the Output compare registers values, the Output 
  Compare interrupts are generated and, in the handler routine, these pins are turned OFF.
  
@note Delay values mentioned above are theoretical (obtained when the system clock frequency 
      is exactly 64 MHz). Since the generated system clock frequency may vary from one board to another observed
      delay might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, Output, Compare, InActive, Signals, Delay, Frequency

@par Directory contents

  - TIM/TIM_OCInactive/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCInactive/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - TIM/TIM_OCInactive/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCInactive/Src/stm32wbxx_it.c          Interrupt handlers
  - TIM/TIM_OCInactive/Src/main.c                  Main program
  - TIM/TIM_OCInactive/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCInactive/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55 
    board and can be easily tailored to any other supported device 
    and development board.      

  - P-NUCLEO-WB55 Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - PC0: wave form of (TIM2_CH1) (pin 28 in CN7 connector)
      - PC1: wave form of (TIM2_CH2) (pin 30 in CN7 connector)
      - PC2: wave form of (TIM2_CH3) (pin 38 in CN7 connector)
      - PC3: wave form of (TIM2_CH4) (pin 36 in CN7 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

  PC0
               _____________________________________________________
 _____________|                                                     |________________
              <-----------------------1sec-------------------------->
  PC1
               ___________________________
 _____________|                           |__________________________________________
              <-----------500ms----------->
  PC2
               _________________
 _____________|                 |____________________________________________________
              <------250ms------>
 
  PC3
               ___________
 _____________|           |___________________________________________________________
              <---125ms--->


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
