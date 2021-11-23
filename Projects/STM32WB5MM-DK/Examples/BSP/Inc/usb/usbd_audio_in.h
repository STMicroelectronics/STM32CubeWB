/**
  ******************************************************************************
  * @file    usbd_audio_in.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_audio_in.c file.
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

#ifndef __USBD_AUDIO_IN_H
#define __USBD_AUDIO_IN_H

#include "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_AUDIO_IN
  * @{
  */ 

/** @defgroup USBD_AUDIO_IN_Exported_Defines
  * @{
  */ 

#define AUDIO_OUT_EP                                  0x01
#define USB_AUDIO_CONFIG_DESC_SIZ                     109 
#define AUDIO_INTERFACE_DESC_SIZE                     9
#define USB_AUDIO_DESC_SIZ                            0x09
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07
#define AUDIO_DESCRIPTOR_TYPE                         0x21
#define USB_DEVICE_CLASS_AUDIO                        0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_STREAMING_GENERAL                       0x01
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02
/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25
/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06
#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07
#define AUDIO_CONTROL_MUTE                            0x0002
#define AUDIO_FORMAT_TYPE_I                           0x01
#define AUDIO_FORMAT_TYPE_III                         0x03
#define AUDIO_ENDPOINT_GENERAL                        0x01
#define AUDIO_REQ_GET_CUR                             0x81
#define AUDIO_REQ_GET_MIN                             0x82
#define AUDIO_REQ_GET_MAX                             0x83
#define AUDIO_REQ_GET_RES                             0x84
#define AUDIO_REQ_SET_CUR                             0x01
#define AUDIO_OUT_STREAMING_CTRL                      0x02
#define VOL_MIN                                       0xDBE0 
#define VOL_RES                                       0x0023
#define VOL_MAX                                       0x0000 
#define AUDIO_IN_PACKET                  (uint32_t)((((48000/1000)+2)*8)*2) 
#define MIC_IN_TERMINAL_ID                            1
#define MIC_FU_ID                                     2
#define MIC_OUT_TERMINAL_ID                           3
#define USB_INTERFACE_DESCRIPTOR_TYPE                 0x04
/* Audio Data in endpoint */
#define AUDIO_IN_EP                                   0x81 

/* Buffering state definitions */
typedef enum
{
  STATE_USB_WAITING_FOR_INIT = 0,
  STATE_USB_IDLE = 1,
  STATE_USB_REQUESTS_STARTED = 2,  
  STATE_USB_BUFFER_WRITE_STARTED = 3,    
}AUDIO_StatesTypeDef;

/* Number of sub-packets in the audio transfer buffer.*/
#define AUDIO_IN_PACKET_NUM                            6

#define TIMEOUT_VALUE                                  200

/* Audio Commands enmueration */
typedef enum
{
  AUDIO_CMD_START = 1,
  AUDIO_CMD_PLAY,
  AUDIO_CMD_STOP,
}AUDIO_CMD_TypeDef;

typedef enum
{
  AUDIO_OFFSET_NONE = 0,
  AUDIO_OFFSET_HALF,
  AUDIO_OFFSET_FULL,  
  AUDIO_OFFSET_UNKNOWN,    
}AUDIO_OffsetTypeDef;


/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_IN_Exported_TypesDefinitions
  * @{
  */
typedef struct
{
  uint8_t cmd;   
  uint8_t data[USB_MAX_EP0_SIZE];  
  uint8_t len;  
  uint8_t unit;    
}USBD_AUDIO_ControlTypeDef; 

typedef struct
{
  __IO uint32_t              alt_setting;  
  uint8_t                    channels;
  uint32_t                   frequency;
  __IO int16_t                   timeout;
  uint16_t                   buffer_length;    
  uint16_t                   dataAmount;
  uint16_t                   paketDimension;   
  uint8_t                    state;  
  uint16_t                   rd_ptr;  
  uint16_t                   wr_ptr;  
  uint8_t                    upper_treshold;
  uint8_t                    lower_treshold;
  USBD_AUDIO_ControlTypeDef control;   
  uint8_t  *                 buffer;
}USBD_AUDIO_HandleTypeDef; 

typedef struct
{
  int8_t  (*Init)         	(uint32_t  AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
  int8_t  (*DeInit)       	(uint32_t options);
  int8_t  (*Record)     	(void);
  int8_t  (*VolumeCtl)    	(int16_t Volume);
  int8_t  (*MuteCtl)      	(uint8_t cmd);
  int8_t  (*Stop)   		(void);
  int8_t  (*Pause)   		(void);
  int8_t  (*Resume)   		(void);
  int8_t  (*CommandMgr)     (uint8_t cmd);
}USBD_AUDIO_ItfTypeDef;
/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_IN_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_IN_Exported_Variables
  * @{
  */ 
extern USBD_ClassTypeDef  USBD_AUDIO;
/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_IN_Exported_Functions
  * @{
  */ 
uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, USBD_AUDIO_ItfTypeDef *fops);
void USBD_AUDIO_Init_Microphone_Descriptor(USBD_HandleTypeDef   *pdev, uint32_t samplingFrequency, uint8_t Channels);
uint8_t  USBD_AUDIO_Data_Transfer (USBD_HandleTypeDef *pdev, int16_t * audioData, uint16_t dataAmount);

/**
  * @}
  */ 


/**
  * @}
  */ 

/**
  * @}
  */ 
#endif  /* __USBD_AUDIO_IN_H */
