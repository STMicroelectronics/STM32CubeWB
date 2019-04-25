/**
 ******************************************************************************
 * @file    hids_menu.c
 * @author  MCD Application Team
 * @brief   Human Interface Device Server Menu Application
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
