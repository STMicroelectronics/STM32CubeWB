/**
  @page Thread_FTD_Coap_Multicast application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_FTD_Coap_Multicast/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Full Thread Device (FTD) Thread Coap Multicast 
  *          application
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

How to exchange multicast Coap messages.

This application requires two STM32WB55xx boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses two devices, one device will act as a Leader (Router) 
and the other one will act as an End Device(mode child). 

After the reset of the 2 boards, one board will be in Leader mode (Green LED2 ON) 
The other one will be in Child mode (Red LED3 ON).
 
Let's name indifferently one board A and one board B. 
To send a COAP command from board A to board B, press the SW1 Push-Button on board A. 
The board B will receive the COAP command to light on its blue LED1. 
Pressing again same push-button will light off the blue LED1. And so-on. 
Same COAP commands can be sent from board B to board A. 

  ___________________________                       ___________________________
  |  Device 1               |                       | Device 2                |
  |_________________________|                       |_________________________|  
  |                         |                       |                         |
  |                         |                       |                         |
  |        Push Button      |======> COAP =========>|    BLUE LED ON/OFF      |
  |                         |    Resource "light"   |                         |
  |                         |    Mode : Multicast   |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  |        BLUE LED ON/OFF  |<=====> COAP ==========|    Push Button          |
  |                         |    Resource "light"   |                         |
  |                         |    Mode : Multicast   |                         |
  |                         |                       |                         |
  |                         |                       |                         |
  ---------------------------                       ---------------------------
  | Role : Child            |                       | Role : Leader           |
  |                         |                       |                         |
  | LED : Red               |                       | LED : Green             |
  |                         |                       |                         |
  |_________________________|                       |_________________________|

      
@par Keywords

COAP,Thread

@par Directory contents 
  
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/app_common.h            Header for all modules with common definition
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/app_conf.h              Parameters configuration file of the application 
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/app_entry.h             Parameters configuration file of the application
  - Thread/Thread_FTD_Coap_Multicast/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/hw_conf.h               Configuration file of the HW 
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/system_infra.h          System infrastructure header file
  - Thread/Thread_FTD_Coap_Multicast/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/app_entry.c             Initialization of the application
  - Thread/Thread_FTD_Coap_Multicast/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_FTD_Coap_Multicast/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/hw_uart.c               UART driver
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/main.c                  Main program
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_FTD_Coap_Multicast/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
 
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and Dongle)
  
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
 - Rebuild all files of Thread/Thread_FTD_Coap_Multicast application 
   and load your image into the target memory
 - Run the application

Note: when LED1, LED2 and LED3 are toggling it is indicating an error has occurred on application.

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
 
  Note for addressing the low power :
  ----------------------------------
 To set the Application in full power mode, it is requested to set the define 
 CFG_FULL_LOW_POWER to 1 inside the app_conf.h file and to recompile the application.
 In this case, the Application is optimized in terms of power consumption.
 In this configuration,in order to save power,the LEDs are always powered off,
 the debugger cannot be attached any more to the target and there are no more traces.

**** EXTPA ****

The STM32WB is not able to transmit with a power higher than 5 dbm. As such it is
possible to use the EXT PA as an additional supply source.

Signals:
The EXTPA consists of three signals.

- Analogic signal : power supply, constant value.
- Enable/Disable: 0 when the RF is in Sleep/not initialized, 1 otherwise.
- RX/TX : 1 during a transmission, 0 otherwise.
___________________________________________________
|   RF status:    | Sleep mode | RX mode | TX mode |
|_________________|____________|_________|_________|
| Enable/Disable: |     0      |    1    |    1    |
|_________________|____________|_________|_________|
|    RX/TX:	      |    N/A     |    0    |    1    |
|_________________|____________|_________|_________|
|Analogic signal: |	             N/A               |
|_________________|________________________________|

NOTE: The RX/TX signal must be wired on the PB0 I/O (C10-22 on Nucleo).

The Enable/Disable signal by default is mapped on PB9 (C10-5 on Nucleo).
You can change this setting in app_conf.h, by editing GPIO_EXT_PA_EN_PIN and GPIO_EXT_PA_EN_PORT.

Power setting:
The firmware is not aware of the power brought by the ExtPA. It is up to the system integrator to
configure the application to get the desired power.

For instance, if the ExtPA provides an additional power of 10dbm and the desired power for a TX is 12dbm.
The application must be configured to set the power at 2dbm.

M0 firmware:
The M0 firmware remains unchanged. It has been tested with the binary listed above.

EXTPA enabling:
The EXTPA support is not enabled by default.
In hw_conf.h, set CFG_HW_EXTPA_ENABLED to 1.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 