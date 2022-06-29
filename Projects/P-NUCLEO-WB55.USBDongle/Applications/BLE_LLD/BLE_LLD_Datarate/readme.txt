/**
  @page BLE_LLD_Datarate example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    BLE_LLD/BLE_LLD_Datarate/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE LLD BLE_LLD_Datarate application
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

How to send BLE LLD packets in high data rate.

@note The objectives are to communicate using BLE_LLD between 3 boards,
      in BLE Radio format not BLE Stack protocol.
      Appli is based on Press button that send a payload that contains info to Toggle LED
      It is launched after reset.
 
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
    
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/App/app_ble_lld.h     Header for app_ble_lld.c module
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/App/datarate_app.h    Header for Datarate Application datarate_app.c module
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/App/tl_dbg_conf.h     Header for ble_lld debug module
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/App/app_ble_lld.c     contains TL management and Buffer for BLE LLD Application
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/App/datarate_app.c    Datarate program
  - BLE_LLD/BLE_LLD_Datarate/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/app_common.h            Header for all modules with common definition
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/app_conf.h              Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/app_entry.h             Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/app_debug.h             Parameters for Debug capabilities file of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/hw_if.h                 Configuration file of the HW
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/main.h                  Header for main.c module
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32_lpm_if.h          Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/app_entry.c      	      Initialization of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/app_debug.c             Debug capabilities source file
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/hw_timerserver.c        TIMERSERVER for Datarate application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/hw_uart.c               UART Driver
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/main.c                  Main program
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32wbxx_hal_msp.c     HAL MPS for application

@par Hardware and Software environment

  - This application uses two STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55xx_Usb_Dongle 
    board and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following: 
 - Connect 3 STM32WB55xx_Usb_Dongle boards to your PC
 - Open your preferred toolchain 
 - Rebuild all files and load your image into one target memory
 - Rebuild all files of BLE_LLD/BLE_LLD_Datarate application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - Run the application

You need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.

You can control this application, after power and reset the board is in Reception and after pressing SW1 the board is in transmission

In this order and described into main.c:
 
After power On or Reset (ALL the LED are OFF):
 1) Plug 2 STM32WB55xx_Usb_Dongle boards, Radio Init is done. The 2 boards are in reception by default 
 3) Press SW1 to set one board in transmission: it becomes master board
 4) Master board Transmits to reception board every 1500us a Payload containing the LED to Toggle (LED1 or LED2) on StateMachine 0 (STMCH0)
    Alternativaly master board send 1000 first packets with Payload containing LED1 to toggle and 1000 next packets with Payload containing LED2 to toggle
    Each time that reception board receive 100 packets with Payload containing LED to toggle it toggles the LED.
    In this case, on the reception board the LED1 toggle 10 times and after this is the LED2 that toggle and so alternatively.

reception                               |+++++++++(750us)+++++++++>|
              _______________           |_______________            _______________ 
             |               |          |               |          |               |
-------------| RX : STMCH0   |----------| RX : STMCH1   |----------| RX : STMCH0   |--->
             |_______________|          |_______________|          |_______________|


master     (SW1) |++++++++++++++++++++++(1500us)++++++++++++++++++++++>|
                 |___________            _______________                ___________ 
                 |           |          |               |              |           |
-----------------| TX:STMCH0 |----------| RX : STMCH1   |--------------| TX: STMCH0|--->
                 |___________|          |_______________|              |___________|
                 |
                 |
slave      (SW1) |+++++++++(750us)+++++++++>|++++++++++++++++++++++(1500us)++++++++++++++++++++++>|
              _______________               |___________            _______________                ___________ 
             |               |              |           |          |               |              |           |
-------------| RX : STMCH0   |--------------| TX:STMCH1 |----------| RX : STMCH0   |--------------| TX: STMCH1|--->
             |_______________|              |___________|          |_______________|              |___________|



 5) Plug 1 more STM32WB55xx_Usb_Dongle boards, Radio Init is done. this board is in reception by default
 6) Same behavior than first board in reception described in "4)"
 7) Press SW1 to set this last board in transmission: it becomes slave board, as master already present
 8) Slave board Transmits to reception board every 1500us a Payload containing the LED to Toggle (LED3) with a 750us delay shift compared to the master board on StateMachine 1 (STMCH1)

Transmission boards are alternatively in transmission (TX) and reception (RX) when reception is always in reception (RX) but alternatively on STMCH0 and STMCH1.
When master board transmits, slave board receives (LED1 or LED2 also toggle). when slave board transmits, master board receives (LED3 also toggle)
When master and slave are activated, every 750us a packet is send and it is received to the reception board (LED1 or LED2 toggle and LED3 toggle)
StateMachine 0 and StateMachine 1 are using different NetworkID and different channel

You can open VPC Terminal(s) to see master and slave values and you can Press SW1 to reset master and slave values

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
