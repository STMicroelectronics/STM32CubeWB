/**
  @page Ble_Thread_Dyn example
  
  @verbatim
  ******************************************************************************
  * @file    BLE_Thread/Ble_Thread_Dyn/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of application using BLE and Thread protocols in Dynamic
  *          Concurrent mode.
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

How to use BLE application and Thread application in dynamic concurrent mode.

This application requires at least 2 STM32WB55xx-Nucleo board with Dynamic Concurrent Mode Application.
Dynamic means that application can perform at the same time BLE or Thread activities. 

The idea is to present a demo with a Thread Network (at least 2 Nucleo Boards
running Ble_Thread_Dynamic application),and a a BLE connection between 1 Nucleo board running Ble_Zigbee_Dynamic application [2] with either a 
smartphone running "ST BLE Sensor" Application (available on App Store and Google Play), or another
Nucleo board running a BLE_p2pClient application.

The STM32WB55xx-Nucleo board running Ble_Thread_Dynamic application is capable 
of BLE and OpenThread activity at the same time.

@par Keywords

COAP,THREAD,BLE

@par Directory contents 

  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/app_ble.h            Header for app_ble.c module
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Inc/app_common.h          Header for all modules with common definition
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Inc/app_conf.h            Parameters configuration file of the application
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Inc/app_entry.h           Parameters configuration file of the application
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/app_thread.h         Header for app_thread.c module
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/ble_conf.h           BLE Services configuration
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/ble_dbg_conf.h       BLE Traces configuration of the BLE services
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Inc/hw_conf.h             Configuration file of the HW
  - BLE_Thread/Ble_Thread_Dyn/Core/Inc/main.h                     Header for main.c module
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/p2p_server_app.h     Header for BLE P2P Server application
  - BLE_Thread/Ble_Thread_Dyn/Core/Inc/stm32wbxx_hal_conf.h       HAL configuration file
  - BLE_Thread/Ble_Thread_Dyn/Core/Inc/stm32wbxx_it.h             Interrupt handlers header file
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/app_ble.c            BLE Profile implementation
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Src/app_entry.c           Initialization of the application
  - BLE_Thread/Ble_Thread_Dyn/Core/Src/app_thread.c               Thread application implementation
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/Target/hw_ipcc.c         IPCC Driver
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Src/stm32_lpm_if.c        Low Power Manager Interface
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Src/hw_timerserver.c      Timer Server based on RTC
  - BLE_Thread/Ble_Thread_Dyn/Core/Core/Src/hw_uart.c             UART Driver
  - BLE_Thread/Ble_Thread_Dyn/Core/Src/main.c                     Main program
  - BLE_Thread/Ble_Thread_Dyn/STM32_WPAN/App/p2p_server_app.c     BLE P2P Server application implementation
  - BLE_Thread/Ble_Thread_Dyn/Core/Src/stm32wbxx_it.c             Interrupt handlers
  - BLE_Thread/Ble_Thread_Dyn/Core/Src/system_stm32wbxx.c         stm32wbxx system source file
  
 
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

This application requests having the STM32WB5x_BLE_Thread_dynamic_fw.bin binary flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate binary.
All available binaries are located under /Projects/STM32_Copro_Wireless_Binaries directory.
Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the detailed procedure to load the proper
Wireless Coprocessor binary. 


Minimum requirements for the demo:
- 2 STM32WB55xx-Nucleo board in BLE Thread Dynamic Concurrent mode
- 1 STM32WB55xx-Nucleo board running a BLE_p2pClient application
-- OR 
- 2 STM32WB55xx-Nucleo board in BLE Thread Dynamic Concurrent mode 
- 1 Smartphone (Android/IOS) with "ST BLE Sensor" Phone Application (available on App Store and Google Play)
  Install the "ST BLE Sensor" application on the ios/android device
  https://play.google.com/store/apps/details?id=com.st.bluems
  https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8

In order to make the program work, you must do the following: 
 - Connect each STM32WB55xx-Nucleo boards to your PC 
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
If you want to control this application, you can directly send and
 retrieve OpenThread Cli commands connecting an HyperTerminal via an UART RS232 cable using 
 the following connections :
  RXD : CN10(Pin35)   
  TXD : CN10(Pin37)

To get the traces in real time, you can connect an HyperTerminal to the STLink Virtual Com Port. 
    
 For the Cli control and for the traces, the UART must be configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits 
    - Stop Bit = 1 bit
    - Parity = none
    - Flow control = none

**** START DEMO ****
By default, after a reset, the board will start both protocols BLE and OpenThread.
So you BLE activity and OpenThread activity can be done in parallel :

**** BLE ACTIVITY ****
The BLE activity demonstrates a Point-to-Point communication using BLE component. 
The Peripheral device (P2P Server) starts advertising after reset.


a)If using a Nucleo board (running a BLE_p2pClient application [3]),instead of a phone, you can start scanning by 
pressing the SW1 button the BLUE LED will light up on. After scanning complete, this Nucleo board connects itself to the 
P2P server on BLE/Thread dynamic node, and sends a BLE toggle when pressing SW1.
As a result the RED LED toggles on the BLE/Thread device, each time the SW1 button is pressed on BLE_p2pClient board.

b) If using a smartphone, then the ST BLE Sensor App (smart Phone Application) scans and can connect to the P2P Server 
by selecting = "P2PZBSRV1" entry. Once connected, the application starts to search the P2P Services & Characteristics.
Led Button service, Led Characteristic and Button Characteristic are discovered at this stage 
(this service is ST specific).
Once the smartphone is connected, the user can interact with the board by pressing 
"LED1 CONTROL" field which toggles LED1 (Blue) on the board.
You can also use the button 2 on the Nucleo board to send a message to the smartphone application to toggle the bell icon
on the UI.


**** THREAD ACTIVITY ****
After reset, OpenThread is started and nodes tries to build up a Thread network or attach to a existing one
(at least 2 boards running THREAD_FTD_Coap_Multicast application).

In a Thread network, nodes are split into two forwarding roles: Router or End Device.
The Thread Leader is a Router that is responsible for managing the set of Routers in a 
Thread network. An End Device (or child) communicates primarily with a single Router. 

In our Application which uses at least two devices, one device will act as a Leader (Router) 
and the other will act as child or router. 

After the network building, one board will be in Leader mode the other one will be in Child/Router mode.
The leader will then start to send every seconds a multicast COAP Message which will toggle LED3 (Red) of the receivers.
Button 1 is also dedicated to multicast COAP message sending, with same effect on receivers and can be used whatever 
the node role in the network.
If you want to control this application, you can directly send and
retrieve OpenThread Cli commands connecting an HyperTerminal with the ST_Link cable.
(Refer to the THREAD_Cli_Cmd application for more details)
User can trigger a channel scan for instance.

**** BUTTON/LED SUMMARY ****
LED Mapping :
- LED 1 Blue : BLE dedicated, toggle on ST BLE Sensor App request
- LED 2 Green : reflect Radio Activity (On:BLE, Off:15.4)
- LED 3 Red : 15.4 dedicated, Toggle on COAP message reception

Button Mapping :
- Button 1 : Send a Multicast COAP Message to toggle Other's node Red LED 3
- Button 2 : Send a BLE Message to BLE Sensor App to toggle bell icon

**** EXTPA ****

The STM32WB is not able to transmit with a power higher than 5 dbm. As such it is
possible to use the EXT PA as an additional supply source.

Signals:
The EXTPA consists of three signals.

- Analogic signal : power supply, constant value.
- Enable/Disable: 0 when the RF is in Sleep/not initialized, 1 otherwise.
- RX/TX : 1 during a transmission, 0 otherwise.
___________________________________________________
|   RF status:    | Sleep mode | RX mode | TX mode |
|_________________|____________|_________|_________|
| Enable/Disable: |     0      |    1    |    1    |
|_________________|____________|_________|_________|
|    RX/TX:	      |    N/A     |    0    |    1    |
|_________________|____________|_________|_________|
|Analogic signal: |	             N/A               |
|_________________|________________________________|

NOTE: The RX/TX signal must be wired on the PB0 I/O (C10-22 on Nucleo).

The Enable/Disable signal by default is mapped on PB9 (C10-5 on Nucleo).
You can change this setting in app_conf.h, by editing GPIO_EXT_PA_EN_PIN and GPIO_EXT_PA_EN_PORT.

Power setting:
The firmware is not aware of the power brought by the ExtPA. It is up to the system integrator to
configure the application to get the desired power.

For instance, if the ExtPA provides an additional power of 10dbm and the desired power for a TX is 12dbm.
The application must be configured to set the power at 2dbm.

M0 firmware:
The M0 firmware remains unchanged. It has been tested with the binary listed above.

EXTPA enabling:
The EXTPA support is not enabled by default.
In hw_conf.h, set CFG_HW_EXTPA_ENABLED to 1.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 