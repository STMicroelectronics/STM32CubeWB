/**
  @page Thread_NVM application
  
  @verbatim
  ******************************************************************************
  * @file    Thread/Thread_NVM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Coap Generic Application 
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

How to configure NVM for Thread applications.

This application use case requires one STM32WB55xx boards.

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

This application example demonstrates configuration of the NVM(Non Volatile Memory) also called FLASH memory to store Thread network credentials.
 
   _________________________________________________________________________________________________________________________________________________                                            
  |              Device 1                                                                                                                           |
  |_________________________________________________________________________________________________________________________________________________|
  |                                                                                                                                                 |
  |                                                                                                                                                 |
  |      |----> **** STEP1 (FIRST BOOT) ****:                                                                                                       |
  |      |      At startup the device will start Thread on default channel (11) (default credentials)                                               |
  |      |      the device will pass on Leader role (Green LED2 ON)                                                                                 |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       After some delay (3 seconds) Thread device is disabled                                                                             |
  |      |       once disabled state is reached --> APP_THREAD_NvmProcess() is called                                                               |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       APP_THREAD_NvmSave() process is called (copy from RAM buffer to FLASH memory)                                                      |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       Once copy performed, BLUE LED will toggle fastly                                                                                   |
  |      |       and on Traces/Logs message is displayed to power off/power on the device                                                           |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |  (POWER OFF/POWER ON) ->  |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |      **** STEP2 (SECOND BOOT) ****:                                                                                                      |
  |      |      At startup the device will start Thread on channel (14) (credentials set at STEP1)                                                  |
  |      |      the device will pass on Leader role (Green LED2 ON)                                                                                 |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       After some delay (3 seconds) Thread device is disabled                                                                             |
  |      |       once disabled state is reached --> APP_THREAD_NvmProcess() is called                                                               |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       Persistent info erase is performed (RAM buffer erase by calling otInstanceErasePersistentInfo)                                     |
  |      |       once disabled state is reached --> APP_THREAD_NvmProcess() is called                                                               |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       APP_THREAD_NvmSave() process is called (copy from RAM buffer to FLASH memory)                                                      |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           V                                                                                                              |
  |      |       Once copy performed, Thread device credentials are deleted from RAM buffer and FlASH memory. (back to defaults values)             |       
  |      |       BLUE LED will toggle slowly                                                                                                        |
  |      |       and on Traces/Logs message is displayed to power off/power on the device (back to STEP1)                                           |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |  (POWER OFF/POWER ON) ->  |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |                           |                                                                                                              |
  |      |---------------------------|                                                                                                              |
  |                                                                                                                                                 |
  |                                                                                                                                                 |
  |_________________________________________________________________________________________________________________________________________________|                                                                                                                                                |
                                                                                    
      
@par Keywords

COAP, Thread, NVM

@par Directory contents 
  
  - Thread/Thread_NVM/Core/Inc/app_common.h       	Header for all modules with common definition
  - Thread/Thread_NVM/Core/Inc/app_conf.h         	Parameters configuration file of the application 
  - Thread/Thread_NVM/Core/Inc/app_entry.h        	Parameters configuration file of the application
  - Thread/Thread_NVM/STM32_WPAN/App/app_thread.h      Header for app_thread.c module
  - Thread/Thread_NVM/Core/Inc/hw_conf.h          	Configuration file of the HW 
  - Thread/Thread_NVM/Core/Inc/main.h                  Header for main.c module
  - Thread/Thread_NVM/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - Thread/Thread_NVM/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - Thread/Thread_NVM/Core/Src/stm32wbxx_it.h          Interrupt header file
  - Thread/Thread_NVM/Core/Inc/utilities_conf.h   	Configuration file of the utilities
  - Thread/Thread_NVM/Core/Src/app_entry.c        	Initialization of the application
  - Thread/Thread_NVM/STM32_WPAN/App/app_thread.c      Thread application implementation
  - Thread/Thread_NVM/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - Thread/Thread_NVM/Core/Src/stm32_lpm_if.c     	Low Power Manager Interface
  - Thread/Thread_NVM/Core/Src/hw_timerserver.c   	Timer Server Driver
  - Thread/Thread_NVM/Core/Src/hw_uart.c          	UART driver
  - Thread/Thread_NVM/Core/Src/main.c                  Main program
  - Thread/Thread_NVM/Core/Src/stm_logging.c           Logging module for traces
  - Thread/Thread_NVM/Core/Src/stm32xx_it.c            Interrupt handlers
  - Thread/Thread_NVM/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - Thread/Thread_NVM/Core/Src/ee.c                    EEPROM emulator source file
  - Thread/Thread_NVM/Core/Src/flash_driver.c          FLASH memory source file 
  - Thread/Thread_NVM/Core/Src/ee.h                    EEPROM emulator header file
  - Thread/Thread_NVM/Core/Src/ee_cfg.h                EEPROM emulator configuration header file
  - Thread/Thread_NVM/Core/Src/flash_driver.h          FLASH memory source file 
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices (Nucleo board and dongle) 
  
  - This example has been tested with an STMicroelectronics STM32WB55xx_Nucleo 
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
 - Connect 2 STM32WB55xx_Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
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
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 