/**
  @page LPTIM_PulseCounter_Init LPTIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/LPTIM/LPTIM_PulseCounter_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM_PulseCounter_Init example.
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

How to use the LPTIM peripheral in counter mode to generate a PWM output signal 
and update its duty cycle. This example is based on the STM32WBxx
LPTIM LL API. The peripheral is initialized with LL initialization 
function to demonstrate LL init usage.

To reduce power consumption, MCU enters stop mode after starting counting. Each
time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woken up from stop mode and LED2 toggles the last state.

In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM1_IN1 pin, an interrupt is generated and LED2
toggles. If the external function generator is set to provide a square waveform at 1Khz,
the led will toggle each second.

In this example the internal clock provided to the LPTIM1 is LSI (32 kHz),
so the external input is sampled with LSI clock. In order not to miss any event,
the frequency of the changes on the external Input1 signal should never exceed the
frequency of the internal clock provided to the LPTIM1 (LSI for the
present example).

@note On STM32WB, both CPU1 and CPU2 must be in stop mode to set the entire system in stop mode.
      In this example, CPU2 is not started-up and CPU1 configures the CPU2 stop mode.
      If this example would be ported to another application, user must manage CPU2 entering in stop mode.


@par Keywords

Timer, Low Power, Pulse Counter, Stop mode, Interrupt

@par Directory contents 

  - LPTIM/LPTIM_PulseCounter_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_PulseCounter_Init/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_PulseCounter_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - LPTIM/LPTIM_PulseCounter_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PulseCounter_Init/Src/main.c                  Main program
  - LPTIM/LPTIM_PulseCounter_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.
    
  - This example has been tested with NUCLEO-WB15CC board and can be
    easily tailored to any other supported device and development board.
    
  - Connect a square waveform generator to PB5 (Arduino connector CN5 pin D8, Morpho connector CN10 pin 21).
    If the frequency of the signal is 1 kHz, LED2 toggles every second.

  - LED1 remains ON : LPTIM1_IN and LED1 share the same pin PB5.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
