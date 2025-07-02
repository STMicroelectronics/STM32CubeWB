/**
  ******************************************************************************
  * File Name          : App/app_zigbee.c
  * Description        : Zigbee Application.
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "zigbee_interface.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "stm32_seq.h"

#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/zcl.payload.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.time.h"
#include "zcl/general/zcl.ota.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY          500U
#define SW1_ENDPOINT                           17
#define SW1_GROUP_ADDR                         0x0001
#define CHANNEL                                19
#define HW_TS_SERVER_1S_NB_TICKS               (1*1000*1000/CFG_TS_TICK_VAL)
#define OTA_CLIENT_UPGRADE_DELAY               5       /* 5s */

#define APP_ZIGBEE_PERMIT_JOIN_DELAY           60u   /* 60s */

/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);
static void APP_ZIGBEE_App_Init(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);

/* OnOff */
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_Off_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_On_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_Toggle_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);

/* Time */
static uint32_t APP_ZIGBEE_Time_Server_GetTime_cb(struct ZbZclClusterT *clusterPtr, void *arg);
static void APP_ZIGBEE_Time_Server_SetTime_cb(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg);

/* OTA */
static void APP_ZIGBEE_OTA_Server_Init(void);
static bool APP_ZIGBEE_OTA_Server_QueryNextImage_cb(struct ZbZclOtaImageDefinition *query_image, uint8_t field_control, 
                                             uint16_t hadware_version, uint32_t *image_size, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_OTA_Server_ImageBlock_cb(struct ZbZclOtaImageDefinition *image,  struct ZbZclOtaImageData *image_data, 
                                                        uint8_t field_control, uint64_t request_node_address, 
                                                        struct ZbZclOtaImageWaitForData *image_wait, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_OTA_Server_UpgradeEnd_cb(struct ZbZclOtaImageDefinition *image_definition, enum ZclStatusCodeT status, 
                                                               struct ZbZclOtaEndResponseTimes *end_response_times, struct ZbZclAddrInfoT *src_info, void *arg);

static void APP_ZIGBEE_OTA_Server_ImageNotify(void);
static int APP_ZIGBEE_OTA_Server_FindSuitableImage(struct ZbZclOtaImageDefinition *query);
static uint8_t APP_ZIGBEE_OTA_Server_HeaderMake(const struct ZbZclOtaHeader* ota_header, uint8_t* payload);
//static uint32_t APP_ZIGBEE_GetBinSize(void);
static bool     APP_ZIGBEE_GetBinInfo( uint32_t * lSize, uint32_t * lCrc );
static uint32_t GetFirstSecureSector(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);

/* Private variables -----------------------------------------------*/
static TL_CmdPacket_t *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t CptReceiveNotifyFromM0 = 0;
static __IO uint32_t CptReceiveRequestFromM0 = 0;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
uint8_t g_ot_notification_allowed = 0U;

/* OnOff server custom callbacks */
static struct ZbZclOnOffServerCallbacksT ZbZclOnOffServerCallbacks = {
  .off = APP_ZIGBEE_OnOff_Server_Off_cb,
  .on = APP_ZIGBEE_OnOff_Server_On_cb,
  .toggle = APP_ZIGBEE_OnOff_Server_Toggle_cb,
};

/* Time server custom callbacks */
static struct ZbZclTimeServerCallbacks ZbZclTimeServerCallbacks = {
  .get_time = APP_ZIGBEE_Time_Server_GetTime_cb,
  .set_time = APP_ZIGBEE_Time_Server_SetTime_cb,
};

static struct APP_ZIGBEE_OtaServerInfo OTA_server_info;

static struct ZbZclOtaHeader image_header_list[] = {
  /* M0 OTA header */
  {
    .file_identifier = ZCL_OTA_HEADER_FILE_IDENTIFIER,
    .header_version = ZCL_OTA_HEADER_FILE_VERSION,
    .header_length = ZCL_OTA_HEADER_LENGTH_MIN, /* minimum header size (no optional fields) */
    .header_field_control = 0, /* no optional fields in the header */
    .manufacturer_code = ST_ZIGBEE_MANUFACTURER_CODE,
    .image_type = IMAGE_TYPE_FW_COPRO_WIRELESS,
    .file_version = FILE_VERSION_FW_COPRO_WIRELESS,
    .stack_version = ZCL_OTA_STACK_VERSION_PRO,
    .header_string = "M0 binary",
    .total_image_size = 0,
  },
  
  /* M4 OTA header */
  {
    .file_identifier = ZCL_OTA_HEADER_FILE_IDENTIFIER,
    .header_version = ZCL_OTA_HEADER_FILE_VERSION,
    .header_length = ZCL_OTA_HEADER_LENGTH_MIN, /* minimum header size (no optional fields) */
    .header_field_control = 0, /* no optional fields in the header */
    .manufacturer_code = ST_ZIGBEE_MANUFACTURER_CODE,
    .image_type = IMAGE_TYPE_FW_APP,
    .file_version = FILE_VERSION_FW_APP,
    .stack_version = ZCL_OTA_STACK_VERSION_PRO,
    .header_string = "M4 binary",
    .total_image_size = 0,
  },
};

/* OTA server custom callbacks */
static struct ZbZclOtaServerConfig server_config = {
  .profile_id = ZCL_PROFILE_HOME_AUTOMATION,
  .endpoint = SW1_ENDPOINT,
  .image_eval = APP_ZIGBEE_OTA_Server_QueryNextImage_cb,
  .image_read = APP_ZIGBEE_OTA_Server_ImageBlock_cb,
  .image_upgrade_end_req = APP_ZIGBEE_OTA_Server_UpgradeEnd_cb,
  .arg = &OTA_server_info,
};

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;

  struct ZbZclClusterT *ota_server;
  struct ZbZclClusterT *time_server;
  struct ZbZclClusterT *onoff_server;
};
static struct zigbee_app_info zigbee_app_info;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  OTA server Query Next Image callback
 * @param  query_image: Sender image query definition pointer
 * @param  field_control: Additional value field control
 * @param  hadware_version: Optional hardware version parameter
 * @param  image_size: Pointer to image size value
 * @param  arg: Passed argument
 * @retval bool
 */
static bool APP_ZIGBEE_OTA_Server_QueryNextImage_cb(struct ZbZclOtaImageDefinition *query_image, uint8_t field_control, 
                                             uint16_t hadware_version, uint32_t *image_size, void *arg){
  struct APP_ZIGBEE_OtaServerInfo* server_info = (struct APP_ZIGBEE_OtaServerInfo*) arg;
  int i = 0;
    
  APP_DBG("[OTA] Query Next Image request received.");
  APP_DBG("[OTA] Sender device manufacturer code: 0x%04x.", query_image->manufacturer_code);
  APP_DBG("[OTA] Sender device current file version: 0x%08x.", query_image->file_version);
    
  if(field_control == ZCL_OTA_QUERY_FIELD_CONTROL_HW_VERSION){
    /* additional hardware info is provided by the sender */
    APP_DBG("[OTA] Sender device current hardware version: 0x%04x.", hadware_version);
  }
  APP_DBG("[OTA] Sender device is requesting an upgrade for 0x%04x image type.", query_image->image_type);
  
  i = APP_ZIGBEE_OTA_Server_FindSuitableImage(query_image);
  if(i<0){
    APP_DBG("[OTA] A such image is not available.\n");
    return false;
  }
  
  APP_DBG("[OTA] A such image is available.\n");
  *image_size = image_header_list[i].total_image_size;
  
  /* Keep header data to use it for transmitting ZCL OTA header in block transfer callback */
  server_info->requested_image_header = image_header_list[i];
  
  return true;
}


/**
 * @brief  OTA server Image Block callback
 * @param  image: Sender image query definition pointer
 * @param  image_data: Block query parameters pointer
 * @param  field_control: Additional value field control
 * @param  request_node_address: Sender OTA client extended address
 * @param  image_wait: Waiting parameters pointer
 * @param  arg: Passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_OTA_Server_ImageBlock_cb(struct ZbZclOtaImageDefinition * image,  struct ZbZclOtaImageData *image_data, 
                                                        uint8_t field_control, uint64_t request_node_address, 
                                                        struct ZbZclOtaImageWaitForData * image_wait, void * arg){
  
  struct APP_ZIGBEE_OtaServerInfo* server_info = (struct APP_ZIGBEE_OtaServerInfo*) arg;                                                                                                      
  uint8_t header_buffer[ZCL_OTA_HEADER_LENGTH_MAX];
  uint32_t  header_size, header_index;
  uint8_t upgrade_image_tag[ZCL_OTA_IMAGE_BLOCK_SUB_ELEMENT_HEADER];
  unsigned int tag_index;
  
  memset(image_data->data, 0, sizeof(image_data->data));
  
  //* Indicate progress all 1024 Bytes */
  if ( ( image_data->file_offset % RAM_FIRMWARE_BUFFER_SIZE ) < image_data->data_size ) 
  {
    /* Display Transfer Progress */
    APP_DBG("[OTA] FUOTA Transfer at address 0x%08x", ( server_info->ctx.base_address + image_data->file_offset ) );
  }
  
  /* Add ZCL OTA header info if not already done. Only one time */
  if ( server_info->block_transfer.OTA_FileDataGenerated == false )
  {
    /* Construction of Header */
    header_index = APP_ZIGBEE_OTA_Server_HeaderMake( &server_info->requested_image_header, header_buffer );
    if ( header_index != ( server_info->requested_image_header.header_length ) )
    {
        APP_DBG("Error, OTA header invalid.");
        return ZCL_STATUS_FAILURE;
    }
    
    // -- Copy Image Header */
    memcpy( server_info->block_transfer.header_buffer, header_buffer, header_index );
    
    /* Add associated tag to transmit firmware data as OTA sub-element */
    memset( upgrade_image_tag, 0, sizeof(upgrade_image_tag) );
  
    /* Tag ID */
    tag_index = 0;
    zb_zcl_append_uint16( upgrade_image_tag, sizeof(upgrade_image_tag), &tag_index, ZCL_OTA_SUB_TAG_UPGRADE_IMAGE);
  
    /* Length field */
    zb_zcl_append_uint32( upgrade_image_tag, sizeof(upgrade_image_tag), &tag_index, server_info->ctx.binary_size);
  
    memcpy( ( server_info->block_transfer.header_buffer + header_index ), upgrade_image_tag, sizeof(upgrade_image_tag) );
    header_index += tag_index;
   
    server_info->block_transfer.OTA_FileDataGenerated = true;
    server_info->block_transfer.header_size = header_index;
    
    server_info->block_transfer.download_time = HAL_GetTick();
  }
  
  /* -- First copy Header -- */
  if ( image_data->file_offset < server_info->block_transfer.header_size ) 
  {
    header_size = server_info->block_transfer.header_size - image_data->file_offset;
    if ( header_size >= image_data->data_size ) 
    {
      header_size = image_data->data_size;
      memcpy( image_data->data, &server_info->block_transfer.header_buffer[image_data->file_offset], header_size );
    }
    else
    {
      memcpy( image_data->data, &server_info->block_transfer.header_buffer[image_data->file_offset], header_size );
      memcpy( ( image_data->data + header_size ), (uint8_t *)( server_info->ctx.base_address ), ( image_data->data_size - header_size ) );
    }
  }
  else
  {
    /* Beyond the end of the image */
    if ( image_data->file_offset > server_info->requested_image_header.total_image_size ) 
    {
      APP_DBG("[OTA] Warning, offset requested (%d bytes) is too big regarding OTA file size (%d bytes)",
                image_data->file_offset, server_info->requested_image_header.total_image_size);
      return ZCL_STATUS_INVALID_VALUE;
    }
    
    /* If needed adjust the last block for the end of the image */
    if ( ( image_data->file_offset + image_data->data_size ) > server_info->requested_image_header.total_image_size) 
    {
      image_data->data_size = server_info->requested_image_header.total_image_size - image_data->file_offset;
    }

    /* Copy Data on Buffer */
    memcpy( image_data->data, (uint8_t *)( server_info->ctx.base_address + ( image_data->file_offset - server_info->block_transfer.header_size ) ), image_data->data_size );
  }
    
  return ZCL_STATUS_SUCCESS;                                             
}

