/**
  @page Thread_Coap_Generic application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Coap_Generic/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Coap Generic Application 
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to build Thread application based on Coap messages.

This application requires two STM32WB5MM-DK boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child)

After the reset of the 2 boards, one board will be in Leader mode.
The other one will be in Child or Router mode.

The role of the Thread device is displayed on the LCD of the STM32WB5MM-DK Board.
 
Let's name indifferently one board A and one board B. 
To send a COAP command from board A to board B, press the B1 Push-Button on board A. 
The board B will receive the COAP command 
Same COAP commands can be sent from board B to board A.
The reception of the COAP message is displayed on the LCD.


  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |                         |
  |        Push Button -->  |======> COAP =========>|  Display 'O' on the LCD |
  |                         | Resource "light"      |                         |
  |                         | Mode: Multicast       |                         |
  |                         | Type: Non-Confirmable |                         |
  |                         | Code: Put             |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  *************** WAIT 5 Seconds (configurable with WAIT_TIMEOUT) *************
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |=====> COAP ==========>|-------->                |
  |                         | Resource "light"      |         |               |
  |                         | Mode: Multicast       |  CoapRequestHandler()   |
  |                         | Type: Confirmable |             |               |
  |                         | Code: Put             |         |               |
  |                         |                       |         v               |
  |                         |                       |  CoapSendDataResponse() |
  |                         |                       |         |               |
  |                         |                       |         v               |
  | CoapDataRespHandler()<--|<===== COAP <==========| <-------                |
  |                         |                       |                         |
  |                         |                       |                         |
  ---------------------------                       ---------------------------
  | Role : Child or Router  |                       | Role : Leader           |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |_________________________|                       |_________________________|




      
@par Keywords

COAP,Thread

@par Hardware and Software environment

  - This example runs on STM32WB5MMxx devices (Nucleo board and dongle) 
  
  - This example has been tested with an STMicroelectronics STM32WB5MMxx_Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.
    

@par How to use it ? 

This application requests having the stm32wb5x_Thread_FTD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


In order to make the program work, you must do the following: 
 - Connect 2 STM32WB5MMxx_Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
 
 Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.
 
 If you want to control this application, you can directly send and
 retrieve Cli commands connecting an HyperTerminal with the ST_Link cable.
 (Refer to the Thread_Cli_Cmd application for more details) 
    
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
 
 For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 