/**
  @page Thread_Coap_Generic application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Coap_Generic/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Coap Generic Application 
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

How to build Thread application based on Coap messages.

This application requires two STM32WB55xx boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child)

After the reset of the 2 boards, one board will be in Leader mode (displays it LCD) 
The other one will be in Child mode (displays it LCD).
 
Let's name indifferently one board A and one board B. 

To send a COAP command from board A to board B, press the SW1 Push-Button on board A. 
The board B will receive two COAP commands to toggle on LCD 'O' letter (line 4), second command is 
happening 5 seconds (WAIT_TIMEOUT) after the first command. 
Pressing again same push-button will repeat the toggling (taking 5 seconds) of the LCD 'O'. 
Pressing every second will reset the timer, hence, the second COAP command will not happen (need 
to have 5 seconds without pressing the push button).

- press the SW1 Push-Button on board A. 
To send a COAP command (Non-Confirmable) from board A to board B:
The board B will receive COAP commands to toggle LCD 'O' letter
- press the SW2 Push-Button on boad A.
To send a COAP command (Confirmable) from board A to board B:
The board B will receive COAP commands and send to board A a Coap Data response and toggle LCD 'O' letter

Same COAP commands can be sent from board B to board A. 

  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |                         |
  |        	SW1 -->     |======> COAP =========>| LCD TOGGLE 'O'  (ON/OFF)|
  |                         | Resource "light"      |                         |
  |                         | Mode: Multicast       |                         |
  |                         | Type: Non-Confirmable |                         |
  |                         | Code: Put             |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |             SW2 -->     |=====> COAP ==========>|-------->                |
  |                         | Resource "light"      |         |               |
  |                         | Mode: Multicast       |  CoapRequestHandler()   |
  |                         | Type: Confirmable |             |               |
  |                         | Code: Put             |         |               |
  |                         |                       |         v               |
  |                         |                       |  CoapSendDataResponse() |
  |                         |                       |         |               |
  |                         |                       |         v               |
  | CoapDataRespHandler()<--|<===== COAP <==========| <-------                |
  |                         |                       | LCD TOGGLE 'O'  (ON/OFF)|
  |                         |                       |                         |  
  ---------------------------                       ---------------------------
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_Coap_Generic/Core/Inc/app_common.h       	Header for all modules with common definition
  - Thread/Thread_Coap_Generic/Core/Inc/app_conf.h         	Parameters configuration file of the application 
  - Thread/Thread_Coap_Generic/Core/Inc/app_entry.h        	Parameters configuration file of the application
  - Thread/Thread_Coap_Generic/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Coap_Generic/Core/Inc/hw_conf.h          	Configuration file of the HW 
  - Thread/Thread_Coap_Generic/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Coap_Generic/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Coap_Generic/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Coap_Generic/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Coap_Generic/Core/Inc/utilities_conf.h   	Configuration file of the utilities
  - Thread/Thread_Coap_Generic/Core/Src/app_entry.c        	Initialization of the application
  - Thread/Thread_Coap_Generic/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Coap_Generic/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Coap_Generic/Core/Src/stm32_lpm_if.c     	Low Power Manager Interface
  - Thread/Thread_Coap_Generic/Core/Src/hw_timerserver.c   	Timer Server Driver
  - Thread/Thread_Coap_Generic/Core/Src/hw_uart.c          	UART driver
  - Thread/Thread_Coap_Generic/Core/Src/main.c                  Main program
  - Thread/Thread_Coap_Generic/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Coap_Generic/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Coap_Generic/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (DK, Nucleo board and dongle) 
  
  - This example has been tested with an STMicroelectronics STM32WB5MM-DK 
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
 - Connect 2 STM32WB5MM-DK boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
 
 Note: when an error has occurred on application it is indicating on LCD Line 4.
 
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
 