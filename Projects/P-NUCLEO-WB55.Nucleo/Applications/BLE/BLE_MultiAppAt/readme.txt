/**
  @page BLE_MultiAppAt Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_MultiAppAt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ble_multi_app_at application
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

How to use multi BLE applications using a network processor architecture.
  

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, Dual core

@par Directory contents 
  
  - BLE/BLE_MultiAppAt/Core/Inc/stm32wbxx_hal_conf.h	        	HAL configuration file
  - BLE/BLE_MultiAppAt/Core/Inc/stm32wbxx_it.h          					Interrupt handlers header file
  - BLE/BLE_MultiAppAt/Core/Inc/main.h                  						Header for main.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_sv.h           		Header for app_ble_sv.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_cl.h           		Header for app_ble_cl.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_hr.h           		Header for app_ble_hr.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_common.h      Header for app_ble_common.c module
  - BLE/BLE_MultiAppAt/Core/Inc/app_common.h            				Header for all modules with common definition
  - BLE/BLE_MultiAppAt/Core/Inc/app_conf.h              						Parameters configuration file of the application
  - BLE/BLE_MultiAppAt/Core/Inc/app_entry.h            						Parameters configuration file of the application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/ble_conf.h            		BLE Services configuration
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/ble_dbg_conf.h        		BLE Traces configuration of the BLE services
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/p2p_server_app.h      	Header for p2p_server_app.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/p2p_client_app.h      	Header for p2p_client_app.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/hrs_app.h      				Header for hrs_app.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/dis_app.h      				Header for dis_app.c module
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/uart_app.h      				Header for uart_app.c module
  - BLE/BLE_MultiAppAt/Core/Inc/hw_conf.h           							Configuration file of the HW
  - BLE/BLE_MultiAppAt/Core/Inc/utilities_conf.h    							Configuration file of the utilities
  - BLE/BLE_MultiAppAt/Core/Src/stm32wbxx_it.c          					Interrupt handlers
  - BLE/BLE_MultiAppAt/Core/Src/main.c                  						Main program
  - BLE/BLE_MultiAppAt/Core/Src/system_stm32wbxx.c      			stm32wbxx system source file
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_sv.c      			AT_P2PServer BLE Profile implementation
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_cl.c      			AT_P2PClient BLE Profile implementation
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_hr.c      			AT_HeartRate BLE Profile implementation
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/app_ble_common.c      Common functions of the BLE Profile implementations
  - BLE/BLE_MultiAppAt/Core/Src/app_entry.c      							Initialization of the application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/p2p_server_app.c   		P2P Server application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/p2p_client_app.c   		P2P Client application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/hrs_app.c   					Heart Rate application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/dis_app.c   					Device Information application
  - BLE/BLE_MultiAppAt/STM32_WPAN/App/uart_app.c   					UART application
  - BLE/BLE_MultiAppAt/STM32_WPAN/Target/hw_ipcc.c      			IPCC Driver
  - BLE/BLE_MultiAppAt/Core/Src/stm32_lpm_if.c								Low Power Manager Interface
  - BLE/BLE_MultiAppAt/Core/Src/hw_timerserver.c 						Timer Server based on RTC
  - BLE/BLE_MultiAppAt/Core/Src/hw_uart.c 									UART Driver
  
@par Hardware and Software environment

    - This application runs on STM32WB55xx device, Nucleo board (MB1355C)
    
    - Nucleo board (MB1355C) Set-up    
       - Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).
       - Please ensure that the ST-LINK connectors and jumpers are fitted.
       - Connect an USB-UART converter from PC to STM32WB LPUART pins. (PA2(CN10.35) = LPUART1_TX ; PA3(CN10.37) = LPUART1_RX)

One STM32WB55xx and one computer running a Terminal like TeraTerm are used. The computer is the orders giver and the STM32WB55xx is the slave that listens to the orders and performs them.
The STM32WB55xx can either run the AT_P2PServer application, the AT_P2PClient application or the AT_HeartRate application.
To manipulate each application, you can refer to the BLE_p2pServer, BLE_p2pClient and BLE_HeartRate documentation for more details.

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

First demonstration
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Profile application on the android device
	https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation&hl=en
    https://itunes.apple.com/fr/App/st-ble-profile/id1081331769?mt=8

 - Install the ST BLE Sensor application on the ios/android device
	https://play.google.com/store/apps/details?id=com.st.bluems
	https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_MultiAppAt application
 - Open your favorite Terminal software (e.g. TeraTerm) and connect to the correct serial port with "Local echo" activated and a BaudRate of 9600 bauds. The other settings should stay by default.
 - Then, type "AT\r", you should receive "OK", meaning that the application should be operational.
 - Type "AT+SV\r", you should receive "SV OK". You are now in AT P2P Server mode.
 - Type "AT+SV$ADV_START\r". You should receive A and see a green led blinking rapidly on the board.
 - Then, with your phone, click on the App icon, ST BLE Sensor (android device)
 - select the ATP2PSV in the device list, connect to the device and play with the App as well as the terminal, using commands described in the Application Note
   as the instruction "AT+SV$NOTIFY=XXXX\r" with XXXX input parameter being 2 bytes hex values in range [0-9]/[A-F].


Second demonstration
 - Power on the Nucleo board with the BLE_MultiAppAt application
 - Open your favorite Terminal software (e.g. TeraTerm) and connect to the correct serial port with "Local echo" activated and a BaudRate of 9600 bauds. The other settings should stay by default.
 - Then, type "AT\r", you should receive "OK", meaning that the application should be operational.
 - Type "AT+HR\r", you should receive "HR OK". You are now in AT Heart Rate mode.
 - Then, with your phone, click on the App icon, nRF Connect (android device)
 - connect to the device 
 - select the STATHR in the device list, connect to it and play with the App as well as the terminal, using commands described in the Application Note.
 
Third demonstration
	 - Power on two Nucleo boards with the BLE_MultiAppAt application
	 For AT P2P Server
		 - Open your favorite Terminal software (e.g. TeraTerm) and connect to the correct serial port with "Local echo" activated and a BaudRate of 9600 bauds. The other settings should stay by default.
		 - Then, type "AT\r", you should receive "OK", meaning that the application should be operational.
		 - Type "AT+SV\r", you should receive "SV OK". You are now in AT P2P Server mode.
		 - Type "AT+SV$ADV_START\r". You should receive A and see a green led blinking rapidly on the board.
		 - After receiving the character "C", you can start playing with the terminal, using commands described in the Application Note.
	For AT P2P Client
		 - Open your favorite Terminal software (e.g. TeraTerm) and connect to the correct serial port with "Local echo" activated and a BaudRate of 9600 bauds. The other settings should stay by default.
		 - Then, type "AT\r", you should receive "OK", meaning that the application should be operational.
		 - Type "AT+CL\r", you should receive "CL OK". You are now in AT P2P Server mode.
		 - Type "AT+CL$SCAN\r". You should receive the BD Address of the AT P2P Server currently advertising.
		 - Then, type "AT+CL$CONN=XXXXXXXXXXXX\r" with the correct BD Address. You should see C displayed on both terminals. The Client and Server are now connected.
		 - Play with the terminal, using commands described in the Application Note.

   For more details refer to the Application Note: 
  AN5379 - Examples of AT commands on STM32WB 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
