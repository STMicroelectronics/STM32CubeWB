/**
  @page BLE_TransparentMode example
  
  @verbatim
  ******************************************************************************
  * @file    BLE/BLE_TransparentMode/readme.txt 
  * @author  MCD Application Team
  * @brief   add here the very short description of the example (the name ?).
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

How to communicate with the STM32CubeMonitor-RF Tool using the transparent mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - BLE/BLE_TransparentMode/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_TransparentMode/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_TransparentMode/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_TransparentMode/STM32_WPAN/App/tm.h               	Header for tm.c module
  - BLE/BLE_TransparentMode/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_TransparentMode/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_TransparentMode/Core/Inc/app_entry.h              	Parameters configuration file of the application
  - BLE/BLE_TransparentMode/STM32_WPAN/App/ble_conf.h          	BLE Services configuration
  - BLE/BLE_TransparentMode/STM32_WPAN/App/ble_dbg_conf.h      	BLE Traces configuration of the BLE services
  - BLE/BLE_TransparentMode/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_TransparentMode/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_TransparentMode/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_TransparentMode/Core/Src/main.c                  	Main program
  - BLE/BLE_TransparentMode/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_TransparentMode/Core/Src/app_entry.c      			Initialization of the application
  - BLE/BLE_TransparentMode/STM32_WPAN/Target/hw_ipcc.c      	IPCC Driver
  - BLE/BLE_TransparentMode/Core/Src/stm32_lpm_if.c				Low Power Manager Interface
  - BLE/BLE_TransparentMode/Core/Src/hw_timerserver.c 			Timer Server based on RTC
  - BLE/BLE_TransparentMode/Core/Src/hw_uart.c 					UART Driver
  - BLE/BLE_TransparentMode/STM32_WPAN/App/tm.c 				Transparent Mode implementation
  
     
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
 - Rebuild all files and load your image into Target memory
 - OR use BLE_TransparentMode_reference.hex from Binary directory
 - to test the BLE Transparent Mode application, use the STM32CubeMonitor RF tool
 - make the connection between STM32CubeMonitor RF tool and BLE_TransparentMode application
 - send some ACI commands from STM32CubeMonitor RF tool as HCI_RESET, HCI_LE_RECEIVER_TEST, HCI_LE_TRANSMITTER_TEST, ...
 - the application must acknowledge the command with a "Command Complete" answer 
 
For more details refer to the Application Note: 
  AN5289 - Building a Wireless application 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 