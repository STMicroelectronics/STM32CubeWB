/**
  @page BLE_Custom Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Custom/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_Custom application
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

This example is to demonstrate that a BLE_Custom application can be created using CubeMX.
This application includes two BLE services, the first one is a BLE_P2P_Server like, including two characteristics; the second one is a BLE_Heart_Rate like with three characteristics.
All the application behavior must be developed by the user.
A display terminal can be used to display application messages.
This application is able to advertise, to be connected by a Smart phone (ST BLE Sensor application) and to manage the BLE events according to each characteristic. 
This example is done to be used with a Nucleo board (MB1355C) only.
	
@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_Custom/Core/Inc/stm32wbxx_hal_conf.h	        HAL configuration file
  - BLE/BLE_Custom/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_Custom/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_Custom/STM32_WPAN/App/app_ble.h           		Header for app_ble.c module
  - BLE/BLE_Custom/Core/Inc/app_common.h            		Header for all modules with common definition
  - BLE/BLE_Custom/Core/Inc/app_conf.h              		Parameters configuration file of the application
  - BLE/BLE_Custom/Core/Inc/app_entry.h            		Parameters configuration file of the application
  - BLE/BLE_Custom/STM32_WPAN/App/ble_conf.h            	BLE Services configuration
  - BLE/BLE_Custom/STM32_WPAN/App/ble_dbg_conf.h        	BLE Traces configuration of the BLE services
  - BLE/BLE_Custom/STM32_WPAN/App/custom_stm.h   		Header for custom_stm.c module
  - BLE/BLE_Custom/STM32_WPAN/App/custom_app.h   		Header for custom_app.c module
  - BLE/BLE_Custom/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Custom/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Custom/Core/Src/stm32wbxx_it.c          		Interrupt handlers
  - BLE/BLE_Custom/Core/Src/main.c                  		Main program
  - BLE/BLE_Custom/Core/Src/system_stm32wbxx.c      		stm32wbxx system source file
  - BLE/BLE_Custom/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_Custom/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_Custom/STM32_WPAN/App/custom_stm.c   		Custom example services definition
  - BLE/BLE_Custom/STM32_WPAN/App/custom_app.c   		Custom example application
  - BLE/BLE_Custom/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver
  - BLE/BLE_Custom/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_Custom/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_Custom/Core/Src/hw_uart.c 				UART Driver
  
@par Hardware and Software environment

    - This application runs on STM32WB55xx device, Nucleo board (MB1355C)
    
    - Nucleo board (MB1355C) Set-up    
       - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin or stm32wb5x_BLE_Stack_light_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load the image into Target memory
 - This must be done for BLE_Custom (MB1355C) 

 - On the smartphone, open ST BLE Sensor application,
 - if not already installed, install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8
 - Enable Bluetooth communications
 - Open BLE Toolbox and Start Scanning.
 - Connect to MyCST application to access all the services and associated characteristics
 - The BLE_Custom information are displayed on the smartphone


  For more details refer to the Application Note: 
  AN5289 - Building a Wireless application  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
