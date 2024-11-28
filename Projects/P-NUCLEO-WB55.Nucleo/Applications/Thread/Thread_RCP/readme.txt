/**
  @page Thread_RCP application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_RCP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread RCP application
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

@par Application Description 

This application is used to demonstrate the OpenThread Border router feature using an STM32WB device.
To play with the demo, you need a Raspberry Pi 3B (RPi) to perform the link with Ethernet network.

Connect the Nucleo board to the Raspberry Pi (Host) using the ST-Link USB connector (CN15).
The default baud rate is set to 460800. If needed, change the value in MX_USART1_UART_Init().
If user wants to connect a FTDI cable instead of the ST-Link to exchange data with the Host, 
in app_conf.h, invert the values of CFG_DEBUG_TRACE_UART and CFG_CLI_UART. 
In that case, baud rate can also be changed in MX_LPUART1_UART_Init().

Refer to https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_OpenThread_Border_Router_Application 
for more details


@par Keywords

THREAD, RCP

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */