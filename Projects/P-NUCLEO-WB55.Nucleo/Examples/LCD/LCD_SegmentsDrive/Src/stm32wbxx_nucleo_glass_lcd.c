/**
  ******************************************************************************
  * @file    stm32wbxx_nucleo_glass_lcd.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the
  *          LCD Glass driver for the P-NUCLEO-WB55 board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_nucleo_glass_lcd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBXX_NUCLEO
  * @{
  */

/** @defgroup STM32WBXX_NUCLEO_GLASS_LCD P-NUCLEO-WB55 GLASS LCD
  * @brief This file includes the LCD Glass driver for LCD Module of
  *        P-NUCLEO-WB55 board.
  * @{
  */

/* Private constants ---------------------------------------------------------*/

/** @defgroup STM32WBXX_NUCLEO_GLASS_LCD_Private_Constants Private Constants
  * @{
  */
#define ASCII_CHAR_0                  0x30  /* 0 */
#define ASCII_CHAR_AT_SYMBOL          0x40  /* @ */
#define ASCII_CHAR_LEFT_OPEN_BRACKET  0x5B  /* [ */
#define ASCII_CHAR_APOSTROPHE         0x60  /* ` */
#define ASCII_CHAR_LEFT_OPEN_BRACE    0x7B  /* ( */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32WBXX_NUCLEO_GLASS_LCD_Private_Variables Private Variables
  * @{
  */

/**
  @verbatim
================================================================================
                              GLASS LCD MAPPING
================================================================================
LCD allows to display information on six 14-segment digits and 4 bars:

  1       2       3       4       5       6
-----   -----   -----   -----   -----   -----
|\|/| o |\|/| o |\|/| o |\|/| o |\|/|   |\|/|   BAR3
-- --   -- --   -- --   -- --   -- --   -- --   BAR2
|/|\| o |/|\| o |/|\| o |/|\| o |/|\|   |/|\|   BAR1
----- * ----- * ----- * ----- * -----   -----   BAR0

LCD segment mapping:
--------------------
  -----A-----        _
  |\   |   /|   COL |_|
  F H  J  K B
  |  \ | /  |        _
  --G-- --M--   COL |_|
  |  / | \  |
  E Q  P  N C
  |/   |   \|        _
  -----D-----   DP  |_|

 An LCD character coding is based on the following matrix:
COM           0   1   2     3
SEG(n)      { E , D , P ,   N   }
SEG(n+1)    { M , C , COL , DP  }
SEG(23-n-1) { B , A , K ,   J   }
SEG(23-n)   { G , F , Q ,   H   }
with n positive odd number.

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

  @endverbatim
*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26] =
{
  /* A      B      C      D      E      F      G      H      I  */
  0xFE00, 0x6714, 0x1D00, 0x4714, 0x9D00, 0x9C00, 0x3F00, 0xFA00, 0x0014,
  /* J      K      L      M      N      O      P      Q      R  */
  0x5300, 0x9841, 0x1900, 0x5A48, 0x5A09, 0x5F00, 0xFC00, 0x5F01, 0xFC01,
  /* S      T      U      V      W      X      Y      Z  */
  0xAF00, 0x0414, 0x5b00, 0x18C0, 0x5A81, 0x00C9, 0x0058, 0x05C0
};

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10] =
{
  /* 0      1      2      3      4      5      6      7      8      9  */
  0x5F00, 0x4200, 0xF500, 0x6700, 0xEa00, 0xAF00, 0xBF00, 0x04600, 0xFF00, 0xEF00
};

uint32_t Digit[4];     /* Digit frame buffer */

/**
  * @}
  */

/** @defgroup STM32WBXX_NUCLEO_LCD_Private_Functions Private Functions
  * @{
  */
static void Convert(uint8_t *Char, Point_Typedef Point, DoublePoint_Typedef Colon);
static void WriteChar(LCD_HandleTypeDef *hlcd, uint8_t *ch, Point_Typedef Point, DoublePoint_Typedef Colon, uint32_t Position);

/**
  * @}
  */

/** @addtogroup STM32WBXX_NUCLEO_LCD_Exported_Functions
  * @{
  */

