/**
  ******************************************************************************
  * @file    bvopus_service_stm.c
  * @author  MCD Application Team
  * @brief   This file contains definitions for BlueVoice opus service.
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
#include "bvopus_service_stm.h"
#include "opus_interface_stm.h"
#include "common_blesvc.h"
    
#ifndef MIN
#define MIN(a,b)            ((a) < (b) )? (a) : (b)
#endif

/* Private typedef -----------------------------------------------------------*/

/*!< Transport protocol packet type. */
typedef enum 
{
  BV_OPUS_TP_START_PACKET = 0x00,
  BV_OPUS_TP_START_END_PACKET = 0x20,
  BV_OPUS_TP_MIDDLE_PACKET = 0x40, 
  BV_OPUS_TP_END_PACKET = 0x80
} BV_OPUS_TP_Packet_Typedef;

/*!< Transport protocol status. */
typedef enum 
{
  BV_OPUS_TP_WAIT_START = 0, 
  BV_OPUS_TP_WAIT_END = 1
} BV_OPUS_TP_Status_Typedef;

/*!< BV_OPUS_Handle structure definition. */
typedef struct
{
  uint8_t TX_configured;                                /*!< Specifies if the Tx service is configured. */

  BV_OPUS_uuid_t BV_uuid;                               /*!< Specifies the uuid for the bluevoice service and characteristics. */
  
  BV_OPUS_ProfileHandle_t BV_handle;                    /*!< Specifies the handle for the bluevoice service and characteristics. */

  uint8_t AudioNotifEnabled;                            /*!< Audio characteristic enabled. */
  
  uint8_t CtrlNotifEnabled;                             /*!< Control characteristic enabled. */
  
  uint8_t MaxDataLength;                                /*!< Maximum ble packet length. */
  
  uint8_t *pInternalMemory_dec;                         /*!< Pointer to the internal memory used for the BlueVoice decoding. */
  
  uint8_t *pInternalMemory_enc;                         /*!< Pointer to the internal memory used for the BlueVoice encoding. */
  
} BV_OPUS_HandleTypeDef;

static BV_OPUS_HandleTypeDef hBV_OPUS;

/* Private variables ---------------------------------------------------------*/

/*!< Audio service uuid.*/
const uint8_t bvopus_service_uuid[16] =
{
  0x1b,0xc5,0xd5,0xa5,0x02,0x00,0xb4,0x9a,0xe1,0x11,0x01,0x00,0x00,0x00,0x00,0x00
};   

/*!< Audio characteristic uuid.*/
const uint8_t bvopus_audio_char_uuid[16] =
{
  0x1b,0xc5,0xd5,0xa5,0x02,0x00,0x36,0xac,0xe1,0x11,0x02,0x00,0x01,0x00,0x00,0x00
};      

/*!< Control characteristic uuid.*/
const uint8_t bvopus_ctrl_char_uuid[16] =
{
  0x1b,0xc5,0xd5,0xa5,0x02,0x00,0x36,0xac,0xe1,0x11,0x02,0x00,0x02,0x00,0x00,0x00
};  

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  This function is called to add BlueVoiceOPUS Service.
  * @param  None.
  * @retval BV_OPUS_Status: Value indicating success or error code.
  */
BV_OPUS_Status BluevoiceOPUS_AddService(void);

/**
  * @brief  This function is called to add BlueVoice characteristics.
  * @param  service_handle: Handle of the service to which the characteristic must be added.
  * @retval BV_OPUS_Status: Value indicating success or error code.
  */
BV_OPUS_Status BluevoiceOPUS_AddChar(uint16_t service_handle);

/**
 * @brief  This function returns the amount of memory required for the current bluevoice decoder setup.
 * @param  Opus decoder configuration.
 * @retval Number of byte, 0 for invalid parameters.
 */
uint32_t BVOPUS_DEC_getMemorySize(OPUS_IF_DEC_ConfigTypeDef *DEC_configOpus);

/**
 * @brief  This functions returns the amount of memory required for the current bluevoice encoder setup.
 * @param  Opus encoder configuration.
 * @retval Number of byte, 0 for invalid parameters.
 */
