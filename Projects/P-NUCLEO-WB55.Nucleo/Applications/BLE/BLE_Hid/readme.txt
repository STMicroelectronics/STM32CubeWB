/**
  @page BLE_Hid example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_Hid/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Human Interface Device example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the Human Interface Device profile as specified by the BLE SIG.

Add here a detailled but simple description of the example. this description will be
reused inside the application note which list all the example. This paragraph end when
a "." is at the end of a line like the following one ==> .

Add here the behavior of the example.
It is free text.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Add here all the importants words of the example separated by "," + $moduleName$ $exampleName$
$moduleName$, $exampleName$, Security, IEC 60870-5, hardware CRC, 

@par Directory contents 
  
  - BLE/BLE_Hid/Core/Inc/stm32wbxx_hal_conf.h	    HAL configuration file
  - BLE/BLE_Hid/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_Hid/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_Hid/STM32_WPAN/App/app_ble.h                Header for app_ble.c module
  - BLE/BLE_Hid/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_Hid/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_Hid/Core/Inc/app_entry.h                    Parameters configuration file of the application
  - BLE/BLE_Hid/STM32_WPAN/App/ble_conf.h               BLE Services configuration
  - BLE/BLE_Hid/STM32_WPAN/App/ble_dbg_conf.h           BLE Traces configuration of the BLE services
  - BLE/BLE_Hid/STM32_WPAN/App/hids_app.h               Header for hids_app.c module
  - BLE/BLE_Hid/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_Hid/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_Hid/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_Hid/Core/Src/main.c                  	Main program
  - BLE/BLE_Hid/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_Hid/STM32_WPAN/App/app_ble.c      	        BLE Profile implementation
  - BLE/BLE_Hid/Core/Src/app_entry.c                    Initialization of the application
  - BLE/BLE_Hid/STM32_WPAN/App/hids_app.c      	        Human Interface Device Service application
  - BLE/BLE_Hid/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_Hid/Core/Src/stm32_lpm_if.c			Low Power Manager Interface
  - BLE/BLE_Hid/Core/Src/hw_timerserver.c               Timer Server based on RTC
  - BLE/BLE_Hid/Core/Src/hw_uart.c                      UART Driver


     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55VG-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requests having the stm32wb5x_BLE_Stack_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary.  

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
On the Nucleo board side:
 - Power on the Nucleo board with the BLE_Hid application

On the PC side:
 - enable the Bluetooth 4.0 communication or install any Bluetooth 4.0 USB adapter dongle
 - do a connection to the STM32WB55xx device with PIN code 111111.
 - open a terminal window with the following settings:
   baud rate of 115200
   Byte size of 8
   Parity None
   Stop bits 1
   Data Flow Control None
 
On the Nucleo board side:
 Each action on the SW1 button display a menu in the terminal window, a kind of a mouth 
 with 3 button and a wheel emulation.
 In this menu you can toggle each mouth button state, set the vertical an horizontal position
 of the mouse pointer between -127 to +127, set the wheel indentation between -127 to +127.
 When you validate the Save & Quit menu item the setting becomes effective and you can 
 see the result on the screen of the PC.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 