/**
 * @brief  OTA server Upgrade End callback
 * @param  image_definition: Sender image query definition pointer
 * @param  status: ZCL upgrade end status pointer
 * @param  end_response_times: Upgrade end times pointer (current and upgrade time)
 * @param  arg: Passed argument
 * @retval ZCL status code
 */

static enum ZclStatusCodeT APP_ZIGBEE_OTA_Server_UpgradeEnd_cb(struct ZbZclOtaImageDefinition * image_definition, enum ZclStatusCodeT status, 
                                                               struct ZbZclOtaEndResponseTimes * end_response_times, struct ZbZclAddrInfoT *src_info, void *arg){
  uint32_t  upgrade_time;
  double    upgrade_troughput;
  struct APP_ZIGBEE_OtaServerInfo * server_info = (struct APP_ZIGBEE_OtaServerInfo *) arg;  
  uint32_t lTransfertThroughputInt, lTransfertThroughputDec;
  
  APP_DBG("**************************************************************\n");
  APP_DBG("[OTA] Upgrade End request received.");
  
  /* Show upgrade end status */
  switch (status) {
    case ZCL_STATUS_SUCCESS:
      upgrade_time = ( HAL_GetTick() - server_info->block_transfer.download_time ) / 1000;
      upgrade_troughput = (((double)server_info->requested_image_header.total_image_size / upgrade_time ) / 1000) * 8;
      lTransfertThroughputInt = (uint32_t)upgrade_troughput;
      lTransfertThroughputDec = (uint32_t)( ( upgrade_troughput - lTransfertThroughputInt ) * 100 );
      
      APP_DBG("UpgradeEnd status SUCCESS, responding with:");
      APP_DBG("  Upgrade time: %d", upgrade_time);
      //APP_DBG("  Average throughput: %.2f Kbits/s", upgrade_troughput);
      APP_DBG("  - Average throughput = %d.%d kbit/s.", lTransfertThroughputInt, lTransfertThroughputDec );
      break;

    case ZCL_STATUS_INVALID_IMAGE:
      APP_DBG("UpgradeEnd status INVALID_IMAGE");
      return ZCL_STATUS_INVALID_IMAGE;

    case ZCL_STATUS_REQUIRE_MORE_IMAGE:
      APP_DBG("UpgradeEnd status REQUIRE_MORE_IMAGE");
      return ZCL_STATUS_REQUIRE_MORE_IMAGE;

    case ZCL_STATUS_ABORT:
      APP_DBG("UpgradeEnd status ABORT");
      return ZCL_STATUS_ABORT;

    default:
      APP_DBG("UpgradeEnd UNKNOWN status 0x%02x", status);
      return ZCL_STATUS_FAILURE;
    }
  
  APP_DBG("**************************************************************\n");

  return ZCL_STATUS_SUCCESS;                                             
}

