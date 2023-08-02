/**
  ******************************************************************************
  * @file    DispTools.c
  * @author  MCD Application Team
  * @brief   This file contains functions for creating MMI on a terminal.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */



/* *****************************************************************************
 * *****    Include Files declaration.                                     *****
 * ****************************************************************************/
#include "ble_common.h"
#include "stm_queue.h"
#include "DispTools.h"
#include "app_conf.h"


/* *****************************************************************************
 * *****    Define & Macro declaration.                                    *****
 * ****************************************************************************/

#define RX_BUFFER_SIZE                                                        64


/* -- Definition of Lines in DisplayTitle() function -- */
#define LINE_START                      0       /* Case of Test Name is in Bold. */
#define LINE_TESTNAME                   1
#define LINE_COMPANY                    2
#define LINE_CHIP_DEMUX                 3
#define LINE_SOFT_PROTO                 4
#define LINE_END                        5

#define MAX_X                          80
#define MAX_Y                          24

/* -- Macro for Menu() function. -- */
#define skip_next_inactive_item(ItemNumb)    \
  /* skip not active item */                 \
  while ( (menu[ItemNumb].Type == TITLE)     \
       || (menu[ItemNumb].Type == VAL16)     \
       || (menu[ItemNumb].Type == VAL32)     \
       || (menu[ItemNumb].Type == SVAL32)    \
       || (menu[ItemNumb].Type == STRTAB)    \
       || (menu[ItemNumb].Type == NULL)      \
       || (*menu[ItemNumb].Item == '\t' ) )  \
  {                                          \
    ItemNumb++;                              \
    if(*menu[ItemNumb].Item == '\0' )        \
      ItemNumb=0;   /* Roll over */          \
  }

/* -- Macro for Menu() function. -- */
#define skip_previous_inactive_item(ItemNumb)        \
  /* skip not active item  */                        \
  while ( (menu[ItemNumb].Type == TITLE)             \
               || (menu[ItemNumb].Type == VAL16)     \
               || (menu[ItemNumb].Type == VAL32)     \
               || (menu[ItemNumb].Type == SVAL32)    \
               || (menu[ItemNumb].Type == STRTAB)    \
               || (menu[ItemNumb].Type == NULL)      \
               || (*menu[ItemNumb].Item == '\t' ) )  \
  {                                                  \
    if (ItemNumb == 0)                               \
      ItemNumb=k;   /* max item */                   \
    else                                             \
      ItemNumb--;                                    \
  }

/* -- Macro for Menu() function. -- */
#define skip_next_inactive_item2(ItemNumb, MenuCtrlEntry)  \
  /* skip not active item */                               \
  while (MenuCtrlEntry.ItemsPos[ItemNumb].Selectable == 0) \
  {                                                        \
    ItemNumb++;                                            \
    if(MenuCtrlEntry.nbEntries == ItemNumb )               \
      ItemNumb=0;   /* Roll over */                        \
  }
        

#define skip_previous_inactive_item2(ItemNumb, MenuCtrlEntry) \
while (MenuCtrlEntry.ItemsPos[ItemNumb].Selectable == 0)      \
{                                                             \
  if (ItemNumb == 0)                                          \
   ItemNumb = MenuCtrlEntry.nbEntries;   /* max item */       \
  else                                                        \
   ItemNumb--;                                                \
}


typedef struct
{
  uint8_t x;
  uint8_t y;
} Pos_t;


typedef struct
{
  Pos_t WinPosUpperLeft ;
  Pos_t WinPosDownRigth;
} WinPosSize_t;

typedef struct
{
  uint8_t Field_x;
  uint8_t Cursor_x;
  uint8_t NbLines;
} ColInfo_t;

typedef struct
{
  Pos_t ItemPos;
  uint8_t ColIndex;
  uint8_t Selectable;
} ItemPos_t;

typedef struct
{
  uint8_t created;            /* indicate if the menu has been created */
  STRUCTMENU_T *MenuItems;    /* point on the Menu description provide by Caller at creation time */
  ItemPos_t ItemsPos[MAXITEMNB];
  ColInfo_t ColInfo[MAXCOLNB+2];
  WinPosSize_t WinPos;
  uint8_t NbCol;
  uint8_t ItemSelectable;
  uint8_t nbItems;
  uint8_t nbEntries;
} MenuCtrlData_t;


/* *****************************************************************************
 * *****    Global Variables                                               *****
 * ****************************************************************************/
uint8_t BusyFlag = FREE_FLAG;
#ifdef CFG_CONSOLE_MENU
static uint8_t InputCharFromUart; 
static uint8_t RxQueueBuffer[RX_BUFFER_SIZE];  
#endif
static queue_t RxQueue;
static MenuCtrlData_t MenuCtrlData[MAX_MENU];
static char str[133], strDisplay[133];


static uint16_t NbDigit (int32_t * ptTabNber, uint16_t * NbElements);
static void GetNumberBox_Range (char *szFmt, STRUCTCAPTUREVAL_T * stRangeVal,
       uint8_t iX, uint8_t iY);
static uint16_t SelectBox (char *szMenu[], uint8_t iLineNumb, uint8_t iX,
        uint8_t iY, uint8_t mode);
static uint16_t SelectNumberBox (int32_t * ptTabNber, uint8_t iLineNumb,
        uint8_t iX, uint8_t iY, uint8_t mode);
static uint16_t CheckVaribleType (char *fmtStr);
static int8_t AToHex (char *szBuffer, uint32_t * plNumber);
static int8_t AToDec (const char *szString, int32_t *piNumber);
uint16_t StringLength (const char *szString);
static uint8_t IsSpace (int8_t cCar);
static uint8_t IsDigit (int8_t cCar);
void ClearLines (uint8_t iStart_Line, uint8_t iNumber_Of_Line);
static uint16_t DisplaySTMVersion (uint8_t iX, uint8_t iY);
int8_t ReadPcStr (uint8_t x, uint8_t y, char buffer[], int8_t nbDigit,
      int8_t pad, int8_t deflt, int8_t mode);
void PosCur (uint8_t iX, uint8_t iY);
void PrintPcCrt (uint8_t x, uint8_t y, char *szFormat, ...);
void PrintCrt (char *szFormat, ...);
char UartReadChar(uint8_t blocking);
#ifdef CFG_CONSOLE_MENU
static void ConsoleMenu_UART_RxCpltCallback( void );
void SaveCursorPos(void);
void RestoreCursorPos(void);
void DummyFunc(__IO uint32_t * lpUart_Base, int8_t * pcFormat, ...);
static void _DisplayMenu(int8_t MenuHandle, uint8_t flag);
#endif


/**
  * @brief  This function initialize the console
  * @param  None
  * @retval None
  */
void ConsoleMenu_Init(void)
{
#ifdef CFG_CONSOLE_MENU
  HW_UART_Init(CFG_CONSOLE_MENU);

  CircularQueue_Init(&RxQueue, RxQueueBuffer, RX_BUFFER_SIZE, 1, CIRCULAR_QUEUE_NO_WRAP_FLAG);
  
  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, ConsoleMenu_UART_RxCpltCallback);

  return;
#endif
}


#ifdef CFG_CONSOLE_MENU
/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  */
void ConsoleMenu_UART_RxCpltCallback( void )
{
  CircularQueue_Add(&RxQueue, &InputCharFromUart,1,1);

  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, ConsoleMenu_UART_RxCpltCallback);

  return;
 }
#endif


uint16_t UartTransmitStr(char *StrToDisp)
{
#ifdef CFG_CONSOLE_MENU
  uint16_t jj;

  jj = 0;
  while (StrToDisp[jj] != 0)
  {
    HW_UART_Transmit(CFG_CONSOLE_MENU, (uint8_t*)&StrToDisp[jj++], 1, 0xFFFFFFFF);
  }
  return jj;
#else
  return 0;
#endif
}


/**
 * *****************************************************************************
 * @fn    char UartReadChar(uint8_t blocking)
 * @author  -
 * @brief   Wait until the receipt of a character from an Uart or JTAG,
 *          then convert it into ASCII and return the character
 * @param   blocking : blocking mode
 * @return  Return the character received from the serial link or the JTAG.
 * *****************************************************************************
 */
char UartReadChar(uint8_t blocking)
{
  uint16_t size;
  uint8_t *c;
  static uint8_t  ESC_Seq = 0;
  uint8_t leave = 0;
  char retc = (char)EOF;
  static uint16_t ESC_Timeout = 0;
    
  while (!leave)
  {
    c = CircularQueue_Remove(&RxQueue, &size);
    if (!c) 
    { 
     if (ESC_Seq == 1)
     {
       if (--ESC_Timeout == 0) 
       {
         retc = ESC;
         ESC_Seq = 0;
         break; /* Assume ESC have been hit */ 
       }
     }
     if (!blocking) leave=1;
    }
    else if (c) /* I got a char */
    {
      retc = *c;
      if ((*c == ESC) && (ESC_Seq == 0))  /* ESC sequence */
      {
        ESC_Timeout = 5000;  /* Maybe better to arm a timer */
        ESC_Seq = 1;          /* If ESC AND Esc_seq ==> error case, here we just restart ESC sequence */
        retc = 0;
      }
      else if (ESC_Seq == 1)  /* check second char in ESC sequence */
      {
        if (*c == '[')  /* True ESC sequence */
        {
          /* wait for next char in ESC seq */
          ESC_Seq++;   
          retc = 0;
          ESC_Timeout = 0;
        }
        else          /* Unexpected char following ESC, so ESC hit [no ESC sequence ] */
        {
          leave = 1;
          ESC_Seq = 0;
          retc = ESC;
          ESC_Timeout = 0;
        }
      }
      else if (ESC_Seq == 2)
      {
        leave = 1;
        ESC_Seq = 0;
        retc = *c | 0x80;  
      }
      else
      {
        leave = 1;
      }
    }  
  }
  
  return retc;       
}


/**
 *******************************************************************************
 * @fn    int8_t ReadPcStr(int32_t x, int32_t y, int8_t buffer[],int8_t nbDigit, 
 *        int8_t pad, int8_t deflt, int8_t mode)
 * @author  BINI Jean-Claude
 * @brief Take the characters from the 'UART1_BASE' or the keypad, translate 
 * them into ASCII, and place them into the buffer. The characters will be 
 * accepted until the ENTER character is received. LEFT, RIGHT, BACK SPACE, 
 * DEL functions are available.
 * When nbDigit characters are received, the function does not accept additional 
 * characters.
 *
 * @param   x, y : Column & Line position.
 * @param   buffer : Address of the buffer where data will be placed
 * @param   nbDigit : Number max of characters accepted
 * @param   pad : Is the padding string. If pad = '.' and nbdigit = 6 the 
 *          function will display : ......
 * @param   deflt : If TRUE the function displays the default value contained 
 *                  into the string buffer.
 *                  If FALSE the string buffer is considered as empty
 * @param   mode :  If mode = INS means that the character capture uses the 
 *          insert mode.
 *          If mode = OVERSTK means that the function uses the overstrike mode.
 *
 * @return  Return the number of character read
 *******************************************************************************
 */
int8_t ReadPcStr(uint8_t x, uint8_t y, char buffer[], int8_t nbDigit,
                 int8_t pad, int8_t deflt, int8_t mode)
{
  uint8_t idx, nbChar, i;
  char data;
  int8_t valid;
  static char padding[2];

  padding[0] = pad;
  padding[1] = 0;
  PosCur(x, y);
  /*---- If a default value is required it is printed and nbChar 
         is initiated ---*/
  if (deflt == TRUE)
  {
    PrintCrt("%s", buffer);
    nbChar = (uint8_t) StringLength(buffer);
    for (i = nbChar; i < nbDigit; i++)
    {
      /* print padding pattern */
      PrintCrt(padding);

    }
  }
  else
  {
    for (i = 0; i < nbDigit; i++)
    {
      /* print padding pattern */
      PrintCrt(padding);

    }
    nbChar = 0;
  }
  PosCur(x, y);
  idx = 0;
  valid = FALSE;
  while (valid == FALSE)
  {
    data = UartReadChar(TRUE);
    switch (data)
    {
      case ENTER:
        valid = TRUE;
        break;

      case DEL:
        if (idx < nbChar)
        {
          for (i = idx; i < nbChar; i++)
          {
            buffer[i] = buffer[i + 1];
          }
          PosCur(x, y);
          for (i = 0; i < nbDigit; i++)
            PrintCrt(padding);

          PrintPcCrt(x, y, "%s", buffer);
          PosCur(x + idx, y);
          nbChar--;
        }
        break;

      case BKSP:
        if (idx != 0)
        {
          for (i = idx; i <= nbChar; i++)
          {
            buffer[i - 1] = buffer[i];
          }
          idx--;
          PosCur(x, y);
          for (i = 0; i < nbDigit; i++)
          {
            PrintCrt(padding);
          }
          PrintPcCrt(x, y, "%s", buffer);
          PosCur(x + idx, y);
          nbChar--;
        }
        break;

      case LEFT:
        if (idx != 0)
        {
          idx--;
          PosCur(x + idx, y);
        }
        break;

      case RIGHT:
        if (idx < nbChar)
        {
          idx++;
          PosCur(x + idx, y);
        }
        break;

      default:
        if (mode == INS)
        {
          if ((data > 0x1f) && (data < 0x80))
          {
            if (nbChar < nbDigit)
            {
              i = nbChar;
              do
              {
                buffer[i + 1] = buffer[i];
              }
              while (i-- != idx);

              nbChar++;
              buffer[idx] = data;
              PrintPcCrt(x, y, "%s", buffer);
              idx++;
              PosCur(x + idx, y);
            }
          }
        }
        else
        {
          if ((data > 0x1f) && (data < 0x80))
          {
            if (idx < nbDigit)
            {
              buffer[idx] = data;
              buffer[idx + 1] = 0;
              for (i = idx + 1; i <= nbDigit; i++)
              {
                PrintCrt(padding);
              }
              if (idx == nbChar)
                nbChar++;

              PrintPcCrt(x, y, "%s", buffer);
              idx++;
              PosCur(x + idx, y);
            }
          }
        }
        break;
    }
  }
  return nbChar;
}


/*
 * ****************************************************************
 *      MANAGE TERMINAL (DISPLAY & CURSOR)  FUNCTIONS             *
 *                                                                *
 *****************************************************************/

/**
 *******************************************************************************
 * @fn      void ClearLines(uint16_t iStart_Line, uint16_t iNumber_Of_Line)
 * @author  -
 * @brief Clear the number of line specified on terminal.
 *
 * @param USARTx : Is the base address of the serial interface.
 * @param iStart_Line : Location of the first line to clear
 * @param iNumber_Of_Line : Number of line to clear
 *
 * @return: none.
 *******************************************************************************
 **/
void ClearLines(uint8_t iStart_Line, uint8_t iNumber_Of_Line)
{
  uint16_t iCpt;

  for (iCpt = 0; iCpt < iNumber_Of_Line; iCpt++)
  {
    PosCur(1, iStart_Line + iCpt);
    UartTransmitStr("\033[K");
  }
}

/**
 *******************************************************************************
 * @fn      void ClearScreen()
 * @author  -
 * @brief Clear the full screen on terminal
 * @param None
 * @return  none.
 *******************************************************************************
 **/
void ClearScreen()
{
  UartTransmitStr("\033[2J");
  PosCur(1, 1);
}


/**
 *******************************************************************************
 * @fn      void PosCur(uint16_t iX, uint16_t iY)
 * @author  -
 * @brief Locate cursor at the position X & Y
 * @param iX, iY : Column & Line cursor position.
 * @return none.
 *******************************************************************************
 **/
void PosCur(uint8_t iX, uint8_t iY)
{
  char szCtrl[30] = "\33[01;01H";

  szCtrl[3] = (int8_t) ((iY % 10) + '0');
  szCtrl[2] = (int8_t) ((iY / 10) + '0');
  szCtrl[6] = (int8_t) ((iX % 10) + '0');
  szCtrl[5] = (int8_t) ((iX / 10) + '0');

  UartTransmitStr(szCtrl);
}

/**
 *******************************************************************************
 * @fn      void SaveCursorPos()
 * @author  -
 * @brief Save the current position of the cursor.
 * @param None
 * @return  none.
 *******************************************************************************
 **/
void SaveCursorPos(void)
{
  PrintCrt("%c7", 27);
}


/**
 *******************************************************************************
 * @fn      void RestoreCursorPos()
 * @author  -
 * @brief Restore the last value of the cursor saved with SaveCursorPos() function.
 * @param  none
 * @return  none.
 *******************************************************************************
 **/
void RestoreCursorPos(void)
{
  PrintCrt("%c8", 27);
}


/*
 * ****************************************************************
 *          STRING MANIPULATIONS  FUNCTIONS                       *
 *                                                                *
 *****************************************************************/

/**
 *******************************************************************************
 * @fn      uint8_t IsDigit(int8_t cCar)
 * @author  -
 * @brief Test if Character cCar is an ASCII value between '0' and '9' including.
 * @param   cCar : Character to test.
 * @return  TRUE if the ASCII value of cCar equals '0'or'1'or...'9', else FALSE.
 *******************************************************************************
 **/
uint8_t IsDigit(int8_t cCar)
{
  if ((cCar < '0') || (cCar > '9'))
  {
    return (FALSE);
  }
  else
  {
    return (TRUE);
  }
}


/**
 *******************************************************************************
 * @fn      uint8_t IsSpace(int8_t cCar)
 * @author  -
 * @brief Test if Character cCar is the ASCII value ' ' (Space).
 * @param   cCar : Character to test.
 * @return  TRUE if the ASCII value of cCar equals ' ', else FALSE.
 *******************************************************************************
 **/
uint8_t IsSpace(int8_t cCar)
{
  if (cCar == ' ')
  {
    return (TRUE);
  }

  return (FALSE);
}


/**
 *******************************************************************************
 * @fn    uint16_t StringLength(const int8_t * szString)
 * @author  -
 * @brief   Return the length of a string.
 * @param   szString : String pointer
 * @return  Length of the string without the '\0' char
 *******************************************************************************
 */
uint16_t StringLength(const char *szString)
{
  uint16_t iLength;

  iLength = 0;
  while (*szString++ != '\0')
  {
    if (++iLength == 0xFFFF)
    {
      break;
    }
  }

  return (iLength);
}


/**
 *******************************************************************************
 * @fn      boolean AToDec(const int8_t *szString, int32_t *piNumber)
 * @author  -
 * @brief   Conversion of an ASCII number to an integer.
 *
 * @param   szString : Is the address of a string containing the DEC number to 
 *                     convert
 * @param   piNumber : Address where the converted number can be put
 * @return  TRUE when the number is correctly converted, else FALSE.
 *******************************************************************************
 */
static int8_t AToDec(const char *szString, int32_t * piNumber)
{
  int8_t *pcChar = (int8_t *) szString;
  int32_t iValue = 0, iSign;

  while (IsSpace(*pcChar))
  {
    pcChar++;
  };

  if ((*pcChar) == '-')
  {
    iSign = -1;
    pcChar++;
  }
  else
  {
    if ((*pcChar) == '+')
    {
      pcChar++;
    }
    iSign = 1;
  };

  while ((*pcChar != 0) && (*pcChar != ' '))    /* space(s) at the end */
  {
    if (IsDigit(*pcChar))
    {
      iValue = 10 * iValue + (*pcChar - '0');
      pcChar++;
    }
    else
    {
      return (FALSE);
    }
  }

  *piNumber = iSign * iValue;
  return (TRUE);
}


/**
 *******************************************************************************
 * @fn      boolean AToHex(int8_t * szBuffer, uint32_t *plNumber)
 * @author  -
 * @brief   Conversion of an ASCII number to a long.
 *
 * @param   szBuffer : Is the address of a string containing the Hexa number to 
 *                     convert
 * @param   plNumber : Address where the converted number can be put
 * @return  TRUE when the number is correctly converted, else FALSE.
 *******************************************************************************
 */
int8_t AToHex(char *szBuffer, uint32_t * plNumber)
{
  uint8_t cCar;
  uint32_t lNumber;

  while (IsSpace(*szBuffer))
  {
    szBuffer++;
  }

  lNumber = 0;
  while ((*szBuffer != 0) && (*szBuffer != ' '))        /* Space(s) at the end */
  {
    cCar = *szBuffer;

    if (IsDigit(cCar))
    {
      cCar = (cCar - '0');
    }
    else
    {
      if ((cCar >= 'a') && (cCar <= 'f'))
      {
        cCar = (cCar - 'a' + 10);
      }
      else
      {
        if ((cCar >= 'A') && (cCar <= 'F'))
        {
          cCar = (cCar - 'A' + 10);
        }
        else
        {
          return (FALSE);
        }
      }
    }
    lNumber = (lNumber << 4) | cCar;
    szBuffer++;
  }

  *plNumber = lNumber;
  return (TRUE);
}


/*
 * ****************************************************************
 *          STRING DISPLAY FUNCTIONS                              *
 *                                                                *
 *****************************************************************/

/**
 *******************************************************************************
 * @fn      void DummyFunc(__IO uint32_t *lpUart_Base, int8_t *pcFormat, ...)
 * @author  -
 * @brief This function replace the PrintCrt() when no display is required.
 *
 * @param   lpUart_Base : is the base address of the serial interface
 * @param   pcFormat : Represent the string including the format of the data
 *             to be displayed. %d, %x, %s. %c, %u (uint32_t) are supported.
 *
 * @return  TRUE of Test OK, else FALSE.
 *******************************************************************************
 **/
void DummyFunc(__IO uint32_t * lpUart_Base, int8_t * pcFormat, ...)
{
}


/**
 *******************************************************************************
 * @fn      void PrintCrt(int8_t* szFormat, ...)
 * @author  -
 * @brief   This function print messages into the Display.
 * @param   szFormat : Represent the string including the format of the data
 *             to be displayed. %d, %x, %s. %c, %u (uint32_t) are supported.
 * @return  none
 *******************************************************************************
 */
void PrintCrt(char *szFormat, ...)
{
  uint8_t i;
  uint32_t ulval;
  int8_t *sval;
  char fmtDetect, *pt;
  int32_t slval;
  va_list ptArg;

  /* -------   Execute the print if BusyFlag = FREE_FLAG  -------- */
  if (BusyFlag == FREE_FLAG)
  {
    BusyFlag = BUSY_FLAG;
    /* -------  This is for multi-thread */
    fmtDetect = FALSE;
    va_start(ptArg, szFormat);
    i = 0;
    for (pt = szFormat; *pt != 0; pt++)
    {
      str[i++] = *pt;
      if (fmtDetect == FALSE)
      {
        if (*pt == '%')
        {
          fmtDetect = TRUE;
        }
      }
      else
      {
        switch (*pt)
        {
        case 'd':
        case 'c':
          fmtDetect = FALSE;
          str[i++] = 0;
          slval = va_arg(ptArg, long int);

          sprintf((char *) strDisplay, (char *) str, slval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;

        case 'X':
        case 'x':
        case 'u':
          fmtDetect = FALSE;
          str[i++] = 0;
          ulval = va_arg(ptArg, unsigned long int);

          sprintf((char *) strDisplay, (char *) str, ulval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;

        case 's':
          fmtDetect = FALSE;
          str[i++] = 0;
          sval = va_arg(ptArg, int8_t *);
          sprintf((char *) strDisplay, (char *) str, sval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;
        }
      }
    }
    str[i++] = 0;
    UartTransmitStr(str);
    va_end(ptArg);
    BusyFlag = FREE_FLAG;
  }
}


/**
 *******************************************************************************
 * @fn      void PrintPcCrt(int32_t x, int32_t y, int8_t* szFormat, ...)
 * @author  -
 * @brief   This function print messages into the Display at the x,y position.
 * @param x, y   : column & line position.
 * @param   szFormat : Represent the string including the format of the data
 *             to be displayed. %d, %x, %s. %c, %u (unsigned int32_t) are supported.
 * @return  none
 *******************************************************************************
 */
void PrintPcCrt(uint8_t x, uint8_t y, char *szFormat, ...)
{
  uint8_t i;
  uint32_t ulval;
  int8_t *sval;
  char fmtDetect, *pt;
  int32_t slval;
  va_list ptArg;

  /* -------   Execute the print if BusyFlag = FREE_FLAG  -------- */
  if (BusyFlag == FREE_FLAG)
  {
    BusyFlag = BUSY_FLAG;
    /* -------  This is for multi-thread */
    PosCur(x, y);
    fmtDetect = FALSE;
    va_start(ptArg, szFormat);
    i = 0;

    for (pt = szFormat; *pt != 0; pt++)
    {
      str[i++] = *pt;
      if (fmtDetect == FALSE)
      {
        if (*pt == '%')
        {
          fmtDetect = TRUE;
        }
      }
      else
      {
        switch (*pt)
        {
        case 'd':
        case 'c':
          fmtDetect = FALSE;
          str[i++] = 0;
          slval = va_arg(ptArg, long int);

          sprintf((char *) strDisplay, (char *) str, slval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;

        case 'X':
        case 'x':
        case 'u':
          fmtDetect = FALSE;
          str[i++] = 0;
          ulval = va_arg(ptArg, unsigned long int);

          sprintf((char *) strDisplay, (char *) str, ulval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;

        case 's':
          fmtDetect = FALSE;
          str[i++] = 0;
          sval = va_arg(ptArg, int8_t *);
          sprintf((char *) strDisplay, (char *) str, sval);
          i = 0;
          UartTransmitStr(strDisplay);
          break;
        }
      }
    }
    str[i++] = 0;
    UartTransmitStr(str);
    va_end(ptArg);
    BusyFlag = FREE_FLAG;       /* allow another printf to be executed */
  }
}


/**
 *******************************************************************************
* @fn      void DisplayTitle(int32_t iY, char * szTitle, char * szVersion, 
*                            char * szFclk)
* @author -
* @brief  Display the title, the version of the OneC on the uart pointed by 
*         USARTx, at the line indicated by iY. The title will take 7 lines.
*
* @param  iY        : line position when the display will start
* @param  szTitle   : Points to the string containing the title to display
* @param  szVersion : Points to the string containing the version number. 
*                     If the string is empty (""), the version number will not 
*                     be displayed.
* @param  szFclk    : ARM clk actual frequency value
*
* @return None
 *******************************************************************************
**/
void DisplayTitle(uint8_t iY, char *szTitle, char *szVersion)
{
  uint32_t DecVal, IntVal;

  IntVal = SystemCoreClock / 1000000;
  DecVal = ((SystemCoreClock - (IntVal * 1000000)) + 500) / 1000;
  PrintPcCrt(1, iY,
             "________________________________________________________________________________");

  PrintPcCrt(24, iY + 2, "%c[\033[34;01m%s %c[0m", 27, szTitle, 27);
  PrintPcCrt(29, iY + 3, "%c[\033[34;01mST Microelectronics %c[0m", 27, 27);

  if (*szVersion != 0)
  {
    PrintPcCrt(5, iY + 5, "Soft version : %s", szVersion);
    PrintPcCrt(50, iY + 5, "Freq. ArmClk : %d.%d MHz", IntVal, DecVal);
  }

  PrintPcCrt(1, iY + 6,
             "________________________________________________________________________________");
  PrintCrt("\r\n");
}


/**
 *******************************************************************************
 * @fn      void DisplaySubTitle(int y, int8_t *title, int8_t dispVer)
 * @author  -
 * @brief Display the title, the version of the OneC on the uart pointed by 
 *        USARTx, at the line indicated by y. The title will take 2 lines.
 *
 * @param   y   : line position when the display will start.
 * @param   title : Points to the string containing the title to display
 * @param   dispVer : When TRUE, the OneC version is display, else only the 
 *                    title will be displayed.
 *
 * @return  None
 *******************************************************************************
 */
void DisplaySubTitle(int y, char *title, char dispVer)
{
  int dx;
  uint16_t lg;

  lg = StringLength(title);
  if (lg > 38)
    dx = 1;
  else
    dx = (40 - lg) / 2;
  PrintPcCrt(dx, y, "%c#6%s", 27, title);

  if (dispVer == TRUE)
    DisplaySTMVersion(25, y + 1);
}


/**
 *******************************************************************************
 * @fn      uint16_t DisplaySTMVersion(USART_TypeDef* uint16_t iX, uint16_t iY)
 * @author  -
 * @brief Display the Version of the Chip on the terminal pointed by
      at the position indicated by x, y.
 * @param USARTx : Is the base address of the serial interface .
 * @param iX, iY : column & line position.
 * @return  Value read in the Chip Version Register
 *******************************************************************************
**/
uint16_t DisplaySTMVersion(uint8_t iX, uint8_t iY)
{
  PrintPcCrt(iX, iY, "STM Version : STM32Fxxx");

  return (00);
}


/*
 * ****************************************************************
 *          MANAGE DISPLAY CFG_MENU FUNCTIONS                         *
 *                                                                *
 *****************************************************************/

/**
 *******************************************************************************
 * @fn      uint16_t DisplayMenu(int8_t * menu[], uint16_t lineNumb, int32_t x, 
 *                               int32_t y)
 * @author  -
 * @brief Displays the menu pointed by the parameters menu[], and select the 
 *      line indicated by 'line' with UP, DOWN, and VALID keys (issued from the 
 *      UART or for the keypad), it is possible to select the desired option.
 *      key : 'UP'      Select the previous available line
 *      key : 'DOWN'    Select the next available line
 *      key : 'ENTER'   Validate the selected line
 *      key : 'ESC'     Exit from the menu
 *
 * @param   USARTx : Pointer indicating the address base of the uart to be used
 * @param   menu :  Pointer of a table containing the menu to be displayed.
 *          THE LAST LINE OF THE TABLE MUST BE : ""
 * @param   lineNumb : Line number to display.
 * @param   x, y : Define the Column & Line where will be displayed the upper 
 *                 left corner of the menu.
 *
 * @return  Return the line menu selected (lineNumb).
            If lineNumb = 0xFE ---> Home
            else               ---> Line menu selected
 *******************************************************************************
 **/
uint16_t DisplayMenu(char *menu[], uint8_t lineNumb, uint8_t x, uint8_t y)
{
  uint16_t valid, max, i, j, end, maxLength, length;
  char c;

  valid = FALSE;
  max = 1;
  maxLength = 0;
  /* Look for the max number of menu lines to control exit function */
  for (i = 0; i < 23; i++)
  {
    if (*menu[i] == '\0')
    {
      max = i - 1;
      break;
    }
        /*--- Search the max length of the strings ---*/
    length = StringLength(menu[i]);
    if (maxLength < length)
    {
      maxLength = length;
    }
  }
  PrintPcCrt(1, 24,
             "Use : 'UP', 'DOWN' to select an option ; 'RETURN' to validate, 'ESC' to exit");

        /*--- Display the menu, and erase the used area ---*/
  for (i = 0; i <= max; i++)
  {
    PrintPcCrt(x, y + i + 1, "    %s", menu[i]);
    end = maxLength - StringLength(menu[i]);
    for (j = 0; j < end; j++)
      PrintCrt(" ");
  }
        /*--- Do not accept a number of line greater than the number of line ---*/
  if (lineNumb > max)
    lineNumb = 0;

        /*--- init terminal display ---*/
  PrintPcCrt(x + 1, y + 1 + lineNumb, "->");
  while (!valid)
  {
    c = UartReadChar(TRUE);
    switch (c)
    {
                /*------- 'Home' or Escape key --------*/
    case ESC:
      ClearLines (24, 1);
      lineNumb = HOME;
      valid = TRUE;
      break;

                /*------- 'Down' key --------*/
    case '2':
    case DOWN:
      PrintPcCrt(x + 1, y + 1 + lineNumb, "  ");
      lineNumb++;
      if (*menu[lineNumb] == '\0')
        lineNumb = 0;

      PrintPcCrt(x + 1, y + 1 + lineNumb, "->");
      break;

                /*------- 'Up' key --------*/
    case '8':
    case UP:
      PrintPcCrt(x + 1, y + 1 + lineNumb, "  ");
      if (lineNumb == 0)
        lineNumb = max;
      else
        lineNumb--;

      PrintPcCrt(x + 1, y + 1 + lineNumb, "->");
      break;

                /*------- 'valid' key --------*/
    case ENTER:
      ClearLines (24, 1);
      valid = TRUE;
      break;
    }
  }
  return lineNumb;
}


static void _DisplayMenu(int8_t MenuHandle, uint8_t flag)
{
  uint8_t i;
  uint16_t value16;      /* for VAL16 and NUM16 types */
  int32_t svalue32;
  uint32_t uvalue32;
  uint16_t varType;
  int32_t *ptSvalue32;
  STRUCTCAPTUREVAL_T *ptStruct;
  char** ptr;
    
  i=0;
  while (*MenuCtrlData[MenuHandle].MenuItems[i].Item != '\0')
  {
    if (*MenuCtrlData[MenuHandle].MenuItems[i].Item == '\t') 
    {
      i++;
      continue;
    }
    if (flag & DISPLAY)
    {
      /* display the description */
      PrintPcCrt(MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.x, MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, "%s",  MenuCtrlData[MenuHandle].MenuItems[i].Item);
    }
    if ((flag & DISPLAY ) || (flag & REFRESH))
    {
      /* display the associated field */
      switch (MenuCtrlData[MenuHandle].MenuItems[i].Type)
      {
        case TOGGLE:
        case SEL_TXT:
        case STRTAB:
          /* get pointer value of pointer of string */
          ptr = (char **) MenuCtrlData[MenuHandle].MenuItems[i].Msg;
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[i].Var;            
          PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x,
                      MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, ptr[value16]);
          break;
          
        case SEL_NUM:
          ptSvalue32 = (int32_t *) MenuCtrlData[MenuHandle].MenuItems[i].Msg;
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[i].Var;  /* get value */
          
          SelectNumberBox(ptSvalue32, value16, MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x,
                          MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, DISPLAY);
          break;
          
        case NUM32:
        case VAL32:
          ptStruct = (STRUCTCAPTUREVAL_T *) MenuCtrlData[MenuHandle].MenuItems[i].Var;
          varType = CheckVaribleType(MenuCtrlData[MenuHandle].MenuItems[i].Msg);
          if (varType == SNUM)
          {
            svalue32 = *(int32_t *) ptStruct->CapValue;
            PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x, 
              MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, MenuCtrlData[MenuHandle].MenuItems[i].Msg, svalue32);
          }
          if (varType == UNUM)
          {
            uvalue32 = *(uint32_t *) ptStruct->CapValue;
            PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x, 
                       MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, MenuCtrlData[MenuHandle].MenuItems[i].Msg, uvalue32);
          }
          break;
        case TITLE:
        case ACTION:
        case NULL:
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[i].Var;
          PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x, 
                     MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, MenuCtrlData[MenuHandle].MenuItems[i].Msg, value16);
          break;
      
        case FUNCT:
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[i].Var;
          PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, MenuCtrlData[MenuHandle].MenuItems[i].Msg);
          break;
         default:
          PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[i].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[i].ItemPos.y, "???");
          break;
      }                       /* end switch */
    }
    i++;
  }
}


/* Create the menu internal data */
int8_t CreateMenu(STRUCTMENU_T * menu, uint8_t x, uint8_t y, int8_t flag, int8_t MenuHandle)
{
  uint16_t length;
  uint8_t Handle = 0xFF;
  uint8_t i,j;
  char tempstr[20];
  uint16_t NbElem;
  char** ptr;

  uint8_t cur_x;
  uint8_t cur_y;
  uint8_t  x_win_offset , y_win_offset;
  uint8_t cur_col;
  uint8_t col_inter_space;
  uint8_t NbLine;
  

  uint8_t desc_max_length, cursor_offset, val_max_length;

  
  /* Look For free slot */

  if (!(flag & RECREATE))
  {
    for (i=0; i < MAX_MENU; i++)
    {
      if (!MenuCtrlData[i].created) break;
    }

    if (i == MAX_MENU)   /* No more room to create menu */
    {
      return -1;
    }
    Handle = i;
    
  }
  else
  {
    Handle = MenuHandle;
    menu = MenuCtrlData[Handle].MenuItems;
  }

  /* reset the data structure */
  memset (&MenuCtrlData[Handle], 0, sizeof(MenuCtrlData_t));
  MenuCtrlData[Handle].MenuItems = menu;
  /*------------------------------------------------------ */
  /* parse info from menu structure */
  
  /* ==== Look for the max number of menu lines to control exit function ==== */
  /* to be updated if WINDOW flag */

  if (flag & DRAW_WINDOW)
  {    
    x_win_offset = 1;
    y_win_offset = 1;
  }
  else
  {
    x_win_offset = 0;
    y_win_offset = 0;
  }

  MenuCtrlData[Handle].WinPos.WinPosUpperLeft.x = x;
  MenuCtrlData[Handle].WinPos.WinPosUpperLeft.y = y;

  MenuCtrlData[Handle].WinPos.WinPosDownRigth.x = x ;
  MenuCtrlData[Handle].WinPos.WinPosDownRigth.y = y;

      
  cur_x = x + x_win_offset;
  cur_y = y + y_win_offset;
  col_inter_space = 2;
  desc_max_length = 0;
  cursor_offset = 0;
  val_max_length = 0;
  cur_col = 0;
  NbLine = 0;

  
  for (i = 0; i < MAXITEMNB; i++)
  {
    /* end of Menu, leave */
    if (* MenuCtrlData[Handle].MenuItems[i].Item == '\0')   
    {
      if (i == 0)
      {
        return -1;  /* no Item ?? */
      }
      /* adjust Windows size anc check if fit in screen */      
      MenuCtrlData[Handle].WinPos.WinPosDownRigth.x += desc_max_length + cursor_offset + val_max_length + x_win_offset;

      /* Out of screen ? */
      if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.x > MAX_X)
        return -1;

      if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.y <= (cur_y + y_win_offset))
        MenuCtrlData[Handle].WinPos.WinPosDownRigth.y = cur_y + y_win_offset;

      /* Out of screen ? */
      if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.y>= MAX_Y)
        return -1;

      /* set col info */   
      MenuCtrlData[Handle].ColInfo[cur_col].Cursor_x = cur_x +  desc_max_length;
      MenuCtrlData[Handle].ColInfo[cur_col].Field_x = cur_x + desc_max_length + cursor_offset;
      MenuCtrlData[Handle].ColInfo[cur_col].NbLines = NbLine;
      MenuCtrlData[Handle].NbCol = cur_col + 1;
      MenuCtrlData[Handle].created = 1; 
      
      break;
    }
    
    /* New column */
    if (* MenuCtrlData[Handle].MenuItems[i].Item == '\t')   
    {
      /* adjust Windows size  */      
      MenuCtrlData[Handle].WinPos.WinPosDownRigth.x += desc_max_length + cursor_offset + val_max_length + x_win_offset;


      if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.y <= (cur_y + y_win_offset))
        MenuCtrlData[Handle].WinPos.WinPosDownRigth.y = cur_y + y_win_offset;

      /* set col info */   
      MenuCtrlData[Handle].ColInfo[cur_col].Cursor_x = cur_x +  desc_max_length;
      MenuCtrlData[Handle].ColInfo[cur_col].Field_x = cur_x + desc_max_length + cursor_offset;

      MenuCtrlData[Handle].ColInfo[cur_col].NbLines = NbLine;

      /* New col */
      cur_col++;

      /* store nb coluomns ? */
      if (cur_col > MAXCOLNB)
      {                         /* test if max reached */
        *menu[i].Item = '\0';   /* force end of menu instead of end of column */

        /* check Win Size */
        if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.x > MAX_X)
         return -1;

        /* Out of screen ? */
        if (MenuCtrlData[Handle].WinPos.WinPosDownRigth.x >= MAX_Y)
         return -1;

        /* set col info */   
        MenuCtrlData[Handle].ColInfo[cur_col].Cursor_x = cur_x +  desc_max_length;
        MenuCtrlData[Handle].ColInfo[cur_col].Field_x = cur_x + desc_max_length + cursor_offset;
        MenuCtrlData[Handle].NbCol = cur_col;
        MenuCtrlData[Handle].created = 1; 

        break;
      }      
       
      MenuCtrlData[Handle].nbEntries++;  /* count Column separator as entry */
      /* adjust cur_x */
      cur_x = cur_x + desc_max_length + cursor_offset + val_max_length +  col_inter_space; 

      /* reset cur_y */
      cur_y = y + y_win_offset;      

      cursor_offset = 1;
      desc_max_length = 0;
      val_max_length = 0;
      NbLine = 0;
    }
    else
    {
      if (i == MAXITEMNB)           /* table too long */
      {
        *menu[MAXITEMNB - 1].Item = '\0';   /* force end of menu */
        continue;
      }
       
      MenuCtrlData[Handle].nbItems++;
      MenuCtrlData[Handle].nbEntries++;
      MenuCtrlData[Handle].ItemsPos[i].ItemPos.x= cur_x;
      MenuCtrlData[Handle].ItemsPos[i].ItemPos.y= cur_y++; 
      MenuCtrlData[Handle].ItemsPos[i].ColIndex = cur_col;
      NbLine++;
      
      
      length = StringLength(menu[i].Item);
      desc_max_length = length > desc_max_length ?  length : desc_max_length;
      
     
      /* evaluate size of value */
      switch (menu[i].Type)
      {
        case TOGGLE:
        case SEL_TXT:
           MenuCtrlData[Handle].ItemsPos[i].Selectable = 1;
           MenuCtrlData[Handle].ItemSelectable = 1;
           cursor_offset = 3;
  
        case STRTAB:
          /* get pointer value of pointer of string */
          ptr = (char **) menu[i].Msg;
          j = 0;
          while (*ptr[j] != '\0') /* check all string */
          {
            length = StringLength(ptr[j++]);
            if ( val_max_length < length)
            {
              val_max_length = length;
            }
          }
          break;
  
        case ACTION:
          MenuCtrlData[Handle].ItemsPos[i].Selectable = 1;
          MenuCtrlData[Handle].ItemSelectable = 1;
          cursor_offset = 3;

  
        case TITLE:
          if (menu[i].Var != NULL)        /* if value to be displayed */
          {
            /* -- Test to print a value -- */
            sprintf((char *) tempstr, (char *) menu[i].Msg, 1);
            length = StringLength(tempstr);
          }
          else
          {
            length = StringLength(menu[i].Msg);
          }
  
          if ( val_max_length < length)
          {
            val_max_length = length;
          }
          break;
  
        case FUNCT:
          MenuCtrlData[Handle].ItemsPos[i].Selectable = 1;
          MenuCtrlData[Handle].ItemSelectable = 1;
          cursor_offset = 3;
          
          length = StringLength(menu[i].Msg);
          if (val_max_length < length)
          {
            val_max_length = length;
          }
          break;
  
        case SEL_NUM:
          MenuCtrlData[Handle].ItemsPos[i].Selectable = 1;
          MenuCtrlData[Handle].ItemSelectable = 1;
          cursor_offset = 3;
          
          /* compute the length */
          length = NbDigit((int32_t *) menu[i].Msg, &NbElem);
          if (val_max_length < length)
          {
            val_max_length = length;
          }
          break;
  
        case NUM32:
          MenuCtrlData[Handle].ItemsPos[i].Selectable = 1;
          MenuCtrlData[Handle].ItemSelectable = 1;          
          cursor_offset = 3;
  
        case VAL16:
        case SVAL32:
        case VAL32:
          /* -- Test to print a value -- */
          sprintf((char *) tempstr, (char *) menu[i].Msg, 1);
          length = StringLength(tempstr);
          if ( val_max_length < length)
          {
            val_max_length = length;
          }
          break;
  
        default:
          break;
       
      }
    }
  }
  
  /*---------------------------------------------------------------------------- */
  /*  -----                       Now Display                              ----- */
  /*---------------------------------------------------------------------------- */
  if (flag & DISPLAY)
    _DisplayMenu(Handle, flag);
  
  return Handle;
}


void DeleteMenu(int8_t MenuHandle)
{
  MenuCtrlData[MenuHandle].created = 0; 
}


/**
 *******************************************************************************
 * @fn      uint16_t Menu(STRUCTMENU_T * menu, uint16_t ItemNumb, int x, int y, 
 *                        int flag)
 * @author  -
 * @brief Displays the menu pointed by the parameters menu[], and select the 
 *      line indicated by 'line'
 *      with ARROW and VALID keys (issued from the UART or for the keypad), 
 *      it is possible to select the desired option.
 *      key : 'UP'      Select the previous available line
 *      key : 'DOWN'    Select the next available line
 *      key : 'LEFT'    Select the next available line to left with rollover to 
 *                      right
 *      key : 'RIGHT'   Select the next available line to right with rollover to 
 *                      left
 *      key : 'ENTER'   Validate the selected line with action depend on type
 *      key : 'ESC'     Exit from the menu
 *
 * @param menu :  Pointer of a structure containing the menu to be displayed.
 *          THE LAST LINE OF THE TABLE MUST BE : ""
 * @param ItemNumb : Item number to display.
 * @param x, y : Define the Column & Line where will be displayed the upper left
 *               corner of the menu
 * @param flag :   DISPLAY for full display only
 *                 CAPTURE for selection only
 *                 DISPLAY+CAPTURE for full display and selection
 *                 REFRESH to refresh variable values
 *
 * @return  ItemNumb : Return the Item selected.
 *          if ItemNumb = 0xFE ---> Home
 *          else               ---> ItemNumb selected
 *******************************************************************************
 **/
uint16_t RunMenu(uint8_t MenuHandle, uint16_t ItemNumb, uint8_t x, uint8_t y,
              uint8_t flag)
{
  uint16_t valid;
  uint8_t ytemp;
  char c;
  char **ptr;            /* pointer of a string pointer */
  static void (*funcptr) (void);        /* function pointer */
  static uint16_t value16;      /* for VAL16 and NUM16 types */
  STRUCTCAPTUREVAL_T *ptStruct;  
  static int32_t *ptSvalue32;
  static uint8_t PrintActionInfo = TRUE;
  static uint8_t PrintCursor = TRUE;

  if (flag & RECREATE)
  {
    /* Recreate the menu already created, this is just to allow a kind of dynamic update of complete menu items and location */
    if (CreateMenu(NULL, x,y,DISPLAY,MenuHandle) == -1)
    {
      return 0xFFFF; 
    }   
  }
  else
     _DisplayMenu(MenuHandle, flag);
    

  if ((flag & CAPTURE) && (MenuCtrlData[MenuHandle].ItemSelectable))
  {
    if (PrintActionInfo)
    {
      PrintPcCrt(1, MAX_Y,
                "Use : ARROWS to select an option ; 'RETURN' to validate, 'ESC' to exit");
      PrintActionInfo = FALSE;
    }
  }
  
  if (!MenuCtrlData[MenuHandle].ItemSelectable)
  {
    return 0xFFFF;  /* nothing to select */
  }
 
  /*------------------------------------------------------ */
  /* Main loop , waiting key an do some action if valid pressed */
  /* clear the special bit ItemNumber */
  ItemNumb &= ~0xFF00;
  if (flag & CAPTURE)           /* only if capture data */
  {
    valid = 0;
    
       /*--- init terminal display ---*/
        /*--- Do not accept a number of line greater than the number of line ---*/
    if (ItemNumb > MenuCtrlData[MenuHandle].nbEntries)
      ItemNumb = 0;
    skip_next_inactive_item2(ItemNumb,MenuCtrlData[MenuHandle]);
    if (PrintCursor)
    {
      PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, 
                 MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, 
                 "->");
    }
    while (!valid)
    {      
      c = UartReadChar(!(flag & CAPTURE_NON_BLOCKING));
      switch (c)
      {
                /*------- 'Home' or Escape key --------*/
      case ESC:
        ClearLines (MAX_Y, 1);
        PrintActionInfo = TRUE;
        if ((flag & CAPTURE_NON_BLOCKING))
          /* add ESC flag to return item number */       
          ItemNumb |=  ESC_FLAG;  
        else
          ItemNumb = HOME;     
        
        valid = TRUE;
        break;

                /*------- 'Down' key --------*/
      case '2':
      case DOWN:

        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "  ");
        ItemNumb++;
        if (*MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Item == '\0')
          ItemNumb = 0;         /* Roll over */
        skip_next_inactive_item2(ItemNumb,MenuCtrlData[MenuHandle] );

        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "->");
        break;

                /*------- 'Up' key --------*/
      case '8':
      case UP:
        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "  ");
        if (ItemNumb == 0)
          ItemNumb = MenuCtrlData[MenuHandle].nbEntries - 1;            /* max item number */
        else
          ItemNumb--;
        skip_previous_inactive_item2(ItemNumb,MenuCtrlData[MenuHandle]);

        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "->");
        break;

                /*------- 'Left' key --------*/
      case '4':
      case LEFT:
        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "  ");
        ytemp  = MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y;
        /* loop while find same y coord. */
        do
        {
          if (ItemNumb == 0)
            ItemNumb = MenuCtrlData[MenuHandle].nbEntries - 1 ;       /* max item number */
          else
            ItemNumb--;          
          skip_previous_inactive_item2(ItemNumb, MenuCtrlData[MenuHandle]);
        }
        while (MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y != ytemp);
        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "->");
        break;

                /*------- 'Right' key --------*/
      case '6':
      case RIGHT:
        PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "  ");
        ytemp = MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y;
        /* loop while find same y coord. */
        do
        {
          ItemNumb++;
          if (*MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Item == '\0')
            ItemNumb = 0;       /* Roll over */
          skip_next_inactive_item2(ItemNumb,MenuCtrlData[MenuHandle]);

        }
        while (MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y != ytemp);

       PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Cursor_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, "->");
        break;

                /*------- 'valid' key --------*/
      case ENTER:
        switch (MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Type)
        {
        case TOGGLE:           /* TOGGLE Value and display message */
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var;   /* get value */
          value16 = ~value16 & 1;       /* toggle value */
          *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var = value16;   /* write back new value */
          ptr = (char **) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Msg;   /* get pointer value of pointer of string */
          PrintPcCrt(MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y, ptr[value16]);
          break;

        case SEL_TXT:          /* SELECTBOX on Item */
          ptr = (char **) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Msg;   /* get pointer value of pointer of string */
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var;   /* get value */
          value16 =
            SelectBox(ptr, value16,MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y,
                      CAPTURE);
          *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var = value16;   /* write back new value */
          break;

        case SEL_NUM:          /* SELECT NUMBER BOX on Item */
          ptSvalue32 = (int32_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Msg;
          value16 = *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var;   /* get value */
          value16 =
            SelectNumberBox(ptSvalue32, value16,
                            MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y,
                            CAPTURE);
          *(uint16_t *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var = value16;   /* write back new value */
          break;

        case NUM32:            /* 32 bits number entry */
          ptStruct = (STRUCTCAPTUREVAL_T *) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var;
          GetNumberBox_Range(MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Msg, ptStruct,
                             MenuCtrlData[MenuHandle].ColInfo[MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ColIndex].Field_x, MenuCtrlData[MenuHandle].ItemsPos[ItemNumb].ItemPos.y);
          break;

        case ACTION:           /* Action is made in user routine */
          break;

        case FUNCT:            /* Run user function */
          funcptr = (void (*)()) MenuCtrlData[MenuHandle].MenuItems[ItemNumb].Var;    /* get pointer value of pointer of function */
          funcptr();
          break;

        default:
          break;
        }
        ClearLines (MAX_Y, 1);
        PrintActionInfo= TRUE;
        if ((flag & CAPTURE_NON_BLOCKING))
         ItemNumb |=  ENTER_FLAG;
        valid = TRUE;
        
        break;
      }  
      if (flag & CAPTURE_NON_BLOCKING)
        valid = TRUE;
        
    }                           /* end while !VALID */
  }                             /* end if CAPTURE */
  return ItemNumb;
}


uint16_t CheckVaribleType(char *fmtStr)
{
  char lpCaract;
  uint16_t ii;
  uint16_t step;

  ii = 0;
  step = 0;
  while (*(fmtStr + ii) != 0)
  {
    lpCaract = *(fmtStr + ii);
    /* search for '%' character */
    if ((lpCaract == '%') && (step == 0))
    {
      step = 1;
    }
    if ((lpCaract == 'd') && (step == 1))
    {
      return SNUM;
    }
    if ((lpCaract == 'u') && (step == 1))
    {
      return UNUM;
    }
    if ((lpCaract == 'i') && (step == 1))
    {
      return SNUM;
    }
    if ((lpCaract == 'x') && (step == 1))
    {
      return UNUM;
    }
    if ((lpCaract == 'X') && (step == 1))
    {
      return UNUM;
    }
    ii++;
  }
  return FALSE;
}


/**
 *******************************************************************************
 * @fn      uint16_t SelectBox(int8_t * szMenu[], uint16_t iLineNumb, 
 *                             uint16_t iX, uint16_t iY)
 * @author  -
 * @brief Displays the option pointed by the parameters menu[], and select the 
 *      line indicated by 'line'.
 *      With UP, DOWN, and VALID keys (issued from the UART or for the keypad), 
 *      it is possible toselect the desired option. ONLY THE SELECTED LINE IS 
 *      DISPLAYED.
 *
 * @param szMenu : Pointer of a table containing the menu to be displayed.
 *             THE LAST LINE OF THE TABLE MUST BE : ""
 * @param iLineNumb : Line number to display .
 * @param iX, iY : Column & Line position of Upper Left corner of the Select box.
 *
 * @return: Return the line menu selected.
 *******************************************************************************
 **/
uint16_t SelectBox(char *szMenu[], uint8_t iLineNumb, uint8_t iX,
                   uint8_t iY, uint8_t mode)
{
  uint16_t valid, max, i, length, maxLength, end;

  valid = FALSE;
  max = 1;
  maxLength = 0;

  PrintPcCrt(1, 24,
             "Select option using 'UP', 'DOWN' and 'RETURN' keys                         ");

  /* -- Look for the max number of menu lines to control exit function -- */
  for (i = 0; i < 64; i++)
  {
    if (*szMenu[i] == '\0')
    {
      max = i - 1;
      break;
    }
        /*--- Search the max length of the strings ---*/
    length = StringLength(szMenu[i]);
    if (maxLength < length)
    {
      maxLength = length;
    }
  }

  while (!valid)
  {
        /*--- Display the szMenu, and erase the used area ---*/
    PrintPcCrt(iX, iY, "%s", szMenu[iLineNumb]);
    end = maxLength - StringLength(szMenu[iLineNumb]);

    for (i = 0; i < end; i++)
    {
      PrintCrt(" ");
    }

    /* =====  Display only  ===== */
    if (mode != CAPTURE)
    {
      break;
    }

    PrintCrt("<");
    switch (UartReadChar(TRUE))
    {
                /*------- 'UP' key --------*/
    case '8':
    case UP:
      iLineNumb++;
      if (*szMenu[iLineNumb] == '\0')
      {
        iLineNumb = 0;
      }
      break;

                /*------- 'DOWN' key --------*/
    case '2':
    case DOWN:
      if (iLineNumb == 0)
      {
        iLineNumb = max;
      }
      else
      {
        iLineNumb--;
      }
      break;

                /*------- 'valid' or 'ENTER' key --------*/
    case ENTER:
      /* -- Erase the '<' -- */
      PrintPcCrt((iX + maxLength), iY, " ");
      ClearLines (24, 1);
      valid = TRUE;
      break;
    }
  }

  return (iLineNumb);
}


/**
 *******************************************************************************
 * @fn      uint16_t SelectNumberBox(int32_t * ptTabNber, uint16_t iLineNumb, 
 *                                   uint16_t iX, uint16_t iY)
 * @author  -
 * @brief Displays the option pointed by the parameters *ptTabNber, and select 
 *      the line indicated by 'iLineNumb'.
 *      With UP, DOWN, and VALID keys (issued from the UART or for the keypad), 
 *      it is possible to
 *      select the desired option. ONLY THE SELECTED LINE IS DISPLAYED.
 *
 * @param ptTabNber : Pointer of a table containing the menu to be displayed.
 *             THE LAST LINE OF THE TABLE IS DETECTED WHEN 2 CONSECUTIVE VALUES 
 *             ARE EQUAL
 * @param iLineNumb : Line number to display .
 * @param iX, iY : Column & Line position of Upper Left corner of the Select box.
 *
 * @return: Return the line menu selected.
 **************************************************************************
 **/
uint16_t SelectNumberBox(int32_t * ptTabNber, uint8_t iLineNumb,
                         uint8_t iX, uint8_t iY, uint8_t mode)
{
  uint16_t valid, max, maxLength;

  char szFormat[10];

  szFormat[0] = '%';
  szFormat[2] = 'd';
  szFormat[3] = 0;

  valid = FALSE;
  max = 1;
  maxLength = 0;

  /* -- Look for the max number of menu lines to control exit function -- */
  maxLength = NbDigit(ptTabNber, &max);

  while (!valid)
  {
                                /*--- Display the szMenu, and erase the used area ---*/
    szFormat[1] = '0' + maxLength;

    /* =====  Display only  ===== */
    if (mode != CAPTURE)
    {
      PrintPcCrt(iX, iY, szFormat, *(ptTabNber + iLineNumb));
      break;
    }
    PrintPcCrt(1, 24,
               "Select option using 'UP', 'DOWN' and 'RETURN' keys                         ");
    PrintPcCrt(iX, iY, szFormat, *(ptTabNber + iLineNumb));
    PrintCrt("<");

    switch (UartReadChar(TRUE))
    {
                                                /*------- 'UP' key --------*/
    case '8':
    case UP:
      if (iLineNumb == max)
      {
        iLineNumb = 0;
      }
      else
      {
        iLineNumb++;
      }
      break;

                                                /*------- 'DOWN' key --------*/
    case '2':
    case DOWN:
      if (iLineNumb == 0)
      {
        iLineNumb = max;
      }
      else
      {
        iLineNumb--;
      }
      break;

                                                /*------- 'valid' or 'ENTER' key --------*/
    case ENTER:
      /* -- Erase the '<' -- */
      PrintPcCrt((iX + maxLength), iY, " ");
      ClearLines (24, 1);
      valid = TRUE;
      break;
    }
  }
  return (iLineNumb);
}

/**
 *******************************************************************************
 * @fn      void GetNumberBox_Range( char * szFmt, void * CaptureVal, 
 *                                   STRUCTRANGE * stRangeVal, uint16_t iX, 
 *                                   uint16_t iY)
 * @author  -
 * @brief This function allow displays on a terminal the text pointed by 
 *      "szFmt", to the positioniX, iY. If the string contains format characters
 *      such as "%d, %4d, %06d, %04x...;",
 *      the function will display the default value 'CaptureVal' and wait for an
 *      hexa number or decimal number. The number entried is validate by the 
 *      'ENTER' key. This function will return the converted number only when 
 *      the number was correctly decoded.
 *
 * @param szFmt : Points the formatted string containing information on the 
 *                message to display,
 *                and on the number to get.
 * @param CaptureVal : Is the default value to display
 * @param iX, iY : Column & Line position.
 *
 * @return: Return the binary value of the number captured.
 *******************************************************************************
 **/
void GetNumberBox_Range(char *szFmt, STRUCTCAPTUREVAL_T * stRangeVal,
                        uint8_t iX, uint8_t iY)
{
  uint8_t cLength, cStatus;
  uint16_t iCol, iCpt;
  char szFormat[10], szString[20];
  int8_t *lpCaract;
  uint8_t ValidRange;

  int32_t sNumber;
  uint32_t uNumber;

  cStatus = FALSE;
  while (cStatus == FALSE)
  {
    cStatus = TRUE;
    iCpt = 0;
    iCol = 0;
    PosCur(iX, iY);

    for (lpCaract = (int8_t *) szFmt; (*lpCaract != 0) && (cStatus != FALSE);
         lpCaract++)
    {
      if (*lpCaract != '%')
      {
        PrintCrt("%c", *lpCaract);
        iCol++;
        continue;
      }
      szFormat[iCpt++] = *lpCaract;
      lpCaract++;
      while ((*lpCaract == '+'))
      {
        lpCaract++;
      }

      cLength = 0;
      if ((*lpCaract == '0') || (*lpCaract == '-'))
      {
        szFormat[iCpt++] = *lpCaract;
        lpCaract++;
      }
      while (IsDigit(*lpCaract))
      {
        szFormat[iCpt++] = *lpCaract;
        cLength = cLength * 10 + (*lpCaract++ - '0');
      }
      if (*lpCaract == 'l')
      {
        lpCaract++;
      }

      switch (*lpCaract)
      {
      case 'd':
      case 'i':
        szFormat[iCpt++] = *lpCaract;
        szFormat[iCpt++] = 0;
        ValidRange = 0;
        sNumber = *((int32_t *) stRangeVal->CapValue);
        /* Display a message if range check is enabled */
        if (*((int32_t *) stRangeVal->MinValue) !=
            *((int32_t *) stRangeVal->MaxValue))
        {
          PrintPcCrt(1, 24,
                     "The number must be between %d and %d                                       ",
                     *((int32_t *) stRangeVal->MinValue),
                     *((int32_t *) stRangeVal->MaxValue));
        }
        while (ValidRange == 0)
        {
          sprintf((char *) szString, (char *) &szFormat[0], sNumber);
          if (cLength == 0)
          {
            cLength = 10;
          }
          ReadPcStr((iX + iCol), iY, szString, (int8_t) cLength, '.', TRUE,
                    OVERSTK);
          if (AToDec(szString, &sNumber) == FALSE)
          {
            cStatus = FALSE;
          }

          /* if captured number is between MinValue and  MaxValue */
          if ((sNumber >= *((int32_t *) stRangeVal->MinValue))
              && (sNumber <= *((int32_t *) stRangeVal->MaxValue)))
          {
            ValidRange = 1;
          }

          /* if MinValue = MaxValue, then no range is enabled */
          if (*((int32_t *) stRangeVal->MinValue) >=
              *((int32_t *) stRangeVal->MaxValue))
          {
            ValidRange = 1;
          }

        }
        /* value updated only when it is valid */
        *((int32_t *) stRangeVal->CapValue) = sNumber;
        iCol = iCol + cLength;
        break;

      case 'u':
      case 'x':
      case 'X':
        szFormat[iCpt++] = *lpCaract;
        szFormat[iCpt++] = 0;
        ValidRange = 0;
        uNumber = *((uint32_t *) stRangeVal->CapValue);
        /* Display a message if range check is enabled */
        if (*((uint32_t *) stRangeVal->MinValue) !=
            *((uint32_t *) stRangeVal->MaxValue))
        {
          PrintPcCrt(1, 24,
                     "The number must be between 0x%X and 0x%X                                       ",
                     *((uint32_t *) stRangeVal->MinValue),
                     *((uint32_t *) stRangeVal->MaxValue));
        }
        while (ValidRange == 0)
        {
          sprintf((char *) szString, (char *) &szFormat[0], uNumber);
          if (cLength == 0)
          {
            cLength = 8;
          }

          ReadPcStr((iX + iCol), iY, szString, (int8_t) cLength, '.', TRUE,
                    OVERSTK);
          if (AToHex(szString, &uNumber) == FALSE)
          {
            cStatus = FALSE;
          }

          /* if captured number is between MinValue and  MaxValue */
          if ((uNumber >= *((uint32_t *) stRangeVal->MinValue))
              && (uNumber <= *((uint32_t *) stRangeVal->MaxValue)))
          {
            ValidRange = 1;
          }

          /* if MinValue = MaxValue, then no range is enabled */
          if (*((uint32_t *) stRangeVal->MinValue) >=
              *((uint32_t *) stRangeVal->MaxValue))
          {
            ValidRange = 1;
          }
        }

        /* value updated only when it is valid */
        *((uint32_t *) stRangeVal->CapValue) = uNumber;
        iCol = iCol + cLength;
        break;
      }
    }
  }
  ClearLines (24, 1);
}

uint16_t NbDigit(int32_t * ptTabNber, uint16_t * NbElements)
{
  int32_t Prev;
  uint16_t ii;
  uint16_t kk;
  int32_t div;
  uint16_t maxLength;

  maxLength = 0;

  /* -- Look for the max number of menu lines to control exit function -- */
  Prev = *ptTabNber + 1;        /* shall be different from the fistr value */
  for (ii = 0; ii < 64; ii++)
  {
    /* consecutive duplicated value indicates the end of the list */
    if (*(ptTabNber + ii) == Prev)
    {
      break;
    }
        /*--- Search the max length of the strings ---*/
    div = 10;
    for (kk = 1; kk < 10; kk++)
    {
      if (*(ptTabNber + ii) / div == 0)
      {
        break;
      }
      div = div * 10;
    }
    if (*(ptTabNber + ii) < 0)
    {                           /* If the number is negative, a digital is added for '-' */
      kk++;
    }
    if (maxLength < kk)
    {
      maxLength = kk;
    }
    Prev = *(ptTabNber + ii);
  }
  *NbElements = ii - 1;
  return maxLength;
}
