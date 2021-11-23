/**
  @page ADC_SingleConversion_TriggerSW_IT ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_SingleConversion_TriggerSW_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerSW_IT example.
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

How to use ADC to convert a single channel at each SW start, 
conversion performed using programming model: interrupt.
This example is based on the STM32WBxx ADC HAL API.

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
ADC interruption enabled: EOC (end of conversion of ADC group regular).

Example execution:
From the main program execution, a ADC software start is performed
every second, ADC group regular converts the selected channel.
When conversion is completed ADC interruption occurs.
IRQ handler callback function reads conversion data from ADC data register
and stores it into a variable.

LED2 is used to monitor program execution status:
- Normal operation: ADC group regular activity can be observed with LED toggle:
  - At each ADC conversion: LED toggle once (every 1sec)
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

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, interruption

@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/nucleo_wb15cc_conf.h     BSP configuration file
  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/system_stm32wbxx.c      STM32WBxx system source file


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
