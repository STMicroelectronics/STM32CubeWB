/**
  @page BLE_Mesh_ThermometerSensor example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    BLE/BLE_Mesh_ThermometerSensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Thermometer Sensor Node demo example.
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  
  @par Application Description
This is the implementation of a BLE Mesh Vendor profile as specified by the BLE SIG.

This example is to demonstrate the BLE Mesh Vendor profile by using a STM32WB5MM Discovery Kit as BLE Mesh Vendor Node 
in order to send measured temperature over BLE Mesh Network through vendor command.


@par Directory contents 
  
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_common.h         	   	Header for all modules with common definition
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_conf.h            	  	Parameters configuration file of the application
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_debug.h            	 	Header for app_debug.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_entry.h            		Parameters configuration file of the application
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_stts221.h				Header for app_stts221.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/app_vl53l0x.h				Header for app_vl53l0x.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/hw_flash.h           		Header for hw_flash.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/hw_if.h						HW interface
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/lp_timer.h					Header for lp_timer.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/stm32_lpm_if.h				Header for stm32_lpm_if.c module (LP management)
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/stm32wb5mm_dk_conf.h		Configuration file of the Discovery board
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Mesh_ThermometerSensor/Core/Inc/vcp_conf.h    				Configuration of the vcp interface
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/app_debug.c        			Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/app_stts221.c				Temperature Application file
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/app_vl53l0x.c				Proximity Application file
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/hw_flash.c					FLASH drive file
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/hw_uart.c					hardware access
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/lp_timer.c					Low power timer
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/main.c                  	Main program
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/stm32_lpm_if.c				Low Power Manager Interface
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/stm32wbxx_it.c       		Interrupt handlers
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/system_stm32wbxx.c    		stm32wbxx system source file  
  - BLE/BLE_Mesh_ThermometerSensor/Core/Src/vl53l0x/					Proximity sensor files
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/app_ble.h          	Header for app_ble.c module
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/ble_conf.h         	BLE Services configuration
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/ble_dbg_conf.h     	BLE Traces configuration of the BLE services
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/app_ble.c      		BLE Profile implementation

  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_config.c         	Config Model APIs callback from ST BLE Mesh Library
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_config.h         	Header file for the application config file 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_config_client.c 	Config Client Model APIs callback from ST BLE Mesh Library
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_config_client.h   	Header file for the application config Client file 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_generic.c         	Application interface for Generic Mesh Models
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_generic.h         	Header of Application interface for Generic Mesh Models 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_generic_client.c	Application interface for Generic Client Mesh Models
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_generic_client.h	Header of Application interface for Generic Client Mesh Models 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light.c           	BLE Mesh Light Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light.h           	Header of BLE Mesh Light Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light_client.c   	BLE Mesh Light Client Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light_client.h    	Header of BLE Mesh Light Client Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light_lc.c        	BLE Mesh Light Lightness Controller Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_light_lc.h        	Header of BLE Mesh Light Lightness Controller Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_mesh.c            	BLE Mesh application implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_mesh.h            	Header of BLE Mesh application implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_nvm.c             	BLE Mesh NVM application implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_nvm.h             	Header of BLE Mesh NVM application implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_sensor.c          	BLE Mesh Sensor Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_sensor.h          	Header of BLE Mesh Sensor Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_sensor_client.c 	BLE Mesh Sensor Client Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_sensor_client.h  	Header of BLE Mesh Sensor Client Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_vendor.c          	BLE Mesh Vendor Model implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/appli_vendor.h          	Header of BLE Mesh Vendor Model implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/hal_common.h            	Header for common function of HAL file
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/mesh_cfg.h              	Header for Mesh configuration
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/mesh_cfg_usr.h          	Header for user Mesh configuration
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/models_if.h             	Header for the BLE Mesh Models Interface file
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/models_if.c             	BLE Mesh Models Interface file
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/pal_nvm.c               	BLE Mesh NVM management implementation
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/pal_nvm.h               	Header of BLE Mesh NVM management implementation 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/svcctl_conf.c           	Service Controller configuration API
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/PWM_config.c            	Pulse Width Modulation configuration
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/PWM_config.h            	Header of Pulse Width Modulation configuration 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/PWM_handlers.c          	Pulse Width Modulation handlers
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/PWM_handlers.h          	Header of Pulse Width Modulation handlers 
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/tl_conf.h               	Configuration file of the Transport layer
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/tl_if.c                 	Transport Layer interface
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/user_if.h               	Header file for User interface file
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/user_if.c               	User interface file
  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/App/vcp_conf.h              	Configuration of the vcp interface

  - BLE/BLE_Mesh_ThermometerSensor/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver

  
@par Hardware and Software environment

    - This application runs on STM32WB5MM Discovery Kit devices
  
    - STM32WB5MM Discovery Kit board Set-up    
       - Connect the Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).

@par How to use it ? 

This application requests having the stm32wb5x_BLE_Stack_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  
   
In order to make the program work, you must do the following :
 - Open your IAR toolchain 
 - Rebuild all files and load the image into Target memory
 - Run the example

For more details refer to the Application Note: 
  AN5292_STM32WB_How_to_build_BLE_Mesh_Application

  
@Introduction

The ST BLE-Mesh library supports all of the following features in a Mesh node:
1. Relay: to forward received network PDUs on advertising or GATT bearer, if the Proxy feature is enabled
2. Proxy: to forward received network PDUs between GATT and advertising bearers
3. Friend: can perform duties such as scanning for other associated nodes that are usually Low Power mode
4. Low Power: consumes much less power as the node usually does not perform any scanning
A typical BLE Mesh node radio should always be in scanning mode to avoid missing packets over the air, but this kind of
operation is not supported by many devices due to continuous energy requirements.
To enable low power operation, BLE Mesh Profile v1.0 defines the concept of Friendship.
In a Mesh network, a Low Power feature-enabled Mesh node (LPN) offloads its packet receiving (scanning) duty to an adjacent
Friend feature-enabled Mesh node (FN). The LPN is not always scanning and remains inactive (or sleeps) for most of its life
cycle, significantly reducing its energy requirements. Nodes running on batteries or energy harvesting techniques usually
support the Low Power feature.
Once connected to each other, the FN and LPN are considered friends and their connection is known as a Friendship. At least
one FN must be in the direct radio range of an LPN to establish Friendship.
  
@Configuration

The Thermometer project supports Proxy, Relay and Friend features. 

@Demo setup based on STM32WB5MM-DK board

General default setting defined in mesh_cfg_usr.h:
        ENABLE_VENDOR_MODEL_SERVER                                    		 (1) /* VENDOR SERVER ONOFF MODEL ON FIRST ELEMENT */
        APPLICATION_NUMBER_OF_ELEMENTS                                         1 /* NUMBER OF ELEMENT SUPPORTED */ 
        APPLICATION_SIG_MODELS_MAX_COUNT                                      10 /* TOTAL NUMBER OF MODELS SUPPORTED */
        APPLICATION_VENDOR_MODELS_MAX_COUNT                                    1 /* TOTAL NIMBER OF VENDOR MODELS SUPPORTED */
        ENABLE_PB_ADV                                                            /* Provisioning Bearer over advertising channels support */
        ENABLE_PB_GATT                                                           /* Provisioning Bearer over GATT proxy support */ 

