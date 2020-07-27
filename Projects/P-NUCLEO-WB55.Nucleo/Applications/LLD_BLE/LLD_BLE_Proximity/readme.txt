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

This example is to communicate Over The Air (OTA) using LLD_BLE between several boards in BLE Radio format not BLE Stack protocol. 
 LLD_BLE_Proximity is an application that catch Local ID of Board in proximity.

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
 - Rebuild all files of LLD_BLE/LLD_BLE_Proximity application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - Run the application
 
 If you want to control this application, you can directly press buttons and put in proximity some board 
 In this order and described into main.c:
 
 
 ///////////////         OBJECTIVES       //////////////////////////
 The objectif is to communicate Over The Air (OTA) using LLD_BLE between several boards, in BLE Radio format not BLE Stack protocol. 
 LLD_BLE_Proximity is an application that catch Local ID of Board in proximity
 This is answer for detecting people sick of Covid-19 that we have in contact very close during the last 14 days (or more) 
 With current config less than -80 dBm is not takin into account (>4 meters distance)
 
 LLD_BLE is a 2-level stack implemented just over the Hardware and Radio layer.
	Lowest Layer also called Low Level or LL
		It is just over the Hardware and Radio Layer.
		It contains all the API to Set/Configure/Initialize all the parameters for Sending/receiving BLE Radio format packet data OTA
	Over LL layer there is HAL level
		It contains a reduced number of API to Send/Receive BLE Radio format packet with predefined parameters
		It works by calling a set of LL API
		It make simple and fast to Send/receive Packet
		But It does allow the user to change all the Radio parameters
		
 lld_ble contains LLD API HAL and LL API
 app_lld_ble contains Transport Layer Command call from CPU1 to CPU2 + Buffer management + IT Radio management from CPU2

 ///////////////         APPLICATION      //////////////////////////
 After power On or Reset  (ALL the LED are OFF)
 1) Press SW1 to Init
 2) Press SW2 to Launch Application from 0:RX ActionPacket
//    ___________________________
//   |               Ack _____   | 
//   |       true |---=>|2:TX |__|       
//   |   _____    |  |  |_____|     _____
//   |=>|0:RX |___|  |_____________|1:RX |        
//   |  |_____|___    _____________|_____|<=-|  
//   |            |  |   _____      _____    | 
//   |      false |---=>|3:TX |--=>|4:RX |___|
//   |                  |_____| Ack|_____|___
//   |_______________________________________|
   During 0:RX in RXMode it listen OTA if TX is done from Another Board for same AccessAddress at same Channel
   If detecting:true it launch a 2:TX ACK packet (different AccessAddress and MachineState for Ack)
   If not it launch the 3:TX packet with its own ID and wait for 4:RX ACK (different AccessAddress and MachineState for Ack)
     If 4:RX is true: it means that other device has send its 2:TX packet (Same Ack AccessAddress and MachineState)
       Then it launch a 4:RX after a long wakeup to make this device out the OTA detection during few RX)
     If 4:RX is false: no device has send a Ack 2:TX it start again to listen 0:RX after a regular wakeup.
   1:RX is equivalent to 0:RX but it is done after a longer wakeup.
 3) Press SW3 to stop Radio and Display Results on terminal
Exemple:
Local     ID  >  c6  17            // Current device ID under terminal
Radio Get ID 3>  6f  b8            // device detected
Radio Get ID 2>  f2  b5            // device detected
Radio Get ID 1>   a  2f            // device detected

 Serial Port Setup TERMINAL
 Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none 
 
 ///////////////         MAIN CODE        //////////////////////////
 A set of parameters common to LL API are proposed
 
 channel / WakeupTime / ReceiveWindowTime are predefined as proxChannel / proxWakeup / proxReceive
 
 networkID=~proxID for LL data (statemachine 0)
 networkID=proxID  for LL ack  (statemachine 3)
 
 TxBuffer is also predefined, presstxBufferTab maximum size is 258
 1 for Header (set by user)
 1 for Payload Length 0x18 (set by user) 
 0x18(24) payload is proxSTDEVICEUDN[0:5]

 TxBuffer for ACK answer (proxAcktxBuffer) is complementary to TxBuffer for data (proxtxBuffer)
    
 dataRoutine_Action and dataRoutineDone_hal_BLE are used


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */