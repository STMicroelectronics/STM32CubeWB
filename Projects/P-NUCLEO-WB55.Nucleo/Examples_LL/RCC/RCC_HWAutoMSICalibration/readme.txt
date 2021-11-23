/**
  @page RCC_HWAutoMSICalibration RCC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_HWAutoMSICalibration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC example.
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

Use of the MSI clock source hardware autocalibration and LSE clock (PLL mode) to obtain a precise MSI clock. 

In the associated software, after start-up, SYSCLK is configured to the max 
frequency using the PLL with MSI as clock source.

When a 32.768 kHz external oscillator is present in the application,it is possible to configure 
the MSI in a PLL-mode by setting the MSIPLLEN bit. 
When configured in PLL-mode, the MSI automatically calibrates itself thanks to the LSE. 
This mode is available for all MSI frequency ranges. At 48 MHz, the MSI in PLL-mode can 
be used for the USB OTG FS device, saving the need of an external high-speed crystal.

The program waits until User push-button (SW1) is pressed to begin the auto calibration 
procedure (fast blink of the LED2):
 - Enable and wait for LSE is ready.
 - Enable PLL-Mode feature
 - LED2 is turn on to indicate end of the HW calibration procedure.

@note User can connect on oscilloscope to MCO pin (PA.08) to check MSI frequency before and after
calibration procedure.

@note User can enable USE_TIMEOUT switch to detect a problem with LSE activation (LED2 is blinking 
every 1 sec in case of issue)


@par Keywords

RCC, System, MSI clock , PLL-mode, LSE clock, PLL

@par Directory contents 

  - RCC/RCC_HWAutoMSICalibration/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - RCC/RCC_HWAutoMSICalibration/Inc/main.h                  Header for main.c module
  - RCC/RCC_HWAutoMSICalibration/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_HWAutoMSICalibration/Src/stm32wbxx_it.c          Interrupt handlers
  - RCC/RCC_HWAutoMSICalibration/Src/main.c                  Main program
  - RCC/RCC_HWAutoMSICalibration/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

  - P-NUCLEO-WB55 Set-up
    - External LSE Oscillator is mandatory to be able to run this example.
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.08: Arduino connector CN9 pin D6, Morpho connector CN10 pin 25 (MB1355)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
