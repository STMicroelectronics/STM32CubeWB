/**
  @page BLE_MeshLightingPRFNode application
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    BLE/BLE_MeshLightingPRFNode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Lighting Proxy Relay Friend Node demo application.
  ******************************************************************************
  *
  * Copyright (c) 2019-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par application Description 
This is the implementation of the BLE Mesh Lighting profile as specified by the BLE SIG,
a Proxy-Relay-Friend Node with the capacity to handle a Friendship with a Low Power Node.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE mesh, BLE pairing, BLE profile, Dual core

@par Directory contents
  - BLE_MeshLightingPRFNode/Core/Inc/app_common.h                  App Common application configuration file for STM32WPAN Middleware.
  - BLE_MeshLightingPRFNode/Core/Inc/app_conf.h                    Application configuration file for STM32WPAN Middleware.
  - BLE_MeshLightingPRFNode/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE_MeshLightingPRFNode/Core/Inc/app_entry.h                   Interface to the application
  - BLE_MeshLightingPRFNode/Core/Inc/flash_driver.h                Dual core Flash driver interface
  - BLE_MeshLightingPRFNode/Core/Inc/hw_conf.h                     Configuration of hardware interface
  - BLE_MeshLightingPRFNode/Core/Inc/hw_if.h                       Hardware Interface
  - BLE_MeshLightingPRFNode/Core/Inc/lp_timer.h                    Header for lp_timer.c module
  - BLE_MeshLightingPRFNode/Core/Inc/main.h                        Header for main.c module
  - BLE_MeshLightingPRFNode/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file.
  - BLE_MeshLightingPRFNode/Core/Inc/stm32wbxx_it.h                This file contains the headers of the interrupt handlers.
  - BLE_MeshLightingPRFNode/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module (device specific LP management)
  - BLE_MeshLightingPRFNode/Core/Inc/utilities_conf.h              Configuration file for STM32 Utilities.
  - BLE_MeshLightingPRFNode/Core/Inc/vcp_conf.h                    Configuration of the vcp interface
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_config.h          Header file for the application config file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_config_client.h   Application interface for Generic Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_generic.h         Application interface for Generic Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_generic_client.h  Application interface for Generic Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light.h           Application interface for Light Mesh Model
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light_client.h    Application interface for Generic Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light_lc.h        Application interface for Light Control Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_mesh.h            Header file for the user application file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_nvm.h             Header file for the NVM application file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_sensor.h          Application interface for Light Mesh Model
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_sensors_client.h  Application interface for Sensor Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_vendor.h          Application interface for Vendor Mesh Models  
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/app_ble.h               Header for ble application
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/ble_conf.h              Configuration file for BLE Middleware.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/ble_dbg_conf.h          Debug configuration file for BLE Middleware.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/hal_common.h            Common functions of HAL file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/mesh_cfg_usr.h          user configurable settings 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/models_if.h             Mesh Modes interface file header 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/PWM_config.h            Header Configuration file for PWM.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/PWM_handlers.h          Header Handlers for PWM and other support functions.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/sensor_cfg_usr.h        sensor server initialization parameters 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/tl_conf.h               Transport Layer Configuration
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/user_if.h               Header file for User interface file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/vcp_conf.h              Configuration of the vcp interface
  - BLE_MeshLightingPRFNode/Core/Src/app_debug.c                   Debug capabilities source file for STM32WPAN Middleware
  - BLE_MeshLightingPRFNode/Core/Src/app_entry.c                   Entry point of the Application
  - BLE_MeshLightingPRFNode/Core/Src/flash_driver.c                Dual core Flash driver interface
  - BLE_MeshLightingPRFNode/Core/Src/hw_timerserver.c              Hardware timerserver source file for STM32WPAN Middleware.
  - BLE_MeshLightingPRFNode/Core/Src/hw_uart.c                     HW UART source file for STM32WPAN Middleware.
  - BLE_MeshLightingPRFNode/Core/Src/lp_timer.c                    Low power timer to be used within Mesh Application.
  - BLE_MeshLightingPRFNode/Core/Src/main.c                        BLE application with BLE core
  - BLE_MeshLightingPRFNode/Core/Src/stm32wbxx_hal_msp.c           This file provides code for the MSP Initialization 
                                                                   and de-Initialization codes.
  - BLE_MeshLightingPRFNode/Core/Src/stm32wbxx_it.c                Main Interrupt Service Routines.
                                                                   This file provides template for all exceptions handler and
                                                                   peripherals interrupt service routine.
  - BLE_MeshLightingPRFNode/Core/Src/stm32_lpm_if.c                Low layer function to enter/exit low power modes (stop, sleep).
  - BLE_MeshLightingPRFNode/Core/Src/system_stm32wbxx.c            CMSIS Cortex Device Peripheral Access Layer System Source File
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_config.c          Config Model APIs callback from ST BLE Mesh Library
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_config_client.c   Application interface for Config CLient Mesh Model
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_generic.c         Application interface for Generic Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_generic_client.c  Application interface for Generic Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light.c           Application interface for Lighting Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light_client.c    Application interface for Generic Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_light_lc.c        Application interface for light LC Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_mesh.c            User Application file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_nvm.c             User Application file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_sensor.c          Application interface for Sensor Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_sensors_client.c  Application interface for Generic Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/appli_vendor.c          Application interface for Vendor Mesh Models 
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/app_ble.c               BLE Application
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/models_if.c             Mesh Modes interface file of the application
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/pal_nvm.c               Flash management for the Controller
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/PWM_config.c            Configuration file for PWM.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/PWM_handlers.c          Handlers for PWM and other support functions.
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/svcctl_conf.c           Configuration of the BLE service controller
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/tl_if.c                 Transport layer interface to BLE
  - BLE_MeshLightingPRFNode/STM32_WPAN/app/user_if.c               User interface file 
  - BLE_MeshLightingPRFNode/STM32_WPAN/target/hw_ipcc.c            Hardware IPCC source file for STM32WPAN Middleware.
     
@par Hardware and Software environment

  - This application runs on STM32WB55xx devices.
  
  - This application has been tested with an STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device 
    and development board.

  - This application is by default configured to support low power mode ( No traces - No debugger )
    This can be modified in app_conf.h (CFG_LPM_SUPPORTED, CFG_DEBUG_BLE_TRACE, CFG_DEBUG_APP_TRACE)

  - SMPS can be used to obtain the best performance in power consumption.
    This feature depends on board components and its activation can be done through specific define (CFG_USE_SMPS)
    configurable in app_conf.h
    See AN5246 - Usage of STM32WB Series microcontrollers.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to change the
Wireless Coprocessor binary or see following wiki for Hardware setup:
https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and flash the board with the executable file
 
For more details refer to the Application Note: 
  AN5292_STM32WB_How_to_build_BLE_Mesh_Application
  
@Introduction

The ST BLE-Mesh library supports all of the following features in a Mesh node:
1. Relay: to forward received network PDUs on advertising or GATT bearer, if the Proxy feature is enabled
2. Proxy: to forward received network PDUs between GATT and advertising bearers
3. Friend: can perform duties such as scanning for other associated nodes that are usually Low Power mode
4. Low Power: consumes much less power as the node usually does not perform any scanning
A typical BLE Mesh node radio should always be in scanning mode to avoid missing packets over the air, but this kind of
operation is not supported by many devices due to continuous energy requirements.
To enable low power operation, BLE Mesh Profile v1.0 defines the concept of Friendship.
In a Mesh network, a Low Power feature-enabled Mesh node (LPN) offloads its packet receiving (scanning) duty to an adjacent
Friend feature-enabled Mesh node (FN). The LPN is not always scanning and remains inactive (or sleeps) for most of its life
cycle, significantly reducing its energy requirements. Nodes running on batteries or energy harvesting techniques usually
support the Low Power feature.
Once connected to each other, the FN and LPN are considered friends and their connection is known as a Friendship. At least
one FN must be in the direct radio range of an LPN to establish Friendship.
  
@Configuration

Low Power Node
Low Power feature support is defined in mesh_cfg_usr.h by the predefinition of ENABLE_LOW_POWER_FEATURE.
Note: A Low Power feature-enabled node does not support any other feature: all the other features should be
undefined.

@Demo setup based on STM32WB55RG-Nucleo

General default setting defined in mesh_cfg_usr.h:
        ENABLE_GENERIC_MODEL_SERVER_ONOFF                                    (1) /* GENERIC SERVER ONOFF MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_LEVEL                                    (1) /* GENERIC SERVER LEVEL MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF                              (1) /* GENERIC SERVER POWER ONOFF MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP                        (1) /* GENERIC SERVER POWER ONOFF SETUP MODEL ON FIRST ELEMENT */
        ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS                                  (1) /* LIGHT LIGHTNESS SERVER MODEL ON FIRST ELEMENT */
        ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP                            (1) /* LIGHT LIGHTNESS SETUP SERVER MODEL ON FIRST ELEMENT */
        APPLICATION_NUMBER_OF_ELEMENTS                                         1 /* NUMBER OF ELEMENT SUPPORTED */ 
        APPLICATION_SIG_MODELS_MAX_COUNT                                       6 /* TOTAL NUMBER OF MODELS SUPPORTED */
        APPLICATION_VENDOR_MODELS_MAX_COUNT                                    1 /* TOTAL NIMBER OF VENDOR MODELS SUPPORTED */
        ENABLE_RELAY_FEATURE                                                     /* RELAY SUPPORT */
        ENABLE_PROXY_FEATURE                                                     /* PROXY SUPPORT FOR GATT CONNECTION */
        ENABLE_FRIEND_FEATURE                                                    /* FRIEND SUPPORT FOR FRIENDSHIP WITH LOW POWER NODE */
        ENABLE_PB_ADV                                                            /* Provisioning Bearer over advertising channels support */
        ENABLE_PB_GATT                                                           /* Provisioning Bearer over GATT proxy support */ 

