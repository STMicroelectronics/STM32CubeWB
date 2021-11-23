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

How to use the CRYP peripheral to encrypt and decrypt data using AES in chaining
modes (ECB, CBC, CTR).

In this example, the following key sizes are used: 128 or 256.

For this example, a firmware is used to interface with the Crypto peripheral
in polling mode.

This example behaves as follows:

1. ECB mode:
   - AES128 ECB encryption
   - AES256 ECB encryption
   - AES128 ECB decryption
   - AES256 ECB decryption
2. CBC mode:
   - AES128 CBC encryption
   - AES256 CBC encryption
   - AES128 CBC decryption
   - AES256 CBC decryption
3. CTR mode:
   - AES128 CTR encryption
   - AES256 CTR encryption
   - AES128 CTR decryption
   - AES256 CTR decryption

Official NIST test patterns available in "nistspecialpublication800-38a.pdf" reference
document are used in the example.

For CBC and CTR, data swapping as described in Reference Manual "AES data registers
and data swapping" section is applied for illustration purposes:
- byte swapping for CBC input and output data
- bit swapping for CTR input and output data.

When resorting to IAR Embedded Workbench IDE, AES algorithms processing status
are displayed on debugger Terminal I/O as follows: View --> Terminal I/O.
When resorting to MDK-ARM KEIL IDE, AES algorithms processing status
are displayed on debugger as follows:  View --> Serial Viewer --> Debug (printf) Viewer.
When resorting to STM32CubeIDE:
Command Code is displayed on debugger as follows: Window--> Show View--> Console.
In Debug configuration :
- Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
- window\ Startup,add the command "monitor arm semihosting enable"

Results can be displayed on PC (through HyperTerminal or TeraTerm) by Virtual Com Port
in enabling USE_VCP_CONNECTION compilation switch and with the proper configuration
(115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).

When all ciphering and deciphering operations are successful, LED1 is turned on.
In case of ciphering or deciphering issue, LED3 is turned on.

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
