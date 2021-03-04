/**
  @page BLE_LLD_Pressbutton example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    BLE_LLD/BLE_LLD_Pressbutton/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE LLD BLE_LLD_Pressbutton application
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

How to make blinking LED between 2 STM32WB55xx boards by pressing buttons.

@note The objectives are to communicate using BLE_LLD between 2 boards,
      in BLE Radio format not BLE Stack protocol.
      BLE_LLD_Pressbutton can use 2 terminal to display communication between 2 boards.
      BLE_LLD_Pressbutton can be alos used with BLE_LLD_Lowpower.
      But Appli is based to show in a very simple way a communication between 2 boards:
      It is based on pressing button and blinking answer
 
@note BLE_LLD is a 2-level stack implemented just over the Hardware and Radio layer.
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
      ble_lld module contains LLD API HAL and LL API
      app_ble_lld module contains Transport Layer Command call from CPU1 to CPU2 + Buffer management + IT Radio management from CPU2

@par Keywords

BLE_LLD, Connectivity, BLE, LLD, IPCC, HAL, Dual core, send and receive Packet

@par Directory contents 
    
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/app_ble_lld.h     Header for app_ble_lld.c module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/ble_lld.h         Header for ble_lld.c module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/ble_lld_private.h Header for ble_lld private module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/tl_dbg_conf.h     Header for ble_lld debug module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/ring_buffer.h     Header for ring buffer.c module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/app_ble_lld.c     contains TL management and Buffer for BLE LLD Application
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/ble_lld.c         contains LL and HAL API
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/ring_buffer.c     contains ring buffer API
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_common.h            Header for all modules with common definition
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_conf.h              Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_entry.h             Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/gpio_lld.h              Parameters for gpio configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/hw_if.h                 Configuration file of the HW
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/pressbutton_app.h       Header for Pressbutton Application pressbutton_app.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/main.h                  Header for main.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm_logging.h	         Header for stm_logging.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32_lpm_if.h          Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/app_entry.c             Initialization of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/gpio_lld.c              GPIO for application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/hw_uart.c               UART Driver
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/pressbutton_app.c       Pressbutton program
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/main.c                  Main program
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm_logging.c           Logging for application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32wbxx_hal_msp.c     HAL MPS for application

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
 - Rebuild all files of BLE_LLD/BLE_LLD_Pressbutton application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - Run the application
 
BLE_LLD_Pressbutton used alternatively LL API and HAL API for Send/receive by pressing SW1

If you want to control this application, you can directly press buttons 
In this order and described into main.c:
 
After power On or Reset  (ALL the LED are OFF)
   
 1) Press SW2 or SW3 to Init =>  GREEN and RED LED becomes ON after Init
    LL API : BLUE LED is OFF
    -   BLE_LLD_Init(HS_STARTUP_TIME, 1, BLE_LLD_hot_ana_config_table, ENABLE); 
    ...
    HAL API: BLUE LED is ON
    -   HAL_BLE_Init();

    To send    a Packet go to 2)
    or
    To receive a Packet go to 3)
 
 2) Press SW2 for Send 500 Chained Packets                             ===>    GREEN is switched off during TX
    LL API : BLUE LED is OFF
    -   BLE_LLD_MakeActionPacketPending(); // after setting Packet is pending and start TX after TIMEOUT IT
    ...
    HAL API: BLUE LED is ON
    -   HAL_BLE_SendPacketWithAck();  // send is done using HAL API

    or
 
 3) Press SW3 for receiving 400 Packets (HAL) and 500 Packets (LL)     ===>    RED is switched off during RX
    LL API : BLUE LED is OFF
    -   BLE_LLD_MakeActionPacketPending(); // after setting Packet is pending and start RX after TIMEOUT IT
    ...
    HAL API: BLUE LED is ON
    -   HAL_BLE_ReceivePacketWithAck();  // receive is done using HAL API
    
      
 4) Press SW1 (switch API and AccessAddress)
    using LL API is default => BLUE LED is OFF (if SW1 has not been pressed)
    using HAL API           => BLUE LED is ON 
    press SW1 switch from one API to another
   
   
When one board Send :    RED LED is ON
When one board Receive:  GREEN LED is ON

When one board Send and other Receive and communicates with ACK, 
Sending board GREEN LED toggle (Terminal shows the TX with RX Ack communication)
Receiving board RED LED toggle (Terminal shows the RX with TX Ack communication)
 
At the END GREEN and RED LED are ON (Terminal shows the final number of TX/RX)

BLE_LLD_Pressbutton can be used wit BLE_LLD_Lowpower.
Instead of configuring the second board with Send or Receive mode.
BLE_LLD_Lowpower automatically and alternatively Send and Receive Packets, nothing to do
Then, Only first board with BLE_LLD_Pressbutton is managed with button 


Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
