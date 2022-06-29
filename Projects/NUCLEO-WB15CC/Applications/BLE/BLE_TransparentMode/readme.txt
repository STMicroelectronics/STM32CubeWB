/**
  @page BLE_TransparentMode application
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_TransparentMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_TransparentMode application.
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

@par application Description

How to communicate with the STM32CubeMonitor-RF Tool using the transparent mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, Dual core

@par Directory contents 
  
  - BLE/BLE_TransparentMode/Core/Inc/app_common.h                Header for all modules with common definition
  - BLE/BLE_TransparentMode/Core/Inc/app_conf.h                  Application configuration file for STM32WPAN Middleware
  - BLE/BLE_TransparentMode/Core/Inc/app_debug.h                 Header for app_debug.c module
  - BLE/BLE_TransparentMode/Core/Inc/app_entry.h                 Interface to the application
  - BLE/BLE_TransparentMode/Core/Inc/hw_conf.h                   Configuration file of the HW
  - BLE/BLE_TransparentMode/Core/Inc/hw_if.h                     Hardware Interface
  - BLE/BLE_TransparentMode/Core/Inc/main.h                      Header for main.c module
  - BLE/BLE_TransparentMode/Core/Inc/nucleo_wb15cc_conf.h        NUCLEO-WB15CC board configuration file.
  - BLE/BLE_TransparentMode/Core/Inc/standby.h                   Header for standby.c module (specific code for standby low power mode)
  - BLE/BLE_TransparentMode/Core/Inc/stm32wbxx_hal_conf.h        HAL configuration file
  - BLE/BLE_TransparentMode/Core/Inc/stm32wbxx_it.h              Interrupt handlers header file
  - BLE/BLE_TransparentMode/Core/Inc/stm32_lpm_if.h              Header for stm32_lpm_if.c module (device specific LP management)
  - BLE/BLE_TransparentMode/Core/Inc/utilities_conf.h            Configuration file of the utilities
  - BLE/BLE_TransparentMode/STM32_WPAN/App/ble_conf.h          	 BLE Services configuration
  - BLE/BLE_TransparentMode/STM32_WPAN/App/ble_dbg_conf.h      	 BLE Traces configuration of the BLE services
  - BLE/BLE_TransparentMode/STM32_WPAN/App/tl_dbg_conf.h         Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_TransparentMode/STM32_WPAN/App/tm.h               	 Header for tm.c module
  - BLE/BLE_TransparentMode/Core/Src/app_debug.c                 Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_TransparentMode/Core/Src/app_entry.c                 Initialization of the application
  - BLE/BLE_TransparentMode/Core/Src/hw_timerserver.c            Timer Server based on RTC
  - BLE/BLE_TransparentMode/Core/Src/hw_uart.c                   UART Driver
  - BLE/BLE_TransparentMode/Core/Src/main.c                      Main program
  - BLE/BLE_TransparentMode/Core/Inc/standby.c                   Specific code for standby low power mode
  - BLE/BLE_TransparentMode/Core/Src/stm32wbxx_hal_msp.c         This file provides code for the MSP Initialization and de-Initialization
  - BLE/BLE_TransparentMode/Core/Src/stm32wbxx_it.c              Interrupt handlers
  - BLE/BLE_TransparentMode/Core/Src/stm32_lpm_if.c              Low Power Manager Interface
  - BLE/BLE_TransparentMode/Core/Src/system_stm32wbxx.c          stm32wbxx system source file
  - BLE/BLE_TransparentMode/STM32_WPAN/App/tm.c 		 Transparent Mode implementation
  - BLE/BLE_TransparentMode/STM32_WPAN/Target/hw_ipcc.c          IPCC Driver
     
@par Hardware and Software environment

  - This application runs on STM32WB15xx devices.
  
  - This application has been tested with an STMicroelectronics STM32WB15CC-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb15_BLE_Stack_full_extended_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file
 - OR use BLE_TransparentMode_reference.hex from Binary directory
 - to test the BLE Transparent Mode application, use the STM32CubeMonitor RF tool
 - make the connection between STM32CubeMonitor RF tool and BLE_TransparentMode application
 - send some ACI commands from STM32CubeMonitor RF tool as HCI_RESET, HCI_LE_RECEIVER_TEST, HCI_LE_TRANSMITTER_TEST, ...
 - the application must acknowledge the command with a "Command Complete" answer 
 
For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 