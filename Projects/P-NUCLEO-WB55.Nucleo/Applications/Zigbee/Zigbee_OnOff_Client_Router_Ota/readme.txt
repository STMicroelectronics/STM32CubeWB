/**
  @page Zigbee_OnOff_Client_Router_Ota application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_OnOff_Client_Router_Ota/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee OnOff Cluster OTA application as a client  
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

How to use an updated OnOff cluster Zigbee application previously downloaded via OTA.

@note This application is not supported by CubeMx but has been copied from the project Zigbee_OnOff_Client_Router generated
      by CubeMx with some modifications in order to be able to download it with the Zigbee_OTA_Client_Router application.
      The steps to be done to move from the Zigbee_OnOff_Client_Router application to the Zigbee_OnOff_Client_Router_Ota application are :
      - Copy the full folder Zigbee_OnOff_Client_Router.
      - Replace the linker file stm32wb55xx_flash_cm4.icf by stm32wb55xx_flash_cm4_ota.icf ( this adds the placement
        of the two sections TAG_OTA_START and TAG_OTA_END ).
      - Remove the update of SCB->VTOR in the file system_stm32wbxx.c ( The VTOR is already set by the Thread_Ota application
        and shall not be changed to a different value).
      - SW3 is mapped. When pushed forces a reboot on Zigbee_OTA_Client_Router application (so that a new FW OTA update can be done).


This application is to be used when playing with the Zigbee_OTA_Client_Router and Zigbee_OTA_Server_Coord applications.
When running the OTA example (based on Zigbee_OTA_Client_Router and Zigbee_OTA_Server_Coord applications), this application
is the one that can be transferred from the Coordinator to the Router. 


The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the OnOff cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through the push button 
in order to make the LED toggling.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Server_Coord or any other application with a running ZCL OnOff server
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Client_Router
    

In order for the OTA application to be able to reconnect to the coordinator network after an upgrade, the stack parameters 
have to be persisted. This is why Zigbee_OnOff_Client_Router_Ota is using data persistence mechanism 
(with both Flash storage and RAM cache). Stack parameters are saved with the Zigbee_OTA_Client_Router application 
(its using persistence too) while processing to the update. 

Therefore, after a successful application firmware upgrade, the client router can rejoin the network.
As with the persistent data demo applications set, the NVM can be cleared by pressing push button SW2 (client side).
    

              Device 1                                      Device 2
        
             ---------                                      ---------
             |       |       ZbZclOnOffClientToggleReq      |       |
  PushB SW1=>|Client | -----------------------------------> |Server | =>LED
             |       |                                      |       |
             |       |<--                                   |       |
  PushB Sw2=>|       |   |   /* Clear NVM data              |       |
             |       |---                                   |       |
             |       |                                      |       |
             |       |<--                                   |       |
  PushB SW3=>|       |   | /* Reboot on OTA app */          |       |
             |       |---                                   |       |
             ---------                                      ---------
  
 To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network. It must be a device that is
      hosting a ZCL OnOff server (Device2 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      
      Start the second board. This board is configured as Zigbee router with persistence and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
            
  b)  At this stage the second board tries to start from persistence this leads to two choices:

        - Persistence Data read from NVM are valid -> the router takes back is role in the network. 
          GREEN LED2 is ON indicating a restart form persistence.
          
        - No persitence data found or wrong data,a fresh start is performed, the Zigbee network is automatically created 
          and BLUE LED (LED1) is ON on all devices.
        
          
     It is now possible to send OnOff Cluster commands from the client to the server in multicast mode 
     by pressing on the SW1 push button. 
     You must see the LED3 toggling on the server side. (this for all devices). Persistence Data are updated in NVM.
     (Stack parameters).
     
   c)  If the user power cycle the Device 1, Persistence Data are read back and cluster attribute (LED3 state) restored.
       The device start from persistence and GREEN LED LED2 is on.
   
   d)  Push Button SW2 is used to delete NVM (fresh start will be done on next start up ,reproduce a)


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

