/**
  @page BLE_Proximity application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Proximity/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Proximity application.
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

How to use the Proximity profile as specified by the BLE SIG.

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

  - BLE/BLE_Proximity/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_Proximity/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_Proximity/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE/BLE_Proximity/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_Proximity/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_Proximity/Core/Inc/hw_if.h                       Hardware Interface header file
  - BLE/BLE_Proximity/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_Proximity/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module  
  - BLE/BLE_Proximity/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_Proximity/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_Proximity/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_Proximity/STM32_WPAN/App/app_ble.h               Header for app_ble.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_Proximity/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_Proximity/STM32_WPAN/App/ias_app.h               Header for ias_app.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/lls_app.h               Header for lls_app.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/tps_app.h               Header for tps_app.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_Proximity/Core/Src/app_debug.c                   Debug utilities
  - BLE/BLE_Proximity/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_Proximity/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_Proximity/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_Proximity/Core/Src/main.c                        Main program
  - BLE/BLE_Proximity/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_Proximity/Core/Src/stm32wbxx_hal_msp.c           MSP Initialization Module
  - BLE/BLE_Proximity/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_Proximity/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_Proximity/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_Proximity/STM32_WPAN/App/ias_app.c               Immediate Alerte Service application
  - BLE/BLE_Proximity/STM32_WPAN/App/lls_app.c               Link Loss Service application
  - BLE/BLE_Proximity/STM32_WPAN/App/tps_app.c               Tx Power Service application
  - BLE/BLE_Proximity/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

@par Hardware and Software environment

  - This application runs on STM32WB55xx devices.

  - This application has been tested with an STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it?

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
 - Install the ST BLE Profile application on the android device
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox

The STM32WB55xx board is used as GATT server and GAP peripheral.
With the Smart Phone application:
 - Select the device (STM32WB55xx)
 - Select Immediate Alert Service or Link Loss Service.
 - The Immediate Alert Service sets the alert level
   - The corresponding Led is switched ON on the nucleo board.
      - No Alert: green led
      - Mild Alert: blue led
      - High Alert: red led

 - The Link Loss Service sets an alert level. This alert level (No, Mild or High) is set by the user on the phone.
    Then when the link is lost (the phone has been far enough away from the Nucleo board for example)
    the corresponding Led is switched on, on the board.
 
 Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview

For more details refer to the Application Note:
  AN5289 - Building a Wireless application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
