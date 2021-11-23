/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_audio.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32wb5mm_dk_audio.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB5MM_DK_AUDIO_H
#define STM32WB5MM_DK_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include <stdlib.h>
#include "pdm2pcm_glo.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */ 
  
/** @defgroup STM32WB5MM_DK_AUDIO STM32WB5MM_DK AUDIO
  * @{
  */

/** @defgroup STM32WB5MM_DK_AUDIO_Exported_Variables STM32WB5MM_DK AUDIO Exported Variables
  * @{
  */
extern SAI_HandleTypeDef            hAudioInSai;

typedef struct
{
  uint32_t                    Device;
  uint32_t                    SampleRate;
  uint32_t                    BitsPerSample;
  uint32_t                    ChannelsNbr;
  uint32_t                    Volume;
}BSP_AUDIO_Init_t;

typedef struct
{
  uint32_t                    Instance;            /* Audio IN instance              */
  uint32_t                    Device;              /* Audio IN device to be used     */
  uint32_t                    SampleRate;          /* Audio IN Sample rate           */
  uint32_t                    BitsPerSample;       /* Audio IN Sample resolution     */
  uint32_t                    ChannelsNbr;         /* Audio IN number of channel     */
  uint16_t                    *pBuff;              /* Audio IN record buffer         */
  uint32_t                    Size;                /* Audio IN record buffer size    */
  uint32_t                    Volume;              /* Audio IN volume                */
  uint32_t                    State;               /* Audio IN State                 */
  uint32_t                    IsMspCallbacksValid; /* Is Msp Callbacks registered     */
  uint32_t DecimationFactor;
}AUDIO_IN_Ctx_t;

