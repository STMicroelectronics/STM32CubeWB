/**
  @page HAL_TimeBase_RTC_ALARM HAL TimeBase RTC Alarm
  
  @verbatim
  ******************************************************************************
  * @file    HAL/HAL_TimeBase_RTC_ALARM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HAL TimeBase RTC Alarm example.
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

How to customize HAL using RTC alarm as main source of time base, 
instead of Systick.

The User push-button (SW1) is used to suspend or Resume tick increment. 

Each time the button is pressed; an interrupt is generated (External line 4)
and in the ISR the uwIncrementState is checked:
  1- If the uwIncrementState = 0: the tick increment is suspended by calling 
     HAL_SuspendTick() API (RTC alarm interrupt is disabled).
  2- If the uwIncrementState = 1: the tick increment is Resumed by calling 
     HAL_ResumeTick() API(RTC alarm interrupt is enabled).

The alarm is configured to assert an interrupt when the RTC reaches 1 ms 

The example brings, in user file, a new implementation of the following HAL weak functions:

HAL_InitTick() 
HAL_SuspendTick()
HAL_ResumeTick()

This implementation will overwrite native implementation in stm32wbxx_hal.c
and so user functions will be invoked instead when called.

The following time base functions are kept as implemented natively:

HAL_IncTick()
HAL_Delay()

In an infinite loop, LED2 toggles spaced out over 500ms delay, except when tick increment is suspended.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, RTC Alarm, Time base, HAL

@par Directory contents  

  - HAL/HAL_TimeBase_RTC_ALARM/Inc/stm32wbxx_hal_conf.h               HAL configuration file
  - HAL/HAL_TimeBase_RTC_ALARM/Inc/stm32wbxx_it.h                     Interrupt handlers header file
  - HAL/HAL_TimeBase_RTC_ALARM/Inc/main.h                                   Header for main.c module  
  - HAL/HAL_TimeBase_RTC_ALARM/Src/stm32wbxx_it.c                     Interrupt handlers
  - HAL/HAL_TimeBase_RTC_ALARM/Src/main.c                                   Main program
  - HAL/HAL_TimeBase_RTC_ALARM/Src/stm32wbxx_hal_msp.c                HAL MSP file
  - HAL/HAL_TimeBase_RTC_ALARM/Src/stm32wbxx_hal_timebase_rtc_alarm.c HAL time base rtc alarm functions
  - HAL/HAL_TimeBase_RTC_ALARM/Src/system_stm32wbxx.c                 STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with STMicroelectronics P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
