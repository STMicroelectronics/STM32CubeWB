/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Liquid Crystal Display (LCD) module
  *          mounted on STM32WB5MM_DK evaluation board.
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive indirectly an LCD.
   - This driver supports the SSD1315 LCD.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the LCD using the BSP_LCD_Init() function.

     o Select the LCD layer to be used using the BSP_LCD_SelectLayer() function.
     o Enable the LCD display using the BSP_LCD_DisplayOn() function.
     o Disable the LCD display using the BSP_LCD_DisplayOff() function.
     o Refresh the LCD display using the BSP_LCD_Refresh() function.
     o Set Page of the LCD display using the BSP_LCD_SetPage() function.
     o Set Column of the LCD display using the BSP_LCD_SetColumn() function.
     o Setup Scrolling of the LCD display using the BSP_LCD_ScrollingSetup() function.
     o Set the display brightness using the BSP_LCD_SetBrightness() function.
     o Get the display brightness using the BSP_LCD_GetBrightness() function.
     o Write a pixel to the LCD memory using the BSP_LCD_WritePixel() function.
     o Read a pixel from the LCD memory using the BSP_LCD_ReadPixel() function.
     o Draw an horizontal line using the BSP_LCD_DrawHLine() function.
     o Draw a vertical line using the BSP_LCD_DrawVLine() function.
     o Draw a bitmap image using the BSP_LCD_DrawBitmap() function.
     o Shift a bitmap image using the BSP_LCD_ShiftBitmap() function.

  + Display on LCD
     o Clear the whole LCD using the BSP_LCD_Clear() function.
     o Display a character on the specified line and column using the UTIL_LCD_DisplayChar()
       function or a complete string line using the UTIL_LCD_DisplayStringAtLine() function.
     o Display a string line on the specified position (x,y in pixel) and align mode
       using the UTIL_LCD_DisplayStringAtLine() function.
     o Draw and fill a basic shapes (dot, line, rectangle, circle, ellipse, .. bitmap, raw picture)
       on LCD using a set of functions.
  Note:
  --------
    Regarding the "Instance" parameter, needed for all functions, it is used to select
    an LCD instance. On the STM32WB5MM_DK board, there's one instance. Then, this
    parameter should be 0.

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_lcd.h"
#include "stm32wb5mm_dk_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */

/** @defgroup STM32WB5MM_DK_LCD STM32WB5MM_DK LCD
  * @brief      This file includes the LCD driver of
  *             STM32WB5MM_DK boards.
  * @{
  */

/** @defgroup STM32WB5MM_DK_LCD_Private_Constants Private Constants
  * @{
  */

/* LINK UTIL LCD */
const LCD_UTILS_Drv_t LCD_Driver =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  BSP_LCD_GetXSize,
  BSP_LCD_GetYSize,
  NULL,
  BSP_LCD_GetPixelFormat
};
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Private_Macros Private Macros
  * @{
  */
#define POLY_X(Z)               ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)               ((int32_t)((Points + (Z))->Y))
#define ABS(X)  (((X) > 0U) ? (X) : -(X))

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Exported_Variables Exported Variables
  * @{
  */
void                 *LcdCompObj = NULL;
BSP_LCD_Ctx_t        LcdCtx[LCD_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Private_Variables Private Variables
  * @{
  */
static SSD1315_Drv_t     *LcdDrv = NULL;
/**
  * @}
  */

/** STM32WB5MM_DK_LCD_Private_FunctionPrototypes Private Functions
  */
/*******************************************************************************
 ********************************* LINK LCD ***********************************/
static void LCD_MspInit(void);
static void LCD_MspDeInit(void);
static int32_t LCD_IO_Init(void);
static int32_t LCD_IO_DeInit(void);

#if (USE_LCD_CTRL_SSD1315 == 1)
static int32_t SSD1315_Probe(uint32_t Orientation);
#endif /* USE_LCD_CTRL_SSD1315 */
/**
  */

/** @addtogroup STM32WB5MM_DK_LCD_Exported_Functions
  * @{
  */
/**
  * @brief  Initializes the LCD.
  * @param  Instance LCD Instance
  * @param  Orientation LCD_ORIENTATION_LANDSCAPE
  * @retval BSP status
  */
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR )
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Orientation == LCD_ORIENTATION_LANDSCAPE)
    {
      LcdCtx[Instance].Width  = LCD_DEFAULT_WIDTH;
      LcdCtx[Instance].Height = LCD_DEFAULT_HEIGHT;
    }
    else
    {
      LcdCtx[Instance].Width  = LCD_DEFAULT_HEIGHT;
      LcdCtx[Instance].Height = LCD_DEFAULT_WIDTH;
    }
    
    /* registers the function and initialize the controller */
    if(SSD1315_Probe(Orientation) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
  }

  return ret;
}

/**
  * @brief  De-Initializes the LCD resources.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->DeInit(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  LCD_MspDeInit();

  return ret;
}

/**
  * @brief  Gets the LCD Active LCD Pixel Format.
  * @param  Instance LCD Instance
  * @param  PixelFormat Active LCD Pixel Format
  * @retval BSP status
  */
int32_t BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @param  Instance LCD Instance
  * @param  pXSize pointer to Used LCD X size
  * @retval BSP status
  */
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *pXSize)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetXSize != NULL)
  {
    if(LcdDrv->GetXSize(LcdCompObj, pXSize) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    *pXSize = LcdCtx[Instance].Width;
  }
  
  return ret;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance LCD Instance
  * @param  pYSize pointer to Used LCD Y size
  * @retval BSP status
  */
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *pYSize)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetYSize != NULL)
  {
    if(LcdDrv->GetYSize(LcdCompObj, pYSize) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    *pYSize = LcdCtx[Instance].Height;
  }

  return ret;
}

