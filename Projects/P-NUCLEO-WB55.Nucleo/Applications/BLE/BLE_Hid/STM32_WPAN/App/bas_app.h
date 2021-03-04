
/**
 ******************************************************************************
 * @file    bats_app.h
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    7-Dec-2016
 * @brief   Header for bats_app.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#ifndef __BAS_APP_H
#define __BAS_APP_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void BASAPP_Init(uint8_t index);
void BASAPP_Level(void);


#ifdef __cplusplus
}
#endif

#endif /*__BAS_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