/**
  * @brief  Configure the LCD Blink mode and Blink frequency.
  * @param  BlinkMode: specifies the LCD blink mode.
  *   This parameter can be one of the following values:
  *     @arg LCD_BLINKMODE_OFF:           Blink disabled
  *     @arg LCD_BLINKMODE_SEG0_COM0:     Blink enabled on SEG[0], COM[0] (1 pixel)
  *     @arg LCD_BLINKMODE_SEG0_ALLCOM:   Blink enabled on SEG[0], all COM (up to 8
  *                                       pixels according to the programmed duty)
  *     @arg LCD_BLINKMODE_ALLSEG_ALLCOM: Blink enabled on all SEG and all COM
  *                                       (all pixels)
  * @param  BlinkFrequency: specifies the LCD blink frequency.
  *     @arg LCD_BLINKFREQUENCY_DIV8:    The Blink frequency = fLcd/8
  *     @arg LCD_BLINKFREQUENCY_DIV16:   The Blink frequency = fLcd/16
  *     @arg LCD_BLINKFREQUENCY_DIV32:   The Blink frequency = fLcd/32
  *     @arg LCD_BLINKFREQUENCY_DIV64:   The Blink frequency = fLcd/64
  *     @arg LCD_BLINKFREQUENCY_DIV128:  The Blink frequency = fLcd/128
  *     @arg LCD_BLINKFREQUENCY_DIV256:  The Blink frequency = fLcd/256
  *     @arg LCD_BLINKFREQUENCY_DIV512:  The Blink frequency = fLcd/512
  *     @arg LCD_BLINKFREQUENCY_DIV1024: The Blink frequency = fLcd/1024
  * @retval None
  */
void BSP_LCD_GLASS_BlinkConfig(LCD_HandleTypeDef *hlcd, uint32_t BlinkMode, uint32_t BlinkFrequency)
{
  __HAL_LCD_BLINK_CONFIG(hlcd, BlinkMode, BlinkFrequency);
}

/**
  * @brief Configure the bar level on LCD by writing bar value in LCD frame buffer.
  * @param BarLevel: specifies the LCD GLASS Battery Level.
  *     This parameter can be one of the following values:
  *     @arg BATTERYLEVEL_OFF: LCD GLASS Battery Empty
  *     @arg BATTERYLEVEL_1_4: LCD GLASS Battery 1/4 Full
  *     @arg BATTERYLEVEL_1_2: LCD GLASS Battery 1/2 Full
  *     @arg BATTERYLEVEL_3_4: LCD GLASS Battery 3/4 Full
  *     @arg BATTERYLEVEL_FULL: LCD GLASS Battery Full
  * @retval None
  */
void BSP_LCD_GLASS_BarLevelConfig(LCD_HandleTypeDef *hlcd, uint8_t BarLevel)
{
  switch (BarLevel)
  {
      /* BATTERYLEVEL_OFF */
    case BATTERYLEVEL_OFF:
      /* Set BAR0 & BAR2 off */
      HAL_LCD_Write(hlcd, LCD_BAR0_2_COM, ~(LCD_BAR0_SEG | LCD_BAR2_SEG), 0);
      /* Set BAR1 & BAR3 off */
      HAL_LCD_Write(hlcd, LCD_BAR1_3_COM, ~(LCD_BAR1_SEG | LCD_BAR3_SEG), 0);
      break;

      /* BARLEVEL 1/4 */
    case BATTERYLEVEL_1_4:
      /* Set BAR0 on & BAR2 off */
      HAL_LCD_Write(hlcd, LCD_BAR0_2_COM, ~(LCD_BAR0_SEG | LCD_BAR2_SEG), LCD_BAR0_SEG);
      /* Set BAR1 & BAR3 off */
      HAL_LCD_Write(hlcd, LCD_BAR1_3_COM, ~(LCD_BAR1_SEG | LCD_BAR3_SEG), 0);
      break;

      /* BARLEVEL 1/2 */
    case BATTERYLEVEL_1_2:
      /* Set BAR0 on & BAR2 off */
      HAL_LCD_Write(hlcd, LCD_BAR0_2_COM, ~(LCD_BAR0_SEG | LCD_BAR2_SEG), LCD_BAR0_SEG);
      /* Set BAR1 on & BAR3 off */
      HAL_LCD_Write(hlcd, LCD_BAR1_3_COM, ~(LCD_BAR1_SEG | LCD_BAR3_SEG), LCD_BAR1_SEG);
      break;

      /* Battery Level 3/4 */
    case BATTERYLEVEL_3_4:
      /* Set BAR0 & BAR2 on */
      HAL_LCD_Write(hlcd, LCD_BAR0_2_COM, ~(LCD_BAR0_SEG | LCD_BAR2_SEG), (LCD_BAR0_SEG | LCD_BAR2_SEG));
      /* Set BAR1 on & BAR3 off */
      HAL_LCD_Write(hlcd, LCD_BAR1_3_COM, ~(LCD_BAR1_SEG | LCD_BAR3_SEG), LCD_BAR1_SEG);
      break;

      /* BATTERYLEVEL_FULL */
    case BATTERYLEVEL_FULL:
      /* Set BAR0 & BAR2 on */
      HAL_LCD_Write(hlcd, LCD_BAR0_2_COM, ~(LCD_BAR0_SEG | LCD_BAR2_SEG), (LCD_BAR0_SEG | LCD_BAR2_SEG));
      /* Set BAR1 on & BAR3 on */
      HAL_LCD_Write(hlcd, LCD_BAR1_3_COM, ~(LCD_BAR1_SEG | LCD_BAR3_SEG), (LCD_BAR1_SEG | LCD_BAR3_SEG));
      break;

    default:
      break;
  }

  /* Update the LCD display */
  HAL_LCD_UpdateDisplayRequest(hlcd);
}

