echo off
REM Set CubeProgrammer path release 2.1 and above
SET PROG="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer2\bin\STM32_Programmer_CLI.exe"

REM Next commands use bootloader with USB connection
REM Store 128 bits key in clear format
%PROG% -c port=usb1 -wusrkey Simple_128.bin

REM Store 256 bits key in clear format
%PROG% -c port=usb1 -wusrkey Simple_256.bin

REM Store 128 bits master key in clear format
REM Index reported is 0x0: OK or 0xFF: Master key already here
%PROG% -c port=usb1 -wusrkey Master.bin

REM Store 128 bits key encrypted with master key
%PROG% -c port=usb1 -wusrkey Encrypted_128.bin

REM Store 128 bits key encrypted with master key
%PROG% -c port=usb1 -wusrkey Encrypted_256.bin
pause
