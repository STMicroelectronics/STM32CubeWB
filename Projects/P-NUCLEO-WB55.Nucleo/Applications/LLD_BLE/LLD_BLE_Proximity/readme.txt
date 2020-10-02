/**
  @page LLD_BLE_Proximity example
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    LLD_BLE/LLD_BLE_Proximity/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LLD BLE LLD_BLE_Proximity application
  ******************************************************************************
  *
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to create a Proximity detection with all other boards that are running this same application.

@note The objectives are to communicate using LLD_BLE between several boards,
      in BLE Radio format not BLE Stack protocol. 
      LLD_BLE_Proximity is an application that catch Local ID of Board in proximity
      This is a solution for detecting people sick of Covid-19 that we have in contact very close.
      Current config less than -80 dBm is not takin into account (>4 meters distance)

@note LLD_BLE is a 2-level stack implemented just over the Hardware and Radio layer.
      Lowest Layer also called Low Level or LL
	It is just over the Hardware and Radio Layer.
	It contains all the API to Set/Configure/Initialize all the parameters for Sending/receiving BLE Radio format packet data
      Over LL layer there is HAL level
	It contains a reduced number of API to Send/Receive BLE Radio format packet with predefined parameters
	It works by calling a set of LL API
	It make simple and fast to Send/receive Packet
	But It does allow the user to change all the Radio parameters
		
@note LL is for user that want to customize the Radio and BLE parameters, it is more complex to implement
      HAL is for user that want to Send/Receive in a very simple way less complex, without configuring LL 
      lld_ble module contains LLD API HAL and LL API
      app_lld_ble module contains Transport Layer Command call from CPU1 to CPU2 + Buffer management + IT Radio management from CPU2

@par Keywords

LLD_BLE, Connectivity, BLE, LLD, IPCC, HAL, Dual core, send and receive Packet

@par Directory contents 
  
  - LLD_BLE/LLD_BLE_Proximity/STM32_WPAN/App/app_lld_ble.h      Header for app_lld_ble.c module
  - LLD_BLE/LLD_BLE_Proximity/STM32_WPAN/App/lld_ble.h         	Header for lld_ble.c module
  - LLD_BLE/LLD_BLE_Proximity/STM32_WPAN/App/app_lld_ble.c      contains TL management and Buffer for LLD BLE Application
  - LLD_BLE/LLD_BLE_Proximity/STM32_WPAN/App/lld_ble.c         	contains LL and HAL API
  - LLD_BLE/LLD_BLE_Proximity/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/app_common.h            	Header for all modules with common definition
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/app_entry.h             	Parameters configuration file of the application
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/gpio_lld.h             	Parameters for gpio configuration file of the application
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/hw_conf.h           	Configuration file of the HW
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/hw_if.h           	Configuration file of the HW
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/main.h                  	Header for main.c module
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/stm_logging.h	        Header for stm_logging.c module
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/stm32_lpm_if.h          	Header for stm32_lpm_if.c module
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/stm32wbxx_hal_conf.h	HAL configuration file
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - LLD_BLE/LLD_BLE_Proximity/Core/Inc/utilities_conf.h    	Configuration file of the utilities
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/app_entry.c      	Initialization of the application
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/gpio_lld.c               GPIO for for application
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/hw_uart.c 	        UART Driver
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/main.c                  	Main program
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/stm_logging.c            Logging for application
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/stm32_lpm_if.c		Low Power Manager Interface
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - LLD_BLE/LLD_BLE_Proximity/Core/Src/stm32wbxx_hal_msp.c 	HAL MPS for application
  
@par Hardware and Software environment

  - This application uses two STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55RG-Nucleo 
    board and can be easily tailored to any other supported device 
    and development board.
    
  - On STM32WB55RG-Nucleo, the jumpers must be configured as described
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
 - Connect 2 STM32WB55xx-Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into one target memory
 - Rebuild all files of LLD_BLE/LLD_BLE_Proximity application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - Run the application
 
LLD_BLE_Proximity used only LL API not HAL

If you want to control this application, you can directly press buttons and put in proximity some board 
In this order and described into main.c:

After power On or Reset  (ALL the LED are OFF)
  
 1) Press SW1 to Init:

 2) Press SW2 to Launch Application from 0:RX ActionPacket:

    ActionPacket filling
    ___________________________
   |               Ack _____   | 
   |       true |---=>|2:TX |__|       
   |   _____    |  |  |_____|     _____
   |=>|0:RX |___|  |_____________|1:RX |        
   |  |_____|___    _____________|_____|<=-|  
   |            |  |   _____      _____    | 
   |      false |---=>|3:TX |--=>|4:RX |___|
   |                  |_____| Ack|_____|___
   |_______________________________________|
   
   After SW1 and SW2
   0:RX (RX mode and listen if TX is send from another board)
     if true  => 2:TX (send a TX Ack) and goes back to 0:RX
     if false => 3:TX (send its Local ID) and goes to  4:RX 
   4:RX (RX mode and listen if a TX Ack is send from another board)
     if true  => 1:RX (RX mode and listen if TX is send from another board = equivalent to 0:RX )
     if false => 0:RX  
     
   1:RX and 0:RX differs that 1:RX has a Wakeup time much longer that 0:RX
   It makes the board in sleep mode for a while in order to allow other board to comunicate
      
   
 3) Press SW3 to stop Radio and Display Results on terminal

Exemple:
Local     ID  >  c6  17            // Current device ID under terminal
Radio Get ID 3>  6f  b8            // device detected
Radio Get ID 2>  f2  b5            // device detected
Radio Get ID 1>   a  2f            // device detected

Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none 
 
0x18(24) payload is proxSTDEVICEUDN[0:5]

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */