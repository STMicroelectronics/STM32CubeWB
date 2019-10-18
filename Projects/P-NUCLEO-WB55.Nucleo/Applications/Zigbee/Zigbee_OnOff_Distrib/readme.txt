/**
  @page Zigbee_OnOff_Distrib application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_OnOff_Distrib/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee OnOff Cluster application using a 
  *          distributed network. 
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

How to use OnOff cluster on a Distributed Zigbee network.

The purpose of this application is to show how to create a Zigbee distributed network, and 
how to communicate from one node to another one using the OnOff cluster. Once the Zigbee mesh 
network is created, the user can send requests to the different boards through the push buttons 
in order to make the LED toggling.

Each device acts as a client and a server at the same time, making the communication between the 
different boards symmetrical (bi-directorial)


              Device 1                                      Device 2
        
             ---------                                      ---------
             |       |         ZbZclOnOffClientToggleReq    |       |
      PushB=>|Client | -----------------------------------> |Server | =>LED
             |       |                                      |       |
             |       |                                      |       |
             |       |         ZbZclOnOffClientToggleReq    |       |
      LED <= |Server | <------------------------------------|Client |<=PushB
             |       |                                      |       |
              --------                                      ---------


This application requires at minimum two STM32WB55xx nucleo boards, but you can use up four boards 
when using configuration A and B. When using configuration C, only two boards are requested.

This application requests having the stm32wb5x_zigbee_full_fw.bin binary flashed on the wireless coprocessor 
and the correct Zigbee_OnOff_Distrib.bin binary flashed on the application processor. 

  
To setup the application :

  a)  Open the project, build it and load your generated application on all your STM32WB devices.
  
 To run the application :

  a)  Start the first board, wait for four seconds. Start the second board, wait for four seconds. Do the same 
      for the other boards if applicable.
      Note : In order to create correctly the zigbee network, it is important to NOT switch on all the boards 
             at the same time. 
      
  b)  At this stage, the Zigbee network is automatically created and it is possible to send OnOffCluster commands 
      in multicast mode by pressing on the SW1 or SW2 push button. 
      Press on SW1 (resp SW2). You must see the LED1 (resp LED2) toggling on all board connected to this 
      newly created Zigbee network.


              ----------             -----------
              |Device 1 |............|Device 2 |           
              |(distr.) |            |(distr.) |
              -----------            -----------
                  .                       .
                  .                       .
                  .                       .
             -----------                  .
             |Device 3 |...................      
             |(distr.) |
             -----------

 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board) 
  
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

=> Loading of the stm32wb5x_zigbee_full_fw.bin binary

  This application requests having the stm32wb5x_zigbee_full_fw.bin binary flashed on the Wireless Coprocessor.
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

