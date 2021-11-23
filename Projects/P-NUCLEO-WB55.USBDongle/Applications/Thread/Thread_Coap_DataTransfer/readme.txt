/**
  @page Thread_Coap_DataTransfer application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Coap_DataTransfer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread_Coap_DataTransfer application
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

How to transfer large blocks of data through the CoAP messaging protocol.

This application requires the use of at least two STM32WB55xx_Dongle boards.It demonstrates the 
ability to transfer large blocks of data through the CoAP messaging protocol. This application could be 
further developed into an Over The Air Firmware Update. However at the current state it stands as a 
Proof of Concept for the ability of transferring large blocks of data through the network. Namely
5 blocks of data of 128bits length.

In a Thread network, IPv6 addressing is split into 3 modes and 3 scopes. 
Multicast, Unicast and Anycast modes: 
  Multicast is used to communicate messages to multiple devices at once depending on the scope used in the address.
Unicast is used to communicate messages to a specific node in the network.
Anycast is used when the thread interface's routing locator is unknown.

The scope determines the level of network access to be targeted :
Link-Local scope : all interfaces are reachable through a single radio transmission hop.
Mesh-Local scope : all interfaces reachable from within the same thread network.
Global scope     : all interfaces reachable from within and outside the same thread network.

In our application, we will use the MEsh-local scope and multicast addressing types to probe the 
mesh-local IP addresses of the child devices to which the File would be transferred. 
 nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application, which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device (child mode)

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) and 
the other one will be in Child mode (Red LED3 ON) Once the child mode is established for 
one of the devices, it starts the provisioning procedure in multicast mode in order to probe 
the IP address of the leader device. Then this is used to start the File transfer procedure
in unicast mode, which success is marked by the lightning of the blue LED.
 

  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |  Provisioning enabled() |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |  Provisioning Request   |                       |                         |
  |       send()            |======> COAP =========>|-------------            |
  |                         |Resource "provisioning"|             |           |
  |                         |Mode : Multicast       |             v           |
  |                         |Type : Non-Confirmable |    Provisioning Request |
  |                         |Code : Get             |         Handler()       |
  |                         |Payload: empty         |             |           |
  |                         |                       |             v           |
  |                         |                       | Provisioning Response   |
  |                         |                       |         send()          |
  |                         |                       |             |           |
  |            -------------|<===== COAP ========== |<------------            |
  |            |            |Resource "provisioning"|                         |
  |            v            |Mode : Multicast       |                         |
  |  Provisioning Response  |Type : Non Confirmable |                         |
  |          Handler()      |Code : Put             |                         |
  |            |            |Payload : MeshLocal EUI|                         |
  |            |            |                       |                         |
  |            |            |                       |                         |
  |            v            |                       |                         |
  | ---->Get_NextBuffer()   |                       |                         |
  ||           |            |                       |                         |
  ||           v            |                       |                         |
  || Unicast_Data_Send()    |                       |                         |
  ||                        |                       |                         |
  ||           |            |                       |                         |
  ||           v            |                       |                         |
  ||            ----------->|=====>CoAP============>|-------------            |
  ||                        |Resource : "File"      |             |           |
  ||                        |Mode     : Unicast     |             v           |
  ||                        |Type     : Confirmable | Data_Request_Handler()  |
  ||                        |Code     : Put         |                         |
  ||                        |Payload  : Buffer[]    |             |           |
  ||                        |                       |             v           |
  ||                        |                       | Check_Message_Validity  |
  ||                        |                       |             |           |
  ||                        |                       |             v           |
  ||                        |                       |   Data_Response_Send()  |
  ||                        |                       |                         |
  ||                        |                       |             |           |
  || Data_Response_Handler()|<=====CoAP=============|<------------            |
  ||                        |Resource: "File"       |                         |
  ||         |              |Mode    : Unicast      |                         |
  ||         |              |Type    : Acknowledgment                         |
  ||         |              |Code    : Changed      |                         |
  ||         |              |Payload : empty        |                         |
  ||         v              |                       |                         |
  ||         /\             |                       |                         |
  ||        /  \            |                       |                         |
  ||   Yes /    \           |                       |                         |
  | <---- /ID <5?\          |                       |                         |
  |       \      /          |                       |                         |
  |        \    /           |                       |                         |
  |         \  /            |                       |                         |
  |          \/             |                       |                         |
  |           | NO          |                       |                         |
  |           v             |                       |                         |
  |  Blue Led ON            |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  ---------------------------                       ---------------------------
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  | LED : Red               |                       | LED : Green             |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,THREAD

@par Directory contents 
  
  - Thread/Thread_Coap_DataTransfer/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_Coap_DataTransfer/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_Coap_DataTransfer/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_Coap_DataTransfer/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Coap_DataTransfer/Core/Inc/data_transfer.h         Array definition for Data Transfer application
  - Thread/Thread_Coap_DataTransfer/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_Coap_DataTransfer/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Coap_DataTransfer/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Coap_DataTransfer/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Coap_DataTransfer/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Coap_DataTransfer/Core/Inc/system_infra.h          System infrastructure header file
  - Thread/Thread_Coap_DataTransfer/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_Coap_DataTransfer/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_Coap_DataTransfer/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Coap_DataTransfer/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Coap_DataTransfer/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_Coap_DataTransfer/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_Coap_DataTransfer/Core/Src/main.c                  Main program
  - Thread/Thread_Coap_DataTransfer/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Coap_DataTransfer/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Coap_DataTransfer/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and dongle).
  
  - This example has been tested with an STMicroelectronics STM32WB55RG-Dongle 
    board and can be easily tailored to any other supported device 
    and development board.
    
  
@par How to use it ? 

This application requests having the stm32wb5x_Thread_FTD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


In order to make the program work, you must do the following: 
 - Connect 2 STM32WB55xx_Dongle boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example on one board first wait until it switches to Leader Role
    (indicated through the lightning of a green LED)
 - Run the example on the second board and wait until it switches to Child Role
    (indicated through the lightning of the Red LED)
 - After a few seconds, if everything goes right, a blue LED should be lit indicating the end of the File transfer 
 - If the above step didn't go as planned, you should look through the traces to locate the source of the error.
    
 Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 