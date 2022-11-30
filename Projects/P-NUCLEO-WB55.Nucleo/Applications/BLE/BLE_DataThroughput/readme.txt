/**
  @page BLE_DataThroughput application 
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_DataThroughput/readme.txt 
  * @author  MCD Application Team
  * @brief   Data transfer via notification from server to client with a timer that enables to calculate the throughput at application level.
  ******************************************************************************
  *
  * Copyright (c) 2019-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par application Description

How to use data throughput via notification from server to client using BLE component.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_DataThroughput/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_DataThroughput/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_DataThroughput/Core/Inc/app_debug.h               Header for app_debug.c module
  - BLE/BLE_DataThroughput/Core/Inc/app_entry.h              	Parameters configuration file of the application
  - BLE/BLE_DataThroughput/Core/Inc/hw_conf.h                 Configuration file of the HW
  - BLE/BLE_DataThroughput/Core/Inc/hw_if.h                   Hardware Interface
  - BLE/BLE_DataThroughput/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_DataThroughput/Core/Inc/stm32wbxx_hal_conf.h		  HAL configuration file
  - BLE/BLE_DataThroughput/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_DataThroughput/Core/Inc/stm32_lpm_if.h            Header for stm32_lpm_if.c module (device specific LP management)
  - BLE/BLE_DataThroughput/Core/Inc/utilities_conf.h    		  Configuration file of the utilities
  - BLE/BLE_DataThroughput/STM32_WPAN/App/app_ble.h           Header for app_ble.c module
  - BLE/BLE_DataThroughput/STM32_WPAN/App/ble_conf.h          BLE Services configuration
  - BLE/BLE_DataThroughput/STM32_WPAN/App/ble_dbg_conf.h      BLE Traces configuration of the BLE services
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dts.h			          Header for Data Throughput Service implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_client_app.h     Header for Data Throughput Client Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_server_app.h     Header for Data Throughput Server Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/tl_dbg_conf.h        Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_DataThroughput/Core/Src/app_debug.c               Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_DataThroughput/Core/Src/app_entry.c      			    Initialization of the application
  - BLE/BLE_DataThroughput/Core/Src/hw_timerserver.c 			    Timer Server based on RTC
  - BLE/BLE_DataThroughput/Core/Src/hw_uart.c 					      UART Driver
  - BLE/BLE_DataThroughput/Core/Src/main.c                  	Main program
  - BLE/BLE_DataThroughput/Core/Src/stm32wbxx_hal_msp.c       This file provides code for the MSP Initialization and de-Initialization
  - BLE/BLE_DataThroughput/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_DataThroughput/Core/Src/stm32_lpm_if.c				    Low Power Manager Interface
  - BLE/BLE_DataThroughput/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_DataThroughput/STM32_WPAN/App/app_ble.c      		  BLE Profile implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dts.c			          Data Throughput Service implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_client_app.c     Data Throughput Client Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_server_app.c     Data Throughput Server Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
     
@par Hardware and Software environment

  - This application runs on STM32WB55xx devices.
  
  - This application has been tested with an STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device 
    and development board.

  - This application is by default configured to support low power mode ( No traces - No debugger )
    This can be modified in app_conf.h (CFG_LPM_SUPPORTED, CFG_DEBUG_BLE_TRACE, CFG_DEBUG_APP_TRACE)

  - SMPS can be used to obtain the best performance in power consumption.
    This feature depends on board components and its activation can be done through specific define (CFG_USE_SMPS)
    configurable in app_conf.h
    See AN5246 - Usage of STM32WB Series microcontrollers.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file
 
Two P-NUCLEO-WB55 boards are used, one central and one peripheral. 
There is a board configured as GATT client and the other as GATT server.
Only the GATT server board support a Data transfer service with a transmission characteristic that supports notification.
The GATT client board support the Data transfer by writing without response to the GATT server board Reception characteristic.
Both boards need to be compiled by changing the definition in app_conf.h
Use #define GATT_CLIENT 0 or 1
When the board is defined as GATT_CLIENT 1 the board is also defined as GAP central.
When the board is defined as GATT_CLIENT 0 (GATT server) the board is also defined as GAP peripheral.
Open a VT100 terminal on Central and Peripheral side (ST Link Com Port, @115200 bauds).
At startup the connection is established and encryption started(if #define ENCRYPTION_ON 1 uncomment in app_conf.h).
 - The peripheral device starts advertising.
 - The central device scans and automatically connects to the peripheral (use of CFG_DEV_ID_PERIPH_SERVER). 
 - After pairing and authentication (if enabled),  
   the client starts to search the data transfer service and characteristic.
 - the client enables the notification of the transmission characteristic.
On server side, the notification is started when the SW1 button is pushed (blue LED is ON), 
it stops when SW1 is pushed again (blue LED is OFF).
The Data Transfer with write without response can be started and stopped from SW1 GATT Client board (indicated by blue LED).
SW2 changes the PHY (1M or 2M). 
SW3 (long push) clears the security database (aci_gap_clear_security_db command)
On server: 
SW3 (short push) sends the slave pairing request (aci_gap_slave_security_req command)
On client:
SW3 (short push) sends the pairing request (aci_gap_send_pairing_req command)
On the client terminal receiving the current notification, the number of bytes per second is displayed.
On the server terminal receiving the current write without response, the number of bytes per second is displayed.
One P-NUCLEO-WB55 board is used as GATT Server and GAP peripheral.
Use #define GATT_CLIENT 0 (in app_conf.h)
This board can be connected with a smartphone supporting ST BLE Toolbox application.
Start a scan and choose DT_SERVER board to connect.
SW1 starts the notification data.
SW2 changes the PHY (1M or 2M). 
SW3 (short push) sends the slave pairing request (aci_gap_slave_security_req command)
SW3 (long push) clears the security database (aci_gap_clear_security_db command)  

In app_conf.h 
if #define CFG_TX_PHY    2 and #define CFG_RX_PHY    2, link is set to 2M
if #define CFG_TX_PHY    1 and #define CFG_RX_PHY    1, link stays at 1M
 
Available Wiki page:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview


For more details refer to the Application Note: 
  AN5289 - Building a Wireless application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
