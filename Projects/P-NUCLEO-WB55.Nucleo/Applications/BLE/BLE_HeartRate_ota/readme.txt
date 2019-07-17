/**
  @page BLE_HeartRate_ota example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_HeartRate_ota/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_HeartRate_ota example.
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

How to use the Heart Rate profile as specified by the BLE SIG.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note This application is not supported by CubeMx but has been copied from the project BLE_HeartRate generated
      by CubeMx with some modifications in order to be able to download it with the BLE_Ota application.
	  The steps to be done to move from the BLE_HeartRate application to the BLE_HeartRate_ota application are :
	  - Copy the full folder BLE_HeartRate
	  - Replace the linker file stm32wb55xx_flash_cm4.icf by stm32wb55xx_flash_cm4_ota.icf ( this adds the placement
	  of the two sections TAG_OTA_START and TAG_OTA_END ).
	  - Remove the update of SCB->VTOR in the file system_stm32wbxx.c ( The VTOR is already set by the BLE_Ota application
	  and shall not be changed to a different value)
          - set BLE_CFG_OTA_REBOOT_CHAR to 1 in ble_conf.h

@par Directory contents 
  
  - BLE/BLE_HeartRate_ota/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_HeartRate_ota/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_HeartRate_ota/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/app_ble.h          Header for app_ble.c module
  - BLE/BLE_HeartRate_ota/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_HeartRate_ota/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_HeartRate_ota/Core/Inc/app_entry.h            	Parameters configuration file of the application
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/ble_conf.h         BLE Services configuration
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/ble_dbg_conf.h     BLE Traces configuration of the BLE services
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/dis_app.h          Header for dis_app.c module
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/hrs_app.h          Header for hrs_app.c module
  - BLE/BLE_HeartRate_ota/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_HeartRate_ota/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_HeartRate_ota/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_HeartRate_ota/Core/Src/main.c                  	Main program
  - BLE/BLE_HeartRate_ota/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/app_ble.c      	BLE Profile implementation
  - BLE/BLE_HeartRate_ota/Core/Src/app_entry.c      		Initialization of the application
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/dis_app.c      	Device Information Service application
  - BLE/BLE_HeartRate_ota/STM32_WPAN/App/hrs_app.c      	Heart Rate Service application
  - BLE/BLE_HeartRate_ota/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_HeartRate_ota/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_HeartRate_ota/Core/Src/hw_timerserver.c 		Timer Server based on RTC
  - BLE/BLE_HeartRate_ota/Core/Src/hw_uart.c 				UART Driver

     
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
 - OR use the BLE_HeartRate_ota_reference.bin from Binary directory
   - to be flashed at 0x0800 7000

 On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en
    https://itunes.apple.com/fr/App/st-ble-profile/id1081331769?mt=8

 - Install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_HeartRate_ota application
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to a device
 - select the HRSTM in the device list

Next demonstration 
 - Move to download panel with the smart Phone Application 
 - select the binary to be downloaded on the Application Processor
   -  BLE_HeartRate_ota_reference.bin or BLE_p2pServer_ota_reference.bin have to be copied into Smart phone directory
 - Start download
 - New Application is running and can be connected    
 
The Heart Rate is displayed each second on the android device.

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 
  UM2551 - STM32CubeWB Nucleo demonstration firmware
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 