/**
 ******************************************************************************
 * @file    hids_menu.c
 * @author  MCD Application Team
 * @brief   Human Interface Device Server Menu Application
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
static STRUCTMENU_T Hid_Menu[] =
{
#if(CFG_GATT_CLIENT == 0)
  {  "Input Report Notification",               NULL,       ACTION,  " "           },
  {  "Boot Keyboard Input Report Notification", NULL,       ACTION,  " "           },
  {  "Boot Mouse Input Report Notification",    NULL,       ACTION,  " "           },
#endif                                                                             
  {  ""                                                                            },
};

enum _HID_MENU
{
#if(CFG_GATT_CLIENT == 0)
  HID_IRN = 0,
  HID_BKIRN,
  HID_BMIRN,
#endif                                                                         
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

  *report = 0;
  
  ClearScreen ();

  Handle = CreateMenu(Hid_Menu, 
                      3, 
                      9, 
                      0, 
                      0);

  do
  {
    DisplayTitle (1, cMenuTitle, cSoftwareVersion);

    wItem = RunMenu(Handle, 
                    wItem, 
                    3, 
                    9, 
                    CAPTURE+DISPLAY); 
    switch (wItem)
    {
#if(CFG_GATT_CLIENT == 0)
      case HID_IRN:
        {
          *action_type = 1;
          wItem = HOME;
        }
        break;
        
      case HID_BKIRN:
        {
          *action_type = 2;
          wItem = HOME;
        }
        break;
      
      case HID_BMIRN:
        {
          *action_type = 3;
          wItem = HOME;
        }
        break;
#endif
      
      case ESCAPE:
        {
          *action_type = 0;
          wItem = HOME;
        }
        break;
        
      default :
        break;
    }
  }
  while (wItem != HOME);

  ClearScreen ();
  DeleteMenu(Handle);
}



