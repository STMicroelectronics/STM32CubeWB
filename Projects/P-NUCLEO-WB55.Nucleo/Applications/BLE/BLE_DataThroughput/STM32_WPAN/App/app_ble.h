/**
 ******************************************************************************
 * @file    app_ble.h
 * @author  MCD Application Team
 * @brief   Header for ble application
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
#ifndef __APP_BLE_H
#define __APP_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"
  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void APP_BLE_Init( void );
uint8_t APP_BLE_ComputeCRC8( uint8_t *DataPtr , uint8_t Datalen );

void APP_BLE_Key_Button1_Action(void);
void APP_BLE_Key_Button2_Action(void);
void APP_BLE_Key_Button3_Action(void);

#ifdef __cplusplus
}
#endif

#endif /*__APP_BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
