/**
  ******************************************************************************
  * @file    lcd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the lcd supported by the 
  *          STM32WB55MM-DK board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM                4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t ButtonState;
static uint8_t LCD_Feature = 0;
/* Private function prototypes -----------------------------------------------*/
static void LCD_Show_Feature(uint8_t feature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature);

  while (1)
  {
    if(++LCD_Feature < LCD_FEATURES_NUM)
    {
      LCD_Show_Feature (LCD_Feature);
    }
    else
    {
      return;
    }
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_LCD_ScrollingStop(0);
      return;
    }
  }
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{70, 40}, {120, 50}, {60, 60}};
  Point Points2[5];
  uint32_t x_size, y_size;
  
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  Points2[0].X = 10;
  Points2[0].Y = 10;
  Points2[1].X = 30;
  Points2[1].Y = 30;
  Points2[2].X = 50;
  Points2[2].Y = 50;
  Points2[3].X = 80;
  Points2[3].Y = 20;
  Points2[4].X = 30;
  Points2[4].Y = 60;
  
  UTIL_LCD_SetFuncDriver(&LCD_Driver); /* SetFunc before setting device */
  UTIL_LCD_SetDevice(0);            /* SetDevice after funcDriver is set */
  BSP_LCD_DisplayOn(0);
  
  UTIL_LCD_SetFont(&Font12);
  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
  UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(0);
  
  switch (feature)
  {
  case 0:
    /* Text Feature */
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Text features", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(2000);
    
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"Left", LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    UTIL_LCD_DisplayStringAt(0, 20, (uint8_t *)"Center", CENTER_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)"Right", RIGHT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    UTIL_LCD_SetFont(&Font16);
    UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"Font16", LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(60, 50, (uint8_t *)"Font12", LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    UTIL_LCD_SetFont(&Font8);
    UTIL_LCD_DisplayStringAt(100, 58, (uint8_t *)"Font8", LEFT_MODE);
    BSP_LCD_Refresh(0);
    UTIL_LCD_SetFont(&Font12);
    HAL_Delay(2000);
    break;
    
  case 1:
    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Draw features", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 30, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(2000);
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_FillPolygon(Points, 3, SSD1315_COLOR_WHITE);
    UTIL_LCD_DrawPolygon(Points2, 5, SSD1315_COLOR_WHITE);
    UTIL_LCD_DrawRect(0, 0, x_size , y_size - 1, SSD1315_COLOR_WHITE);
    UTIL_LCD_FillRect(82, 10, 20 , 20, SSD1315_COLOR_WHITE);
    UTIL_LCD_DrawCircle(45, 10,  2, SSD1315_COLOR_WHITE);
    UTIL_LCD_FillCircle(55, 10,  3, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(3000);
    break;
    
  case 2:
    /* Draw Bitmap */
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Draw Bitmap", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 30, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(2000);
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    BSP_LCD_DrawBitmap(0, 0, 0, (uint8_t *)stlogo);
    BSP_LCD_Refresh(0);
    HAL_Delay(3000);
    break;
    
  case 3:
    /* Scrolling feature */
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Scroll feature", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(3000);
    UTIL_LCD_DisplayStringAt(0, 12, (uint8_t *)"scrolling Left", CENTER_MODE);
    BSP_LCD_Refresh(0);
    BSP_LCD_ScrollingStart(0);
    UTIL_LCD_DisplayStringAtLine(3, (uint8_t*)"<---");
    UTIL_LCD_DisplayStringAtLine(4, (uint8_t*)"<---");
    UTIL_LCD_DisplayStringAtLine(2, (uint8_t*)"<---");
    BSP_LCD_Refresh(0);
    BSP_LCD_ScrollingSetup(0,SSD1315_SCROLL_LEFT, 3, 7, SSD1315_SCROLL_FREQ_25FRAMES);
    BSP_LCD_Refresh(0);
    HAL_Delay(3000);
    
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Scroll feature", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 12, (uint8_t *)"scrolling right", CENTER_MODE);
    BSP_LCD_Refresh(0);
    BSP_LCD_ScrollingStart(0);
    UTIL_LCD_DisplayStringAtLine(3, (uint8_t*)"--->");
    UTIL_LCD_DisplayStringAtLine(4, (uint8_t*)"--->");
    UTIL_LCD_DisplayStringAtLine(2, (uint8_t*)"--->");
    BSP_LCD_Refresh(0);
    BSP_LCD_ScrollingSetup(0,SSD1315_SCROLL_RIGHT, 3, 7, SSD1315_SCROLL_FREQ_25FRAMES);
    BSP_LCD_Refresh(0);
    HAL_Delay(3000);
    BSP_LCD_ScrollingStop(0);
    break;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */
 