#if ((USE_HAL_SAI_REGISTER_CALLBACKS == 1))
typedef struct
{
  pSAI_CallbackTypeDef            pMspSaiInitCb;
  pSAI_CallbackTypeDef            pMspSaiDeInitCb;
}BSP_AUDIO_IN_Cb_t;
#endif /* ((USE_HAL_SAI_REGISTER_CALLBACKS == 1)) */

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_AUDIO_Exported_Constants STM32WB5MM_DK AUDIO Exported Constants
  * @{
  */
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
#define HTONS(A)  ((((A) & (uint16_t)0xff00) >> 8) | (((A) & (uint16_t)0x00ff) << 8))

/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K      (uint32_t)192000U
#define AUDIO_FREQUENCY_176K      (uint32_t)176400U
#define AUDIO_FREQUENCY_96K       (uint32_t)96000U
#define AUDIO_FREQUENCY_88K       (uint32_t)88200U
#define AUDIO_FREQUENCY_48K       (uint32_t)48000U
#define AUDIO_FREQUENCY_44K       (uint32_t)44100U
#define AUDIO_FREQUENCY_32K       (uint32_t)32000U
#define AUDIO_FREQUENCY_22K       (uint32_t)22050U
#define AUDIO_FREQUENCY_16K       (uint32_t)16000U
#define AUDIO_FREQUENCY_11K       (uint32_t)11025U
#define AUDIO_FREQUENCY_8K        (uint32_t)8000U 

/* AUDIO RESOLUTION */
#define AUDIO_RESOLUTION_16b                16U
#define AUDIO_RESOLUTION_24b                24U
#define AUDIO_RESOLUTION_32b                32U

/*------------------------------------------------------------------------------
                        AUDIO IN defines parameters
------------------------------------------------------------------------------*/

/* SAI Configuration defines */
#define AUDIO_IN_SAI_INSTANCE                                    SAI1_Block_A
#define AUDIO_IN_SAI_CLK_ENABLE()                                __SAI1_CLK_ENABLE()
#define AUDIO_IN_SAI_SCK_PIN                                     GPIO_PIN_8
#define AUDIO_IN_SAI_SCK_GPIO_PORT                               GPIOA
#define AUDIO_IN_SAI_SCK_GPIO_CLK_ENABLE()                       __GPIOA_CLK_ENABLE()
#define AUDIO_IN_SAI_SCK_AF                                      GPIO_AF3_SAI1

#define AUDIO_IN_SAI_SD2_PIN                                    GPIO_PIN_9
#define AUDIO_IN_SAI_SD2_GPIO_PORT                              GPIOA
#define AUDIO_IN_SAI_SD2_GPIO_CLK_ENABLE()                      __GPIOA_CLK_ENABLE();
#define AUDIO_IN_SAI_SD2_AF                                     GPIO_AF3_SAI1

/* SAI DMA definitions */
#define AUDIO_IN_SAI_DMAx_CLK_ENABLE()                          __DMA1_CLK_ENABLE()
#define AUDIO_IN_SAI_DMAx_PERIPH_DATA_SIZE                      DMA_PDATAALIGN_BYTE
#define AUDIO_IN_SAI_DMAx_MEM_DATA_SIZE                         DMA_MDATAALIGN_BYTE

/* Audio In devices */
#ifndef AUDIO_IN_CHANNELS
#define AUDIO_IN_CHANNELS 1
#endif
#ifndef AUDIO_IN_SAMPLING_FREQUENCY
#define AUDIO_IN_SAMPLING_FREQUENCY 16000
#endif
#define AUDIO_IN_BIT_DEPTH              AUDIO_RESOLUTION_16b
#define AUDIO_VOLUME_INPUT              64U
#define BSP_AUDIO_INSTANCE              0U
#define BSP_AUDIO_IN_IT_PRIORITY        6U
#define AUDIO_IN_DIGITAL_MIC            0x10U
#define CHANNEL_DEMUX_MASK              0x55U

#define ENABLE_HIGH_PERFORMANCE_MODE    0U
#if (ENABLE_HIGH_PERFORMANCE_MODE == 1U)
#define PDM_FREQ_16K                    1536
#endif

#ifndef PDM_FREQ_16K
#define PDM_FREQ_16K                    1280
#endif

/*Number of millisecond of audio at each DMA interrupt*/
#define N_MS_PER_INTERRUPT               (20U)

/* Default Audio IN internal buffer size */   
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        (uint32_t)((AUDIO_IN_SAMPLING_FREQUENCY/1000)*2)*N_MS_PER_INTERRUPT    

/* Audio In states */
#define AUDIO_IN_STATE_RESET               0U
#define AUDIO_IN_STATE_RECORDING           1U
#define AUDIO_IN_STATE_STOP                2U
#define AUDIO_IN_STATE_PAUSE               3U

/* Audio In instances number:
   Instance 0 is SAI
 */
#define AUDIO_IN_INSTANCES_NBR             1U
/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_AUDIO_Exported_Variables
  * @{
  */
/* Recording context */
extern AUDIO_IN_Ctx_t                         AudioInCtx[];
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_AUDIO_IN_Exported_Functions STM32WB5MM_DK AUDIO IN Exported Functions
  * @{
  */
int32_t BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit);
int32_t BSP_AUDIO_IN_DeInit(uint32_t Instance);
#if ((USE_HAL_SAI_REGISTER_CALLBACKS == 1U))
int32_t BSP_AUDIO_IN_RegisterDefaultMspCallbacks (uint32_t Instance);
int32_t BSP_AUDIO_IN_RegisterMspCallbacks (uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks);
#endif /* ((USE_HAL_SAI_REGISTER_CALLBACKS == 1U)) */
int32_t BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes);
int32_t BSP_AUDIO_IN_Stop(uint32_t Instance);
int32_t BSP_AUDIO_IN_Pause(uint32_t Instance);
int32_t BSP_AUDIO_IN_Resume(uint32_t Instance);

int32_t BSP_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes);
int32_t BSP_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device);
int32_t BSP_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device);
int32_t BSP_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device);

int32_t BSP_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device);
int32_t BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t BSP_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);
int32_t BSP_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);
int32_t BSP_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State);

/* Specific PDM recodr APIs */
int32_t BSP_AUDIO_IN_PDMToPCM_Init(uint32_t Instance, uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
int32_t BSP_AUDIO_IN_PDMToPCM(uint32_t Instance, uint16_t *PDMBuf, uint16_t *PCMBuf);

void BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance);

/**
  * @}
  */

/** @addtogroup BSP_AUDIO_IN_Private_Functions
  * @{
  */
HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hSai);
HAL_StatusTypeDef MX_SAI1_Block_A_Init(SAI_HandleTypeDef* hsai,uint32_t PDM_FREQ);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32WB5MM_DK_AUDIO_H */