/**
 * @brief  Time server GetTime callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  arg: Passed argument
 * @retval None
 */
static uint32_t APP_ZIGBEE_Time_Server_GetTime_cb(struct ZbZclClusterT *clusterPtr, void *arg){
  uint32_t time = HAL_GetTick();
  return time;
}

/**
 * @brief  Time server SetTime callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  time_val: time value to set
 * @param  arg: Passed argument
 * @retval None
 */
static void APP_ZIGBEE_Time_Server_SetTime_cb(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg){
  
  /* Unused function */
  
  UNUSED(clusterPtr);
  UNUSED(time_val);
  UNUSED(arg);
}

/**
 * @brief  OnOff server Off callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  srcInfo: Sender address info
 * @param  arg: Passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_Off_cb(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *srcInfo, void *arg)
{
    uint8_t endpoint;

    endpoint = ZbZclClusterGetEndpoint(clusterPtr);
    if (endpoint == SW1_ENDPOINT) {
        APP_DBG("LED_RED OFF");
        BSP_LED_Off(LED_RED);
        (void)ZbZclAttrIntegerWrite(clusterPtr, ZCL_ONOFF_ATTR_ONOFF, 0);
    }
    else {
        /* Unknown endpoint */
        return ZCL_STATUS_FAILURE;
    }
    return ZCL_STATUS_SUCCESS;
}

/**
 * @brief  OnOff server On callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  srcInfo: Sender address info
 * @param  arg: Passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_On_cb(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *srcInfo, void *arg)
{
    uint8_t endpoint;

    endpoint = ZbZclClusterGetEndpoint(clusterPtr);
    if (endpoint == SW1_ENDPOINT) {
        APP_DBG("LED_RED ON");
        BSP_LED_On(LED_RED);
        (void)ZbZclAttrIntegerWrite(clusterPtr, ZCL_ONOFF_ATTR_ONOFF, 1);
    }
    else {
        /* Unknown endpoint */
        return ZCL_STATUS_FAILURE;
    }
    return ZCL_STATUS_SUCCESS;
}

/**
 * @brief  OnOff server Toggle callback
 * @param  clusterPtr: ZCL Cluster pointer
 * @param  srcInfo: Sender address info
 * @param  arg: Passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_OnOff_Server_Toggle_cb(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *srcInfo, void *arg)
{
    uint8_t attrVal;

    if (ZbZclAttrRead(clusterPtr, ZCL_ONOFF_ATTR_ONOFF, NULL,
            &attrVal, sizeof(attrVal), false) != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }
    if (attrVal != 0) {
        return APP_ZIGBEE_OnOff_Server_Off_cb(clusterPtr, srcInfo, arg);
    }
    else {
        return APP_ZIGBEE_OnOff_Server_On_cb(clusterPtr, srcInfo, arg);
    }
}

/**
 * @brief  OTA Server Image Notify command
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_OTA_Server_ImageNotify(void){
  enum ZclStatusCodeT status = ZCL_STATUS_SUCCESS;
  struct ZbApsAddrT dst;
  struct ZbZclOtaImageDefinition image_definition;
  uint32_t  size;
  uint32_t  crc = 0;
  int       pos = 0; 
  
  /* We have to prevent OTA client devices that a new version is available */
  
  /* Destination configuration */
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = 255; /* Broadcast => any endpoint */
  dst.nwkAddr = 0xffff; /* Broadcast mode */ 
  
  /* Image definition configuration */
  memset(&image_definition, 0, sizeof(image_definition));
  image_definition.manufacturer_code = ST_ZIGBEE_MANUFACTURER_CODE;
  image_definition.image_type = OTA_server_info.ctx.file_type;
  
  pos = APP_ZIGBEE_OTA_Server_FindSuitableImage(&image_definition);
  if ( pos < 0 )
  {
    APP_DBG("[OTA] A such image is not available.");
    return;
  }
  
  image_definition.file_version = OTA_server_info.ctx.file_version;
  
  /* Obtain binary size from the binary stored in Flash */
  if ( APP_ZIGBEE_GetBinInfo( &size, &crc ) == FALSE )
  {
     APP_DBG("[OTA] FUTOA not found.");
     return;
  }
  OTA_server_info.ctx.binary_size = size;
  OTA_server_info.ctx.binary_crc = crc;
  
  /* Update total image size of the OTA file 
     (will be dynamically generated from binary stored in Flash ) */
  image_header_list[pos].total_image_size = size  /* binary size */
    + image_header_list[pos].header_length        /* header size */
    + ZCL_OTA_IMAGE_BLOCK_SUB_ELEMENT_HEADER;     /* upgrade tag size */
    
  APP_DBG("[OTA] Binary CRC: 0x%04X.", OTA_server_info.ctx.binary_crc);
  APP_DBG("[OTA] Binary size: %d bytes.", OTA_server_info.ctx.binary_size);
  APP_DBG("[OTA] OTA binary total size: %d bytes.", image_header_list[pos].total_image_size);
  
  /* Reset all Transfer Information */
  OTA_server_info.block_transfer.header_size = 0x00;
  OTA_server_info.block_transfer.OTA_FileDataGenerated = FALSE;
  
  /* Sending an Image Notify request to the client 
   * QueryJitter parameter set to 0 => No flooding prevention */
  APP_DBG("[OTA] Sending Image Notify request to all OTA client devices (broadcast mode).\n");
  status = ZbZclOtaServerImageNotifyReq(zigbee_app_info.ota_server, &dst, 
                                        ZCL_OTA_NOTIFY_TYPE_FILE_VERSION, 0, &image_definition);
  if(status != ZCL_STATUS_SUCCESS){
    APP_DBG("ZbZclOtaServerImageNotifyReq failed.\n");
    assert(0);
  }
}

/**
 * @brief  OTA Server find image helper
 * @param  query: image definition requested
 * @retval image index value
 */
static int APP_ZIGBEE_OTA_Server_FindSuitableImage(struct ZbZclOtaImageDefinition *query){
    struct ZbZclOtaHeader *image;

    for (uint32_t i = 0; i < (sizeof(image_header_list)/sizeof(*image_header_list)); i++) {
        image = &image_header_list[i];

        /* allow either wildcard or exact match */
        if ((query->manufacturer_code != ZB_MFG_CODE_WILDCARD)
            && (image->manufacturer_code != query->manufacturer_code)) {
            continue;
        }

        /* allow either wildcard or exact match */
        if ((query->image_type != ZCL_OTA_IMAGE_TYPE_WILDCARD)
            && (image->image_type != query->image_type)) {
            continue;
        }

        /* allow both upgrade and downgrade but not exactly same version */
        if (image->file_version == query->file_version) {
            continue;
        }

        return i;
    }

    return -1;
}

/**
 * @brief  OTA Server OTA file format header generation helper
 * @param  ota_header: OTA header structure
 * @param  payload: resulting OTA header in binary format
 * @retval Payload index value
 */
static uint8_t APP_ZIGBEE_OTA_Server_HeaderMake(const struct ZbZclOtaHeader* ota_header, uint8_t* payload)
{
    unsigned int index = 0;
    uint8_t length = sizeof(struct ZbZclOtaHeader);
    
    /* Check OTA header file identifier */
    if (ota_header->file_identifier != ZCL_OTA_HEADER_FILE_IDENTIFIER) {
        return 0;
    }

    if (zb_zcl_append_uint32(payload, length, &index, ota_header->file_identifier) < 0) {
        return 0;
    }
 
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->header_version) < 0) {
        return index;
    }
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->header_length) < 0) {
        return index;
    }
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->header_field_control) < 0) {
        return index;
    }
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->manufacturer_code) < 0) {
        return index;
    }
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->image_type) < 0) {
        return index;
    }
    if (zb_zcl_append_uint32(payload, length, &index, ota_header->file_version) < 0) {
        return index;
    }
    if (zb_zcl_append_uint16(payload, length, &index, ota_header->stack_version) < 0) {
        return index;
    }
    if (index + 32 > length) {
        return index;
    }
    
    (void)memcpy(payload + index, ota_header->header_string, 32);
    index += 32;

    if (zb_zcl_append_uint32(payload, length, &index, ota_header->total_image_size) < 0) {
        return index;
    }

    /* optional fields are based field control */
    if (ota_header->header_field_control & ZCL_OTA_HEADER_FIELD_CONTROL_SECURITY_VERSION) {
        if (zb_zcl_append_uint8(payload, length, &index, ota_header->sec_credential_version) < 0) {
            return index;
        }
    }
    if (ota_header->header_field_control & ZCL_OTA_HEADER_FIELD_CONTROL_DEVICE_SPECIFIC) {
        if (zb_zcl_append_uint64(payload, length, &index, ota_header->file_destination) < 0) {
            return index;
        }
    }
    if (ota_header->header_field_control & ZCL_OTA_HEADER_FIELD_CONTROL_HARDWARE_VERSIONS) {
        if (zb_zcl_append_uint16(payload, length, &index, ota_header->min_hardware_version) < 0) {
            return index;
        }
        if (zb_zcl_append_uint16(payload, length, &index, ota_header->max_hardware_version) < 0) {
            return index;
        }
    }
    return index;
}

/**
 * @brief  Getting embedded FW binary size helper
 * @param  None
 * @retval Size
 */