/**
  * @brief  Write a character in the LCD RAM buffer.
  * @param  ch: The character to display.
  * @param  Point: A point to add in front of char.
  *          This parameter can be one of the following values:
  *              @arg POINT_OFF: No point to add in front of char.
  *              @arg POINT_ON: Add a point in front of char.
  * @param  Colon: Flag indicating if a colon character has to be added in front
  *                     of displayed character.
  *          This parameter can be one of the following values:
  *              @arg DOUBLEPOINT_OFF: No colon to add in back of char.
  *              @arg DOUBLEPOINT_ON: Add an colon in back of char.
  * @param  Position: Position in the LCD of the character to write.
  *                   This parameter can be any value in range [0:6].
  * @retval None
  * @note   Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void BSP_LCD_GLASS_DisplayChar(LCD_HandleTypeDef *hlcd, uint8_t *ch, Point_Typedef Point, DoublePoint_Typedef Colon, uint32_t Position)
{
  WriteChar(hlcd, ch, Point, Colon, Position);

  /* Update the LCD display */
  HAL_LCD_UpdateDisplayRequest(hlcd);
}

/**
  * @brief  Write a character string in the LCD RAM buffer.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void BSP_LCD_GLASS_DisplayString(LCD_HandleTypeDef *hlcd, uint8_t *ptr)
{
  DigitPosition_Typedef position = LCD_DIGIT_POSITION_1;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (position <= LCD_DIGIT_POSITION_6))
  {
    /* Write one character on LCD */
    WriteChar(hlcd, ptr, POINT_OFF, DOUBLEPOINT_OFF, position);

    /* Point on the next character */
    ptr++;

    /* Increment the character counter */
    position++;
  }
  /* Update the LCD display */
  HAL_LCD_UpdateDisplayRequest(hlcd);
}

/**
  * @brief  Clear the whole LCD RAM buffer.
  * @retval None
  */
void BSP_LCD_GLASS_Clear(LCD_HandleTypeDef *hlcd)
{
  HAL_LCD_Clear(hlcd);
}

/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Specifies the speed of the scroll, low value gives
  *         higher speed
  * @retval None
  * @note   Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void BSP_LCD_GLASS_ScrollSentence(LCD_HandleTypeDef *hlcd, uint8_t *ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
  uint8_t repetition = 0, nbrchar = 0, sizestr = 0;
  uint8_t *ptr1;
  uint8_t str[6] = "";

  if (ptr == 0)
  {
    return;
  }

  /* To calculate end of string */
  for (ptr1 = ptr, sizestr = 0; *ptr1 != 0; sizestr++, ptr1++);

  ptr1 = ptr;

  BSP_LCD_GLASS_DisplayString(hlcd, str);
  HAL_Delay(ScrollSpeed);

  /* To shift the string for scrolling display*/
  for (repetition = 0; repetition < nScroll; repetition++)
  {
    for (nbrchar = 0; nbrchar < sizestr; nbrchar++)
    {
      *(str) = * (ptr1 + ((nbrchar + 1) % sizestr));
      *(str + 1) = * (ptr1 + ((nbrchar + 2) % sizestr));
      *(str + 2) = * (ptr1 + ((nbrchar + 3) % sizestr));
      *(str + 3) = * (ptr1 + ((nbrchar + 4) % sizestr));
      *(str + 4) = * (ptr1 + ((nbrchar + 5) % sizestr));
      *(str + 5) = * (ptr1 + ((nbrchar + 6) % sizestr));
      BSP_LCD_GLASS_Clear(hlcd);
      BSP_LCD_GLASS_DisplayString(hlcd, str);

      HAL_Delay(ScrollSpeed);
    }
  }
}

/**
  * @}
  */

