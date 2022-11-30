/**
  ******************************************************************************
  * @file    usbd_audio_if.c
  * @author  MCD Application Team
  * @brief   USB Device Audio interface file.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio_if.h"
#include "stm32wb5mm_dk_errno.h"

extern uint16_t PDM_Buffer[];
extern BSP_AUDIO_Init_t MicParams;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t Audio_Init(uint32_t  AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
static int8_t Audio_DeInit(uint32_t options);
static int8_t Audio_Record(void);
static int8_t Audio_VolumeCtl(int16_t Volume);
static int8_t Audio_MuteCtl(uint8_t cmd);
static int8_t Audio_Stop(void);
static int8_t Audio_Pause(void);
static int8_t Audio_Resume(void);
static int8_t Audio_CommandMgr(uint8_t cmd);

/* Private variables ---------------------------------------------------------*/
extern USBD_HandleTypeDef hUSBDDevice;
USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops = {
  Audio_Init,
  Audio_DeInit,
  Audio_Record,
  Audio_VolumeCtl,
  Audio_MuteCtl,
  Audio_Stop,
  Audio_Pause,
  Audio_Resume,
  Audio_CommandMgr,
};


/* Private functions ---------------------------------------------------------*/
/* This table maps the audio device class setting in 1/256 dB to a
* linear 0-64 scaling used in pdm_filter.c. It is computed as
* 256*20*log10(index/64). */
const int16_t vol_table[65] =
{ 0x8000, 0xDBE0, 0xE1E6, 0xE56B, 0xE7EB, 0xE9DB, 0xEB70, 0xECC7,
0xEDF0, 0xEEF6, 0xEFE0, 0xF0B4, 0xF176, 0xF228, 0xF2CD, 0xF366,
0xF3F5, 0xF47C, 0xF4FB, 0xF574, 0xF5E6, 0xF652, 0xF6BA, 0xF71C,
0xF778, 0xF7D6, 0xF82D, 0xF881, 0xF8D2, 0xF920, 0xF96B, 0xF9B4,
0xF9FB, 0xFA3F, 0xFA82, 0xFAC2, 0xFB01, 0xFB3E, 0xFB79, 0xFBB3,
0xFBEB, 0xFC22, 0xFC57, 0xFC8C, 0xFCBF, 0xFCF1, 0xFD22, 0xFD51,
0xFD80, 0xFDAE, 0xFDDB, 0xFE07, 0xFE32, 0xFE5D, 0xFE86, 0xFEAF,
0xFED7, 0xFF00, 0xFF25, 0xFF4B, 0xFF70, 0xFF95, 0xFFB9, 0xFFD0,
0x0000 };


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the AUDIO media low layer.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @param  BitRes: desired bit resolution
  * @param  ChnlNbr: number of channel to be configured
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_Init(uint32_t  AudioFreq, uint32_t BitRes, uint32_t ChnlNbr)
{
#ifndef DISABLE_USB_DRIVEN_ACQUISITION 
  return BSP_AUDIO_IN_Init(BSP_AUDIO_INSTANCE, &MicParams);
#else
  return BSP_ERROR_NONE;
#endif
}

/**
  * @brief  De-Initializes the AUDIO media low layer.      
  * @param  options: Reserved for future use
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_DeInit(uint32_t options)
{
  return BSP_ERROR_NONE;
}

/**
  * @brief  Start audio recording engine
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_Record(void)
{
#ifndef DISABLE_USB_DRIVEN_ACQUISITION  
  return BSP_AUDIO_IN_Record(BSP_AUDIO_INSTANCE, (uint8_t *) PDM_Buffer, 0);
#else
  return BSP_ERROR_NONE;
#endif
}

/**
  * @brief  Controls AUDIO Volume.
  * @param  vol: Volume level
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_VolumeCtl(int16_t Volume)
{
  /* Call low layer volume setting function */
  uint32_t j, mic_instance;

  j = 0;
  /* Find the setting nearest to the desired setting */
  while(j<64 &&
        abs(Volume-vol_table[j]) > abs(Volume-vol_table[j+1])) {
          j++;
        }
  mic_instance = 0;
  /* Now do the volume adjustment */
  return BSP_AUDIO_IN_SetVolume(mic_instance, j);  
}

/**
  * @brief  Controls AUDIO Mute.
  * @param  cmd: Command opcode
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_MuteCtl(uint8_t cmd)
{
  return BSP_ERROR_NONE;
}

/**
  * @brief  Stops audio acquisition
  * @param  none
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_Stop(void)
{
#ifndef DISABLE_USB_DRIVEN_ACQUISITION
  return BSP_AUDIO_IN_Stop(BSP_AUDIO_INSTANCE);
#else
  return BSP_ERROR_NONE;
#endif
}

/**
  * @brief  Pauses audio acquisition
  * @param  none
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_Pause(void)
{
  return 0;
}

/**
  * @brief  Resumes audio acquisition
  * @param  none
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_Resume(void)
{
  return BSP_ERROR_NONE;
}

/**
  * @brief  Manages command from usb
  * @param  None
  * @retval BSP_ERROR_NONE in case of success, AUDIO_ERROR otherwise
  */
static int8_t Audio_CommandMgr(uint8_t cmd)
{
  return BSP_ERROR_NONE;
}

/**
  * @brief  Fills USB audio buffer with the right amount of data, depending on the
  *         channel/frequency configuration
  * @param  audioData: pointer to the PCM audio data
  * @param  PCMSamples: number of PCM samples to be passed to USB engine
  * @note Depending on the calling frequency, a coherent amount of samples must be passed to
  *       the function. E.g.: assuming a Sampling frequency of 16 KHz and 1 channel,
  *       you can pass 16 PCM samples if the function is called each millisecond,
  *       32 samples if called every 2 milliseconds and so on.
  */
void Send_Audio_to_USB(int16_t * audioData, uint16_t PCMSamples)
{
  USBD_AUDIO_Data_Transfer(&hUSBDDevice, (int16_t *)audioData, PCMSamples);
}
