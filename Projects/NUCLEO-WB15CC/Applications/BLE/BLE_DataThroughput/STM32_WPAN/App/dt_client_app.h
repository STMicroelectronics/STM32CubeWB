
/**
  ******************************************************************************
 * @file    dt_client_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_client_app.c module
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
