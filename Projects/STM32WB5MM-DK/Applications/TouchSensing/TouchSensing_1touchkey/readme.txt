/**
  @page TouchSensing_1touchkey 1 touchkey sensor example
 
  @verbatim
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_1touchkey\readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TouchSensing 1 touchkey sensor example.
  ******************************************************************************
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

Use of the STMTouch driver with 1 touchkey sensor.
This application also uses the Environment Change System (ECS) and
Detection Time Out (DTO).

Observed behaviour:

- The LD4 is HIGH GREEN when the TS1 sensor is touched.
- The LD4 is Low GREEN when the TS1 sensor is approached.
- The LD4 is BLUE each time the ECS process is finished.
- The LD4 is RED in case of error.

- The ECS is ON when no touch is detected (sensor is in the RELEASE state).
  The ECS is always present but its behaviour can be modified using some parameters in
  the tsl_conf.h file.

- You can experiment the DTO by touching a sensor for few seconds. The RELEASE state
  is automatically entered and a re-calibration is performed after the timeout is reached.
  This timeout is programmable by the TSLPRM_DTO parameter in the tsl_conf.h file.

@par Keywords

Touch sensing, TouchSensing

@par Project Directory contents

    - TouchSensing_1touchkey\Inc\main.h                Header for main.c file
    - TouchSensing_1touchkey\Inc\stm32wb5mm_dk_conf.h  Discovery Module configuration file
    - TouchSensing_1touchkey\Inc\stm32wbxx_hal_conf.h  HAL Library configuration file
    - TouchSensing_1touchkey\Inc\stm32wbxx_it.h        Header for stm32wbxx_it.c file
    - TouchSensing_1touchkey\Src\stmCriticalSection.h  Header for stmCriticalSection.c file
    - TouchSensing_1touchkey\Inc\tsl_conf.h            STMTouch driver configuration file
    - TouchSensing_1touchkey\Inc\tsl_user.h            Header for tsl_user.c file

    - TouchSensing_1touchkey\Src\main.c                Main program file
    - TouchSensing_1touchkey\Src\stm32wbxx_hal_msp.c   HAL MSP module file
    - TouchSensing_1touchkey\Src\stm32wbxx_it.c        Interrupt handlers file
    - TouchSensing_1touchkey\Src\stmCriticalSection.c  STMStudio lock/unlock mechanism file
    - TouchSensing_1touchkey\Src\system_stm32wbxx.c    System initialization file
    - TouchSensing_1touchkey\Src\touchsensing.c        Touchsensing instances configuration file
    - TouchSensing_1touchkey\Src\tsl_user.c            Touchsensing channels/banks description file
    
@par Hardware and Software environment

  - This example runs on STM32WB5MMxx devices.
    
  - This example has been tested with STM32WB5MM Discovery Kit board. It can be
    easily tailored to any other devices that embed the TSC peripheral and to
    any other development board supporting touchsensing sensor.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