/** @addtogroup STM32WBXX_NUCLEO_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Convert an ascii char to the a LCD digit.
  * @param  Char: a char to display.
  * @param  Point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  Colon : flag indicating if a colon character has to be added in front
  *         of displayed character.
  *         This parameter can be: DOUBLEPOINT_OFF or DOUBLEPOINT_ON.
  * @retval None
  */
static void Convert(uint8_t *Char, Point_Typedef Point, DoublePoint_Typedef Colon)
{
  uint16_t ch = 0 ;
  uint8_t loop = 0, index = 0;

  switch (*Char)
  {
    case ' ' :
      ch = 0x00;
      break;

    case '*':
      ch = C_STAR;
      break;

    case '(' :
      ch = C_OPENPARMAP;
      break;

    case ')' :
      ch = C_CLOSEPARMAP;
      break;

    case 'd' :
      ch = C_DMAP;
      break;

    case 'm' :
      ch = C_MMAP;
      break;

    case 'n' :
      ch = C_NMAP;
      break;

    case 'µ' :
      ch = C_UMAP;
      break;

    case '-' :
      ch = C_MINUS;
      break;

    case '+' :
      ch = C_PLUS;
      break;

    case '/' :
      ch = C_SLATCH;
      break;

    case '°' :
      ch = C_PERCENT_1;
      break;
    case '%' :
      ch = C_PERCENT_2;
      break;
    case 255 :
      ch = C_FULL;
      break ;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ch = NumberMap[*Char - ASCII_CHAR_0];
      break;

    default:
      /* The character Char is one letter in upper case*/
      if ((*Char < ASCII_CHAR_LEFT_OPEN_BRACKET) && (*Char > ASCII_CHAR_AT_SYMBOL))
      {
        ch = CapLetterMap[*Char - 'A'];
      }
      /* The character Char is one letter in lower case*/
      if ((*Char < ASCII_CHAR_LEFT_OPEN_BRACE) && (*Char > ASCII_CHAR_APOSTROPHE))
      {
        ch = CapLetterMap[*Char - 'a'];
      }
      break;
  }

  /* Set the digital point can be displayed if the point is on */
  if (Point == POINT_ON)
  {
    ch |= 0x0002;
  }

  /* Set the "COL" segment in the character that can be displayed if the colon is on */
  if (Colon == DOUBLEPOINT_ON)
  {
    ch |= 0x0020;
  }

  for (loop = 12, index = 0 ; index < 4; loop -= 4, index++)
  {
    Digit[index] = (ch >> loop) & 0x0f; /*To isolate the less significant digit */
  }
}

/**
  * @brief  Write a character in the LCD frame buffer.
  * @param  ch: the character to display.
  * @param  Point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  Colon: flag indicating if a colon character has to be added in front
  *         of displayed character.
  *         This parameter can be: DOUBLEPOINT_OFF or DOUBLEPOINT_ON.
  * @param  Position: position in the LCD of the character to write [0:6]
  * @retval None
  */
