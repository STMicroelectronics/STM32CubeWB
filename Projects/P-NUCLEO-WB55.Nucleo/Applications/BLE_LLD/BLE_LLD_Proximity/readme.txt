/**
  @page BLE_LLD_Proximity example
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    BLE_LLD/BLE_LLD_Proximity/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE LLD BLE_LLD_Proximity application
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

How to use BLE LLD to detect nearby boards?

This solution can be used in contact tracing for Covid-19 since it provides an
estimation of the distance with other boards.

For this to work, all the boards use the same basic protocol:
  - periodically sends its unique ID
  - listens for nearby boards’ IDs the rest of the time

Pitfalls:
  - since this protocol uses broadcasts, acknowledgment must not be sent
  - to ensure fairness, all the boards must use the same period
  - with a perfectly fixed period, if 2 boards transmit at the same time, their
    transmissions will always collide, so a random offset is added at the end
    of each period before actual transmission to avoid that.

Improvements:
No power save is implemented for now, but this can be added using the same
protocol.

@note BLE LLD and HAL overview
  LLD and HAL have the same goal, provide low level radio communication between
  devices, using BLE radio hardware.

  LLD is implemented on top of BLE PHY.
  This layer provides the most control over radio core, so is more complicated
  to use than HAL.

  HAL is implemented on top of LLD.
  This layer provides basic control over the radio core, so is more simple to
  use than LLD.

This application only uses HAL.

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
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/proximity_app.h         Header for Proximity Application proximity_app.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/main.h                  Header for main.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm_logging.h	         Header for stm_logging.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32_lpm_if.h          Header for stm32_lpm_if.c module
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Inc/utilities_conf.h        Configuration file of the utilities
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/app_entry.c             Initialization of the application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/gpio_lld.c              GPIO for application
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/hw_uart.c               UART Driver
  - BLE_LLD/BLE_LLD_Pressbutton/Core/Src/proximity_app.c         Proximity program
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

@par How to use it?

Follow this procedure to run the demo:
 - Connect at least 2 STM32WB55xx-Nucleo boards to your PC
 - Open your preferred toolchain 
 - Rebuild all files and load your image into one target memory
 - Rebuild all files of BLE_LLD/BLE_LLD_Proximity application 
   and load your image into the other target memory
   + load stm32wb5x_BLE_LLD_fw.bin
 - With a terminal emulator, open a connection to the serial port of one board
   (115200 bauds 8 bits no parity 1 stop bit)
 - Move boards relatively to each other to see updates
 
Example :
@verbatim
Device ID: 26321

Nearby devices:
    ID   | power | timestamp
----------------------------
   47772 |   207 |  9312368
  521961 |   177 |  9558549
@endverbatim

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */