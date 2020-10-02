/**
  @page Zigbee_OnOff_Router_NVM application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_OnOff_Router_NVM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee OnOff Cluster application as a client  
  *          using a centralized network. 
  ******************************************************************************
  *
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

How to use OnOff cluster and the persistent data on a centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the OnOff cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through the push button 
in order to make the LED toggling.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Coord_NVM
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Router_NVM
    

                  Device 1                                       Device 2
        
                 ---------                                      ---------
                 |       |       ZbZclOnOffClientToggleReq      |       |
      PushB SW1=>|Client | -----------------------------------> |Server | =>LED3
       LED3   <= |Server | <----------------------------------- |Client | <= PushB SW1
                 |       |       ZbZclOnOffClientToggleReq      |       |
      PushB SW2=>|       |                                      |       |         
      PushB SW3=>|       |                                      |       |
                 --------                                       ---------
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_OnOff_Coord_NVM application (Device2 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      
      Start the second board. This board is configured as Zigbee router with persistence and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
            
  b)  At this stage the second board tries to start from persistence this leads to two choices:

        - Persistent Data read from NVM are valid -> the router takes back is role in the network and restore the on/off cluster
          attribute saved in NVM (LED3 on or off). GREEN LED2 is ON indicating a restart form persistence.
          
        - No Persistent data found or wrong data,a fresh start is performed, the Zigbee network is automatically created 
          and BLUE LED (LED1) is ON on all devices.
        
          
     It is now possible to send OnOff Cluster commands from the client to the server in multicast mode 
     by pressing on the SW1 push button. 
     You must see the LED3 toggling on the server side. (this for all devices). Persistence Data are updated in NVM. Stack and cluster 
     attribute (LED3 state).
     
   c)  If the user power cycle the Device 1, Persistent Data are read back and cluster attribute (LED3 state) restored.
       The device start from persistence and GREEN LED LED2 is on.
       Pushing SW1 on any boards toggle LED3.
   
   d)  Push Button SW2 is used to delete NVM (fresh start will be done on next start up ,reproduce a))
   
   e)  Push Button SW3 is used to run a NVM diagnostics (flash test)

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
*/
