/**
  @page USART_Communication_Rx_IT USART Receiver example (IT Mode)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/USART/USART_Communication_Rx_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_Communication_Rx_IT example.
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

Configuration of GPIO and USART peripherals to receive characters 
from an HyperTerminal (PC) in Asynchronous mode using an interrupt. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button (SW1) is linked with EXTI.
USART RX Not Empty interrupt is enabled.
Virtual Com port feature of STLINK could be used for UART communication between board and PC.

Example execution:
When character is received on USART Rx line, a RXNE interrupt occurs.
USART IRQ Handler routine is then checking received character value.
On a specific value ('S' or 's'), LED2 is turned On.
Received character is echoed on Tx line.
On press on User push-button (SW1), LED2 is turned Off.
In case of errors, LED2 is blinking.

@par Keywords

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, Receiver

@par Directory contents 

  - USART/USART_Communication_Rx_IT/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Rx_IT/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Rx_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Rx_IT/Src/stm32wbxx_it.c          Interrupt handlers
  - USART/USART_Communication_Rx_IT/Src/main.c                  Main program
  - USART/USART_Communication_Rx_IT/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.
    
  - P-NUCLEO-WB55 Set-up
    In order to select use of Virtual Com port feature of STLINK for connection between P-NUCLEO-WB55 and PC,
    User has to set USE_VCP_CONNECTION define to 1 in main.h file.
    If so, please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

    If VCP is not used (USE_VCP_CONNECTION define set to 0 in main.h file), Connect USART1 TX/RX 
    to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter) :
    - Connect STM32 MCU board USART1 TX pin (GPIO PA.09 connected to pin 19 of CN10 connector)
      to PC COM port RX signal
    - Connect STM32 MCU board USART1 RX pin (GPIO PA.10 connected to pin 31 of CN10 connector)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal
    
  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control). 
    Port should be selected according to USE_VCP_CONNECTION value.

  - Launch the program. Enter characters on PC communication SW side.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
