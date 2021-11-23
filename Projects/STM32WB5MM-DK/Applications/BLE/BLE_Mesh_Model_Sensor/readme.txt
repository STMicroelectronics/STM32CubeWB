/**
  @page BLE_Mesh_Model_Sensor example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Mesh_Model_Sensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Thermometer Sensor Node demo example.
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  
  @par Application Description
This is the implementation of a BLE Mesh Sensor Model  (Client and Server) as specified by the BLE SIG.

This example is to demonstrate the BLE Mesh Sensor Model (Client and Server) by using a STM32WB5MM Discovery Kit as BLE Mesh Sensor Node 
in order to send measured temperature and distance on client request over BLE Mesh Network through sensor command.

@par Keywords

Connectivity, BLE, Mesh, Model, Sensor, BLE protocol, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_common.h         	   	Header for all modules with common definition
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_conf.h            	  	Parameters configuration file of the application
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_debug.h            	 	Header for app_debug.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_entry.h            		Parameters configuration file of the application
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_stts221.h			Header for app_stts221.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/app_vl53l0x.h			Header for app_vl53l0x.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/hw_flash.h           		Header for hw_flash.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/hw_if.h				HW interface
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/lp_timer.h			Header for lp_timer.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/stm32_lpm_if.h			Header for stm32_lpm_if.c module (LP management)
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/stm32wb5mm_dk_conf.h		Configuration file of the Discovery board
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Mesh_Model_Sensor/Core/Inc/vcp_conf.h    			Configuration of the vcp interface
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/app_debug.c        		Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/app_entry.c      		Initialization of the application
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/app_stts221.c			Temperature Application file
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/app_vl53l0x.c			Proximity Application file
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/hw_flash.c			FLASH drive file
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/hw_timerserver.c 		Timer Server based on RTC
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/hw_uart.c			hardware access
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/lp_timer.c			Low power timer
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/main.c                  		Main program
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/stm32wbxx_it.c       		Interrupt handlers
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/system_stm32wbxx.c    		stm32wbxx system source file  
  - BLE/BLE_Mesh_Model_Sensor/Core/Src/vl53l0x/				Proximity sensor files
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/app_ble.h          	Header for app_ble.c module
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/ble_conf.h         	BLE Services configuration
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/ble_dbg_conf.h     	BLE Traces configuration of the BLE services
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/app_ble.c      		BLE Profile implementation

  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_config.c         	Config Model APIs callback from ST BLE Mesh Library
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_config.h         	Header file for the application config file 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_config_client.c 	Config Client Model APIs callback from ST BLE Mesh Library
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_config_client.h   	Header file for the application config Client file 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_generic.c         	Application interface for Generic Mesh Models
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_generic.h         	Header of Application interface for Generic Mesh Models 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_generic_client.c	Application interface for Generic Client Mesh Models
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_generic_client.h	Header of Application interface for Generic Client Mesh Models 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light.c           	BLE Mesh Light Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light.h           	Header of BLE Mesh Light Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light_client.c   	BLE Mesh Light Client Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light_client.h    	Header of BLE Mesh Light Client Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light_lc.c        	BLE Mesh Light Lightness Controller Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_light_lc.h        	Header of BLE Mesh Light Lightness Controller Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_mesh.c            	BLE Mesh application implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_mesh.h            	Header of BLE Mesh application implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_nvm.c             	BLE Mesh NVM application implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_nvm.h             	Header of BLE Mesh NVM application implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_sensor.c          	BLE Mesh Sensor Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_sensor.h          	Header of BLE Mesh Sensor Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_sensor_client.c 	BLE Mesh Sensor Client Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_sensor_client.h  	Header of BLE Mesh Sensor Client Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_vendor.c          	BLE Mesh Vendor Model implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/appli_vendor.h          	Header of BLE Mesh Vendor Model implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/hal_common.h            	Header for common function of HAL file
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/mesh_cfg.h              	Header for Mesh configuration
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/mesh_cfg_usr.h          	Header for user Mesh configuration
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/models_if.h             	Header for the BLE Mesh Models Interface file
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/models_if.c             	BLE Mesh Models Interface file
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/pal_nvm.c               	BLE Mesh NVM management implementation
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/pal_nvm.h               	Header of BLE Mesh NVM management implementation 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/svcctl_conf.c           	Service Controller configuration API
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/PWM_config.c            	Pulse Width Modulation configuration
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/PWM_config.h            	Header of Pulse Width Modulation configuration 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/PWM_handlers.c          	Pulse Width Modulation handlers
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/PWM_handlers.h          	Header of Pulse Width Modulation handlers 
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/tl_conf.h               	Configuration file of the Transport layer
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/tl_if.c                 	Transport Layer interface
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/user_if.h               	Header file for User interface file
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/user_if.c               	User interface file
  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/App/vcp_conf.h              	Configuration of the vcp interface

  - BLE/BLE_Mesh_Model_Sensor/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver

  
@par Hardware and Software environment

    - This application runs on STM32WB5MM Discovery Kit devices
  
    - STM32WB5MM Discovery Kit board Set-up    
       - Connect the Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).

@par How to use it ? 

This application requests having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
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

The Server and Client projects both support Proxy, Relay and Friend features. 

@Demo setup based on STM32WB5MM-DK board

General default setting defined in mesh_cfg_usr.h:
        ENABLE_SENSOR_MODEL_SERVER                                    	     (1) /* VENDOR SERVER ONOFF MODEL ON FIRST ELEMENT */
        APPLICATION_NUMBER_OF_ELEMENTS                                         1 /* NUMBER OF ELEMENT SUPPORTED */ 
        APPLICATION_SIG_MODELS_MAX_COUNT                                      10 /* TOTAL NUMBER OF MODELS SUPPORTED */
        APPLICATION_VENDOR_MODELS_MAX_COUNT                                    1 /* TOTAL NIMBER OF VENDOR MODELS SUPPORTED */
        ENABLE_PB_ADV                                                            /* Provisioning Bearer over advertising channels support */
        ENABLE_PB_GATT                                                           /* Provisioning Bearer over GATT proxy support */ 

