/**
  @page Zigbee_MeterId_Client_Router application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_MeterId_Client_Router/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee Meter Identification Cluster application as a client  
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

How to use the Meter Identification cluster on a device acting as a client with Router role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the Meter Identification cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through push buttons SW1 and SW2.

For this application it is requested to have:

- 1 STM32WB55xx (Nucleo or USB dongle) board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_MeterId_Server_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_MeterId_Client_Router
    
    
Demo use case :

This demo shows how to use Meter Identification cluster.
The Meter Identification Server set up its read-only attributes (read-only attributes local write allowed).
The Meter Identification Client can remotely read them by requesting a remote read.  

Two read-only attributes are used : 
- the Company Name (ZCL string)
- the Meter Type ID (16-bit integer)
    

              Device 1                                                                        Device 2
          
             +--------+                                                                      +--------+
             |        |                                                                      |        |                                       
             | Client |                                                                   -->| Server | 
             |        |        ZbZclAttrStringWriteShort(ZCL_METER_ID_ATTR_COMPANY_NAME) |   |        |
             |        |                                                                   ---|        |
             |        |                                                                      |        |
             |        |                                                                   -->|        | 
             |        |       ZbZclAttrStringWriteShort(ZCL_METER_ID_ATTR_METER_TYPE_ID) |   |        |
             |        |                                                                   ---|        |
             |        |                                                                      |        |
             |        |             ZbZclReadReq(ZCL_METER_ID_ATTR_COMPANY_NAME)             |        |
  PushB SW1=>|        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |             ZbZclReadReq(ZCL_METER_ID_ATTR_METER_TYPE_ID)            |        |
  PushB SW2=>|        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             +--------+                                                                      +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_MeterId_Server_Coord application (Device2 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee router and will be attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices.
      it is now possible to remotely read Meter Identification server attributes on the client by pressing on 
      the SW1/SW2 push button. 

 Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

@par Keywords

Zigbee
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - For the Zigbee_MeterId_Server_Coord application, a STM32WB55xx USB dongle can be used.
  
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

