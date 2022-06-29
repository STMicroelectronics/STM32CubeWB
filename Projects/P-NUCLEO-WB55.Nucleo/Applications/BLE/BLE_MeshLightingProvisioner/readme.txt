/**
  @page BLE_MeshLightingProvisioner application
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    BLE/BLE_MeshLightingProvisioner/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Lighting Embedded Provisioner demo application.
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
This is the implementation of the BLE Mesh Lighting Embedded Provisioner as specified by the BLE SIG,
a Node with the capacity of creating MESH network from unprovisionned Nodes, like Proxy-Relay-Friend Nodes or
Low Power Nodes.

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
  - BLE_MeshLightingProvisioner/Core/Inc/app_common.h                  App Common application configuration file for STM32WPAN Middleware.
  - BLE_MeshLightingProvisioner/Core/Inc/app_conf.h                    Application configuration file for STM32WPAN Middleware.
  - BLE_MeshLightingProvisioner/Core/Inc/app_debug.h                   Header for app_debug.c module
  - BLE_MeshLightingProvisioner/Core/Inc/app_entry.h                   Interface to the application
  - BLE_MeshLightingProvisioner/Core/Inc/flash_driver.h                Dual core Flash driver interface
  - BLE_MeshLightingProvisioner/Core/Inc/hw_conf.h                     Configuration of hardware interface
  - BLE_MeshLightingProvisioner/Core/Inc/hw_if.h                       Hardware Interface
  - BLE_MeshLightingProvisioner/Core/Inc/lp_timer.h                    Header for lp_timer.c module
  - BLE_MeshLightingProvisioner/Core/Inc/main.h                        Header for main.c module
  - BLE_MeshLightingProvisioner/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file.
  - BLE_MeshLightingProvisioner/Core/Inc/stm32wbxx_it.h                This file contains the headers of the interrupt handlers.
  - BLE_MeshLightingProvisioner/Core/Inc/stm32_lpm_if.h                Header for stm32_lpm_if.c module (device specific LP management)
  - BLE_MeshLightingProvisioner/Core/Inc/utilities_conf.h              Configuration file for STM32 Utilities.
  - BLE_MeshLightingProvisioner/Core/Inc/vcp_conf.h                    Configuration of the vcp interface
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_config.h          Header file for the application config file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_config_client.h   Application interface for Generic Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_generic.h         Application interface for Generic Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_generic_client.h  Application interface for Generic Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light.h           Application interface for Light Mesh Model
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light_client.h    Application interface for Generic Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light_lc.h        Application interface for Light Control Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_mesh.h            Header file for the user application file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_nvm.h             Header file for the NVM application file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_sensor.h          Application interface for Light Mesh Model
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_sensors_client.h  Application interface for Sensor Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_vendor.h          Application interface for Vendor Mesh Models  
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/app_ble.h               Header for ble application
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/ble_conf.h              Configuration file for BLE Middleware.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/ble_dbg_conf.h          Debug configuration file for BLE Middleware.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/hal_common.h            Common functions of HAL file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/mesh_cfg_usr.h          user configurable settings 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/models_if.h             Mesh Modes interface file header 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/PWM_config.h            Header Configuration file for PWM.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/PWM_handlers.h          Header Handlers for PWM and other support functions.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/sensor_cfg_usr.h        sensor server initialization parameters 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/tl_conf.h               Transport Layer Configuration
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/user_if.h               Header file for User interface file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/vcp_conf.h              Configuration of the vcp interface
  - BLE_MeshLightingProvisioner/Core/Src/app_debug.c                   Debug capabilities source file for STM32WPAN Middleware
  - BLE_MeshLightingProvisioner/Core/Src/app_entry.c                   Entry point of the Application
  - BLE_MeshLightingProvisioner/Core/Src/flash_driver.c                Dual core Flash driver interface
  - BLE_MeshLightingProvisioner/Core/Src/hw_timerserver.c              Hardware timerserver source file for STM32WPAN Middleware.
  - BLE_MeshLightingProvisioner/Core/Src/hw_uart.c                     HW UART source file for STM32WPAN Middleware.
  - BLE_MeshLightingProvisioner/Core/Src/lp_timer.c                    Low power timer to be used within Mesh Application.
  - BLE_MeshLightingProvisioner/Core/Src/main.c                        BLE application with BLE core
  - BLE_MeshLightingProvisioner/Core/Src/stm32wbxx_hal_msp.c           This file provides code for the MSP Initialization 
                                                                       and de-Initialization codes.
  - BLE_MeshLightingProvisioner/Core/Src/stm32wbxx_it.c                Main Interrupt Service Routines.
                                                                       This file provides template for all exceptions handler and
                                                                       peripherals interrupt service routine.
  - BLE_MeshLightingProvisioner/Core/Src/stm32_lpm_if.c                Low layer function to enter/exit low power modes (stop, sleep).
  - BLE_MeshLightingProvisioner/Core/Src/system_stm32wbxx.c            CMSIS Cortex Device Peripheral Access Layer System Source File
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_config.c          Config Model APIs callback from ST BLE Mesh Library
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_config_client.c   Application interface for Config CLient Mesh Model
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_generic.c         Application interface for Generic Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_generic_client.c  Application interface for Generic Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light.c           Application interface for Lighting Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light_client.c    Application interface for Generic Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_light_lc.c        Application interface for light LC Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_mesh.c            User Application file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_nvm.c             User Application file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_sensor.c          Application interface for Sensor Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_sensors_client.c  Application interface for Generic Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/appli_vendor.c          Application interface for Vendor Mesh Models 
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/app_ble.c               BLE Application
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/models_if.c             Mesh Modes interface file of the application
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/pal_nvm.c               Flash management for the Controller
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/PWM_config.c            Configuration file for PWM.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/PWM_handlers.c          Handlers for PWM and other support functions.
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/svcctl_conf.c           Configuration of the BLE service controller
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/tl_if.c                 Transport layer interface to BLE
  - BLE_MeshLightingProvisioner/STM32_WPAN/app/user_if.c               User interface file 
  - BLE_MeshLightingProvisioner/STM32_WPAN/target/hw_ipcc.c            Hardware IPCC source file for STM32WPAN Middleware.

     
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

General default setting defined in mesh_cfg_usr.h:
        ENABLE_GENERIC_MODEL_SERVER_ONOFF                                    (1) /* GENERIC SERVER ONOFF MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_LEVEL                                    (1) /* GENERIC SERVER LEVEL MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF                              (1) /* GENERIC SERVER POWER ONOFF MODEL ON FIRST ELEMENT */
        ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP                        (1) /* GENERIC SERVER POWER ONOFF SETUP MODEL ON FIRST ELEMENT */
        ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS                                  (1) /* LIGHT LIGHTNESS SERVER MODEL ON FIRST ELEMENT */
        ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP                            (1) /* LIGHT LIGHTNESS SETUP SERVER MODEL ON FIRST ELEMENT */
        ENABLE_CONFIG_MODEL_CLIENT                                           (1) /* CONFIGURATION CLIENT MODEL ON FIRST ELEMENT */
        APPLICATION_NUMBER_OF_ELEMENTS                                         1 /* NUMBER OF ELEMENT SUPPORTED */ 
        APPLICATION_SIG_MODELS_MAX_COUNT                                       6 /* TOTAL NUMBER OF MODELS SUPPORTED */
        APPLICATION_VENDOR_MODELS_MAX_COUNT                                    1 /* TOTAL NIMBER OF VENDOR MODELS SUPPORTED */
        ENABLE_RELAY_FEATURE                                                     /* RELAY SUPPORT */
        ENABLE_PROXY_FEATURE                                                     /* PROXY SUPPORT FOR GATT CONNECTION */
        ENABLE_FRIEND_FEATURE                                                    /* FRIEND SUPPORT FOR FRIENDSHIP WITH LOW POWER NODE */
        DYNAMIC_PROVISIONER                                                      /* DYNAMIC PROVISIONER SUPPORT */
        ENABLE_PB_ADV                                                            /* Provisioning Bearer over advertising channels support */
        ENABLE_PB_GATT                                                           /* Provisioning Bearer over GATT proxy support */ 

