/**
  @page Thread_Udp application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Udp/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread_Udp application
  ******************************************************************************
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

How to transfer data using UDP.

This application requires the use of at least two STM32WB55xx boards. 
It demonstrates the ability to transfer a block of data through UDP.
 
Nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application, which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device (child mode)

Test 1:
After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) and 
the other one will be in Child mode (Red LED3 ON).
Once the child mode is established for one of the devices, the user pushes SW1 button
to start a UDP sending procedure in multicast mode.
If the pattern is correctly received on the Leader side Blue LED toggles its previous
state (ON if it was OFF and OFF if it was ON).

Test 2:
Note: The same test can be performed from Leader to Child.
 
-Test 1
  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |  UDP open socket        |
  |                         |                       |  UDP bind to UDP_PORT   |
  |                         |                       |                         |
  |                         |<= Push Sw1            |                         |
  |                         |                       |                         |
  |     UDP send()          |======> UDP =========> |-------------            |
  |                         |UDP_PORT "1234"        |             |           |
  |                         |Address : ff02::1      |             |           |
  |                         |Payload: udpBuffer[]   |             |           |
  |                         |                       |             v           |
  |                         |                       |        UDP Receive ()   |
  |                         |                       |             |           |
  |                         |                       |             |           |
  |                         |                       |             v           |
  |                         |                       |        Blue Led TOGGLES |
  |                         |                       |                         |
  ---------------------------                       ---------------------------
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  | LED : Red               |                       | LED : Green             |
  |                         |                       |                         |
  |_________________________|                       |_________________________|


-Test 2
  ___________________________                       ___________________________
  |  Device 2               |                       | Device 1                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |  UDP open socket        |
  |                         |                       |  UDP bind to UDP_PORT   |
  |                         |                       |                         |
  |                         |<= Push Sw1            |                         |
  |                         |                       |                         |
  |     UDP send()          |======> UDP =========> |-------------            |
  |                         |UDP_PORT "1234"        |             |           |
  |                         |Address : ff02::1      |             |           |
  |                         |Payload: udpBuffer[]   |             |           |
  |                         |                       |             v           |
  |                         |                       |        UDP Receive ()   |
  |                         |                       |             |           |
  |                         |                       |             |           |
  |                         |                       |             v           |
  |                         |                       |        Blue Led TOGGLES |
  |                         |                       |                         |
  ---------------------------                       ---------------------------
  | Role : Leader           |                       | Role : Child            |
  |                         |                       |                         |
  | LED : Green             |                       |    LED : Red            |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,THREAD

@par Directory contents 
  
  - Thread/Thread_Udp/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_Udp/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_Udp/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_Udp/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Udp/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_Udp/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Udp/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Udp/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Udp/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Udp/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_Udp/Core/Inc/vcp_conf.h              Configuration file of Virtual Com Port
  - Thread/Thread_Udp/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_Udp/Core/Src/hw_timerserver.c        Timer Server based on RTC
  - Thread/Thread_Udp/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Udp/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Udp/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_Udp/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_Udp/Core/Src/main.c                  Main program
  - Thread/Thread_Udp/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Udp/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_Udp/Core/Src/stm32wbxx_hal_msp.c     MSP Initialization
  - Thread/Thread_Udp/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Udp/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and dongle)
  
  - This example has been tested with STMicroelectronics STM32WB55RG_Nucleo  
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
 - Connect 2 STM32WB55xx boards to your PC (can be Nucleo and/or dongle)
 - Open your preferred toolchain
 - Rebuild all files of Thread/Thread_Udp application 
   and load your image into the target memory
 - Run the example on one board first wait until it switches to Leader Role
    (indicated through the lightning of a green LED)
 - Run the example on the second board and wait until it switches to Child Role
    (indicated through the lightning of the Red LED)
 - After a few seconds, if everything goes right, a blue LED should be lit indicating the end of the File transfer 
 - If the above step didn't go as planned, you should look through the traces to locate the source of the error.

Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.
  
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
 
For the traces, the UART must be configured as follows: 
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 