/**
  @page Zigbee_APS_Router application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_APS_Router/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee APS application using a
  *          centralized network and acting as a Router. 
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

How to use the APS layer in an application acting as a Router within a centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how use the APSDE interface to the Zigbee stack directly, to send and receive raw 
APS messages between devices on the network.

Once the Zigbee mesh  network is created, the user can send APS requests from the 
Zigbee router to the Zigbee coordinator through push buttons SW1.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_APS_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_APS_Router
    
    
Demo use case :

This demo shows how to use APSDE interface on a customer application.
Packets are sent using the APSDE Data Request API.
Packets are received by a filter callback configured at the device initialization.

The APS messages use a profile of 0xf000, which is in the manufacturer
specific range for profile Ids and was arbitrarily chosen. In an actual
product, the profile Id should be registered with the Zigbee Alliance.

The router will send APS request commands (clusterID = 0x0000) to 
the coordinator (at address 0x0000) through push button SW1.
If a device receives a request command, it sends a response command 
(cluster Id = 0x0001) back to the originating device.

The payloads used in this application are arbitrary and not used by the sender or receiver.
APS frame exchange scheme is available through traces.
    

              Device 1                                                                        Device 2
          
             +--------+                                                                      +--------+
             |        |                                                                      |        |                                       
             | Router |                                                                      | Coord  | 
             |        |                                                                      |        |
             |        |                                                                      |        |
             |        |                       ZbApsdeDataReqCallback()                       |        |
  PushB SW1=>|        | -------------------------------------------------------------------> |        |
             |        | <------------------------------------------------------------------- |        |
             |        |                                                                      |        |
             +--------+                                                                      +--------+
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_APS_Coord application (Device2 in the above diagram). 
      Start immediately after, the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices. 

  c) Press on the push button (SW1) on Device 1 (Router board) and check the messages being exchanged via the traces.
     To get the traces, refer to explanation defined at the end of this readme.tx.

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

