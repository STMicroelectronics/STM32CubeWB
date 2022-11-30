/**
  @page Thread_Ota_Server application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Ota_Server/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Thread Ota Server Application 
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

How to update Over The Air (OTA) FW application and Copro Wireless binary using Thread (Server side).

This application requires two STM32WB55xx boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child)

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) 
The other one will be in Child mode (Red LED3 ON)
 
This Thread application provides implementation example for Updating FW on Application
side and on Copro Wireless side.
Thread_Ota_Server can communicate only with Thread_Ota application.
 
IMPORTANT PREREQUISITES:
On Thread_Ota_Server a binary image has to be loaded at @0x08010000 before starting the process.
It can be FW application example (compatible with Ota settings, see Thread_Coap_Generic_Ota for details)
or encrypted Copro Wireless binary.


Following diagram describes the Update procedure:
  ____________________________________                                 ______________________________________          
  |  Device 1                         |                                | Device 2                            |
  |  Thread_Ota_Server                |                                | Thread_Ota                          |
  |___________________________________|                                |_____________________________________|
  |                                   |                                |                                     |
  | Before starting FW for update     |                                | At startup, application performs    |
  |  must be written at following     |                                |   a delete of FLASH memory sectors  |
  |  @ = 0x08010000                   |                                |   starting from @ = 0x08010000      |
  |                                   |                                |   to SFSA (Option Byte) limit       |  
  |                                   |                                |                                     |  
  | Get Mesh-Local EID                |                                |                                     |
  |  (Endpoint Identifier) of         |                                |                                     |
  |  Thread_Ota device                |                                |                                     |
  |                                   |                                |                                     |
  | Push Button  ----->               |                                |                                     |                          
  |  - SW1 (OTA for APP Update)       |                                |                                     |
  |  - SW3 (OTA for Copro Update)     |                                |                                     |
  |                                   | ====== COAP REQUEST =========> |                                     |
  |                                   | Resource: "FUOTA_PROVISIONING" |                                     |
  |                                   | Mode: Multicast                |                                     |
  |                                   | Type: Confirmable              |     returns Mesh-Local EID          |
  |                                   | Code: Get                      |                                     |
  |                                   | Payload  : OtaContext          |                                     |                                     |
  |                                   |                                |                                     |
  |Thread_Ota Mesh-Local EID received | <=====COAP CONFIRMATION ====== |                                     |
  |                                   |                                |                                     |
  |                                   |                                |                                     |
  |                                   |                                |                                     |
  | Send FUOTA parameters:            | ============> COAP =========>  |                                     |
  |   - File Type (App or Corpro)     | Resource: "FUOTA_PARAMETERS"   |                                     |
  |   - Base address for the download | Mode: Unicast                  |    Saves FUOTA parameters           |
  |   - Magic Keyword                 | Type: Confirmable              |     and confirms                    |
  |                                   | Code: Put                      |                                     |
  |                                   |                                |                                     |
  |   Waits for confirmation          | <=====COAP CONFIRMATION ====== |                                     |
  |                                   |                                |                                     |
  | Once confirmation received        |                                |                                     |
  |   ->Starts FUOTA data transfer    |                                |                                     |
  |                                   |                                |                                     |
  |                                   |                                |                                     |                                     
  |--> Data Transfer                  | ============> COAP =========>  |    BLUE LED TOGGLING                |
  ||   (400 bytes Payload)            | Resource: "FUOTA_SEND"         |                                     |      
  ||                                  | Mode: Unicast                  |                                     |
  ||                                  | Type: Confirmable              |   Each time data buffer is received |
  ||                                  | Code: Put                      |    writes it to FLASH memory        |
  ||                                  | Payload  : Buffer[]            |                                     |
  ||                                  |                                |                                     |
  ||            Ack received          | <=====COAP CONFIRMATION ====== |                                     | 
  ||                |                 |                                |                                     |
  ||                V                 |                                |                                     |
  ||                /\                |                                |                                     |
  ||               /  \               |                                |                                     |
  ||              /    \              |                                |                                     |
  ||             /      \             |                                |                                     |
  ||            /        \            |                                |                                     |
  ||   NO      / END OF   \           |                                |                                     |
  | --------- /  TRANSFER? \          |                                |                                     |
  |           \  (MAGIC    /          |                                |                                     |
  |            \  KEYWORD /           |                                |                                     |
  |             \ FOUND) /            |                                |                                     |
  |              \      /             |                                |                                     |
  |               \    /              |                                |                                     |
  |                \  /               |                                |                                     |
  |                 \/                |                                |                                     |
  |             YES |                 |                                |                                     |
  |                 |                 |                                |                                     |
  |                 V                 |                                |                                     |
  |  Display FUOTA Transfer details:  |                                |    At the end of data transfer      |
  |    - Payload size used            |                                |     - BLUE LED OFF                  |
  |    - Transfer Time                |                                |     - Reboot the application        |
  |    - Average Throughput           |                                |                                     |
  |                                   |                                |                                     |
  |                                   |                                | On Reboot:                          |
  |                                   |                                | -if File_Type is FW_APP, boot on    |
  |                                   |                                |   Thread_Coap_Generic_Ota or any    | 
  |                                   |                                |   appilcation supporting Ota at     | 
  |                                   |                                |   @ 0x10000                         | 
  |                                   |                                |   (see Thread_Coap_Generic_Ota)     | 
  |                                   |                                |                                     | 
  |                                   |                                | -if File_Type is FW_COPRO_WIRELESS  | 
  |                                   |                                |   boot on FUS.                      | 
  |                                   |                                |   FUS will then install encrypted   | 
  |                                   |                                |   binary on Copro Wireless side.    | 
  |                                   |                                |                                     | 
  |                                   |                                |                                     | 
  |                                   |                                |                                     |  
  --------------------------------    -                                 --------------------------------------       
  | Role : Leader                     |                                | Role : Child                        |
  |                                   |                                |                                     |
  | LED : Green                       |                                | LED : Red                           |
  |                                   |                                |                                     |
  |___________________________________|                                |_____________________________________|
                                                                                                  
      
@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_Ota_Server/Core/Inc/app_common.h       	    Header for all modules with common definition
  - Thread/Thread_Ota_Server/Core/Inc/app_conf.h         	    Parameters configuration file of the application 
  - Thread/Thread_Ota_Server/Core/Inc/app_entry.h        	    Parameters configuration file of the application
  - Thread/Thread_Ota_Server/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Ota_Server/Core/Inc/hw_conf.h          	    Configuration file of the HW 
  - Thread/Thread_Ota_Server/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Ota_Server/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Ota_Server/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Ota_Server/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Ota_Server/Core/Inc/utilities_conf.h   	    Configuration file of the utilities
  - Thread/Thread_Ota_Server/Core/Src/app_entry.c        	    Initialization of the application
  - Thread/Thread_Ota_Server/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Ota_Server/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Ota_Server/Core/Src/stm32_lpm_if.c     	    Low Power Manager Interface
  - Thread/Thread_Ota_Server/Core/Src/hw_timerserver.c   	    Timer Server Driver
  - Thread/Thread_Ota_Server/Core/Src/hw_uart.c          	    UART driver
  - Thread/Thread_Ota_Server/Core/Src/main.c                  Main program
  - Thread/Thread_Ota_Server/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Ota_Server/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Ota_Server/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
 
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

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 

To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
 For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 