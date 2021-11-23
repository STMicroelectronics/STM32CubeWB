/**
  @page Thread_Coap_Generic_Ota application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Coap_Generic_Ota/readme.txt 
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

How to build Thread application based on Coap messages (OTA mode).

This application requires two STM32WB55xx boards.

@note This application is not supported by CubeMx but has been copied from the project Thread_Coap_Generic generated
      by CubeMx with some modifications in order to be able to download it with the Thread_Ota application.
      The steps to be done to move from the Thread_Coap_Generic application to the Thread_Coap_Generic_Ota application are :
      - Copy the full folder Thread_Coap_Generic.
      - Replace the linker file stm32wb55xx_flash_cm4.icf by stm32wb55xx_flash_cm4_ota.icf ( this adds the placement
        of the two sections TAG_OTA_START and TAG_OTA_END ).
      - Remove the update of SCB->VTOR in the file system_stm32wbxx.c ( The VTOR is already set by the Thread_Ota application
        and shall not be changed to a different value).
      - SW3 is mapped. When pushed forces a reboot on Thread_Ota application (so that a new FW OTA update can be done).

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child)

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) 
The other one will be in Child mode (Red LED3 ON)
 
Let's name indifferently one board A and one board B. 
To send a COAP command from board A to board B, press the SW1 Push-Button on board A. 
The board B will receive the COAP command to light on its blue LED1. 
Pressing again same push-button will light off the blue LED1. And so-on. 
Same COAP commands can be sent from board B to board A. 

  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |                         |
  |    Push Button SW1 -->  |======> COAP =========>| BLUE LED TOGGLE (ON/OFF)|
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
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  | LED : Red               |                       | LED : Green             |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/app_common.h       	    Header for all modules with common definition
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/app_conf.h         	    Parameters configuration file of the application 
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/app_entry.h        	    Parameters configuration file of the application
  - Thread/Thread_Coap_Generic_Ota/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/hw_conf.h          	    Configuration file of the HW 
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Coap_Generic_Ota/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Coap_Generic_Ota/Core/Inc/utilities_conf.h   	    Configuration file of the utilities
  - Thread/Thread_Coap_Generic_Ota/Core/Src/app_entry.c        	    Initialization of the application
  - Thread/Thread_Coap_Generic_Ota/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Coap_Generic_Ota/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Coap_Generic_Ota/Core/Src/stm32_lpm_if.c     	    Low Power Manager Interface
  - Thread/Thread_Coap_Generic_Ota/Core/Src/hw_timerserver.c   	    Timer Server Driver
  - Thread/Thread_Coap_Generic_Ota/Core/Src/hw_uart.c          	    UART driver
  - Thread/Thread_Coap_Generic_Ota/Core/Src/main.c                  Main program
  - Thread/Thread_Coap_Generic_Ota/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Coap_Generic_Ota/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Coap_Generic_Ota/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
 
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
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 