/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_lcd.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32wb5mm_dk_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB5MM_DK_LCD_H
#define SSTM32WB5MM_DK_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_errno.h"
#include "../Common/lcd.h"

/* Include audio component Driver */
#ifndef USE_LCD_CTRL_SSD1315
#define USE_LCD_CTRL_SSD1315          1U
#endif

#if (USE_LCD_CTRL_SSD1315 == 1)
/* Include audio component Driver */
#include "../Components/ssd1315/ssd1315.h"
#endif
#include "../../../Utilities/Fonts/fonts.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */

/** @defgroup STM32WB5MM_DK_LCD STM32WB5MM_DK LCD
  * @{
  */

/** @defgroup STM32WB5MM_DK_LCD_Exported_Constants Exported Constants
  * @{
  */
#define LCD_INSTANCES_NBR                1U
#define LCD_ORIENTATION_LANDSCAPE        SSD1315_ORIENTATION_LANDSCAPE
#define LCD_DEFAULT_WIDTH                SSD1315_LCD_PIXEL_WIDTH
#define LCD_DEFAULT_HEIGHT               SSD1315_LCD_PIXEL_HEIGHT
#define LCD_COLOR_BLACK                  SSD1315_COLOR_BLACK
#define LCD_COLOR_WHITE                  SSD1315_COLOR_WHITE
   
/*##################### LCD ###################################*/
/**
  * @brief  LCD Chip Select macro definition
  */
 #define LCD_CS_LOW()                    HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
 #define LCD_CS_HIGH()                   HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Control pins
  */
#define LCD_CS_PIN                      GPIO_PIN_0		/* PH. 0*/
#define LCD_CS_GPIO_PORT                GPIOH			/* GPIOH */
#define LCD_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOH_CLK_DISABLE()

/**
  * @brief  LCD Reset macro definition
  */
#define LCD_RST_LOW()                    HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()                   HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Reset pins
  */
#define LCD_RST_PIN                     GPIO_PIN_8		/* PC. 08*/
#define LCD_RST_GPIO_PORT               GPIOC			/* GPIOC */
#define LCD_RST_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define LCD_RST_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOC_CLK_DISABLE()

/**
  * @brief  LCD Data/Command macro definition
  */
#define LCD_DC_LOW()                    HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()                   HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)

/**
  * @brief  
  */
#define LCD_DC_PIN                      GPIO_PIN_9		/* PC. 09*/
#define LCD_DC_GPIO_PORT                GPIOC    		/* GPIOC */
#define LCD_DC_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()
  
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Exported_Types Exported Types
  * @{
  */
typedef struct
{
  uint32_t Width;
  uint32_t Height;
  uint32_t IsMspCallbacksValid;
}BSP_LCD_Ctx_t;

/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_LCD_Private_Variables
  * @{
  */
extern void                 *LcdCompObj;
extern BSP_LCD_Ctx_t        LcdCtx[];
extern const LCD_UTILS_Drv_t LCD_Driver;
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Exported_Functions Exported Functions
  * @{
  */
int32_t  BSP_LCD_Init(uint32_t Instance, uint32_t Orientation);
int32_t  BSP_LCD_DeInit(uint32_t Instance);

/* LCD generic APIs: Display control */
int32_t  BSP_LCD_DisplayOn(uint32_t Instance);
int32_t  BSP_LCD_DisplayOff(uint32_t Instance);
int32_t  BSP_LCD_SetBrightness(uint32_t Instance, uint32_t Brightness);
int32_t  BSP_LCD_GetBrightness(uint32_t Instance, uint32_t *Brightness);
int32_t  BSP_LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t  BSP_LCD_GetYSize(uint32_t Instance, uint32_t *YSize);
int32_t  BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat);
int32_t  BSP_LCD_SetOrientation(uint32_t Instance, uint32_t Orientation);
int32_t  BSP_LCD_GetOrientation(uint32_t Instance, uint32_t *Orientation);
int32_t  BSP_LCD_Refresh(uint32_t Instance);
int32_t  BSP_LCD_SetPage(uint32_t Instance, uint16_t Page);
int32_t  BSP_LCD_SetColumn(uint32_t Instance, uint16_t Column);
int32_t  BSP_LCD_ScrollingSetup(uint32_t Instance, uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency);
int32_t  BSP_LCD_ScrollingStart(uint32_t Instance);
int32_t  BSP_LCD_ScrollingStop(uint32_t Instance);
int32_t  BSP_LCD_SetCursor(uint32_t Instance, uint32_t Xpos, uint32_t Ypos);
int32_t  BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t  BSP_LCD_ShiftBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, int16_t Xshift, int16_t Yshift, uint8_t *pBmp);
int32_t  BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t  BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t  BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
int32_t  BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
int32_t  BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t  BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t  BSP_LCD_Clear(uint32_t Instance, uint32_t Color);
int32_t  BSP_LCD_SetActiveLayer(uint32_t Instance, uint32_t LayerIndex);
int32_t  BSP_LCD_SetPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t  BSP_LCD_GetPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);

/* LCD specific APIs */
int32_t  BSP_LCD_WriteReg(uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t  BSP_LCD_ReadReg(uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t  BSP_LCD_SendData(uint8_t *pData, uint16_t Length);
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

#endif /* STM32WB5MM_DK_LCD_H */
