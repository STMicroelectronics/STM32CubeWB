/**
  @page Phy_802_15_4_Cli application
  
  @verbatim
  ******************************************************************************
  * @file    /Phy_802_15_4/Phy_802_15_4_Cli/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Phy_802_15_4_Cli application
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

@par Application Description
  
How to create a "PHY_802.15.4 command line interface" application on STM32WB55xx boards using terminals.

@note A dedicated application allows the control and the test of the 802_15_4 radio via a command line interface.
      This application request the usage of the following wireless copro binary: stm32wb5x_Phy_802_15_4_fw.bin.

@note For this application it is requested to have:

  1 STM32WB55xx board loaded with: 
    - wireless coprocessor : stm32wb5x_Phy_802_15_4_fw.bin
    - application : Phy_802_15_4_Cli
            
  The commands are sent via an UART from an HyperTerminal (PC) to the STM32WB55RG_Nucleo Board.
  At startup. after typing return on the HyperTerminal, a prompt '802.15.4 valid cli >' should
  be displayed on the Hyperterminal.
  The red LED (LED3) is on.
  The Hyperterminal must be configured as follow :
  
      - BaudRate = 115200 baud  
      - Word Length = 8 Bits 
      - Stop Bit = 1 bit
      - Parity = none
      - Flow control = none


   ___________________________ 
  |                           |     
  |           ________________|                       _____________________________________
  |          |USART1          |                      |     HyperTerminal 1                 |
  |          |                |                      |=> Used to ctrl the stack via Cli Cmd|
  |          |                |                      |                                     |
  |          |                |                      |                                     |
  |          |                |______________________|                                     |
  |          |                |______________________| ST_Link virtual port                |
  |          |                |     ST_Link Cable    |                                     |             
  |          |                |                      |                                     |
  |          |                |                      |                                     |
  |          |                |                      |                                     |             
  |          |________________|                      |_____________________________________|          
  |                           |                       
  |                           |                      
  |_STM32WB55RG_Nucleo________|                      




  - Through these commands, it is possible to control and configure the board.
  
  - On the HyperTerminal, the user can type the command 'Help' in order to display the list of all
    commands available.

  - As an example, the user can launch the following commands
   
   802.15.4 valid cli >Help
      -> List all commands available
              
   802.15.4 valid cli >Set_channel 18 
       -> Set the channel to 18

   
@par Keywords

Zigbee, 802.15.4

@par Directory contents 

  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/app_common.h            	Header for all modules with common definition
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/app_entry.h             	Parameters configuration file of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/app_lld_tests.h          Header for lld test of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/gpio_lld.h             	Parameters for gpio configuration file of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/hw_conf.h           		Configuration file of the HW
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/hw_if.h           		Configuration file of the HW
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/main.h                  	Header for main.c module
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/stm_logging.h		    Header for stm_logging.c module
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/stm32_lpm_if.h          	Header for stm32_lpm_if.c module
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/app_entry.c      		Initialization of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/app_lld_tests.c          lld test of the application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/gpio_lld.c          	    GPIO for for application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/hw_ipcc.c 			    IPCC lld test Driver
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/hw_uart.c 			    UART Driver
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/main.c                  	Main program
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/stm_logging.c            Logging for application
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/stm32_lpm_if.c		    Low Power Manager Interface
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - Phy_802_15_4/Phy_802_15_4_Cli/Core/Src/stm32wbxx_hal_msp.c 		HAL MPS for application 
  
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55RG_Nucleo 
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

=> Loading of the stm32wb5x_Phy_802_15_4_fw.bin binary

  This application requests having the stm32wb5x_Phy_802_15_4_fw.bin binary flashed on the Wireless Coprocessor.
  If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
  All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
  Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
  Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
  Wireless Coprocessor binary. 



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
*/

