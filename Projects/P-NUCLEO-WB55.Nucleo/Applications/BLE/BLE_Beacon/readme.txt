/**
  @page BLE_Beacon application
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Beacon/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_Beacon application
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

How to advertise 3 types of beacon ( tlm, uuid, url ).

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
  
  - BLE_Beacon/Core/Inc/app_common.h                            App Common application configuration file for STM32WPAN Middleware.
  - BLE_Beacon/Core/Inc/app_conf.h                              Application configuration file for STM32WPAN Middleware.
  - BLE_Beacon/Core/Inc/app_debug.h                             Header for app_debug.c module
  - BLE_Beacon/Core/Inc/app_entry.h                             Interface to the application
  - BLE_Beacon/Core/Inc/hw_conf.h                               Configuration of hardware interface
  - BLE_Beacon/Core/Inc/hw_if.h                                 Hardware Interface
  - BLE_Beacon/Core/Inc/main.h                                  Header for main.c module
  - BLE_Beacon/Core/Inc/stm32wbxx_hal_conf.h                    HAL configuration file.
  - BLE_Beacon/Core/Inc/stm32wbxx_it.h                          This file contains the headers of the interrupt handlers.
  - BLE_Beacon/Core/Inc/stm32_lpm_if.h                          Header for stm32_lpm_if.c module (device specific LP management)
  - BLE_Beacon/Core/Inc/utilities_conf.h                        Configuration file for STM32 Utilities.
  - BLE_Beacon/STM32_WPAN/App/app_ble.h                         Header for ble application
  - BLE_Beacon/STM32_WPAN/App/ble_conf.h                        Configuration file for BLE Middleware.
  - BLE_Beacon/STM32_WPAN/App/ble_dbg_conf.h                    Debug configuration file for BLE Middleware.
  - BLE_Beacon/STM32_WPAN/App/eddystone_beacon.h                Header for eddystone beacon parameter  
  - BLE_Beacon/STM32_WPAN/App/eddystone_tlm_service.h           Header for eddystone_tlm_service.c module 
  - BLE_Beacon/STM32_WPAN/App/eddystone_uid_service.h           Header for eddystone_uid_service.c module 
  - BLE_Beacon/STM32_WPAN/App/eddystone_url_service.h           Header for eddystone_url_service.c module 
  - BLE_Beacon/STM32_WPAN/App/ibeacon.h                         Header for ibeacon beacon parameter
  - BLE_Beacon/STM32_WPAN/App/ibeacon_service.h                 Header for ibeacon_service.c module 
  - BLE_Beacon/STM32_WPAN/App/tl_dbg_conf.h                     Debug configuration file for stm32wpan transport layer interface.
  - BLE_Beacon/Core/Src/app_debug.c                             Debug capabilities source file for STM32WPAN Middleware
  - BLE_Beacon/Core/Src/app_entry.c                             Entry point of the Application
  - BLE_Beacon/Core/Src/hw_timerserver.c                        Hardware timerserver source file for STM32WPAN Middleware.
  - BLE_Beacon/Core/Src/hw_uart.c                               HW UART source file for STM32WPAN Middleware.
  - BLE_Beacon/Core/Src/main.c                                  BLE application with BLE core
  - BLE_Beacon/Core/Src/stm32wbxx_hal_msp.c                     This file provides code for the MSP Initialization 
  - BLE_Beacon/Core/Src/stm32wbxx_it.c                          Main Interrupt Service Routines.
  - BLE_Beacon/Core/Src/stm32_lpm_if.c                          Low layer function to enter/exit low power modes (stop, sleep).
  - BLE_Beacon/Core/Src/system_stm32wbxx.c                      CMSIS Cortex Device Peripheral Access Layer System Source File
  - BLE_Beacon/STM32CubeIDE/Application/User/Core/syscalls.c    STM32CubeIDE Minimal System calls file
  - BLE_Beacon/STM32CubeIDE/Application/User/Core/sysmem.c      STM32CubeIDE Minimal System Memory calls file
  - BLE_Beacon/STM32_WPAN/App/app_ble.c                         BLE Application
  - BLE_Beacon/STM32_WPAN/App/eddystone_tlm_service.c           Eddystone tlm service application 
  - BLE_Beacon/STM32_WPAN/App/eddystone_uid_service.c           Eddystone uid service application
  - BLE_Beacon/STM32_WPAN/App/eddystone_url_service.c           Eddystone url service application
  - BLE_Beacon/STM32_WPAN/App/ibeacon_service.c                 Ibeacon service application 
  - BLE_Beacon/STM32_WPAN/Target/hw_ipcc.c                      Hardware IPCC source file for STM32WPAN Middleware.
     
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
 - 4 Beacon types are available and have to be defined in app_con.h file : 
	-#define CFG_BEACON_TYPE must be set at one of the following value :
		- CFG_EDDYSTONE_UID_BEACON_TYPE
		- CFG_EDDYSTONE_URL_BEACON_TYPE
		- CFG_EDDYSTONE_TLM_BEACON_TYPE
		- CFG_IBEACON

 - On the smartphone, download Beacon Scanner application,
 - Enable Bluetooth communications
 - Run Beacon Scanner application and start scanning
 - The Beacon information are displayed on the smartphone

Available Wiki page:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview


For more details refer to the Application Note: 
  AN5289 - Building a Wireless application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
