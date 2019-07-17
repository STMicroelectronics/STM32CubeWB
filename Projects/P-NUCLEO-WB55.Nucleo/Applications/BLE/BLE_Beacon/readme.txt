/**
  @page BLE_Beacon add here description
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Beacon/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_Beacon application
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

How to advertize 3 types of beacon ( tlm, uuid, url ).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - BLE/BLE_Beacon/Core/Inc/stm32wbxx_hal_conf.h			HAL configuration file
  - BLE/BLE_Beacon/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_Beacon/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_Beacon/STM32_WPAN/App/app_ble.h             	Header for app_ble.c module
  - BLE/BLE_Beacon/Core/Inc/app_common.h            		Header for all modules with common definition
  - BLE/BLE_Beacon/Core/Inc/app_conf.h              		Parameters configuration file of the application
  - BLE/BLE_Beacon/Core/Inc/app_entry.h             		Parameters configuration file of the application
  - BLE/BLE_Beacon/STM32_WPAN/App/ble_conf.h            	BLE Services configuration
  - BLE/BLE_Beacon/STM32_WPAN/App/ble_dbg_conf.h        	BLE Traces configuration of the BLE services
  - BLE/BLE_Beacon/Core/Inc/hw_conf.h           	    	Configuration file of the HW
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_beacon.h    	Configuration file of eddystone beacon
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_tlm_service.h   Header for eddystone_tlm_service.c module
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_uid_service.h   Header for eddystone_uid_service.c module
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_url_service.h   Header for eddystone_url_service.c module
  - BLE/BLE_Beacon/STM32_WPAN/App/ibeacon.h    				Configuration file of ibeacon
  - BLE/BLE_Beacon/STM32_WPAN/App/ibeacon_service.h     	Header for ibeacon_service.c module
  - BLE/BLE_Beacon/Core/Inc/utilities_conf.h    			Configuration file of the utilities
  - BLE/BLE_Beacon/Core/Src/stm32wbxx_it.c          		Interrupt handlers
  - BLE/BLE_Beacon/Core/Src/main.c                  		Main program
  - BLE/BLE_Beacon/Core/Src/system_stm32wbxx.c      		stm32wbxx system source file
  - BLE/BLE_Beacon/STM32_WPAN/App/app_ble.c      			BLE Profile implementation
  - BLE/BLE_Beacon/Core/Src/app_entry.c      				Initialization of the application
  - BLE/BLE_Beacon/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver
  - BLE/BLE_Beacon/Core/Src/stm32_lpm_if.c					Low Power Manager Interface
  - BLE/BLE_Beacon/Core/Src/hw_timerserver.c 				Timer Server based on RTC
  - BLE/BLE_Beacon/Core/Src/hw_uart.c 						UART Driver
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_tlm_service.c 	tlm beacon implementation
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_uid_service.c 	uuid beacon implementation
  - BLE/BLE_Beacon/STM32_WPAN/App/eddystone_url_service.c 	url beacon implementation
  - BLE/BLE_Beacon/STM32_WPAN/App/ibeacon_service.c 		ibeacon implementation

     
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

Limitation
 - The Update Over The Air user data section is not supported
 - The Application erase with SW1 button is not supported  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 