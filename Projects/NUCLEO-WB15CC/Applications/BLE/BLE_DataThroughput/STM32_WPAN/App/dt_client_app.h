
/**
 ******************************************************************************
 * @file    dt_client_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_client_app.c module
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
#ifndef __DTC_APPLICATION_H
#define __DT_CLIENT_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DTC_App_Init(void);
void DTC_App_LinkReadyNotification(uint16_t ConnectionHandle);

#ifdef __cplusplus
}
#endif

#endif /*__DTC_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