static void WriteChar(LCD_HandleTypeDef *hlcd, uint8_t *ch, Point_Typedef Point, DoublePoint_Typedef Colon, uint32_t Position)
{
  uint32_t data = 0x00;
  /* To convert displayed character in segment in array digit */
  Convert(ch, (Point_Typedef)Point, (DoublePoint_Typedef)Colon);

  switch (Position)
  {
      /* Position 1 on LCD (Digit1)*/
    case LCD_DIGIT_POSITION_1:
      data = ((Digit[0] & 0x1) << LCD_SEG0_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG1_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG22_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG23_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT1_COM0, LCD_DIGIT1_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG0_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG1_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG22_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG23_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT1_COM1, LCD_DIGIT1_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG0_SHIFT) | (((Digit[2] & 0x2) >> 1) << LCD_SEG1_SHIFT)
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG22_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG23_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT1_COM2, LCD_DIGIT1_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG0_SHIFT) | (((Digit[3] & 0x2) >> 1) << LCD_SEG1_SHIFT)
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG22_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG23_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT1_COM3, LCD_DIGIT1_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

      /* Position 2 on LCD (Digit2)*/
    case LCD_DIGIT_POSITION_2:
      data = ((Digit[0] & 0x1) << LCD_SEG2_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG3_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG20_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG21_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT2_COM0, LCD_DIGIT2_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG2_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG3_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG20_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG21_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT2_COM1, LCD_DIGIT2_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG2_SHIFT) | (((Digit[2] & 0x2) >> 1) << LCD_SEG3_SHIFT)
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG20_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG21_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT2_COM2, LCD_DIGIT2_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG2_SHIFT) | (((Digit[3] & 0x2) >> 1) << LCD_SEG3_SHIFT)
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG20_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG21_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT2_COM3, LCD_DIGIT2_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

      /* Position 3 on LCD (Digit3)*/
    case LCD_DIGIT_POSITION_3:
      data = ((Digit[0] & 0x1) << LCD_SEG4_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG5_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG18_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG19_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT3_COM0, LCD_DIGIT3_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG4_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG5_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG18_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG19_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT3_COM1, LCD_DIGIT3_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG4_SHIFT) | (((Digit[2] & 0x2) >> 1) << LCD_SEG5_SHIFT)
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG18_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG19_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT3_COM2, LCD_DIGIT3_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG4_SHIFT) | (((Digit[3] & 0x2) >> 1) << LCD_SEG5_SHIFT)
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG18_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG19_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT3_COM3, LCD_DIGIT3_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

      /* Position 4 on LCD (Digit4)*/
    case LCD_DIGIT_POSITION_4:
      data = ((Digit[0] & 0x1) << LCD_SEG6_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG7_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG16_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG17_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT4_COM0, LCD_DIGIT4_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG6_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG7_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG16_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG17_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT4_COM1, LCD_DIGIT4_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG6_SHIFT) | (((Digit[2] & 0x2) >> 1) << LCD_SEG7_SHIFT)
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG16_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG17_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT4_COM2, LCD_DIGIT4_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG6_SHIFT) | (((Digit[3] & 0x2) >> 1) << LCD_SEG7_SHIFT)
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG16_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG17_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT4_COM3, LCD_DIGIT4_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

      /* Position 5 on LCD (Digit5)*/
    case LCD_DIGIT_POSITION_5:
      data = ((Digit[0] & 0x1) << LCD_SEG8_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG9_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG14_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG15_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT5_COM0, LCD_DIGIT5_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG8_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG9_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG14_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG15_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT5_COM1, LCD_DIGIT5_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG8_SHIFT) /* | (((Digit[2] & 0x2) >> 1) << LCD_SEG9_SHIFT) (No double point on Digit 5 )*/
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG14_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG15_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT5_COM2, LCD_DIGIT5_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG8_SHIFT) /* | (((Digit[3] & 0x2) >> 1) << LCD_SEG9_SHIFT) (No double point on Digit 5 )*/
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG14_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG15_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT5_COM3, LCD_DIGIT5_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

      /* Position 6 on LCD (Digit6)*/
    case LCD_DIGIT_POSITION_6:
      data = ((Digit[0] & 0x1) << LCD_SEG10_SHIFT) | (((Digit[0] & 0x2) >> 1) << LCD_SEG11_SHIFT)
             | (((Digit[0] & 0x4) >> 2) << LCD_SEG12_SHIFT) | (((Digit[0] & 0x8) >> 3) << LCD_SEG13_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT6_COM0, LCD_DIGIT6_COM0_SEG_MASK, data); /* 1G 1B 1M 1E */

      data = ((Digit[1] & 0x1) << LCD_SEG10_SHIFT) | (((Digit[1] & 0x2) >> 1) << LCD_SEG11_SHIFT)
             | (((Digit[1] & 0x4) >> 2) << LCD_SEG12_SHIFT) | (((Digit[1] & 0x8) >> 3) << LCD_SEG13_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT6_COM1, LCD_DIGIT6_COM1_SEG_MASK, data) ; /* 1F 1A 1C 1D  */

      data = ((Digit[2] & 0x1) << LCD_SEG10_SHIFT) /* | (((Digit[2] & 0x2) >> 1) << LCD_SEG11_SHIFT) (No double point on Digit 6 )*/
             | (((Digit[2] & 0x4) >> 2) << LCD_SEG12_SHIFT) | (((Digit[2] & 0x8) >> 3) << LCD_SEG13_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT6_COM2, LCD_DIGIT6_COM2_SEG_MASK, data) ; /* 1Q 1K 1Col 1P  */

      data = ((Digit[3] & 0x1) << LCD_SEG10_SHIFT) /* | (((Digit[3] & 0x2) >> 1) << LCD_SEG11_SHIFT) (No double point on Digit 6 )*/
             | (((Digit[3] & 0x4) >> 2) << LCD_SEG12_SHIFT) | (((Digit[3] & 0x8) >> 3) << LCD_SEG13_SHIFT);
      HAL_LCD_Write(hlcd, LCD_DIGIT6_COM3, LCD_DIGIT6_COM3_SEG_MASK, data) ; /* 1H 1J 1DP 1N  */
      break;

    default:
      break;
  }
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