//static uint32_t APP_ZIGBEE_GetBinSize(void)
static bool APP_ZIGBEE_GetBinInfo( uint32_t * lSize, uint32_t * lCrc )
{
  uint32_t flash_current_offset = 0;
  uint64_t read64 = 0;
  bool binary_parsing_on_going = TRUE;
  bool no_error_keyword_or_size = TRUE;
  uint32_t first_secure_sector_idx;
  uint32_t maximum_reachable_offset;

  first_secure_sector_idx = GetFirstSecureSector();

  /* Compute Maximum reachable address */
  maximum_reachable_offset = ( first_secure_sector_idx * 0x1000u ) - 1u;
  APP_DBG("maximum_reachable_offset =  0x%x", maximum_reachable_offset);

  while ( binary_parsing_on_going == TRUE )
  {
    read64 = *(uint64_t*)( OTA_server_info.ctx.base_address + flash_current_offset );

    /* Test if current word contains magic keyword */
    if ( ( ( read64 & 0x00000000FFFFFFFFu ) == OTA_server_info.ctx.magic_keyword )
        || ( ( ( read64 & 0xFFFFFFFF00000000u ) >> 32u ) == OTA_server_info.ctx.magic_keyword ) )
    {
      binary_parsing_on_going = FALSE;
      
      // -- CRC have only magic Keyword --
      if ( ( read64 & 0x00000000FFFFFFFFu ) == OTA_server_info.ctx.magic_keyword )
      {
        *lCrc = ( *lCrc ^ ( read64 & 0x00000000FFFFFFFFul ) );
      }
      else
      {
        *lCrc = ( *lCrc ^ ( read64 & 0x00000000FFFFFFFFul ) ) ^ ( ( read64 & 0xFFFFFFFF0000000ul ) >> 32u );
      }
    }
    else
    {
      *lCrc = ( *lCrc ^ ( read64 & 0x00000000FFFFFFFFul ) ) ^ ( ( read64 & 0xFFFFFFFF0000000ul ) >> 32u );
    }
    flash_current_offset += 8;

    /* Test current offset is still below SFSA (Non-Secure Memory Address) */
    if ( flash_current_offset > maximum_reachable_offset )
    {
      no_error_keyword_or_size = FALSE;
      binary_parsing_on_going = FALSE;
    }
  }

  *lSize = flash_current_offset;
  return no_error_keyword_or_size;
}

/**
 * @brief  Getting flash first secure sector helper
 * @param  None
 * @retval First flash secure sector 
 */
static uint32_t GetFirstSecureSector(void)
{
  uint32_t first_secure_sector_idx, sfsa_field, sbrv_field, sbrv_field_sector;

  /* Read SFSA */
  sfsa_field = (READ_BIT(FLASH->SFR, FLASH_SFR_SFSA) >> FLASH_SFR_SFSA_Pos);
  APP_DBG("SFSA OB = 0x%x", sfsa_field);
  APP_DBG("SFSA Option Bytes set to sector = 0x%x (0x080%x)", sfsa_field, sfsa_field*4096);

  /* Read SBRV */
  /* Contains the word aligned CPU2 boot reset start address offset within the selected memory area by C2OPT. */
  sbrv_field = (READ_BIT(FLASH->SRRVR, FLASH_SRRVR_SBRV) >> FLASH_SRRVR_SBRV_Pos);
  APP_DBG("SBRV OB = 0x%x", sbrv_field);
  /* Divide sbrv_field by 1024 to be compared to SFSA value */
  sbrv_field_sector = sbrv_field / 1024;
  APP_DBG("SBRV Option Bytes set to sector = 0x%x (0x080%x)", sbrv_field_sector, sbrv_field*4);

  /* If SBRV is below SFSA then set first_secure_sector_idx to SBRV */
  if (sbrv_field_sector < sfsa_field)
  {
    first_secure_sector_idx = sbrv_field_sector;
  }
  else
  {
    first_secure_sector_idx = sfsa_field;
  }

  APP_DBG("first_secure_sector_idx = 0x%x", first_secure_sector_idx);

  return first_secure_sector_idx;
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Task associated with OTA Server ImageNotify command */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_OTA_SERVER_IMAGE_NOTIFY, UTIL_SEQ_RFU, APP_ZIGBEE_OTA_Server_ImageNotify);
  
  /* Initialize Zigbee OTA server parameters */
  APP_ZIGBEE_OTA_Server_Init();
}

/**
 * @brief  OTA server initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_OTA_Server_Init(void){
  uint16_t  iShortAddress;
  
  iShortAddress = ZbShortAddress( zigbee_app_info.zb );
  APP_DBG("OTA Server with Short Address 0x%04X.", iShortAddress );
  APP_DBG("OTA Server init done!\n");
} /* APP_ZIGBEE_OTA_Server_Init */

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_Init(void)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;

  APP_DBG("APP_ZIGBEE_Init");

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_ZIGBEE_CheckWirelessFirmwareInfo();

  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();

  /* Register task */
  /* Create the different tasks */

  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessNotifyM0ToM4);
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessRequestM0ToM4);

  /* Task associated with network creation process */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, UTIL_SEQ_RFU, APP_ZIGBEE_NwkForm);
  
  /* Task associated with push button SW1, 2 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);

 /* Task associated with application init */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_START, UTIL_SEQ_RFU, APP_ZIGBEE_App_Init);
  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();
  
 

} /* APP_ZIGBEE_Init */

/**
 * @brief  Initialize Zigbee stack layers
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_StackLayersInit(void)
{
  APP_DBG("APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
  assert(zigbee_app_info.zb != NULL);

  /* Create the endpoint and cluster(s) */
  APP_ZIGBEE_ConfigEndpoints();

  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  /* run the task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_StackLayersInit */

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_METER_INTERFACE;

  /* Endpoint: SW1_ENDPOINT */
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);
  
  /* OnOff Server */
  zigbee_app_info.onoff_server = ZbZclOnOffServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &ZbZclOnOffServerCallbacks, NULL);
  assert(zigbee_app_info.onoff_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.onoff_server);
  
  /* Time server */
  zigbee_app_info.time_server = ZbZclTimeServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &ZbZclTimeServerCallbacks, NULL);
  assert(zigbee_app_info.time_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.time_server);

  /* OTA Server */
  zigbee_app_info.ota_server = ZbZclOtaServerAlloc(zigbee_app_info.zb, &server_config, &OTA_server_info);
  assert(zigbee_app_info.ota_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.ota_server); 
} /* APP_ZIGBEE_ConfigEndpoints */

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{
  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  {
    struct ZbStartupT config;
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    /* Set the centralized network */
    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_COORD");
    config.startupControl = ZbStartTypeForm;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);

    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /*Channel in use */

    /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
     * a blocking call on the M4. */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS) {
      zigbee_app_info.join_delay = 0U;
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      APP_DBG("Startup failed, re-attempting to form a network after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  else
  {
    /* Assign ourselves to the group addresses */
    APP_ZIGBEE_ConfigGroupAddr();

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
  }
  
  /* Starting application init task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_NwkForm */

/**
 * @brief  Set group addressing mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigGroupAddr(void)
{
  struct ZbApsmeAddGroupReqT req;
  struct ZbApsmeAddGroupConfT conf;

  memset(&req, 0, sizeof(req));
  req.endpt = SW1_ENDPOINT;
  req.groupAddr = SW1_GROUP_ADDR;
  ZbApsmeAddGroupReq(zigbee_app_info.zb, &req, &conf);

} /* APP_ZIGBEE_ConfigGroupAddr */

/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/
struct ZbStartupWaitInfo {
  bool active;
  enum ZbStatusCodeT status;
};

static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
  struct ZbStartupWaitInfo *info = cb_arg;

  info->status = status;
  info->active = false;
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_STARTUP_ENDED);
} /* ZbStartupWaitCb */

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL) {
    return ZB_STATUS_ALLOC_FAIL;
  }
  memset(info, 0, sizeof(struct ZbStartupWaitInfo));

  info->active = true;
  status = ZbStartup(zb, config, ZbStartupWaitCb, info);
  if (status != ZB_STATUS_SUCCESS)
  {
    free(info);
    return status;
  }
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_STARTUP_ENDED);
  status = info->status;
  free(info);
  return status;
} /* ZbStartupWait */

/**
 * @brief  Trace the error or the warning reported.
 * @param  ErrId :
 * @param  ErrCode
 * @retval None
 */
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode)
{
  switch (ErrId) {
  default:
    APP_ZIGBEE_TraceError("ERROR Unknown ", 0);
    break;
  }
} /* APP_ZIGBEE_Error */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief  Warn the user that an error has occurred.In this case,
 *         the LEDs on the Board will start blinking.
 *
 * @param  pMess  : Message associated to the error.
 * @param  ErrCode: Error code associated to the module (Zigbee or other module if any)
 * @retval None
 */
static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode)
{
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  while (1U == 1U) {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
} /* APP_ZIGBEE_TraceError */

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Zigbee
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t *p_wireless_info = &wireless_info_instance;
  
  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success) {
    APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch (p_wireless_info->StackType) {
    case INFO_STACK_TYPE_ZIGBEE_FFD:
      APP_DBG("FW Type : FFD Zigbee stack");
      break;
   case INFO_STACK_TYPE_ZIGBEE_RFD:
      APP_DBG("FW Type : RFD Zigbee stack");
      break;
    default:
      /* No Zigbee device supported ! */
      APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
     // print the application name
    char* __PathProject__ =(strstr(__FILE__, "Zigbee") ? strstr(__FILE__, "Zigbee") + 7 : __FILE__);
    char *del;
    if ( (strchr(__FILE__, '/')) == NULL)
        {del = strchr(__PathProject__, '\\');}
    else 
        {del = strchr(__PathProject__, '/');}
    
        int index = (int) (del - __PathProject__);
        APP_DBG("Application flashed: %*.*s",index,index,__PathProject__);
    
    //print channel
    APP_DBG("Channel used: %d", CHANNEL);
    //print Link Key
    APP_DBG("Link Key: %.16s", sec_key_ha);
    //print Link Key value hex   
    char Z09_LL_string[ZB_SEC_KEYSIZE*3+1];
    Z09_LL_string[0]=0;
    for(int str_index=0; str_index < ZB_SEC_KEYSIZE; str_index++)
      {           
        sprintf(&Z09_LL_string[str_index*3],"%02x ",sec_key_ha[str_index]);
      }
  
    APP_DBG("Link Key value: %s",Z09_LL_string);
        
    //print clusters allocated
    APP_DBG("Clusters allocated are:");  
    APP_DBG("OnOff Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("Time Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("OTA Server on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

/**
 * @brief  Button 1 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process(void)
{
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  /* Server info fields set to 0 */
  memset(&OTA_server_info, 0, sizeof(OTA_server_info));
  
  APP_DBG("[OTA] M4 application firmware upgrade available.\n");
  
  /*  Update OTA server context file type */
  OTA_server_info.ctx.file_type =  fileType_APP ;
  OTA_server_info.ctx.base_address = FUOTA_APP_FW_BINARY_ADDRESS;
  OTA_server_info.ctx.magic_keyword = FUOTA_MAGIC_KEYWORD_APP;
  OTA_server_info.ctx.file_version = FILE_VERSION_FW_APP;
    
  /* Launch Image Notify task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_OTA_SERVER_IMAGE_NOTIFY, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_SW1_Process */

/**
 * @brief  Button 2 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process(void)
{
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  /* Server info fields set to 0 */
  memset(&OTA_server_info, 0, sizeof(OTA_server_info));
  
  APP_DBG("[OTA] M0 wireless coprocessor firmware upgrade available.\n");
  
  /*  Update OTA server context file type */
  OTA_server_info.ctx.file_type = fileType_COPRO_WIRELESS;
  OTA_server_info.ctx.base_address = FUOTA_COPRO_FW_BINARY_ADDRESS;
  OTA_server_info.ctx.magic_keyword = FUOTA_MAGIC_KEYWORD_COPRO_WIRELESS;
  OTA_server_info.ctx.file_version = FILE_VERSION_FW_COPRO_WIRELESS;
    
  /* Launch Image Notify task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_OTA_SERVER_IMAGE_NOTIFY, CFG_SCH_PRIO_0);
    
} /* APP_ZIGBEE_SW2_Process */


/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer)
{
  p_ZIGBEE_otcmdbuffer = p_buffer;
} /* APP_ZIGBEE_RegisterCmdBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;
} /* ZIGBEE_Get_OTCmdPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdRspPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)((TL_EvtPacket_t *)p_ZIGBEE_otcmdbuffer)->evtserial.evt.payload;
} /* ZIGBEE_Get_OTCmdRspPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_NotificationPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_notif_M0_to_M4)->evtserial.evt.payload;
} /* ZIGBEE_Get_NotificationPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_M0RequestPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_request_M0_to_M4)->evtserial.evt.payload;
}

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransfer(void)
{
  Zigbee_Cmd_Request_t *cmd_req = (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;

  /* Zigbee OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.plen = 8U + (cmd_req->Size * 4U);

  TL_ZIGBEE_SendM4RequestToM0();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();
} /* ZIGBEE_CmdTransfer */

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0 with notification
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransferWithNotif(void)
{
	g_ot_notification_allowed = 1;
	ZIGBEE_CmdTransfer();
}

/**
 * @brief  This function is called when the M0+ acknowledge  the fact that it has received a Cmd
 *
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_CmdEvtReceived(TL_EvtPacket_t *Otbuffer)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();
} /* TL_ZIGBEE_CmdEvtReceived */

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_NotReceived(TL_EvtPacket_t *Notbuffer)
{
  p_ZIGBEE_notif_M0_to_M4 = Notbuffer;

  Receive_Notification_From_M0();
} /* TL_ZIGBEE_NotReceived */

/**
 * @brief  This function is called before sending any ot command to the M0
 *         core. The purpose of this function is to be able to check if
 *         there are no notifications coming from the M0 core which are
 *         pending before sending a new ot command.
 * @param  None
 * @retval None
 */
void Pre_ZigbeeCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
} /* Pre_ZigbeeCmdProcessing */

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
} /* Wait_Getting_Ack_From_M0 */

/**
 * @brief  Receive an acknowledgment from the M0+ core.
 *         Each command send by the M4 to the M0 are acknowledged.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Ack_From_M0(void)
{
  UTIL_SEQ_SetEvt(EVENT_ACK_FROM_M0_EVT);
} /* Receive_Ack_From_M0 */

/**
 * @brief  Receive a notification from the M0+ through the IPCC.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Notification_From_M0(void)
{
    CptReceiveNotifyFromM0++;
    UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief  This function is called when a request from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_M0RequestReceived(TL_EvtPacket_t *Reqbuffer)
{
    p_ZIGBEE_request_M0_to_M4 = Reqbuffer;

    CptReceiveRequestFromM0++;
    UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief Perform initialization of TL for Zigbee.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_TL_INIT(void)
{
    ZigbeeConfigBuffer.p_ZigbeeOtCmdRspBuffer = (uint8_t *)&ZigbeeOtCmdBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotAckBuffer = (uint8_t *)ZigbeeNotifRspEvtBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotifRequestBuffer = (uint8_t *)ZigbeeNotifRequestBuffer;
    TL_ZIGBEE_Init(&ZigbeeConfigBuffer);
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void)
{
  if (CptReceiveNotifyFromM0 != 0)
  {
    /* Reset counter */
    CptReceiveNotifyFromM0 = 0;
    Zigbee_CallBackProcessing();
  }
}

/**
 * @brief Process the requests coming from the M0.
 * @param None
 * @return None
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
    if (CptReceiveRequestFromM0 != 0) {
        CptReceiveRequestFromM0 = 0;
        Zigbee_M0RequestProcessing();
    }
}


