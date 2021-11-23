/**
  @page SPI_HalfDuplex_ComPollingIT_Slave SPI Half Duplex IT example
  
  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_HalfDuplex_ComPollingIT_Slave/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Half Duplex IT example.
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

Data buffer transmission/reception between 
two boards via SPI using Polling (LL driver) and Interrupt modes (HAL driver).

   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PA5) |______________________|(PA5)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PA6)|          ____________|(PA6)MISO         |      |
  |      |                  |          |           |                  |      |
  |      |         MOSI(PA7)|__________|           |(PA7)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32WBxx Master________|                      |_STM32WBxx Slave_________|


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.
The Half-Duplex SPI transmission (8bit) is done using LL Driver on Master board (Tx) by using function 
LL_SPI_TransmitData8.
The The Half-Duplex SPI reception (8bit) is done using HAL Driver on Slave board (Rx) by using function 
HAL_SPI_Receive_IT.

Example execution:
First step, press the User push-button (SW1), this action initiates a Half-Duplex transfer 
between Master and Slave.
After end of transfer, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles quickly on master board waiting User push-button (SW1) to be pressed.
 - LED2 turns ON on slave board if reception is complete and OK.
 - LED3 turns ON when there is an error in reception process. 

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, SPI, Half-duplex, Transmission, Reception, Slave, MISO, MOSI, Polling, Interrupt

@par Directory contents 

  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Inc/stm32wbxx_it.h          SPI interrupt handlers header file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Inc/main.h                  Header for main.c module  
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Src/stm32wbxx_it.c          SPI interrupt handlers
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Src/main.c                  Main program
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Src/system_stm32wbxx.c      STM32WBxx system source file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT_Slave/Src/stm32wbxx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.

  - Take care to cable connection between Master and Slave Board:
    Cable shall be smaller than 5 cm and rigid if possible.

  - This example has been tested with NUCLEO-WB15CC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-WB15CC Set-up
    - Connect Master board PA5 (CN10, pin 11) to Slave Board PA5 (CN10, pin 11)
    - Connect Master board PA7 (CN10, pin 15) to Slave Board PA6 (CN7, pin 30)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files (master project) and load your image into target memory
    o Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
    o Load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 