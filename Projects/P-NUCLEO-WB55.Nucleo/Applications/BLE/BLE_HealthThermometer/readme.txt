/**
  @page BLE_HealthThermometer application
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_HealthThermometer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_HealthThermometer application.
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

@par application Description

How to use the Health Thermometer profile as specified by the BLE SIG.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 

  - BLE/BLE_HealthThermometer/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_HealthThermometer/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_HealthThermometer/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE/BLE_HealthThermometer/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_HealthThermometer/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_HealthThermometer/Core/Inc/hw_if.h                       Hardware Interface header file
  - BLE/BLE_HealthThermometer/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_HealthThermometer/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module  
  - BLE/BLE_HealthThermometer/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_HealthThermometer/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_HealthThermometer/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/app_ble.h               Header for app_ble.c module
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/dis_app.h               Header for dis_app.c module
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/hts_app.h               Header for hts_app.c module
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_HealthThermometer/Core/Src/app_debug.c                   Debug utilities
  - BLE/BLE_HealthThermometer/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_HealthThermometer/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_HealthThermometer/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_HealthThermometer/Core/Src/main.c                        Main program
  - BLE/BLE_HealthThermometer/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_HealthThermometer/Core/Src/stm32wbxx_hal_msp.c           MSP Initialization Module
  - BLE/BLE_HealthThermometer/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_HealthThermometer/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/dis_app.c               Device Information Service application
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/hts_app.c               Health Thermometer Service application
  - BLE/BLE_HealthThermometer/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

@par Hardware and Software environment

  - This application runs on STM32WB55xx devices.

  - This application has been tested with an STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device 
    and development board.

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

 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor and/or ST BLE Toolbox applications:
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox

 - Power on the Nucleo board with the BLE_HealthThermometer application
 - Then, click on the App icon, STM32 BLE Toolbox (android device)
 - select the HTSTM in the device list
 - connect the application
 - open Health Thermometer service

The temperature is displayed each second on the android device.

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview

For more details refer to the Application Note:
  AN5289 - Building a Wireless application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
