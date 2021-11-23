/**
  @page ADC_SingleConversion_TriggerTimer_DMA_Init ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerTimer_DMA_Init example.
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

How to use an ADC peripheral to perform a single ADC conversion on a channel 
at each trigger event from a timer. Converted data is indefinitely transferred 
by DMA into a table (circular mode).
This example is based on the STM32WBxx ADC LL API.
The peripheral initialization is done using LL unitary service functions 
for optimization purposes (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from HW trigger: timer peripheral.
DMA is configured to transfer conversion data in an array, in circular mode.
A timer is configured in time base and to generate TRGO events.

Example execution:
From the first press on User push-button (SW1), the ADC performs 1 conversion of the selected
channel at each trig from timer.
DMA transfers conversion data to the array.
When DMA reaches the selected number of transfers,
DMA half transfer and transfer complete interruptions occurs.
Data array is updated indefinitely (DMA in circular mode).
LED2 is turned off at DMA half transfer and turned on at DMA transfer complete.

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "aADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values (array of data)

LED2 is blinking every 1 sec in case of error. 
 
Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for User push-button (SW1)
  1 GPIO for LED2
  1 GPIO for analog input: PA.01 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA
  Timer

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, DMA transfer, trigger


@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55RGVx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
