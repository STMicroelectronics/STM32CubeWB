/**
  @page Zigbee_Commissioning_Client_Coord application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_Commissioning_Client_Coord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee Commissioning Cluster application
  *          (Coordinator part) acting as a Client using a centralized network. 
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

How to use the Commissioning cluster on a device acting as a Client with Coordinator role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to operate a commissioning process using the Commissioning cluster. Once the Zigbee mesh 
network is created, the user can send requests for the commissioning process from client to 
server (through push buttons SW1).

The router network info can be checked by pressing push button SW1. It will show: 
- Network address assigned to the device.
- Network PAN ID and extended PAN ID.

Note: You can press push button SW1 on Commissioning client router before and after
      Commissioning process to see the changes.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Commissioning_Server_Router
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_Commissioning_Client_Coord
    
    
Demo use case :

This demo shows how to use Commissioning cluster.
The Commssioning cluster is used to operate a commissioning process on a not connected 
Zigbee router (Commissioning server) from the Zigbee coordinator (Commissioning client).

For that, Commissioning process from the client uses INTERPAN communication that allows to 
address requests to not connected devices.

The Commissioning process is as follow:

0. Initial server configuration: commissioning capabiliy (INTERPAN communication) enabled.
   --> device MAC layer configured to listen for packets.
   --> the server is able to handle requests even if not connected through the network.

1. The client sends Reset Startup request to server.
   --> the server current startup configuration is cleared.
   
2. The client remotely writes commissioning server attributes (INTERPAN communication):
   - ZCL_COMMISSION_CLI_ATTR_SHORT_ADDR
   - ZCL_COMMISSION_CLI_ATTR_EPID
   - ZCL_COMMISSION_CLI_ATTR_STACKPROFILE
   - ZCL_COMMISSION_CLI_ATTR_CHANNELMASK
   - ZCL_COMMISSION_CLI_ATTR_USEINSECJOIN
   - ZCL_COMMISSION_CLI_ATTR_PRECONFLINKKEY
   - ZCL_COMMISSION_CLI_ATTR_NWKKEYTYPE
   - ZCL_COMMISSION_CLI_ATTR_STARTUPCONTROL
   - ZCL_COMMISSION_CLI_ATTR_SCANATTEMPTS
   
   --> the server startup configuration changes.
   
3. The client sends Save Startup request to server (request with an index value of 0).
   --> the server current startup configuration is saved (under index 0).
   
4. The client sends Reset Startup request to server.
   --> the server current startup configuration is cleared.
   
5. The client sends Restore Startup request to server (request with an index value of 0).
   --> the server restore current startup configuration from a saved 
       startup configuration (index 0).   
       
6. The client sends Restart device request to server.
   --> the server operate a full startup process with current startup configuration.
   --> the server router joins the network.
    

              Device 1                                                                        Device 2
          
              +--------+                                                                      +--------+
             |        |                                                                      |        |  
             | Client |                                                                      | Server |     
             |        |                   /* Commissioning server init */                    |        |              
             |        |                                                                   -->|        | 
             |        |                                   ZbZclCommissionServerEnable()  |   |        |
             |        |                                                                   ---|        |
             |        |                                                                      |        |
             |        |                                          /* Show network info */  <--|        | <=PushB SW1
             |        |                                                                      |        |
             |        |                                                                      |        |
             |        |                                                                      |        |
             |        |                    /* Commissioning process */                       |        |
             |        |                                                                      |        |
             |        |               ZbZclCommissionClientSendResetStartup()                |        |
  PushB SW1=>|        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |                           ZbZclWriteReq()                            |        |
             |        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |                ZbZclCommissionClientSendSaveStartup()                |        |
             |        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |               ZbZclCommissionClientSendResetStartup()                |        |
             |        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |               ZbZclCommissionClientSendResoreStartup()               |        |
             |        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |              ZbZclCommissionClientSendRestartStartup()               |        |
             |        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             |        |                                                                   -->|        | 
             |        |     /* Waiting the delay mentioned in the RestartStartup cmd */  |   |        |=> GREEN LED ON (during delay)
             |        |                                                                   ---|        |
             |        |                                                                      |        |
             |        |                                                                   -->|        | 
             |        |                                   /* Router joins the network*/  |   |        |=> BLUE LED ON
             |        |                                                                   ---|        |
             |        |                                                                      |        |
             |        |                                                                      |        | 
             |        |                                 /* Show updated network info */  <-- |        |<=PushB SW1
             |        |                                                                      |        |
             |        |                                                                      |        |
             +--------+                                                                      +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_Commissioning_Client_Coord application (Device1 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee router but it will not connect to the 
      Zigbee network. Connecting the router to the network is the goal of this demo.

  b)  Press the SW1 button on Device 2 to check for the network information through the traces.

  c)  Press the SW1 button to initiate the commissioning
      
  d)  When Commissioning process is complete, the router joins the network and its Blue LED (LED1) is ON.

  e)  Press the SW1 button again on Device 2 and check that the network information have been updated.

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

