/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio driver for the STM32WB5MM-DK
  *          discovery board.
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
  @verbatim
  How To use this driver:
  -----------------------
   + This driver supports STM32WBxx devices on STM32WB5MM-DK board.
   + Call the function BSP_AUDIO_IN_Init() for AUDIO IN initialization: 
        Instance : Select the input instance. Can be 0 (SAI).
        AudioInit: Audio In structure to select the following parameters.
                   - Device: Select the input device (digital mic).
                   - SampleRate: Select the input sample rate (8Khz .. 96Khz).
                   - BitsPerSample: Select the input resolution (16 or 32bits per sample).
                   - ChannelsNbr: Select the input channels number(1 for mono).
                   - Volume: Select the input volume(0% .. 100%).

      This function configures all the hardware required for the audio application (SAI, GPIOs, DMA and interrupt if needed).
      This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication has failed 
      (try to un-plug the power or reset device in this case).

      User can update the SAI or the clock configurations by overriding the weak MX functions MX_SAI1_Block_A_Init(), MX_SAI_ClockConfig().
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_IN_RegisterMspCallbacks() function.
      User can restore the default MSP configuration by calling BSP_AUDIO_IN_RegisterDefaultMspCallbacks().
      To use these two functions, user have to enable USE_HAL_SAI_REGISTER_CALLBACKS within stm32wb55xx_hal_conf.h file.

   + Call the function BSP_AUDIO_IN_Record() to record audio stream. The recorded data are stored to user buffer.
        Instance : Select the input instance. Can be 0 (SAI).
        pBuf: pointer to user buffer.
        NbrOfBytes: Total size of the buffer to be sent in Bytes.

   + Call the function BSP_AUDIO_IN_Pause() to pause recording.
   + Call the function BSP_AUDIO_IN_Resume() to resume recording.
   + Call the function BSP_AUDIO_IN_Stop() to stop recording.
   + Call the function BSP_AUDIO_IN_GetDevice() to get the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_GetSampleRate() to get the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_GetBitPerSample() to get the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_GetChannelsNbr() to get the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_SetVolume() to update the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetVolume() to get the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetState() to get the AUDIO IN state.

   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_IN_XXX_CallBack() and only their prototypes are declared in
      the stm32wb5mm_dk_audio.h file (refer to the example for more details on the callbacks implementations).

   + The driver API and the callback functions are at the end of the stm32wb5mm_dk_audio.h file.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_audio.h"
#include "stm32wb5mm_dk_errno.h"
#include "arm_math.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */ 
  
/** @defgroup STM32WB5MM_DK_AUDIO STM32WB5MM_DK AUDIO
  * @{
  */ 

/** @defgroup STM32WB5MM_DK_AUDIO_Private_Macros STM32WB5MM_DK AUDIO Private Macros
  * @{
  */


/**
  * @}
  */
  
/** @defgroup STM32WB5MM_DK_AUDIO_Exported_Variables STM32WB5MM_DK AUDIO Exported Variables
  * @{
  */
/* Recording context */
AUDIO_IN_Ctx_t                         AudioInCtx[AUDIO_IN_INSTANCES_NBR] = {0};

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_AUDIO_Private_Variables STM32WB5MM_DK AUDIO Private Variables
  * @{
  */
#define DECIMATOR_NUM_TAPS (16)
#define DECIMATOR_BLOCK_SIZE (16U*N_MS_PER_INTERRUPT)
#define DECIMATOR_FACTOR 2
#define DECIMATOR_STATE_LENGTH (DECIMATOR_BLOCK_SIZE + (DECIMATOR_NUM_TAPS) -1)
static arm_fir_decimate_instance_q15 ARM_Decimator_State[MAX_AUDIO_IN_CHANNEL_NBR_TOTAL];

/* PDM filters params */
static PDM_Filter_Handler_t  PDM_FilterHandler[MAX_AUDIO_IN_CHANNEL_NBR_TOTAL];
static PDM_Filter_Config_t   PDM_FilterConfig[MAX_AUDIO_IN_CHANNEL_NBR_TOTAL];

SAI_HandleTypeDef            hAudioInSai;
#define PDM_INTERNAL_BUFFER_SIZE_SAI          ((MAX_MIC_FREQ / 8) * MAX_AUDIO_IN_CHANNEL_NBR_TOTAL * N_MS_PER_INTERRUPT)
static uint16_t SAI_InternalBuffer[PDM_INTERNAL_BUFFER_SIZE_SAI];

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_AUDIO_Private_Functions STM32WB5MM_DK AUDIO Private Functions
  * @{
  */
static void SAI_MspInit(SAI_HandleTypeDef *hsai);

/**
  * @}
  */ 

/** @defgroup STM32WB5MM_DK_AUDIO_IN_Exported_Functions STM32WB5MM_DK AUDIO IN Exported Functions
  * @{
  */ 

/**
  * @brief  Initialize wave recording.
  * @param  Instance  AUDIO IN Instance.
  * @param  AudioInit Init structure
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit)
{  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(AudioInit->ChannelsNbr > MAX_AUDIO_IN_CHANNEL_NBR_TOTAL)
    {
      return BSP_ERROR_WRONG_PARAM;
    }
    /* Store the audio record context */
    AudioInCtx[Instance].Device          = AudioInit->Device;
    AudioInCtx[Instance].ChannelsNbr     = AudioInit->ChannelsNbr;
    AudioInCtx[Instance].SampleRate      = AudioInit->SampleRate; 
    AudioInCtx[Instance].BitsPerSample   = AudioInit->BitsPerSample;
    AudioInCtx[Instance].Volume          = AudioInit->Volume;
    AudioInCtx[Instance].State           = AUDIO_IN_STATE_RESET;

    if(Instance == 0U)
    {
      uint32_t PDM_Clock_Freq;     
      switch (AudioInit->SampleRate)
      {
      case AUDIO_FREQUENCY_8K:
        PDM_Clock_Freq = 1280;
        break;

      case AUDIO_FREQUENCY_16K:
        PDM_Clock_Freq = PDM_FREQ_16K;
        break;

      case AUDIO_FREQUENCY_32K:
        PDM_Clock_Freq = 2048;
        break;

      case AUDIO_FREQUENCY_48K:
        PDM_Clock_Freq = 3072;
        break;

      default:
        PDM_Clock_Freq = 0;
        break;
      }
      if (PDM_Clock_Freq == 0)
      {
        return BSP_ERROR_WRONG_PARAM;
      }
      AudioInCtx[Instance].DecimationFactor = (PDM_Clock_Freq * 1000U)/AudioInit->SampleRate;
      AudioInCtx[Instance].Size = (PDM_Clock_Freq/8U) * 2U * N_MS_PER_INTERRUPT;
      /* Initialize SAI */
      __HAL_SAI_RESET_HANDLE_STATE(&hAudioInSai);
      /* PLL clock is set depending by the AudioFreq */ 
      if(MX_SAI_ClockConfig(&hAudioInSai) != HAL_OK)
      {
        return  BSP_ERROR_CLOCK_FAILURE;
      }
      if(HAL_SAI_GetState(&hAudioInSai) == HAL_SAI_STATE_RESET)
      {
        SAI_MspInit(&hAudioInSai);
      }
      hAudioInSai.Instance = AUDIO_IN_SAI_INSTANCE;
      if(MX_SAI1_Block_A_Init(&hAudioInSai,PDM_Clock_Freq) != HAL_OK)
      {
        return  BSP_ERROR_PERIPH_FAILURE;
      }
      if (BSP_AUDIO_IN_PDMToPCM_Init(Instance, AudioInCtx[0].SampleRate, AudioInCtx[0].ChannelsNbr, AudioInCtx[0].ChannelsNbr)!= BSP_ERROR_NONE)
      {
        return  BSP_ERROR_NO_INIT;
      }
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_STOP;
    /* Return BSP status */ 
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Deinit the audio IN peripherals.
  * @param  Instance  AUDIO IN Instance.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_DeInit(uint32_t Instance)
{  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Instance == 0U)
    {
      /* Initialize the hAudioInSai Instance parameter */
      hAudioInSai.Instance = AUDIO_IN_SAI_INSTANCE;
      if(HAL_SAI_DeInit(&hAudioInSai) != HAL_OK)
      {
        return BSP_ERROR_PERIPH_FAILURE;
      }
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_RESET;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1U)
/**
* @brief BSP AUDIO In Filter Msp Callback registering
* @param Instance    AUDIO IN Instance
* @param CallBacks   pointer to filter MspInit/MspDeInit functions
* @retval BSP status
*/
int32_t BSP_AUDIO_IN_RegisterMspCallbacks (uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else 
  {  
    if(Instance != 0U)
    {
      return BSP_ERROR_WRONG_PARAM;
    }
    else /* (Instance == 0U) */
    {
      if(HAL_SAI_RegisterCallback(&hAudioInSai, HAL_SAI_MSPINIT_CB_ID, CallBacks->pMspSaiInitCb) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else if(HAL_SAI_RegisterCallback(&hAudioInSai, HAL_SAI_MSPDEINIT_CB_ID, CallBacks->pMspSaiDeInitCb) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      { 
      }
    }
    if(ret == BSP_ERROR_NONE)
    {
      AudioInCtx[Instance].IsMspCallbacksValid = 1;
    }
  }
  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1U) */

/**
  * @brief  Initialize the PDM library.
  * @param Instance    AUDIO IN Instance
  * @param  AudioFreq  Audio sampling frequency
  * @param  ChnlNbrIn  Number of input audio channels in the PDM buffer
  * @param  ChnlNbrOut Number of desired output audio channels in the  resulting PCM buffer
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_PDMToPCM_Init(uint32_t Instance, uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
  if(Instance != 0U)
  {
    return  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t index;    
    static int16_t aState_ARM[MAX_AUDIO_IN_CHANNEL_NBR_TOTAL][DECIMATOR_STATE_LENGTH];
    static int16_t aCoeffs[] = { -1406, 1634, -1943, 2386, -3080, 4325, -7223, 21690, 21690, -7223, 4325, -3080, 2386, -1943, 1634, -1406, };

    /* Enable CRC peripheral to unlock the PDM library */
    __HAL_RCC_CRC_CLK_ENABLE();

    for(index = 0; index < ChnlNbrIn; index++)
    {
      volatile uint32_t error = 0;
      /* Init PDM filters */
      PDM_FilterHandler[index].bit_order  = PDM_FILTER_BIT_ORDER_LSB;
      PDM_FilterHandler[index].endianness = PDM_FILTER_ENDIANNESS_LE;
      PDM_FilterHandler[index].high_pass_tap = 2122358088;
      PDM_FilterHandler[index].out_ptr_channels = (uint16_t)ChnlNbrOut;
      PDM_FilterHandler[index].in_ptr_channels  = (uint16_t)ChnlNbrIn;

      /* PDM lib config phase */
      PDM_FilterConfig[index].output_samples_number = (uint16_t) ((AudioFreq/1000U) * N_MS_PER_INTERRUPT);
      PDM_FilterConfig[index].mic_gain = 24;

      switch (AudioInCtx[0].DecimationFactor)
      {
      case 16:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_16;
        break;
      case 24:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_24;
        break;
      case 32:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_32;
        break;
      case 48:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_48;
        break;
      case 64:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_64;
        break;
      case 80:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        break;
      case 128:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_128;
        break;
      case 160:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        PDM_FilterConfig[index].output_samples_number *= 2U;        
        PDM_FilterHandler[index].out_ptr_channels = 1;
        (void)arm_fir_decimate_init_q15  (&ARM_Decimator_State[index], DECIMATOR_NUM_TAPS, DECIMATOR_FACTOR,
                                          aCoeffs, aState_ARM[index], DECIMATOR_BLOCK_SIZE);
         break;
      default:
        break;
      }
      error = PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM_FilterHandler[index]));
      if (error!=0U)
      {
        return  BSP_ERROR_NO_INIT;
      }
      error = PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler[index], &PDM_FilterConfig[index]);
      if (error!=0U)
      {
        return  BSP_ERROR_NO_INIT;
      }
    }
  } 
  return BSP_ERROR_NONE;
}

