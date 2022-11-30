/**
  @page Zigbee_TempMeas_Server_Coord application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_TempMeas_Server_Coord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee TempMeas Cluster application as a client  
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

How to use the Device Temperature cluster on a device acting as a Server with Coordinator role on a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the TempMeas cluster. Once the Zigbee mesh 
network is created, the server store in its attribute the temperature sensor acquisition on a 500ms based period. 
The temperature is displayed on the LCD and green LED toggle at each acquisition. Sensor min/max/tolerance are
displayed as well.
On the client device the server temperature is reported and displayed automatically every 10s (LED toggle in blue)
On the client the user can start a 500ms read over the air of the server temperature pushing button1 (LED toggle in purple
on each read). To stop the 500ms read the user push the button2.  

For this application it is requested to have:

- 1 STM32WB5MMxx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_TempMeas_Server_Coord
    
- 1 or more STM32WB5MMxx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_TempMeas_Client_Router
    

              Device 1                                      Device 2
        
             ---------                                      ---------
             |       |       ZbZclReadReq (every 500ms)	    |       |
    PushSW1=>|Client | -----------------------------------> |Server | =>LCD Local sensor temp 
  LCD sever<=|       | <-----ZbZclReadRsp------------------ |       |   500ms duty cycle
  temp       |       |                                      |       |
             |       | <---ZclReport (every 10s)------------|       |
    PushSW2=>|       |                                      |       |   500ms duty cycle
    (stop)   |       |                                      |       |
              --------                                      ---------
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_TempMeas_Server_Coord application (Device2 in the above diagram). 
      Wait for the message 'Join OK'  being displayed on the LCD. 
      Start the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created Join OK is displayed on both devices.
      it is now possible to receive a 10s duty cycle report form Server (blue LED toggling) 
      by pressing on the SW2 (B2) push button. 
      You start a 500ms duty cycle send a read temp request to server (purple LED toggling)  
      by pressing on the SW1 (B1) push button.
      You stop the 1s duty cycle read. (reporting is still running though)  


 

@par Keywords

Zigbee
 
@par Hardware and Software environment

  - This example runs on STM32WB5MMxx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB5MM-DK 
    board and can be easily tailored to any other supported device 
    and development board.
    
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

