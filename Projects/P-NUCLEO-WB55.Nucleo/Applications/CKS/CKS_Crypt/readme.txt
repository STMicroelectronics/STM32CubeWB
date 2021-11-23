/**
  @page CKS_Crypt example
  
  @verbatim
  ******************************************************************************
  * @file    XXX/CKS_Crypt/readme.txt 
  * @author  MCD Application Team
  * @brief   Example of CKS feature usage.
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

How to use CKS feature to store AES crypto keys in secure area.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

CKS

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
Or it can be done by using STM32CubeProgrammer tool (Version 2.2 or higher) with bootloader USB connection.

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files with correct WRITE_KEYS define value
      - If WRITE_KEYS is defined, keys will be provisionned by the code itself.
      - If WRITE_KEYS is not defined, keys shall be provisionned by STM32CubeProgrammer with bootloader USB connection.
	    [a batch script "Store_keys.bat" is available under /CKS_Crypt/Keys]
 - Flash the board with the executable file
 - Power on the Nucleo board with the CRYP_CKS application
 
If all test are passed, LED1 is switched On.
It one test fails, LED3 is switched On

note: Access to Bootloader USB Interface (system flash) with Boot mode selected by Boot0 pin set to VDD
- For P-NUCLEO-WB55.Nucleo:
	Jumper between CN7.5(VDD) and CN7.7(Boot0)
	Power ON via USB_USER and Jumper JP1(USB_MCU)
	
- To get details on installed STM32_Programmer_CLI supported options use "--help"
- for STM32_Programmer_CLI (Version 2.2 or higher) options used:
  -wusrkey                : Write user key
     <file_path>        : User key file path
     <keytype=1|2|3>    : User key type, values : 1, 2 or 3.
                        : 1 = simple key, 2 = master key, 3 = encrypted key.
- remove option <keytype=xx> for STM32_Programmer_CLI (Version <= 2.1)
						
For more details refer to the Application Note: 
  AN5185 - ST firmware upgrade services for STM32WB Series
  UM2237 - STM32CubeProgrammer software description
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 