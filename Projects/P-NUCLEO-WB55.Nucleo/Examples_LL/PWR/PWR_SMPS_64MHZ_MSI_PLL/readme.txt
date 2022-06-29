/**
  @page PWR_SMPS_64MHZ_MSI_PLL PWR SMPS example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_SMPS_64MHZ_MSI_PLL/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR SMPS example.
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

This example shows how to use power converters of STM32WB (SMPS, LDO and LP-LDO)
depending on Vdd voltage and low-power mode. In this example, 
the system clock source is the MSI PLL at 64MHz.

User can act on 2 parameters to use power converters of STM32WB:

 - Usage of SMPS or LDO:
   Caution: BOR level must be set to level 1, 2, 3 or 4 (not level 0, default configuration)
            The recommended BOR level is level 1: BOR1 voltage rising edge 2.1V, falling edge 2.0V (refer to device datasheet for more information).
            To change BOR level, use tool ST-Link utility to update option bytes.
   User must apply a variation of Vdd: Remove jumper JP2 and supply JP2 pin "VDD" by an external voltage.
   This is the use case, for example , of a battery voltage variation depending on lifetime or current load.
   - If Vdd > BOR level: Vcore is supplied by SMPS + LDO
     SMPS is in mode step-down and is converting power for the main LDO.
     The transition of SMPS mode bypass to step-down is not done automatically by HW. This example performs it by SW.
     LED2 toggles at fast speed (2.5Hz).
   - If Vdd < BOR level: Vcore is supplied by LDO
     SMPS is in mode bypass, Vdd is connected directly to the main LDO.
     The transition of SMPS mode step-down to bypass is done automaticcaly by HW.
     LED2 toggles at slow speed (1Hz).

 - Usage of LP-LDO:
   User must make the device enter in low-power mode standby: enter in standby with push button (through pin PC.04), exit from stanby with LL_PWR_WAKEUP_PIN2 on pin PC13 or with button RESET.
   - If device enters in low-power mode:
     Vcore is supplied by LP-LDO (Vdd is connected directly to the LP-LDO)
     If SMPS was in mode step-down, then it is switching to mode open to preserve energy stored in decoupling capacitor.
     Note: Other low-power modes that can use LP-LDO: stop1, stop2, standby, shutdown
     LED2 is turned off.
   - If device exit from low-power mode:
     Vcore is supplied by SMPS+LDO or LDO (refer to case above).
     LED2 is turned on during 1 second (to indicate wake-up from low-power), then toggles at speed depending on SMPS mode.

This example has configuration option, selected with literals:
 - RF stack configuration: RF_STACK_AVAILABLE
Refer to comments inlined in code for their usage.

@note When Vdd voltage is low, LED2 luminosity is low and can appear as turned-off.
      In this case, to monitor LED2 toggling frequency, an oscilloscope should be used.

@note Wake-up pin connection: On board STM32WB Nucleo64, pin PC13 is not connected to user button by default, solder bridge SB48 must be closed.
      Alternate solution: connect a wire between PC4 (pin connected to user button) and PC13.

@note To measure MCU current consumption on board STM32WB Nucleo,
      board configuration must be applied:
      - remove all jumpers on connector JP5 to avoid leakages between ST-Link circuitry and STM32WB device.
      - remove jumper JP2 and connect an amperemeter to measure current between the 2 connectors of JP2.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Keywords

PWR, SMPS, step-down

@par Directory contents 

  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Inc/main.h                  Header for main.c module
  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Src/stm32wbxx_it.c          Interrupt handlers
  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Src/main.c                  Main program
  - PWR/PWR_SMPS_64MHZ_MSI_PLL/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