uint32_t BVOPUS_ENC_getMemorySize(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus);

/**
  * @brief  This function must be called when there is a LE attribute modified event. 
  * @param  attr_handle: Attribute handle.
  * @param  attr_len: Attribute length.
  * @param  attr_value: Attribute value.
  * @retval BVOPUS_STM_evt_code_t: Value indicating the operation code.
  */
BVOPUS_STM_evt_code_t BluevoiceOPUS_AttributeModified_CB(uint16_t attr_handle, uint16_t attr_len, uint8_t *attr_value);

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not.
 */
static SVCCTL_EvtAckStatus_t BVOPUS_Event_Handler(void *Event);

/**
  * @brief  This function is called to parse a BV_OPUS_TP packet.
  * @param  buffer_out: pointer to the output buffer.
  * @param  buffer_in: pointer to the input data.
  * @param  len: buffer in length
  * @retval Buffer out length.
  */
uint32_t BluevoiceOPUS_TP_Parse(uint8_t* buffer_out, uint8_t* buffer_in, uint32_t len);

/**
  * @brief  This function is called to prepare a BV_OPUS_TP packet.
  * @param  buffer_out: pointer to the buffer used to save BV_OPUS_TP packet.
  * @param  buffer_in: pointer to the input data.
  * @param  len: buffer in length
  * @retval Buffer out length.
  */
uint32_t BluevoiceOPUS_TP_Encapsulate(uint8_t* buffer_out, uint8_t* buffer_in, uint16_t len); 


/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  BlueVoice Opus Service initialization
 * @param  None
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
void BVOPUS_STM_Init(void)
{
  /* Register the event handler to the BLE controller */
  SVCCTL_RegisterSvcHandler(BVOPUS_Event_Handler);

  memset(&hBV_OPUS, 0, sizeof(hBV_OPUS));
  
  /* Set default ble packet size equal to 20B*/
  hBV_OPUS.MaxDataLength = 20;
    
  BluevoiceOPUS_AddService();
  
  BluevoiceOPUS_AddChar(hBV_OPUS.BV_handle.ServiceHandle);
}

/**
 * @brief  BlueVoiceOPUS Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not.
 */
static SVCCTL_EvtAckStatus_t BVOPUS_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  BVOPUS_STM_evt_code_t Evt_code;
    
  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      {
        blecore_evt = (evt_blecore_aci*)event_pckt->data;
        switch(blecore_evt->ecode)
        {
          case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
            {
              aci_gatt_attribute_modified_event_rp0 *pr = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
              Evt_code = BluevoiceOPUS_AttributeModified_CB(pr->Attr_Handle, pr->Attr_Data_Length, pr->Attr_Data); 
              if(Evt_code == BVOPUS_STM_START_STREAMING_EVT)
              {
                hBV_OPUS.AudioNotifEnabled = 1;
              }
              if(Evt_code == BVOPUS_STM_STOP_STREAMING_EVT)
              {
                hBV_OPUS.AudioNotifEnabled = 0;
              }
              BVOPUS_STM_APP_Notification(Evt_code);
            }
            break;

          default:
            break;
        }
      }
      break;

    default:
      break;
  }
  
  return(return_value);
}

/**
  * @brief  This function must be called when there is a LE attribute modified event. 
  * @param  attr_handle: Attribute handle.
  * @param  attr_len: Attribute length.
  * @param  attr_value: Attribute value.
  * @retval BVOPUS_STM_evt_code_t: Value indicating the operation code.
 */
BVOPUS_STM_evt_code_t BluevoiceOPUS_AttributeModified_CB(uint16_t attr_handle, uint16_t attr_len, uint8_t *attr_value)
{    
  if (attr_handle == (hBV_OPUS.BV_handle.CharAudioHandle + 2))
  {
    if (attr_value[0] == 0x01)
    { 
      return BVOPUS_STM_START_STREAMING_EVT;
    }
    else if(attr_value[0] == 0x00)
    {
      return BVOPUS_STM_STOP_STREAMING_EVT;
    }  
  }
  else if(attr_handle == (hBV_OPUS.BV_handle.CharCtrlHandle + 2))
  {
    if (attr_value[0] == 0x01)
    {
      hBV_OPUS.CtrlNotifEnabled = 1;
      return BVOPUS_STM_ENABLE_CTRL_EVT;
    }
    else if(attr_value[0] == 0x00)
    {
      hBV_OPUS.CtrlNotifEnabled = 0;
      return BVOPUS_STM_DISABLE_CTRL_EVT;
    }  
  }
  return BVOPUS_STM_ERROR_EVT;
}

/**
 * @brief  This functions returns the amount of memory required for the current bluevoice encoder setup.
 * @param  Opus encoder configuration.
 * @retval Number of byte, 0 for invalid parameters.
 */
uint32_t BVOPUS_ENC_getMemorySize(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus) 
{  
  if((ENC_configOpus->bitrate<6000) || (ENC_configOpus->bitrate>510000))
  {
    return 0;
  }
  if((ENC_configOpus->ms_frame!=2.5f) &&
       (ENC_configOpus->ms_frame!=5.0f) &&
         (ENC_configOpus->ms_frame!=10.0f) &&
           (ENC_configOpus->ms_frame!=20.0f) &&
             (ENC_configOpus->ms_frame!=40.0f) &&
               (ENC_configOpus->ms_frame!=60.0f))
  {
    return 0;
  }
  
  uint32_t tot_enc_size, max_enc_frame_size; 
  
  /* size x2 (worst case encode VBR) x2 (temp buffer used for BVTP) */
  max_enc_frame_size = (ENC_configOpus->bitrate/8/((uint16_t)(1000.0f/ENC_configOpus->ms_frame)))*4; 
  tot_enc_size = max_enc_frame_size + (max_enc_frame_size/19)+1;
  
  return tot_enc_size;
}
 
/**
 * @brief  This function returns the amount of memory required for the current bluevoice decoder setup.
 * @param  Opus decoder configuration.
 * @retval Number of byte, 0 for invalid parameters.
 */
uint32_t BVOPUS_DEC_getMemorySize(OPUS_IF_DEC_ConfigTypeDef *DEC_configOpus) 
{
  if((DEC_configOpus->bitrate<6000) || (DEC_configOpus->bitrate>510000))
  {
    return 0;
  }
  if((DEC_configOpus->ms_frame!=2.5f) &&
       (DEC_configOpus->ms_frame!=5.0f) &&
         (DEC_configOpus->ms_frame!=10.0f) &&
           (DEC_configOpus->ms_frame!=20.0f) &&
             (DEC_configOpus->ms_frame!=40.0f) &&
               (DEC_configOpus->ms_frame!=60.0f))
  {
    return 0;
  }
  
  uint32_t tot_dec_size = (DEC_configOpus->bitrate/8/((uint16_t)(1000.0f/DEC_configOpus->ms_frame)))*2;
  
  return tot_dec_size;
}

/**
  * @brief  This function is called to add BlueVoiceOPUS Service.
  * @param  None.
  * @retval BV_OPUS_Status: Value indicating success or error code.
  */
BV_OPUS_Status BluevoiceOPUS_AddService(void)
{
  tBleStatus ret;

  memcpy(hBV_OPUS.BV_uuid.service_uuid.Service_UUID_128, bvopus_service_uuid, sizeof(bvopus_service_uuid));                 
              
  ret = aci_gatt_add_service(UUID_TYPE_128, &hBV_OPUS.BV_uuid.service_uuid, PRIMARY_SERVICE, 9, &hBV_OPUS.BV_handle.ServiceHandle); 
                             
  if (ret != BLE_STATUS_SUCCESS)
  {
    return BV_OPUS_ERROR;
  }
  
  return BV_OPUS_SUCCESS;    
}

/**
  * @brief  This function is called to add BlueVoice characteristics.
  * @param  service_handle: Handle of the service to which the characteristic must be added.
  * @retval BV_OPUS_Status: Value indicating success or error code.
  */
BV_OPUS_Status BluevoiceOPUS_AddChar(uint16_t service_handle)
{
  tBleStatus ret;
   
  memcpy(hBV_OPUS.BV_uuid.audio_uuid.Char_UUID_128, bvopus_audio_char_uuid, sizeof(bvopus_audio_char_uuid));

  ret = aci_gatt_add_char(hBV_OPUS.BV_handle.ServiceHandle, UUID_TYPE_128, &hBV_OPUS.BV_uuid.audio_uuid,
                          (CFG_BLE_MAX_ATT_MTU-3), CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16, 1,
                          &hBV_OPUS.BV_handle.CharAudioHandle);
                      
  if (ret != BLE_STATUS_SUCCESS)
  {
    return BV_OPUS_ERROR;
  }  
  memcpy(hBV_OPUS.BV_uuid.ctrl_uuid.Char_UUID_128, bvopus_ctrl_char_uuid, sizeof(bvopus_ctrl_char_uuid));

  ret = aci_gatt_add_char(hBV_OPUS.BV_handle.ServiceHandle, UUID_TYPE_128, &hBV_OPUS.BV_uuid.ctrl_uuid,
                          20, CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16, 1,
                          &hBV_OPUS.BV_handle.CharCtrlHandle);
                      
  if (ret != BLE_STATUS_SUCCESS)
  {
    return BV_OPUS_ERROR;
  }
       
  hBV_OPUS.TX_configured = 1;
  
  return BV_OPUS_SUCCESS;
}

/**
 * @brief  This function initializes Opus encoder.
 * @param  Opus encoder configuration.
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BVOPUS_CodecEncInit(OPUS_IF_ENC_ConfigTypeDef *ENC_configOpus) 
{
  uint32_t enc_size = BVOPUS_ENC_getMemorySize(ENC_configOpus);
  if(enc_size == 0)
  {
    return BV_OPUS_INVALID_PARAM;
  }
  hBV_OPUS.pInternalMemory_enc = (uint8_t *)malloc(enc_size);

  if(!OPUS_IF_ENC_IsConfigured())
  {  
    int opus_err;
    OPUS_IF_Status status = OPUS_IF_ENC_Init(ENC_configOpus, &opus_err);
    if(status == OPUS_IF_INVALID_PARAM)
    {
      return BV_OPUS_INVALID_PARAM;
    }
    else if(status == OPUS_IF_ERROR)
    {
      return BV_OPUS_ERROR;
    }    
  }
  
  return BV_OPUS_SUCCESS;
}

/**
 * @brief  This function initializes Opus decoder.
 * @param  Opus decoder configuration.
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BVOPUS_CodecDecInit(OPUS_IF_DEC_ConfigTypeDef *DEC_configOpus)
{
  uint32_t dec_size = BVOPUS_DEC_getMemorySize(DEC_configOpus);
  if(dec_size == 0)
  {
    return BV_OPUS_INVALID_PARAM;
  }
  hBV_OPUS.pInternalMemory_dec = (uint8_t *)malloc(dec_size);
  
  if(!OPUS_IF_DEC_IsConfigured())
  {
    int opus_err;
    OPUS_IF_Status status = OPUS_IF_DEC_Init(DEC_configOpus, &opus_err);
    if(status == OPUS_IF_INVALID_PARAM)
    {
      return BV_OPUS_INVALID_PARAM;
    }
    else if(status == OPUS_IF_ERROR)
    {
      return BV_OPUS_ERROR;
    }    
  }
  
  return BV_OPUS_SUCCESS;
}


/**
 * @brief  This function sets the maximum ble packet size.
 * @param  max_length: maximum ble packet size.
 * @retval None.
 */                      
void BluevoiceOPUS_SetMaxDataLength(uint16_t max_length) 
{
  hBV_OPUS.MaxDataLength = (max_length-3);
}


/**
 * @brief  This function returns the audio notification status.
 * @param  None.
 * @retval 1 if audio notification are enabled, 0 otherwise.
 */
uint8_t BluevoiceOPUS_isAudioNotifEnabled(void) 
{
  return hBV_OPUS.AudioNotifEnabled;
}

/**
 * @brief  This function returns the control notification status.
 * @param  None.
 * @retval 1 if the control notification are enabled, 0 otherwise.
 */
uint8_t BluevoiceOPUS_isCtrlNotifEnabled(void) 
{
  return hBV_OPUS.CtrlNotifEnabled;
}

/**
 * @brief  This function is called to decode audio data using opus.
 * @param  buf_in: pointer to the encoded buffer.
 * @param  len_in: buffer in length.
 * @param  buf_out: pointer to the decoded buffer.
 * @param  pcm_samples: number of PCM samples decoded.
 * @param  opus_err: @ref opus_errorcodes.
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BluevoiceOPUS_ParseData(uint8_t *buf_in, uint8_t len_in, uint8_t *buf_out, uint16_t *pcm_samples, int *opus_err)
{
  if(!OPUS_IF_DEC_IsConfigured())
  {
    return BV_OPUS_DEC_NOT_CONFIG;
  }
  
  uint32_t OpusDecBufLen = BluevoiceOPUS_TP_Parse(hBV_OPUS.pInternalMemory_dec, buf_in, len_in);
  
  if (OpusDecBufLen > 0) 
  {
    *opus_err = OPUS_IF_DEC_Decode(hBV_OPUS.pInternalMemory_dec, OpusDecBufLen, buf_out);  
    
    if(*opus_err<=0)
    { 
      *pcm_samples = 0;
      return BV_OPUS_ERROR;
    }
    else
    {
      *pcm_samples = *opus_err;
      return BV_OPUS_SUCCESS;
    }    
  }
  else
  {
    return BV_OPUS_PKT_NOT_COMPLETE;
  }
}

/**
 * @brief  This function is called to packetize, encode and send audio data.
 * @param  data_buffer: data to be sent
 * @param  opus_err: @ref opus_errorcodes.
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BluevoiceOPUS_SendAudioData(uint8_t *data_buffer, int *opus_err) 
{
  if(!hBV_OPUS.TX_configured)
  {
    return BV_OPUS_TX_HANDLE_NOT_AVAILABLE;   
  }
   
  if(!hBV_OPUS.AudioNotifEnabled)
  {
    return BV_OPUS_NOTIF_DISABLE;
  }
  
  if(!OPUS_IF_ENC_IsConfigured())
  {
    return BV_OPUS_ENC_NOT_CONFIG;
  }
  
  uint32_t j = 0, len = 0, tot_len = 0;
  uint32_t timeout_cnt = 0;
  int32_t OpusEncBufLen = 0;
  uint8_t *OpusEncBuf_BV_OPUS_TP;
  *opus_err = 0;
  
  OpusEncBufLen = OPUS_IF_ENC_Encode((uint8_t *) data_buffer, hBV_OPUS.pInternalMemory_enc);

  if(OpusEncBufLen<=0)
  {
    *opus_err = OpusEncBufLen;
    return BV_OPUS_ERROR;
  }

  OpusEncBuf_BV_OPUS_TP = &hBV_OPUS.pInternalMemory_enc[OpusEncBufLen];
  
  
  tot_len = BluevoiceOPUS_TP_Encapsulate(OpusEncBuf_BV_OPUS_TP, hBV_OPUS.pInternalMemory_enc, OpusEncBufLen); 
  
  /* Data are sent as notifications*/
  while (j < tot_len) 
  {
    len = MIN(hBV_OPUS.MaxDataLength, tot_len - j);

    while(aci_gatt_update_char_value(hBV_OPUS.BV_handle.ServiceHandle, hBV_OPUS.BV_handle.CharAudioHandle, 0,
                                     len,(uint8_t*) &OpusEncBuf_BV_OPUS_TP[j]) != BLE_STATUS_SUCCESS)
    {
      /* Radio is busy */
      /*-------------------------------------------------------*/
      timeout_cnt++;
      if (timeout_cnt >= BV_OPUS_SENDING_TIMEOUT)
      {             
        return BV_OPUS_TIMEOUT;
      }
    }
    j += len;
  }

  return BV_OPUS_SUCCESS;
}

