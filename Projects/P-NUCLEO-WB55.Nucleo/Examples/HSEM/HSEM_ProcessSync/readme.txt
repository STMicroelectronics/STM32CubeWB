/**
  @page HSEM_ProcessSync HSEM process Sync example
  
  @verbatim
  ******************************************************************************
  * @file    HSEM/HSEM_ProcessSync/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HSEM Process Sync example.
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

How to use a HW semaphore to synchronize 2 process.

In this example, HW semaphore (9) is used to synchronize 2 process.

process 1 : Toggle LED2 every 500 ms
process 2 : Set LED1 On.

First, semaphore (9) is taken using HAL API HAL_HSEM_Take with process ID  set to "12". 
if the semaphore is taken LED2 every 500 ms .
The semaphore is released after 5 seconds, using HAL API HAL_HSEM_Release. During this period, the LED2 
is toggling to mention that the semaphore is not available.

Note that HAL_HSEM_Take use the 2-Step semaphore take method (refer to the HW Semaphore
description in the reference manual)

Pushing the "User (SW1)" button will generate an EXTI interrupt. 
The corresponding EXTI callback will try to take the same semaphore (9) using 
HAL API HAL_HSEM_FastTake. 
If the semaphore has been released by first process , then semaphore can be taken 
immediately and it will set LED1 On.

If not, HW semaphore 9 not released yet by the first process , then the HAL_HSEM_FastTake API 
will return HAL_ERROR meaning that the semaphore is already taken by another process.
In this case the HAL API HAL_HSEM_ActivateNotification enable the notification receive once 
the semaphore will be released. 
Treatment will be postponed until the semaphore is released.
Once done, a HSEM interrupt is generated (semaphore 9), and HAL_HSEM_FreeCallback is asserted 
to set LED1 On.

Note that the API HAL_HSEM_FastTake use the 1-Step semaphore take method (refer to the HW Semaphore
description in the reference manual) where the process ID is assumed to be zero.

As a result both process will be synchronized, second process will toggle LED1
only when first process has ended toggle LED2 10 times.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Hardware semaphore, Semaphore, HSEM, EXTI, synchronize, Lock, Unlock, Take, Release, Process

@par Directory contents 

  - HSEM/HSEM_ProcessSync/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - HSEM/HSEM_ProcessSync/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - HSEM/HSEM_ProcessSync/Inc/main.h                  Header for main.c module  
  - HSEM/HSEM_ProcessSync/Src/stm32wbxx_it.c          Interrupt handlers
  - HSEM/HSEM_ProcessSync/Src/stm32wbxx_hal_msp.c     HAL MSP file
  - HSEM/HSEM_ProcessSync/Src/main.c                  Main program
  - HSEM/HSEM_ProcessSync/Src/system_stm32wbxx.c      stm32wbxx system source file

@par Hardware and Software environment

  - This example runs on STM32WB55RGVx device.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
