/**
  @page Thread_Cli_Cmd application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Cli_Cmd/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread Cli command application
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

How to control the Thread stack via Cli commands.
The Cli (Command Line Interface) commands are sent via USB from STM32WB55xx_Usb_Dongle 
to an HyperTerminal (PC).

   _________________________                         ________________________________
  |                         |                       |                                | 
  |      ________           |                       |    _________________________   |
  |     |        |          |                       |   | HyperTerminal 1         |  |
  |     |        |          |                       |   |=> Used to control the   |  |
  |     |        |          |     Cli commands      |   |   stack via Cli commands|  |                    |
  |     |        |          |                       |   |                         |  |
  |     |        |    USB   | _____________________ |   |                         |  |
  |     |        |          |      USB Connector    |   |                         |  |
  |     |        |          |                       |   |_________________________|  |           
  |     |________|          |                       |                                |
  |                         |                       |                                |
  |                         |                       |                                |             
  |                         |                       |                                |          
  |                         |                       |________________PC______________|
  |_STM32WB55xx_Usb_Dongle _|                      



- Through the Cli commands, it is possible to control and configure the Thread 
stack.
- On the HyperTerminal, the user can type the command 'help' in order to display the list of 
all available cli commands. Additional information can be found looking to the OpenThread 
web site :https://openThread.io/guides/

- As example, the user can play the following scenario in order to properly initiate the Thread
mesh network by typing the following commands:

  >panid 0x1234
   Done
  >channel 12
   Done
  >ifconfig up
   Done
  >thread start
   Done

At this point, the user can check the state of its device by using the cli command 'state' 
   >state
   Leader

- When running on two STM32WB55xx_Usb_Dongle boards the same Thread_Cli_Cmd application, and by 
  playing the same scenario as described above on both boards, the first board should reach the 
  state 'leader'while the second one should reach the state 'child'. 

- When the Thread device changes state, a specific LED color is being displayed.
  LED2 ON (Green) means that the device is in "Leader" mode.
  LED3 ON (Red) means that the device is in "Router" or "Child" mode.
  LED2 OFF and LED3 OFF means that the device is disabled or detached.
  
- When all LEDS are blinking, it means that a fatal error has been detected.

     
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

@par Directory contents 
  
  - Thread/Thread_Cli_Cmd/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_Cli_Cmd/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_Cli_Cmd/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_Cli_Cmd/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Cli_Cmd/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_Cli_Cmd/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Cli_Cmd/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Cli_Cmd/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Cli_Cmd/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Cli_Cmd/Core/Inc/system_infra.h          System infrastructure header file
  - Thread/Thread_Cli_Cmd/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_Cli_Cmd/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_Cli_Cmd/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Cli_Cmd/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Cli_Cmd/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_Cli_Cmd/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_Cli_Cmd/Core/Src/main.c                  Main program
  - Thread/Thread_Cli_Cmd/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Cli_Cmd/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Cli_Cmd/Core/Src/system_stm32wbxx.c      stm32wbxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55xx_Usb_Dongle 
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
 - Plug your STM32WB55xx_Usb_Dongle board to your PC 
 - Rebuild all files and load your image OR use the Thread_Cli_Cmd_reference.hex from Binary directory
 - Run the example and use the HyperTerminal to interact with the Board via the Cli commands


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */