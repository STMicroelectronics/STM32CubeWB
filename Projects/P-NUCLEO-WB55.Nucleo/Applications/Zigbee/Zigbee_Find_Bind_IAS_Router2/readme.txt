/* Copyright [2020 - 2020] Exegin Technologies Limited. All rights reserved. */

/**
  @page Zigbee_Find_Bind_IAS_Router2 application

  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_Find_Bind_IAS_Router2/readme.txt
  * @author  Exegin Technologies Limited, MCD Application Team
  * @brief   Description of the Zigbee Finding and Binding application
  *          using a centralized network.
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

How to use the Finding and Binding feature on a device using IAS cluster, acting as a Client with Router role within a Centralized Zigbee network.

The purpose of this application is to show how Finding and Binding (F&B) works on a Zigbee centralized network.
This will demonstrate how F&B initiator will automatically create bindings to bindable clusters on the
F&B target.
Once clusters are bound, they will be able to communicate through bindings which will toggle LEDs and output
messages through the serial interface.

For this specific application:

  ZC embeds following clusters:
    - ZCL Scenes Server
    - ZCL Messaging Server
    - ZCL IAS Warning Device Server
    - ZCL OnOff Server

  ZC Will have the following functionality:
    BUTTON1: Turn on identify mode for 30 seconds (For F&B)
    BUTTON2: Send ZCL Display Message Command via its bindings (Sending will fail if no bindings to this cluster exists)

  ZC Has the following LEDs:
    RED: IAS Warning status
    GREEN: OnOff attribute state
    BLUE:
      ON: Network formation success
      OFF: Network formation failure

For this application it is requested to have:

- 1 STM32WB55xx board loaded with:
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Find_Bind_Coord

- 1 or more STM32WB55xx board loaded with:
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Find_Bind_OnOff_Router1

- 1 or more STM32WB55xx board loaded with:
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Find_Bind_IAS_Router2


              Router 1                                      Coordinator

             ---------                                      ---------
             |       |       ZCL commands                   |       |
      PushB=>|Client | <----------------------------------> |Server | =>LED
             |       |                  |                   |       | =>UART
             |       |                  |                   |       |
              --------                  |                   ---------
                                        |
             Router 2                   |
                                        |
             ---------                  |
             |       |                  |
      PushB=>|Client |<-----------------
             |       |
             |       |
              --------


 ---------------------------------      ------------------------------    ------------------------
 | Zigbee_Find_Bind_OnOff_Router1|      |Zigbee_Find_Bind_IAS_Router2|    |Zigbee_Find_Bind_Coord |
 ---------------------------------      ------------------------------    ------------------------
             |                                       |                             | 
             |                                       |                 Power up  =>|
             |                                       |                             |Allocate 
             |                                       |                             | -OnOff
             |                                       |                             | -IdentifyServer, 
             |                                       |                             | -ScenesServer,
             |                                       |                             | -IasWdServer,
             |                                       |                             | -MsgServer, 
             |                                       |                             |Clusters
             |                                       |                             |
             |                                       |                             |Creation of the network
             |                                       |                             |(Blue LED goes On)
             |                                       |                             |
             |                                       |                      SW1  =>|Turn on the                                                       
             |                                       |                             |Identify Mode 
             |                                       |                             |(valid during 30 Seconds)
             |                                       |                             |
             |                           Power up  =>|                             |
             |                                       |                             |
             |                                       |Allocate                     |
             |                                       | -MsgClient                  |
             |                                       | -IdentifyClient             |
             |                                       |Clusters.                    |
             |                                       |                             |           
 Power up  =>|                                       |                             |
             |Allocate                               |                             |
             |-OnOffClient                           |                             |
             |-IdentifyClient                        |                             |
             |-ScenesClient                          |                             |
             |Clusters                               |                             |
             |                                       |                             |
             |After a few seconds,                   |After a few seconds,         |
             |the blue LED should light on           |the blue LED should light on |
             |(The network is now established)       |(The network is now establ.) |
             |                                       |                             |
             |Once the find and bind procedure       |                             |
             |is completed, the green LED goes On    |                             | 
             |(Binding entries created = 3)          |                             |
             |                                       |                             | 
             |(Note : On ZR1, the find and bind is   |                             |
             |initiated automatically)               |                             |
             |                                       |                             |
             |                                       |                             |
             |                                 SW3=> |Initiate the                 |
             |                                       |find and bind procedure      |
             |                                       |                             |
             |                                       |Once the find and bind       |
             |                                       |procedure is completed,      |
             |                                       |the green LED goes On        |
             |                                       |(Binding entries created = 2)|    
             |                                       |                             |
             |                                       |Allocate                     |
             |                                       |-IAS_client cluster          |
             |                                       |Clusters                     |
             |                                       |once the first find          |
             |                                       |and bind as been done        |
             |                                       |                             |
             |                                       |                             |
             |                            Display msg|<----------------------------|<= SW2
             |                      (refer to traces)|                             |
             |                                       |                             |
             |                                       |                             |
             |                                  SW3=>|Initiate a new               |
             |                                       |find and bind (adding IAS)   |
             |                                       |(Binding entries created = 3)|
             |                                       |                             |
             |                                       |                             |
             |                                       |                             |
             |                                       |    Sending Warning command  |
             |                                 SW2 =>|---------------------------->|
             |                                       |                             |Red LED
             |                                       |                             |goes on 
             |                                       |                             |for 3 sec
             |                                       |                             |
       SW3 =>|---------------------------------------------------onOff togle------>|Green LED
             |                                       |                             |toggles
             |                                       |                             |
             |                                       |                             |
       SW1 =>|---------------------------------------------------Store Scene------>| Store Green LED status
             |                                       |                             | (either on or off)
             |                                       |                             |
       SW3 =>|---------------------------------------------------onOff togle------>|Green LED
             |                                       |                             |toggles
             |                                       |                             |
             |                                       |                             |
       SW2 =>|---------------------------------------------------Recall Scene----->| If the Green LED status
             |                                       |                             | stored previously via the 
             |                                       |                             | store scene command was on,
             |                                       |                             | the green LED will go back 
             |                                       |                             | to on
             |                                       |                             |
             
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.

 To run the application :

*Step 1*
On power, the Zigbee Coordinator (ZC) will initialize the stack and allocate the following clusters on endpoint 10:
* ZCL Scenes Server
* ZCL Messaging Server
* ZCL IAS Warning Device Server
* ZCL OnOff Server
The application will then form the network and will turn on the BLUE LED on successful network formation.
Should the network formation fail, the BLUE LED will remain off and the application will reattempt network formation after five seconds.

*Step 2*
On ZC, the user can press the following button at any time after successful network formation:
* BUTTON 1: Turn on Identify Mode for 30 Seconds
NOTE: This button needs to be pressed before any of the routers perform finding and binding.

*Step 3a*
On power, Zigbee Router 1 (ZR1) will initialize the stack and allocate the following clusters on endpoint 11:
* ZCL Scenes Client
* ZCL OnOff Client
The application will then attempt to join a network, and start the Finding and Binding procedure as an Initiator.
The ZR2 will turn on the BLUE LED on successful network join, and turn on the GREEN LED on successful F&B procedure.
Should the network join or F&B procedure fail, the RED LED will turn on and the application will attempt to send a Network Leave, and then attempt to join and F&B after five seconds.

*Step 3b*
On power, Zigbee Router 2 (ZR2) will initialize the stack and allocate the following clusters on endpoint 12:
* ZCL Messaging Client
The application will then attempt to join a network. User will need to press Button 3 to start the Finding and Binding procedure as an Initiator.
The ZR2 will turn on the BLUE LED on successful network join, and turn on the GREEN LED on successful F&B procedure.
Should the network join fail the RED LED will turn on, and then attempt to join after five seconds.

*Step 4*
On ZR1, the user can press the following buttons at any time after successful network join and F&B to send ZCL commands via the established bindings:
* BUTTON1: ZCL Store Scenes Command
* BUTTON2: ZCL Recall Scenes Command
* BUTTON3: ZCL OnOff Toggle Command

*Step 4a*
ZC will store the current value of the OnOff attribute upon reception of the ZCL Store Scenes Command.

*Step 4b*
ZC will change the current value of the OnOff attribute upon reception of the ZCL OnOff Toggle Command.
The ZC will turn on the GREEN LED if the OnOff attribute is set to On.
Otherwise, the GREEN LED will be off if the OnOff attribute is set to Off.

*Step 4c*
ZC will restore the current value of the OnOff attribute upon reception of the ZCL recall Scenes Command.
The GREEN LED will match the status of the OnOff attribute once the scene has been restored.

*Step 5a*
On ZR2, the user can press BUTTON 1 at any time after successful network join and F&B to send a ZCL Get Last Message command from the ZCL Messaging Client via the established bindings.
This will prompt the ZC Messaging Server to send a Display Last Message command, and will display messages via the UART.

*Step 5b*
On ZC, the user can press the following button at any time after successful network formation:
* BUTTON 2: Send ZCL Display Message command via Bindings
NOTE: This command will fail unless the messaging server is bound to a client.

*Step 6*
Via application specific means, ZR2 will acknowledge that it has successfully performed F&B from previous steps.
Pressing BUTTON 3 again will allocate the ZCL IAS Warning Device Server cluster on endpoint 12, and will re-perform Finding and Binding as an Initiator.
The GREEN LED will stay on if the F&B procedure is successful.
Otherwise the GREEN LED will turn off, and the user can press BUTTON 3 again to reattempt F&B.

*Step 7a*
On ZR2, the user can press BUTTON 1 at any time after a successful network join and find and bind to send a ZCL Get Last Message command from the ZCL Messaging Client via the established bindings.
This will prompt the ZC Messaging Server to send a Display Last Message command, and will display messages via the UART.

*Step 7b*
ZR2 will now have BUTTON 2 available to send the ZCL Start Warning command via the established bindings.
This will prompt the ZC WD Server to turn on the RED LED for the duration of the Warn Device Command.

Note: when LED1, LED2 and LED3 are all toggling it is indicating an error has occurred on application.

@par Keywords

Zigbee

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.

  - This example has been tested with an STMicroelectronics STM32WB55RG_Nucleo
    board and can be easily tailored to any other supported device
    and development board.

  - On STM32WB55RG_Nucleo, the jumpers must be configured as described
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

=> Loading of the stm32wb5x_Zigbee_FFD_fw.bin binary

  This application requests having the stm32wb5x_Zigbee_FFD_fw.bin binary flashed on the Wireless Coprocessor.
  If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
  All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
  Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
  Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
  Wireless Coprocessor binary.

=> Getting traces
  To get the traces you need to connect your Board to the Hyperterminal (through the STLink Virtual COM Port).
  The UART must be configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

=> Running the application

  Refer to the Application description at the beginning of this readme.txt

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>