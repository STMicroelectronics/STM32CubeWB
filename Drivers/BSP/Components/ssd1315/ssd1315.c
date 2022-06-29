/**
  ******************************************************************************
  * @file    ssd1315.c
  * @author  MCD Application Team
  * @brief   This file includes the LCD driver for SSD1315 LCD.
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

/* Includes ------------------------------------------------------------------*/
#include "ssd1315.h"
#include <stdio.h>
#include <string.h>
/** @addtogroup BSP
* @{
*/

/** @addtogroup Components
* @{
*/

/** @addtogroup SSD1315
* @brief     This file provides a set of functions needed to drive the
*            SSD1315 LCD.
* @{
*/

/** @defgroup SSD1315_Private_TypesDefinitions
* @{
*/

/**
* @}
*/

/** @defgroup SSD1315_Private_Defines
* @{
*/

/**
* @}
*/

/** @defgroup SSD1315_Private_Macros
* @{
*/

/**
* @}
*/

/** @defgroup SSD1315_Private_Variables
* @{
*/
SSD1315_Drv_t   SSD1315_Driver =
{
  SSD1315_Init,
  SSD1315_DeInit,
  SSD1315_ReadID,
  SSD1315_DisplayOn,
  SSD1315_DisplayOff,
  SSD1315_SetBrightness,
  SSD1315_GetBrightness,
  SSD1315_SetOrientation,
  SSD1315_GetOrientation,
  SSD1315_Refresh,
  SSD1315_SetPage,
  SSD1315_SetColumn,
  SSD1315_ScrollingSetup,
  SSD1315_ScrollingStart,
  SSD1315_ScrollingStop,
  SSD1315_SetCursor,
  SSD1315_DrawBitmap,
  SSD1315_ShiftBitmap,
  SSD1315_FillRGBRect,
  SSD1315_DrawHLine,
  SSD1315_DrawVLine,
  SSD1315_FillRect,
  SSD1315_GetPixel,
  SSD1315_SetPixel,
  SSD1315_GetXSize,
  SSD1315_GetYSize,
};

#if defined ( __ICCARM__ )  /* IAR Compiler */
  #pragma data_alignment = 16
uint8_t              PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER];
#elif defined (__GNUC__)    /* GNU Compiler */
uint8_t              PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER] __attribute__ ((aligned (16)));
#else                       /* ARM Compiler */
__align(16) uint8_t  PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER];
#endif /* __ICCARM__ */
/* The below table handle the different values to be set to Memory Data Access Control
   depending on the orientation and pbm image writing where the data order is inverted
*/

/**
* @}
*/

/** @defgroup SSD1315_Private_FunctionPrototypes Private Functions Prototypes
* @{
*/
static int32_t SSD1315_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length);
static int32_t SSD1315_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length);
static int32_t SSD1315_IO_Delay(SSD1315_Object_t *pObj, uint32_t Delay);
static void ssd1315_Clear(uint16_t ColorCode);
/**
* @}
*/

/** @addtogroup SSD1315_Exported_Functions
* @{
*/

/**
  * @brief  Register component IO bus.
  * @param  pObj Component object pointer.
  * @param  pIO  Component IO structure pointer.
  * @retval Component status.
  */
