/**
  @page Zigbee_PollControl_Server_SED application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_PollControl_Server_SED/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee Poll Control Cluster application as a server  
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

How to use the Poll Control cluster on a Sleepy End Device (SED) acting as a Server within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the Poll Control cluster. Once the Zigbee mesh 
network is created, the user can start the demo scenario on the client through push button SW1.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_PollControl_Client_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_RFD_fw.bin
    - application : Zigbee_PollControl_Server_SED
    
    
Demo use case :

This demo shows how to use Poll Control for remotely operating on a Sleepy End Device (SED).

The Poll Control server, on the SED, sends periodically Checkin requests to the Poll Control client 
to know its polling policy (Long or Fast Poll) until the next Checkin request.
The client can respond to Checkin requests with:
- Fast Poll field set to false => the server will continue Long Polling (default behavior). 
- Fast Poll field set to true  => the server is going to enter in Fast Polling mode 
  (during the specified Fast Poll timeout).

During this Fast Polling period, the SED is not sleeping and remote operation are possible.
So, during this period of time, the client remotely writes Poll Control server attributes.

The client will respond to the next server Checkin request with a Fast Poll response when push 
button SW1 is pressed (client side}.
The current Poll Control configuration of the server can be shown by pressing button SW1 (server side).
It can be used to validate the client remote operation during the server Fast Polling period.


              Device 1                                                                                  Device 2
          
             +--------+                                                                                +--------+
             |        |                                                                                |        |                                       
             | Client |                                                                             -->| Server | 
             |        |                          ZbZclAttrIntegerWrite(ZCL_POLL_CHECK_IN_INTERVAL) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                          /* Default Long Poll mode */                          |        |
             |        |                                                                                |        |
             |        |                             /* Checkin request */                              |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        |             /* Checkin response (Fast Poll false) => Long Poll */              |        |
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             |        |                                                                                |        |
             |        |                              /* Fast Poll mode */                              |        |
             |        |                                                                                |        |
             |        | <--                                                                            |        |
  PushB SW1=>|        |    |  zcl_poll_client_set_checkin_rsp(ZCL_PWR_PROF_STATE_PROFILE_IDLE)         |        |
             |        | ---                                                                            |        |
             |        |                                                                                |        |
             |        |                             /* Checkin request */                              |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        |             /* Checkin response (Fast Poll true) => Short Poll */              |        |=> GREEN LED ON if CFG_FULL_LOW_POWER=0
             |        | -----------------------------------------------------------------------------> |        |   (during fast poll timeout)
             |        |                                                                                |        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |                            /* Show local Poll Control server attributes */ |   |        |<=PushB SW1 if CFG_FULL_LOW_POWER=0
             |        |                                log visible if CFG_FULL_LOW_POWER=0          ---|        |
             |        |                                                                                |        |
             +--------+                                                                                +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_PollControl_Client_Coord application (Device 1 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee SED and will attached to the network created 
      by the coordinator. The Blue LED (LED1) of the SED will turn ON depending on the value of CFG_FULL_LOW_POWER 
	  (if the CFG_FULL_LOW_POWER=0 then the power will be minimal BUT the SED'LEDs will not light).
	  Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices.

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

