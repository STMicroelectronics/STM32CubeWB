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
#include "zcl/zcl.h"
#include "zcl/general/zcl.ota.h"
  
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
#define IMAGE_TYPE_FW_COPRO_WIRELESS           0x01    /* M0 binary  */
#define IMAGE_TYPE_FW_APP                      0x02    /* M4 binary  */
#define FILE_VERSION_FW_COPRO_WIRELESS         0x02
#define FILE_VERSION_FW_APP                    0x02
#define RAM_FIRMWARE_BUFFER_SIZE               1024

#define OTA_NB_SIMULTANEOUS_TRANSFER           6u     /* Number of OTA Client that can be download in same time. */
  

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

/*
 *  OTA structures definition
 */
enum APP_ZIGBEE_OtaFileTypeDef_t{
  fileType_COPRO_WIRELESS = IMAGE_TYPE_FW_COPRO_WIRELESS,
  fileType_APP = IMAGE_TYPE_FW_APP,
};

struct APP_ZIGBEE_OtaContext_t{
  enum APP_ZIGBEE_OtaFileTypeDef_t  file_type;
  uint8_t   file_version;
  uint32_t  binary_size;
  uint32_t  base_address;
  uint32_t  binary_crc;
  uint32_t  magic_keyword;
};

struct APP_ZIGBEE_OtaBlocTransfer_t{
  //uint64_t  dlClientExtAddress;  
  //uint32_t  flash_addr;  
  bool      OTA_FileDataGenerated;
  uint32_t  header_size;
  uint32_t  download_time;
  //uint64_t  max_buffer_offset;
  //uint8_t   firmware_buffer[RAM_FIRMWARE_BUFFER_SIZE];
  uint8_t   header_buffer[RAM_FIRMWARE_BUFFER_SIZE];
  
};

/* OTA server data */
struct APP_ZIGBEE_OtaServerInfo {
  struct ZbZclOtaHeader requested_image_header;
  struct APP_ZIGBEE_OtaContext_t ctx;
  //struct APP_ZIGBEE_OtaBlocTransfer_t block_transfer[OTA_NB_SIMULTANEOUS_TRANSFER];
  struct APP_ZIGBEE_OtaBlocTransfer_t block_transfer;
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

