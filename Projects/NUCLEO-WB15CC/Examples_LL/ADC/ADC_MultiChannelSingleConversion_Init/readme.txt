/**
  @page ADC_MultiChannelSingleConversion_Init ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_MultiChannelSingleConversion_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultiChannelSingleConversion_Init example.
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

How to use an ADC peripheral to convert several channels. ADC conversions are 
performed successively in a scan sequence.
This example is based on the STM32WBxx ADC LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:
ADC is configured in single conversion mode, from SW trigger.
Sequencer of ADC group regular (default group available on ADC of all STM32 devices)
is configured to convert 3 channels: 1 channel from GPIO, 2 internal channels:
internal voltage reference VrefInt and temperature sensor.
DMA is configured to transfer conversion data in an array of size three elements
(one array address for conversion data of each channel) in RAM memory,
in circular mode.

Example execution:
Every second, ADC performs conversion of a channel among the 3 channels
of the scan sequence, successively at each conversion start (discontinuous mode enabled).

Note: In case of discontinuous mode is disabled, then the entire scan sequence
      is converted in burst from one conversion start.

ADC conversion data of each sequence rank is transferred by DMA into the array
"uhADCxConvertedData":
- uhADCxConvertedData[0]: ADC channel set on sequence rank 1 (GPIO as analog input)
- uhADCxConvertedData[1]: ADC channel set on sequence rank 2 (VrefInt)
- uhADCxConvertedData[2]: ADC channel set on sequence rank 3 (Temperature sensor)

When sequence is completed, it restarts from the beginning: first channel 
in the sequence, data transfer in first array address (rollback).

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
- Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)
- Value of voltage on GPIO pin (on which is mapped ADC channel, cf pin below) (unit: mV)
- Value of internal voltage reference VrefInt (unit: mV)
- Value of temperature (unit: degree Celsius)

Note: For this example purpose, analog reference voltage (Vref+) is computed
      from ADC conversion of internal voltage reference VrefInt and used
      to compute other conversion data.
      This voltage should correspond to value of literal "VDDA_APPLI".
      This procedure can be performed when value of voltage Vref+
      is unknown in the application.
      (This is not the case in this example due to target board
      supplied by a LDO regulator providing a known constant voltage
      of value "VDDA_APPLI").
      In typical case of Vref+ connected to Vdd, it allows to
      deduce Vdd value.

LED2 is used to monitor program execution status:
- Normal operation: Activity of ADC scan sequence
  can be observed with LED toggle:
  - At each ADC conversion: LED toggle once (every 1sec)
  - At each scan conversion completed: LED toggle 4 times quickly (10Hz)
- Error: LED remaining turned on

Debug: variables to monitor with debugger:
- "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
- "uhADCxConvertedData_VrefAnalog_mVolt":         Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)
- "uhADCxConvertedData": ADC group regular conversion data
- "uhADCxConvertedData_VoltageGPIO_mVolt":        Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
- "uhADCxConvertedData_VrefInt_mVolt":            Value of internal voltage reference VrefInt (unit: mV)
- "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celsius)

Connection needed:
Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).
Note: If no power supply available, this pin can be let floating (in this case
      ADC conversion data will be undetermined).

Other peripherals used:
  1 GPIO for LED2
  1 GPIO for analog input: ADC channel 6 on pin PA.01 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  1 DMA channel

@par Keywords

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, DMA transfer, sequence, temperature sensor, internal voltage reference, VrefInt, discontinuous

@par Directory contents 

  - ADC/ADC_MultiChannelSingleConversion_Init/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - ADC/ADC_MultiChannelSingleConversion_Init/Inc/main.h                  Header for main.c module
  - ADC/ADC_MultiChannelSingleConversion_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_MultiChannelSingleConversion_Init/Src/stm32wbxx_it.c          Interrupt handlers
  - ADC/ADC_MultiChannelSingleConversion_Init/Src/main.c                  Main program
  - ADC/ADC_MultiChannelSingleConversion_Init/Src/system_stm32wbxx.c      STM32WBxx system source file


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
