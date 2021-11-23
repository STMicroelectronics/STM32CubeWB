/**
  @page Thread_RCP_Cli_Cmd application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_RCP_Cli_Cmd/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread RCP Cli command application
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

How to control the Thread stack via Cli commands and trigger an automatic commissioning joiner sequence from a press button.

The Cli (Command Line Interface) commands are sent via an UART from an HyperTerminal (PC)
to the STM32WB55RG_Nucleo Board.

   ___________________________ 
  |                           |     
  |           ________________|                       _____________________________________
  |          |USART1          |                      |     HyperTerminal 1                 |
  |          |                |                      |=> Used to ctrl the stack via Cli Cmd|
  |          |                |                      |                                     |
  |          |                |                      |                                     |
  |          |                |______________________|                                     |
  |          |                |______________________| ST_Link virtual port                |
  |          |                |     ST_Link Cable    |                                     |             
  |          |                |                      |                                     |
  |          |                |                      |                                     |
  |          |                |                      |                                     |             
  |          |________________|                      |_____________________________________|          
  |                           |                       
  |           _______________ |                       ______________________________________
  |          |LPUART1         |                      |     HyperTerminal 2 (Optional)       |
  |          |                |                      |=> Used to display traces             |   
  |          |                |                      |                                      |
  |          |CN10(Pin37) TX  |______________________|RX                                    | 
  |          |                |                      |                                      |
  |          |                |     RS232 Cable      |                                      |             
  |          |                |                      |                                      |
  |          |CN10(Pin35 )RX  |______________________|TX                                    |          
  |          |                |                      |                                      |           
  |          |________________|                      |______________________________________|          
  |                           |                                                
  |                           |                      
  |                           |                      
  |_STM32WB55RG_Nucleo________|                      



- Through the Cli commands, it is possible to control and configure the Thread 
stack.
- On the HyperTerminal, the user can type the command 'help' in order to display the list of 
all available cli commands. Additional information can be found looking at the OpenThread 
web site: https://openthread.io/guides/

- As an example, the user can play the following scenario in order to properly initiate the Thread
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

- When running on two STM32WB55RG_Nucleo boards the same Thread_RCP_Cli_Cmd application, and by playing the 
 same scenario as described above on both boards, the first board should reach the state 'leader' 
 while the second one should reach the state 'child'. 

- When the Thread device changes state, a specific LED color is being displayed.
  LED2 ON (Green) means that the device is in "Leader" mode.
  LED3 ON (Red) means that the device is in "Router" or "Child" mode.
  LED2 OFF and LED3 OFF means that the device is disabled or detached.
  
- When all LEDS are blinking, it means that a fatal error has been detected.

     
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

THREAD

@par Directory contents 
  
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_RCP_Cli_Cmd/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/system_infra.h          System infrastructure header file
  - Thread/Thread_RCP_Cli_Cmd/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_RCP_Cli_Cmd/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_RCP_Cli_Cmd/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/main.c                  Main program
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_RCP_Cli_Cmd/Core/Src/system_stm32wbxx.c      stm32wbxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and dongle) 
  
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

This application requests having the stm32wb5x_Thread_FTD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


In order to make the program work, you must do the following: 
 - Connect an STM32WB55xx_Nucleo board to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image
 - Connect one HyperTerminal (ST_Link) to be able to control the Thread stack via Cli commands 
 - Connect a second HyperTerminal as described below to get the traces (optional)
 - Run the example and use the HyperTerminal to interact with the Board via the Cli commands

Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none


@par How to use this application as a joiner which triggers a commissioning process thanks to ST Border Router.

This Application is providing a One press button feature to start joiner commissioning process.
Please Refer to RCP application readme to get the big picture of the commissioning process.
Once the joiner device (running RCP_Cli_Cmd application) is ready to be commissioned into a Thread Network (thanks to ST Border Router),
just press Button SW1.
The Blue LED starts blinking at a slow pace, indicating that the commissioning is in progress.
If the commissioning process end with a success, the Blue LED gets solid indicating that the node as joined the network.
If the commissioning process end with a failure, the Blue LED gets a fast blinking pace then switch off.


 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */