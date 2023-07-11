/**
  ******************************************************************************
  * @file    opus_interface_stm.c
  * @author  MCD Application Team
  * @brief   This file contains definitions for the opus interface.
  ******************************************************************************
* @attention
  *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "opus_interface_stm.h"

/**
 * @brief Opus_HandleTypeDef structure definition
 */
typedef struct
{
  uint16_t ENC_frame_size;                              /*!< Specifies the encoder frame size. */
  
  uint16_t DEC_frame_size;                              /*!< Specifies the decoder frame size. */
  
  uint16_t max_enc_frame_size;                          /*!< Specifies the maximum size of the encoder frame. */
  
  OpusEncoder *Encoder;                                 /*!< Opus encoder. */
  
  uint8_t ENC_configured;                               /*!< Specifies if the Encoder is configured. */
  
  OpusDecoder *Decoder;                                 /*!< Opus decoder. */
  
  uint8_t DEC_configured;                               /*!< Specifies if the Decoder is configured. */

} OPUS_IF_Codec_HandleTypeDef;

  
static OPUS_IF_Codec_HandleTypeDef hOpus;


/**
 * @brief  Encoder initialization.
 * @param  ENC_configOpus: Opus encoder configuration.
 * @param  opus_err: @ref opus_errorcodes
 * @retval OPUS_IF_Status: Value indicating success or error (in case of error check opus_err).
 */
