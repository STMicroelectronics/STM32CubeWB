/**
  ******************************************************************************
  * @file    hids.h
  * @author  MCD Application Team
  * @brief   Header for hids.c module
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



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HIDS_H
#define __HIDS_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  HIDS_REPORT_NOTIFICATION_ENABLED,
  HIDS_REPORT_NOTIFICATION_DISABLED,
  HIDS_KEYB_INPUT_NOTIFY_ENABLED,
  HIDS_KEYB_INPUT_NOTIFY_DISABLED,
  HIDS_MOUSE_INPUT_NOTIFY_ENABLED,
  HIDS_MOUSE_INPUT_NOTIFY_DISABLED,
  HIDS_OUTPUT_REPORT,
  HIDS_KEYBOARD_INPUT_REPORT,
  HIDS_KEYBOARD_OUTPUT_REPORT,
  HIDS_MOUSE_INPUT_REPORT
} HIDS_Opcode_Notification_evt_t;

typedef struct
{
  HIDS_Opcode_Notification_evt_t  HIDS_Evt_Opcode;
  uint8_t   Instance;
  uint8_t   Index;
  uint8_t   ReportLength;
  uint8_t   *pReport;
} HIDS_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HIDS_Init(void);
tBleStatus HIDS_Update_Char(uint16_t UUID, 
                            uint8_t service_instance, 
                            uint8_t Report_Index, 
                            uint8_t report_size,
                            uint8_t *pPayload);
void HIDS_Notification(HIDS_App_Notification_evt_t *pNotification);


#ifdef __cplusplus
}
#endif

#endif /*__HIDS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
