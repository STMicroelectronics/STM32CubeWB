/**
  @page BLE_Peripheral_Lite Application

  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Peripheral_Lite/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE_Peripheral_Lite application
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

@par Application Description

How to communicate with simple BLE peripheral with minimum activated features.

This example is to demonstrate simple BLE peripheral operation with bidirectional communication with minimalistic CPU2 configuration flow
and Lite service support with minimum needed features activation (not including e.g. Tasks Sequencer, Timer Server, Low-Power Manager).

One STM32WB55xx board is used acting as GAP Peripheral and GATT Server.
Android/iOS BLE compatible device with recommended ST BLE Profile application (such as ST BLE Sensor application)
installed to be used as GAP Central and GATT Client.

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
  
  - BLE/BLE_Peripheral_Lite/Inc/stm32wbxx_hal_conf.h       HAL configuration file
  - BLE/BLE_Peripheral_Lite/Inc/stm32wbxx_it.h             Interrupt handlers header file
  - BLE/BLE_Peripheral_Lite/Inc/main.h                     Header for main.c module
  - BLE/BLE_Peripheral_Lite/Inc/gatt_service.h             Header for gatt_service.c module
  - BLE/BLE_Peripheral_Lite/Inc/app_common.h               Header for all modules with common definition
  - BLE/BLE_Peripheral_Lite/Inc/app_conf.h                 Parameters configuration file of the application
  - BLE/BLE_Peripheral_Lite/Inc/hw_conf.h                  Configuration file of the HW
  - BLE/BLE_Peripheral_Lite/Inc/utilities_conf.h           Configuration file of the utilities 
  - BLE/BLE_Peripheral_Lite/Src/stm32wbxx_it.c             Interrupt handlers
  - BLE/BLE_Peripheral_Lite/Src/main.c                     Main program and BLE Profile implementation
  - BLE/BLE_Peripheral_Lite/Src/system_stm32wbxx.c         stm32wbxx system source file
  - BLE/BLE_Peripheral_Lite/Src/gatt_service.c             custom STM my very own service implementation
  - BLE/BLE_Peripheral_Lite/Src/hw_ipcc.c                  IPCC Driver
  - BLE/BLE_Peripheral_Lite/Src/app_debug.c                CPU2 debug and trace support
 
@par Hardware and Software environment

  - This application runs on STM32WB55xx devices & Nucleo board (MB1355C)  
 
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
 - OR use the BLE_Peripheral_Lite_reference.hex from Binary directory
 - This must be done for BLE_Peripheral_Lite (MB1355C) 

First demonstration
On the android/ios device, enable the Bluetooth communications, and if not done before,
 - Install the ST BLE Sensor application on the ios/android device
      https://play.google.com/store/apps/details?id=com.st.bluems
      https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

 - Power on the Nucleo board with the BLE_Peripheral_Lite application
 - Then, click on the App icon, ST BLE Sensor (android device)
 - connect to the device
 - select the "HELLO!" device(containing My Very Own service as P2PServer profile) in the list and play with the button to switch on/off blue LED on nucleo board.

Demonstration scheduling

 - Once the code (BLE_Peripheral_Lite) is downloaded into the STM32WB55xx board and executed, the module is initialized. 
 - The red LED switch On to indicate that the CPU2 is initializing and switch Off when CPU2 is initialized.
 - The green LED switch On to indicate that the wireless stack FW is running.
 - The blue LED switch On to indicate that the wireless stack FW has been initialized.
   and blue LED switch OFF when custom STM GATT service called "My very own service" has been initialized.
   
 - The Peripheral device (BLE_Peripheral_Lite) starts advertising with data using local name "HELLO!".
 - The green led blinks for each advertising event slowly each ~0.5sec.
 - The Central device (ST BLE Sensor smart phone application) starts scanning and when the peripheral is found and selected in advices list,
   it connects to the Server device Nucleo board (MB1355C) and the green led toggles faster. 
 - The button event on ST BLE Sensor application interface enables to switch On/Off blue led on Nucleo board.
 - Notification is sent from the server (Peripheral device - Nucleo board (MB1355C)) to the client (Central device - Smart phone) every ~1sec.
 - When the Peripheral device (Nucleo board (MB1355C)) is disconnected, advertising is restarted and it is possible to connect to it again.
 
 For more details refer to the Application Note: 
  AN5289 - Building a Wireless application  
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
