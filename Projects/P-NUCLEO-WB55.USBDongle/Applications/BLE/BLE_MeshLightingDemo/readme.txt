/**
  @page BLE_MeshLightingDemo example
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    BLE/BLE_MeshLightingDemo/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Lighting demo example.
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
This is the implementation of the BLE Mesh Lighting profile as specified by the BLE SIG.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
            mesh_lighting_demo
  - BLE/BLE_MeshLightingDemo/Core/Inc/app_common.h            	Header for all modules with common definition
  - BLE/BLE_MeshLightingDemo/Core/Inc/app_conf.h              	Parameters configuration file of the application
  - BLE/BLE_MeshLightingDemo/Core/Inc/app_entry.h            	Parameters configuration file of the application
  - BLE/BLE_MeshLightingDemo/Core/Inc/hw_conf.h           		Configuration file of the HW
  - BLE/BLE_MeshLightingDemo/Core/Inc/main.h                  	Header for main.c module
  - BLE/BLE_MeshLightingDemo/Core/Inc/stm32wbxx_hal_conf.h		HAL configuration file
  - BLE/BLE_MeshLightingDemo/Core/Inc/stm32wbxx_it.h          	Interrupt handlers header file
  - BLE/BLE_MeshLightingDemo/Core/Inc/utilities_conf.h    		Configuration file of the utilities
  - BLE/BLE_MeshLightingDemo/Core/Inc/vcp_conf.h    		    Configuration file of Virtual Com Port Interface
  - BLE/BLE_MeshLightingDemo/Core/Src/app_entry.c      		    Initialization of the application
  - BLE/BLE_MeshLightingDemo/Core/Src/stm32_lpm_if.c		    Low Power Manager Interface
  - BLE/BLE_MeshLightingDemo/Core/Src/hw_timerserver.c 		    Timer Server based on RTC
  - BLE/BLE_MeshLightingDemo/Core/Src/hw_uart.c 			    UART Driver
  - BLE/BLE_MeshLightingDemo/Core/Src/main.c                  	Main program
  - BLE/BLE_MeshLightingDemo/Core/Src/stm32wbxx_it.c          	Interrupt handlers
  - BLE/BLE_MeshLightingDemo/Core/Src/system_stm32wbxx.c      	stm32wbxx system source file
  - BLE/BLE_MeshLightingDemo/Core/Inc/app_ble.h               	Header for app_ble.c module
  - BLE/BLE_MeshLightingDemo/Core/Inc/tl_conf.h           		Configuration file of the Transport layer
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/app_ble.c      		BLE Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/app_ble.h      		Header of BLE Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_generic.c     BLE Mesh Generic Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_generic.h     Header of BLE Mesh Generic Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_light.c       BLE Mesh Light Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_light.h       Header of BLE Mesh Light Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_light_lc.c    BLE Mesh Light Lightness Controller Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_light_lc.h    Header of BLE Mesh Light Lightness Controller Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_mesh.c        BLE Mesh application implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_mesh.h        Header of BLE Mesh application implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_nvm.c         BLE Mesh NVM application implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_nvm.h         Header of BLE Mesh NVM application implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_sensor.c      BLE Mesh Sensor Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_sensor.h      Header of BLE Mesh Sensor Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_vendor.c      BLE Mesh Vendor Profile implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/appli_vendor.h      Header of BLE Mesh Vendor Profile implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/ble_conf.h          BLE Services configuration
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/ble_dbg_conf.h      BLE Traces configuration of the BLE services
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/hal_common.h        Header for common function of HAL file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/mesh_cfg.h         	Header for Mesh configuration
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/mesh_cfg_usr.h      Header for user Mesh configuration
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/models_if.h         Header for the BLE Mesh Models Interface file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/models_if.c         BLE Mesh Models Interface file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/pal_nvm.c           BLE Mesh NVM management implementation
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/pal_nvm.h           Header of BLE Mesh NVM management implementation 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/PWM_config.c        Pulse Width Modulation configuration
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/PWM_config.h        Header of Pulse Width Modulation configuration 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/PWM_handlers.c      Pulse Width Modulation handlers
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/PWM_handlers.h      Header of Pulse Width Modulation handlers 
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/svcctl_conf.c 		Service Controller configuration API
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/tl_conf.h           Configuration file of the Transport layer
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/tl_if.c 			Transport Layer interface
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/user_if.h 		    Header file for User interface file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/user_if.c 		    User interface file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/App/vcp_conf.h          Header Virtual Com Port configuration file
  - BLE/BLE_MeshLightingDemo/STM32_WPAN/Target/hw_ipcc.c     	IPCC Driver

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics USB DONGLE board (MB1293C)
    and can be easily tailored to any other supported device and development board.

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
 
For more details refer to the Application Note: 
  AN5292_STM32WB_How_to_build_BLE_Mesh_Application
  
* <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 