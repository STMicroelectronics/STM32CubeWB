/**
  @page BLE_DataThroughput 
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_DataThroughput/readme.txt 
  * @author  MCD Application Team
  * @brief   Data transfer via notification from server to client with a timer that enables to calculate the throughput at application level.
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
  
  - BLE/BLE_DataThroughput/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_DataThroughput/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_DataThroughput/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_DataThroughput/STM32_WPAN/App/app_ble.h             Header for app_ble.c module
  - BLE/BLE_DataThroughput/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_DataThroughput/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_DataThroughput/Core/Inc/app_entry.h              	Parameters configuration file of the application
  - BLE/BLE_DataThroughput/STM32_WPAN/App/ble_conf.h            BLE Services configuration
  - BLE/BLE_DataThroughput/STM32_WPAN/App/ble_dbg_conf.h        BLE Traces configuration of the BLE services
  - BLE/BLE_DataThroughput/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_DataThroughput/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_client_app.h     	Header for Data Throughput Client Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_server_app.h       Header for Data Throughput Server Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dts.h			        Header for Data Throughput Service implementation
  - BLE/BLE_DataThroughput/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_DataThroughput/Core/Src/main.c                  	Main program
  - BLE/BLE_DataThroughput/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_DataThroughput/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_DataThroughput/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_DataThroughput/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_DataThroughput/Core/Src/stm32_lpm_if.c				Low Power Manager Interface
  - BLE/BLE_DataThroughput/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_DataThroughput/Core/Src/hw_uart.c 					UART Driver
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_client_app.c     	Data Throughput Client Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dt_server_app.c       Data Throughput Server Application implementation
  - BLE/BLE_DataThroughput/STM32_WPAN/App/dts.c			        Data Throughput Service implementation

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55xx-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 
Two STM32WB55xx boards are used, one central and one peripheral. 
They are both configured as GATT client and GATT server.
They both support a Data transfer service with a transmission characteristic that supports notification.
Both boards need to be compiled by changing the definition in app_conf.h
Use #define CFG_BLE_CENTRAL 0 or 1 and CFG_SERVER_ONLY 0
One board is defined as GAP peripheral, the other board is defined as GAP central.
Open a VT100 terminal on Central and Peripheral side (ST Link Com Port, @115200 bauds).
At startup the connection is established and encryption started(if #define ENCRYPTION_ON 1 uncomment in app_conf.h).
 - The peripheral device starts advertising.
 - The central device scans and automatically connects to the peripheral (use of CFG_DEV_ID_PERIPH_SERVER). 
 - After pairing and authentication (if enabled),  
   the client on each device starts to search the data transfer service and characteristic.
 - Each client enables the notification of the transmission characteristic.
On server side, the notification is started when the SW1 button is pushed (blue led is ON), 
it stops when SW1 is pushed again (blue led is OFF).
The notification can be started and stopped from both sides.
On the client terminal receiving the current notification, the number of bytes per second is displayed.

One STM32WB55xx board is used as peripheral and server only.
Use #define CFG_BLE_CENTRAL 0 and CFG_SERVER_ONLY 1 (in app_conf.h)
This board can be connected with a smartphone supporting ST BLE SENSOR application.
Start a scan and choose DT_SERVER board to connect.
SW1 starts the notification data.
SW2 changes the PHY (1M or 2M). 
SW3 (short push) sends the slave pairing request (aci_gap_slave_security_req command)
SW3 (long push) clears the security database (aci_gap_clear_security_db command)  

In app_conf.h 
if #define CFG_TX_PHY    2 and #define CFG_RX_PHY    2, link is set to 2M
if #define CFG_TX_PHY    1 and #define CFG_RX_PHY    1, link stays at 1M
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 