The following demo based on STM32WB5MM-DK has been set up using one board as a Thermometer Sensor and one or more boards as Temperature Indicators.

SENSOR SERVER NODE Setup:
Step 1. By default, the Project is Setup to support a Sensor Server Node. Ensure the ENABLE_SENSOR_MODEL_SERVER macro is enabled while ENABLE_SENSOR_MODEL_CLIENT is disabled in mesh_cfg_usr.h:
	/* Enable Sensor Model Server or Sensor Model Client */
	#define ENABLE_SENSOR_MODEL_SERVER                                           (1)

	//#define ENABLE_SENSOR_MODEL_CLIENT                                           (1)                                   

Step 2. Generate and flash binary in the first board.
		Once flashed, the Node Type (Sensor Server), and the board BD Address are displayed on the LCD screen.


SENSOR CLIENT NODE Setup:
Step 1. In mesh_cfg_usr.h,  disable the ENABLE_SENSOR_MODEL_SERVER macro and enable ENABLE_SENSOR_MODEL_CLIENT:
	/* Enable Sensor Model Server or Sensor Model Client */
	//#define ENABLE_SENSOR_MODEL_SERVER                                           (1)

	#define ENABLE_SENSOR_MODEL_CLIENT                                           (1)                                   

Step 2. Generate and flash binary in the second board.
	Once flashed, the Node Type (Sensor Client), and the board BD Address are displayed on the LCD screen.


Demo operation
Step 3. Connect an HyperTerminal to each board (e.g Tera Term):
		- Setup the Serial port as follow:
			Speed: 			115 200
			Data:  			8 bit
			Parity:			none
			Stop bits:		1 bits
			Flow Control: 	none
		- Select the Serial Port corresponding to your first board
		- Repeat the operation for the second board	
		Note: 	You can identify which board is connected to which HyperTerminal window by pushing reset button on one board. 
				The associated Terminal windows displays reset logs.

Step 4. Verify the boards are correctly unprovisioned, by keeping pressed User Button 1 (B1) and Reset button, 
        releasing the Reset button while keeping B1 pressed until the Blue LED is blinking.
		You can see "Unprovisioned device" message on the HyperTerminal logs of the board.

