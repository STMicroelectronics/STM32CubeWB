/**
  @page BLE_HeartRateFreeRTOS example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_HeartRateFreeRTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_HeartRateFreeRTOS example.
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

@par Example Description

How to use the Heart Rate profile as specified by the BLE SIG with FreeRTOS.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note This application is not supported by CubeMx but has been copied from the project BLE_HeartRate generated
      by CubeMx with some modifications to replace the call to the scheduler by the use of the cmsis_os interface

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/main.h                  Header for main.c module
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/app_ble.h         Header for app_ble.c module
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/app_common.h            Header for all modules with common definition
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/app_conf.h              Parameters configuration file of the application
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/app_entry.h             Parameters configuration file of the application
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/ble_conf.h        BLE Services configuration
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/ble_dbg_conf.h    BLE Traces configuration of the BLE services
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/dis_app.h         Header for dis_app.c module
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/hrs_app.h         Header for hrs_app.c module
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE/BLE_HeartRateFreeRTOS/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/FreeRTOSConfig.h  Configuration file of FreeRTOS
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/main.c                  Main program
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/app_ble.c         BLE Profile implementation
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/app_entry.c             Initialization of the application
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/dis_app.c         Device Information Service application
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/App/hrs_app.c         Heart Rate Service application
  - BLE/BLE_HeartRateFreeRTOS/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/hw_timerserver.c        Timer Server based on RTC
  - BLE/BLE_HeartRateFreeRTOS/Core/Src/hw_uart.c               UART Driver

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55VG-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file

 On the android/ios device, enable the Bluetooth communications, and if not done before,

 - Install the ST BLE Sensor application on the ios/android device
      https://play.google.com/store/apps/details?id=com.st.bluems
      https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_HeartRateFreeRTOS application
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to a device
 - select the HRSTM in the device list

The Heart Rate is displayed each second on the android device.

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 