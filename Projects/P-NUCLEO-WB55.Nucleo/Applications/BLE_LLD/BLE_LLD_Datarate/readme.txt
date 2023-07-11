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
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/gpio_lld.h              Parameters for gpio configuration file of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/hw_if.h                 Configuration file of the HW
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/main.h                  Header for main.c module
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm_logging.h           Header for stm_logging.c module
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32_lpm_if.h          Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Datarate/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/app_entry.c      	      Initialization of the application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/gpio_lld.c              GPIO for application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/hw_timerserver.c        TIMERSERVER for Datarate application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/hw_uart.c               UART Driver
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/main.c                  Main program
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm_logging.c           Logging for application
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE_LLD/BLE_LLD_Datarate/Core/Src/stm32wbxx_hal_msp.c     HAL MPS for application

@par Hardware and Software environment

  - This application uses three STM32WB55xx devices.
  
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

This application requires having the stm32wb5x_BLE_LLD_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.

In order to make the program work, you must do the following: 
 - Connect the STM32WB55xx-Nucleo boards to your PC
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory of all the boards

You can control this application, after power and reset Pressing SW1 or SW2 to set respectively in Reception or Transmission the board 
In this order and described into main.c:
 
After power On or Reset (ALL the LED are OFF):
 1) Radio Init is done and the Action Packets are configured
 2) Press SW1 to set the first board in reception 
 3) Press SW2 to set the second board in transmission, it will be master board
 4) Second board (master) Transmits to First board (reception) every 1500us a Payload.  
    Alternativaly master board send 1000 first packets with LED1 into the payload and 1000 next packets with LED2 state into the payload
    Each time that reception board receive 100 packets it toggle the corresponding LED. So, the LED1 toggle 10 times and after this is the LED2 that toggle 10 times
 5) Press SW2 to set the last board in transmission, it will be slave board
 6) Last board (slave) Transmits to First board (reception) every 1500us a Payload. The packet contains the LED to Toggle (LED3) with a 750us delay shift compared to the master board
 7) master and slave are alternatively in transmission and reception
 8) When master transmits, slave receives (LED1 or LED2 also toggle). when slave transmits, master receives (LED3 also toggle)
 9) When master and slave are activated, every 750us a packet is send and it is received to the reception board

Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
