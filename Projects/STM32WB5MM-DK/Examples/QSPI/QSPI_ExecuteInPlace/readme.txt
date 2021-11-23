/**
  @page QSPI_ExecuteInPlace QSPI execute in place example
  
  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_ExecuteInPlace/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the code execution from QSPI memory example.
  ******************************************************************************
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example describes how to execute a part of the code from the QSPI memory. To do this, 
a section is created where the function is stored.

At the startup, the QSPI memory is erased, then the data are copied from the initialization
section of the flash to the QSPI memory. And finally the QSPI is configured in memory-mapped 
mode and the code in executed in a forever loop.

PWM_LED_RED, PWM_LED_GREEN and PWM_LED_BLUE toggle in a forever loop.
PWM_LED_RED is on as soon as an error is returned by HAL API, and no more LED toggles.

In this example, HCLK is configured at 64 MHz.
QSPI prescaler is set to 2, so QSPI frequency is = HCLK/(QSPI_Prescaler+1) = 64 MHz/(2+1)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, QUADSPI, Erase, Write, Execute in Place, Memory Mapped

@par Directory contents

  - QSPI/QSPI_ExecuteInPlace/Inc/stm32wb5mm_dk_conf.h BSP configuration file
  - QSPI/QSPI_ExecuteInPlace/Inc/stm32wbxx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ExecuteInPlace/Inc/stm32wbxx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ExecuteInPlace/Inc/main.h               Header for main.c module
  - QSPI/QSPI_ExecuteInPlace/Src/stm32wbxx_it.c       Interrupt handlers
  - QSPI/QSPI_ExecuteInPlace/Src/main.c               Main program
  - QSPI/QSPI_ExecuteInPlace/Src/system_stm32wbxx.c   STM32WBXX system source file
  - QSPI/QSPI_ExecuteInPlace/Src/stm32wbxx_hal_msp.c  HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32WB5MM devices.

  - This example has been tested with STM32WB5MM-DK board and can be
    easily tailored to any other supported device and development board.

  - A specific region for the QSPI memory has been added in the scatter file 
   (stm32wb5mxx_flash_cm4.icf for IAR toolchain, stm32wb5mxx_flash_cm4.sct for Keil toolchain)
   in order to map functions in this memory

  - The debugger does not work when code is executed from QSPI memory.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
