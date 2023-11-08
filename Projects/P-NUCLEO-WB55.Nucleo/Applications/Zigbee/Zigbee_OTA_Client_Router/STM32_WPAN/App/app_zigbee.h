/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_zigbee.h
  * Description        : Header for Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_ZIGBEE_H
#define APP_ZIGBEE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tl.h"
#include "tl_zigbee_hci.h"
#include "stdbool.h"
  
/* FUOTA specific defines ----------------------------------------------------*/

/* Keyword found at the end of Zigbee Ota compatible application binaries */
#define FUOTA_MAGIC_KEYWORD_APP                0x94448A29

/* Keyword found at the end of encrypted Copro Wireless binaries */
#define FUOTA_MAGIC_KEYWORD_COPRO_WIRELESS     0xD3A12C5E

/* Define Address for Application FW Update */
#define FUOTA_APP_FW_BINARY_ADDRESS            (FLASH_BASE + CFG_APP_START_SECTOR_INDEX*0x1000)

/* Define Address for Copro Wireless FW Update */
#define FUOTA_COPRO_FW_BINARY_ADDRESS          (FLASH_BASE + CFG_APP_START_SECTOR_INDEX*0x1000)

#define FUOTA_NUMBER_WORDS_64BITS              50
#define FUOTA_PAYLOAD_SIZE                     (FUOTA_NUMBER_WORDS_64BITS * 8)

/* ZCL OTA specific defines ------------------------------------------------------*/
#define ST_ZIGBEE_MANUFACTURER_CODE            0x1041
#define CURRENT_HARDWARE_VERSION               0x01
#define CURRENT_FW_COPRO_WIRELESS_FILE_VERSION 0x01
#define CURRENT_FW_APP_FILE_VERSION            0x01
#define IMAGE_TYPE_FW_COPRO_WIRELESS           0x01 /* M0 binary  */
#define IMAGE_TYPE_FW_APP                      0x02 /* M4 binary  */
#define RAM_FIRMWARE_BUFFER_SIZE               1024

/* Exported types ------------------------------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum {
    ERR_ZIGBE_CMD_TO_M0,
    ERR_ZIGBEE_CHECK_WIRELESS
} ErrAppliIdEnum_t;

/**
  * @brief  APP_ZIGBEE Status structures definition
  */
typedef enum
{
  APP_ZIGBEE_OK       = 0x00,
  APP_ZIGBEE_ERROR    = 0x01,
} APP_ZIGBEE_StatusTypeDef;

/*
 *  OTA structures definition
 */
enum APP_ZIGBEE_OtaFileTypeDef_t{
  fileType_COPRO_WIRELESS = IMAGE_TYPE_FW_COPRO_WIRELESS,
  fileType_APP = IMAGE_TYPE_FW_APP,
};

struct OTA_currentFileVersion{
  enum APP_ZIGBEE_OtaFileTypeDef_t fileType;
  uint8_t fileVersion;
};

struct APP_ZIGBEE_OtaContext_t{
  enum APP_ZIGBEE_OtaFileTypeDef_t  file_type;
  uint8_t file_version;
  uint32_t binary_size;
  uint32_t binary_calc_crc;
  uint32_t binary_srv_crc;
  uint32_t base_address;
  uint32_t magic_keyword;
};

struct APP_ZIGBEE_OtaWriteInfo_t{
  uint8_t firmware_buffer[RAM_FIRMWARE_BUFFER_SIZE];
  uint32_t firmware_buffer_current_offset;  
  uint32_t flash_current_offset;
  bool buffer_full;
};

struct Zigbee_OTA_client_info {
  struct APP_ZIGBEE_OtaContext_t ctx;
  struct APP_ZIGBEE_OtaWriteInfo_t write_info;
  uint16_t image_type;
  uint32_t current_file_version;
  uint32_t requested_image_size;
  uint32_t download_time;
};

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_ZIGBEE_Init(void);
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer);
void APP_ZIGBEE_TL_INIT(void);
void Pre_ZigbeeCmdProcessing(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ZIGBEE_H */

