/**
  @page Mac_802_15_4_Coordinator application
  
  @verbatim
  ******************************************************************************
  * @file    Mac_802_15_4/Mac_802_15_4_Coordinator/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using MAC 802.15.4 protocols implementing 
             light 802.15.4 Coordinator services - Coordinator
  ******************************************************************************
  *
  * Copyright (c) 2019-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description

How to use MAC 802.15.4 Association and Data exchange.

This application requires two STM32WB55xx-Nucleo boards. One device will act as 
802.15.4 coordinator ensuring association response and network establishment. 
The other device will act as a MAC 802.15.4 Node. The Node request 
association on startup and send data to the coordinator. 

Below example implements the Coordinator Devices.

@par Keywords

MAC, 802.15.4, association, data, STM32WBA, Coordinator, ED scan, diassociation, Active Scan, poll request, Coordinator

@par Directory contents 


Mac_802_15_4/Mac_802_15_4_Coordinator/

   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/app_common.h                        	Header for all modules with common definition
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/app_conf.h                          	Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/app_entry.h                         	Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/hw_conf.h                           	Configuration file of the HW
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/main.h                              	Header for main.c module
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm_logging.h                       	Application header file for logging
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm32wbxx_hal_conf.h                	STM32 HAL configuration file.
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm32wbxx_it.h                      	Interrupt handlers header file
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/utilities_conf.h                    	Header for Configuration of utilities
   - Mac_802_15_4/Mac_802_15_4_Coordinator/STM32_WPAN/App/app_mac_802_15_4_process.h    	Header for Coordinator MAC 802.15.4 processing tasks
   - Mac_802_15_4/Mac_802_15_4_Coordinator/STM32_WPAN/App/app_mac_802_15_4.h            	Header for Coordinator MAC 802.15.4 Core Mac Device implementation
   
   
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/app_entry.c                             Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm32_lpm_if.c                          Low Power Manager Interface
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/hw_uart.c                               UART Driver
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/main.c                                  Main program
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm_logging.c                           Logging Implementation
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/stm32wbxx_it.c                          exceptions handler and peripherals interrupt service routine
   - Mac_802_15_4/Mac_802_15_4_Coordinator/Core/Inc/system_stm32wbxx.c                      CMSIS Cortex Device Peripheral Access Layer implementation
   - Mac_802_15_4/Mac_802_15_4_Coordinator/STM32_WPAN/App/app_mac_802_15_4_process.c    	Coordinator MAC 802.15.4 processing tasks implementation
   - Mac_802_15_4/Mac_802_15_4_Coordinator/STM32_WPAN/App/app_mac_802_15_4.c            	Coordinator MAC 802.15.4 Core Mac Device implementation
 

  
 
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

This application requests having the stm32wb5x_Mac_802_15_4_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


Minimum requirements for the demo:
- 1 STM32WB55xx-Nucleo board in MAC 802.15.4 Coordinator mode 
using current generated binary.
- 1 STM32WB55xx-Nucleo board in MAC 802.15.4 Node
(see MAC 802.14.4 Node example)

In order to make the program work, you must do the following: 
 - Connect 2 STM32WB55xx-Nucleo boards to your PC 
 
 COORDINATOR :
         - Open your preferred toolchain 
         - Rebuild all files and load your image into 
         COORDINATOR target memory 
         - The example run on Reset
 
 NODE :
         - Refers to  MAC 802.14.4 NODE example to build 
           and load generated image
     
To get the traces in real time, you can connect an HyperTerminal to the LPUART (CN10-35) Com Port.
 
 
 The Serial interface must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

  You can also use an MAC 802.15.4 other the air sniffer to spy MAC 802.15.4 frames exchanged between the devices.
 
You can define the channel by uncommenting the define DEMO_CHANNEL in app_mac_802_15_4.h and setting the channel 
between 11 and 26 (ONLY COORDINATOR SIDE).
  
**** START DEMO ****

By default, after a reset, both board starts. Coordinator must be started firstly to be able to answer
to the node association request (average 10 seconds).

This MAC 802.15.4 example is to demonstrate Point-to-Point communication using MAC components between 
a COORDINATOR and a NODE. 

Upon COORDINATOR start, the device initializes the MAC 802.15.4 and associates resources in non-beaconned
enable mode. 

MAC PIB is then configured as follow :

  - PanID 						 : 0xBEEF, can be define in app_conf.h, if define MAC_802_15_4_RANDOM_PAN_ID enable, the PANID is random
  - Coordinator Extended Address : depending to the device but begin by 00:80:E1
  - Coordinator Short Address    : 0x2000, can be define in app_mac_802_15_4.h
  - Poll Short Address           : 0x2001 - 0x2010, can be define in app_mac_802_15_4.h
  - Channel                      : take the lest noisy channel, can be define in app_mac_802_15_4.h
  - Beacon Payload               : 0x42,0x5A,0x48, can be define in app_conf.h

Once MAC PIB is initialized, Coordinator is started on least noisy channel with ED scan as an association capable coordinator.
At this stage the Coordinator is waiting for an association request. Blue LED (ID 1) is switch on once 
the coordinator is started.

On Node side, the MAC layer is initialized on startup. In this state, blue LED (ID 1) is switch on.

The Node device can recognize the coordinator device with the beacon payload define in app_conf.h on both devices.

Node issues an association request to the Coodinator to retrieve its short address. 
On in coming association request Coordinator's green LED (ID 2) is switch on.

Once the association between two devices is completed, Node's green LED (ID 2) is switch on to indicate the node 
is associated to the coordinator. 

The Node sends data to the coordinator that issues a Data Indication.
On both devices side, on data transmission red led (ID 3) blinked.

User can issue display all associated devices, by pressing the button 1 (SW1). 

User can issue check if device is not associated, by pressing the button 2 (SW2). Data transmission with ACK, if the node associated don't response, then the device is remove of associated device table, can be then observe using OTA sniffer,
UART logs and LED status. 

If you want to restart the demo, simply reset the NODE.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */