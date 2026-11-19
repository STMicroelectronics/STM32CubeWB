/**
  @page PWR_ModesSelection PWR Low Power Modes Selection Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Low Power Modes Selection example.
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

@par Example Description

This example shows how to configure the STM32WB15 to enter and measure the
current consumption of the various low power modes supported by the device,
selected interactively through a menu displayed on a serial terminal (UART).

At startup, the system clock is set to 64 MHz (PLL from MSI) and the USART1
console is initialized (115200 bauds). A menu listing the following
low power scenarios is then printed on the terminal.

Enter the index corresponding to the desired scenario. The selected
configuration is applied (clock, RTC/LSE, SRAM retention, then the actual
low power mode entry) and its summary is printed before the USART is shut
down, so that the current consumption of the MCU alone can be measured on
the board's current measurement point (JP2 / IDD jumper).

@par Notes 

   1.To measure MCU current consumption on board STM32WB Nucleo, board configuration must be applied:
      - remove all jumpers on connector JP5 to avoid leakages between ST-Link circuitry and STM32WB device.
      - remove jumper JP2 and connect an amperemeter to measure current between the 2 connectors of JP2.

  2. This example can not be used in DEBUG mode due to the fact
      that the Cortex-M4 core is no longer clocked during low power mode
      so debugging features are disabled.

  3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  4. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par power modes supported by the example 

     00 --> RUN        - 64 MHz                              
     01 --> RUN        - 16 MHz                              
     02 --> SLEEP      - 2 MHz                               
     03 --> STOP 0     - 2 MHz                                   
     04 --> STOP 1     - 2 MHz                                   
     05 --> STANDBY    - SRAM retention + RTC
     06 --> STANDBY    - no SRAM retention + RTC
     07 --> STANDBY    - no SRAM retention + no RTC
     08 --> SHUTDOWN   - w/ RTC
     09 --> SHUTDOWN   - wo/ RTC

@par Keywords

Power, PWR, Low Power, Run mode, Sleep mode, Stop mode, Standby mode, Shutdown mode, RTC, UART console

@par Directory contents

  - PWR/PWR_ModesSelection/Inc/nucleo_wb15cc_conf.h    BSP configuration file
  - PWR/PWR_ModesSelection/Inc/stm32wbxx_hal_conf.h    HAL Configuration file
  - PWR/PWR_ModesSelection/Inc/stm32wbxx_it.h          Header for stm32wbxx_it.c
  - PWR/PWR_ModesSelection/Inc/main.h                  Header file for main.c
  - PWR/PWR_ModesSelection/Inc/console.h               Header for console.c
  - PWR/PWR_ModesSelection/Inc/system_config.h         Header for system_config.c
  - PWR/PWR_ModesSelection/Inc/lowpower_scenarios.h    Header for lowpower_scenarios.c
  - PWR/PWR_ModesSelection/Src/system_stm32wbxx.c      STM32WBxx system clock configuration file
  - PWR/PWR_ModesSelection/Src/stm32wbxx_it.c          Interrupt handlers
  - PWR/PWR_ModesSelection/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - PWR/PWR_ModesSelection/Src/main.c                  Main program
  - PWR/PWR_ModesSelection/Src/console.c               UART console (menu I/O, printf retargeting)
  - PWR/PWR_ModesSelection/Src/system_config.c         Low power modes configuration (clock, RTC, SRAM retention, PWR mode entry)
  - PWR/PWR_ModesSelection/Src/lowpower_scenarios.c    Menu handling and scenario orchestration

@par Hardware and Software environment

  - This example runs on STM32WB15xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-WB15CC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-WB15CC set-up:
    - OPEN ST-LINK Virtual COM port (USART1, PA9/PA10).
    - To measure the current consumption, remove the JP2 jumper and connect
      an amperemeter to JP2 to measure IDD current.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Open a serial terminal on the ST-LINK VCP and select a low power scenario
   from the menu

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

