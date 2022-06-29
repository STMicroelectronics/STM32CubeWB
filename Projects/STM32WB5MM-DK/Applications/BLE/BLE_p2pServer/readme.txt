/**
  @page BLE_p2pServer Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_p2pServer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BLE_p2pServer application
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

How to demonstrate Point-to-Point communication using BLE (as GATT server).

A STM32WB5MMxx boards(MB1292B) is used, acting as GATT server.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents

  - BLE/BLE_p2pServer/Core/Inc/app_common.h                Header for all modules with common definition
  - BLE/BLE_p2pServer/Core/Inc/app_conf.h                  Parameters configuration file of the application
  - BLE/BLE_p2pServer/Core/Inc/app_debug.h                 Header for app_debug.c module
  - BLE/BLE_p2pServer/Core/Inc/app_entry.h                 Parameters configuration file of the application
  - BLE/BLE_p2pServer/Core/Inc/hw_conf.h                   Configuration file of the HW
  - BLE/BLE_p2pServer/Core/Inc/hw_if.h                     HW interface
  - BLE/BLE_p2pServer/Core/Inc/main.h                      Header for main.c module
  - BLE/BLE_p2pServer/Core/Inc/stm32_lpm_if.h              Header for stm32_lpm_if.c module (LP management)
  - BLE/BLE_p2pServer/Core/Inc/stm32wb5mm_dk_conf.h        Configuration file of the Discovery board
  - BLE/BLE_p2pServer/Core/Inc/stm32wbxx_hal_conf.h        HAL configuration file
  - BLE/BLE_p2pServer/Core/Inc/stm32wbxx_it.h              Interrupt handlers header file
  - BLE/BLE_p2pServer/Core/Inc/utilities_conf.h            Configuration file of the utilities
  - BLE/BLE_p2pServer/Core/Src/app_debug.c                 Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_p2pServer/Core/Src/app_entry.c                 Initialization of the application
  - BLE/BLE_p2pServer/Core/Src/hw_timerserver.c            Timer Server based on RTC
  - BLE/BLE_p2pServer/Core/Src/hw_uart.c                   UART Driver
  - BLE/BLE_p2pServer/Core/Src/main.c                      Main program
  - BLE/BLE_p2pServer/Core/Src/stm32_lpm_if.c              Low Power Manager Interface
  - BLE/BLE_p2pServer/Core/Src/stm32wbxx_hal_msp.c         MSP Initialization and de-Initialization code
  - BLE/BLE_p2pServer/Core/Src/stm32wbxx_it.c              Interrupt handlers
  - BLE/BLE_p2pServer/Core/Src/system_stm32wbxx.c          stm32wbxx system source file
  - BLE/BLE_p2pServer/STM32_WPAN/Target/hw_ipcc.c          IPCC Driver
  - BLE/BLE_p2pServer/STM32_WPAN/App/ble_conf.h            BLE Services configuration
  - BLE/BLE_p2pServer/STM32_WPAN/App/ble_dbg_conf.h        BLE Traces configuration of the BLE services
  - BLE/BLE_p2pServer/STM32_WPAN/App/tl_dbg_conf.h         Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_p2pServer/STM32_WPAN/App/app_ble.c             BLE Profile implementation
  - BLE/BLE_p2pServer/STM32_WPAN/App/app_ble.h             Header for app_ble.c module
  - BLE/BLE_p2pServer/STM32_WPAN/App/p2p_server_app.c      P2P Server application
  - BLE/BLE_p2pServer/STM32_WPAN/App/p2p_server_app.h      Header for p2p_server_app.c module
  - BLE/BLE_p2pServer/STM32_WPAN/App/dis_app.c             Device Information Service application
  - BLE/BLE_p2pServer/STM32_WPAN/App/dis_app.h             Header for dis_app.c module


@par Hardware and Software environment

  - This example runs on STM32WB5MM-DK board (MB1292B).

@par How to use it ?

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load the image into target memory
 - Run the example

Demonstration with smartphone as Client
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor application on the ios/android device
     https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Sensor

 - Power on the STM32WB5MM-DK board with the BLE_P2P_Server application
 - Then, click on the App icon, ST BLE Sensor (smartphone)
 - connect to the device
 - select the P2PSRVx in the device list and play with the Light and the SW1 button of the board

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview

For more details refer to the Application Note:
  AN5289 - Building a Wireless application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
