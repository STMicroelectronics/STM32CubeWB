/**
  @page BLE_p2pClient_Ext Extended Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_p2pClient_Ext/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_p2pClient_Ext application
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

Demontrates a BLE scanner with connections from an extended and a legacy advertising

Two Nucleo STM32WB55xx boards (MB1355C) are used, one acting as GATT client, and one as GATT server.

@par Keywords

Connectivity, BLE, extended, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_p2pClient_Ext/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE/BLE_p2pClient_Ext/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE/BLE_p2pClient_Ext/Core/Inc/main.h                  Header for main.c module
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/app_ble.h         Header for app_ble.c module
  - BLE/BLE_p2pClient_Ext/Core/Inc/app_common.h            Header for all modules with common definition
  - BLE/BLE_p2pClient_Ext/Core/Inc/app_conf.h              Parameters configuration file of the application
  - BLE/BLE_p2pClient_Ext/Core/Inc/app_entry.h             Parameters configuration file of the application
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/ble_conf.h        BLE Services configuration
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/ble_dbg_conf.h    BLE Traces configuration of the BLE services
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/p2p_client_app.h  Header for p2p_lcient_app.c module
  - BLE/BLE_p2pClient_Ext/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE/BLE_p2pClient_Ext/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE/BLE_p2pClient_Ext/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE/BLE_p2pClient_Ext/Core/Src/main.c                  Main program
  - BLE/BLE_p2pClient_Ext/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/app_ble.c         BLE Profile implementation
  - BLE/BLE_p2pClient_Ext/Core/Src/app_entry.c             Initialization of the application
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/App/p2p_client_app.c  P2P Client Application Implementation
  - BLE/BLE_p2pClient_Ext/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - BLE/BLE_p2pClient_Ext/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE/BLE_p2pClient_Ext/Core/Src/hw_timerserver.c        Timer Server based on RTC
  - BLE/BLE_p2pClient_Ext/Core/Src/hw_uart.c               UART Driver

@par Hardware and Software environment

    - This application runs on STM32WB55xx Nucleo board (MB1355C)
    
    - Nucleo board Set-up    
       - Connect that Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_extended_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  
Pay attention to the linker file that is specific when using the extended wireless stack. 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load the image into Target memory
 - This must be done for BLE_p2pClient_Ext and BLE_p2pServer_Ext applications.

Open a terminal on Central and Peripheral side (ST Link Com Port, @115200 bauds).
At startup on BLE_p2pServer_Ext side, depending on the chosen configuration in app_conf.h, traces show the started advertising sets. 
The central device (BLE_p2pClient_Ext) scans and receives HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT from BLE_p2pServer_Ext device.
Result is printed over the UART and presented in a tab format with following infornations:
 - BD address
 - Type
 - Rssi
 - Property
 - Data length
 - SID
 - Some flags information
 
Pressing the buttons will perform the following actions:
 - SW1: stop the scan and connect to a server if any detected.
 - SW2: toggle a filter on extended result only.
 - SW3: stop/start scanning, blue LED reflects the status.

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
