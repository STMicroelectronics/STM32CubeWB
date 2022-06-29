/**
  @page BLE_AT_Server Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_AT_Server/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_AT_Server Application.
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

@par Application Description

How to demonstrate Point-to-Point communication using BLE component (as GATT server).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
       
The STM32WB Series microcontroller can be controlled:
  Either through a terminal emulator such as Tera Term or Termite:
  one STM32WB55xx board is used, acting as GATT server.
  BLE_AT_Server application is downloaded in a Nucleo board (MB1355C).
  
  Either through an embedded microcontroller running AT client application like:
  one B-U585I-IOT02A board (STM32U5 connected via LPUART to STM32WB module)
  BLE_AT_Server application is downloaded in STM32WB5M module.
  
The client could be located in a smartphone also, using ST BLE Sensor or ST BLE ToolBox applications. 

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, LPUART, PWR, BLE protocol, BLE pairing, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_AT_Server/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_AT_Server/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_AT_Server/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_AT_Server/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_AT_Server/Core/Inc/hw_if.h                       Configuration file of the Hardware Interface
  - BLE/BLE_AT_Server/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_AT_Server/Core/Inc/stm32_lpm_if.h                Low layer function to enter/exit low power modes
  - BLE/BLE_AT_Server/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_AT_Server/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_AT_Server/Core/Src/main.c                        Main program
  - BLE/BLE_AT_Server/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_AT_Server/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_AT_Server/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_AT_Server/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_AT_Server/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_AT_Server/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_AT_Server/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_AT_Server/Core/Src/app_debug.c                   Interface to support debug in the application
  - BLE/BLE_AT_Server/Core/Inc/app_debug.h                   Header of application debug
  - BLE/BLE_AT_Server/Core/Src/ble_at_server.c               AT server application
  - BLE/BLE_AT_Server/Core/Inc/ble_at_server.h               Header for ble_at_server.c module
  - BLE/BLE_AT_Server/Core/Src/ble_at_server_cb.c            AT server callback functions
  - BLE/BLE_AT_Server/Core/Inc/ble_at_server_cb.h            Header for ble_at_server_cb.c module
  - BLE/BLE_AT_Server/Core/Src/stm32wbxx_hal_msp.c           MSP Initialization and deinitialisation
  - BLE/BLE_AT_Server/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_AT_Server/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_AT_Server/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface.
  - BLE/BLE_AT_Server/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_AT_Server/STM32_WPAN/App/app_ble.h               Header for app_ble.c module
  - BLE/BLE_AT_Server/STM32_WPAN/App/custom_app.c            Custom service application
  - BLE/BLE_AT_Server/STM32_WPAN/App/custom_app.h            Header for custom_app.c module
  - BLE/BLE_AT_Server/STM32_WPAN/App/custom.c                Custom service
  - BLE/BLE_AT_Server/STM32_WPAN/App/custom.h                Header for custom.c module
  - BLE/BLE_AT_Server/STM32_WPAN/App/dis_app.c               Device Information Service application
  - BLE/BLE_AT_Server/STM32_WPAN/App/dis_app.h               Header for dis_app.c module
  - BLE/BLE_AT_Server/STM32_WPAN/App/hrs_app.c               Heart Rate Service application
  - BLE/BLE_AT_Server/STM32_WPAN/App/hrs_app.h               Header for hrs_app.c module
  - BLE/BLE_AT_Server/STM32_WPAN/App/p2p_server_app.c        P2P Server application
  - BLE/BLE_AT_Server/STM32_WPAN/App/p2p_server_app.h        Header for p2p_server_app.c module
  - BLE/BLE_AT_Server/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

@par Hardware and Software environment

  - This application runs on STM32WB55xx and STM32WB5MM module devices.
    
  - Commands are received over LPUART interface with 9600 8N1 configuration. 
    On a P-NUCLEO-WB55.Nucleo board use PA2 (CN10.35) LPUART TX and PA3 (CN10.37) LPUART RX.
      
  - USB to UART cable can be used to send AT commands from a PC.

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
One of the following demonstrations can be performed.

First demonstration using predefined applications
 On the android/ios smartphone, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor and/or ST BLE Toolbox applications:
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Sensor
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox


 - Power on the Nucleo board running BLE_AT_Server application.
 - Open a terminal emulator to send data to STM32WB LPUART interface( @9600 bauds 8N1, append CR LF) 
 - Send AT, OK must be received
 - Send AT+BLE_SVC=1, creates P2P_server service and starts advertising. OK is received
 - Then, open ST BLE Sensor (smartphone) and connect to P2PSRV1 device.
 - Events are received over LPUART : BLE_EVT_CONN=1, BLE_EVT_UPD_CONN=1
 - Play with the Light on the smartphone, events are received over LPUART : BLE_EVT_WRITE=1,2,x
 - Send over the LPUART AT+BLE_NOTIF_VAL=1,2,1 – toggles button characteristic status, (AT+BLE_NOTIF_VAL=1,2,0 has no effect)
 - Disconnect on the smartphone, an event is received : BLE_EVT_CONN=0 
 - Send over LPUART AT+BLE_SVC=2, creates Heart Rate service and starts advertising. OK is received
 - Connect to the device HRSTM: events received BLE_EVT_CONN=1, BLE_EVT_UPD_CONN=1 
 - Send AT+BLE_NOTIF_VAL=2,1,70 – sets HeartRate characteristic value to 70
 - Disconnect the smartphone: event received BLE_EVT_CONN=0

Second demonstration consists in creating a custom service
 - Install the ST BLE Sensor and/or ST BLE Toolbox applications:
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Sensor
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox

 - Power on your board running BLE_AT_Server application.
 - Open a terminal emulator to send data to STM32WB LPUART interface( @9600 bauds 8N1, append CR LF) 
 - Send AT, OK must be received
 - Send AT+BLE_INIT=1, initialization
 - Send AT+BLE_SVC=3,2,0xaabb,10 creation of a custom service
 - Send AT+BLE_CHAR_ADD=3,1,1,0x3344,2,26,0,1 creation of a characteristic with properties: read, write, notify
 - Send AT+BLE_CHAR_ADD=3,2,1,0x5566,2,40,0,1 creation of a second characteristic with properties: write, indicate
 - Send AT+BLE_ADV=1, starts advertising
 - Then, open ST BLE toolbox (smartphone) and connect to AT_SERVER device, discover service and characteristics.
 - Send from the smartphone on characteristic 1: write 0x1122, event received on terminal: BLE_EVT_WRITE=3,1,0x1122 (service 3, characteristic 1)
 - Send from the smartphone on characteristic 2:a write 0x3344, event received on terminal: BLE_EVT_WRITE=3,2,0x3344 (service 3, characteristic 2)
 - From the smartphone, enable notification and indication
 - Send AT+BLE_NOTIF_VAL=3,1,1, notification received on the smartphone
 - Send AT+BLE_INDIC_VAL=3,2,1, indication received on the smartphone

For more details refer to the Wiki page: 
  https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_AT_Server  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
