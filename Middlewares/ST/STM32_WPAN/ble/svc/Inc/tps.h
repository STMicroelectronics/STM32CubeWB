/**
  ******************************************************************************
  * @file    tps.h
  * @author  MCD Application Team
  * @brief   Header for tps.c module
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
#ifndef __TPS_H
#define __TPS_H

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
void TPS_Init(void);
tBleStatus TPS_Update_Char(uint16_t UUID, uint8_t *pPayload);

#ifdef __cplusplus
}
#endif

#endif /*__TPS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
