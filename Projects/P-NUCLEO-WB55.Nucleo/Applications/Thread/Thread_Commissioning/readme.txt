/**
  @page Thread_Commissioning application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_Commissioning/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread Commissioning application
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

How to use Thread commissioning process.

This application shows a device being able to distribute its parameters 
(channel, panid, etc..) to another device thanks to the commissioning.

This application requires two STM32WB55xx_Nucleo boards. One device will act as commissioner.
The other device will act as joiner. In this application, the commissioner will accept a 
newcomer to reach a Thread network.



@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_Commissioning/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_Commissioning/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_Commissioning/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_Commissioning/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_Commissioning/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_Commissioning/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_Commissioning/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_Commissioning/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_Commissioning/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_Commissioning/Core/Inc/system_infra.h          System infrastructure header file
  - Thread/Thread_Commissioning/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_Commissioning/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_Commissioning/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_Commissioning/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_Commissioning/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_Commissioning/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_Commissioning/Core/Src/main.c                  Main program
  - Thread/Thread_Commissioning/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_Commissioning/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_Commissioning/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - Thread/Thread_Commissioning/Core/Src/stm32wbxx_hal_msp.c     MSP used to handle the Timer

@par Hardware and Software environment

  - This application uses two STM32WB55xx devices (Nucleo board and Dongle).
  
  - This example has been tested with an STMicroelectronics STM32WB55 Nucleo or Dongle 
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
 - Connect 2 STM32WB55xx boards to your PC 
 - Open your preferred toolchain
 - Rebuild all files of Thread/Thread_Commissioning application 
   and load your image into the target memory
 - Run the application 

Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

 Step 1 : At start both devices are not attached to any Thread network. LED1 (blue) is 
          toggling slowly waiting for a user action. Both devices are in 'disabled' mode.

 Step 2 : On one device (Device1) , press on the SW1 push button. Following this action, Device1 will
          reach the 'leader' state inside the Thread network after a few seconds and act as commissioner.
          LED2 (green) should light on. 

          Note : Device1 behaves as if the user had typed the following Cli commands once it has reached
                 the 'leader' state.
                  > commissioner start
                  > commissioner joiner add * MY_PASSWD
                 The * wildcard allows any Joiner with the specified Joiner Credential to commission onto the network. 
                 The Joiner Credential is used (along with the Extended PAN ID and Network Name) to generate the 
                 Pre-Shared Key for the Device (PSKd). The PSKd is then used to authenticate a device during Thread 
                 commissioning.

 Step 3 : On the other device (Device2), press on the SW3 push button. Following this action, Device2 will
          act as joiner using the same credential as specified on the commissioner.
           
          Note : Device2 behaves as if the user had typed the following Cli command: 
                  > joiner start MY_PASSWD

 Step4 : The commissioning is now proceeding. This step can take more than 100 seconds. LED1 (blue) is 
         toggling quickly on Device2.

 Step5 : Once the commissioning is completed, the LED1 (blue) stays 'On' on both devices and the Thread network 
         is now established. One device acts as a Leader (Green LED on) and the other device acts as a Child (Red LED on)


 Remarks :
 If you want to control this application, you can directly send and
 retrieve Cli commands connecting an HyperTerminal with the ST_Link cable.
 (Refer to the Thread_Cli_Cmd application for more details) 
    
To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port.
 
 For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */