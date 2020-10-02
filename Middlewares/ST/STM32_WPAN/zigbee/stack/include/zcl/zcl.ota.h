/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/* PICS.ZCL.OTA
 *
 * OTA Upgrade Image
 * OUI1 | True
 * OUI2a | False
 * OUI2b | True
 *
 * OTA.S | True
 * OTA.C | True
 *
 * Server Attributes
 * OTA.S.Afffd | True
 * OTA.S.Afffe | False
 *
 * Commands Received
 * OTA.S.C01.Rsp | True
 * OTA.S.C03.Rsp | True
 * OTA.S.C04.Rsp | False
 * OTA.S.C06.Rsp | True
 * OTA.S.C08.Rsp | False
 *
 * Commands Generated
 * OTA.S.C00.Tx | True
 * OTA.S.C02.Tx | True
 * OTA.S.C05.Tx | True
 * OTA.S.C07.Tx | True
 * OTA.S.C09.Tx | False
 * OOMTS6 | False
 *
 * OTA Upgrade Policies
 * OUPS1 | True
 * OUPS2 | True (customer application can override)
 * OUPS3 | False
 * OUPS4 | True
 * OUPS5 | False
 *
 * Image Notify Payload Types
 * INPT01 | True
 * INPT02 | True
 * INPT03 | True
 *
 * Client Attributes
 * OTA.C.A0000 | True
 * OTA.C.A0001 | True
 * OTA.C.A0002 | True
 * OTA.C.A0003 | True
 * OTA.C.A0004 | True
 * OTA.C.A0005 | True
 * OTA.C.A0006 | True
 * OTA.C.A0007 | True
 * OTA.C.A0008 | True
 * OTA.C.A0009 | True
 * OTA.C.A000a | True
 * OTA.C.A000b | True
 * OTA.C.A000c | True
 * OTA.C.Afffd | True
 * OTA.C.Afffe | False
 *
 * Commands Received
 * OTA.C.C00.Rsp | True
 * OTA.C.C02.Rsp | True
 * OTA.C.C05.Rsp | True
 * OTA.C.C07.Rsp | True
 * OTA.C.C09.Rsp | False
 *
 * Commands Generated
 * OTA.C.C01.Tx | True
 * OTA.C.C03.Tx | True
 * OTA.C.C04.Tx | False
 * OTA.C.C06.Tx | True
 * OTA.C.C08.Tx | False
 * OOMTC6 | False
 *
 * OTA Upgrade Policies
 * OUPC0a | depends on customer application
 * OUPC0b | depends on customer application
 * OUPC1 | True
 * OUPC2 | True
 * OUPC3 | True
 * OUPC4 | True
 * OUPC5 | False
 * OUPC6 | False
 * OUPC7 | True
 * OUPC8 | True
 * OUPC9 | depends on customer application
 * OUPC10 | depends on customer application
 * OUPC11 | True
 * OUPC12 | True
 */

#ifndef ZCL_OTA_H
# define ZCL_OTA_H

/* OTA cluster dependencies */
/* Prototype the structs we need in this header file, rather than including
 * other header files. This file is included by tools outside of zigbee.git. */
struct ZbZclClusterT;
struct ZbApsAddrT;
struct ZbZclCommandRspT;

/* OTA Upgrade Server Attribute IDs */
enum ZbZclOtaSvrAttrId {
    ZCL_OTA_ATTR_UPGRADE_SERVER_ID = 0x0000,
    ZCL_OTA_ATTR_FILE_OFFSET = 0x0001,
    ZCL_OTA_ATTR_CURRENT_FILE_VERSION = 0x0002,
    ZCL_OTA_ATTR_CURRENT_STACK_VERSION = 0x0003,
    ZCL_OTA_ATTR_DOWNLOAD_FILE_VERSION = 0x0004,
    ZCL_OTA_ATTR_DOWNLOAD_STACK_VERSION = 0x0005,
    ZCL_OTA_ATTR_IMAGE_UPGRADE_STATUS = 0x0006,
    ZCL_OTA_ATTR_MANUFACTURER_ID = 0x0007,
    ZCL_OTA_ATTR_IMAGE_TYPE_ID = 0x0008,
    ZCL_OTA_ATTR_MIN_BLOCK_PERIOD = 0x0009,
    ZCL_OTA_ATTR_IMAGE_STAMP = 0x000a,
    ZCL_OTA_ATTR_UPGRADE_ACTIVATION_POLICY = 0x00b,
    ZCL_OTA_ATTR_UPGRADE_TIMEOUT_POLICY = 0x000c
};

/* OTA Upgrade Status Attribute Values */
enum ZbZclOtaStatus {
    ZCL_OTA_STATUS_NORMAL = 0,
    ZCL_OTA_STATUS_DOWNLOAD_IN_PROGRESS = 1,
    ZCL_OTA_STATUS_DOWNLOAD_COMPLETE = 2,
    ZCL_OTA_STATUS_WAITING_TO_UPGRADE = 3,
    ZCL_OTA_STATUS_COUNT_DOWN = 4,
    ZCL_OTA_STATUS_WAIT_FOR_MORE = 5
};

/* OTA ZCL commands */
enum ZbZclOtaCommandId {
    ZCL_OTA_COMMAND_IMAGE_NOTIFY = 0x00,
    ZCL_OTA_COMMAND_QUERY_IMAGE_REQUEST = 0x01,
    ZCL_OTA_COMMAND_QUERY_IMAGE_RESPONSE = 0x02,
    ZCL_OTA_COMMAND_IMAGE_BLOCK_REQUEST = 0x03,
    ZCL_OTA_COMMAND_IMAGE_PAGE_REQUEST = 0x04,
    ZCL_OTA_COMMAND_IMAGE_BLOCK_RESPONSE = 0x05,
    ZCL_OTA_COMMAND_UPGRADE_END_REQUEST = 0x06,
    ZCL_OTA_COMMAND_UPGRADE_END_RESPONSE = 0x07,
    ZCL_OTA_COMMAND_QUERY_FILE_REQUEST = 0x08,
    ZCL_OTA_COMMAND_QUERY_FILE_RESPONSE = 0x09
};

/* OTA Upgrade Zigbee Stack Version Values */
enum ZbZclOtaStackVersion {
    ZCL_OTA_STACK_VERSION_2006 = 0x0000,
    ZCL_OTA_STACK_VERSION_2007 = 0x0001,
    ZCL_OTA_STACK_VERSION_PRO = 0x0002,
    ZCL_OTA_STACK_VERSION_IP = 0x0003
};

/* OTA Upgrade UpgradeActivationPolicy enumerations */
enum ZbZclOtaActivationPolicy {
    ZCL_OTA_ACTIVATION_POLICY_SERVER = 0x00,
    ZCL_OTA_ACTIVATION_POLICY_OUT_OF_BAND = 0x01
};

/* OTA Upgrade UpgradeTimeoutPolicy enumerations */
enum ZbZclOtaTimeoutPolicy {
    ZCL_OTA_TIMEOUT_POLICY_APPLY_UPGRADE = 0x00,
    ZCL_OTA_TIMEOUT_POLICY_DO_NOT_APPLY = 0x01
};

/* OTA Upgrade Security Credential Version enumerations */
enum ZbZclOtaSecCredential {
    ZCL_OTA_SEC_CRED_SE_1_0 = 0x00,
    ZCL_OTA_SEC_CRED_SE_1_1 = 0x01,
    ZCL_OTA_SEC_CRED_SE_2_0 = 0x02,
    ZCL_OTA_SEC_CRED_SE_1_2 = 0x03
};

/* OTA Upgrade Image Notify Command Payload enumerations */
enum ZbZclOtaImageNotifyCmd {
    ZCL_OTA_NOTIFY_TYPE_JITTER = 0x00,
    ZCL_OTA_NOTIFY_TYPE_MFG_CODE = 0x01,
    ZCL_OTA_NOTIFY_TYPE_IMAGE_TYPE = 0x02,
    ZCL_OTA_NOTIFY_TYPE_FILE_VERSION = 0x03
};

/* OTA Upgrade Field Control Hardware Version enumerations */
enum ZbZclOtaQueryFldCtrlHwVer {
    ZCL_OTA_QUERY_FIELD_CONTROL_HW_VERSION = 0x01
};

/* OTA Upgrade Image Block Request Field Control Bitmask enumerations */
enum ZbZclOtaImageBlkReqFldCtrl {
    ZCL_OTA_IMAGE_BLOCK_FC_IEEE = 0x01,
    ZCL_OTA_IMAGE_BLOCK_FC_MAX_BLOCK = 0x02
};

#define ZCL_OTA_HARDWARE_VERSION_UNDEFINED          ZCL_INVALID_UNSIGNED_16BIT

#define ZCL_OTA_HEADER_FILE_IDENTIFIER              0x0BEEF11E
#define ZCL_OTA_HEADER_FILE_VERSION                 0x0100
#define ZCL_OTA_HEADER_LENGTH_MIN                   56U /* no optional fields */
#define ZCL_OTA_HEADER_LENGTH_MAX                   69U /* all optional fields */
#define ZCL_OTA_IMAGE_BLOCK_SUB_ELEMENT_HEADER      6U /* tag id [2] tag length [4] */
#define ZCL_OTA_IMAGE_BLOCK_ZCL_MESSAGE_SIZE        14U /* size of Image Block Response ZCL excluding data */
#define ZCL_OTA_BLOCK_DATA_SIZE_MAX \
    (ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_OTA_IMAGE_BLOCK_ZCL_MESSAGE_SIZE)

struct ZbZclOtaHeader {
    /* Mandatory fields. */
    uint32_t file_identifier;
    uint16_t header_version;
    uint16_t header_length;
    uint16_t header_field_control;
    uint16_t manufacturer_code;
    uint16_t image_type;
    uint32_t file_version;
    uint16_t stack_version;
    uint8_t header_string[32];
    uint32_t total_image_size;
    /* Optional fields. */
    uint8_t sec_credential_version;
    uint64_t file_destination;
    uint16_t min_hardware_version;
    uint16_t max_hardware_version;
};

/* OTA Upgrade Header Field Control Bitmask enumerations */
enum ZbZclOtaHeaderFieldCtrlBitmask {
    ZCL_OTA_HEADER_FIELD_CONTROL_SECURITY_VERSION = 0x01,
    ZCL_OTA_HEADER_FIELD_CONTROL_DEVICE_SPECIFIC = 0x02,
    ZCL_OTA_HEADER_FIELD_CONTROL_HARDWARE_VERSIONS = 0x04
};

/* OTA Header Image Definition structure */
struct ZbZclOtaImageDefinition {
    uint16_t manufacturer_code;
    uint16_t image_type;
    uint32_t file_version;
};

/* OTA Upgrade Image Types enumerations */
enum ZbZclOtaImageType {
    ZCL_OTA_IMAGE_TYPE_MFG_MIN = 0x0000,
    ZCL_OTA_IMAGE_TYPE_MFG_MAX = 0xffbf,
    ZCL_OTA_IMAGE_TYPE_CLI_SECURITY_CRED = 0xffc0,
    ZCL_OTA_IMAGE_TYPE_CLI_CONFIG = 0xffc1,
    ZCL_OTA_IMAGE_TYPE_SERVER_LOG = 0xffc2,
    ZCL_OTA_IMAGE_TYPE_PICTURE = 0xffc3,
    ZCL_OTA_IMAGE_TYPE_WILDCARD = 0xffff
};

/* OTA Upgrade Tag Identifiers enumerations */
enum ZbZclOtaSubElementTag {
    ZCL_OTA_SUB_TAG_UPGRADE_IMAGE = 0x0000,
    ZCL_OTA_SUB_TAG_ECDSA_SIG1 = 0x0001,
    ZCL_OTA_SUB_TAG_ECDSA_CERT_1 = 0x0002,
    ZCL_OTA_SUB_TAG_IMAGE_INTEGRITY_CODE = 0x0003,
    ZCL_OTA_SUB_TAG_PICTURE_DATA = 0x0004,
    ZCL_OTA_SUB_TAG_ECDSA_SIG2 = 0x0005,
    ZCL_OTA_SUB_TAG_ECDSA_CERT_2 = 0x0006
};
#define ZCL_OTA_SUB_TAG_TOTAL               7U

/* OTA Upgrade Image Data structure */
struct ZbZclOtaImageData {
    uint32_t file_offset;
    uint8_t data_size;
    uint8_t data[256];
};

/* Image Block Response Command Payload with WAIT_FOR_DATA status structure */
struct ZbZclOtaImageWaitForData {
    uint32_t current_time;
    uint32_t request_time;
    uint16_t minimum_block_period;
};

/* Upgrade End Response command structure */
struct ZbZclOtaEndResponseTimes {
    uint32_t current_time;
    uint32_t upgrade_time;
};

/* OTA Client API */
/** OTA Upgrade callbacks configuration */
struct ZbZclOtaClientCallbacksT {
    /* discover_complete: If NULL and ZbZclOtaClientDiscover is successful, the default handler
     * will automatically call ZbZclOtaClientQueryNextImageReq. */
    enum ZclStatusCodeT (*discover_complete)(struct ZbZclClusterT *cluster, void *arg);

    /* If NULL, the OTA Client has default callback handlers to take care of a typical OTA
     * firmware upgrade file. */
    enum ZclStatusCodeT (*image_notify)(struct ZbZclClusterT *cluster,
        uint8_t payload_type, uint8_t jitter, struct ZbZclOtaImageDefinition *image_definition,
        struct ZbApsdeDataIndT *data_ind, struct ZbZclHeaderT *zcl_header);

    void (*query_next)(struct ZbZclClusterT *cluster, enum ZclStatusCodeT status,
        struct ZbZclOtaImageDefinition *image_definition, uint32_t image_size, void *arg);

    /* update_raw: Raw image data is sent through this callback, to update running hash of image for example. */
    enum ZclStatusCodeT (*update_raw)(struct ZbZclClusterT *cluster, uint8_t length, uint8_t *data, void *arg);

    enum ZclStatusCodeT (*write_tag)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint16_t tag_id, uint32_t tag_length, uint8_t data_length, uint8_t *data, void *arg);

    /* write_image: If set, called by the 'write_tag' default handler (ZbZclOtaClientImageWriteTagCb)
     * for ZCL_OTA_SUB_TAG_UPGRADE_IMAGE data. The return status codes tell the OTA Client what
     * to do next:
     *      ZCL_STATUS_SUCCESS              : request the next block
     *      ZCL_STATUS_WAIT_FOR_DATA        : wait for NHLE to call ??? before requesting the next block.
     *      Anything else                   : abort the OTA download */
    enum ZclStatusCodeT (*write_image)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint8_t length, uint8_t *data, void *arg);

    /* image_validate: If NULL, provide the ca_pub_key_array in ZbZclOtaClientConfig. */
    enum ZclStatusCodeT (*image_validate)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header, void *arg);

    enum ZclStatusCodeT (*upgrade_end)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint32_t current_time, uint32_t upgrade_time, void *arg);

    /* reboot: Shall not be NULL. If set, called by the 'upgrade_end' default handler if successful. */
    void (*reboot)(struct ZbZclClusterT *cluster, void *arg);

    /* abort_download: Shall not be NULL. */
    enum ZclStatusCodeT (*abort_download)(struct ZbZclClusterT *cluster, enum ZbZclOtaCommandId commandId, void *arg);
};

struct ZbZclOtaClientConfig {
    uint16_t profile_id;
    uint8_t endpoint;

    enum ZbZclOtaActivationPolicy activation_policy;
    enum ZbZclOtaTimeoutPolicy timeout_policy; /* Value written to ZCL_OTA_ATTR_UPGRADE_TIMEOUT_POLICY. */
    uint32_t image_block_delay; /* milliseconds */

    /* App needs to provide information about the current running image and hardware. */
    struct ZbZclOtaHeader current_image;
    uint16_t hardware_version; /* Version (0xNN00) | Revision (0x00NN) */

    /* If the image_validate() callback is set to the default callback for Suite 2
     * (by ZbZclOtaClientGetDefaultCallbacks), it requires the OTA Certificate
     * Authority's Public Key. */
    const uint8_t *ca_pub_key_array;
    unsigned int ca_pub_key_len;

    /* OTA client callbacks. Can be intialzied by calling ZbZclOtaClientGetDefaultCallbacks(). */
    struct ZbZclOtaClientCallbacksT callbacks;
};

/* Called to retrieve the default callbacks for ECDSA Suite 2 support. */
void ZbZclOtaClientGetDefaultCallbacks(struct ZbZclOtaClientCallbacksT *callbacks);

/* Allocate the OTA Client */
struct ZbZclClusterT * ZbZclOtaClientAlloc(struct ZigBeeT *zb, struct ZbZclOtaClientConfig *config, void *arg);

/* Callback "discover" is called once complete */
enum ZclStatusCodeT ZbZclOtaClientDiscover(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *addr);

void ZbZclOtaClientDiscoverForced(struct ZbZclClusterT *cluster, uint64_t ieee, uint8_t endpoint);

enum ZclStatusCodeT ZbZclOtaClientQueryNextImageReq(struct ZbZclClusterT *cluster,
    struct ZbZclOtaImageDefinition *image_definition, uint8_t field_control,
    uint16_t hardware_version);

enum ZclStatusCodeT ZbZclOtaClientImageTransferStart(struct ZbZclClusterT *cluster);
/* ZbZclOtaClientImageTransferResume is only called by the NHLE if it returned
 * ZCL_STATUS_WAIT_FOR_DATA for the write_image callback. */
enum ZclStatusCodeT ZbZclOtaClientImageTransferResume(struct ZbZclClusterT *cluster);

/*------------------------------------------------------------------------------
 * OTA Server
 *------------------------------------------------------------------------------
 */

/* Callback to determine if a suitable image exists. Implements policies for
 * the OTA server such as is a downgrade permitted.
 */
typedef bool (*ZbZclOtaServerImageEvalT)(struct ZbZclOtaImageDefinition *query_image,
    uint8_t field_control, uint16_t hadware_version, uint32_t *image_size, void *arg);

/* called to retrieve more image data from the server */
typedef enum ZclStatusCodeT (*ZbZclOtaServerImageReadT)(struct ZbZclOtaImageDefinition *image,
    struct ZbZclOtaImageData *image_data, uint8_t field_control, uint64_t request_node_address,
    struct ZbZclOtaImageWaitForData *image_wait, void *arg);

/* called when the client sends an Upgrade End request with status of SUCCESS
 * server responds with   */
typedef enum ZclStatusCodeT (*ZbZclOtaServerUpgradeEndTimeT)(struct ZbZclOtaHeader *image,
    uint32_t *current_time, uint32_t *upgrade_time, void *arg);

/* called when the client sends an Upgrade End request  */
typedef enum ZclStatusCodeT (*ZbZclOtaServerUpgradeEndReqT)(
    struct ZbZclOtaImageDefinition *image_definition,
    enum ZclStatusCodeT *status, struct ZbZclOtaEndResponseTimes *end_response_times, void *arg);

/* Server side configuration */
struct ZbZclOtaServerConfig {
    uint16_t profile_id;
    uint8_t endpoint;
    uint16_t minimum_block_period;
    uint32_t upgrade_end_current_time;
    uint32_t upgrade_end_upgrade_time;
    ZbZclOtaServerImageEvalT image_eval;
    ZbZclOtaServerImageReadT image_read;
    ZbZclOtaServerUpgradeEndReqT image_upgrade_end_req;
    void *arg; /* application argument to be returned in callbacks */
};

struct ZbZclClusterT * ZbZclOtaServerAlloc(struct ZigBeeT *zb, struct ZbZclOtaServerConfig *config, void *arg);

/* Registering an image does not automatically send an Image Notify message, the OTA Server application can use */
/* ZbZclOtaServerImageNotifyReq after registering an image to notify clients of the availability of a new image */
enum ZclStatusCodeT ZbZclOtaServerImageNotifyReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint8_t payload_type, uint8_t jitter, struct ZbZclOtaImageDefinition *image_definition);

/* The OTA server can send an upgrade end response unsolicited */
enum ZclStatusCodeT ZbZclOtaServerUpgradeEndResp(struct ZbZclClusterT *cluster, const struct ZbApsAddrT dst,
    struct ZbZclOtaImageDefinition *image_definition, struct ZbZclOtaEndResponseTimes end_response_times);

/*------------------------------------------------------------------------------
 * OTA Misc Helpers
 *------------------------------------------------------------------------------
 */
uint8_t ZbZclOtaHeaderParse(const uint8_t *payload, const uint8_t length, struct ZbZclOtaHeader *header);

#endif
