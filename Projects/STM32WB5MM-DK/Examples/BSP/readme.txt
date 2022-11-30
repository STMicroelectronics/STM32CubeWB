/**
  @page BSP  Description of the BSP example
  
  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

How to use the different BSP drivers of external devices mounted on : STM32WB5MM-DK  board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.

This example shows how to use the different functionalities of components
available on the board by switching between all tests using User Button1 push-button.

 ** Push the User B1 push-button to start first Test.

### LCD ###
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

### TEMPERATURE ###
This example shows how to use the sensors to get temperature values. Both, 
STTS22H and LPS22HB sensors are used. 

### GYROSCOPE ###
This example shows how to use the ISM330DHCX sensor to get gyroscope values.

### ACCELERO ###
This example shows how to use the ISM330DHCX sensor to get accelerometer values.

### PROXIMITY ###
This example shows how to use the VL53L0X sensor to get distance values.

### QSPI ###
This example provides of how to write, read and buffers compare for external
flash memory using QSPI communication.

### AUDIO RECORD ###
This document provides a description of the microphone streaming application based 
on STM32WB5MM-DK board. It allows acquisition of  a digital MEMS microphones,
PDM to PCM decimation and streaming towards a host PC via a dedicated USB Audio Input driver.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, DMA, Audio record, Microphone, LCD, QSPI, QUADSPI, Humidity, Display, Button, Temperature, Sensor,
Accelerometer, Distance, STTS22H, ISM330DHCX, VL53L0X, PDM, PCM, USB.

@par Directory contents 

  - BSP/Src/main.c                        Main program
  - BSP/Src/system_stm32wbxx.c            STM32WBxx system clock configuration file
  - BSP/Src/stm32wbxx_it.c                Interrupt handlers 
  - BSP/Src/sensors.c                     Tests of stts22h, iis2mdc, ism330dhcx and lps22hh sensors
  - BSP/Src/lcd.c                         LCD drawing features
  - BSP/Src/record.c                      Record and Play audio
  - BSP/Src/qspi.c                        QUADSPI Read/Write features
  - BSP/Src/proximity.c                   Tests of vl53l0x sensor
  - BSP/Inc/main.h                        Main program header file  
  - BSP/Inc/stm32wbxx_hal_conf.h          HAL Configuration file
  - BSP/Inc/stm32wbxx_it.h                Interrupt handlers header file
  - BSP/Inc/stm32wb5mm_dk_conf.h          STM32WB5MM-DK board configuration file

@par Hardware and Software environment  

  - This example runs on STM32WB5MMxx devices.

  - This example has been tested with STMicroelectronics STM32WB5MM-DK
    boards.


@par How to use it ? 

In order to make the program work, you must do the following :    
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - In order to stream audio via USB, connect STM32WB5MM-DK USB connector to a host PC

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
