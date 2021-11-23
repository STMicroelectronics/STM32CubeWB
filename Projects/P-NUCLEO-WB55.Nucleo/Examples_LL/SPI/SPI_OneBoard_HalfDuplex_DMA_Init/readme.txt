/**
  @page SPI_OneBoard_HalfDuplex_DMA_Init SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_HalfDuplex_DMA_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_OneBoard_HalfDuplex_DMA_Init example.
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

Configuration of GPIO and SPI peripherals to transmit 
bytes from an SPI Master device to an SPI Slave device in DMA mode. This example
is based on the STM32WBxx SPI LL API. The peripheral initialization uses the 
LL initialization function to demonstrate LL init usage.

This example works with only one P-NUCLEO-WB55.

SPI1 Peripheral is configured in Master mode Half-Duplex Tx.
SPI2 Peripheral is configured in Slave mode Half-Duplex Rx.
GPIO associated to User push-button is linked with EXTI. 

Example execution:

LED2 is blinking Fast (200ms) and wait User push-button (SW1) action.
Press User push-button (SW1) on BOARD start a Half-Duplex communication through DMA.
On MASTER side (SPI1), Clock will be generated on SCK line, Transmission done on MOSI Line.
On SLAVE side (SPI2) reception is done through the MISO Line.

LED2 is On if data is well received.

In case of errors, LED2 is blinking Slowly (1s).

@par Keywords

Connectivity, SPI, Half-duplex, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

@par Directory contents 

  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/main.c                  Main program
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on P-NUCLEO-WB55 devices.

  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

  - P-NUCLEO-WB55 Set-up
    - Connect Master SCK  PA5 to Slave SCK  PB13
    - Connect Master MOSI PA7 to Slave MISO PB14

Relation with Board connector:
  PA5  is connected to pin 11 of CN10 connector
  PA7  is connected to pin 15 of CN10 connector
  PB13 is connected to pin 30 of CN10 connector
  PB14 is connected to pin 28 of CN10 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
