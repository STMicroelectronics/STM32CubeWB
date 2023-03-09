/**
  @page Zigbee_OnOff_Client_Router application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_OnOff_Client_Router/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee OnOff Cluster application as a client  
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

How to use the OnOff cluster on a device acting as a Client with Router role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the OnOff cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through the push button. 
The reception of the message triggers a RGB LED.

For this application it is requested to have:

- 1 STM32WB5MMxx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Server_Coord
    
- 1 or more STM32WB5MMxx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Client_Router
    

              Device 1                                      Device 2
        
             ---------                                      ---------
             |       |       ZbZclOnOffClientOn/OffReq      |       |
     PushB1=>|Client | -----------------------------------> |Server | =>LED bright On/Off
             |       |                                      |       |
             |       |                                      |       |
             |       |                              PushB1=>|-------|-----|
             |       |                                      |       |     | Permit Join request
             |       |                                      |       |<----|
             ---------                                      ---------
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_OnOff_Server_Coord application (Device2 in the above diagram). 
      Wait for the message 'Network Ready'  being displayed on the LCD. 
      Start the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and the message 'Network Ready' should
      appear on the LCD screen of the coordinator board and 'Join Channel: 15' on the client board.
      It is now possible to send OnOff Cluster commands from the client to the server in unicast mode 
      by pressing on the B1 push button on client board. 
      You must see the RGB LED toggling on the server side.

  c)  After the Network forming timeout, a press on B1 push button on Server board send a request
      to permit Join the network during PERMIT_JOIN_DELAY (60sec) for a new device.

@par Keywords

Zigbee
 
@par Hardware and Software environment

  - This example runs on STM32WB5MMxx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB5MM-DK 
    board and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ? 

=> Loading of the stm32wb5x_Zigbee_FFD_fw.bin binary

  This application requests having the stm32wb5x_Zigbee_FFD_fw.bin binary flashed on the Wireless Coprocessor.
  If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
  All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
  Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
  Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
  Wireless Coprocessor binary. 

=> Getting traces
  To get the traces you need to connect your Board to the HyperTerminal (through the STLink Virtual COM Port).
  The UART must be configured as follows:

    - Baud Rate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

=> Running the application

  Refer to the Application description at the beginning of this readme.txt

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>

