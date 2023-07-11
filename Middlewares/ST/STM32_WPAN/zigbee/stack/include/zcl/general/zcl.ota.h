/**
 * @file zcl.ota.h
 * @heading Over-The-Air Upgrade
 * @brief ZCL Over-The-Air (OTA) Upgrade cluster header
 * ZCL 7 section 11
 * ZCL 8 section 11
 * @copyright Copyright [2014 - 2022] Exegin Technologies Limited. All rights reserved.
 */

/* @PICS.ZCL.OTA
 *
 * OTA Upgrade Image
 * OUI1 | See Test Spec | True
 * OUI2a | See Test Spec | False
 * OUI2b | See Test Spec | True
 *
 * OTA.S | Server | True
 * OTA.C | Client | True
 *
 * Server Attributes
 * OTA.S.Afffd | ClusterRevision | True
 * OTA.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * OTA.S.C01.Rsp | Query Next Image Request | True
 * OTA.S.C03.Rsp | Image Block Request | True
 * OTA.S.C04.Rsp | Image Page Request | False
 * OTA.S.C06.Rsp | Upgrade End Request | True
 * OTA.S.C08.Rsp | Query Device Specific File Request | False
 *
 * Commands Generated
 * OTA.S.C00.Tx | Image Notify | True
 * OTA.S.C02.Tx | Query Next Image Response | True
 * OTA.S.C05.Tx | Image Block Response | True
 * OTA.S.C07.Tx | Upgrade End Response | True
 * OTA.S.C09.Tx | Query Device Specific File Response | False
 * OOMTS6 | See Test Spec | False
 *
 * OTA Upgrade Policies
 * OUPS1 | See Test Spec | True
 * OUPS2 | See Test Spec | True | Customer application can override
 * OUPS3 | See Test Spec | False
 * OUPS4 | See Test Spec | True
 * OUPS5 | See Test Spec | False
 *
 * Image Notify Payload Types
 * INPT01 | See Test Spec | True
 * INPT02 | See Test Spec | True
 * INPT03 | See Test Spec | True
 *
 * Client Attributes
 * OTA.C.A0000 | UpgradeServerID | True
 * OTA.C.A0001 | FileOffset | True | Optional
 * OTA.C.A0002 | CurrentFileVersion | True | Optional
 * OTA.C.A0003 | CurrentZigBeeStackVersion | True | Optional
 * OTA.C.A0004 | DownloadedFileVersion | True | Optional
 * OTA.C.A0005 | DownloadedZigBeeStackVersion | True | Optional
 * OTA.C.A0006 | ImageUpgradeStatus | True | Optional
 * OTA.C.A0007 | ManufacturerID | True | Optional
 * OTA.C.A0008 | ImageTypeID | True | Optional
 * OTA.C.A0009 | MinimumBlockPeriod | True | Optional
 * OTA.C.A000a | ImageStamp | True | Optional
 * OTA.C.A000b | UpgradeActivationPolicy | True | Optional
 * OTA.C.A000c | UpgradeTimeoutPolicy | True | Optional
 * OTA.C.Afffd | ClusterRevision | True
 * OTA.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * OTA.C.C00.Rsp | Image Notify | True
 * OTA.C.C02.Rsp | Query Next Image Response | True
 * OTA.C.C05.Rsp | Image Block Response | True
 * OTA.C.C07.Rsp | Upgrade End Response | True
 * OTA.C.C09.Rsp | Query Device Specific File Response | False
 *
 * Commands Generated
 * OTA.C.C01.Tx | Query Next Image Request | True
 * OTA.C.C03.Tx | Image Block Request | True
 * OTA.C.C04.Tx | Image Page Request | False
 * OTA.C.C06.Tx | Upgrade End Request | True
 * OTA.C.C08.Tx | Query Device Specific File Request | False
 *
 * OTA Upgrade Policies
 * OUPC0a | See Test Spec | Depends on customer application
 * OUPC0b | See Test Spec | Depends on customer application
 * OUPC1 | See Test Spec | True
 * OUPC2 | See Test Spec | True
 * OUPC3 | See Test Spec | True
 * OUPC4 | See Test Spec | True
 * OUPC5 | See Test Spec | False
 * OUPC6 | See Test Spec | False
 * OUPC7 | See Test Spec | True
 * OUPC8 | See Test Spec | True
 * OUPC9 | See Test Spec | Depends on customer application
 * OUPC10 | See Test Spec | Depends on customer application
 * OUPC11 | See Test Spec | True
 * OUPC12 | See Test Spec | True
 */

#ifndef ZCL_OTA_H
# define ZCL_OTA_H

/* OTA cluster dependencies */
/* Prototype the structs we need in this header file, rather than including
 * other header files. This file is included by tools outside of zigbee.git. */
struct ZbZclClusterT;
struct ZbApsAddrT;
struct ZbZclCommandRspT;

/** OTA Upgrade Server Attribute IDs */
enum ZbZclOtaSvrAttrId {
    ZCL_OTA_ATTR_UPGRADE_SERVER_ID = 0x0000, /**< UpgradeServerID */
    ZCL_OTA_ATTR_FILE_OFFSET = 0x0001,
    /**< FileOffset (Optional).
     *
     * When writing a non-zero value to ZCL_OTA_ATTR_FILE_OFFSET,
     * the entire OTA Header must have already been received by this cluster.
     * This can be used to resume a transfer that was interrupted by the local device
     * being power cycled in the middle of an OTA download. The procedure in this case is
     * to start the transfer over (ZbZclOtaClientImageTransferStart) and during the first
     * write_image() callback to the application, you write a new value to the FileOffset
     * attribute before returning from the callback. The OTA Client will then request the
     * next block starting from the new FileOffset.
     *
     * During the write_image() callback, the application can also compare
     * that the current OTA header matches the previous one, if saved before the reset.
     *
     * Since some data will be skipped by the OTA Client after the reset, the application
     * must define it's own image_validate() callback. The default image_validate()
     * callback will have computed an invalid hash of the incoming data. */
    ZCL_OTA_ATTR_CURRENT_FILE_VERSION = 0x0002, /**< CurrentFileVersion (Optional) */
    ZCL_OTA_ATTR_CURRENT_STACK_VERSION = 0x0003, /**< CurrentZigBeeStackVersion (Optional) */
    ZCL_OTA_ATTR_DOWNLOAD_FILE_VERSION = 0x0004, /**< DownloadedFileVersion (Optional) */
    ZCL_OTA_ATTR_DOWNLOAD_STACK_VERSION = 0x0005, /**< DownloadedZigBeeStackVersion (Optional) */
    ZCL_OTA_ATTR_IMAGE_UPGRADE_STATUS = 0x0006,
    /**< ImageUpgradeStatus (Optional).
     *
     * Writing ZCL_OTA_STATUS_NORMAL (0) to this attribute will reset the cluster back to defaults. */
    ZCL_OTA_ATTR_MANUFACTURER_ID = 0x0007, /**< ManufacturerID (Optional) */
    ZCL_OTA_ATTR_IMAGE_TYPE_ID = 0x0008, /**< ImageTypeID (Optional) */
    ZCL_OTA_ATTR_MIN_BLOCK_PERIOD = 0x0009, /**< MinimumBlockPeriod (Optional) */
    ZCL_OTA_ATTR_IMAGE_STAMP = 0x000a, /**< ImageStamp (Optional) */
    ZCL_OTA_ATTR_UPGRADE_ACTIVATION_POLICY = 0x00b, /**< UpgradeActivationPolicy (Optional) */
    ZCL_OTA_ATTR_UPGRADE_TIMEOUT_POLICY = 0x000c /**< UpgradeTimeoutPolicy (Optional) */
};

/** OTA Upgrade Status Attribute Values (ImageUpgradeStatus) */
enum ZbZclOtaStatus {
    ZCL_OTA_STATUS_NORMAL = 0, /**< Normal */
    ZCL_OTA_STATUS_DOWNLOAD_IN_PROGRESS = 1, /**< Download in progress */
    ZCL_OTA_STATUS_DOWNLOAD_COMPLETE = 2, /**< Download complete */
    ZCL_OTA_STATUS_WAITING_TO_UPGRADE = 3, /**< Waiting to upgrade */
    ZCL_OTA_STATUS_COUNT_DOWN = 4, /**< Count down */
    ZCL_OTA_STATUS_WAIT_FOR_MORE = 5 /**< Waiting to Upgrade via External Event */
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

/** OTA Upgrade Zigbee Stack Version Values */
enum ZbZclOtaStackVersion {
    ZCL_OTA_STACK_VERSION_2006 = 0x0000, /**< ZigBee 2006 */
    ZCL_OTA_STACK_VERSION_2007 = 0x0001, /**< ZigBee 2007 */
    ZCL_OTA_STACK_VERSION_PRO = 0x0002, /**< ZigBee Pro */
    ZCL_OTA_STACK_VERSION_IP = 0x0003 /**< ZigBee IP */
};

/** OTA Upgrade UpgradeActivationPolicy enumerations */
enum ZbZclOtaActivationPolicy {
    ZCL_OTA_ACTIVATION_POLICY_SERVER = 0x00, /**< OTA Server Activation Allowed */
    ZCL_OTA_ACTIVATION_POLICY_OUT_OF_BAND = 0x01 /**< Out-of-band Activation Only */
};

/** OTA Upgrade UpgradeTimeoutPolicy enumerations */
enum ZbZclOtaTimeoutPolicy {
    ZCL_OTA_TIMEOUT_POLICY_APPLY_UPGRADE = 0x00, /**< Apply Upgrade After Timeout */
    ZCL_OTA_TIMEOUT_POLICY_DO_NOT_APPLY = 0x01 /**< Do not Apply Upgrade After Timeout */
};

/** OTA Upgrade Security Credential Version enumerations */
enum ZbZclOtaSecCredential {
    ZCL_OTA_SEC_CRED_SE_1_0 = 0x00, /**< SE 1.0 */
    ZCL_OTA_SEC_CRED_SE_1_1 = 0x01, /**< SE 1.1 */
    ZCL_OTA_SEC_CRED_SE_2_0 = 0x02, /**< SE 2.0 */
    ZCL_OTA_SEC_CRED_SE_1_2 = 0x03 /**< SE 1.2 */
};

/** OTA Upgrade Image Notify Command Payload enumerations */
enum ZbZclOtaImageNotifyCmd {
    ZCL_OTA_NOTIFY_TYPE_JITTER = 0x00, /**< Query jitter */
    ZCL_OTA_NOTIFY_TYPE_MFG_CODE = 0x01, /**< Query jitter and manufacturer code */
    ZCL_OTA_NOTIFY_TYPE_IMAGE_TYPE = 0x02, /**< Query jitter, manufacturer code, and image type */
    ZCL_OTA_NOTIFY_TYPE_FILE_VERSION = 0x03 /**< Query jitter, manufacturer code, image type, and new file version */
};

/** OTA Upgrade Field Control Hardware Version enumerations */
enum ZbZclOtaQueryFldCtrlHwVer {
    ZCL_OTA_QUERY_FIELD_CONTROL_HW_VERSION = 0x01 /**< Hardware Version */
};

/** OTA Upgrade Image Block Request Field Control Bitmask enumerations */
enum ZbZclOtaImageBlkReqFldCtrl {
    ZCL_OTA_IMAGE_BLOCK_FC_IEEE = 0x01, /**< Request node’s IEEE address Present */
    ZCL_OTA_IMAGE_BLOCK_FC_MAX_BLOCK = 0x02 /**< MinimumBlockPeriod present */
};

#define ZCL_OTA_HARDWARE_VERSION_UNDEFINED          ZCL_INVALID_UNSIGNED_16BIT

#define ZCL_OTA_HEADER_FILE_IDENTIFIER              0x0BEEF11E
#define ZCL_OTA_HEADER_FILE_VERSION                 0x0100

#define ZCL_OTA_FILE_VERSION_WILDCARD               0xffffffffU

/* The client SHOULD wait for a (upgrade) command from the server.
 * Note that value of 0xffffffff SHOULD not be used for RequestTime. */
#define ZCL_OTA_UPGRADE_TIME_WAIT                   0xffffffffU

#define ZCL_OTA_HEADER_LENGTH_MIN                   56U /* no optional fields */
#define ZCL_OTA_HEADER_LENGTH_MAX                   69U /* all optional fields */
#define ZCL_OTA_IMAGE_BLOCK_SUB_ELEMENT_HEADER      6U /* tag id [2] tag length [4] */

#define ZCL_OTA_IMAGE_BLOCK_REQ_HDR_SIZE_MIN        14U
#define ZCL_OTA_IMAGE_BLOCK_REQ_HDR_SIZE_MAX        24U /* + RequestNodeAddress, MinimumBlockPeriod */
#define ZCL_OTA_IMAGE_BLOCK_RSP_HDR_SIZE            14U /* size of Image Block Response ZCL excluding data */

/* If NWK secured only, then safe max = 66 - 3 - 14 = 49 */
#define ZCL_OTA_BLOCK_DATA_SIZE_NWKSEC_MAX \
    (ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE - ZCL_HEADER_MIN_SIZE - ZCL_OTA_IMAGE_BLOCK_RSP_HDR_SIZE)
/* If APS and NWK secured, then safe max = 57 - 3 -14 = 40 */
#define ZCL_OTA_BLOCK_DATA_SIZE_APSSEC_MAX \
    (ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - ZCL_HEADER_MIN_SIZE - ZCL_OTA_IMAGE_BLOCK_RSP_HDR_SIZE)

/** OTA Upgrade Header Fields structure */
struct ZbZclOtaHeader {
    uint32_t file_identifier; /**< OTA Upgrade file identifier */
    uint16_t header_version; /**< OTA Header version */
    uint16_t header_length; /**< OTA Header length */
    uint16_t header_field_control; /**< OTA Header Field control */
    uint16_t manufacturer_code; /**< Manufacturer code */
    uint16_t image_type; /**< Image type */
    uint32_t file_version; /**< File version */
    uint16_t stack_version; /**< Zigbee Stack version */
    uint8_t header_string[32]; /**< OTA Header string */
    uint32_t total_image_size; /**< Total Image size (including header) */
    uint8_t sec_credential_version; /**< Security credential version (Optional) */
    uint64_t file_destination; /**< Upgrade file destination (Optional) */
    uint16_t min_hardware_version; /**< Minimum hardware version (Optional) */
    uint16_t max_hardware_version; /**< Maximum hardware version (Optional) */
};

/** OTA Upgrade Header Field Control Bitmask enumerations */
enum ZbZclOtaHeaderFieldCtrlBitmask {
    ZCL_OTA_HEADER_FIELD_CONTROL_SECURITY_VERSION = 0x01, /**< Security Credential Version Present */
    ZCL_OTA_HEADER_FIELD_CONTROL_DEVICE_SPECIFIC = 0x02, /**< Device Specific File */
    ZCL_OTA_HEADER_FIELD_CONTROL_HARDWARE_VERSIONS = 0x04 /**< Hardware Versions Present */
};

/** OTA Header Image Definition structure */
struct ZbZclOtaImageDefinition {
    uint16_t manufacturer_code; /**< Manufacturer code */
    uint16_t image_type; /**< Image type */
    uint32_t file_version; /**< File version */
};

/** OTA Upgrade Image Types enumerations */
enum ZbZclOtaImageType {
    ZCL_OTA_IMAGE_TYPE_MFG_MIN = 0x0000, /**< Manufacturer Specific (start) */
    ZCL_OTA_IMAGE_TYPE_MFG_MAX = 0xffbf, /**< Manufacturer Specific (end) */
    ZCL_OTA_IMAGE_TYPE_CLI_SECURITY_CRED = 0xffc0, /**< Client Security credentials */
    ZCL_OTA_IMAGE_TYPE_CLI_CONFIG = 0xffc1, /**< Client Configuration */
    ZCL_OTA_IMAGE_TYPE_SERVER_LOG = 0xffc2, /**< Server Log */
    ZCL_OTA_IMAGE_TYPE_PICTURE = 0xffc3, /**< Picture */
    ZCL_OTA_IMAGE_TYPE_WILDCARD = 0xffff /**< Wild Card */
};

/** OTA Upgrade Tag Identifiers enumerations */
enum ZbZclOtaSubElementTag {
    ZCL_OTA_SUB_TAG_UPGRADE_IMAGE = 0x0000, /**< Upgrade Image */
    ZCL_OTA_SUB_TAG_ECDSA_SIG1 = 0x0001, /**< ECDSA Signature (Crypto Suite 1) */
    ZCL_OTA_SUB_TAG_ECDSA_CERT_1 = 0x0002, /**< ECDSA Signing Certificate (Crypto Suite 1) */
    ZCL_OTA_SUB_TAG_IMAGE_INTEGRITY_CODE = 0x0003, /**< Image Integrity Code */
    ZCL_OTA_SUB_TAG_PICTURE_DATA = 0x0004, /**< Picture Data */
    ZCL_OTA_SUB_TAG_ECDSA_SIG2 = 0x0005, /**< ECDSA Signature (Crypto Suite 2) */
    ZCL_OTA_SUB_TAG_ECDSA_CERT_2 = 0x0006 /**< ECDSA Signing Certificate (Crypto Suite 2) */
};
#define ZCL_OTA_SUB_TAG_TOTAL               7U

/** OTA Upgrade Image Data structure */
struct ZbZclOtaImageData {
    uint32_t file_offset; /**< File Offset */
    uint8_t data_size; /**< Data Size */
    uint8_t data[256]; /**< Data */
};

/** Image Block Response Command Payload with WAIT_FOR_DATA status structure */
struct ZbZclOtaImageWaitForData {
    uint32_t current_time; /**< Current time */
    uint32_t request_time; /**< Request time */
    uint16_t minimum_block_period; /**< MinimumBlock Period */
};

/** Upgrade End Response command structure */
struct ZbZclOtaEndResponseTimes {
    uint32_t current_time; /**< Current time */
    uint32_t upgrade_time; /**< Upgrade time */
};

/* OTA Client API */
/** OTA Upgrade callbacks configuration */
struct ZbZclOtaClientCallbacksT {
    void (*discover_complete)(struct ZbZclClusterT *cluster, enum ZclStatusCodeT status, void *arg);
    /**< Callback to Server, invoked on discovery of OTA Upgrade Server.
     * If NULL and ZbZclOtaClientDiscover is successful, the default handler
     * will automatically call ZbZclOtaClientQueryNextImageReq.
     */

    enum ZclStatusCodeT (*image_notify)(struct ZbZclClusterT *cluster,
        uint8_t payload_type, uint8_t jitter, struct ZbZclOtaImageDefinition *image_definition,
        struct ZbApsdeDataIndT *data_ind, struct ZbZclHeaderT *zcl_header);
    /**< Callback to Server, invoked on receipt of Image Notify Server command
     * If NULL, the OTA Client has default callback handlers to take care of a typical OTA firmware upgrade file.
     */

    void (*query_next)(struct ZbZclClusterT *cluster, enum ZclStatusCodeT status,
        struct ZbZclOtaImageDefinition *image_definition, uint32_t image_size, void *arg);
    /**< Callback to Server, invoked on receipt of Query Next Image Response command
     * Void return
     */

    enum ZclStatusCodeT (*update_raw)(struct ZbZclClusterT *cluster, uint8_t length, uint8_t *data, void *arg);
    /**< Raw image data is sent through this callback, to update running hash of image for example.
     * Normally, this callback should be left to use the cluster's default internal handler, but if
     * the application does define its own callback, it must also provide its own handler for the
     * 'image_validate' callback.
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
     * (e.g. ZCL_STATUS_FAILURE).
     */

    enum ZclStatusCodeT (*write_tag)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint16_t tag_id, uint32_t tag_length, uint8_t data_length, uint8_t *data, void *arg);
    /**< Callback to write tag sub-element information. Normally this is left as-is after the
     * cluster is allocated to use the default internal callback handler. The default handler will
     * call the 'write_image' callback to process any image tag data (i.e. the actual firmware binary).
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
     * (e.g. ZCL_STATUS_FAILURE).
     */

    enum ZclStatusCodeT (*write_image)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint8_t length, uint8_t *data, void *arg);
    /**< If set, this is called by the cluster's default 'write_tag' handler for
     * ZCL_OTA_SUB_TAG_UPGRADE_IMAGE data. If the application has defined a different 'write_tag'
     * handler, then this callback will never be called. In that case, the handling of image tags
     * would take place in the 'write_tag' callback.
     *
     * The return status codes tell the OTA Client what to do next:
     *      ZCL_STATUS_SUCCESS              : request the next block
     *      ZCL_STATUS_WAIT_FOR_DATA        : wait before requesting the next block.
     *      Anything else                   : abort the OTA download
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
     * (e.g. ZCL_STATUS_FAILURE).
     */

    enum ZclStatusCodeT (*image_validate)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header, void *arg);
    /**< If NULL, provide the ca_pub_key_array in ZbZclOtaClientConfig.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*upgrade_end)(struct ZbZclClusterT *cluster, struct ZbZclOtaHeader *header,
        uint32_t current_time, uint32_t upgrade_time, void *arg);
    /**< Callback to handle Upgrade End Request command. The application should update the
     * ZCL_OTA_ATTR_IMAGE_UPGRADE_STATUS attribute accordingly. Writing ZCL_OTA_STATUS_NORMAL to
     * the ZCL_OTA_ATTR_IMAGE_UPGRADE_STATUS attribute will reset the cluster back to defaults.
     * If the callback returns ZCL_STATUS_SUCCESS, the cluster data is not reset, since the OTA
     * upgrade may be waiting to finish through an external event or timeout.
     * If the callback returns a ZCL error status (e.g. ZCL_STATUS_FAILURE), the cluster is
     * reset back to defaults.
     */

    void (*reboot)(struct ZbZclClusterT *cluster, void *arg);
    /**< Shall not be NULL. If set, called by the 'upgrade_end' default handler if successful.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*abort_download)(struct ZbZclClusterT *cluster, enum ZbZclOtaCommandId commandId, void *arg);
    /**< Callback to inform application of a problem with the download. It gives the application
     * the option to perform network diagnostics or repairs (e.g. NWK Route Discovery) before
     * attempting to restart the OTA transfer where it left off. The application can also tell the
     * OTA Client to abort this transfer by returning ZCL_STATUS_SUCCESS.
     * This callback cannot be set to NULL.
     *
     * The commandId is the OTA command responsible for aborting the transmission
     * (e.g. ZCL_OTA_COMMAND_IMAGE_BLOCK_RESPONSE).
     *
     * If the application returns ZCL_STATUS_SUCCESS, the OTA transfer will be aborted and cluster
     * reset to defaults, ready for a new transfer.
     * If the application returns an error (i.e. ZCL_STATUS_FAILURE), the OTA transfer will be paused
     * rather than aborted, and can be resumed by the application calling ZbZclOtaClientImageTransferResume.
     */
};

/** OTA Upgrade Client Configuration structure */
struct ZbZclOtaClientConfig {
    uint16_t profile_id; /**< Profile ID */
    uint8_t endpoint; /**< Endpoint on which the cluster is created on */

    enum ZbZclOtaActivationPolicy activation_policy; /**< Activation Policy */
    enum ZbZclOtaTimeoutPolicy timeout_policy; /**< Timeout Policy - Value written to ZCL_OTA_ATTR_UPGRADE_TIMEOUT_POLICY */
    uint32_t image_block_delay; /**< Image block delay - milliseconds */

    /* App needs to provide information about the current running image and hardware. */
    struct ZbZclOtaHeader current_image; /**< OTA Upgrade Header Fields structure */
    uint16_t hardware_version; /**< Hardware version - Version (0xNN00) | Revision (0x00NN) */

    /* If the image_validate() callback is set to the default callback for Suite 2
     * (by ZbZclOtaClientGetDefaultCallbacks), it requires the OTA Certificate
     * Authority's Public Key. */
    const uint8_t *ca_pub_key_array; /**< CA Public Key array */
    unsigned int ca_pub_key_len; /**< CA Public Key array length */

    /* OTA client callbacks. Can be intialzied by calling ZbZclOtaClientGetDefaultCallbacks(). */
    struct ZbZclOtaClientCallbacksT callbacks; /**< Client callback functions */
};

/**
 * Load the default callbacks for ECDSA Suite 2 support
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @return Void
 */
void ZbZclOtaClientGetDefaultCallbacks(struct ZbZclOtaClientCallbacksT *callbacks);

/**
 * Create a new instance of the OTA Upgrade Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param config Pointer to OTA client configuration structure
 * @param arg Pointer to application data provided in initiating API call
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOtaClientAlloc(struct ZigBeeT *zb, struct ZbZclOtaClientConfig *config, void *arg);

/**
 * Discover OTA Upgrade Server
 * @param cluster Cluster instance from which to send this command
 * @param addr Destination address for discovery
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaClientDiscover(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *addr);

/**
 * Set the OTA Upgrade Server directly (without discovery)
 * @param cluster Cluster instance from which to send this command
 * @param ieee OTA Upgrade server IEEE address
 * @param endpoint Endpoint on which to create cluster
 * @return Void
 */
void ZbZclOtaClientDiscoverForced(struct ZbZclClusterT *cluster, uint64_t ieee, uint8_t endpoint);

/**
 * Send a Query Next Image Request command
 * @param cluster Cluster instance from which to send this command
 * @param image_definition OTA Header Image Definition structure
 * @param field_control Field Control
 * @param hardware_version Hardware Version
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaClientQueryNextImageReq(struct ZbZclClusterT *cluster,
    struct ZbZclOtaImageDefinition *image_definition, uint8_t field_control,
    uint16_t hardware_version);

/**
 * Initiate an OTA Upgrade transfer.
 * @param cluster Cluster instance from which to send this command
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaClientImageTransferStart(struct ZbZclClusterT *cluster);

/**
 * Resume an OTA Upgrade transfer.
 * ZbZclOtaClientImageTransferResume is called if the NHLE (app) returned
 * ZCL_STATUS_WAIT_FOR_DATA for the write_image callback. The OTA Client won't request
 * the next block of data in this case, until this function is called.
 * @param cluster Cluster instance from which to send this command
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaClientImageTransferResume(struct ZbZclClusterT *cluster);

/* OTA Upgrade Server */

/* OTA Upgrade Server callbacks */
/**
 * Callback to determine if a suitable image exists
 * Implements policies for the OTA server
 * @param query_image OTA Header Image Definition structure
 * @param field_control Field Control
 * @param hardware_version Hardware Version
 * @param image_size Size of Image
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return True if image exists, false otherwise
 */
typedef bool (*ZbZclOtaServerImageEvalT)(struct ZbZclOtaImageDefinition *query_image,
    uint8_t field_control, uint16_t hadware_version, uint32_t *image_size, void *arg);

/**
 * Callback to read image data from the server
 * @param image OTA Header Image Definition structure
 * @param image_data OTA Upgrade Image Data structure
 * @param field_control Field Control
 * @param request_node_address Address of requestor
 * @param image_wait Image Block Response Command Payload with WAIT_FOR_DATA status structure
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
typedef enum ZclStatusCodeT (*ZbZclOtaServerImageReadT)(struct ZbZclOtaImageDefinition *image,
    struct ZbZclOtaImageData *image_data, uint8_t field_control, uint64_t request_node_address,
    struct ZbZclOtaImageWaitForData *image_wait, void *arg);

/**
 * Callback to handle a SUCCESS response from the client from a Server Upgrade End Response command
 * @param image OTA Upgrade Header Fields structure
 * @param current_time Current Time
 * @param upgrade_time Upgrade Time
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
typedef enum ZclStatusCodeT (*ZbZclOtaServerUpgradeEndTimeT)(struct ZbZclOtaHeader *image,
    uint32_t *current_time, uint32_t *upgrade_time, void *arg);

/**
 * Callback to handle an Upgrade End Request command
 * @param image_definition OTA Header Image Definition structure
 * @param status ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 * @param end_response_times Upgrade End Response command structure
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
typedef enum ZclStatusCodeT (*ZbZclOtaServerUpgradeEndReqT)(
    struct ZbZclOtaImageDefinition *image_definition,
    enum ZclStatusCodeT *status, struct ZbZclOtaEndResponseTimes *end_response_times, void *arg);

/** OTA Upgrade Server Configuration structure */
struct ZbZclOtaServerConfig {
    uint16_t profile_id; /**< Profile ID */
    uint8_t endpoint; /**< Endpoint on which the cluster is created on */
    uint16_t minimum_block_period; /**< Minimum block period */
    uint32_t upgrade_end_current_time; /**< Upgrade End Current Time */
    uint32_t upgrade_end_upgrade_time; /**< Upgrade End Upgrade Time */
    ZbZclOtaServerImageEvalT image_eval; /**< Callback to determine if a suitable image exists */
    ZbZclOtaServerImageReadT image_read; /**< Callback read image data from the server */
    ZbZclOtaServerUpgradeEndReqT image_upgrade_end_req; /**< Callback to handle client Upgrade End request */
    void *arg; /**< Pointer to application data that will later be provided back to the callback function when invoked */
};

/**
 * Create a new instance of the OTA Upgrade Server cluster
 * @param zb Zigbee stack instance
 * @param config OTA Upgrade Server Configuration structure
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOtaServerAlloc(struct ZigBeeT *zb, struct ZbZclOtaServerConfig *config, void *arg);

/**
 * Send an OTA Image Notify Server command
 * Registering an image does not automatically send an Image Notify message, the OTA Server application can use
 * ZbZclOtaServerImageNotifyReq after registering an image to notify clients of the availability of a new image
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param payload_type Payload Type
 * @param jitter Jitter
 * @param image_definition OTA Header Image Definition structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaServerImageNotifyReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint8_t payload_type, uint8_t jitter, struct ZbZclOtaImageDefinition *image_definition);

/**
 * Send an OTA Upgrade End Response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param image_definition OTA Header Image Definition structure
 * @param end_response_times Upgrade End Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOtaServerUpgradeEndResp(struct ZbZclClusterT *cluster, const struct ZbApsAddrT dst,
    struct ZbZclOtaImageDefinition *image_definition, struct ZbZclOtaEndResponseTimes end_response_times);

/* OTA Upgrade Helper Functions */
/**
 * Parse an OTA Upgrade payload buffer's header information
 * @param payload OTA Upgrade Payload buffer
 * @param length Length of OTA Upgrade Payload
 * @param header OTA Upgrade Header Fields structure
 * @return Number of bytes parsed
 */
uint8_t ZbZclOtaHeaderParse(const uint8_t *payload, const uint8_t length, struct ZbZclOtaHeader *header);

#endif
