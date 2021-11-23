/**
  @page ADC_Oversampling ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_Oversampling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_Oversampling example.
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
Use ADC to convert a single channel but using oversampling feature to increase resolution. 

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
Oversampling is configured to perform 128 conversions
with a right shift of 3 before returning a result.

Example execution:
The ADC performs conversions of the selected channel in continuous mode from SW start
trigger.
Then, SW is waiting for conversion to complete. When done, it reads conversion data from
ADC data register, stores it into a variable, and convert it into mVolt in another one.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

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

Board settings:
 - ADC is configured to convert ADC_CHANNEL_6 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).
 - The voltage input on ADC channel must be provided by an external source connected to Arduino connector CN8 pin A2, Morpho connector CN7 pin 32.


To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA1 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).

P-NUCLEO-WB55 board LED is be used to monitor the program execution status:
 - Normal operation: LED2 is toggling at each conversion, at a frequency of 10Hz.
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, oversampling

@par Directory contents 

  - ADC/ADC_Oversampling/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - ADC/ADC_Oversampling/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_Oversampling/Inc/main.h                        Header for main.c module  
  - ADC/ADC_Oversampling/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_Oversampling/Src/main.c                        Main program
  - ADC/ADC_Oversampling/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - ADC/ADC_Oversampling/Src/system_stm32wbxx.c      STM32WBxx system source file


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
