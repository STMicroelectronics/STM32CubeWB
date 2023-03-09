/**
  @page BLE_Zigbee_Dyn_SED example
  
  @verbatim
  ******************************************************************************
  * @file    BLE_Zigbee/BLE_Zigbee_Dyn_SED/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using BLE and Zigbee (acting as sleep end device) 
  *          in dynamic concurrent mode.
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

How to use BLE application and Zigbee application (acting as sleep end device) in dynamic concurrent mode.

The idea is to present a demo with a Zigbee Network (at least 2 Nucleo Boards, Zb coordinator
running Zigbee_OnOff_Server_Coord application[1] and Zb SED running BLE_Zigbee_Dyn_SED application[2]),
and a BLE connection between 1 Nucleo board running BLE_Zigbee_Dyn_SED application [2] with either a 
smartphone running "ST BLE Sensor" Application (available on App Store and Google Play), or another
Nucleo board running a BLE_p2pClient application[3].
The STM32WB55xx-Nucleo board running BLE_Zigbee_Dyn_SED application is capable of switching
dynamically and transparently from BLE network to Zigbee network and vice-versa.
Please note that no LEDs or buttons are active on the SED device as it is configured in Low power mode (LPM).

The demo architecture is summarized in the following figure:

    ______________               ______________               ______________ 
  /              \             /              \             /              \
  |              |             |              |             |              |
  |         SW1 => BLE Toggle  | Dynamic mode |  ZB Toggle  |              |
  |   BLE App    |===========> |  BLE/ZB App  |============>|    ZB App    |
  |              |             |              |             |              |
  |              |             |  (ZB SED)    |             | (ZB coord.)  |
  \______________/             \______________/             \______________/
        [3]                          [2]                          [1]  
 
 


(*)Note:
  BLE_Zigbee_Dyn_SED application is compiled with by default the define CFG_FULL_LOW_POWER set to 1.
  (in app_conf.h) 
  The define CFG_FULL_LOW_POWER when set to 1 will not configure resources that have impact on consumption 
  (LED, Traces, Access to Debugger, etc...). If you want to use and see the LEDs, the define CFG_FULL_LOW_POWER 
  must be set to 0.


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

This application requests having the STM32WB5x_BLE_Zigbee_RFD_dynamic_fw.bin binary flashed on the M0 Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to load the proper
Wireless Coprocessor binary. 


Minimum requirements for the demo:
- 1 STM32WB55xx-Nucleo board in BLE Zigbee Dynamic Concurrent mode [2]
- 1 STM32WB55xx-Nucleo board or more for creating Zigbee Network [1]
  (see example Zigbee_OnOff_Server_Coord and Zigbee_OnOff_Client_Router for more details on how to use it)
- 1 STM32WB55xx-Nucleo board running a BLE_p2pClient application[3]
-- OR
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
By default, after a reset, the ZB/BLE board [2] will start with both BLE and Zigbee modes activated.

The BLE example implements Point-to-Point communication using BLE component. 
1- The ZB/BLE device (P2P Server) starts BLE advertising and simultaneously starts to join the Zigbee Network.
2- The node will then attach the existing Zigbee network (Zb coordinator running Zigbee_OnOff_Server_Coord application [1]).
3- When the Zigbee connection is established, the Zigbee SED will send every second a ZB ON_OFF toggle to the ZB coordinator [1]
-- The RED Led toggles on the coordinator [1].  
4- The BLE connection will be started as follows, depending if your BLE device is a smartphone or a 3rd Nucleo board:
4.1 If using a smartphone, then the ST BLE Sensor App (smart Phone Application) scans and can connect to the P2P Server 
by selecting = "P2PZBSRV1" entry.
-- Once connected, the application starts to search the P2P Services & Characteristics.
-- Led Button service, Led Characteristic and Button Characteristic are discovered at this stage (this service is ST specific).
4.2 If using a 3rd Nucleo board (running a BLE_p2pClient application [3]), you can start scanning by pressing the SW1 button
-- the BLUE LED will light up on [3]
-- After scanning complete, the 3rd Nucleo board [3] connects to the P2P server on BLE/ZB [2] dynamic node, and sends a BLE toggle when pressing SW1.

NOTE : Please note that no LEDs or buttons are active on the SED device by default as it is configured in Low power mode (LPM).
       BLE_Zigbee_Dyn_SED application is compiled with by default the define CFG_FULL_LOW_POWER set to 1 (in app_conf.h) 
       If you want to use and see the LEDs, the define CFG_FULL_LOW_POWER must be set to 0.
       

   ______________               ______________               ______________ 
  /              \             /              \             /              \
  |              |             |              |             |              |
  |         SW1 => BLE Toggle  | Dynamic mode |  ZB Toggle  |              |
  |              |             |  BLE/ZB App  |  (every sec)|              |
  |              |             |              |             |              |
  |              |             |              |             |              |
  |   BLE App    |===========> |              |============>|    ZB App    |
  |              |             |              |             |              |
  |              |             |  (ZB SED)    |             | (ZB coord.)  |
  \______________/             \______________/             \______________/
        [3]                          [2]                          [1]  

This Demo illustrates the simultaneous BLE and ZB connections, with the RED Led toggling on the ZB coordinator[1], reflecting ZB activity, while BLE activity can be shown on the BLE App.

On BLE Disconnection, the STM32WB55xx-Nucleo board [3] will restart advertising. 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 
