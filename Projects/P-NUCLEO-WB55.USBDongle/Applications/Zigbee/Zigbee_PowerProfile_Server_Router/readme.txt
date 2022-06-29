/**
  @page Zigbee_PowerProfile_Server_Router application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_PowerProfile_Server_Router/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee Power Profile Cluster application as a server  
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

How to use the Power Profile cluster on a device acting as a Server with Router role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the Power Profile cluster. Once the Zigbee mesh 
network is created, the user can start the demo scenario on the server through push button SW1.

For this application it is requested to have:

- 1 STM32WB55xx board (Nucleo or USB dongle) loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_PowerProfile_Client_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_PowerProfile_Server_Router
    
    
Demo use case :

This demo shows how to use Power Profile cluster in a Appliance/Home Gateway configuration by simulating a White Good generic behaviour.
The Power Profile Server (e.g the appliance) is not remotly controllable. Therefore, attributes 
are initialized locally on server side.
The Power Profile Server start the demo (SW1 pushed) and notifications are sent on the client (e.g the Home Gateway).
The following state machine is simulated on the server : Idle -> Programmed -> Waiting to start -> Running -> Ended. 

There are different actions grouped in 3 main steps. 

Step 1:
=======

- Power Profile State Notification (Idle, no remote control) is sent by Server.
- Power Profile Notification is sent from Server to Client.
    + Power profile ID 1.
    + Single Energy Phase.
    + Expected duration (90 mn).
    + MaxActivationDelay set to 0xFFFF as first Energy Phase.
- Power Profile State Notification (Programmed, no remote control) is sent by Server.

Step 2:
=======

- Get Power Profile Price is sent by Server.
- Get Power Profile Price Response is sent by Client (Cost of 10, Profile ID 1).

Step 3:
=======

- Power Profile State Notification (Waiting to start, no remote control) is sent by Server.
- Wait 2 seconds.

Step 4:
=======

- Power Profile State Notification (Running, no remote control) is sent by Server.
- Wait 10 seconds (Green LED blinking on Server side).
- Power Profile State Notification (Ended, no remote control) is sent by Server.


              Device 1                                                                                  Device 2
          
             +--------+                                                                                +--------+
             |        |                                                                                |        |                                       
             | Client |                                                                             -->| Server | 
             |        |        ZbZclAttrStringWriteShort(ZCL_POWER_PROF_SVR_ATTR_TOTAL_PROFILENUM) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |          ZbZclAttrStringWriteShort(ZCL_POWER_PROF_SVR_ATTR_MULTIPLE_SCHED) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |           ZbZclAttrStringWriteShort(ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |           ZbZclAttrStringWriteShort(ZCL_POWER_PROF_SVR_ATTR_ENERGY_REMOTE) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |           ZbZclAttrStringWriteShort(ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE) |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                   /* Step 1 */                                 |        |
             |        |                                                                                |        |
             |        |         ZbZclPowerProfServerStateNotify(ZCL_PWR_PROF_STATE_PROFILE_IDLE)       |        |
             |        | <----------------------------------------------------------------------------- |        |<== PushB SW1
             |        | -----------------------------------------------------------------------------> |        |         
             |        |                                                                                |        |
             |        |                      ZbZclPowerProfServerProfileNotify                         |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        |                                                                                |        |
             |        |      ZbZclPowerProfServerStateNotify(ZCL_PWR_PROF_STATE_PROFILE_PROGRAMMED)    |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             |        |                                                                                |        |
             |        |                                   /* Step 2 */                                 |        |
             |        |                                                                                |        |
             |        |                          ZbZclPowerProfServerGetPriceReq                       |        | 
             |        | <----------------------------------------------------------------------------- |        |
             |        |                                                                                |        |
             |        |                           ZbZclPowerProfClientPriceRsp                         |        | 
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             |        |                                                                                |        |
             |        |                                   /* Step 3 */                                 |        |
             |        |                                                                                |        |
             |        |     ZbZclPowerProfServerStateNotify(ZCL_PWR_PROF_STATE_PHASE_WAITING_START)    |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |                                                                    Wait 2s |   |        |
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |                                                                                |        |
             |        |                                   /* Step 4 */                                 |        |
             |        |                                                                                |        |
             |        |        ZbZclPowerProfServerStateNotify(ZCL_PWR_PROF_STATE_PHASE_RUNNING)       |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             |        |                                                                             -->|        | 
             |        |                                                                   Wait 10s |   |        |=> GREEN LED is blinking
             |        |                                                                             ---|        |
             |        |                                                                                |        |
             |        |        ZbZclPowerProfServerStateNotify(ZCL_PWR_PROF_STATE_PROFILE_ENDED)       |        |
             |        | <----------------------------------------------------------------------------- |        |
             |        | -----------------------------------------------------------------------------> |        |
             |        |                                                                                |        |
             +--------+                                                                                +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_PowerProfile_Client_Coord application (Device1 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices.
      it is now possible to simulate a generic appliance behaviour on the server by pressing the SW1 push button. 

 Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

@par Keywords

Zigbee
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - For the Zigbee_PowerProfile_Server_Router application, a STM32WB55xx Nucleo can be used.
  
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