OPUS_IF_Status OPUS_IF_ENC_Init(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus, int *opus_err) 
{
  if((ENC_configOpus->application!=OPUS_APPLICATION_VOIP) && 
       (ENC_configOpus->application!=OPUS_APPLICATION_AUDIO) && 
         (ENC_configOpus->application!=OPUS_APPLICATION_RESTRICTED_LOWDELAY))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if((ENC_configOpus->bitrate<6000) || (ENC_configOpus->bitrate>510000))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if(ENC_configOpus->complexity>10)
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if(ENC_configOpus->channels==0)
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if((ENC_configOpus->ms_frame!=2.5f) &&
       (ENC_configOpus->ms_frame!=5.0f) &&
         (ENC_configOpus->ms_frame!=10.0f) &&
           (ENC_configOpus->ms_frame!=20.0f) &&
             (ENC_configOpus->ms_frame!=40.0f) &&
               (ENC_configOpus->ms_frame!=60.0f))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if((ENC_configOpus->sample_freq!=8000) && 
       (ENC_configOpus->sample_freq!=12000) &&
         (ENC_configOpus->sample_freq!=16000) &&
           (ENC_configOpus->sample_freq!=24000) &&
             (ENC_configOpus->sample_freq!=48000))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  
  hOpus.ENC_frame_size = (uint16_t)(((float)(ENC_configOpus->sample_freq/1000))*ENC_configOpus->ms_frame);
  
  hOpus.max_enc_frame_size = (ENC_configOpus->bitrate/8/((uint16_t)(1000.0f/ENC_configOpus->ms_frame)))*4; 
  
  *opus_err = 0;
  OPUS_IF_Status status;
  
  hOpus.Encoder = opus_encoder_create(ENC_configOpus->sample_freq, ENC_configOpus->channels, ENC_configOpus->application, opus_err);

  if (*opus_err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }
  
  status = OPUS_IF_ENC_Set_Bitrate(ENC_configOpus, ENC_configOpus->bitrate, opus_err);
  if (status != OPUS_IF_SUCCESS)
  {
    return OPUS_IF_ERROR;
  }
  
  status = OPUS_IF_ENC_Set_Complexity(ENC_configOpus, ENC_configOpus->complexity, opus_err);
  if (status != OPUS_IF_SUCCESS) 
  {
    return OPUS_IF_ERROR;
  }
  
  hOpus.ENC_configured = 1;

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Encoder deinit function.
 * @param  None.
 * @retval None.
 */
void OPUS_IF_ENC_Deinit(void)
{
  opus_encoder_destroy(hOpus.Encoder);
  hOpus.ENC_configured = 0;
  hOpus.ENC_frame_size = 0;
  hOpus.max_enc_frame_size = 0;
}

/**
  * @brief  This function returns if the the Opus Encoder is configured.
  * @param  None.
  * @retval uint8_t: 1 if the Encoder is configured 0 otherwise.
  */
uint8_t OPUS_IF_ENC_IsConfigured(void)
{
 return hOpus.ENC_configured;
}

/**
 * @brief  Decoder initialization.
 * @param  DEC_configOpus: Opus decoder configuration.
 * @param  opus_err: @ref opus_errorcodes
 * @retval OPUS_IF_Status: Value indicating success or error (in case of error check opus_err).
 */
OPUS_IF_Status OPUS_IF_DEC_Init(OPUS_IF_DEC_ConfigTypeDef *DEC_configOpus, int *opus_err) 
{
  if((DEC_configOpus->bitrate<6000) || (DEC_configOpus->bitrate>510000))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if(DEC_configOpus->channels==0)
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if((DEC_configOpus->ms_frame!=2.5f) &&
       (DEC_configOpus->ms_frame!=5.0f) &&
         (DEC_configOpus->ms_frame!=10.0f) &&
           (DEC_configOpus->ms_frame!=20.0f) &&
             (DEC_configOpus->ms_frame!=40.0f) &&
               (DEC_configOpus->ms_frame!=60.0f))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  if((DEC_configOpus->sample_freq!=8000) && 
       (DEC_configOpus->sample_freq!=12000) &&
         (DEC_configOpus->sample_freq!=16000) &&
           (DEC_configOpus->sample_freq!=24000) &&
             (DEC_configOpus->sample_freq!=48000))
  {
    return OPUS_IF_INVALID_PARAM;
  }

  hOpus.DEC_frame_size = (uint16_t)(((float)(DEC_configOpus->sample_freq/1000))*DEC_configOpus->ms_frame);

  hOpus.Decoder = opus_decoder_create(DEC_configOpus->sample_freq, DEC_configOpus->channels, opus_err);
  
  if (*opus_err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }
  
  hOpus.DEC_configured = 1;

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Decoder deinit function.
 * @param  None.
 * @retval None.
 */
void OPUS_IF_DEC_Deinit(void) 
{ 
  opus_decoder_destroy(hOpus.Decoder);
  hOpus.DEC_configured = 0;
  hOpus.DEC_frame_size = 0;
}

/**
  * @brief  This function returns if the the Opus Decoder is configured.
  * @param  None.
  * @retval uint8_t: 1 if the Decoder is configured 0 otherwise.
  */
uint8_t OPUS_IF_DEC_IsConfigured(void)
{
 return hOpus.DEC_configured;
}

/**
 * @brief  Set bitrate to be used for encoding
 * @param  ENC_configOpus: Opus encoder configuration.
 * @param  bitrate: Indicate the bitrate in bit per second.
 * @param  opus_err: @ref opus_errorcodes.
 * @retval OPUS_IF_Status: Value indicating success or error (in case of error check opus_err).
 */
OPUS_IF_Status OPUS_IF_ENC_Set_Bitrate(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus, int bitrate, int *opus_err) 
{
  if((bitrate<6000) || (bitrate>510000))
  {
    return OPUS_IF_INVALID_PARAM;
  }
  
  *opus_err =  opus_encoder_ctl(hOpus.Encoder, OPUS_SET_BITRATE(bitrate));
  if (*opus_err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }
  
  ENC_configOpus->bitrate = bitrate;

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Set constant bitrate option for the encoder.
 * @param  None.
 * @retval OPUS_IF_Status: Value indicating success or error.
 */
OPUS_IF_Status OPUS_IF_ENC_Set_CBR(void) 
{
  int err =  opus_encoder_ctl(hOpus.Encoder, OPUS_SET_VBR(0));
  if (err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }

  return OPUS_IF_SUCCESS;
}
  
/**
 * @brief  Set variable bitrate option for the encoder.
 * @param  None.
 * @retval OPUS_IF_Status: Value indicating success or error.
 */
OPUS_IF_Status OPUS_IF_ENC_Set_VBR(void) 
{
  int err =  opus_encoder_ctl(hOpus.Encoder, OPUS_SET_VBR(1));
  if (err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }

  return OPUS_IF_SUCCESS;
}
  
/**
 * @brief  Set complexity to be used for encoding
 * @param  ENC_configOpus: Opus encoder configuration.
 * @param  complexity: value from o to 10.
 * @param  opus_err: @ref opus_errorcodes.
 * @retval OPUS_IF_Status: Value indicating success or error (in case of error check opus_err).
 */
OPUS_IF_Status OPUS_IF_ENC_Set_Complexity(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus, int complexity, int *opus_err) 
{
  if(complexity>10)
  {
    return OPUS_IF_INVALID_PARAM;
  }
    
  *opus_err = opus_encoder_ctl(hOpus.Encoder, OPUS_SET_COMPLEXITY(complexity));
   
  if (*opus_err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }
  
  ENC_configOpus->complexity = complexity;

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Force the encoder to use only SILK
 * @param  None.
 * @retval OPUS_IF_Status: Value indicating success or error.
 */
OPUS_IF_Status OPUS_IF_ENC_Force_SILKmode(void) 
{
  int err =  opus_encoder_ctl(hOpus.Encoder, OPUS_SET_FORCE_MODE(MODE_SILK_ONLY));

  if (err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Force the encoder to use only CELT.
 * @param  None.
 * @retval OPUS_IF_Status: Value indicating success or error.
 */
OPUS_IF_Status OPUS_IF_ENC_Force_CELTmode(void) 
{
  int err =  opus_encoder_ctl(hOpus.Encoder, OPUS_SET_FORCE_MODE(MODE_CELT_ONLY));
  
  if (err != OPUS_OK) 
  {
    return OPUS_IF_ERROR;
  }

  return OPUS_IF_SUCCESS;
}

/**
 * @brief  Encoding functions.
 * @param  buf_in: pointer to the PCM buffer to be encoded.
 * @param  buf_out: pointer to the Encoded buffer.
 * @retval Number of bytes in case of success, 0 vice-versa.
 */
int OPUS_IF_ENC_Encode(uint8_t * buf_in, uint8_t * buf_out) 
{
  return opus_encode(hOpus.Encoder, (opus_int16 *) buf_in, hOpus.ENC_frame_size, (unsigned char *) buf_out, (opus_int32) hOpus.max_enc_frame_size);
}

/**
 * @brief  Decoding functions.
 * @param  buf_in: pointer to the Encoded buffer to be decoded.
 * @param  len: length of the buffer in.
 * @param  buf_out: pointer to the Decoded buffer.
 * @retval Number of decoded samples or @ref opus_errorcodes.
 */
int OPUS_IF_DEC_Decode(uint8_t * buf_in, uint32_t len, uint8_t * buf_out) 
{
  return opus_decode(hOpus.Decoder, (unsigned char *) buf_in, (opus_int32) len, (opus_int16 *) buf_out, hOpus.DEC_frame_size, 0);
}