int32_t SSD1315_RegisterBusIO(SSD1315_Object_t *pObj, SSD1315_IO_t *pIO)
{
  int32_t ret;

  if(pObj == NULL)
  {
    ret = SSD1315_ERROR;
  }
  else
  {
    pObj->IO.Init           = pIO->Init;
    pObj->IO.DeInit         = pIO->DeInit;
    pObj->IO.WriteReg       = pIO->WriteReg;
    pObj->IO.ReadReg        = pIO->ReadReg;
    pObj->IO.GetTick        = pIO->GetTick;

    pObj->Ctx.ReadReg       = SSD1315_ReadRegWrap;
    pObj->Ctx.WriteReg      = SSD1315_WriteRegWrap;
    pObj->Ctx.handle    = pObj;
  }

  if (pObj->IO.Init != NULL)
  {
      ret = pObj->IO.Init();
  }
  else
  {
     ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Initialize the SSD1315 LCD Component.
  * @param  pObj Component object.
  * @param  ColorCoding RGB mode.
  * @param  Orientation Display orientation.
  * @retval Component status.
  */
int32_t SSD1315_Init(SSD1315_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;

  if((pObj == NULL) || (Orientation > SSD1315_ORIENTATION_LANDSCAPE))
  {
    ret = SSD1315_ERROR;
  }
  else
  {
    if (pObj->IsInitialized == 0)
    {
      pObj->IsInitialized = 1;
	  pObj->Orientation = Orientation;
      (void)SSD1315_IO_Delay(pObj, 100);
      /* Driving ability setting */
      data = SSD1315_READWRITE_CMD;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_CHARGE_PUMP_SETTING;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_MEMORY_ADRESS_MODE;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_LOWER_COLUMN_START_ADRESS;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_DISPLAY_START_LINE_1;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_REMAPPED_MODE;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_CONTRAST_CONTROL;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      data = SSD1315_DISPLAY_ON;
      ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
      ssd1315_Clear(SSD1315_COLOR_BLACK); 
      ret += ssd1315_write_reg(&pObj->Ctx, 1, PhysFrameBuffer,  SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
    }
    else
    {
      ret = SSD1315_ERROR;
    }
  }
  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  De-Initialize the ssd1315 LCD Component.
  * @param  pObj Component object.
  * @retval Component status.
  */
int32_t SSD1315_DeInit(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK;

  if(pObj->IsInitialized != 0U)
  {
    ret += SSD1315_DisplayOff(pObj);

    pObj->IsInitialized = 0;
  }

  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the SSD1315 ID.
  * @param  pObj Component object.
  * @param  Id Component ID.
  * @retval The component status.
  */
int32_t SSD1315_ReadID(SSD1315_Object_t *pObj, uint32_t *Id)
{
  /* Feature not supported */
  (void)pObj;
  (void)Id;
  return SSD1315_ERROR;
}

/**
  * @brief  Enables the Display.
  * @param  pObj Component object.
  * @retval The component status.
  */
int32_t SSD1315_DisplayOn(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;
  data = SSD1315_CHARGE_PUMP_SETTING;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_DISPLAY_ON;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  
  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Disables the Display.
  * @param  pObj Component object.
  * @retval The component status.
  */
int32_t SSD1315_DisplayOff(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;
  
  data = SSD1315_CHARGE_PUMP_SETTING;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_HIGHER_COLUMN_START_ADRESS_1;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_DISPLAY_OFF;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  
  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Set the display brightness.
  * @param  pObj Component object.
  * @param  Brightness display brightness to be set.
  * @retval Component status.
  */
int32_t SSD1315_SetBrightness(SSD1315_Object_t *pObj, uint32_t Brightness)
{
  /* Feature not supported */
  (void)pObj;
  (void)Brightness;
  return SSD1315_ERROR;
}

/**
  * @brief  Get the display brightness.
  * @param  pObj Component object.
  * @param  Brightness display brightness to be returned.
  * @retval Component status.
  */
int32_t SSD1315_GetBrightness(SSD1315_Object_t *pObj, uint32_t *Brightness)
{
  /* Feature not supported */
  (void)pObj;
  (void)Brightness;
  return SSD1315_ERROR;
}

/**
  * @brief  Set the Display Orientation.
  * @param  pObj Component object.
  * @param  Orientation SSSD1315_ORIENTATION_LANDSCAPE.
  * @retval The component status.
  */
int32_t SSD1315_SetOrientation(SSD1315_Object_t *pObj, uint32_t Orientation)
{
  /* Feature not supported */
  (void)pObj;
  (void)Orientation;
  return SSD1315_ERROR;
}

/**
  * @brief  Set the Display Orientation.
  * @param  pObj Component object.
  * @param  Orientation SSD1315_ORIENTATION_LANDSCAPE.
  * @retval The component status.
  */
int32_t SSD1315_GetOrientation(SSD1315_Object_t *pObj, uint32_t *Orientation)
{
  /* Feature not supported */
  (void)pObj;
  (void)Orientation;
  return SSD1315_ERROR;
}

/**
  * @brief  Set Cursor position.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @retval The component status.
  */
int32_t SSD1315_SetCursor(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos)
{
 /* Feature not supported */
 (void)pObj;
 (void)Xpos;
 (void)Ypos;
 return SSD1315_ERROR;
}

/**
  * @brief  Refresh Display.
  * @param  pObj Component object.
  * @retval The component status.
  */

int32_t SSD1315_Refresh(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK; 
  uint8_t data;

  data = SSD1315_DISPLAY_START_LINE_1;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_SET_COLUMN_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_DISPLAY_START_LINE_64;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_SET_PAGE_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  data = SSD1315_LOWER_COLUMN_START_ADRESS_15;
  ret += ssd1315_write_reg(&pObj->Ctx, 1,&data, 1);
  ret += ssd1315_write_reg(&pObj->Ctx, 1,PhysFrameBuffer, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}
/**
  * @brief  Displays a bitmap picture.
  * @param  pObj Component object.
  * @param  Xpos Bmp X position in the LCD.
  * @param  Ypos Bmp Y position in the LCD.
  * @param  pBmp Bmp picture address.
  * @retval The component status.
  */

int32_t SSD1315_DrawBitmap(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t  ret = SSD1315_OK;
  uint32_t index = 0, size = 0;
  uint32_t height = 0, width  = 0;
  uint32_t x = 0, y  = 0, y0 = 0;
  uint32_t XposBMP = 0, YposBMP  = 0;

  /* Read bitmap size */
  size = pBmp[2] + (pBmp[3] << 8) + (pBmp[4] << 16)  + (pBmp[5] << 24);

  /* Get bitmap data address offset */
  index = pBmp[10] + (pBmp[11] << 8) + (pBmp[12] << 16)  + (pBmp[13] << 24);

  /* Read bitmap width */
  width = pBmp[18] + (pBmp[19] << 8) + (pBmp[20] << 16)  + (pBmp[21] << 24);

  /* Read bitmap height */
  height = pBmp[22] + (pBmp[23] << 8) + (pBmp[24] << 16)  + (pBmp[25] << 24);

  /* Size conversion */
  size = (size - index)/2;

  /* Apply offset to bypass header */
  pBmp += index;

  /* if bitmap cover whole screen */
  if((Xpos == 0) && (Xpos == 0) & (size == (SSD1315_LCD_PIXEL_WIDTH * SSD1315_LCD_PIXEL_HEIGHT/8)))
  {
    memcpy(PhysFrameBuffer, pBmp, size);
  }
  else
  {
    x=Xpos+width;
    y=Ypos+height;
    y0 = Ypos;

    for(; Xpos < x; Xpos++, XposBMP++)
    {
      for(Ypos = y0, YposBMP = 0; Ypos < y; Ypos++, YposBMP++)
      {
        /* if bitmap and screen are aligned on a Page */
        if(((Ypos%8) == 0) && (y-Ypos >= 8) && ((YposBMP%8) == 0))
        {
          PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] = pBmp[XposBMP+((YposBMP/8)*width)];
          Ypos+=7;
          YposBMP+=7;
        }
        else
        {
          /* Draw bitmap pixel per pixel */
          if( (pBmp[XposBMP+((YposBMP/8)*width)]&(1<<(YposBMP%8))) != 0)
            {
              if (SSD1315_SetPixel(pObj, Xpos, Ypos, SSD1315_COLOR_WHITE) != SSD1315_OK)
              {
                ret = SSD1315_ERROR;
                break;
              }
            }
            else
            {
              if (SSD1315_SetPixel(pObj, Xpos, Ypos, SSD1315_COLOR_BLACK) != SSD1315_OK)
                {
                  ret = SSD1315_ERROR;
                  break;
                }
            }
        }
      }
    }
  }
  if(ret != SSD1315_OK)
   {
     ret = SSD1315_ERROR;
   }
  return ret;
}

/**
  * @brief  Shift and Displays a bitmap picture loaded in the internal Flash.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Xshift specifies number of pixel to shift on X position.
  * @param  Yshift specifies number of pixel to shift on Y position.
  * @param  pbmp Bmp picture address in the internal Flash.
  * @retval The component status.
  */
int32_t SSD1315_ShiftBitmap(SSD1315_Object_t *pObj,uint16_t Xpos, uint16_t Ypos, int16_t Xshift, int16_t Yshift, uint8_t *pbmp)
{
  int32_t  ret = SSD1315_OK;
  uint32_t index = 0, size = 0;
  uint32_t height = 0, width  = 0, original_width  = 0;
  uint32_t x = 0, y  = 0, y0 = 0;
  uint32_t XposBMP = 0, YposBMP  = 0, original_YposBMP = 0;
  
  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  
  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;
  original_width = width;
  if( Xshift>=0)
  {
    Xpos = Xpos + Xshift;
    width = width - Xshift;
  }
  else
  {
    width = width + Xshift;
    XposBMP = -Xshift;
  }
  
  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;
  if( Yshift>=0)
  {
    height = height - Yshift;
    Ypos = Ypos + Yshift;
  }
  else
  {
    height = height + Yshift;
    YposBMP = -Yshift;
  }
  original_YposBMP = YposBMP;
  
  /* Size conversion */
  size = (size - index)/2;
  size = size - ((Xshift*height/8)+(Yshift*width/8 ));
  
  /* Apply offset to bypass header */
  pbmp += index;
  
  /* if bitmap cover whole screen */
  if((Xpos == 0) && (Xpos == 0) & (size == (SSD1315_LCD_PIXEL_WIDTH * SSD1315_LCD_PIXEL_HEIGHT/8)))
  {
    memcpy(PhysFrameBuffer, pbmp, size);
  }
  else
  {
    x=Xpos+width;
    y=Ypos+height;
    y0 = Ypos;
    
    for(; Xpos < x; Xpos++, XposBMP++)
    {
      for(Ypos = y0, YposBMP = original_YposBMP; Ypos < y; Ypos++, YposBMP++)
      {
        /* if bitmap and screen are aligned on a Page */
        if(((Ypos%8) == 0) && (y-Ypos >= 8) && ((YposBMP%8) == 0))
        {
          PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] = pbmp[XposBMP+((YposBMP/8)*original_width)];
          Ypos+=7;
          YposBMP+=7;
        }
        else
        {
          /* Draw bitmap pixel per pixel */
          if( (pbmp[XposBMP+((YposBMP/8)*original_width)]&(1<<(YposBMP%8))) != 0)
          {
            if (SSD1315_SetPixel(pObj, Xpos, Ypos, SSD1315_COLOR_WHITE) != SSD1315_OK)
            {
              ret = SSD1315_ERROR;
              break;
            }
          }
          else
          {
            if (SSD1315_SetPixel(pObj, Xpos, Ypos, SSD1315_COLOR_BLACK) != SSD1315_OK)
            {
              ret = SSD1315_ERROR;
              break;
            }
          }
        }
      }
    }
  }
  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Fill RGB Rectangle.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  pData Pointer to the character data.
  * @param  Width Rectangle width.
  * @param  Height Rectangle height.
  * @retval The component status.
  */
int32_t SSD1315_FillRGBRect(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
  int32_t  ret = SSD1315_OK;
  uint32_t i;
  uint32_t color, j;
  for(i = 0; i < Height; i++)
  {
    for(j = 0; j < Width; j++)
    {
      color = *pData | (*(pData + 1) << 8) | (*(pData + 2) << 16) | (*(pData + 3) << 24);
      if(SSD1315_SetPixel (pObj, Xpos + j, Ypos + i, color)!= SSD1315_OK)
      {
        ret = SSD1315_ERROR;
      }
      pData += 4;
    }
  }

  return ret;
}


/**
  * @brief  Draw horizontal line.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Length specifies the Line length.
  * @param  Color Specifies the RGB color.
  * @retval The component status.
  */
int32_t SSD1315_DrawHLine(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t  ret = SSD1315_OK;
  uint32_t i = 0;

  /* Sent a complete horizontal line */
  for (i = Xpos; i < (Xpos+Length); i++)
  {
    SSD1315_SetPixel(pObj,i, Ypos, Color);
  }
  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Draw vertical line.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Length specifies the Line length.
  * @param  Color Specifies the RGB color.
  * @retval The component status.
  */
int32_t SSD1315_DrawVLine(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t  ret = SSD1315_OK;
  uint32_t i = 0;
  
  for (i = Ypos; i < (Ypos+Length); i++)
  {
    SSD1315_SetPixel(pObj,Xpos, i, Color);
  }
  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Fill rectangle.
  * @param  pObj Component object.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Width Rectangle width.
  * @param  Height Rectangle height.
  * @param  Color Draw color.
  * @retval Component status.
  */
int32_t SSD1315_FillRect(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  int32_t ret = SSD1315_OK;
  uint32_t i;

  for(i = 0U; i < Height; i++)
  {
    if (SSD1315_DrawHLine(pObj, Xpos, (i + Ypos), Width, Color) != SSD1315_OK)
    {
      ret = SSD1315_ERROR;
      break;
    }
  }

  return ret;
}

/**
  * @brief  Write pixel.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Color the RGB pixel color.
  * @retval The component status.
  */
int32_t SSD1315_SetPixel(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  int32_t  ret = SSD1315_OK;
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  /* Set color */
  if (Color == SSD1315_COLOR_WHITE)
  {
    PhysFrameBuffer[Xpos + (Ypos / 8) * SSD1315_LCD_PIXEL_WIDTH] |= 1 << (Ypos % 8);
  }
  else
  {
    PhysFrameBuffer[Xpos + (Ypos / 8) * SSD1315_LCD_PIXEL_WIDTH] &= ~(1 << (Ypos % 8));
  }
  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }

  return ret;
}

/**
  * @brief  Read pixel.
  * @param  pObj Component object.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Color the LCD pixel color.
  * @retval The component status.
  */
int32_t SSD1315_GetPixel(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
   int32_t  ret = SSD1315_OK;
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  if ((Xpos >= SSD1315_LCD_PIXEL_WIDTH) || (Ypos >= SSD1315_LCD_PIXEL_HEIGHT)) 
  {
    *Color = 0;
  }
  else
  {
    *Color = PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] & (1 << Ypos%8);
    if (*Color != 0)
    {
      *Color = 1;
    }
    else
    {
      *Color = 0;
    }
  }
  
  return ret;
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  pObj Component object.
  * @param  The Lcd Pixel Width.
  * @retval The component status.
  */
int32_t SSD1315_GetXSize(SSD1315_Object_t *pObj, uint32_t *XSize)
{
  int32_t  ret = SSD1315_OK;

  if (pObj->Orientation == SSD1315_ORIENTATION_LANDSCAPE)
  {
    *XSize = 128;
  }
  else
  {
    ret = SSD1315_ERROR;
  }

  return ret;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  pObj Component object.
  * @param  The Lcd Pixel Height.
  * @retval The component status.
  */
int32_t SSD1315_GetYSize(SSD1315_Object_t *pObj, uint32_t *YSize)
{
  int32_t  ret = SSD1315_OK;

  if (pObj->Orientation == SSD1315_ORIENTATION_LANDSCAPE)
  {
    *YSize = 64;
  }
  else
  {
    ret = SSD1315_ERROR;
  }

  return ret;
}

/** @defgroup ST7735_Private_Functions  Private Functions
  * @{
  */

/**
  * @brief  Set Page position.
  * @param  pObj Component object.
  * @param  Page specifies the Page position (0-7).
  * @retval The component status.
  */
int32_t SSD1315_SetPage(SSD1315_Object_t *pObj, uint16_t Page)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;

  /* Set Page position  */
  data = (SSD1315_SET_PAGE_START_ADRESS | Page);
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Set Column position.
  * @param  pObj Component object.
  * @param  Column specifies the Column position (0-127).
  * @retval The component status.
  */
int32_t SSD1315_SetColumn(SSD1315_Object_t *pObj, uint16_t Column)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;
  /* Set Column position  */

  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = (SSD1315_LOWER_COLUMN_START_ADRESS | Column);
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_DISPLAY_START_LINE_32;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Scrolling Display Page.
  * @param  pObj Component object.
  * @param  ScrollMode SSD1315_SCROLL_RIGHT or SSD1315_SCROLL_LEFT
  * @param  StartPage Start page  for scrolling:
            @arg  0..7
  * @param  EndPage End page for scrolling:
            This must be larger or equal to StartLine
            @arg  0..7
  * @param  Frequency SSD1315_SCROLL_FREQ_2FRAMES to SSD1315_SCROLL_FREQ_256FRAMES
  * @retval The component status.
  */
int32_t SSD1315_ScrollingSetup(SSD1315_Object_t *pObj, uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;

  /* Scrolling setup sequence */
  data = ScrollMode;                                     /* Right/Left Horizontal Scroll */
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = StartPage;                                      /* start page address*/
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = Frequency;                                      /* Frequency*/
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = EndPage;                                        /* End page address*/
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;           
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);
  data = SSD1315_CONTRAST_CONTROL_2;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Start Display Scrolling.
  * @param  pObj Component object.
  * @retval The component status.
  */
int32_t SSD1315_ScrollingStart(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;

  /* Start scrolling sequence */
  data = SSD1315_ACTIVATE_SCROLL;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Stop Display Scrolling.
  * @param  pObj Component object.
  * @retval The component status.
  */
int32_t SSD1315_ScrollingStop(SSD1315_Object_t *pObj)
{
  int32_t ret = SSD1315_OK;
  uint8_t data;

  /* Stop scrolling  sequence */
  data = SSD1315_DESACTIVATE_SCROLL;
  ret += ssd1315_write_reg(&pObj->Ctx, 1, &data, 1);

  if (ret != SSD1315_OK)
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

/**
  * @brief  Read register wrapped function.
  * @param  handle Component object handle.
  * @param  Reg The target register address to read.
  * @param  pData The target register value to be red.
  * @param  Length Buffer size to be red.
  * @retval error status.
  */
static int32_t SSD1315_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  SSD1315_Object_t *pObj = (SSD1315_Object_t *)handle;

  return pObj->IO.ReadReg(Reg, pData, Length);
}

/**
  * @brief  Write register wrapped function.
  * @param  handle Component object handle.
  * @param  Reg The target register address to write.
  * @param  pData The target register value to be written.
  * @param  Length Buffer size to be written.
  * @retval error status.
  */
static int32_t SSD1315_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  SSD1315_Object_t *pObj = (SSD1315_Object_t *)handle;

  return pObj->IO.WriteReg(Reg, pData, Length);
}

/**
  * @brief  Clear Display screen.
  * @param  ColorCode the color use to clear the screen (SSD1315_COLOR_WHITE or SSD1315_COLOR_BLACK).
  * @retval None
  */
static void ssd1315_Clear(uint16_t ColorCode)
{
  /* Check color */
  if (ColorCode == SSD1315_COLOR_WHITE) 
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_WHITE, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
  }
  else
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_BLACK, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
  }
}

/**
  * @brief  SSD1315 delay.
  * @param  Delay Delay in ms.
  * @retval Component error status.
  */
static int32_t SSD1315_IO_Delay(SSD1315_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
  return SSD1315_OK;
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
