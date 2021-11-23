/**
  @page Ble_Thread_Static example
  
  @verbatim
  ******************************************************************************
  * @file    BLE_Thread/Ble_Thread_Static/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using BLE and Thread protocols in Static 
  *          Concurrent mode.
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

How to use BLE application and Thread application in static concurrent mode.

This application requires one STM32WB55xx-Nucleo board with Static Concurrent Mode Application.
Static means that application can perform alternatively BLE or Thread activities. 

The idea is to present a demo with a Thread Network (at least 2 Nucleo Boards
running THREAD_FTD_Coap_Multicast application) and a BLE connection between 1 Nucleo
board running Ble_Thread_Static application with a smartphone running 
"ST BLE Sensor" Application (available on App Store and Google Play)
The STM32WB55xx-Nucleo board running Ble_Thread_Static application is capable 
of switching from BLE network to Thread network and vice-versa.

@par Keywords

COAP,THREAD,BLE

@par Directory contents 

  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/app_ble.h            Header for app_ble.c module
  - BLE_Thread/Ble_Thread_Static/Core/Core/Inc/app_common.h          Header for all modules with common definition
  - BLE_Thread/Ble_Thread_Static/Core/Core/Inc/app_conf.h            Parameters configuration file of the application
  - BLE_Thread/Ble_Thread_Static/Core/Core/Inc/app_entry.h           Parameters configuration file of the application
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/app_thread.h         Header for app_thread.c module
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/ble_conf.h           BLE Services configuration
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/ble_dbg_conf.h       BLE Traces configuration of the BLE services
  - BLE_Thread/Ble_Thread_Static/Core/Core/Inc/hw_conf.h             Configuration file of the HW
  - BLE_Thread/Ble_Thread_Static/Core/Inc/main.h                     Header for main.c module
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/p2p_server_app.h     Header for BLE P2P Server application
  - BLE_Thread/Ble_Thread_Static/Core/Inc/stm32wbxx_hal_conf.h       HAL configuration file
  - BLE_Thread/Ble_Thread_Static/Core/Inc/stm32wbxx_it.h             Interrupt handlers header file
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/app_ble.c            BLE Profile implementation
  - BLE_Thread/Ble_Thread_Static/Core/Core/Src/app_entry.c           Initialization of the application
  - BLE_Thread/Ble_Thread_Static/Core/Src/app_thread.c               Thread application implementation
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/Target/hw_ipcc.c         IPCC Driver
  - BLE_Thread/Ble_Thread_Static/Core/Core/Src/stm32_lpm_if.c        Low Power Manager Interface
  - BLE_Thread/Ble_Thread_Static/Core/Core/Src/hw_timerserver.c      Timer Server based on RTC
  - BLE_Thread/Ble_Thread_Static/Core/Core/Src/hw_uart.c             UART Driver
  - BLE_Thread/Ble_Thread_Static/Core/Src/main.c                     Main program
  - BLE_Thread/Ble_Thread_Static/STM32_WPAN/App/p2p_server_app.c     BLE P2P Server application implementation
  - BLE_Thread/Ble_Thread_Static/Core/Src/stm32wbxx_it.c             Interrupt handlers
  - BLE_Thread/Ble_Thread_Static/Core/Src/system_stm32wbxx.c         stm32wbxx system source file
  
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55RG-Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.
    
  - On STM32WB55RG-Nucleo, the jumpers must be configured as described
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

This application requests having the STM32WB5x_BLE_Thread_static_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


Minimum requirements for the demo:
- 1 STM32WB55xx-Nucleo board in BLE Thread Static Concurrent mode 
  (This device will be referred as NODE1)
- 2 STM32WB55xx-Nucleo board or more for creating Thread Network 
  (see example THREAD_FTD_Coap_Multicast for more details on how to use it)
- 1 Smartphone (Android/IOS) with "ST BLE Sensor" Phone Application (available on App Store and Google Play)
  Install the "ST BLE Sensor" application on the ios/android device
  https://play.google.com/store/apps/details?id=com.st.bluems
  https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

In order to make the program work, you must do the following: 
 - Connect 1 STM32WB55xx-Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 If you want to control this application, you can directly send and
 retrieve Cli commands connecting an HyperTerminal with the ST_Link cable.
 (Refer to the THREAD_Cli_Cmd application for more details) 
    
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
 
 For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

**** START DEMO ****
By default, after a reset, the board will start in BLE mode.

The BLE example is to demonstrate Point-to-Point communication using BLE component. 
The Peripheral device (P2P Server) starts advertising,
ST BLE Sensor smart Phone Application) scans and can connect to the P2P Server 
by selecting = "P2P_SERVER1" entry.
Once connected, the application starts to search the P2P Services & Characteristics.
Led Button service, Led Characteristic and Button Characteristic are discovered at this stage 
(this service is ST specific).
Once the smartphone is connected, the user can interact with the board by pressing 
"LED1 CONTROL" field which toggles LED1 on the board.

**** SWITCHING TO THREAD *****
This can be done in different ways:
 - By sending (WRITE) "0x0102" command to the board via BLE. (Switch to Thread button in the smartphone application)
 - By pressing SW2 Button on the STM32WB55xx-Nucleo board

**** THREAD MODE ****
Then the boards switch to thread protocol and corresponding application.
The node will then attach the existing Thread network (at least 2 boards running 
THREAD_FTD_Coap_Multicast application).

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses at least three devices, one device will act as a Leader (Router) 
and the other will act as child or router. 

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) 
The other one will be in Child/Router mode (Red LED3 ON).

SENDING COAP MESSAGE: 
On any board of the Thread network, it is possible to send COAP multicast message by pressing SW1 button.
The reception of the COAP message will then toggle BLUE LED1 on the board. 

**** SWITCHING TO BLE *****
This is done by pressing SW2 Button on the STM32WB55xx-Nucleo board.

The STM32WB55xx-Nucleo board will restart in BLE mode, and start advertising. 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 