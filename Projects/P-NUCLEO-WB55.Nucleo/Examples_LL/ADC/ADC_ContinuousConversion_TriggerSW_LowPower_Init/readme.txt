/**
  @page ADC_ContinuousConversion_TriggerSW_LowPower_Init ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_ContinuousConversion_TriggerSW_LowPower_Init example.
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

@par Example Description

How to use an ADC peripheral with ADC low-power features. This example is based on
the STM32WBxx ADC LL API. The peripheral initialization is done using LL unitary service functions 
for optimization purposes (performance and size).

Description of ADC low power features:
ADC conversions occurrences are limited to the minimum necessary
to reduce power consumption.
These features are intended to be used by applications needing ADC
conversions at few occurrences and without latency constraints.
ADC low power features are used most commonly with programming
model: polling.


Example configuration:
ADC is configured to convert a single channel, in continuous conversion mode,
from SW trigger.
ADC low power feature enabled: auto wait.
Note: On some other STM32 series, an additional ADC low feature is available: 
      auto power-off.

Example execution:
From the first press on User push-button (SW1), the ADC converts the selected channel
continuously, but with conversions managed by ADC mode low-power:
After the first trigger (software start in this example), following conversions
are not launched successively automatically: the ADC performs 1 conversion
and remains idle (ADC does not perform any other conversion)
until conversion data has been processed by user software.
Note: this is the goal of low power feature auto-wait: to automatically adapt
      the ADC conversions trigs to the speed of the software that reads the data. 
      Moreover, this avoids risk of overrun for low frequency applications.
At each press of User push-button (SW1), software reads the ADC conversion data
and stores it into a variable.
This action automatically trigs another ADC conversion start.
LED2 is turned on.
Note: the ADC conversion data read is corresponding to previous
      ADC conversion start, independently of delay during which ADC was idle.
      Therefore, the ADC conversion data may be outdated: does not correspond 
      to the current voltage level on the selected ADC channel.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

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

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, trigger


@par Directory contents 

  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Inc/main.h                  Header for main.c module
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Src/main.c                  Main program
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


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
