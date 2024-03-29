/**
  @page BLE_p2pServerThreadX Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_p2pServerThreadX/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_p2pServerThreadX application
  ******************************************************************************
  *
  * Copyright (c) 2019-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to demonstrate Point-to-Point communication using BLE component (as GATT server). 

Two P-NUCLEO-WB55 boards are used, one acting as GATT client, and one as GATT server.
For example, BLE_P2PClient application is downloaded in a USB DONGLE board (MB1293C) and BLE_p2pServerThreadX application in a Nucleo board (MB1355C).
The client could be located in a phone also, using the ST BLE Sensor application instead of the MB1293C board. 

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core, ThreadX

@par Directory contents 
  
  - BLE/BLE_p2pServerThreadX/Core/Inc/stm32wbxx_hal_conf.h         HAL configuration file
  - BLE/BLE_p2pServerThreadX/Core/Inc/tx_user.h                    ThreadX configuration file
  - BLE/BLE_p2pServerThreadX/Core/Inc/app_debug.h                  Header for app_debug.c module
  - BLE/BLE_p2pServerThreadX/Core/Inc/stm32wbxx_it.h               Interrupt handlers header file
  - BLE/BLE_p2pServerThreadX/Core/Inc/main.h                       Header for main.c module
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/app_ble.h              Header for app_ble.c module
  - BLE/BLE_p2pServerThreadX/Core/Inc/app_common.h                 Header for all modules with common definition
  - BLE/BLE_p2pServerThreadX/Core/Inc/app_conf.h                   Parameters configuration file of the application
  - BLE/BLE_p2pServerThreadX/Core/Inc/app_entry.h                  Parameters configuration file of the application
  - BLE/BLE_p2pServerThreadX/Core/Inc/hw_if.h                      Hardware Interface header file
  - BLE/BLE_p2pServerThreadX/Core/Inc/stm32_lpm_if.h               Header for stm32_lpm_if.c module  
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/ble_conf.h             BLE Services configuration
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/ble_dbg_conf.h         BLE Traces configuration of the BLE services
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/p2p_server_app.h       Header for p2p_server_app.c module
  - BLE/BLE_p2pServerThreadX/Core/Inc/hw_conf.h                    Configuration file of the HW
  - BLE/BLE_p2pServerThreadX/Core/Inc/utilities_conf.h             Configuration file of the utilities
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/tl_dbg_conf.h          Debug configuration file for transport layer interface
  - BLE/BLE_p2pServerThreadX/Core/Src/app_debug.c                  Debug utilities
  - BLE/BLE_p2pServerThreadX/Core/Src/stm32wbxx_hal_msp.c          MSP Initialization Module
  - BLE/BLE_p2pServerThreadX/Core/Src/stm32wbxx_hal_timebase_tim.c Timebase Module
  - BLE/BLE_p2pServerThreadX/Core/Src/stm32wbxx_it.c               Interrupt handlers
  - BLE/BLE_p2pServerThreadX/Core/Src/main.c                       Main program
  - BLE/BLE_p2pServerThreadX/Core/Src/system_stm32wbxx.c           stm32wbxx system source file
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/app_ble.c              BLE Profile implementation
  - BLE/BLE_p2pServerThreadX/Core/Src/app_entry.c                  Initialization of the application
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/App/p2p_server_app.c       P2P Server application
  - BLE/BLE_p2pServerThreadX/STM32_WPAN/Target/hw_ipcc.c           IPCC Driver
  - BLE/BLE_p2pServerThreadX/Core/Src/stm32_lpm_if.c               Low Power Manager Interface
  - BLE/BLE_p2pServerThreadX/Core/Src/hw_timerserver.c             Timer Server based on RTC
  - BLE/BLE_p2pServerThreadX/Core/Src/hw_uart.c                    UART Driver
  - BLE/BLE_p2pServerThreadX/Core/Src/tx_initialize_low_level.S    ThreadX Low Level Initialization

@par Hardware and Software environment

    - This application runs on STM32WB55xx devices, USB DONGLE board (MB1293C) & P-NUCLEO-WB55 (MB1355C)

    - USB DONGLE board (MB1293C) Set-up    
       - Connect the USB DONGLE boards to your PC 
       - Connect external ST-LINK to dedicated ST LINK Pin
    
    - P-NUCLEO-WB55 (MB1355C) Set-up    
       - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.
    
    - Low power is used in the context of ThreadX OS, so be sure to use:
      CFG_LPM_SUPPORTED = 0 with TX_LOW_POWER = 0 to disable low power
      or
      CFG_LPM_SUPPORTED = 1 with TX_LOW_POWER = 1 to enable low power
      TX_LOW_POWER flag is managed at IDE project level (both compiler and assembler)

    - If TX_ENABLE_EVENT_TRACE is defined TraceX traces are enabled.
      These traces can be extracted using the debugger an launching the specific
      macro $PROJ_DIR$\save_tracex.mac (only available for IAR). This macro is responsible to
      copy the content of "TraceXBuf" array (used to store the traces) into an external file
      placed in "$PROJ_DIR$\TraceX_traces.trx". 
      This file can be opened through Azure RTOS TraceX Microsoft desktop application.
      Details to use this dapplication can be found here:
      https://learn.microsoft.com/en-us/azure/rtos/tracex/       


@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file
 - OR use the BLE_p2pServerThreadX_reference.hex from Binary directory

 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor and/or ST BLE Toolbox applications:
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Sensor
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox

First demonstration

 - Power on the Nucleo board with the BLE_p2pServerThreadX application
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to the device
 - select the P2PSRVX1 in the device list and play with the Light and the SW1 button of the board

Second demonstration
 - BLE_p2pServerThreadX may be connected by BLE_p2pClient.
 - Once the code (BLE_p2pServerThreadX & BLE_p2pClient) is downloaded into the two STM32WB55xx boards and executed, the modules are initialized. 

 - The Peripheral device (BLE_p2pServerThreadX) starts advertising (during 1 minute), the green led blinks for each advertising event.
 - The Central device (BLE_p2pClient) starts scanning when pressing the User button (SW1) on the USB Dongle board. 
 - BLE_p2pClient blue led becomes on. 
 - Scan req takes about 5 seconds.
 - Make sure BLE_p2pServerThreadX advertises, if not press reset button or switch off/on to restart advertising.
 - Then, it automatically connects to the BLE_p2pServerThreadX. 
 - Blue led turns off and green led starts blinking as on the MB1355C. Connection is done.
 - When pressing SW1 on a board, the blue led toggles on the other one.
 - The SW1 button can be pressed independently on the GATT Client or on the GATT Server.
 - When the server is located on a MB1355C, the connection interval can be modified from 50ms to 1s and vice-versa using SW2. 
 - The green led on the 2 boards blinks for each advertising event, it means quickly when 50ms and slowly when 1s. 
 - Passing from 50ms to 1s is instantaneous, but from 1s to 50ms takes around 10 seconds.
 - The SW1 event, switch on/off blue led, depends on the connection Interval event. 
 - So the delay from SW1 action and blue led change is more or less fast.

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview


For more details refer to the Application Note: 
  AN5289 - Building a Wireless application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