/**
  * @brief  Converts  audio format from PDM to PCM.
  * @param  Instance  AUDIO IN Instance  
  * @param  PDMBuf    Pointer to PDM buffer data
  * @param  PCMBuf    Pointer to PCM buffer data
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_PDMToPCM(uint32_t Instance, uint16_t *PDMBuf, uint16_t *PCMBuf)
{
  if(Instance != 0U)
  {
    return  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t index;
    
    for(index = 0; index < AudioInCtx[Instance].ChannelsNbr; index++)
    {
     
      if (AudioInCtx[Instance].SampleRate == 8000U)
      {
        uint16_t Decimate_Out[8*N_MS_PER_INTERRUPT];
        uint32_t ii;
        uint16_t PDM_Filter_Out[16*N_MS_PER_INTERRUPT];
        
        (void)PDM_Filter(&((uint8_t*)(PDMBuf))[index], PDM_Filter_Out, &PDM_FilterHandler[index]);
        (void)arm_fir_decimate_q15 (&ARM_Decimator_State[index], (q15_t *)&(PDM_Filter_Out), (q15_t*)&(Decimate_Out), DECIMATOR_BLOCK_SIZE);
        for (ii=0; ii<(8U*N_MS_PER_INTERRUPT); ii++)
        {
          PCMBuf[(ii * AudioInCtx[Instance].ChannelsNbr) + index] = Decimate_Out[ii];
        }
      }
      else
      {
        (void)PDM_Filter(&((uint8_t*)(PDMBuf))[index], (uint16_t*)&(PCMBuf[index]), &PDM_FilterHandler[index]);
      }
    }
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Start audio recording.
  * @param  Instance  AUDIO IN Instance.
  * @param  pBuf  Main buffer pointer for the recorded data storing
  * @param  NbrOfBytes  Size of the record buffer.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes)
{
  if(Instance >= (AUDIO_IN_INSTANCES_NBR) )
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    AudioInCtx[Instance].pBuff = (uint16_t*)pBuf;
    if(Instance == 0U)
    {
      UNUSED(NbrOfBytes);
      if(HAL_SAI_Receive_DMA(&hAudioInSai, (uint8_t *)SAI_InternalBuffer, (uint16_t)(AudioInCtx[Instance].Size)) != HAL_OK)
      {
        return BSP_ERROR_PERIPH_FAILURE;
      }
      /* Update BSP AUDIO IN state */
      AudioInCtx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Stop audio recording.
  * @param  Instance  AUDIO IN Instance.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Stop(uint32_t Instance)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
  else
  {
    if(Instance == 0U)
    {
      if(HAL_SAI_DMAStop(&hAudioInSai) != HAL_OK)
      {
        return BSP_ERROR_PERIPH_FAILURE;
      }
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_STOP;
  } 
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Pause the audio file stream.
  * @param  Instance  AUDIO IN Instance.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Pause(uint32_t Instance)
{
  __HAL_SAI_DISABLE(&hAudioInSai);
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Instance == 0U)
    {
      if(HAL_SAI_DMAPause(&hAudioInSai)!= HAL_OK)
      {
        return BSP_ERROR_WRONG_PARAM;
      }
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_PAUSE;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Resume the audio file stream.
  * @param  Instance  AUDIO IN Instance.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Resume(uint32_t Instance)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
  else 
  {
    if(Instance == 0U)
    { 
      if(HAL_SAI_DMAResume(&hAudioInSai)!= HAL_OK)
      {
        return BSP_ERROR_WRONG_PARAM;
      }
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_RECORDING;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance.
  * @param  Device    The audio input device used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio Input Device */
    *Device = AudioInCtx[Instance].Device;
  }
  return BSP_ERROR_NONE;
}


/**
  * @brief  Get Audio In frequency
  * @param  Instance  AUDIO IN Instance.
  * @param  SampleRate  Audio Input frequency to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in frequency */
    *SampleRate = AudioInCtx[Instance].SampleRate;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get Audio In Resolution
  * @param  Instance  AUDIO IN Instance.
  * @param  BitsPerSample  Input resolution to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in resolution */
    *BitsPerSample = AudioInCtx[Instance].BitsPerSample;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get Audio In Channel number
  * @param  Instance  AUDIO IN Instance.
  * @param  ChannelNbr  Channel number to be used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Channel number to be returned */
    *ChannelNbr = AudioInCtx[Instance].ChannelsNbr;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the current audio in volume level.
  * @param  Instance  AUDIO IN Instance.
  * @param  Volume    Volume level to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else if (Instance == 0U)
  {
    uint32_t index;
    static int16_t VolumeGain[] = 
    {
      -12,-12,-6,-3,0,2,3,5,6,7,8,9,9,10,11,11,12,12,13,13,14,14,15,15,15,
      16,16,17,17,17,17,18,18,18,19,19,19,19,19,20,20,20,20,21,21,21,21,21,
      22,22,22,22,22,22,23,23,23,23,23,23,23,24,24,24,24,24,24,24,25,25,25,
      25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,
      27,27,28,28,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,
      29,29,29,29,30,30,30,30,30,30,30,31  
    };
    for (index = 0; index < AudioInCtx[Instance].ChannelsNbr; index++)
    {
      if (PDM_FilterConfig[index].mic_gain != VolumeGain[Volume])
      {
        PDM_FilterConfig[index].mic_gain = VolumeGain[Volume];
        (void)PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler[index], &PDM_FilterConfig[index]);
      }
    }
  }
  else
  {
    /* Update AudioIn Context */
    AudioInCtx[Instance].Volume = Volume;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;  
}

/**
  * @brief  Get the current audio in volume level.
  * @param  Instance  AUDIO IN Instance.
  * @param  Volume    Volume level to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Input Volume to be returned */
    *Volume = AudioInCtx[Instance].Volume;
  }
  /* Return BSP status */
  return BSP_ERROR_NONE;  
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance.
  * @param  State     Audio Out state
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Input State to be returned */
    *State = AudioInCtx[Instance].State;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  BSP AUDIO IN interrupt handler.
  * @param  Instance Audio in instance.
  * @param  Device Device of the audio in stream.
  * @retval None.
  */
void BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into
  byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is
  written into the buffer that the user indicates when calling the BSP_AUDIO_IN_Start(...) function.
  * @param hSai: SAI handle. Not used
  * @retval None
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hSai)
{
  UNUSED(hSai);
  uint8_t * DataTempSAI = &(((uint8_t *)SAI_InternalBuffer)[AudioInCtx[0].Size/2U]);

  memcpy(AudioInCtx[0].pBuff, DataTempSAI, AudioInCtx[0].Size/2U);
  BSP_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into 
  byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is 
  written into the buffer that the user indicates when calling the BSP_AUDIO_IN_Start(...) function.
  * @param hSai: SAI handle. Not used
  * @retval None
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hSai)
{
  UNUSED(hSai);
  uint8_t * DataTempSAI = (uint8_t *)SAI_InternalBuffer;

  memcpy(AudioInCtx[0].pBuff, DataTempSAI, AudioInCtx[0].Size/2U);
  BSP_AUDIO_IN_HalfTransfer_CallBack(0);
}

/**
  * @brief  User callback when record buffer is filled.
  * @retval None
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @retval None
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Audio IN Error callback function.
  * @retval None
  */
__weak void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{ 
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function is called when an Interrupt due to transfer error on or peripheral
  error occurs. */
}

/**
  * @}
  */
  
/** @defgroup BSP_AUDIO_IN_Private_Functions STM32WB5MM_DK AUDIO IN Private Functions
  * @{
  */

/*******************************************************************************
Static Functions
*******************************************************************************/

/**
  * @brief AUDIO IN SAI MSP Init
  * @param hsai SAI handle
  * @retval None
  */
static void SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  static DMA_HandleTypeDef hSaiDma;
  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the SAI peripheral clock */
  AUDIO_IN_SAI_CLK_ENABLE();  
  /* Enable SAI GPIO clocks */
  AUDIO_IN_SAI_SCK_GPIO_CLK_ENABLE();
  AUDIO_IN_SAI_SD2_GPIO_CLK_ENABLE();  
  
  /* SAI pins configuration: SCK and SD pins ------------------------------*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_InitStruct.Pin       = AUDIO_IN_SAI_SCK_PIN;
  GPIO_InitStruct.Alternate = AUDIO_IN_SAI_SCK_AF;
  HAL_GPIO_Init(AUDIO_IN_SAI_SCK_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Pin       = AUDIO_IN_SAI_SD2_PIN ;
  GPIO_InitStruct.Alternate = AUDIO_IN_SAI_SD2_AF;
  HAL_GPIO_Init(AUDIO_IN_SAI_SD2_GPIO_PORT, &GPIO_InitStruct);
  
  /* Enable the DMA clock */
  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();  
  
  /* Configure the hSaiDma handle parameters */
  hSaiDma.Instance                 = DMA1_Channel1;
  hSaiDma.Init.Request             = DMA_REQUEST_SAI1_A;
  hSaiDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
  hSaiDma.Init.PeriphDataAlignment = AUDIO_IN_SAI_DMAx_PERIPH_DATA_SIZE;
  hSaiDma.Init.MemDataAlignment    = AUDIO_IN_SAI_DMAx_MEM_DATA_SIZE;
  hSaiDma.Init.Mode                = DMA_CIRCULAR;
  hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hsai, hdmarx, hSaiDma);    
  /* Deinitialize the Stream for new transfer */
  (void)HAL_DMA_DeInit(&hSaiDma);    
  /* Configure the DMA Stream */
  (void)HAL_DMA_Init(&hSaiDma);
  
  /* SAI DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, BSP_AUDIO_IN_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
  * @brief  Clock Config.
  * @param  hsai SAI handle if required
  * @note   This API is called by BSP_AUDIO_IN_Init()
  *         Being __weak it can be overwritten by the application
  * @retval HAL_OK if no problem during execution, HAL_ERROR otherwise
  */
