/**
  ******************************************************************************
  * @file    usbd_audio_if.h
  * @author  MCD Application Team
  * @brief   Header for usbd_audio_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_AUDIO_IF_H
#define __USBD_AUDIO_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio_in.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Send_Audio_to_USB(int16_t * audioData, uint16_t PCMSamples);


#endif /* __USBD_AUDIO_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
