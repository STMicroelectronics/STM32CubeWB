/**
  ******************************************************************************
  * @file    record.c 
  * @author  MCD Application Team
  * @brief   Audio application. 
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AUDIO_OUT_MS                                    (20)
#define AUDIO_OUT_SAMPLING_FREQUENCY                    16000

/* Private macro -------------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Exported_Variables 
  * @{
  */
USBD_HandleTypeDef hUSBDDevice;
extern USBD_AUDIO_ItfTypeDef  USBD_AUDIO_fops;
uint16_t PDM_Buffer[((((2 * AUDIO_IN_CHANNELS * AUDIO_IN_SAMPLING_FREQUENCY) / 1000) * MAX_DECIMATION_FACTOR) / 16)* N_MS_PER_INTERRUPT ];
uint16_t PCM_Buffer[((AUDIO_IN_SAMPLING_FREQUENCY)/1000)  * N_MS_PER_INTERRUPT ];
BSP_AUDIO_Init_t MicParams;
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Private_Variables 
  * @{
  */
/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Exported_Function 
  * @{
  */
static void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
static void Start_Acquisition(void);
static void AudioProcess(void);

/**
  * @brief  Audio Record Demo.
  */
void Audio_Record_Demo(void)
{
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(0);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Record ", CENTER_MODE);
  UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, 15, (uint8_t *)"Open Voice Recorder", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 27, (uint8_t *)"Application", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"Press user button1", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 52, (uint8_t *)"to skip audio demo", CENTER_MODE);
  BSP_LCD_Refresh(0);
  
  /* Initialize USB descriptor basing on channels number and sampling frequency */
  USBD_AUDIO_Init_Microphone_Descriptor(&hUSBDDevice, AUDIO_OUT_SAMPLING_FREQUENCY, AUDIO_CHANNELS_OUT);
  /* Init Device Library */
  USBD_Init(&hUSBDDevice, &AUDIO_Desc, 0);
  /* Add Supported Class */
  USBD_RegisterClass(&hUSBDDevice, &USBD_AUDIO);
  /* Add Interface callbacks for AUDIO Class */  
  USBD_AUDIO_RegisterInterface(&hUSBDDevice, &USBD_AUDIO_fops);
  /* Start Device Process */
  USBD_Start(&hUSBDDevice);
  
  /* Start audio acquisition and streaming */  
#ifdef DISABLE_USB_DRIVEN_ACQUISITION
  Init_Acquisition_Peripherals(AUDIO_IN_SAMPLING_FREQUENCY, AUDIO_IN_CHANNELS, 0);
  Start_Acquisition();
#endif
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_IN_Stop(BSP_AUDIO_INSTANCE);
      USBD_DeInit(&hUSBDDevice);
      BSP_AUDIO_IN_DeInit(BSP_AUDIO_INSTANCE);
      
      return;
    }
  }
}

/**
  * @brief  Half Transfer user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{  
  AudioProcess();
}

/**
  * @brief  Transfer Complete user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{  
  AudioProcess();
}

/**
  * @brief  User function that is called when 1 ms of PDM data is available.
  * 		  In this application only PDM to PCM conversion and USB streaming
  *                  is performed.
  * 		  User can add his own code here to perform some DSP or audio analysis.
  * @param  none
  * @retval None
  */
static void AudioProcess(void)
{
  BSP_AUDIO_IN_PDMToPCM(BSP_AUDIO_INSTANCE, (uint16_t * )PDM_Buffer, (uint16_t*)PCM_Buffer);
  Send_Audio_to_USB((int16_t *)PCM_Buffer, ((AUDIO_IN_SAMPLING_FREQUENCY/1000)*N_MS_PER_INTERRUPT)); 
}

/**
  * @brief  User function that is called when 1 ms of PDM data is available.
  * 		  In this application only PDM to PCM conversion and USB streaming
  *                  is performed.
  * 		  User can add his own code here to perform some DSP or audio analysis.
  * @param  none
  * @retval None
  */
static void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{  
  MicParams.BitsPerSample = 16;
  MicParams.ChannelsNbr = ChnlNbrIn;
  MicParams.Device = AUDIO_IN_DIGITAL_MIC;
  MicParams.SampleRate = AudioFreq;
  MicParams.Volume = AUDIO_VOLUME_INPUT;
  
  if (BSP_AUDIO_IN_Init(BSP_AUDIO_INSTANCE, &MicParams)!=BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  User function that is called when 1 ms of PDM data is available.
  * 		  In this application only PDM to PCM conversion and USB streaming
  *                  is performed.
  * 		  User can add his own code here to perform some DSP or audio analysis.
  * @param  none
  * @retval None
  */
static void Start_Acquisition(void)
{  
  if (BSP_AUDIO_IN_Record(BSP_AUDIO_INSTANCE, (uint8_t *) PDM_Buffer, AUDIO_IN_BUFFER_SIZE)!=BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @}
  */
