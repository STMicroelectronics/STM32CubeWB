/**
  @page BLE_Proximity example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Proximity/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Proximity example.
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

How to use the Proximity profile as specified by the BLE SIG.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 
  
  - BLE/BLE_Proximity/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_Proximity/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_Proximity/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/app_ble.h          Header for app_ble.c module
  - BLE/BLE_Proximity/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_Proximity/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_Proximity/Core/Inc/app_entry.h              Parameters configuration file of the application
  - BLE/BLE_Proximity/STM32_WPAN/App/ble_conf.h         BLE Services configuration
  - BLE/BLE_Proximity/STM32_WPAN/App/ble_dbg_conf.h     BLE Traces configuration of the BLE services
  - BLE/BLE_Proximity/STM32_WPAN/App/ias_app.h      	Header for ias_app.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/lls_app.h      	Header for lls_app.c module
  - BLE/BLE_Proximity/STM32_WPAN/App/tps_app.h      	Header for tps_app.c module
  - BLE/BLE_Proximity/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Proximity/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Proximity/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_Proximity/Core/Src/main.c                  	Main program
  - BLE/BLE_Proximity/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_Proximity/STM32_WPAN/App/app_ble.c      	BLE Profile implementation
  - BLE/BLE_Proximity/Core/Src/app_entry.c      		Initialization of the application
  - BLE/BLE_Proximity/STM32_WPAN/App/ias_app.c      	Immediate Alerte Service application
  - BLE/BLE_Proximity/STM32_WPAN/App/lls_app.c      	Link Loss Service application
  - BLE/BLE_Proximity/STM32_WPAN/App/tps_app.c      	Tx Power Service application
  - BLE/BLE_Proximity/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_Proximity/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_Proximity/Core/Src/hw_timerserver.c 		Timer Server based on RTC
  - BLE/BLE_Proximity/Core/Src/hw_uart.c 				UART Driver

     
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
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 
 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en

The STM32WB55xx board is used as GATT server and GAP peripheral.
With the Smart Phone application: 
 - Select the device (STM32WB55xx)
 - Select Find Me or Proximity Profile. 
   - Find Me Profile includes Immediate Alert Service.
   - Proximity Profile includes Immediate Alert Service, Link Loss Service and Tx power Service. 

 - The Find Me Service sets the alert level
   - The corresponding led is switched ON on the nucleo board.
      - No Alert: green led
      - Mild Alert: blue led
      - High Alert: red led

 - The Proximity Service Set an RSSI threshold 
   - The red led is switched ON (green led switched OFF) if the Path loss value exceeds the threshold
   - The green led is switched ON red led switched OFF) if the Path loss is under the threshold.
   - The path loss value varies with the distance beetween the phone and the nucleo board.

 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 