Provisioner Node
Provisioner feature support is defined in mesh_cfg_usr.h by the predefinition of ENABLE_PROVISIONER_FEATURE or DYNAMIC_PROVISIONER.

When the ENABLE_PROVISIONER_FEATURE is defined after generating and flashing the binary, 
the Provisioner Node is self provisioned and configured during the initialization.

When the DYNAMIC_PROVISIONER is defined after generating, flashing the binary and the initialization the provisioner is like 
an unprovisioned device that can be provisioned and configured by 
BLE-Mesh Android (https://play.google.com/store/apps/details?id=com.st.bluenrgmesh&hl=en) or 
BLE-Mesh iOS (https://apps.apple.com/us/app/st-ble-mesh/id1348645067) smartphone application.
To dynamically set the device as a provisioner self provisioned and self configured, you can run the following command in an external
terminal: atep root
Here are the external terminal settings:
   baud rate of 115200
   Byte size of 8
   Parity None
   Stop bits 1
   Data Flow Control None
   
Establishment of Link by ID between a Provisioner and an unprovisioned device:

                     Provisioner                 Device 1                   Device 2                   Device 3
                         |                          |                          |                          |         
                        Scan                        |                          |                          |         
                         |<-----------------Unprovisioned Device Beacon (UUID 3, OOB info)----------------|         
                         |                          |                          |                          |         
                         |<---Unprovisioned Device--|                          |                          |         
                         | Beacon (UUID 1, OOB info)|                          |                          |         
                         |                          |                          |                          |         
                         |<---Unprovisioned Device Beacon (UUID 2, OOB info)---|                          |         
                         |                          |                          |                          |         
                 Provisioner chooses                |                          |                          |
                 to provision device                |                          |                          |
                    with UUID 2                     |                          |                          |
                         |                          |                          |                          |         
                         |-------------------Link Open (UUID 2)--------------->|                          |                   
                         |                          |                          |                          |         
                         |<----------------------Link Ack----------------------|                          |         
                         |                          |                          |                          |         
                                          PB-ADV Provisioning Process                                     |         
                         |                          |                          |                          |         
                         |<------------------Link Close (Reason)---------------|                          |         
                         |                          |                          |                          |         

PB-ADV Provisioning Process

                       Client                                                                         Server
                     Provisioner                                                                     New Device
                         |                                                                                |
               Scan for Un-provisioned                                                                    |
               Device, Link establishment                                                                 |               
                         |                                                                                |
                         |<-----------------------Un-provisioned Device Adv beacon------------------------|
                         |                                                                                |
                         |------------------------10% = Invite for Provisioning-------------------------->|
                         |                                                                                |
                         |<--------------------20% = Provisioning Capabilities sharing--------------------|
                         |                                                                                |
                         |------------------------30% = Provisioning start PDU--------------------------->|
                         |                                                                                |
                         |------------------------40% = Provisioning Public Key-------------------------->|
                         |                                                                                |
                  Calculate ECDH                                                                    Calculate ECDH
                         |                                                                                |
                         |<-----------------------50% = Provisioning Public Key---------------------------|
                         |                                                                                |
                      Session                                                                          Session     
                  Key calculation                                                                  Key calculation
                         |                                                                                |
                         |----------------60% = Provisioning Data (Network key, IV Index,---------------->|
                         |                      Unicast address, Key Index, Flags)                        |
                         |                                                                                |
                         |<-----------------------70% = Provisioning Completed----------------------------|
                         |                                                                                |
                   Disconnection                                                                    Disconnection
                                                                                             
Configuration Process                                                                        
                                                                                             
                     Provisioner                                                                   Provisioned Device
                         |                                                                                |
                   Scan for Proxy                                                                         |
                         |                                                                                |
                         |----------------------------Get Composition Data------------------------------->|
                         |                                                                                |
                         |<---------------------Status = Receive Composition data-------------------------|
                         |                                                                                |
                         |----------------------------80% = App Key Add---------------------------------->|
                         |                                                                                |
                         |<------------------------Status = App Key Status--------------------------------|
                         |                                                                                |
                         |-----------------90% = App Bind with Models selected by user------------------->|
                         |                                                                                |
                         |<------------------------Status = Binding Status--------------------------------|
                         |                                                                                |
                         |-------------100% = Add Subsciption with Models selected by user--------------->|
                         |                                                                                |
                         |<----------------------Status = Subscription Status-----------------------------|
                         |                                                                                |
                         |-------------100% = Set Publication Address for Client Node-------------------->|
                         |                                                                                |
                         |<----------------------Status = Publication Status------------------------------|
                         |                                                                                |
                    Configuration                                                                   Configuration
                     completed                                                                       completed

In the Provisioner terminal, after self provisioning and self configuration you can:
- scan for un-provisioned device with the command: atep scan
  The UUID addresses of the unprovisioned devices are displayed like as following:
  Device-0 -> F81D4FAE7DEC4B53A154F6FB0726E1C0
- provision and configure the un-provisioned device with the command: atep prvn-0 2
  0: is the scanned Device ID, 0 for Device-0 with the UUID address F81D4FAE7DEC4B53A154F6FB0726E1C0
  2: is the unicast address you want to associates to the provisioned node (1 is used for the provisioner Node)
  Many lines are displayed on the terminal, until the following one:
  38281 Appli_ConfigClient_ConfigureNode - **Node is configured**

By pressing [SW1] button on the Provisioner Node:
1. Provisioner Node publishes a LED ON command to the default group and the Provisioner Node blue LED immediately turns on.
2. As soon as the Provisioned Node receives the LED ON command, the blue LED immediately turns on.

                  Provisioner Node                                                                Provisioned Node
                        |                                                                                |
              SW1 PUSH->|                                                                                |
           Blue LED on->|                                                                                |
                        |--------------------------------LED on----------------------------------------->|<-Blue LED on
                        |                                                                                |
              SW1 PUSH->|                                                                                |
          Blue LED off->|                                                                                |
                        |--------------------------------LED off---------------------------------------->|<-Blue LED off
                        |                                                                                |

By pressing [SW1] button on the Provisioned Node:
1. Provisioned Node publishes a LED ON command to the default group and the Provisioned Node blue LED immediately turns on.
3. As soon as the Provisioner Node receives the LED ON command, the blue LED immediately turns on.

                  Provisioner Node                                                                Provisioned Node
                        |                                                                                |
                        |                                                                                |<-SW1 PUSH
                        |                                                                                |<-Blue LED on
           Blue LED on->|<--------------------------------LED on-----------------------------------------|
                        |                                                                                |
                        |                                                                                |<-SW1 PUSH
                        |                                                                                |<-Blue LED off
          Blue LED off->|<--------------------------------LED off----------------------------------------|
                        |                                                                                |

From Provisioner Node terminal following command can be sent:
- atcl c000 8202 01 01
  c000: default group address
  8202: opcode of the GENERIC ONOFF SET command (see chapter 7.1 Messages summary of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  01: The target value of the Generic OnOff state (see chapter 3.2.1.2 Generic OnOff Set of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  01: Transaction Identifier (see chapter 3.2.1.2 Generic OnOff Set of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  => Blue LED of Provisioner Node and Provisioned Node turns on.
From Provisioned Node terminal following command can be sent:
- atcl c000 8202 00 01
  c000: default group address
  8202: opcode of the GENERIC ONOFF SET command (see chapter 7.1 Messages summary of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  00: The target value of the Generic OnOff state (see chapter 3.2.1.2 Generic OnOff Set of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  01: Transaction Identifier (see chapter 3.2.1.2 Generic OnOff Set of the Mesh Model Bluetooth SIG Specification Revision v1.0)
  => Blue LED of Provisioner Node and Provisioned Node turns off.
  
Available Wiki pages:
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_overview
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_overview
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_LPN
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_Lighting
  - https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_MESH_Embedded_Provisioner


For more details refer to the Application Note: 
  AN5289 - Building a Wireless application
  AN5292 - STM32WB How to build BLE Mesh Application
 
* <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
