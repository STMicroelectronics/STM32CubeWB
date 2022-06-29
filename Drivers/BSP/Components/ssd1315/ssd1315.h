/**
  ******************************************************************************
  * @file    ssd1315.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the ssd1315.c
  *          driver.
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
#ifndef SSD1315_H
#define SSD1315_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ssd1315_reg.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup SSD1315
  * @{
  */

/** @defgroup SSD1315_Exported_Types SSD1315 Exported Types
  * @{
  */
typedef int32_t (*SSD1315_Init_Func)     (void);
typedef int32_t (*SSD1315_DeInit_Func)   (void);
typedef int32_t (*SSD1315_GetTick_Func)  (void);
typedef int32_t (*SSD1315_Delay_Func)    (uint32_t);
typedef int32_t (*SSD1315_WriteReg_Func)(uint16_t, uint8_t*, uint16_t);
typedef int32_t (*SSD1315_ReadReg_Func) (uint16_t, uint8_t*, uint16_t);

typedef struct
{
  SSD1315_Init_Func             Init;
  SSD1315_DeInit_Func           DeInit;
  SSD1315_WriteReg_Func         WriteReg;
  SSD1315_ReadReg_Func          ReadReg;
  SSD1315_GetTick_Func          GetTick;
} SSD1315_IO_t;


typedef struct
{
  SSD1315_IO_t         IO;
  ssd1315_ctx_t        Ctx;
  uint8_t              IsInitialized;
  uint32_t             Orientation;
} SSD1315_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(SSD1315_Object_t*, uint32_t, uint32_t);
  int32_t (*DeInit           )(SSD1315_Object_t*);
  int32_t (*ReadID           )(SSD1315_Object_t*, uint32_t*);
  int32_t (*DisplayOn        )(SSD1315_Object_t*);
  int32_t (*DisplayOff       )(SSD1315_Object_t*);
  int32_t (*SetBrightness    )(SSD1315_Object_t*, uint32_t);
  int32_t (*GetBrightness    )(SSD1315_Object_t*, uint32_t*);
  int32_t (*SetOrientation   )(SSD1315_Object_t*, uint32_t);
  int32_t (*GetOrientation   )(SSD1315_Object_t*, uint32_t*);
  int32_t (*Refresh          )(SSD1315_Object_t*);
  int32_t (*SetPage          )(SSD1315_Object_t*, uint16_t);
  int32_t (*SetColumn        )(SSD1315_Object_t*, uint16_t);
  int32_t (*ScrollingSetup   )(SSD1315_Object_t*, uint16_t, uint16_t, uint16_t, uint16_t);
  int32_t (*ScrollingStart   )(SSD1315_Object_t*);
  int32_t (*ScrollingStop    )(SSD1315_Object_t*);

  /* Drawing functions*/
  int32_t ( *SetCursor       ) (SSD1315_Object_t*, uint32_t, uint32_t);
  int32_t ( *DrawBitmap      ) (SSD1315_Object_t*, uint32_t, uint32_t, uint8_t *);
  int32_t ( *ShiftBitmap     ) (SSD1315_Object_t*,uint16_t, uint16_t, int16_t, int16_t, uint8_t*);
  int32_t ( *FillRGBRect     ) (SSD1315_Object_t*, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t ( *DrawHLine       ) (SSD1315_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *DrawVLine       ) (SSD1315_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *FillRect        ) (SSD1315_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetPixel        ) (SSD1315_Object_t*, uint32_t, uint32_t, uint32_t*);
  int32_t ( *SetPixel        ) (SSD1315_Object_t*, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetXSize        ) (SSD1315_Object_t*, uint32_t *);
  int32_t ( *GetYSize        ) (SSD1315_Object_t*, uint32_t *);
}SSD1315_Drv_t;

/**
  * @}
  */

/** @defgroup SSD1315_Exported_Constants  SSD1315 Exported Constants
  * @{
  */

/**
  * @brief  SSD1315 return code
  */
#define SSD1315_OK                (0)
#define SSD1315_ERROR             (-1)

/**
  * @brief  SSD1315 Size
  */
#define  SSD1315_LCD_PIXEL_WIDTH    ((uint16_t)128)
#define  SSD1315_LCD_PIXEL_HEIGHT   ((uint16_t)64)

#define  SSD1315_LCD_COLUMN_NUMBER  ((uint16_t)128)
#define  SSD1315_LCD_PAGE_NUMBER    ((uint16_t)8)

/**
  *  @brief LCD_Orientation
  *  Possible values of Display Orientation
  */
#define SSD1315_ORIENTATION_LANDSCAPE        0x00U /* Landscape orientation choice of LCD screen              */

/**
  *  @brief LCD_Format
  *  Possible values of Display format
  */
#define SSD1315_FORMAT_DEFAULT               0x00U

/**
  * @}
  */

/** @defgroup SSD1315_Exported_Functions SSD1315 Exported Functions
  * @{
  */
int32_t SSD1315_RegisterBusIO(SSD1315_Object_t *pObj, SSD1315_IO_t *pIO);
int32_t SSD1315_Init(SSD1315_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t SSD1315_DeInit(SSD1315_Object_t *pObj);
int32_t SSD1315_ReadID(SSD1315_Object_t *pObj, uint32_t *Id);
int32_t SSD1315_DisplayOn(SSD1315_Object_t *pObj);
int32_t SSD1315_DisplayOff(SSD1315_Object_t *pObj);
int32_t SSD1315_SetBrightness(SSD1315_Object_t *pObj, uint32_t Brightness);
int32_t SSD1315_GetBrightness(SSD1315_Object_t *pObj, uint32_t *Brightness);
int32_t SSD1315_SetOrientation(SSD1315_Object_t *pObj, uint32_t Orientation);
int32_t SSD1315_GetOrientation(SSD1315_Object_t *pObj, uint32_t *Orientation);
int32_t SSD1315_Refresh(SSD1315_Object_t *pObj);

int32_t SSD1315_SetPage(SSD1315_Object_t *pObj, uint16_t Page);
int32_t SSD1315_SetColumn(SSD1315_Object_t *pObj, uint16_t Column);
int32_t SSD1315_ScrollingSetup(SSD1315_Object_t *pObj, uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency);
int32_t SSD1315_ScrollingStart(SSD1315_Object_t *pObj);
int32_t SSD1315_ScrollingStop(SSD1315_Object_t *pObj);

int32_t SSD1315_SetCursor(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t SSD1315_DrawBitmap(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t SSD1315_ShiftBitmap(SSD1315_Object_t *pObj,uint16_t Xpos, uint16_t Ypos, int16_t Xshift, int16_t Yshift, uint8_t *pbmp);
int32_t SSD1315_FillRGBRect(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
int32_t SSD1315_DrawHLine(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t SSD1315_DrawVLine(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t SSD1315_DrawLine(SSD1315_Object_t *pObj, uint32_t X1pos, uint32_t Y1pos, uint32_t X2pos, uint32_t Y2pos, uint32_t Color);
int32_t SSD1315_FillRect(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
int32_t SSD1315_SetPixel(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t SSD1315_GetPixel(SSD1315_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t SSD1315_GetXSize(SSD1315_Object_t *pObj, uint32_t *XSize);
int32_t SSD1315_GetYSize(SSD1315_Object_t *pObj, uint32_t *YSize);

/**
  * @}
  */

/** @addtogroup SSD1315_Exported_Variables SSD1315 Exported Variables
 * @{
 */
/* LCD driver structure */
extern SSD1315_Drv_t   SSD1315_Driver;
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

#endif /* SSD1315_H */
