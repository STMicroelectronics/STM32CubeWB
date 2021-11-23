/**
  @page QSPI_ReadWrite_DMA QSPI Read/Write in DMA mode example

  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_ReadWrite_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   QSPI Read/Write in DMA mode example.
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

How to erase part of the QSPI memory, write data in DMA mode,
read data in DMA mode and compare the result in a forever loop.

LED_GREEN toggles each time the data have been checked
LED_RED toggles as soon as an error is returned by HAL API
LED_RED is on as soon as a data is wrong

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

Memory, QUADSPI, Erase, Read, Write, DMA

@par Directory contents

  - QSPI/QSPI_ReadWrite_DMA/Inc/stm32wbxx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ReadWrite_DMA/Inc/stm32wbxx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ReadWrite_DMA/Inc/main.h               Header for main.c module
  - QSPI/QSPI_ReadWrite_DMA/Src/stm32wbxx_it.c       Interrupt handlers
  - QSPI/QSPI_ReadWrite_DMA/Src/main.c               Main program
  - QSPI/QSPI_ReadWrite_DMA/Src/system_stm32wbxx.c   STM32WBxx system source file
  - QSPI/QSPI_ReadWrite_DMA/Src/stm32wbxx_hal_msp.c  HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.

  - This example has been tested with NUCLEO-WB55RG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-WB55RG Set-up :
    Tested on memory Micron N25Q064A with following connections
    - Connect PA2 to S# (Chip Select)
    - Connect PA3 to C (Clock)
    - Connect PB9 to DQ0 (Serial Data)
    - Connect PB8 to DQ1 (Serial Data)
    - Connect PA7 to DQ2 (DQ2)
    - Connect PA6 to DQ3 (DQ3)
    - Connect NRST to RESET# (Reset)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
