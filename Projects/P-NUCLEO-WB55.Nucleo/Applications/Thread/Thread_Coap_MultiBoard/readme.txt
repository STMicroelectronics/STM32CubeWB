/**
  @page Thread_Coap_MultiBoard application
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    Thread/Thread_Coap_MultiBoard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread_Coap_MultiBoard application
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to use Coap for sending message to multiple boards.

This application requires the use of several STM32WB55xx_Nucleo boards.
  Minimal: 2 boards.
  Maximum: 5 boards.

The purpose of this application is to create a small Thread Mesh network.
When the setup is correctly configured, a COAP request is automatically 
transferred continuously from one board to another one in the following order: 

    Board 1 -> Board 2 -> (...) -> Board n -> Board 1 -> Etc.. 

Example of use case using 4 boards running the Thread_Coap_MultiBoard application: 

Figure 1: 

T = To
  __________          __________         __________          __________      
  |Board 1 |          |Board 2 |         |Board 3 |          |Board 4 |   
  |        |--Coap--> |        |         |        |          |        |
  |        |          |        |         |        |          |        |
  ----------          ----------         ----------          ----------
                       Blue Led 
                       blinking
                       twice
 
T = To + 2 sec
  __________          __________          __________          __________      
  |Board 1 |          |Board 2 |          |Board 3 |          |Board 4 |   
  |        |          |        |--Coap--> |        |          |        |       
  |        |          |        |          |        |          |        |
  ----------          ----------          ----------          ----------
                                          Blue Led 
                                          blinking
                                          twice
 
T = To + 4 sec
  __________          __________          __________          __________      
  |Board 1 |          |Board 2 |          |Board 3 |          |Board 4 |   
  |        |          |        |          |        |--Coap--> |        |       
  |        |          |        |          |        |          |        |
  ----------          ----------          ----------          ----------
                                                               Blue Led 
                                                               blinking
                                                               twice
 
T = To + 6 sec
  __________          __________         __________          __________      
  |Board 1 |          |Board 2 |         |Board 3 |          |Board 4 |   
  |        |<-Coap-   |        |         |        |          |        |--
  |        |       |  |        |         |        |          |        |  |
  ----------       |  ----------         ----------          ----------  |
   Blue Led        |                                                     |
   blinking         -----------------------------------------------------
   twice


T = To + 8 sec
  __________          __________         __________          __________      
  |Board 1 |          |Board 2 |         |Board 3 |          |Board 4 |   
  |        |--Coap--> |        |         |        |          |        |
  |        |          |        |         |        |          |        |
  ----------          ----------         ----------          ----------
                       Blue Led 
                       blinking
                       twice


To each board is associated a specific IPv6 address: 
 "fdde:ad00:beef:0:442f:ade1:3fc:1f3a" for Board number 1
 "fdde:ad00:beef:0:442f:ade1:3fc:1f3b" for Board number 2
 "fdde:ad00:beef:0:442f:ade1:3fc:1f3c" for Board number 3 if present
 "fdde:ad00:beef:0:442f:ade1:3fc:1f3d" for Board number 4 if present
 "fdde:ad00:beef:0:442f:ade1:3fc:1f3e" for Board number 5 if present


    
@par Keywords
  
COAP,THREAD

@par Directory contents 
  
  - Thread/Thread_Coap_MultiBoard/Core/Inc/app_common.h           Header for all modules with common definition
  - Thread/Thread_Coap_MultiBoard/Core/Inc/app_conf.h             Parameters configuration file of the application
  - Thread/Thread_Coap_MultiBoard/STM32_WPAN/App/app_thread.h     Header for app_thread.c module 
  - Thread/Thread_Coap_MultiBoard/Core/Inc/hw_conf.h              Configuration file of the HW 
  - Thread/Thread_Coap_MultiBoard/Core/Inc/main.h                 Application header file
  - Thread/Thread_Coap_MultiBoard/Core/Inc/stm_logging.h          Application header for logging
  - Thread/Thread_Coap_MultiBoard/Core/Inc/stm32wbxx_hal_conf.h   HAL configuration file
  - Thread/Thread_Coap_MultiBoard/Core/Src/stm32wbxx_it.h         Interrupt header file
  - Thread/Thread_Coap_MultiBoard/Core/Inc/utilities_conf.h       Configuration file of the utilities
  - Thread/Thread_Coap_MultiBoard/Core/Src/app_entry.c            Initialization of the application
  - Thread/Thread_Coap_MultiBoard/STM32_WPAN/App/app_thread.c     Thread application implementation
  - Thread/Thread_Coap_MultiBoard/STM32_WPAN/Target/hw_ipcc.c     IPCC Driver
  - Thread/Thread_Coap_MultiBoard/Core/Src/stm32_lpm_if.c         Low Power Manager Interface
  - Thread/Thread_Coap_MultiBoard/Core/Src/hw_uart.c              UART driver
  - Thread/Thread_Coap_MultiBoard/Core/Src/main.c                 Main program
  - Thread/Thread_Coap_MultiBoard/Core/Src/stm_logging.c          Logging module for traces
  - Thread/Thread_Coap_MultiBoard/Core/Src/stm32xx_it.c           Interrupt handlers
  - Thread/Thread_Coap_MultiBoard/Core/Src/system_stm32wbxx.c     stm32wbxx system source file

 
@par Hardware and Software environment

  - This example runs on STM32WB55xx_Nucleo devices.
  
  - This example has been tested with STMicroelectronics STM32WB55xx_Nucleo  
    boards and can be easily tailored to any other supported device 
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

This application requests having the stm32wb5x_Thread_FTD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


In order to make the program work, you must do the following: 
 - Connect from 2 up to 5 STM32WB55xx_Nucleo boards to your PC 
 - Open your preferred toolchain
 - Rebuild all files of Thread/Thread_Coap_MultiBoard application 
   and load your image into the target memory
 - Run the application 

Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

  1)  Starting the application: 
   ============================
  At start, once all boards have been powered up, a device will act as 'leader' while all other boards 
  will act as 'router' or 'child'. 
   -The device which acts as 'leader' must have his green LED powered on.
   -The devices which act as 'router' or 'child' must have their red LED powered on.
  There should be only one 'leader' in the Thread network.
  Once all boards have been powered on, the user needs to configure the different boards.

  2)  Configuration of the different boards: 
  ==========================================
  To setup the system, the user needs to associate an ID to each board (ID 1 for board 1, ID 2 for board 2, etc..)
  This is done thanks to the usage of SW1 push button, following the sequence below: 

  -Step 2a:
  Wait until one board has reached the state 'leader' (green Led), and all other boards have reached
  the state 'router' or 'child' red.

  -Step 2b:
  It is now requested to associate an unique ID to each board. The ID of the 'leader' must always be equal to the 
  number of boards available inside the Thread network.
  Suppose you have 4 boards inside your network, you must associate the ID number 4 to the board acting as 'leader'.
  ID number 1 and ID number 2 are then respectively associated to the two other boards which are acting as 'router' or
  'child'. To associate ID 4 to the 'leader' board, press 4 times on the SW1 button. The first time you press on SW1, 
  you should see the 'blue Led' blinking once. The second time you press on SW1, you should see the 'blue Led' blinking twice.
  The third time you pressed on SW1,you should see the 'blue Led' blinking three times. The fourth time you press on SW1,
  you should see the 'blue Led' blinking 4 times. At this stage, in this example, the ID 4 is now associated to the 'leader' board. 

 -Step 2c:
  Following the same rules as in Step2b, (in case there are 4 boards inside the network), it is now requested 
  to associate ID 1 to board 1, ID 2 to board 2, etc...
  To associate ID 1 on board 1, you must press one time on SW1 push button of board 1
  To associate ID 2 on Board 2, you must press two times on SW1 push button of board 2
  To associate ID 3 on Board 3, you must press three times on SW1 push button of board 3

  3)  Start sending COAP requests: 
  ================================
  Start the sending of the Coap requests by pressing on SW3 on the 'leader' device.
  At this stage, you should see the COAP request being propagated from one board to another one as shown on the Figure 1 on top 
  of this readme.txt.

  The COAP request has the following characteristics: 
     -Type: Confirmable/Unicast
     -Command: PUT

To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
For the traces, the UART must be configured as follows: 
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

 