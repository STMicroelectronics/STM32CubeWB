/**
  @page ADC_AnalogWatchdog ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_AnalogWatchdog example.
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

How to use the ADC peripheral to perform conversions with an analog watchdog 
and out-of-window interrupts enabled.

Example configuration:
The ADC launch conversions in continuous mode on the selected channel. DMA is configured
to transfer conversion data in an array, in circular mode.
Analog watchdog 1 is enabled, minimum and maximum thresholds are respectively set to 1/8
and 5/8 of full range scale (between about 0.41V and 2.06V with full range of 3.3V).

Example execution:
From the start, the ADC converts the selected channel continuously. DMA transfers conversion data to the array. Results array is updated indefinitely (DMA in circular mode).
If the conversion results is out of the analog window thresholds, ADC analog window IRQ handler (HAL_ADC_LevelOutOfWindowCallback) is called "ubAnalogWatchdogStatus" is SET and
LED2 is turned on.

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data (array of data)
 - "ubAnalogWatchdogStatus": ADC analog window status (SET = voltage out of window / RESET otherwise)

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

Board settings:
 - ADC is configured to convert ADC_CHANNEL_6 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).
 - The voltage input on ADC channel must be provided by an external source connected to Arduino connector CN8 pin A2, Morpho connector CN7 pin 32.


To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA1 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32).

P-NUCLEO-WB55 board LED is be used to monitor the program execution status:
 - Normal operation: LED2 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if voltage into of AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, trigger, analog watchdog, threshold, window

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                        Header for main.c module  
  - ADC/ADC_AnalogWatchdog/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - ADC/ADC_AnalogWatchdog/Src/main.c                        Main program
  - ADC/ADC_AnalogWatchdog/Src/system_stm32wbxx.c      STM32WBxx system source file


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
