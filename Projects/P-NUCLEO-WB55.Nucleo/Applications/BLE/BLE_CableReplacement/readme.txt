/**
  @page BLE_CableReplacement example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_CableReplacement/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Cable Replacement example.
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

How to use the Point-to-Point communication using BLE component. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - BLE/BLE_CableReplacement/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_CableReplacement/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_CableReplacement/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_CableReplacement/STM32_WPAN/App/app_ble.h           Header for app_ble.c module
  - BLE/BLE_CableReplacement/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_CableReplacement/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_CableReplacement/Core/Inc/app_entry.h               Parameters configuration file of the application
  - BLE/BLE_CableReplacement/STM32_WPAN/App/ble_conf.h          BLE Services configuration
  - BLE/BLE_CableReplacement/STM32_WPAN/App/ble_dbg_conf.h      BLE Traces configuration of the BLE services
  - BLE/BLE_CableReplacement/STM32_WPAN/App/crs_app.h           Header for crs_app.c module
  - BLE/BLE_CableReplacement/STM32_WPAN/App/crc_app.h           Header for crc_app.c module
  - BLE/BLE_CableReplacement/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_CableReplacement/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_CableReplacement/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_CableReplacement/Core/Src/main.c                  	Main program
  - BLE/BLE_CableReplacement/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_CableReplacement/STM32_WPAN/App/app_ble.c      	    BLE Profile implementation
  - BLE/BLE_CableReplacement/Core/Src/app_entry.c      		    Initialization of the application
  - BLE/BLE_CableReplacement/STM32_WPAN/App/crs_app.c      	    Cable Replacement Service application
  - BLE/BLE_CableReplacement/STM32_WPAN/App/crc_app.c      	    Cable Replacement Client application
  - BLE/BLE_CableReplacement/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_CableReplacement/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_CableReplacement/Core/Src/hw_timerserver.c 		    Timer Server based on RTC
  - BLE/BLE_CableReplacement/Core/Src/hw_uart.c 				UART Driver

     
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
 
Two STM32WB55xx boards are used one acting as GATT client, the other as GATT server.
Both boards need to be compiled with different code by changing the #definitions in app_conf.h
#define GATT_CLIENT         0/1    /* 1 = Device is GATT Client, 0 = Device is GATT Server */
If device is GATT client, it is also central (defined in ble_conf.h) 
If device is GATT server, it is also peripheral (defined in ble_conf.h)

Open VT100 Terminals on Client and Server side. Connected to ST Link COM port @115200.
Once the code is downloaded on the two STM32WB55xx boards and executed,
the modules are initialized. 
The Peripheral device (Cable Replacement Server) starts advertising,
The Central device (Cable Replacement Client) scans and automatically connects to the Cable Replacement Server.
Once connected, the Cable Replacement Client (GATT Client) starts to search the Cable Replacement Services & Characteristics.
The above phase takes about 5 seconds, after that period Reception and Transmission parts will appear in the Client and Server VT100 Terminals.
On each VT100 Terminal windows you can type the text you want to send and receive the text sent by the connected device.

To summarise:
  *1*  User write text on GATT Server VT100 Terminal
  *2*  Server  ----------- Notification packet ------>   Client
or
  *1*  User write text on GATT Client VT100 Terminal
  *2*  Client  ----------- Wtite Command packet ------>   Server
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 