/**
  * @brief  Switch On the display.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DisplayOn != NULL)
  {
    if(LcdDrv->DisplayOn(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Switch Off the display.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DisplayOff != NULL)
  {
    if(LcdDrv->DisplayOff(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Refresh the display.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_Refresh(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->Refresh != NULL)
  {
    if(LcdDrv->Refresh(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Set Page.
  * @param  Instance LCD Instance
  * @param  Page LCD Page
  * @retval BSP status
  */
int32_t BSP_LCD_SetPage(uint32_t Instance, uint16_t Page)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetPage != NULL)
  {
    if(LcdDrv->SetPage(LcdCompObj, Page) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Set Column.
  * @param  Instance LCD Instance
  * @param  Column LCD Column
  * @retval BSP status
  */
int32_t BSP_LCD_SetColumn(uint32_t Instance, uint16_t Column)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetColumn != NULL)
  {
    if(LcdDrv->SetColumn(LcdCompObj, Column) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Scrolling Setup.
  * @param  Instance LCD Instance
  * @param  ScrollMode LCD Scroll Mode: SSD1315_SCROLL_RIGHT or SSD1315_SCROLL_LEFT
  * @param  StartPage LCD Start page  for scrolling: 0..7
  * @param  EndPage LCD End page for scrolling: This must be larger or equal to StartLine 0..7
  * @param  Frequency LCD Frequency: SSD1315_SCROLL_FREQ_2FRAMES to SSD1315_SCROLL_FREQ_256FRAMES
  * @retval BSP status
  */
int32_t BSP_LCD_ScrollingSetup(uint32_t Instance, uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->ScrollingSetup != NULL)
  {
    if(LcdDrv->ScrollingSetup(LcdCompObj, ScrollMode, StartPage, EndPage, Frequency) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Scrolling Start.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_ScrollingStart(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->ScrollingStart != NULL)
  {
    if(LcdDrv->ScrollingStart(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Scrolling Stop.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_ScrollingStop(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->ScrollingStop != NULL)
  {
    if(LcdDrv->ScrollingStop(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}


/**
  * @brief  Set the brightness value
  * @param  Instance LCD Instance
  * @param  Brightness [00: Min (black), 100 Max]
  * @retval BSP status
  */
int32_t BSP_LCD_SetBrightness(uint32_t Instance, uint32_t Brightness)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetBrightness != NULL)
  {
    if(LcdDrv->SetBrightness(LcdCompObj, Brightness) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  
  return ret;
}

/**
  * @brief  Get the brightness value
  * @param  Instance LCD Instance
  * @param  Brightness [00: Min (black), 100 Max]
  * @retval BSP status
  */
int32_t BSP_LCD_GetBrightness(uint32_t Instance, uint32_t *Brightness)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetBrightness != NULL)
  {
    if(LcdDrv->GetBrightness(LcdCompObj, Brightness) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Set the LCD Orientation.
  * @param  Instance LCD Instance
  * @param  Orientation LCD orientation to set
  * @retval BSP status
  */
int32_t  BSP_LCD_SetOrientation(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetOrientation != NULL)
  {
    if(LcdDrv->SetOrientation(LcdCompObj, Orientation) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Get the LCD orientation.
  * @param  Instance LCD Instance
  * @param  Orientation LCD Orientation used
  * @retval BSP status
  */
int32_t  BSP_LCD_GetOrientation(uint32_t Instance, uint32_t *Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetOrientation != NULL)
  {
    if(LcdDrv->GetOrientation(LcdCompObj, Orientation) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Get the LCD orientation.
  * @param  Instance LCD Instance
  * @param  Xpos to set the cursor
  * @param  Ypos to set the cursor
  * @retval BSP status
  */
int32_t  BSP_LCD_SetCursor(uint32_t Instance, uint32_t Xpos, uint32_t Ypos)
{
 int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetCursor != NULL)
  {
    if(LcdDrv->SetCursor(LcdCompObj, Xpos, Ypos) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash in currently active layer.
  * @param  Instance LCD Instance
  * @param  Xpos Bmp X position in the LCD
  * @param  Ypos Bmp Y position in the LCD
  * @param  pBmp Pointer to Bmp picture address in the internal Flash
  * @retval BSP status
  */
int32_t BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DrawBitmap != NULL)
  {
    /* Draw the bitmap on LCD */
    if (LcdDrv->DrawBitmap(LcdCompObj, Xpos, Ypos, pBmp) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Draws Shift bitmap picture loaded in the internal Flash in currently active layer.
  * @param  Instance LCD Instance
  * @param  Xpos Bmp X position in the LCD
  * @param  Ypos Bmp Y position in the LCD
  * @param  Xshift  specifies number of pixel to shift on X position.
  * @param  Yshift  specifies number of pixel to shift on Y position.
  * @param  pBmp Pointer to Bmp picture address in the internal Flash
  * @retval BSP status
  */
int32_t BSP_LCD_ShiftBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, int16_t Xshift, int16_t Yshift, uint8_t *pBmp)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->ShiftBitmap != NULL)
  {
    /* shift bitmap on LCD */
    if (LcdDrv->ShiftBitmap(LcdCompObj, Xpos, Ypos, Xshift, Yshift, pBmp) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Fill a rectangle with a BitMap on LCD.
  * @param  Instance LCD Instance.
  * @param  pData Pointer to RGB line data
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Width width of the rectangle to fill.
  * @param  Height height of the rectangle to fill.
  * @retval BSP status.
  */
int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
   int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->FillRGBRect != NULL)
  {
    /* shift bitmap on LCD */
    if (LcdDrv->FillRGBRect(LcdCompObj, Xpos, Ypos, pData, Width, Height) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Draws an horizontal line
  * @param  Instance LCD instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @param  Color Line color
  * @retval BSP status
  */
int32_t BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }

  else if(LcdDrv->DrawHLine != NULL)
  {
    /* Draw the horizontal line on LCD */
    if (LcdDrv->DrawHLine(LcdCompObj, Xpos, Ypos, Length, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Draws a vertical line
  * @param  Instance LCD instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @param  Color Line color
  * @retval BSP status
  */
int32_t BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DrawVLine != NULL)
  {
    /* Draw the vertical line on LCD */
    if (LcdDrv->DrawVLine(LcdCompObj, Xpos, Ypos, Length, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Draws a full rectangle in currently active layer.
  * @param  Instance LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @param  Color Color of rectangle
  * @retval BSP status
  */
int32_t BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->FillRect(LcdCompObj, Xpos, Ypos, Width, Height, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  clear the LCD in currently active layer.
  * @param  Instance LCD Instance
  * @param  Color to set
  * @retval BSP status
  */
int32_t BSP_LCD_Clear(uint32_t Instance, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->FillRect(LcdCompObj, 0, 0, LcdCtx[Instance].Width, LcdCtx[Instance].Height, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}


/**
  * @brief  Reads a LCD pixel color.
  * @param  Instance LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color pointer to RGB pixel color
  * @retval BSP status
  */
int32_t  BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->GetPixel(LcdCompObj, Xpos, Ypos, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Writes a LCD pixel.
  * @param  Instance LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color RGB pixel color
  * @retval BSP status
  */
int32_t  BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->SetPixel(LcdCompObj, Xpos, Ypos, Color) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}


/**
  * @brief  Writes register on LCD register.
  * @param  Reg Register to be written
  * @param  pData pointer to the read data from LCD SRAM.
  * @param  Length length of data be read from the LCD SRAM
  * @retval BSP status
  */
int32_t BSP_LCD_WriteReg(uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Send Data */
  if((ret == BSP_ERROR_NONE) && (Length > 0U))
  {
    if(BSP_LCD_SendData(pData, Length) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Send data to select the LCD SRAM.
  * @param  pData pointer to data to write to LCD SRAM.
  * @param  Length length of data to write to LCD SRAM
  * @retval Error status
  */
int32_t BSP_LCD_SendData(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;
  if(Length==1)
  {
    /* Reset LCD control line CS */
    LCD_CS_LOW();
    LCD_DC_LOW();
      /* Send Data */
      if(BSP_SPI1_Send(pData, Length)!= BSP_ERROR_NONE)
    {
        ret = BSP_ERROR_BUS_FAILURE;
    }
    /* Deselect : Chip Select high */
    LCD_CS_HIGH();
  }
  else
  { 
    LCD_CS_LOW();
    LCD_DC_HIGH();
      /* Send Data */
      if(BSP_SPI1_Send(pData, Length)!= BSP_ERROR_NONE)
    {
        ret = BSP_ERROR_BUS_FAILURE;
    }
    LCD_DC_LOW() ;
    /* Deselect : Chip Select high */
    LCD_CS_HIGH();
  }
  
  return ret;
}

/**
  * @brief  Read data from LCD data register.
  * @param  Reg Register to be read
  * @param  pData pointer to the read data from LCD SRAM.
  * @param  Length length of data be read from the LCD SRAM
  * @retval BSP status
  */
int32_t BSP_LCD_ReadReg(uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;
  UNUSED(Length);
  
  /* Send Reg value to Read */
  if(BSP_LCD_WriteReg(Reg, pData, 0) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();
  
  if (ret == BSP_ERROR_NONE)
  { 
    if(BSP_SPI1_Recv(pData, 2) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
  
  return ret;
}

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_LCD_Private_Functions Private Functions
  * @{
  */

/*******************************************************************************
 ********************************* LINK LCD ***********************************/

/**
  * @brief  Initialize the BSP LTDC Msp.
  * @retval None
  */
static void LCD_MspInit(void)
{
  /* turn LCD on = drive pin low (active low) */
  LCD_CS_LOW();
}

/**
  * @brief  De-Initializes the BSP LTDC Msp
  * @retval None
  */
static void LCD_MspDeInit(void)
{
  /* turn LCD off = drive pin high (active low) */
  LCD_CS_HIGH();
}

#if (USE_LCD_CTRL_SSD1315 == 1)
/**
  * @brief  Register Bus IOs for instance 0 if SSD1315 ID is OK
  * @param  Orientation
  * @retval BSP status
  */
static int32_t SSD1315_Probe(uint32_t Orientation)
{
  int32_t                 ret = BSP_ERROR_NONE;
  SSD1315_IO_t            IOCtx;
  static SSD1315_Object_t SSD1315Obj;
  
  /* Configure the lcd driver : map to LCD_IO function*/
  IOCtx.Init             = LCD_IO_Init;
  IOCtx.DeInit           = LCD_IO_DeInit;
  IOCtx.ReadReg          = BSP_LCD_ReadReg;
  IOCtx.WriteReg         = BSP_LCD_WriteReg;
  IOCtx.GetTick          = BSP_GetTick;
  
  if(SSD1315_RegisterBusIO(&SSD1315Obj, &IOCtx) != SSD1315_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    LcdCompObj = &SSD1315Obj;
    
    LCD_MspInit();
    
    /* LCD Initialization */
    LcdDrv = (SSD1315_Drv_t *)&SSD1315_Driver;
    if(LcdDrv->Init(LcdCompObj, SSD1315_FORMAT_DEFAULT, Orientation) != SSD1315_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  
  return ret;
}
#endif


/**
  * @brief  Initializes LCD low level.
  * @retval BSP status
  */
static int32_t LCD_IO_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure the LCD Chip Select pin --------------------------------------*/
  LCD_CS_GPIO_CLK_ENABLE();
  
  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_CS_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStruct);
  
  /* Set or Reset the control line */
  LCD_CS_LOW();
  
  /* Configure the LCD Data/Control pin -------------------------------------*/
  LCD_DC_GPIO_CLK_ENABLE();
  
  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_DC_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStruct);
  
  /* Set or Reset the control line */
  LCD_DC_LOW();
  
  /* Configure the LCD Reset pin --------------------------------------------*/
  LCD_RST_GPIO_CLK_ENABLE();
  
  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_RST_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);
  
  if (BSP_SPI1_Init() != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  LCD_RST_LOW();
  HAL_Delay(1);
  LCD_RST_HIGH();
  return ret;
}

/**
  * @brief  DeInitializes LCD low level
  * @retval BSP status
  */
static int32_t LCD_IO_DeInit(void)
{
  HAL_GPIO_DeInit(LCD_CS_GPIO_PORT, LCD_CS_PIN);
  HAL_GPIO_DeInit(LCD_DC_GPIO_PORT, LCD_DC_PIN);
  /* Uninitialize LCD Reset Pin */  
  HAL_GPIO_DeInit(LCD_RST_GPIO_PORT, LCD_RST_PIN);

  return BSP_ERROR_NONE;
}
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
