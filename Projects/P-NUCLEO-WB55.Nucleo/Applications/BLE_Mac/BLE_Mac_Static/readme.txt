/**
  @page BLE_Mac_Static example
  
  @verbatim
  ******************************************************************************
  * @file    BLE_Zigbee/BLE_Mac_Static/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using BLE and MAC protocols in static 
  *          concurrent mode.
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

How to use BLE application and 802_15_4 Mac application in static concurrent mode.

This use case requires one STM32WB55xx-Nucleo board with Static Concurrent Mode 
application running on it.Static mode means this application can perform alternatively 
BLE or Mac activities. 

The idea is to present a demo with a device running the BLE/MAC concurrent mode
able to interact with a Phone (or the BLE p2p client application) and a device 
running the MAC RFD application.

@par Keywords

MAC, BLE

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

This application requests having the appropriate binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to 
change the Wireless Coprocessor binary. 



    _____________                    ______________                    ______________ 
  /              \                  /              \                  /              \
  |              |                  |              |                  |              |
  |         SW1 => BLE Toggle       | Static  mode |  MAC Toggle      |              |
  |   BLE App    |----------------> |  BLE/Mac App |<-----------------|    Mac App   |
  |              |                  |              |                  |              |
  |              |                  |  (Mac coord.)|                  | (Mac node)   |
  \______________/                  \______________/                  \______________/
        Board [1]                       Board [2]                        Board [3]
          or
      Smartphone  
 
 
 BLE_p2pClient Application           BLE_Mac_Static Application        Mac_802_15_4_RFD
           +                                     +                              +
 stm32wb5x_BLE_Stack_full_fw.bin Fw  stm32wb5x_Ble_Mac_802_15_4_fw.bin stm32wb5x_Mac_802_15_4_fw.bin
 
 
 

               Board [1]                  Board [2]                       Board [3]
              BLE p2p Client             Static BLE+MAC                   MAC node
                                        (Mac coord.)                    (Mac node) 
             _____________              ________________                 ____________
                  .                            .                            .
         Boot ==> |                            .                            .
                  |                   Boot ==> |                            .
                  |         Start in BLE mode  |                            .
                  |        (Green LED blinking)|                            .
                  |                            |                            .
                  |                            |                            .
  SW1 PushButton=>|                            |                            .
    Start general |                            |                            .
    discovery     |                            |                            .
 (Blue LED On     |                            |                            .
  for 5 sec during|                            |                            .
  the scan)       |                            |                            .
                  | <==BLE CONNECTED==========>|                            . 
                  |                            |                            .
                  |                            |                            .
  SW1 PushButton=>|--------------------------->|                            .
                  |     red LED toggles at     |                            .
                  |     each BLE Client request|                            .
                  |                            |                            .
                  |                            |                            .
                  |           SW2 PushButton=> |                            .
                  |      Start in 802_15_4 mode|                            .
                  |                            |                    Boot ==>|  
                  |    On in coming association|                            |
                  |    request FFD's blue LED  |                            |
                  |    goes on                 |                            |
                  |                            |                            | 
                  |                            |     Once the association   |
                  |                            |     between two devices is |
                  |                            |     completed, green LED is| 
                  |                            |     switched on to indicate| 
                  |                            |     the node is associated | 
                  |                            |     to the coordinator     | 
                  |                            |                            |
                  |                            |<==ASSOCIATION DONE========>|
                  |                            |                            |
                  |                            |            SW1 PushButton=>|
                  |                            |                            |
                  |                            |<---------------------------|
                  |             Red LED toggles|                            |             
                  |             at each MAC    |                            |
                  |             request        |                            | 
                  |                            |                            |
                  |                            |                            |                     
                  |            SW2 PushButton=>|                            |
                  |          back in BLE mode  |                            |
                  |                            |                            |


@par Ke

Minimum requirements for this demo:

- 1 STM32WB55xx-Nucleo board running the BLE Mac static concurrent mode application 
  (This device is referred as [2] on the figure above)
  
- 1 STM32WB55xx-Nucleo board running the Mac RFD static application 
  (This device is referred as [3] on the figure above)
  
- A BLE device. It can be either a phone or the p2p client application.
    If you want to use the phone, you need:  
          A Smartphone (Android/IOS) with "ST BLE Sensor" Phone Application (available on App Store and Google Play)
          Install the "ST BLE Sensor" application on the ios/android device
          https://play.google.com/store/apps/details?id=com.st.bluems
          https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8
   If you want to use a STM32WB55xx-Nucleo, you need:
          A STM32WB55xx-Nucleo board running the Mac RFD application
          (This device is referred as [1] on the figure above)        

    
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.

********************
**** START DEMO ****
********************
By default, after a reset, the board [2] running the static concurrent mode will start in BLE mode.
This device will start advertising (during 1 minute) and the green led will blinks for each advertising event.

* BLE mode
==========
The BLE use case demonstrates point-to-point communication using BLE component.

 If you use the p2p client application :
 
 - The Board [1] (BLE_p2pClient) starts scanning when pressing the User button (SW1) on the Nucleo board.  
   - BLE_p2pClient blue LED becomes on. 
   - Scan req takes about 5 seconds.
   - Then, the Board [1] automatically connects to Board [2]. 
   - Blue led turns off and green led starts blinking. 
   - Connection is done.
   
 If you use a phone :  
 
  The Peripheral device (P2P Server) starts advertising,
  ST BLE Sensor smart Phone Application) scans and can connect to the P2P Server 
  by selecting = "P2P_SERVER1" entry.
  Once connected, the application starts to search the P2P Services & Characteristics.
  Led Button service, Led Characteristic and Button Characteristic are discovered at this stage 
  (this service is ST specific).
  Once the smartphone is connected, the user can interact with the board by pressing 
  "LED1 CONTROL" field which toggles LED1 on the board.  
 - When pressing SW1 on a Board [1], the blue led toggles on Board [2].

* Switching to MAC mode
=======================
 - This is done by pressing on SW2 on Board [2] 
 
* MAC mode
==========

The MAC 802.15.4 use case demonstrates a point-to-point communication using MAC components between 
a Board [2] and Board [3] acting as a node. 

Once in MAC mode, the board[2] initializes the MAC 802.15.4 and associated resources in non-beaconned
enable mode.

MAC PIB is then configured as follow :
    PanID : 0x1AAA
    Coordinator Extended Address : 0xACDE480000000001
    Coordinator Short Address    : 0x1122

Once MAC PIB is initialized, Board [2] is started on channel 21 as an association capable coordinator.
At this stage the Board [2] is waiting for an association request and blue LED (ID 1) is switch on.

It is now possible to power on Board [3] as shown on the figure above.
On node side (Board [3]), the MAC layer is initialized on startup and blue LED (ID 1) is switch on.

Board [3] issues an association request to Board [2] to retrieve its short address. 
Once the association between two devices is completed, Board[3] green LED (ID 2) is switch on to indicate the node 
is associated to the coordinator.

The Board [3] sends data to the coordinator that issues a Data Indication.
On both devices side, on data transmission red led (ID 3) toggle

* Switching back to BLE mode
============================
 - This is done by pressing on SW2 on Board [2] 



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 