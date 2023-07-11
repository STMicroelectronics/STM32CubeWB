/**
  @page Thread_Coap_Generic_ThreadX application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Coap_Generic_ThreadX/readme.txt 
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

How to build Thread application based on Coap messages. (using ThreadX)

This application requires two STM32WB55xx boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child)

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) 
The other one will be in Child mode (Red LED3 ON)
 
Let's name indifferently one board A and one board B. 

- press the SW1 Push-Button on board A. 
To send a COAP command (Non-Confirmable) from board A to board B:
The board B will receive COAP commands to toggle its blue LED1
- press the SW2 Push-Button on boad A.
To send a COAP command (Confirmable) from board A to board B:
The board B will receive COAP commands and send to board A a Coap Data response and toggle its blue LED1

Same COAP commands can be sent from board B to board A. 

  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |                         |
  |                SW1 -->  |======> COAP =========>| BLUE LED TOGGLE (ON/OFF)|
  |                         | Resource "light"      |                         |
  |                         | Mode: Multicast       |                         |
  |                         | Type: Non-Confirmable |                         |
  |                         | Code: Put             |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |                SW2 -->  |=====> COAP ==========>|-------->                |
  |                         | Resource "light"      |         |               |
  |                         | Mode: Multicast       |  CoapRequestHandler()   |
  |                         | Type: Confirmable |             |               |
  |                         | Code: Put             |         |               |
  |                         |                       |         v               |
  |                         |                       |  CoapSendDataResponse() |
  |                         |                       |         |               |
  |                         |                       |         v               |
  | CoapDataRespHandler()<--|<===== COAP <==========| <-------                |
  |                         |                       | BLUE LED TOGGLE (ON/OFF)| 
  |                         |                       |                         |  
  ---------------------------                       ---------------------------
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  | LED : Red               |                       | LED : Green             |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/app_common.h       	Header for all modules with common definition
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/app_conf.h         	Parameters configuration file of the application 
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/app_entry.h        	Parameters configuration file of the application
  - Thread/Thread_Coap_Generic_ThreadX/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/hw_conf.h          	Configuration file of the HW 
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Coap_Generic_ThreadX/Core/Inc/utilities_conf.h   	Configuration file of the utilities
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/app_entry.c        	Initialization of the application
  - Thread/Thread_Coap_Generic_ThreadX/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Coap_Generic_ThreadX/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/stm32_lpm_if.c     	Low Power Manager Interface
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/hw_timerserver.c   	Timer Server Driver
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/hw_uart.c          	UART driver
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/main.c                  Main program
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Coap_Generic_ThreadX/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 _ThreadX
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and dongle) 
  
  - This example has been tested with an STMicroelectronics STM32WB55xx_Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.
    
  - On STM32WB55RG_Nucleo, the jumpers must be configured as described
    in this section. Starting from the top left position up to the bottom 
    right position, the jumpers on the Board must be set as follows:

     CN11:    GND         [OFF]
     JP4:     VDDRF       [ON]
     JP6:     VC0         [ON]
     JP2:     +3V3        [ON] 
     JP1:     USB_STL     [ON]   All others [OFF]
     CN12:    GND         [OFF]
     CN7:     <All>       [OFF]
     JP3:     VDD_MCU     [ON]
     JP5:     GND         [OFF]  All others [ON]
     CN10:    <All>       [OFF]


@par How to use it ? 

This application requests having the stm32wb5x_Thread_FTD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


In order to make the program work, you must do the following: 
 - Connect 2 STM32WB55xx_Nucleo boards to your PC 
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


=> Trace for TheadX :
	- On the IAR project :
		- A new directory (debug_traces) is added to integrate all 'tx_trace*.c' files.
		-	A new 'macro' file named 'TraceForThreadX.mac' is added on EWARM directory.
		-	This new 'macro' file is defined on the 'option .. debugger .. Setup .. Setup macros'.
	- On the 'tx_user.h' file, the #define 'TX_ENABLE_EVENT_TRACE' is uncommented.
	- On the 'app_thread.c' 
		-	Definition of a 'Trace' Buffer (named 'cDebugTraceX') that can contains 512 event described by 64 bytes.
		-	Initialisation of trace by 'tx_trace_enable()' function.
		-	ThreadX trace all events concerning Thread/Semaphore/Mutex. User can add a 'manual' event with the function 'tx_trace_user_event_insert()'
	-To obtains the Trace :
		-	Pause the code.
		-	On the windows 'Quick Watch', write 'save_tracex()' on command line. 
			The macro save the trace on file 'Trace_xx.trx' on the directory 'EWARM'.
		-	Open the software 'Azure RTOS TraceX' that you can obtains on 'https://apps.microsoft.com/store/detail/azure-rtos-tracex/9NF1LFD5XXG3?hl=fr-fr&gl=FR' 
			or on the Windows Store.
		-	Load the '*.trx' file.
 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 