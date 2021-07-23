/**
  @page BLE_p2pServer Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_p2pServer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_p2pServer application
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

@par Application Description

How to demonstrate Point-to-Point communication using BLE component (as GATT server). 

Two STM32WB15xx boards are used, one acting as GATT client, and one as GATT server.
For example, BLE_P2PClient application is downloaded in a Nucleo board (MB1641) and BLE_p2pServer_ota application in a Nucleo board (MB1641).
The client could be located in a phone also, using the ST BLE Sensor application instead of the MB1641 board. 

	
@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_p2pServer/Core/Inc/stm32wbxx_hal_conf.h	        HAL configuration file
  - BLE/BLE_p2pServer/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_p2pServer/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_p2pServer/STM32_WPAN/App/app_ble.h           	Header for app_ble.c module
  - BLE/BLE_p2pServer/Core/Inc/app_common.h            		Header for all modules with common definition
  - BLE/BLE_p2pServer/Core/Inc/app_conf.h              		Parameters configuration file of the application
  - BLE/BLE_p2pServer/Core/Inc/app_entry.h            		Parameters configuration file of the application
  - BLE/BLE_p2pServer/STM32_WPAN/App/ble_conf.h            	BLE Services configuration
  - BLE/BLE_p2pServer/STM32_WPAN/App/ble_dbg_conf.h        	BLE Traces configuration of the BLE services
  - BLE/BLE_p2pServer/STM32_WPAN/App/p2p_server_app.h      	Header for p2p_server_app.c module
  - BLE/BLE_p2pServer/Core/Inc/hw_conf.h           		    Configuration file of the HW
  - BLE/BLE_p2pServer/Core/Inc/utilities_conf.h    		    Configuration file of the utilities
  - BLE/BLE_p2pServer/Core/Src/stm32wbxx_it.c          		Interrupt handlers
  - BLE/BLE_p2pServer/Core/Src/main.c                  		Main program
  - BLE/BLE_p2pServer/Core/Src/system_stm32wbxx.c      		stm32wbxx system source file
  - BLE/BLE_p2pServer/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_p2pServer/Core/Src/app_entry.c      		    Initialization of the application
  - BLE/BLE_p2pServer/STM32_WPAN/App/p2p_server_app.c   	P2P Server application
  - BLE/BLE_p2pServer/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver
  - BLE/BLE_p2pServer/Core/Src/stm32_lpm_if.c			    Low Power Manager Interface
  - BLE/BLE_p2pServer/Core/Src/hw_timerserver.c 		    Timer Server based on RTC
  - BLE/BLE_p2pServer/Core/Src/hw_uart.c 		            UART Driver
  
@par Hardware and Software environment

  - This example runs on STM32WB15xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB15CC-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.
    
    - Nucleo board (MB1641) Set-up    
       - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.

@par How to use it ? 

This application requires having the stm32wb15_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load the image into Target memory
 - OR use the BLE_p2pServer_reference.hex from Binary directory
 - This must be done for BLE_p2pServer

First demonstration
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en
    https://itunes.apple.com/fr/App/st-ble-profile/id1081331769?mt=8

 - Install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_P2P_Server application
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to the device
 - select the P2PSRVx in the device list and play with the Light and the SW1 button of the board


Second demonstration
 - BLE_p2pServer may be connected by BLE_p2pClient.
 - Once the code (BLE_p2pServer & BLE_p2pClient) is downloaded into the two STM32WB15xx boards and executed, the modules are initialized. 

 - The Peripheral device (BLE_p2pServer) starts advertising (during 1 minute), the green led blinks for each advertising event.
 - The Central device (BLE_p2pClient) starts scanning when pressing the User button (SW1) 
   - BLE_p2pClient blue led becomes on. 
   - Scan req takes about 5 seconds.
   - Make sure BLE_p2pServer advertises, if not press reset button or switch off/on to restart advertising.
 - Then, it automatically connects to the BLE_p2pServer. 
   - Blue led turns off and green led starts blinking as on the MB1641. Connection is done.
 - When pressing SW1 on a board, the blue led toggles on the other one.
   - The SW1 button can be pressed independently on the GATT Client or on the GATT Server.
 - When the server is located on a MB1641, the connection interval can be modified from 50ms to 1s and vice-versa using SW2. 
 - The green led on the 2 boards blinks for each advertising event, it means quickly when 50ms and slowly when 1s. 
 - Passing from 50ms to 1s is instantaneous, but from 1s to 50ms takes around 10 seconds.
 - The SW1 event, switch on/off blue led, depends on the connection Interval event. 
   - So the delay from SW1 action and blue led change is more or less fast.

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
