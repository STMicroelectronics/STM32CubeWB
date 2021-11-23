/**
  @page ADC_SingleConversion_TriggerTimer_DMA ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_SingleConversion_TriggerTimer_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerTimer_DMA example.
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
at each trigger event from a timer. Converted data is transferred by DMA 
into a table in RAM memory.
This example is based on the STM32WBxx ADC HAL API.

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from HW trigger: timer peripheral (timer instance: TIM2).
timer is configured to provide a time base of 1kHz.
DMA is configured to transfer conversion data in an array, in circular mode.
A timer is configured in time base and to generate TRGO events.

Example execution:
From the main program execution, the ADC group regular converts the
selected channel at each trig from timer. DMA transfers conversion data to the array,
indefinitely (DMA in circular mode).

When conversion are completed (DMA half or full buffer complete),
interruption occurs and data is processed in callback functions (for this example purpose,
convert digital data to physical value in mV).

LED2 is used to monitor program execution status:
- Normal operation: ADC group regular buffer activity (buffer complete events):
  LED toggle at frequency of time base / half buffer size (approx. 31Hz with this example default settings)
- Error: LED remaining turned on

Debug: variables to monitor with debugger:
- "uhADCxConvertedData": ADC group regular buffer conversion data (resolution 12 bits)
- "uhADCxConvertedData_Voltage_mVolt": ADC group regular buffer conversion data computed to voltage value (unit: mV)

Connection needed:
Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).
Note: If no power supply available, this pin can be let floating (in this case
      ADC conversion data will be undetermined).

Other peripherals used:
  1 GPIO for LED2
  1 GPIO for analog input: ADC channel 6 on pin PA.01 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  1 DMA channel
  1 timer instance

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, regular, injected, DMA transfer, trigger

@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/nucleo_wb15cc_conf.h     BSP configuration file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.
    
  - This example has been tested with NUCLEO-WB15CC board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
