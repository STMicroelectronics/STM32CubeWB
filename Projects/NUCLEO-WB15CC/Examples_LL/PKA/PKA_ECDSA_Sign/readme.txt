/**
  @page PKA_ECDSA_Sign PKA : Public Key Accelerator
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ECDSA_Sign/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PKA_ECDSA_Sign example.
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

How to use the low-layer PKA API to generate an ECDSA signature.

Example execution:
After startup from reset and system configuration, PKA configuration is performed.

To demonstrate the generation of an ECDSA signature, input and output buffers of this
example are extracted from National Institute of Standards and Technology (NIST)
Cryptographic Algorithm Validation Program (CAVP) (http://csrc.nist.gov/groups/STM/cavp/).

One test vector has been extracted from section [P-256,SHA-256] of 
http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip
and adapted to C buffers in SigGen.c and SigGen.h.

The example:
  Loads the input buffers to PKA RAM.
  Launch the computation in interrupt mode.
  Wait for the interrupt callback.
  Retrieve the output buffer.
  Compare to expected results.

After successful sequence, LED2 is turned On. 
In case of errors, LED2 is slowly blinking (1sec period).

@par Keywords

Security, PKA, NIST, CAVP, ECDSA verification

@par Directory contents 

  - PKA/PKA_ECDSA_Sign/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - PKA/PKA_ECDSA_Sign/Inc/main.h                  Header for main.c module
  - PKA/PKA_ECDSA_Sign/Inc/stm32_assert.h          Template file to include assert_failed function
  - PKA/PKA_ECDSA_Sign/Src/stm32wbxx_it.c          Interrupt handlers
  - PKA/PKA_ECDSA_Sign/Src/main.c                  Main program
  - PKA/PKA_ECDSA_Sign/Src/system_stm32wbxx.c      STM32WBxx system source file
  - PKA/PKA_ECDSA_Sign/Src/prime256v1.c            Description of P-256 (ECDSA-256)
  - PKA/PKA_ECDSA_Sign/Inc/prime256v1.h            Header for prime256v1.c
  - PKA/PKA_ECDSA_Sign/Src/SigGen.c                Reflect the content of the test vector from SigGen.txt
  - PKA/PKA_ECDSA_Sign/Src/SigGen.txt              Extract from NIST CAVP
  - PKA/PKA_ECDSA_Sign/Inc/SigGen.h                Header of SigGen.c

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
