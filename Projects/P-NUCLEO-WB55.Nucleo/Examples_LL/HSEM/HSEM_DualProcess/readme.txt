/**
  @page HSEM_DualProcess HSEM : Hardware semaphore
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/HSEM/HSEM_DualProcess/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HSEM_DualProcess example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the low-layer HSEM API to initialize, lock, and unlock hardware 
semaphore in the context of two processes accessing the same resource.

Example execution:
After startup from reset and system configuration, HSEM configuration is performed.

Two processes are used to demonstrate the features:
  PROCESS_A with an identifier of 0xA (This can be changed at will).
  PROCESS_B with an identifier of 0xB (This can be changed at will).

The hardware semaphore used is 0x2 (This can be changed at will).

PROCESS_A takes the semaphore with success.
PROCESS_B takes the semaphore and fail (as already taken by PROCESS_A).
PROCESS_B releases the semaphore and fail (as owned by PROCESS_A).
PROCESS_A releases the semaphore with success.

After successful sequence, LED2 is turned On. 
In case of errors, LED2 is slowly blinking (1sec period).

Additionnaly, this example demonstrate how to:
  Retreive the current process locking a semaphore.
  Retreive the current core locking a semaphore.

Remarks:
  As this example uses two processes to demonstrate the lock/unlock mechanism, it is not
  possible to use the function LL_HSEM_1StepLock which does not use process identifier. Those
  identifier are set to 0. This function is only interesting in the context of multicore where
  the core identifers are used to determine the owner of the semaphore.

@par Keywords

Hardware semaphore, Semaphore, HSEM, Lock, Unlock, Take, Release, Process

@par Directory contents 

  - HSEM/HSEM_DualProcess/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - HSEM/HSEM_DualProcess/Inc/main.h                  Header for main.c module
  - HSEM/HSEM_DualProcess/Inc/stm32_assert.h          Template file to include assert_failed function
  - HSEM/HSEM_DualProcess/Src/stm32wbxx_it.c          Interrupt handlers
  - HSEM/HSEM_DualProcess/Src/main.c                  Main program
  - HSEM/HSEM_DualProcess/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example and observe the LED status

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
