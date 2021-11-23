/**
  @page LCD_SegmentsDrive LCD Glass example
  
  @verbatim
  ******************************************************************************
  * @file    LCD/LCD_SegmentsDrive/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LCD Glass example
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

How to drive a LCD Glass using STM32WBxx hal driver.

This example demonstrate how to display a string on the LCD, use the blinking function of the LCDGlass peripheral.

In addition, some API are provided to easily display letters, numbers and available battery bars.

The P-NUCLEO-WB55 kit does not embeds a LCDGlass. For this example, a customisation of P-NUCLEO-WB55.Nucleo
board is necessary to reproduce the setup presented in the picture setup.jpg.

The LCD Glass of the following boards can be reused for this setup:
  32L476GDISCOVERY: https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-discovery-kits/32l476gdiscovery.html
  32L152CDISCOVERY: https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-discovery-kits/32l152cdiscovery.html
It is a LCD with 24 segments and 4 commons able to display 14-segments digits and 4 bars.

The following manual connections are required:
  (refer to P-NUCLEO-WB55 schematics (MB1355) for STM32WB55RGVx GPIOs physical connections on Morpho connector)
  (refer to 32L152CDISCOVERY (UM1079) for LCDGlass connections and for more information on each segment and common usage)
  
  STM32WB55RGVx   LCDGlass
  GPIO AF11       PIN  FUNCTION
  PA1  SEG0       1    LCDSEG0
  PA2  SEG1       2    LCDSEG1
  PA3  SEG2       3    LCDSEG2
  PA6  SEG3       4    LCDSEG3
  PA7  SEG4       5    LCDSEG4
  PC12 SEG30 /!\  6    LCDSEG5
  PB6  SEG6       7    LCDSEG6
  PC10 SEG28 /!\  8    LCDSEG7
  PB4  SEG8       9    LCDSEG8
  PC6  SEG24 /!\  10   LCDSEG9
  PB10 SEG10      11   LCDSEG10
  PB11 SEG11      12   LCDSEG11
  PB9  COM3       13   LCDCOM3
  PA10 COM2       14   LCDCOM2
  PA9  COM1       15   LCDCOM1
  PA8  COM0       16   LCDCOM0
  PB12 SEG12      17   LCDSEG12
  PB13 SEG13      18   LCDSEG13
  PB14 SEG14      19   LCDSEG14
  PB15 SEG15      20   LCDSEG15
  PB8  SEG16      21   LCDSEG16
  PA15 SEG17      22   LCDSEG17
  PC0  SEG18      23   LCDSEG18
  PC1  SEG19      24   LCDSEG19
  PC2  SEG20      25   LCDSEG20
  PB7  SEG21      26   LCDSEG21
  PC4  SEG22      27   LCDSEG22
  PC5  SEG23      28   LCDSEG23

  MCU PA14 SEG5 cannot be used as multiplexed with JTAG connection.
  MCU PA4 SEG5 cannot be used as MCU SEG10 share the same pin on Morpho connector CN10.
  => MCU SEG5 is replaced by MCU SEG30 on Pin PC12 which is directly available on Morpho connector CN10
  
  MCU PB3 SEG7 cannot be used as sharing the same pin as MCU PB13 SEG13 on CN10
  => MCU SEG7 is replaced by MCU SEG28 on Pin PC10 which is directly available on Morpho connector CN10
  
  MCU PB5 SEG9 cannot be used as sharing the same pin as MCU PB15 SEG15 on CN10
  => MCU SEG9 is replaced by MCU SEG24 on Pin PC10 which is directly available on Morpho connector CN10
  
The following hardware modifications are required on P-NUCLEO-WB55.Nucleo board:
  Solder bridge SB5 must be OPEN
  Solder bridge SB6 must be CLOSED

You should note that, as the PC10 SEG28 and PC12 SEG30 are used, the LCDGlass peripheral must be configured according to 
"COM and SEG multiplexing" chapter from RM0434 for a VFQFPN68 device:
  Duty: 1/4
  MUX_SEG: 1

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

LCD Glass

@par Directory contents 
  
  - LCD/LCD_SegmentsDrive/Inc/stm32wbxx_hal_conf.h         HAL configuration file
  - LCD/LCD_SegmentsDrive/Inc/stm32wbxx_it.h               Interrupt handlers header file
  - LCD/LCD_SegmentsDrive/Inc/main.h                       Header for main.c module
  - LCD/LCD_SegmentsDrive/Src/stm32wbxx_it.c               Interrupt handlers
  - LCD/LCD_SegmentsDrive/Src/main.c                       Main program
  - LCD/LCD_SegmentsDrive/Src/stm32wbxx_hal_msp.c          HAL MSP module 
  - LCD/LCD_SegmentsDrive/Src/system_stm32wbxx.c           STM32WBxx system source file
  - LCD/LCD_SegmentsDrive/Src/stm32wbxx_nucleo_glass_lcd.c BSP drivers of the LCD Glass
  - LCD/LCD_SegmentsDrive/Inc/stm32wbxx_nucleo_glass_lcd.h Header for stm32wbxx_nucleo_glass_lcd.c

@par Hardware and Software environment

  - This example runs on STM32WB55xx devices.
  
  - This example has been tested with an STMicroelectronics P-NUCLEO-WB55.Nucleo
    board with manual modifications and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 