The following demo based on STM32WB5MM-DK has been set up using one board as a Thermometer Sensor and one or more boards as Temperature Indicators.

THERMOMETER NODE Setup:
Step 1. Configure the THERMOMETER Node.
    To select the THERMOMETER Node code, you need to add THERMOMETER_NODE symbol definition into project settings.
	This mode is enabled by default.
	
Step 2. Generate and flash binary in the first board.
		Once flashed, the Node Type (Thermometer/Indicator), the distance and temperature measured by the board are displayed on the LCD screen.

INDICATOR NODE Setup:
Step 1. Configure the INDICATOR Node.
    To select the INDICATOR Node code, you need to replace the thermometer symbol definition by the INDICATOR_NODE symbol definition into project settings.

Step 2. Generate and flash binary in the other boards.
	Once flashed the distance measured by the board is displayed on the LCD screen.

Demo operation
Step 3. Verify your boards are correctly unprovisioned, by keeping pressed User Button 1 (B1) and Reset button, 
        releasing the reset while keeping B1 pressed until the Blue LED is blinking.

Step 4. Provision all the flashed and unprovisioned boards with the default parameters (Fast povisioning -> Quick configuration), 
        using BLE-Mesh Android (https://play.google.com/store/apps/details?id=com.st.bluenrgmesh&hl=en) or 
        BLE-Mesh iOS (https://apps.apple.com/us/app/st-ble-mesh/id1348645067) smartphone application (subscribe and publish to default group).

Step 5. Modify publication address of your Thermometer Node to publish his vendor commands to the Indicator Nodes (directly to the Node r using Group definitions).

The Thermometer will send Vendor command to change the RGB LED color of the Indicator Node depending on the temperature he is measuring.
Blue = low temperature
Green = medium temperature
Red = High temperature

NOTE: The actual code manage temperature between 20 and 30 degrees Celsius.
      You can change it by modifying min_temp, max_temp.
	  into PublishTemp_Task() function

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */