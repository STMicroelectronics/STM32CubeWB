/**
  @page Zigbee_OnOff_Client_Router_ThreadX application
  
  @verbatim
  ******************************************************************************
  * @file    Zigbee/Zigbee_OnOff_Client_Router_ThreadX/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Zigbee OnOff Cluster application as a client  
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

How to use the OnOff cluster on a device acting as a Client with Router role within a Centralized Zigbee network.

The purpose of this application is to show how to create a Zigbee centralized network, and 
how to communicate from one node to another one using the OnOff cluster. Once the Zigbee mesh 
network is created, the user can send requests from the client to the server through the push button 
in order to make the LED toggling.

For this application it is requested to have:

- 1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Server_Coord
    
- 1 or more STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Zigbee_FFD_fw.bin
    - application : Zigbee_OnOff_Client_Router_ThreadX
    

              Device 1                                      Device 2
        
             ---------                                      ---------
             |       |       ZbZclOnOffClientToggleReq      |       |
      PushB=>|Client | -----------------------------------> |Server | =>LED
             |       |                                      |       |
             |       |                                      |       |
              --------                                      ---------
  
To setup the application :

  a)  Open the project, build it and load your generated application on your STM32WB devices.
  
 To run the application :

  a)  Start the first board. It must be the coordinator of the Zigbee network so in this demo application it is
      the device running Zigbee_OnOff_Server_Coord application (Device2 in the above diagram). 
      Wait for the Blue LED (LED1) ON. 
      Start the second board. This board is configured as Zigbee router and will attached to the network created 
      by the coordinator. Do the same for the other boards if applicable.
      
  b)  At this stage, the Zigbee network is automatically created and BLUE LED (LED1) is ON on all devices.
      it is now possible to send OnOff Cluster commands from the client to the server in multicast mode 
      by pressing on the SW1 push button. 
      You must see the RED LED (LED3) toggling on the server side.


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


=> Trace for TheadX :
	- On the IAR project :
		-  	A new directory (debug_traces) is added to integrate all 'tx_trace*.c') files.
		-	A new 'macro' file named 'TraceForThreadX.mac' is added on EWARM directory.
		-	This new 'macro' file is defined on the 'option .. debugger .. Setup .. Setup macros'.
	- On the 'tx_user.h' file, the #define 'TX_ENABLE_EVENT_TRACE' is uncommented.
	- On the 'app_thread.c' 
		-	Definition of a 'Trace' Buffer (named 'cDebugTraceX') that can contains 512 event described by 64 bytes).
		-	Initialisation of trace by 'tx_trace_enable()' function.
		-	ThreadX trace all events concerning Thread/Semaphore/Mutex. User can add a 'manual' event with the function 'tx_trace_user_event_insert()'
	-To obtains the Trace :
		-	Pause the code.
		-	On the windows 'Quick Watch', write 'save_tracex()' on command line. 
			The macro save the trace on file 'Trace_xx.trx' on the directory 'EWARM'.
		-	Open the software 'Azure RTOS TraceX' that you can obtains on 'https://apps.microsoft.com/store/detail/azure-rtos-tracex/9NF1LFD5XXG3?hl=fr-fr&gl=FR' 
			or on the Windows Store.
		-	Load the '*.trx' file.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>

