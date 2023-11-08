/**
  @page BLE_Power_Peripheral Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Power_Peripheral/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BLE_Power_Peripheral application
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


@note This application is to demonstrate that a BLE_Power_Peripheral application can be created using CubeMX.
      This application includes two BLE services, the first one is a BLE_P2P_Server like, including two characteristics; the second one is a BLE_Heart_Rate like with three characteristics.
      All the application behavior must be developed by the user.
@note A display terminal can be used to display application messages.
      This application is able to advertise, to be connected by a Smart phone (ST BLE Sensor application) and to manage the BLE events according to each characteristic.
      This application is done to be used with a Nucleo board (MB1355C) only.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents

  - BLE/BLE_Power_Peripheral/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_Power_Peripheral/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_Power_Peripheral/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE/BLE_Power_Peripheral/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_Power_Peripheral/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_Power_Peripheral/Core/Inc/hw_if.h                       Hardware Interface header file
  - BLE/BLE_Power_Peripheral/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_Power_Peripheral/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module  
  - BLE/BLE_Power_Peripheral/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_Power_Peripheral/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_Power_Peripheral/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/app_ble.h               Header for app_ble.c module
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/custom_app.h            Header for custom_app.c module
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/custom_stm.h            Header for custom_stm.c module
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_Power_Peripheral/Core/Src/app_debug.c                   Debug utilities
  - BLE/BLE_Power_Peripheral/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_Power_Peripheral/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_Power_Peripheral/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_Power_Peripheral/Core/Src/main.c                        Main program
  - BLE/BLE_Power_Peripheral/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_Power_Peripheral/Core/Src/stm32wbxx_hal_msp.c           MSP Initialization Module
  - BLE/BLE_Power_Peripheral/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_Power_Peripheral/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/custom_app.c            PwrCo_Peripheral example application
  - BLE/BLE_Power_Peripheral/STM32_WPAN/App/custom_stm.c            PwrCo_Peripheral example services definition
  - BLE/BLE_Power_Peripheral/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

@par Hardware and Software environment

	- This application runs on STM32WB55xx devices.

	- This application has been tested with an STMicroelectronics P-NUCLEO-WB55
		board and can be easily tailored to any other supported device 
		and development board.

	- This application is by default configured to support low power mode ( No traces - No debugger )
		This can be modified in app_conf.h (CFG_LPM_SUPPORTED, CFG_DEBUG_BLE_TRACE, CFG_DEBUG_APP_TRACE)

	- SMPS can be used to obtain the best performance in power consumption.
		This feature depends on board components and its activation can be done through specific define (CFG_USE_SMPS)
		configurable in app_conf.h
		See AN5246 - Usage of STM32WB Series microcontrollers.

    - Nucleo board (MB1355C) Set-up
      - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
      - Please ensure that the ST-LINK connectors and jumpers are fitted.

@par How to use it?

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Configure parameters as you want in app_conf.h in the USER CONFIGURATION section
 - Rebuild all files and flash the board with the executable file

 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Toolbox application:
   https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox
 - Enable Bluetooth communications
 - Open ST BLE Toolbox and Start Scanning.
 - Connect to PWR application to access all the services and associated characteristics
 - Open InterOpTesting and enable Notify
   
+-------------------+-------------------------+--------------------+-----------------------------------+
| Connection Status | B1 Click                | B2 Click           | B3 Click                          |
+==================-+=========================+====================+===================================+
| Non-Connected     | -                       | Switch Advertising | Switch Advertising type           |
+-------------------+-------------------------+--------------------+-----------------------------------+
| Connected         | Start/Stop Notification | Toggle PHY 1M/2M   | Update connection interval request|
+-------------------+-------------------------+--------------------+-----------------------------------+
   
   Available Wiki pages:
   - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview
   - https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_STM32CubeMX

For more details refer to the Application Note:
  AN5289 - Building a Wireless application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
