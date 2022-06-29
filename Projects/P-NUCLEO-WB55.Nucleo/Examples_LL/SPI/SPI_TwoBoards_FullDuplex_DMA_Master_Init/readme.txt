/**
  @page SPI_TwoBoards_FullDuplex_DMA_Master_Init SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_DMA_Master_Init example.
  ******************************************************************************
  *
  * Copyright (c) 2019-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

Data buffer transmission and reception via SPI using DMA mode. This example is 
based on the STM32WBxx SPI LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size). This
example is for the Master board.

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PA5) |______________________|(PA5)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PA6)|______________________|(PA6)MISO         |      |
  |      |                  |                      |                  |      |
  |      |         MOSI(PA7)|______________________|(PA7)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32WBxx Master _______|                      |_STM32WBxx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32WBxx SPI LL API.

This example is split in two projects, Master board and Slave board:

- Master Board
  SPI1 Peripheral is configured in Master mode.
  DMA1_Channel3 and DMA1_Channel1 configured to transfer Data via SPI peripheral
  GPIO associated to User push-button (SW1) is linked with EXTI. 
  
- Slave Board
  SPI1 Peripheral is configured in Slave mode.
  DMA1_Channel3 and DMA1_Channel1 configured to transfer Data via SPI peripheral


Example execution:
On BOARD MASTER, LED2 is blinking Fast (200ms) and wait User push-button (SW1) action.
Press User push-button (SW1) on BOARD MASTER start a Full-Duplex communication through DMA.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED2 is On on both boards if data is well received.

In case of errors, LED2 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.

@par Keywords

Connectivity, SPI, Full-duplex, Transmission, Reception, Master, MISO, MOSI, DMA

@par Directory contents 

  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on P-NUCLEO-WB55 devices.

  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

  - P-NUCLEO-WB55 Set-up
    - Connect Master board PA5 to Slave Board PA5 (connected to pin 11 of CN10 connector)
    - Connect Master board PA6 to Slave Board PA6 (connected to pin 13 of CN10 connector)
    - Connect Master board PA7 to Slave Board PA7 (connected to pin 15 of CN10 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Load the Master project in Master Board
    o Load the Slave project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
