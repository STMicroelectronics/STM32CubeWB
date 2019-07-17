/**
  @page CKS_Crypt example
  
  @verbatim
  ******************************************************************************
  * @file    XXX/CKS_Crypt/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of CKS feature usage.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use CKS feature to store AES crypto keys in secure area.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

     
@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics STM32WB55VG-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

The STM32WB Series are dual-core devices with one core (CPU1) for user application and another core (CPU2)
dedicated to the wireless real-time aspect execution (either Bluetooth low energy or thread protocols). The Flash
memory used by CPU2 is secured from CPU1 or external access. Communication between the two cores is
ensured by a mailbox and an inter-process channel control hardware block (IPCC).
In addition to the wireless stack execution, CPU2 offers a secure storage service for cryptographic keys used with
a dedicated AES1 hardware block. The key register of this block is accessible only to
CPU2, preventing access to the key by an untrusted process running on CPU1 or by the debug port.
After keys have been provisioned inside the secure area, user application can use them by calling a secure load
service with an index referencing the key and no more the key itself.

Key provisioning:
It can be done either in the application itself by calling RSS services through a dedicated API.
Or it can be done by using STM32CubeProgrammer tool with a bootloader connection.


In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files with correct WRITE_KEYS define value
      - If WRITE_KEYS is defined, keys will be provisionned by the code itself.
      - If WRITE_KEYS is not defined, keys shall be provisionned by STM32CubeProgrammer.
 - Flash the board with the executable file
 - Power on the Nucleo board with the CRYP_CKS application
 
If all test are passed, LED1 is swithed On.
It one test fails, LED3 is switched On

For more details refer to the Application Note: 
  AN5185 - ST firmware upgrade services for STM32WB Series
  UM2237 - STM32CubeProgrammer software description
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 