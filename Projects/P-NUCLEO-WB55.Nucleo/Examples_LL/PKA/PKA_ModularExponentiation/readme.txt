/**
  @page PKA_ModularExponentiation PKA : Public Key Accelerator
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ModularExponentiation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PKA_ModularExponentiation example.
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

How to use the low-layer PKA API to execute RSA modular exponentiation.

Example execution:
After startup from reset and system configuration, PKA configuration is performed.

To demonstrate the RSA modular exponentiation, four file have been created using an
external tool, openssl:
  Src/rsa_priv_2048.pem (Refer to rsa_priv_2048.c for its creation)
  Src/rsa_pub_2048.pem  (extracted from rsa_priv_2048.pem. Refer to rsa_pub_2048.c 
                        for its creation)
  Src/plaintext.bin     (Refer to plaintext.c for its creation)
  Src/cipher.bin        (build from rsa_pub_2048.pem and plaintext.bin. Refer to 
                        plaintext.c for its creation)

The example:
  Compute ciphertext.bin from plaintext.bin and rsa_pub_2048.pem (This is done
    with the public key)
  Compute plaintext.bin from ciphertext.bin and rsa_priv_2048.pem

After successful sequence, LED2 is turned On. 
In case of errors, LED2 is slowly blinking (1sec period).

@par Keywords

Security, PKA, RSA, modular exponentiation, cipher ,decipher

@par Directory contents 

  - PKA/PKA_ModularExponentiation/Inc/stm32wbxx_it.h          Interrupt handlers header file
  - PKA/PKA_ModularExponentiation/Inc/main.h                  Header for main.c module
  - PKA/PKA_ModularExponentiation/Inc/stm32_assert.h          Template file to include assert_failed function
  - PKA/PKA_ModularExponentiation/Src/stm32wbxx_it.c          Interrupt handlers
  - PKA/PKA_ModularExponentiation/Src/main.c                  Main program
  - PKA/PKA_ModularExponentiation/Src/system_stm32wbxx.c      STM32WBxx system source file
  - PKA/PKA_ModularExponentiation/Src/plaintext.bin           Random 2048 bits
  - PKA/PKA_ModularExponentiation/Src/plaintext.c             Reflect the content of plaintext.bin in array format
  - PKA/PKA_ModularExponentiation/Inc/plaintext.h             Header for plaintext.c
  - PKA/PKA_ModularExponentiation/Src/ciphertext.bin          Ciphered plaintext.bin using openssl
  - PKA/PKA_ModularExponentiation/Src/ciphertext.c            Reflect the content of ciphertext.bin in array format
  - PKA/PKA_ModularExponentiation/Inc/ciphertext.h            Header for ciphertext.c
  - PKA/PKA_ModularExponentiation/Src/rsa_priv_2048.pem       RSA private key generated using openssl
  - PKA/PKA_ModularExponentiation/Src/rsa_priv_2048.c         Reflect the content of rsa_priv_2048.pem in array format
  - PKA/PKA_ModularExponentiation/Inc/rsa_priv_2048.h         Header for rsa_priv_2048.c
  - PKA/PKA_ModularExponentiation/Src/rsa_pub_2048.pem        RSA public key extracted from rsa_priv_2048.pem using openssl
  - PKA/PKA_ModularExponentiation/Src/rsa_pub_2048.c          Reflect the content of rsa_pub_2048.pem in array format
  - PKA/PKA_ModularExponentiation/Inc/rsa_pub_2048.h          Header for rsa_pub_2048.c

@par Hardware and Software environment

  - This example runs on STM32WB55RGVx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example and observe the LED status

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
