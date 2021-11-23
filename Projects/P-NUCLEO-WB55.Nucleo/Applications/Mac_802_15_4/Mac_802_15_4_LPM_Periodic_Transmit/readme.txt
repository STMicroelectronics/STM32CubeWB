/**
  @page Mac_802_15_4_LPM_Periodic_Transmit application
  
  @verbatim
  ******************************************************************************
  * @file    Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/readme.txt 
  * @author  MCD Application Connectivity Team
  * @brief   Example of application using MAC 802.15.4 protocols demonstrating
             how to periodically transmit data base on LPTIM timeout with low 
             power mode activated
  ******************************************************************************
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description

How to use MAC 802.15.4 data transmission with STOP1 low power mode enabled.

This application requires one STM32WB55xx-Nucleo board. The device is silently started
as an end device. Once device initialized, the node periodically (every seconds) broadcast 
data over the air. The device goes in low power mode after each transmission and is awakened by
 low power timer's interrupt.
 

@par Keywords

MAC, 802.15.4, low power

@par Directory contents 


Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit

   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/app_common.h                                      Header for all modules with common definition
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/app_conf.h                                        Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/app_entry.h                                       Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/hw_conf.h                                         Configuration file of the HW
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/main.h                                            Header for main.c module
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm_logging.h                                     Application header file for logging
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm32wbxx_hal_conf.h                              STM32 HAL configuration file.
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm32wbxx_it.h                                    Interrupt handlers header file
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/utilities_conf.h                                  Header for Configuration of utilities
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/STM32_WPAN/App/app_lpm_transmit_mac_802_15_4_process.h     Header for Low Power End device Proxy Mac callback implementation
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/STM32_WPAN/App/app_lpm_transmit_mac_802_15_4.h             Header for Low Power End device  MAC 802.15.4 processing tasks
   
   
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/app_entry.c                                      Parameters configuration file of the application
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm32_lpm_if.c                                   Low Power Manager Interface
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/hw_uart.c                                        UART Driver
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/main.c                                           Main program
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm_logging.c                                    Logging Implementation
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/stm32wbxx_it.c                                   exceptions handler and peripherals interrupt service routine
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/Core/Inc/system_stm32wbxx.c                               CMSIS Cortex Device Peripheral Access Layer implementation
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/STM32_WPAN/App/app_lpm_transmit_mac_802_15_4_process.c    Low Power End device Proxy Mac callback implementation implementation
   - Mac_802_15_4/Mac_802_15_4_LPM_Periodic_Transmit/STM32_WPAN/App/app_lpm_transmit_mac_802_15_4.c            Low Power End device MAC 802.15.4 processing implementation
 

  
 
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
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

This application requests having the stm32wb5x_Mac_802_15_4_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary. 


Minimum requirements for the demo:

- 1 STM32WB55xx-Nucleo board in MAC 802.15.4 mode 
using current generated binary.

In order to make the program work, you must do the following: 
 - Connect a STM32WB55xx-Nucleo board to your PC 
 
         - Open your preferred toolchain 
         - Rebuild all files and load your image into the 
          target memory 
         - The example run on Reset

  User may use a MAC 802.15.4 other the air sniffer on channel 16 to spy 
 MAC 802.15.4 broadcasted frames.
  
**** DEMO ****

This MAC 802.15.4 example is to demonstrate a periodic data transmission from an End device configured in
low power mode.   

By default, on reset, the board starts as the RF Protocol Core (ie.: M0+). Once the device and its MAC layer 
are configured, the 802.15.4 node is silently stated and enter in low power mode.
A timer driven by the LPTIM1 is launch and configured to issue a Timeout interruption.
On Timeout event, the application core exit from the low power state and a data transmit data is requested by 
transmitter task. The application core returns in low power state until it received the MCPS data confirmation
from the RF protocol core. Before returning to the low power state, a new timer is then triggered on MCPS data 
confirmation to schedule a new data transmission. The demo runs in an infinite loops.
If user want to restart the demo, he may simply reset the device.

Power consumption can be measured using the X-Nucleo-LPM01A board solution.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */