/**
  @page BLE_HR_p2pServer Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_HR_p2pServer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BLE_HR_p2pServer application
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


@note This application includes two BLE services, the first one is a BLE_P2P_Server like, including two characteristics; the second one is a BLE_Heart_Rate like with three characteristics.

@note A display terminal can be used to display application messages.
      This application is able to advertise, to be connected by a Smart phone (ST BLE Toolbox application) and to manage the BLE events according to each characteristic.
      This application is done to be used with a B-WB1M-WPAN1 board only.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents

  - BLE/BLE_HR_p2pServer/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_HR_p2pServer/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_HR_p2pServer/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE/BLE_HR_p2pServer/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_HR_p2pServer/Core/Inc/b_wb1m_wpan1_conf.h           Configuration file of the board
  - BLE/BLE_HR_p2pServer/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_HR_p2pServer/Core/Inc/hw_if.h                       Hardware Interface header file
  - BLE/BLE_HR_p2pServer/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_HR_p2pServer/Core/Inc/standby.h                     Header for standby.c module
  - BLE/BLE_HR_p2pServer/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module  
  - BLE/BLE_HR_p2pServer/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_HR_p2pServer/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_HR_p2pServer/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/app_ble.h               Header for app_ble.c module
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/custom_app.h            Header for custom_app.c module
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/custom_stm.h            Header for custom_stm.c module
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/template_server_app.h   Header for p2p_server_app.c module
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_HR_p2pServer/Core/Src/app_debug.c                   Debug utilities
  - BLE/BLE_HR_p2pServer/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_HR_p2pServer/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_HR_p2pServer/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_HR_p2pServer/Core/Src/main.c                        Main program
  - BLE/BLE_HR_p2pServer/Core/Src/standby.c                     Specific code for standby low power mode
  - BLE/BLE_HR_p2pServer/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_HR_p2pServer/Core/Src/stm32wbxx_hal_msp.c           MSP Initialization Module
  - BLE/BLE_HR_p2pServer/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_HR_p2pServer/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/custom_app.c            Custom example application
  - BLE/BLE_HR_p2pServer/STM32_WPAN/App/custom_stm.c            Custom example services definition
  - BLE/BLE_HR_p2pServer/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

@par Hardware and Software environment

     - This application runs on B-WB1M-WPAN1 board

     - B-WB1M-WPAN1 board Set-up
       - To power the board use USB cable type A to USB type C connector through MB1880 board or use CN4 connector (Pin 1 : GND, Pin 2 : 5V)
       - Use an external STLINK-V3 in order to flash the binary

@par How to use it?

This application requires having the stm32wb1x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file using an external STLINK-V3

 On the android/ios device:
 - Install the ST BLE Toolbox application:
   https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox
 - Enable Bluetooth communications
 - Open ST BLE Toolbox and Start Scanning.
 - Connect to WB1M_HR application to access all the services and associated characteristics
 - The BLE_HR_p2pServer information are displayed on the smartphone
 - Open Heart Rate and enable Heart Rate Measurement notification:
   - Heart Rate Measurement are sent each second to the smartphone
 - Open P2P_Server and enable P2P Notify:
   - press SW1 on Nucleo to toggle value on the smart phone   
   - write hexa value 0001 to switch the blue led ON on the Nucleo board
   - write hexa value 0000 to switch the blue led OFF on the Nucleo board
 - Pairing is supported using bond button on ST BLE Toolbox

   Available Wiki pages:
   - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview
   - https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_STM32CubeMX

For more details refer to the Application Note:
  AN5289 - Building a Wireless application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