The following demo based on STM32WB55RG-Nucleo has been set up using one board as a Friend node and two
boards as Low Power nodes.

Low Power Node setup (use the Low Power Node BLE_MeshLightingLPN project):
Step 1. Configure LPN parameters.
        Change only if required otherwise skip this step (use application parameters defined in mesh_cfg_usr.h)
	    LPN_RSSI_FACTOR_LEVEL             1
        LPN_RECIVE_WINDOW_FACTOR_LEVEL    1
        LPN_MINIMUM_QUEUE_SIZE_LOG        2
        LPN_RECEIVE_DELAY                 150 ms
        LPN_POLL_TIMEOUT                  2000 (*100 ms)
        LPN_RECEIVE_WINDOW_SIZE           55 ms
        LPN_SUBSCR_LIST_SIZE              2
        LPN_FRIEND_REQUEST_FREQUENCY      50 (*100 ms)
        LPN_FRIEND_POLL_FREQUENCY         25 (*100 ms)
        LPN_MINIMUM_RSSI                  -100
        LPN_NO_OF_RETRIES                 10
Step 2. Generate and flash binary in 2 boards (LPN1, LPN2)

Friend Node setup (use the Proxy Relay Friend Node BLE_MeshLightingPRFNode project):
Step 1. Generate and flash binary in a board (FN)
Step 2. Provision the 3 boards with the default parameters, using BLE-Mesh Android (https://play.google.com/store/apps/details?id=com.st.bluenrgmesh&hl=en) or 
        BLE-Mesh iOS (https://apps.apple.com/us/app/st-ble-mesh/id1348645067) smartphone application (subscribe and publish to default group)


Demo operation
After provisioning, it might take a few seconds for Friendship to be established between FN and LPNs.
In this demo, the FN establishes Friendship with LPN1 and LPN2, LPN2 is optional.
Any packet targeted to LPN1/LPN2 or to a group address subscribed by LPN1 or LPN2 is put in queue by the FN
that forwards packets to LPN1/LPN2 as soon as it receives a Friend Poll from the respective LPN.
The LPN is free to send packets (generate traffic) any time.

By pressing [SW1] button on the Friend Node:
1. FN publishes a LED ON command to the default group and the FN blue LED immediately turns on (when the
FN belongs to the default group).
2. FN forwards a LED ON command to the LPNs subscribed to the default group. As soon as the FN receives
poll from an LPN, it sends LED ON to the respective LPN.

                     FN              LPN1            LPN2
                     |                |               |
           SW1 PUSH->|                |               |
        Blue LED on->|                |               |
                     |<--Friend Poll--|               |
                     |                |               |
                     |-----LED on---->|<-Blue LED on  |
                     |                                |
                     |<----------Friend Poll----------|
                     |                                |
                     |-------------LED on------------>|<-Blue LED on
                     |                                |

By pressing [SW1] button on the Low Power Node 1:
1. LPN1 publishes an LED OFF command to the default group. LPN1 blue LED tunrs off. 
   LPN1 immediately forwards LED OFF command to the default group.
2. FN receives an LED OFF command (as it is subscribed to the default group) and the FN blue LED turns off.
3. FN also sends an LED OFF command to LPN2 (subscribed to the default group) as soon as FN receives
   poll from LPN2.

                     FN              LPN1            LPN2
                     |                |               |
                     |      SW1 PUSH->|<-Blue LED off |
       Blue LED off->|<----LED off----|               |
                     |                                |
                     |<----------Friend Poll----------|
                     |                                |
                     |------------LED off------------>|<-Blue LED off
                     |                                |

Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_overview
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_LPN
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_Lighting


For more details refer to the Application Note: 
  AN5289 - Building a Wireless application
  AN5292 - STM32WB How to build BLE Mesh Application
 
* <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
