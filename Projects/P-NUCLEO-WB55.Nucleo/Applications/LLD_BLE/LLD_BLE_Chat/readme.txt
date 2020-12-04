/**
  @page LLD_BLE_Chat example
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    LLD_BLE/LLD_BLE_Chat/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LLD BLE LLD_BLE_Chat application
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

How to create a "Chat" talk between 2 STM32WB55xx boards using terminals.
 
@note The objectives are to communicate using LLD_BLE between 2 boards,
      in BLE Radio format not BLE Stack protocol.
      LLD_BLE_Chat uses 2 terminal and through uart by typing Text on terminal.
      It sends the text into the payload and this payload is received on other board
      and text into the payload is displayed on terminal using uart.
 
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

  - LLD_BLE/LLD_BLE_Chat/STM32_WPAN/App/app_lld_ble.h         	Header for app_lld_ble.c module
  - LLD_BLE/LLD_BLE_Chat/STM32_WPAN/App/lld_ble.h         	Header for lld_ble.c module
  - LLD_BLE/LLD_BLE_Chat/STM32_WPAN/App/app_lld_ble.c         	contains TL management and Buffer for LLD BLE Application
  - LLD_BLE/LLD_BLE_Chat/STM32_WPAN/App/lld_ble.c         	contains LL and HAL API
  - LLD_BLE/LLD_BLE_Chat/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/app_common.h            	Header for all modules with common definition
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/app_entry.h             	Parameters configuration file of the application
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/gpio_lld.h             	Parameters for gpio configuration file of the application
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/hw_conf.h           		Configuration file of the HW
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/hw_if.h           		Configuration file of the HW
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/main.h                  	Header for main.c module
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/stm_logging.h		        Header for stm_logging.c module
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/stm32_lpm_if.h          	Header for stm32_lpm_if.c module
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - LLD_BLE/LLD_BLE_Chat/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - LLD_BLE/LLD_BLE_Chat/Core/Src/app_entry.c      		Initialization of the application
  - LLD_BLE/LLD_BLE_Chat/Core/Src/gpio_lld.c          	        GPIO for for application
  - LLD_BLE/LLD_BLE_Chat/Core/Src/hw_uart.c 			UART Driver
  - LLD_BLE/LLD_BLE_Chat/Core/Src/main.c                  	Main program
  - LLD_BLE/LLD_BLE_Chat/Core/Src/stm_logging.c                Logging for application
  - LLD_BLE/LLD_BLE_Chat/Core/Src/stm32_lpm_if.c		Low Power Manager Interface
  - LLD_BLE/LLD_BLE_Chat/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - LLD_BLE/LLD_BLE_Chat/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - LLD_BLE/LLD_BLE_Chat/Core/Src/stm32wbxx_hal_msp.c 		HAL MPS for application  

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
 - Rebuild all files of LLD_BLE/LLD_BLE_Chat application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - Run the application

LLD_BLE_Chat used only HAL API for Send/Receive not LL

In this order and described into main.c:

After power On or Reset (ALL the LED are ON if into app_conf.h "CFG_FULL_LOW_POWER" is set to "0" => "#define CFG_FULL_LOW_POWER  0"), Init is done
  
 1) Open 2 terminal 1 per Board (reset if you want Info on Terminal and first "LLD BLE >" )
    Start to CHAT....
    
    When Power or Reset, Init is automatically run with:
    CHAT_BLE_Init
    -   HAL_BLE_Init(); // Init the Radio in BLE mode
    -   HAL_BLE_SetNetworkID(chatID); // Set the network ID
    + encrypt setting without enabling yet
    -   LLD_BLE_SetEncryptionAttributes(STATE_MACHINE_0, chatenc_iv, chatenc_key); // Set the init Vector and the Key for Encrypt
    -   LLD_BLE_SetEncryptionCount(STATE_MACHINE_0, &chatcount_tx[0], &chatcount_rcv[0]);  // Set the account TX and RX
    
    After CHAT_BLE_Init, Listen is done using Sequencer:
    CHAT_ListenPacket
    -   APP_LLD_BLE_SetdataRoutineOption(chatPacketNumberRx,chatPacketStopRx); // Set dataRoutine option : NB of listen packet + Stop if a RX packet is OK
    -   HAL_BLE_ReceivePacketWithAck(chatChannel, chatWakeup, chatrxBuffer, chatAcktxBuffer, chatReceive, dataRoutine_HAL_RxAck);  // HAL API for Receiving and Send Ack
   
    if no packet Receive, and if a packet has to be send: CHAT_ListenPacket is recursively called
    else
    if packet Receive, it prints the Results on Terminal using uart.
    or
    if a packet has to be send, it calls CHAT_SendPacket
    
    A packet has to be send when Text to chat has been write and return into the Terminal uar. In this case CHAT_SendPacket is called instead of CHAT_ListenPacket
    CHAT_SendPacket
    -   APP_LLD_BLE_SetdataRoutineOption(chatPacketNumberTx,chatPacketStopRx); // Set dataRoutine option : NB of send packet + Stop if a RX ACK packet is OK
    -   HAL_BLE_SendPacketWithAck(chatChannel, chatWakeup, chattxBuffer, chatAckrxBuffer, chatReceiveAck, dataRoutine_HAL_TxAck); // HAL API for Send and wait Ack

 2) Press SW1 to alternatively Encrypt/UnEncrypt the Chat
 
    LLD_BLE_StopActivity is done before Alterning Encrypt/UnEncrypt

    CHAT_Encrypt
    -   LLD_BLE_SetEncryptFlags(STATE_MACHINE_0, ENABLE, ENABLE);
    or
    CHAT_UnEncrypt
    -   LLD_BLE_SetEncryptFlags(STATE_MACHINE_0, DISABLE, DISABLE);

    Encrypt when BLUE LED is off
    Non-Encrypt (UnEncrypt) BLUE LED is on (default)
    
 3) Press SW2 to Start the Tone
 
    LLD_BLE_StartTone is launched only one time (SW2 inactive after)
    It can be stopped by resetting board
    or by using LLD_BLE_StopTone
    Chat will not work after StartTone: reset board must be performed after StartTone
    
 3) Press SW3 to Stop the Tone
 
    LLD_BLE_StopTone is launched only one time (SW3 inactive after)
    Chat will not work after StopTone: reset board must be performed after StopTone
 
Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none  
 
255 max for Payload if Non-Encrypt
251 max if Encrypt (4 bytes are added by hardware)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */