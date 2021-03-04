echo off
REM Set CubeProgrammer path (release above > 2.1)
SET PROG="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"

REM Warning: For CubeProgrammer (release 2.1), remove keytype=%KEY_TYPE% argument from commands line below
REM Please refer to RN STM32WB_Copro_Wireless_Binaries for details

REM TO flash KEY via USB (Command Line Interface)
REM 	Access to Bootloader USB Interface (system flash)
REM 	Boot mode selected by Boot0 pin set to VDD
REM 	For P-NUCLEO-WB55.Nucleo :
REM 		Jumper between CN7.5(VDD) and CN7.7(Boot0)
REM 		Power ON via USB_USER and Jumper JP1(USB_MCU)

SET PORT="usb1"  
 
REM Next commands use bootloader with USB connection
REM Store 128 bits key in clear format
SET KEY="Simple_128.bin" 
SET KEY_TYPE="1"
%PROG% -c port=%PORT% -wusrkey %KEY% keytype=%KEY_TYPE%

REM Store 256 bits key in clear format
SET KEY="Simple_256.bin" 
SET KEY_TYPE="1"
%PROG% -c port=%PORT% -wusrkey %KEY% keytype=%KEY_TYPE%

REM Store 128 bits master key in clear format
REM Index reported is 0x0: OK or 0xFF: Master key already here
SET KEY="Master_Key.bin" 
SET KEY_TYPE="2"
%PROG% -c port=%PORT% -wusrkey %KEY% keytype=%KEY_TYPE%

REM Store 128 bits key encrypted with master key
SET KEY="Encrypted_128.bin" 
SET KEY_TYPE="3"
%PROG% -c port=%PORT% -wusrkey %KEY% keytype=%KEY_TYPE%

REM Store 128 bits key encrypted with master key
SET KEY="Encrypted_256.bin" 
SET KEY_TYPE="3"
%PROG% -c port=%PORT% -wusrkey %KEY% keytype=%KEY_TYPE%

pause
