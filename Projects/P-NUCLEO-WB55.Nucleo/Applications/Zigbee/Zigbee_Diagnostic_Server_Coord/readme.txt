/**
  @page Zigbee_Diagnostic_Server_Coord application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_Diagnostic_Server_Coord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee Diagnostic Cluster application as a client  
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

How to use the Diagnostic cluster as a device acting as a Server with Coordinator role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the Diagnostic cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through push buttons SW1 and SW2.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Diagnostic_Server_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Diagnostic_Client_Router
    
    
Demo use case :

This demo shows how to use Diagnostic cluster.
The coordinator (the 'server') is composed of 1 Diagnostic server and 1 OnOff server. 
The router (the 'client') is composed of 1 Diagnostic client and 1 OnOff client. 

The client can send ZCL OnOff toggle request to the OnOff server.  
The client can remotely read Diagnostic server attributes by requesting a remote read.  

Two Diagnostic attributes related to unicast RX/TX are used : 
- ZCL_DIAG_ATTR_APS_RX_UCAST
- ZCL_DIAG_ATTR_APS_TX_UCAST_SUCCESS

When the user reads these attributes, their changes change because the ZCL read is based on RX/TX too.
It's the same for the ZCL OnOff commands.
    

              Device 1                                                                        Device 2
          
             +--------+                                                                      +--------+
             |        |                                                                      |        |                                       
             | Client |                                                                      | Server | 
             |        |                                                                      |        |
             |        |                                                                      |        |
             |        |                                                                      |        |
             |        |                      ZbZclOnOffClientToggleReq()                     |        |
  PushB SW1=>|        | -------------------------------------------------------------------> |        |=> GREEN LED toggle
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |                            ZbZclReadReq()                            |        |
  PushB SW2=>|        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             +--------+                                                                      +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_Diagnostic_Server_Coord application (Device2 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices.
      it is now possible to remotely read Diagnostic server attributes on the client by pressing on 
      the SW1/SW2 push button. 

 Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

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

