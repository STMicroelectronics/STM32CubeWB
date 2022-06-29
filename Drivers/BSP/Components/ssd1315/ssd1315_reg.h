/**
  ******************************************************************************
  * @file    ssd1315_reg.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          ssd1315_reg.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SSD1315_REG_H
#define SSD1315_REG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup SSD1315
  * @{
  */

/** @defgroup SSD1315_REG_Exported_Constants SSD1315 REG Exported Constants
  * @{
  */
  
/**
  * @brief  SSD1315 Colors
  */
#define  SSD1315_COLOR_WHITE            0xFF
#define  SSD1315_COLOR_BLACK            0x00

/**
  * @brief  SSD1315 Scrolling
  */
#define  SSD1315_SCROLL_RIGHT           0x26
#define  SSD1315_SCROLL_LEFT            0x27
#define  SSD1315_ACTIVATE_SCROLL        0x2F
#define  SSD1315_DESACTIVATE_SCROLL     0x2E

#define  SSD1315_SCROLL_FREQ_2FRAMES    0x07
#define  SSD1315_SCROLL_FREQ_3FRAMES    0x04
#define  SSD1315_SCROLL_FREQ_4FRAMES    0x05
#define  SSD1315_SCROLL_FREQ_5FRAMES    0x00
#define  SSD1315_SCROLL_FREQ_25FRAMES   0x06
#define  SSD1315_SCROLL_FREQ_64FRAMES   0x01
#define  SSD1315_SCROLL_FREQ_128FRAMES  0x02
#define  SSD1315_SCROLL_FREQ_256FRAMES  0x03

/**
  * @brief  SSD1315 Commands
  */
#define  SSD1315_CHARGE_PUMP_SETTING            0x8D
#define  SSD1315_READWRITE_CMD                  0x80
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_1   0x10
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_2   0x11
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_3   0x12
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_4   0x13
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_5   0x14
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_6   0x15
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_7   0x16
#define  SSD1315_HIGHER_COLUMN_START_ADRESS_8   0x17
#define  SSD1315_LOWER_COLUMN_START_ADRESS      0x00
#define  SSD1315_LOWER_COLUMN_START_ADRESS_15   0x0F
#define  SSD1315_MEMORY_ADRESS_MODE             0x20
#define  SSD1315_SET_COLUMN_ADRESS              0x21
#define  SSD1315_SET_PAGE_ADRESS                0x22
#define  SSD1315_DISPLAY_START_LINE_1           0x40
#define  SSD1315_DISPLAY_START_LINE_32          0x1F
#define  SSD1315_DISPLAY_START_LINE_64          0x7F
#define  SSD1315_REMAPPED_MODE                  0xC8
#define  SSD1315_CONTRAST_CONTROL               0xA1
#define  SSD1315_CONTRAST_CONTROL_2             0xFF
#define  SSD1315_DISPLAY_ON                     0xAF
#define  SSD1315_DISPLAY_OFF                    0xAE
#define  SSD1315_SET_PAGE_START_ADRESS          0xB0

/**
  * @}
  */

/** @defgroup SSD1315_REG_Exported_Types SSD1315 REG Exported Types
  * @{
  */
typedef int32_t (*SSD1315_Write_Func)    (void *, uint16_t, uint8_t*, uint16_t);    /*  this will connect to the LCD_IO_WriteData */
typedef int32_t (*SSD1315_Read_Func)     (void *, uint16_t, uint8_t*, uint16_t);   /*  this will connect to the LCD_IO_ReadData */

typedef struct
{
  SSD1315_Write_Func     WriteReg;
  SSD1315_Read_Func      ReadReg;
  void                   *handle;
} ssd1315_ctx_t;
/**
  * @}
  */

/** @defgroup SSD1315_REG_Exported_Functions SSD1315 REG Exported Functions
  * @{
  */
int32_t ssd1315_write_reg(ssd1315_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t ssd1315_read_reg(ssd1315_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SSD1315_REG_H */
