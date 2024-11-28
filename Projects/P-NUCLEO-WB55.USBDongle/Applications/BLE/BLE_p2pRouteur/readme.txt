/**
  @page BLE_p2pRouteur Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_p2pRouteur/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE P2P Router application
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
This example is to demonstrate Multipoint communication using BLE component.

STM32WB55xx maintains seven simultaneously connections to transfer information from one device to an another remote.
Smart phone application is connected to one STM32WB55xx P2P router device allowing  the control of six others
STM32WB55xx P2P Server devices (End Devices).

Software Application Architecture demonstrate STM32WB55xx capabilities:
-- 1 slave & 6 masters connections on same device
-- GATT Server & GATT Client on same device
-- Automatic reconnection
-- Special Advertising format is proposed
-- Proprietary Services & Characteristics are used

With P2P Router Application:
-- STM32WB55xx is Peripheral (GAP) / Server Device (GATT)
-- Peripheral to be connected by the Smart Phone
-- STM32WB55xx is Central (GAP) / Client Device(GATT)
-- Central to connect to Server Application 1 to 6 (Peripherals) 

GAP Configuration
-- Router device is placed in Advertising mode at Startup


Once the code is downloaded on the STM32WB55xx board and executed, the modules are initialized. 
The Peripheral device (P2P Server) starts advertising to be connected by Smart Phone application.

Pushing SW1 button starts the SCAN / CONNECTION to END DEVICE
The Central device (P2P Client) scans and automatically connects to the P2P Server found (Server 1 to 6).

Once connected, the P2P Client (GATT Client) starts to search the P2P Services & Characteristics of the connected Server.
Led Button service, Led Characteristic and Button Characteristic are discovered at this stage (this service is specific from STM).
The above phase takes about 5 seconds.
After that period When pressing the User button (SW1) on Server (End Device) board, the service allows providing the information to the
P2P_router application thanks the STM Specific Service. If smart phone connected, this information is transmitted to it.

Smartphone application allows to control P2P Server 1 to 6 (End device) blue LED via the P2P Router application.
 
                        P2P_Server1
                            ^
                            |
                            v
Smartphone <---------> P2P_Router  ...  <-> P2P_Server6
                            ^
                            |
                            V
                        P2P_Server2




@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_p2pRouteur/Core/Inc/app_common.h                Header for all modules with common definition
  - BLE/BLE_p2pRouteur/Core/Inc/app_conf.h                  Application configuration file for STM32WPAN Middleware
  - BLE/BLE_p2pRouteur/Core/Inc/app_debug.h                 Header for app_debug.c module
  - BLE/BLE_p2pRouteur/Core/Inc/app_entry.h                 Interface to the application
  - BLE/BLE_p2pRouteur/Core/Inc/hw_conf.h                   Configuration file of the HW
  - BLE/BLE_p2pRouteur/Core/Inc/hw_if.h                     Hardware Interface
  - BLE/BLE_p2pRouteur/Core/Inc/main.h                      Header for main.c module
  - BLE/BLE_p2pRouteur/Core/Inc/stm32wbxx_hal_conf.h        HAL configuration file
  - BLE/BLE_p2pRouteur/Core/Inc/stm32wbxx_it.h              Interrupt handlers header file
  - BLE/BLE_p2pRouteur/Core/Inc/stm32_lpm_if.h              Header for stm32_lpm_if.c module (device specific LP management)
  - BLE/BLE_p2pRouteur/Core/Inc/utilities_conf.h            Configuration file of the utilities
  - BLE/BLE_p2pRouteur/Core/Inc/vcp_conf.h                  Configuration of the vcp interface
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/app_ble.h             Header for app_ble.c module
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/ble_conf.h            BLE Services configuration
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/ble_dbg_conf.h        BLE Traces configuration of the BLE services
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/p2p_routeur_app.c    Header for p2p routeur application module
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/tl_dbg_conf.h         Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_p2pRouteur/Core/Src/app_debug.c                 Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_p2pRouteur/Core/Src/app_entry.c                 Initialization of the application
  - BLE/BLE_p2pRouteur/Core/Src/hw_timerserver.c            Timer Server based on RTC
  - BLE/BLE_p2pRouteur/Core/Src/hw_uart.c                   UART Driver
  - BLE/BLE_p2pRouteur/Core/Src/main.c                      Main program
  - BLE/BLE_p2pRouteur/Core/Src/stm32wbxx_hal_msp.c         This file provides code for the MSP Initialization and de-Initialization
  - BLE/BLE_p2pRouteur/Core/Src/stm32wbxx_it.c              Interrupt handlers
  - BLE/BLE_p2pRouteur/Core/Src/stm32_lpm_if.c              Low Power Manager Interface
  - BLE/BLE_p2pRouteur/Core/Src/system_stm32wbxx.c          stm32wbxx system source file
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/app_ble.c             BLE Profile implementation
  - BLE/BLE_p2pRouteur/STM32_WPAN/App/p2p_routeur_app.c     P2P Router application
  - BLE/BLE_p2pRouteur/STM32_WPAN/Target/hw_ipcc.c          IPCC Driver
  
@par Hardware and Software environment

  - This application runs on STM32WB55xx devices, USB DONGLE board (MB1293C) & Nucleo board (MB1355C)
    
  - This application has been tested with an STMicroelectronics USB DONGLE board (MB1293C)
    and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following :
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file

On the other STM32WB boards, (MB1355C) for example:
 - flash the boards with the BLE_p2pServer executable file.
 - Server 1 to 6 are defined in app_conf.h file, setting the good #define P2P_SERVERx (1 <= x <= 6).

- Switch on ST BLE Router and the first ST BLE Server board.

 On an android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Toolbox application:
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox
 - click on the ST BLE Toolbox app
 - connect to a device
 - select the P2PROUT in the device list

 - 1 - Green led of BLE_p2pServer should blink (server advertises) until BLE_p2pRouteur is connected.
 - 2 - To connect BLE_p2pRouteur to BLE_p2pServer, just press SW1 of the BLE_p2pRouteur, the blue led is turned on.
 - 3 - As soon as the blue led turns off (about 5 seconds), the connection is done and services/characteristics found.
 - In case of more than one BLE_p2pServer, the best way is to switch on one BLE_p2pServer board at a time and repeat step 1 to 3 until the last BLE_p2pServer.  
 - When the connection is done, Router and all the Device Servers (1 to 6) are displayed on the phone.
 - The smartphone can send write commands to BLE_p2pServer via BLE_p2pRouteur and receive notifications from BLE_p2pServers.
 - For each server, the blue led can be switched on/off when turning on/off the led button of the wanted server on the smartphone display.
 - Each BLE_p2pServer can send a notification event to the smart phone via BLE_p2pRouteur when SW1 is pressed (a bell is displayed or removed on the corresponding server on the phone).  

@note Debug traces can be enabled/disabled in app_conf.h 

  To get the traces, you have to enable CFG_USB_INTERFACE_ENABLE, with CFG_DEBUG_BLE_TRACE for BLE services traces 
  or with CFG_DEBUG_APP_TRACE for application traces.

      On the PC side:
       - open a terminal window with the following settings:
         baud rate of 115200
         Byte size of 8
         Parity None
         Stop bits 1
         Data Flow Control None

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview

For more details refer to the application Note: 
  AN5289 - Building a Wireless application 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
