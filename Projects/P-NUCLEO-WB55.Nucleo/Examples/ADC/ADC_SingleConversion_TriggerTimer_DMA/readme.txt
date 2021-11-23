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
Use ADC to convert a single channel at each trig from timer, 
conversion data are transferred by DMA into an array, indefinitely (circular mode).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from HW trigger: timer peripheral.
DMA is configured to transfer conversion data in an array, in circular mode.
A timer is configured in time base and to generate TRGO events.

Example execution:
From the start, the ADC converts the selected channel at each trig from timer.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).
LED2 is turned on when the DMA transfer is completed (results array full)
and turned off at next DMA half-transfer (result array first half updated).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data (array of data)
 - "aADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values (array of data)

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PA1 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA
  Timer

Board settings:
 - ADC is configured to convert ADC_CHANNEL_6 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).
 - The voltage input on ADC channel must be provided by an external source connected to Arduino connector CN8 pin A2, Morpho connector CN7 pin 32.


To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA1 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).

P-NUCLEO-WB55 board LED is be used to monitor the program execution status:
 - Normal operation: LED2 is turned-on/off in function of ADC conversion
   result.
    - Toggling: "On" upon conversion completion (full DMA buffer filled)
                "Off" upon half conversion completion (half DMA buffer filled)
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, DMA transfer, trigger

@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Inc/main.h                        Header for main.c module  
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/main.c                        Main program
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