Step 5. Setup the BLE-mesh network using the ST BLE-Mesh Android (https://play.google.com/store/apps/details?id=com.st.bluenrgmesh&hl=en) or 
        iOS (https://apps.apple.com/us/app/st-ble-mesh/id1348645067) smartphone application.
		The following instructions are based on the Android version of the application, buttons and menus may differ in the iOS version. 
		In case of doubts or for further information, you can refer to:
			Android User Manual: UM2180 (https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/7e/bb/bd/40/66/0f/46/54/DM00369109/files/DM00369109.pdf/jcr:content/translations/en.DM00369109.pdf)
			iOS User Manual : UM2361 https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/7e/ea/19/73/31/3e/43/1a/DM00477323/files/DM00477323.pdf/jcr:content/translations/en.DM00477323.pdf
			
		- Launch the application and select the "device" interface in the top left corner.
		
		- Your devices should be visible in this interface, click on the (+) button next to one of the devices to begin the provisioning and configuration process of this device.
		  If your Unpvorisioned devices are not visible from this interface, try to reproduce the Step 4.
		
		- Select "Fast Provisioning" button. 
		  The Green LED of your board switches ON to indicate the board is currently used as Proxy by smartphone, this means the smartphone use this board to setup and communicate with the BLE mesh Network.
		
		- Near to "Element 1" click on the "Configure" button. 
		/!\ "Quick Configuration" option of the smartphone application encounter issue to configure Client model, that is the reason why configuration is done manually.
		
		- On the configuration menu you can see if the board you are provisioning is your Server or Client node:
			SENSOR MODEL SERVER indicate your board is the Server Node
			SENSOR MODEL CLIENT indicate your board is the Client Node
			
			a. 	Click on "Bind More Key" text.
				Then "Add New Key" and select the AppKey 0.	  
				In the pop-up, check the circle in front of "App Key 0", the AppKey binding is now done.
				From the binded keys menu, check the circle in front of "App Key 0" one more time.
		
			b. 	In the Subscription List, check "Default: C000" Checkbox 
			c.  In the Set Publish Address, check "Default: C000" Checkbox too
		
		- In the case of Server Node, click on "SENSOR MODEL SERVER" and select the "SENSOR SETUP SERVER" model.
		  Reproduce steps a., b. and c. steps for this model
		
		- Save your settings in the top right corner
		
		- Click on the Arrow in Top Left Corner to exit Configuration menu.
		  At this point your node must be correctly Provisioned ans Configured.
		  It should be visible from the Node interface (click on "Nodes" button in bottom menu).
		
		- You can rename your node by clicking on the setting button next to (P)(R)(F) icons.
		  Click on the Pen icon in Node Name line and rename your Node: 
			"Sensor Server"
			"Sensor Client"
		  Depending on your node.
		
		- Come back to the "Devices" interface and reproduce previous steps for your second board.

Step 6. Once the Client and Server Nodes are provisioned, you can close the smartphone application.
		The Green LED of your Proxy node should switch OFF as it is no longer connected to the Smartphone.

Step 7.	On Short Push on User Button 1 (B1) of the Sensor_Client Node: the Sensor_Client send a request to the Sensor_Server Node. The Client ask the Temperature to the Server.
		The Sensor_Server answer to this request, you can observe the reply is correctly processed in Sensor_Server HyperTerminal: "Sensor_Status - Reply sent"
		On Sensor_Client Hyperterminal, you can see the received Temperature value: "Appli_Sensor_Status - Temperature = 23 degrees"
		  
		On Long Push on User Button 1 (B1) of the Sensor_Client Node: the Sensor_Client send a request to the Sensor_Server Node. The Client ask the Time Of Flight measurement to the Server.
		The Sensor_Server answer to this request, you can observe the reply is correctly processed in Sensor_Server HyperTerminal: "Sensor_Status - Reply sent"
		On Sensor_Client Hyperterminal, you can see the received Distance value: "Appli_Sensor_Status - Distance = 161 cm"		  
		

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */