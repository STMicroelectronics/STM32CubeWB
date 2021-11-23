/**
  ******************************************************************************
  * @file    usbd_audio_if.h
  * @author  MCD Application Team
  * @brief   Header for usbd_audio_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
