/**
  @page BLE_Zigbee_Static example
  
  @verbatim
  ******************************************************************************
  * @file    BLE_Zigbee/BLE_Zigbee_Static/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using BLE and Zigbee protocols in Static 
  *          Concurrent mode.
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
  
@par Application Description

How to use BLE application and Zigbee application in static concurrent mode.

This application requires one STM32WB55xx-Nucleo board with Static Concurrent Mode Application.
Static means that application can perform alternatively BLE or Zigbee activities. 

The idea is to present a demo with a Zigbee Network (at least 2 Nucleo Boards
running Zigbee_OnOff_Server_Coord and Zigbee_OnOff_Client_Router application) and a BLE connection between 1 Nucleo
board running BLE_Zigbee_Static application with a smartphone running 
"ST BLE Sensor" Application (available on App Store and Google Play)
The STM32WB55xx-Nucleo board running BLE_Zigbee_Static application is capable 
of switching from BLE network to Zigbee network and vice-versa.

@par Keywords

Zigbee, BLE

@par Directory contents 

  
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55RG-Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.
    
  - On STM32WB55RG-Nucleo, the jumpers must be configured as described
    in this section. Starting from the top left position up to the bottom 
    right position, the jumpers on the Board must be set as follows:

     CN11:    GND         [OFF]
     JP4:     VDDRF       [ON]
     JP6:     VC0         [ON]
     JP2:     +3V3        [ON] 
     JP1:     USB_STL     [ON]   All others [OFF]
     CN12:    GND         [OFF]
     CN7:     <All>       [OFF]
     JP3:     VDD_MCU     [ON]
     JP5:     GND         [OFF]  All others [ON]
     CN10:    <All>       [OFF]


@par How to use it ? 

This application requests having the stm32wb5x_BLE_Zigbee_FFD_static_fw.bin 
(or the stm32wb5x_BLE_Zigbee_RFD_static_fw.bin) binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


Minimum requirements for the demo:
- 1 STM32WB55xx-Nucleo board in BLE Zigbee Static Concurrent mode 
  (This device will be referred as NODE1)
- 2 STM32WB55xx-Nucleo board or more for creating Zigbee Network 
  (see example Zigbee_OnOff_Server_Coord and Zigbee_OnOff_Client_Router for more details on how to use it)
- 1 Smartphone (Android/IOS) with "ST BLE Sensor" Phone Application (available on App Store and Google Play)
  Install the "ST BLE Sensor" application on the ios/android device
  https://play.google.com/store/apps/details?id=com.st.bluems
  https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

In order to make the program work, you must do the following: 
 - Connect 1 STM32WB55xx-Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
    
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.

**** START DEMO ****
By default, after a reset, the board will start in BLE mode.

The BLE example is to demonstrate Point-to-Point communication using BLE component. 
The Peripheral device (P2P Server) starts advertising,
ST BLE Sensor smart Phone Application) scans and can connect to the P2P Server 
by selecting = "P2P_SERVER1" entry.
Once connected, the application starts to search the P2P Services & Characteristics.
Led Button service, Led Characteristic and Button Characteristic are discovered at this stage 
(this service is ST specific).
Once the smartphone is connected, the user can interact with the board by pressing 
"LED1 CONTROL" field which toggles LED1 on the board.

**** SWITCHING TO ZIGBEE *****
This can be done in different ways:
 - By sending (WRITE) "0x0102" command to the board via BLE. (Switch to Thread button in the smartphone application)
 - By pressing SW2 Button on the STM32WB55xx-Nucleo board

**** ZIGBEE MODE ****
Then the board switch to Zigbee protocol and corresponding application.
The node will then attach the existing Zigbee network (at least 2 boards running 
Zigbee_OnOff_Server_Coord and Zigbee_OnOff_Client_Router application).

The concurrent node is now running Zigbee stack and on top of it the OnOff Client cluster can be used.
Press SW1 button to send OnOff Toggle request to the server (board running Zigbee_OnOff_Server_Coord).
The RED LED will toggle on the server side.

**** SWITCHING BACK TO BLE *****
This is done by pressing SW2 Button on the STM32WB55xx-Nucleo board.

The STM32WB55xx-Nucleo board will restart in BLE mode, and start advertising. 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 