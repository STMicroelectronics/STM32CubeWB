/**
  @page BLE_Ota add here description
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Ota/readme.txt 
  * @author  MCD Application Team
  * @brief   OTA implementation
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

OTA implementation to download a new image into the user flash.

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
  
  - BLE/BLE_Ota/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_Ota/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_Ota/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_Ota/STM32_WPAN/App/app_ble.h            Header for app_ble.c module
  - BLE/BLE_Ota/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_Ota/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_Ota/Core/Inc/app_entry.h              	Parameters configuration file of the application
  - BLE/BLE_Ota/STM32_WPAN/App/ble_conf.h           BLE Services configuration
  - BLE/BLE_Ota/STM32_WPAN/App/ble_dbg_conf.h       BLE Traces configuration of the BLE services
  - BLE/BLE_Ota/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Ota/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Ota/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_Ota/Core/Src/main.c                  	Main program
  - BLE/BLE_Ota/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_Ota/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_Ota/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_Ota/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_Ota/Core/Src/stm32_lpm_if.c				Low Power Manager Interface
  - BLE/BLE_Ota/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_Ota/Core/Src/hw_uart.c 					UART Driver
  - BLE/BLE_Ota/STM32_WPAN/App/otas_app.c 			The OTA service management

     
@par Hardware and Software environment

  - This example runs on STM32WB15xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB15CC-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb15_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Please erase Flash memory before loading example
 - Rebuild all files and load your image into target memory
 - OR use BLE_Ota_reference.hex from Binary directory

On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8
	
The Ble_Ota Application allows a remote device to download an application binary 
on CPU1 application processor or to download a Wireless FW (as radio stack or RSS/FUS updates)
on CPU2 Stack processor.

For application binary update example:
 - At Startup, the Ble_Ota application advertises "STM_OTA"
 - with Smart Phone "ST BLE Sensor" application, scan and connect to "STM_OTA" Application.
 - Next, select the binary to be downloaded on the Application Processor
   -  BLE_HeartRate_ota_reference.bin or BLE_p2pServer_ota_reference.bin have to be copied into Smart phone directory
 - Start download
 - New Application is running and can be connected
  
For Wireless FW binary update example:
 - At Startup, the Ble_Ota application advertises "STM_OTA"
 - with Smart Phone "ST BLE Sensor" application, scan and connect to "STM_OTA" Application.
 - Next, select the binary to be downloaded on the Stack Processor
   - copy into Smart phone directory binaries available in STM32WB_Copro_Wireless_Binaries folder
     as stm32wb15_FUS_fw.bin or stm32wb15_BLE_Stack_full_fw.bin 
    (please refer to UM2551 - STM32CubeWB Nucleo demonstration firmware, chapter 4.3)
 - Start download in free area of non secure Flash memory
 - New Application is running and can be connected

+ Config limitation: the Debug configuration of BLE_Ota CubeIDE project is not supported by current BLE_Ota application settings
as generated binary in Cube IDE debug mode is too large for BLE_ota allocated 24kB Flash sectors (between 0x8000000-0x8006FFF)
considering that applications binaries will be flashed at 0x08007000.
 
For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 