/**
  @page BLE_MeshLightingProvisioner example
  
  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    BLE/BLE_MeshLightingProvisioner/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Mesh Lighting Embedded Provisioner demo example.
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

Connectivity, BLE, IPCC, HSEM, RTC, UART, PWR, BLE protocol, BLE mesh, Dual core

@par Directory contents 
            mesh_lighting_demo
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/app_common.h                  Header for all modules with common definition
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/app_conf.h                    Parameters configuration file of the application
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/app_debug.h                   Interface to support debug in the application
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/app_entry.h                   Parameters configuration file of the application
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/hw_conf.h                     Configuration file of the HW
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/hw_flash.h                    Configuration file of the FLASH driver needed by EE module
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/hw_if.h                       Configuration file of the Hardware Interface
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/lp_timer.h                    Configuration file of the Low power timer to be used within Mesh Application.
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/main.h                        Header for main.c module
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/stm32wbxx_hal_conf.h          HAL configuration file
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/stm32_lpm_if.h                Configuration file of the Low layer function to enter/exit low power modes (stop, sleep).
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/utilities_conf.h              Configuration file of the utilities
  - BLE/BLE_MeshLightingProvisioner/Core/Inc/vcp_conf.h                    Configuration file of Virtual Com Port Interface
  - BLE/BLE_MeshLightingProvisioner/Core/Src/app_debug.c                   Interface to support debug in the application
  - BLE/BLE_MeshLightingProvisioner/Core/Src/app_entry.c                   Initialization of the application
  - BLE/BLE_MeshLightingProvisioner/Core/Src/hw_flash.c                    FLASH driver needed by EE module
  - BLE/BLE_MeshLightingProvisioner/Core/Src/hw_timerserver.c              Timer Server based on RTC
  - BLE/BLE_MeshLightingProvisioner/Core/Src/hw_uart.c                     UART Driver
  - BLE/BLE_MeshLightingProvisioner/Core/Src/lp_timer.c                    Low power timer to be used within Mesh Application.
  - BLE/BLE_MeshLightingProvisioner/Core/Src/main.c                        Main program
  - BLE/BLE_MeshLightingProvisioner/Core/Src/stm32_lpm_if.c                Low Power Manager Interface
  - BLE/BLE_MeshLightingProvisioner/Core/Src/stm32wbxx_it.c                Interrupt handlers
  - BLE/BLE_MeshLightingProvisioner/Core/Src/system_stm32wbxx.c            stm32wbxx system source file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/app_ble.c               BLE Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/app_ble.h               Header of BLE Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_config.c          Application interface for Config Server Mesh Model
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_config.h          Header of Application interface for Config Server Mesh Model 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_config_client.c   Application interface for Config CLient Mesh Model
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_config_client.h   Header of Application interface for Config CLient Mesh Model 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_generic.c         Application interface for Generic Mesh Models
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_generic.h         Header of Application interface for Generic Mesh Models 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_generic_client.c  Application interface for Client Generic Mesh Models
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_generic_client.h  Header of Application interface for Client Generic Mesh Models 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light.c           BLE Mesh Light Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light.h           Header of BLE Mesh Light Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light_client.c    Application interface for Client Light Mesh Models
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light_client.h    Header of Application interface for Client Light Mesh Models 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light_lc.c        BLE Mesh Light Lightness Controller Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_light_lc.h        Header of BLE Mesh Light Lightness Controller Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_mesh.c            BLE Mesh application implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_mesh.h            Header of BLE Mesh application implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_nvm.c             BLE Mesh NVM application implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_nvm.h             Header of BLE Mesh NVM application implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_sensor.c          BLE Mesh Server Sensor Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_sensor.h          Header of BLE Mesh Server Sensor Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_sensor_client.c   BLE Mesh Client Sensor Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_sensor_client.h   Header of BLE Mesh Client Sensor Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_vendor.c          BLE Mesh Vendor Profile implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/appli_vendor.h          Header of BLE Mesh Vendor Profile implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/ble_conf.h              BLE Services configuration
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/ble_dbg_conf.h          BLE Traces configuration of the BLE services
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/hal_common.h            Header for common function of HAL file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/mesh_cfg.h              Header for Mesh configuration
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/mesh_cfg_usr.h          Header for user Mesh configuration
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/models_if.h             Header for the BLE Mesh Models Interface file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/models_if.c             BLE Mesh Models Interface file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/pal_nvm.c               BLE Mesh NVM management implementation
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/pal_nvm.h               Header of BLE Mesh NVM management implementation 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/PWM_config.c            Pulse Width Modulation configuration
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/PWM_config.h            Header of Pulse Width Modulation configuration 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/PWM_handlers.c          Pulse Width Modulation handlers
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/PWM_handlers.h          Header of Pulse Width Modulation handlers 
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/sensor_cfg_usr.c        Header for user Mesh Sensor Model configuration
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/svcctl_conf.c           Service Controller configuration API
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/tl_conf.h               Configuration file of the Transport layer
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/tl_dbg_conf.h           Debug configuration file for stm32wpan transport layer interface.
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/tl_if.c                 Transport Layer interface
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/user_if.h               Header file for User interface file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/user_if.c               User interface file
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/App/vcp_conf.h              Configuration of the vcp interface
  - BLE/BLE_MeshLightingProvisioner/STM32_WPAN/Target/hw_ipcc.c            IPCC Driver

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55RG-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

This application requires having the stm32wb5x_BLE_Stack_full_fw.bin binary flashed on the Wireless Coprocessor.
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
  
* <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 