__weak HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hsai)
{
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  if (AudioInCtx[0].SampleRate == 16000U)
  {
    PeriphClkInitStruct.PLLSAI1.PLLN = 82;
    PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV8;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  }
  else
  {
    PeriphClkInitStruct.PLLSAI1.PLLN = 86;
    PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV7;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  }
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) while(1);
  /* SAI1 clock enable */
  __HAL_RCC_SAI1_CLK_ENABLE(); 
  return ret;
}

/**
  * @brief  Initializes the audio in instance (SAI).
  * @param  hsai SAI handle
  * @param  PDM_FREQ PDM frequency
  * @note   Being __weak it can be overwritten by the application
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_SAI1_Block_A_Init(SAI_HandleTypeDef* hsai,uint32_t PDM_FREQ)
{
  HAL_StatusTypeDef ret = HAL_OK;

  __HAL_SAI_DISABLE(hsai);

  hsai->Init.Protocol = SAI_FREE_PROTOCOL;
  hsai->Init.AudioMode = SAI_MODEMASTER_RX;
  hsai->Init.DataSize = SAI_DATASIZE_32;
  hsai->Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai->Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
  hsai->Init.Synchro = SAI_ASYNCHRONOUS;
  hsai->Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai->Init.NoDivider = SAI_MASTERDIVIDER_DISABLE;
  hsai->Init.MckOverSampling = SAI_MCK_OVERSAMPLING_DISABLE;
  hsai->Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai->Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai->Init.MonoStereoMode = SAI_STEREOMODE;
  hsai->Init.CompandingMode = SAI_NOCOMPANDING;
  hsai->Init.PdmInit.Activation = ENABLE;
  hsai->Init.PdmInit.ClockEnable = SAI_PDM_CLOCK2_ENABLE;
  hsai->FrameInit.FrameLength = 32;
  hsai->Init.PdmInit.MicPairsNbr = 2;
  hsai->Init.AudioFrequency = ((PDM_FREQ * 1000U) / hsai->FrameInit.FrameLength ) * 4U;
  hsai->FrameInit.ActiveFrameLength = 1;
  hsai->FrameInit.FSDefinition = SAI_FS_STARTFRAME;
  hsai->FrameInit.FSPolarity = SAI_FS_ACTIVE_HIGH;
  hsai->FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai->SlotInit.FirstBitOffset = 0;
  hsai->SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai->SlotInit.SlotNumber = 4;
  hsai->SlotInit.SlotActive = 0x00000002;

  if (HAL_SAI_Init(hsai) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return ret;
}

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
