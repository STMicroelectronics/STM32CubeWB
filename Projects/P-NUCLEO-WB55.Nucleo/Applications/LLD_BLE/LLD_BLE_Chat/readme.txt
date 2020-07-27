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

This example is to communicate Over The Air (OTA) using LLD_BLE between 2 boards in BLE Radio format not BLE Stack protocol.

@par Keywords

LLD_BLE, HAL send and receive Packet

@par Directory contents 
  
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
 
 If you want to control this application, you can directly press buttons 
 In this order and described into main.c:
 
 ///////////////         OBJECTIVES       //////////////////////////
 The objectif is to communicate Over The Air (OTA) using LLD_BLE between 2 boards, in BLE Radio format not BLE Stack protocol. 
 
 LLD_BLE is a 2-level stack implemented just over the Hardware and Radio layer.
	Lowest Layer also called Low Level or LL
		It is just over the Hardware and Radio Layer.
		It contains all the API to Set/Configure/Initialize all the parameters for Sending/receiving BLE Radio format packet data OTA
	Over LL layer there is HAL level
		It contains a reduced number of API to Send/Receive BLE Radio format packet with predefined parameters
		It works by calling a set of LL API
		It make simple and fast to Send/receive Packet
		But It does allow the user to change all the Radio parameters
		
 LL is for user that want to customize the Radio and BLE parameters, it is more complex to implement
 HAL is for user that want to Send/Receive in a very simple way less complex, without configuring LL 
 
 lld_ble contains LLD API HAL and LL API
 app_lld_ble contains Transport Layer Command call from CPU1 to CPU2 + Buffer management + IT Radio management from CPU2

 ///////////////         APPLICATION      //////////////////////////
 After power On or Reset  (ALL the LED are ON)
 
 1)Open 2 terminal 1 per Board (reset if you want Info on Terminal and first "LLD BLE >" )
   Start to CHAT....

 2) Press SW1 Encrypt/UnEncrypt the chat
    Encrypt when BLUE LED is off
    Non-Encrypt (UnEncrypt) BLUE LED is on (default)
    This also call the StopRadio API
 
 Serial Port Setup TERMINAL
 Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none  

 A set of parameters common to HAL API are proposed
 
 channel / WakeupTime / ReceiveWindowTime are predefined as chatChannel / chatWakeup / chatReceive
 
 networkID=chatID for HAL
 
 TxBuffer is also predefined, chattxBufferTab maximum size is 258
 1 for Header (set by user)
 1 for Payload Length (set by user / or sw) should be aligned with the payload Length
 255 max for Payload if Non-Encrypt
 251 max if Encrypt (4 bytes are added by hardware)

 TxBuffer for ACK answer is set as chatAcktxBufferTab
    
 dataRoutine for HAL API Send is chatdataCaseTx=dataRoutine_HAL_TxAck
 dataRoutine for HAL API Receive is chatdataCaseRx=dataRoutine_HAL_RxAck
  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */