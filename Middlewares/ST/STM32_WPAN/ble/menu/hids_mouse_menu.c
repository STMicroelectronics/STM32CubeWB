/**
 ******************************************************************************
 * @file    hids_mouse_menu.c
 * @author  MCD Application Team
 * @brief   Human Interface Device Mouse Server Menu Application
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


/* Includes ------------------------------------------------------------------*/
#include "DispTools.h"
#include "hids_menu.h"
#include "app_conf.h"

/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
#define ESCAPE              254


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char *cMenuTitle = "HUMAN INTERFACE DEVICE SERVER";
static char *cSoftwareVersion = "1.0";

/***************************************
*    M E N U   D E F I N I T I O N S   *
***************************************/
static const char * cButtonState[] = 
{
  "Released", 
  "Pressed ",
  "" 
};
static uint16_t Button1, Button2, Button3;
static int32_t XPosVal, XPosMin, XPosMax;
static int32_t YPosVal, YPosMin, YPosMax;
static int32_t WIndVal, WIndMin, WIndMax;
STRUCTCAPTUREVAL_T XPos =
{ &XPosVal, 
  &XPosMin, 
  &XPosMax
};
STRUCTCAPTUREVAL_T YPos =
{ &YPosVal, 
  &YPosMin, 
  &YPosMax 
};
STRUCTCAPTUREVAL_T WInd =
{ 
  &WIndVal, 
  &WIndMin, 
  &WIndMax 
};
static STRUCTMENU_T HID_Set_Report_Menu[] =
{
  {  "Button 1",            &Button1, TOGGLE, cButtonState },
  {  "Button 2",            &Button2, TOGGLE, cButtonState },
  {  "Button 3",            &Button3, TOGGLE, cButtonState },
  {  "Horizontal Position", &XPos,    NUM32,  "%04d"       },
  {  "Vertical Position",   &YPos,    NUM32,  "%04d"       },
  {  "Wheel Indentation",   &WInd,    NUM32,  "%04d"       },
  {  " ",                   NULL,     TITLE,  " "          },
  {  "Save & Quit  ",       NULL,     ACTION, " "          },
  {  ""                                                    }
};

enum _HID_Set_Report_Menu_Enum
{
  HID_B1 = 0,
  HID_B2,
  HID_B3,
  HID_HP,
  HID_VP,
  HID_WI,
  HID_SRNULL,
  HID_SRQUIT
};

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
 **********************************************************************************************************************
 * \fn:     void HIDS_Menu(uint8_t * action_type)
 * \author:  -
 * \brief:  Human Interface Device action Menu.
 * \param:  *action_type pointer on action type
 * \return: None
 **********************************************************************************************************************
**/
void HIDS_Menu (uint8_t * action_type, uint8_t * report)
{
  uint16_t wItem = 0;
  uint8_t Handle;
  
  ClearScreen ();

  XPosVal = 0;
  XPosMin = (int8_t)(-127);
  XPosMax = (int8_t)127;
  YPosVal = 0;
  YPosMin = (int8_t)(-127);
  YPosMax = (int8_t)127;
  WIndVal = 0;
  WIndMin = (int8_t)(-127);
  WIndMax = (int8_t)127;

  Handle = CreateMenu(HID_Set_Report_Menu, 
                      6, 
                      12, 
                      0, 
                      0);

  do
  {
    DisplayTitle (1, cMenuTitle, cSoftwareVersion);
    DisplaySubTitle (10, "Report Setting Menu", FALSE);
    wItem = RunMenu(Handle, 
                    wItem, 
                    6, 
                    12, 
                    CAPTURE+DISPLAY); 
    switch (wItem)
    {
      case HID_SRQUIT:
        {
          *action_type = 1;
          report[0] = Button1;
          report[0] |= Button2 << 1;
          report[0] |= Button3 << 2;
          report[1] = XPosVal;
          report[2] = YPosVal;
          report[3] = WIndVal;
          wItem = HOME;
        }
        break;

      case ESCAPE:
        {
          *action_type = 0;
          wItem = HOME;
        }
        break;

      default:
        break;
    }
  }
  while (wItem != HOME);

  ClearScreen ();
  DeleteMenu(Handle);
}


