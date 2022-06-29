/**
  @page RCC_ClockConfig RCC Clock Config example

  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_ClockConfig/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

The main purpose of this example is to serve as a reference for clock configuration
operation needed by most of the BLE applications.

Thus it is just illustrating how to configure the system clock 
and also how to configure the source clock of some other peripherals used
by BLE applications:

  - RTC: clocked by LSE
  - LPUART1: clocked by PCLK1
  - USART1: clocked by PCLK2
  - RFWKP: clocked by LSE

HSE is selected as system clock source, i.e. the system clock is running at 32 MHz.

@note On the STM32WB5MM-DK board, MCO pins aren't output to the STMOD+ connectors
      thus generated system clock signal cannot be observed with an oscilloscope.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

RCC, System, Clock Configuration, HSE

@par Directory contents

  - RCC/RCC_ClockConfig/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig/Src/stm32wbxx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32wbxx.c      STM32WBxx system source file
  - RCC/RCC_ClockConfig/Src/stm32wbxx_hal_msp.c     HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32WB5Mxx devices.

  - This example has been tested with STM32WB5MM-DK
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
