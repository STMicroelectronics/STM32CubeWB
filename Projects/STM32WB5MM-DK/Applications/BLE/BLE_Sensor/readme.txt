/**
  @page BLE_Sensor Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Sensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_Sensor application
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
  @endverbatim

@par Application Description

This example is to demonstrate capabilities of STM32WB5MM Discovery Kit with the use of board sensors.

Several demonstrations are available in this project : Environmental, Plot Data, LED Control, RSSI, Debug Console.


@par Keywords

Connectivity, BLE, Sensors, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE profile, Dual core

@par Directory contents 
  
  - BLE/BLE_Sensor/Core/Inc/app_common.h               Header for all modules with common definition
  - BLE/BLE_Sensor/Core/Inc/app_conf.h                 Parameters configuration file of the application
  - BLE/BLE_Sensor/Core/Inc/app_debug.h                Header for app_debug.c module
  - BLE/BLE_Sensor/Core/Inc/app_entry.h                Parameters configuration file of the application
  - BLE/BLE_Sensor/Core/Inc/app_vl53l0x.h              Header for app_vl53l0x.c module
  - BLE/BLE_Sensor/Core/Inc/hw_conf.h                  Configuration file of the HW
  - BLE/BLE_Sensor/Core/Inc/hw_if.h                    HW interface
  - BLE/BLE_Sensor/Core/Inc/main.h                     Header for main.c module
  - BLE/BLE_Sensor/Core/Src/s25fl128s_conf.h           Configuration of the S25FL128S QSPI memory
  - BLE/BLE_Sensor/Core/Inc/stm32wb5mm_dk_conf.h       Configuration file of the Discovery board
  - BLE/BLE_Sensor/Core/Inc/stm32wbxx_hal_conf.h       HAL configuration file
  - BLE/BLE_Sensor/Core/Inc/stm32wbxx_it.h             Interrupt handlers header file
  - BLE/BLE_Sensor/Core/Inc/stm32_lpm_if.h             Header for stm32_lpm_if.c module (LP management)
  - BLE/BLE_Sensor/Core/Inc/utilities_conf.h           Configuration file of the utilities
  - BLE/BLE_Sensor/STM32_WPAN/App/app_ble.h            Header for app_ble.c module
  - BLE/BLE_Sensor/STM32_WPAN/App/ble_conf.h           BLE Services configuration
  - BLE/BLE_Sensor/STM32_WPAN/App/ble_dbg_conf.h       BLE Traces configuration of the BLE services
  - BLE/BLE_Sensor/STM32_WPAN/App/env_server_app.h     Header for env_server_app.c
  - BLE/BLE_Sensor/STM32_WPAN/App/motenv_server_app.h  Header for motenv_server_app.c
  - BLE/BLE_Sensor/STM32_WPAN/App/motion_server_app.h  Header for motion_server_app.c 
  - BLE/BLE_Sensor/STM32_WPAN/App/p2p_server_app.h     Header for p2p_server_app.c module            Header for tm.c module
  - BLE/BLE_Sensor/STM32_WPAN/App/tl_dbg_conf.h        Debug configuration file for stm32wpan transport layer interface
  - BLE/BLE_Sensor/STM32_WPAN/App/wb5m_sensor_stm.h    MOTENV Service (Custom STM)
  - BLE/BLE_Sensor/Core/Src/vl53l0x/                   Proximity sensor files
  - BLE/BLE_Sensor/Core/Src/app_debug.c                Debug capabilities source file for STM32WPAN Middleware
  - BLE/BLE_Sensor/Core/Src/app_entry.c                Initialization of the application
  - BLE/BLE_Sensor/Core/Src/app_vl53l0x.c              Proximity Application file
  - BLE/BLE_Sensor/Core/Src/hw_timerserver.c           Timer Server based on RTC 
  - BLE/BLE_Sensor/Core/Src/hw_uart.c                  UART Driver
  - BLE/BLE_Sensor/Core/Src/main.c                     Main program
  - BLE/BLE_Sensor/Core/Src/stm32wbxx_hal_msp.c        MSP Initialization and de-Initialization code
  - BLE/BLE_Sensor/Core/Src/stm32wbxx_it.c             Interrupt handlers
  - BLE/BLE_Sensor/Core/Src/stm32_lpm_if.c             Low Power Manager Interface
  - BLE/BLE_Sensor/Core/Src/system_stm32wbxx.c         stm32wbxx system source file  
  - BLE/BLE_Sensor/STM32_WPAN/App/app_ble.c            BLE Profile implementation
  - BLE/BLE_Sensor/STM32_WPAN/App/env_server_app.c     Handle HW/Environmental Service/Char
  - BLE/BLE_Sensor/STM32_WPAN/App/motenv_server_app.c  MOTENV Server Application
  - BLE/BLE_Sensor/STM32_WPAN/App/motion_server_app.c  Handle HW/Motion (Acc/Gyro/Mag) Service/Char
  - BLE/BLE_Sensor/STM32_WPAN/App/p2p_server_app.c     P2P Server application
  - BLE/BLE_Sensor/STM32_WPAN/App/wb5m_sensor_stm.c    MOTENV Service (Custom STM)
  - BLE/BLE_Sensor/STM32_WPAN/Target/hw_ipcc.c         IPCC Driver

  
@par Hardware and Software environment

    - This application runs on STM32WB5MM Discovery Kit devices
  
    - STM32WB5MM Discovery Kit board Set-up    
       - Connect the Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK).

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup
   
In order to make the program work, you must do the following :
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file

First demonstration
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor application on the ios/android device
    https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Sensor

 - Power on the board with the BLE_Sensor application
 - At powered on, the screen displays the name of the board (STM32WB5MM), the stack version, the fuse version and the BD address during 5 seconds
 - Then, the board starts advertising (during 2 minute). During this phase, the distance measured by the Time-of-Flight sensor is displayed on the screen.
 - To connect the board to the smartphone, click on the App icon, ST BLE Sensor (android device)
 - connect to the device
 - select the STM32WBDK in the device list 
 - Then we have some demonstrations available :
    - environmental : Display humidity and temperature values
    - plot data : Plot data exported by the device (accelerometer, gyroscope, humidity, temperature)
    - LED Control : Switch on/off the LED by clicking on the led icon and receive alarm when SW1 is pushed
    - RSSI : Shows RSSI
    - Debug console : Serial console able to send commands to and receive messages from the board

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_HeartRate

For more details refer to the Application Note: 
  AN5289 - Building a Wireless application  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
