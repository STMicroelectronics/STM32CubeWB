/**
  @page BLE_HealthThermometer example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_HealthThermometer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_HealthThermometer example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the Health Thermometer profile as specified by the BLE SIG.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - BLE/BLE_HealthThermometer/Core/Inc/stm32wbxx_hal_conf.h			HAL configuration file
  - BLE/BLE_HealthThermometer/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_HealthThermometer/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/app_ble.h             	Header for app_ble.c module
  - BLE/BLE_HealthThermometer/Core/Inc/app_common.h            		Header for all modules with common definition
  - BLE/BLE_HealthThermometer/Core/Inc/app_conf.h              		Parameters configuration file of the application
  - BLE/BLE_HealthThermometer/Core/Inc/app_entry.h                 	Parameters configuration file of the application
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/ble_conf.h            	BLE Services configuration
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/ble_dbg_conf.h        	BLE Traces configuration of the BLE services
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/dis_app.h             	Header for dis_app.c module
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/hts_app.h           	Header for hts_app.c module
  - BLE/BLE_HealthThermometer/Core/Inc/hw_conf.h           			Configuration file of the HW
  - BLE/BLE_HealthThermometer/Core/Inc/utilities_conf.h    			Configuration file of the utilities
  - BLE/BLE_HealthThermometer/Core/Src/stm32wbxx_it.c          		Interrupt handlers
  - BLE/BLE_HealthThermometer/Core/Src/main.c                  		Main program
  - BLE/BLE_HealthThermometer/Core/Src/system_stm32wbxx.c      		stm32wbxx system source file
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_HealthThermometer/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/dis_app.c      		Device Information Service application
  - BLE/BLE_HealthThermometer/STM32_WPAN/App/hts_app.c      		Health Thermometer Service application
  - BLE/BLE_HealthThermometer/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver
  - BLE/BLE_HealthThermometer/Core/Src/stm32_lpm_if.c				Low Power Manager Interface
  - BLE/BLE_HealthThermometer/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_HealthThermometer/Core/Src/hw_uart.c 					UART Driver

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55VG-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requests having the stm32wb5x_BLE_Stack_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file

 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en
    https://itunes.apple.com/fr/App/st-ble-profile/id1081331769?mt=8

 - Power on the Nucleo board with the BLE_HealthThermometer application
 - Then, click on the App icon, STM32 BLE Profiles (android device)
 - select the HTSTM in the device list
 - In Profiles, select Health Thermometer application

The temperature is displayed each second on the android device.

 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 