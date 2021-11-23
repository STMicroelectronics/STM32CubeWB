/**
  @page BLE_LLD_Pressbutton example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    BLE_LLD/BLE_LLD_Pressbutton/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE LLD BLE_LLD_Pressbutton application
  ******************************************************************************
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to make toggling LED between 2 boards by pressing buttons.

@note The objectives are to communicate using BLE_LLD between 2 boards,
      in BLE Radio format not BLE Stack protocol.
      BLE_LLD_Pressbutton can use 2 terminals to display communication between 2 boards.
      BLE_LLD_Pressbutton can be used with BLE_LLD_Lowpower.
      But Appli is based to show in a very simple way a communication between 2 boards:
      It is based on pressing button and blinking answer
 
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
    
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/app_ble_lld.h     Header for app_ble_lld.c module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/pressbutton_app.h Header for Pressbutton Application pressbutton_app.c module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/tl_dbg_conf.h     Header for ble_lld debug module
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/app_ble_lld.c     contains TL management and Buffer for BLE LLD Application
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/App/pressbutton_app.c Pressbutton program
  - BLE_LLD/BLE_LLD_Pressbutton/STM32_WPAN/Target/hw_ipcc.c      IPCC Driver
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_common.h            Header for all modules with common definition
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_conf.h              Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/app_entry.h             Parameters configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/gpio_lld.h              Parameters for gpio configuration file of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/hw_conf.h               Configuration file of the HW
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/hw_if.h                 Configuration file of the HW
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/main.h                  Header for main.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm_logging.h	         Header for stm_logging.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32_lpm_if.h          Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/nucleo_wb15cc_conf.h    NUCLEO-WB15CC board configuration file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/app_entry.c             Initialization of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/gpio_lld.c              GPIO for application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/hw_uart.c               UART Driver
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/main.c                  Main program
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm_logging.c           Logging for application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32_lpm_if.c          Low Power Manager Interface
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32wbxx_it.c          Interrupt handlers
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/system_stm32wbxx.c      stm32wbxx system source file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/stm32wbxx_hal_msp.c     HAL MPS for application

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
 - Rebuild all files of BLE_LLD/BLE_LLD_Pressbutton application 
   and load your image into the other target memory
   + load stm32wb1x_BLE_LLD_fw.bin
 - Run the application
 
BLE_LLD_Pressbutton used only HAL API for Send/Receive

If you want to control this application, you can press buttons 
In this order and described into main.c:
 
After power On or Reset  (ALL the LED are OFF) all the Boards are in Received mode
   
 1) Press button SW on first board to interrupt Received mode and Send LED number to second board:
      -SW Number 1 (SW1) for LED Number 1 (LED1)
      -SW Number 2 (SW2) for LED Number 2 (LED2)
      -SW Number 3 (SW3) for LED Number 3 (LED3)

 2) When Second board Receive LED number:
      -LED1 : LED1 is Toggling
             "Led 1 is Toggling" is print
      -LED2 : LED2 is Toggling
             "Led 2 is Toggling" is print
      -LED3 : LED3 is Toggling
             "Led 3 is Toggling" is print

 3) To Switch off the LED press Reset or press the SW Number corresponding of the LED that is ON


Serial Port Setup TERMINAL
Baud Rate:115200 / Data:8 bits / Parity:none / Stop:1bit / Flow Control:none 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */