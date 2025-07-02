/**
  @page BLE_LLD_Chat example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    BLE_LLD/BLE_LLD_Chat/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE LLD BLE_LLD_Chat application
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to create a "Chat" talk between 2 boards using terminals.
 
@note The objectives are to communicate using BLE_LLD between 2 boards,
      in BLE Radio format not BLE Stack protocol.
      BLE_LLD_Chat uses 2 terminal and through uart by typing Text on terminal.
      It sends the text into the payload and this payload is received on other board
      and text into the payload is displayed on terminal using uart.
 
@note BLE_LLD is a 2-level stack implemented just over the Hardware and Radio layer.
      Lowest Layer also called Low Level or LL
	It is just over the Hardware and Radio Layer.
	It contains all the API to Set/Configure/Initialize all the parameters for Sending/receiving BLE Radio format packet data
      Over LL layer there is HAL level
	It contains a reduced number of API to Send/Receive BLE Radio format packet with predefined parameters
	It works by calling a set of LL API
	It make simple and fast to Send/Receive Packet
	But It does allow the user to change all the Radio parameters
		
@note LL is for user that want to customize the Radio and BLE parameters, it is more complex to implement
      HAL is for user that want to Send/Receive in a very simple way less complex, without configuring LL 
      ble_lld module contains LLD API HAL and LL API
      app_ble_lld module contains Transport Layer Command call from CPU1 to CPU2 + Buffer management + IT Radio management from CPU2

@par Keywords

BLE_LLD, Connectivity, BLE, LLD, IPCC, HAL, Dual core, send and receive Packet

@par Directory contents 

  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/App/app_ble_lld.h           Header for app_ble_lld.c module
  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/App/chat_app.h              Header for Chat Application chat_app.c module
  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/App/tl_dbg_conf.h           Header for ble_lld debug module
  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/App/app_ble_lld.c           contains TL management and Buffer for BLE LLD Application
  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/App/chat_app.c              Chat program
  - BLE_LLD/BLE_LLD_Chat/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/app_entry.h             	Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/gpio_lld.h             	Parameters for gpio configuration file of the application
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/hw_if.h           		Configuration file of the HW
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/main.h                  	Header for main.c module
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/stm_logging.h		        Header for stm_logging.c module
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/stm32_lpm_if.h          	Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Chat/Core/Inc/nucleo_wb15cc_conf.h          NUCLEO-WB15CC board configuration file
  - BLE_LLD/BLE_LLD_Chat/Core/Src/app_entry.c      		Initialization of the application
  - BLE_LLD/BLE_LLD_Chat/Core/Src/gpio_lld.c                    GPIO for application
  - BLE_LLD/BLE_LLD_Chat/Core/Src/hw_timerserver.c              TIMERSERVER for Lowpower application
  - BLE_LLD/BLE_LLD_Chat/Core/Src/hw_uart.c 			UART Driver
  - BLE_LLD/BLE_LLD_Chat/Core/Src/main.c                  	Main program
  - BLE_LLD/BLE_LLD_Chat/Core/Src/stm_logging.c                 Logging for application
  - BLE_LLD/BLE_LLD_Chat/Core/Src/stm32_lpm_if.c		Low Power Manager Interface
  - BLE_LLD/BLE_LLD_Chat/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE_LLD/BLE_LLD_Chat/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE_LLD/BLE_LLD_Chat/Core/Src/stm32wbxx_hal_msp.c 		HAL MPS for application  

@par Hardware and Software environment

  - This application uses two STM32WB15xx devices.
  
  - This example has been tested with an STMicroelectronics NUCLEO-WB15CC 
    board and can be easily tailored to any other supported device 
    and development board.
    
  - On NUCLEO-WB15CC, the jumpers must be configured as described
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

In order to make the program work, you must do the following: 
 - Connect 1 NUCLEO-WB15CC board to your PC
 - Open your preferred toolchain 
 - Rebuild all files and load your image into one target memory
 - Rebuild all files of BLE_LLD/BLE_LLD_Chat application 
   and load your image into the other target memory
   + load stm32wb1x_BLE_LLD_fw.bin
 - Run the application

BLE_LLD_Chat used only HAL API for Send/Receive not LL

You can control this application, after power and reset 
You can start to Chat using Uart Com Terminal and you can press buttons

After power On or Reset (ALL the LED are OFF) without pressing SW button:
 1) RadioInit is done and Default Encryption is set (LED1 is ON), Low-Power is not activated
 2) Radio is listening and if text is received from another board it is printed through the Uart Com Terminal
 3) If text is typed through the Uart Com Terminal Radio is no more listenning and text is send to another board
 4) loop on step 2
  
    
Press SW1 to change Chat by applying or not Default Radio Encryption Communication
 1) If Default Encryption is set, Encryption is unset.
 2) If Encryption is not set, Default Encryption is set
This Encryption:
 - uses default KEY and IV parameters. Default KEY and IV must be managed by implementing a Dedicated Encryption Stack not developed in this application
 - is only implemented in order to present how the Encryption LL API are called and how Radio Encryption Communication works
   
Press SW2 to Start the Tone
 1) StartTone is launched only one time (SW2 inactive after)
 2) It can be stopped by resetting board or by using StopTone
    Chat will not work after StartTone: reset board must be performed after StartTone
    
Press SW3 to Stop the Tone
 1) StopTone is launched only one time (SW3 inactive after)
    Chat will not work after StopTone: reset board must be performed after StopTone
 
Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none  
 
255 max for Payload if Non-Encrypt
251 max if Encrypt (4 bytes are added by hardware)
Note: in the implemented project, the message buffer is limited to 200 bytes in all subcases 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */