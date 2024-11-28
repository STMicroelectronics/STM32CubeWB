/**
  @page BSP_Example Description of the BSP example
  
  @verbatim
  ******************************************************************************
  * @file    BSP/BSP_Example/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the different BSP drivers of external devices mounted on : B-WB1M-WPAN1 board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.

This example shows how to use the different functionalities of components
available on the board by switching between all tests using User Button1 push-button.

 ** Push the User B1 push-button to start first Test.

### LED ###
This example shows how to use turn on/off and toggle the LEDs.

### EEPROM ###
This example shows how to Read/Write to the EEPROM (make sure to close the SB14,SB16 and open SB13,SB15 on the board).

### TEMPERATURE ###
This example shows how to use the STTS22H sensor to get temperature values.

### GYROSCOPE ###
This example shows how to use the ISM330DHCX sensor to get gyroscope values.

### ACCELERO ###
This example shows how to use the ISM330DHCX sensor to get accelerometer values.

Depending on IDE, to watch content of Terminal I/O note that
 - When resorting to EWARM IAR IDE:
   * Command Code is displayed on debugger as follows: View --> Terminal I/O
 - When resorting to MDK-ARM KEIL IDE:
   * Command Code is displayed on debugger through HyperTerminal or TeraTerm
 - When resorting to STM32CubeIDE:
   * Command Code is displayed on debugger as follows: Window--> Show View--> Console.
     In Debug configuration:
       - Window\Debugger, select the Debug probe: ST-LINK(OpenOCD)
       - Window\Startup, Depending on IDE, to watch content of Terminal I/O note that

Results can be displayed on PC (through HyperTerminal or TeraTerm) by Virtual Com Port
in enabling USE_VCP_CONNECTION compilation switch and with the proper configuration described
 in paragraph "Hardware and Software environment (make sure to close the SB3,SB6 and open SB4,SB8 on the board).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, Led, EEProm, Memory, Button, Temperature, Sensor,
Accelerometer, Gyroscope, STTS22H, ISM330DHCX.

@par Directory contents 

  - BSP/Src/main.c                        Main program
  - BSP/Src/system_stm32wbxx.c            STM32WBxx system clock configuration file
  - BSP/Src/stm32wbxx_it.c                Interrupt handlers
  - BSP/Src/led.c                         Tests of Turn on/off or toggle the LED
  - BSP/Src/sensors.c                     Tests of stts22h and ism330dhcx sensors
  - BSP/Inc/main.h                        Main program header file  
  - BSP/Inc/stm32wbxx_hal_conf.h          HAL Configuration file
  - BSP/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BSP/Inc/b_wb1m_wpan1_conf.h           B-WB1M-WPAN1 board configuration file

@par Hardware and Software environment  

  - This example runs on STM32WB1Mxx devices.

  - This example has been tested with STMicroelectronics B-WB1M-WPAN1 boards.

  - B-WB1M-WPAN1 Set-up
    Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
    A virtual COM port will then appear in the HyperTerminal.

  - Hyperterminal configuration:
    - Data Length = 8 Bits
    - Stop Bit: One Stop Bit
    - Parity: None
    - BaudRate: 115200 bauds
    - Flow Control: None

@par How to use it ? 

In order to make the program work, you must do the following :    
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
