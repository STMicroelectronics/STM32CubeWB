/**
  @page CRYP_AESModes  Encrypt and Decrypt data using AES Algo in ECB/CBC/CTR
  chaining modes with all possible key sizes.
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm in all modes and all key sizes
             Example
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

How to use the CRYP peripheral to encrypt and decrypt data using AES in chaining
modes (ECB, CBC, CTR).

In this example, the following key sizes are used: 128, 192 or 256.

For this example, a firmware is used to interface with the Crypto peripheral
in polling mode.

This example behaves as follows:

1. ECB mode:
   - AES128 ECB encryption
   - AES192 ECB encryption
   - AES256 ECB encryption
   - AES128 ECB decryption
   - AES192 ECB decryption
   - AES256 ECB decryption
2. CBC mode:
   - AES128 CBC encryption
   - AES192 CBC encryption
   - AES256 CBC encryption
   - AES128 CBC decryption
   - AES192 CBC decryption
   - AES256 CBC decryption
3. CTR mode:
   - AES128 CTR encryption
   - AES192 CTR encryption
   - AES256 CTR encryption
   - AES128 CTR decryption
   - AES192 CTR decryption
   - AES256 CTR decryption

When resorting to IAR Embedded Workbench IDE, AES algorithms processing status
are displayed on debugger Terminal I/O as follows: View --> Terminal I/O.
When resorting to MDK-ARM KEIL IDE, AES algorithms processing status 
are displayed on debugger as follows:  View --> Serial Viewer --> Debug (printf) Viewer.
When resorting to AC6 SW4STM32 IDE:
Command Code is displayed on debugger as follows: Window--> Show View--> Console.
In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
 
When all ciphering and deciphering operations are successful, LED1 is turned on.
In case of ciphering or deciphering issue, LED3 toggles. 

@par Keywords

Security, Cryptography, CRYPT, AES, ECB, CBC, CTR, MAC, USART

@par Directory contents 

  - CRYP/CRYP_AESModes/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_AESModes/Src/stm32wbxx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                  Main program
  - CRYP/CRYP_AESModes/Src/stm32wbxx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AESModes/Src/system_stm32wbxx.c      STM32WBxx system source file


@par Hardware and Software environment


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