/**
 * @brief  This function is called to send control command.
 * @param  data_buffer: data to be sent
 * @param  Nb_bytes: number of bytes to be sent (max 20)
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BluevoiceOPUS_SendCtrlData(uint8_t* data_buffer, uint8_t Nb_bytes)
{
  if(!hBV_OPUS.TX_configured)
  {
    return BV_OPUS_TX_HANDLE_NOT_AVAILABLE;   
  }
  
  if(!hBV_OPUS.CtrlNotifEnabled)
  {
    return BV_OPUS_NOTIF_DISABLE;
  }
  
  uint32_t timeout_cnt = 0;
  if(Nb_bytes > 20)
  {
    /*length too long for a single packet*/
    return BV_OPUS_ERROR;
  }
   
  /* Data are sent as notifications*/
  while (aci_gatt_update_char_value(hBV_OPUS.BV_handle.ServiceHandle, hBV_OPUS.BV_handle.CharCtrlHandle, 0, 
                                    Nb_bytes, (uint8_t *)data_buffer) != BLE_STATUS_SUCCESS)
  {
    /* Radio is busy */
    /*-------------------------------------------------------*/
    timeout_cnt++;
    if (timeout_cnt >= BV_OPUS_SENDING_TIMEOUT)
    {             
      return BV_OPUS_TIMEOUT;
    }
  } 
  return BV_OPUS_SUCCESS;
}

/**
 * @brief  This function is called to send an enable notification request.
 * @param  None
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BluevoiceOPUS_SendEnableNotifReq(void)
{
  uint8_t data[] = {BV_OPUS_CONTROL, BV_OPUS_ENABLE_NOTIF_REQ};
  
  return BluevoiceOPUS_SendCtrlData(data, 2);
}

/**
 * @brief  This function is called to send a disable notification request.
 * @param  None
 * @retval BV_OPUS_Status: Value indicating success or error code.
 */
BV_OPUS_Status BluevoiceOPUS_SendDisableNotifReq(void)
{
  uint8_t data[] = {BV_OPUS_CONTROL, BV_OPUS_DISABLE_NOTIF_REQ}; 
  
  return BluevoiceOPUS_SendCtrlData(data, 2);
}


/**********************************/
/******* Transfer Protocol *******/
/**********************************/

/**
  * @brief  This function is called to parse a BV_OPUS_TP packet.
  * @param  buffer_out: pointer to the output buffer.
  * @param  buffer_in: pointer to the input data.
  * @param  len: buffer in length
  * @retval Buffer out length.
  */
uint32_t BluevoiceOPUS_TP_Parse(uint8_t* buffer_out, uint8_t* buffer_in, uint32_t len) 
{
  static uint32_t tot_len = 0;
  uint32_t buff_out_len;
  static BV_OPUS_TP_Status_Typedef status = BV_OPUS_TP_WAIT_START;
  BV_OPUS_TP_Packet_Typedef packet_type;

  packet_type = (BV_OPUS_TP_Packet_Typedef) buffer_in[0];

  switch (status)
  {
  case BV_OPUS_TP_WAIT_START:
    {
      if (packet_type == BV_OPUS_TP_START_PACKET) 
      {
        /*First part of an Opus packet*/
        /*packet is enqueued*/
        memcpy((uint8_t*) &buffer_out[tot_len], (uint8_t*) &buffer_in[1], (len - 1));
        tot_len += len - 1;
        status = BV_OPUS_TP_WAIT_END;
        return 0; 
      }
      else if (packet_type == BV_OPUS_TP_START_END_PACKET) 
      {
        /*Final part of an Opus packet*/
        /*packet is enqueued*/
        memcpy((uint8_t*) &buffer_out[tot_len], (uint8_t*) &buffer_in[1], (len - 1));

        tot_len += len - 1;

        /*number of bytes of the output packet*/
        buff_out_len = tot_len;

        /*total length set to zero*/
        tot_len = 0;

        /*reset status*/
        status = BV_OPUS_TP_WAIT_START;

        /*return decoded output dimension*/
        return buff_out_len; 
      }
      else
      {
        /* Error */
        return 0;
      }
    }
  case BV_OPUS_TP_WAIT_END: 
    {
      if (packet_type == BV_OPUS_TP_MIDDLE_PACKET) 
      {
        /*Central part of an Opus packet*/
        /*packet is enqueued*/
        memcpy((uint8_t*) &buffer_out[tot_len], (uint8_t*) &buffer_in[1], (len - 1));

        tot_len += len - 1;
        return 0; 
      }
      else if (packet_type == BV_OPUS_TP_END_PACKET) 
      {
        /*Final part of an Opus packet*/
        /*packet is enqueued*/
        memcpy((uint8_t*) &buffer_out[tot_len], (uint8_t*) &buffer_in[1], (len - 1));

        tot_len += len - 1;

        /*number of bytes of the output packet*/
        buff_out_len = tot_len;

        /*total length set to zero*/
        tot_len = 0;

        /*reset status*/
        status = BV_OPUS_TP_WAIT_START;

        /*return decoded output dimension*/
        return buff_out_len; 
      } 
      else 
      {
        /*reset status*/
        status = BV_OPUS_TP_WAIT_START;

        /*total length set to zero*/
        tot_len = 0;

        return 0; /* error */
      }
    }
  }
  return 0;
}

/**
  * @brief  This function is called to prepare a BV_OPUS_TP packet.
  * @param  buffer_out: pointer to the buffer used to save BV_OPUS_TP packet.
  * @param  buffer_in: pointer to the input data.
  * @param  len: buffer in length
  * @retval Buffer out length.
  */
uint32_t BluevoiceOPUS_TP_Encapsulate(uint8_t* buffer_out, uint8_t* buffer_in, uint16_t len) 
{
  uint32_t size = 0, tot_size = 0;
  uint32_t counter = 0;
  BV_OPUS_TP_Packet_Typedef packet_type = BV_OPUS_TP_START_PACKET;

  /* One byte header is added to each BLE packet */

  while (counter < len) 
  {
    size = MIN((hBV_OPUS.MaxDataLength-1), len - counter);

    if (len - counter <= (hBV_OPUS.MaxDataLength-1)) 
    {    
      if (counter == 0) 
      {
        packet_type = BV_OPUS_TP_START_END_PACKET;
      }
      else
      {
        packet_type = BV_OPUS_TP_END_PACKET;
      }
    }

    switch (packet_type) 
    {
    case BV_OPUS_TP_START_PACKET: 
      {
        /*First part of an Opus packet*/
        buffer_out[tot_size] = ((uint8_t) (BV_OPUS_TP_START_PACKET));
        tot_size++;
        packet_type = BV_OPUS_TP_MIDDLE_PACKET;
      }
      break;
    case BV_OPUS_TP_START_END_PACKET: 
      {
        /*First and last part of an Opus packet*/
        buffer_out[tot_size] = ((uint8_t) (BV_OPUS_TP_START_END_PACKET));
        tot_size++;
        packet_type = BV_OPUS_TP_START_PACKET;
      }
      break;
    case BV_OPUS_TP_MIDDLE_PACKET: 
      {
        /*Central part of an Opus packet*/
        buffer_out[tot_size] = ((uint8_t) (BV_OPUS_TP_MIDDLE_PACKET));
        tot_size++;
      }
      break;
    case BV_OPUS_TP_END_PACKET: 
      {
        /*Last part of an Opus packet*/
        buffer_out[tot_size] = ((uint8_t) (BV_OPUS_TP_END_PACKET));
        tot_size++;
        packet_type = BV_OPUS_TP_START_PACKET;
      }
      break;
    }

    /*Input data is incapsulated*/
    memcpy((uint8_t*) &buffer_out[tot_size], (uint8_t *) &buffer_in[counter], size);

    /*length variables update*/
    counter += size;
    tot_size += size;
  }
  return tot_size;
}



