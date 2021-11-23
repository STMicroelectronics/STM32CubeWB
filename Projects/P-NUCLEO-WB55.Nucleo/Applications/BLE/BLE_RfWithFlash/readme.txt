/**
  @page BLE_RfWithFlash example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_RfWithFlash/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_RfWithFlash example
  ******************************************************************************
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to demonstrate the capability to erase/write the flash while a Point-to-Point communication using BLE component is active. 

This project is built on top of BLE_p2pServer where SW3 has been added to erase/write in flash and SW1 has been modified to enable/disable
timing protection versus flash activity from CPU1.
The flash driver implements the algorithm described in AN5289 - Building a Wireless application.
The BLE_RfWithFlash shall be downloaded in a STM32WB55xx board acting as a GATT server.
The client could be either a phone with the ST BLE Sensor application or the BLE_P2PClient application downloaded on a STM32WB55xx board.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, Dual core

@par Directory contents 
  
  - BLE/BLE_RfWithFlash/Core/Inc/stm32wbxx_hal_conf.h	        HAL configuration file
  - BLE/BLE_RfWithFlash/Core/Inc/stm32wbxx_it.h          		Interrupt handlers header file
  - BLE/BLE_RfWithFlash/Core/Inc/main.h                  		Header for main.c module
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/app_ble.h           		Header for app_ble.c module
  - BLE/BLE_RfWithFlash/Core/Inc/app_common.h            		Header for all modules with common definition
  - BLE/BLE_RfWithFlash/Core/Inc/app_conf.h              		Parameters configuration file of the application
  - BLE/BLE_RfWithFlash/Core/Inc/app_entry.h            		Parameters configuration file of the application
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/ble_conf.h            	BLE Services configuration
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/ble_dbg_conf.h        	BLE Traces configuration of the BLE services
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/p2p_server_app.h      	Header for p2p_server_app.c module
  - BLE/BLE_RfWithFlash/Core/Inc/hw_conf.h           			Configuration file of the HW
  - BLE/BLE_RfWithFlash/Core/Inc/utilities_conf.h    			Configuration file of the utilities
  - BLE/BLE_RfWithFlash/Core/Src/stm32wbxx_it.c          		Interrupt handlers
  - BLE/BLE_RfWithFlash/Core/Src/main.c                  		Main program
  - BLE/BLE_RfWithFlash/Core/Src/system_stm32wbxx.c      		stm32wbxx system source file
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/app_ble.c      			BLE Profile implementation
  - BLE/BLE_RfWithFlash/Core/Src/app_entry.c      				Initialization of the application
  - BLE/BLE_RfWithFlash/STM32_WPAN/App/p2p_server_app.c   		P2P Server application
  - BLE/BLE_RfWithFlash/STM32_WPAN/Target/hw_ipcc.c      		IPCC Driver
  - BLE/BLE_RfWithFlash/Core/Src/stm32_lpm_if.c					Low Power Manager Interface
  - BLE/BLE_RfWithFlash/Core/Src/hw_timerserver.c 				Timer Server based on RTC
  - BLE/BLE_RfWithFlash/Core/Src/hw_uart.c 						UART Driver
  - BLE/BLE_RfWithFlash/Core/Src/flash_driver.c 				Flash Driver (implements the algorithm from AN5289)
  - BLE/BLE_RfWithFlash/Core/Inc/flash_driver.h 				Interface to be used to erase/write in Flash
  
@par Hardware and Software environment

    - This application runs on STM32WB55xx devices.
    
    - Nucleo board (MB1355C) Set-up    
       - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load the image into Target memory
 - OR use the BLE_RfWithFlash_reference.hex from Binary directory

First demonstration
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en
    https://itunes.apple.com/fr/App/st-ble-profile/id1081331769?mt=8

 - Install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_RfWithFlash application
 - The Peripheral device (BLE_RfWithFlash) starts advertising (during 1 minute), the green led blinks for each advertising event.
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to the device
 - select the P2PSRVx in the device list and play with the Light and the SW1 button of the board
 - When pressing SW1 on the board, a notification is sent to the phone.
 - The blue led on the board can be switched ON/OFF from the phone.

  Specific implementation for the test flash procedure
  The descriptions above are taken from the BLE_p2pServer and are still valid as the BLE_RfWithFlash application is built on top of the
  BLE_p2pServer application.
  The BLE_RfWithFlash application is working in the same way than BLE_p2pServer application with additional features :
  - Each time SW3 is pressed, the application alternatively :
  	- Erase the last CFG_NBR_OF_FLASH_SECTOR_TO_PROCESS (configurable from app_conf.h). A message is sent over before
    the erase process is started and once it is finished. The application checks that all the sectors have been erased. If it is not
    the case, the RED LED is switched ON and a message is sent over the traces with the first the address that has not been properly erased.
    - Write the last CFG_NBR_OF_FLASH_SECTOR_TO_PROCESS (configurable from app_conf.h) with 64bits data starting from 0x00 and incremented
      by 1 until the end of the test.A message is sent over before the write process is started and once it is finished. The application checks
      that all the sectors have been written. If it is not the case, the RED LED is switched ON and a message is sent over the traces with the
      first the address that has not been properly written.
  - Each time SW1 is pressed, in addition to what is described above, the CPU1 timing protection is alternatively enabled/disabled. Each time
    SW1 is pressed, a message is sent over the traces to notify the protection status.
  - When SW3 is pressed while the CPU1 timing protection has been enabled with SW1, the CPU1 loops into the FlashOperationProc() until the CPU1
    timing protection is disabled. Over all the time, the RF link activity is maintained.

Second demonstration - The phone may be replaced by the BLE_p2pClient application downloaded on a STM32WB55xx board
 - BLE_RfWithFlash may be connected by BLE_p2pClient.
 - Once the code (BLE_RfWithFlash & BLE_p2pClient) is downloaded into the two STM32WB55xx boards and executed, the modules are initialized. 

 - The Peripheral device (BLE_RfWithFlash) starts advertising (during 1 minute), the green led blinks for each advertising event.
 - The Central device (BLE_p2pClient) starts scanning when pressing the User button (SW1) on the USB Dongle board. 
   - BLE_p2pClient blue led becomes on. 
   - Scan req takes about 5 seconds.
   - Make sure BLE_RfWithFlash advertises, if not press reset button or switch off/on to restart advertising.
 - Then, it automatically connects to the BLE_RfWithFlash. 
   - Blue led turns off and green led starts blinking as on the MB1355C. Connection is done.
 - When pressing SW1 on a board, the blue led toggles on the other one.
   - The SW1 button can be pressed independently on the GATT Client or on the GATT Server.
 - When the server is located on a MB1355C, the connection interval can be modified from 50ms to 1s and vice-versa using SW2. 
 - The green led on the 2 boards blinks for each advertising event, it means quickly when 50ms and slowly when 1s. 
 - Passing from 50ms to 1s is instantaneous, but from 1s to 50ms takes around 10 seconds.
 - The SW1 event, switch on/off blue led, depends on the connection Interval event. 
 - So the delay from SW1 action and blue led change is more or less fast.

   For more details refer to the Application Note: 
  AN5289 - Building a Wireless application
  
	
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
