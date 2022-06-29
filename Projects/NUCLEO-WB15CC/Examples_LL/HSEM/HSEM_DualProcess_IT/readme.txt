/**
  @page HSEM_DualProcess_IT HSEM : Hardware semaphore
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/HSEM/HSEM_DualProcess_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HSEM_DualProcess_IT example.
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

How to use the low-layer HSEM API to initialize, lock, and unlock hardware 
semaphore in the context of two processes accessing the same resource. This 
example configures HSEM in interrupt mode to trigger an interrupt when a 
process takes the semaphore.

Example execution:
After startup from reset and system configuration, HSEM configuration is performed.

Two processes are used to demonstrate the features:
  PROCESS_A with an identifier of 0xA (This can be changed at will).
  PROCESS_B with an identifier of 0xB (This can be changed at will).

The hardware semaphore used is 0x2 (This can be changed at will).

PROCESS_A takes the semaphore with interrupt enable.
PROCESS_A releases the semaphore with success.
This trigs the HSEM interrupt where PROCESS_B takes the semaphore.
PROCESS_B releases the semaphore with success.

After successful sequence, LED2 is turned On. 
In case of errors, LED2 is slowly blinking (1sec period).

Additionally, this example demonstrate how to:
  Retrieve the current process locking a semaphore.
  Retrieve the current core locking a semaphore.

Remarks:
  As this example uses two processes to demonstrate the lock/unlock mechanism, it is not
  possible to use the function LL_HSEM_1StepLock which does not use process identifier. Those
  identifier are set to 0. This function is only interesting in the context of multicore where
  the core identifiers are used to determine the owner of the semaphore.

@par Keywords

Hardware semaphore, Semaphore, HSEM, Lock, Unlock, Take, Release, Process

@par Directory contents 

  - HSEM/HSEM_DualProcess_IT/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - HSEM/HSEM_DualProcess_IT/Inc/main.h                  Header for main.c module
  - HSEM/HSEM_DualProcess_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - HSEM/HSEM_DualProcess_IT/Src/stm32wbxx_it.c          Interrupt handlers
  - HSEM/HSEM_DualProcess_IT/Src/main.c                  Main program
  - HSEM/HSEM_DualProcess_IT/Src/system_stm32wbxx.c      STM32WBxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB15CCUx devices.
    
  - This example has been tested with NUCLEO-WB15CC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example and observe the LED status

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
