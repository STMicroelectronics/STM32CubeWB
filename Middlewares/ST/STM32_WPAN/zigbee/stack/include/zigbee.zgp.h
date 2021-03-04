/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZIGBEE_ZGP_H
# define ZIGBEE_ZGP_H

/*lint -save -e621 [ identifier clash within 31 chars <Rule 5.1, REQUIRED> ] */
/*lint -save -e955 [ param name missing from 'callback' prototype <Rule 8.2, REQUIRED> ] */

/* Identifier clash ('ZB_ZGP_COMMAND_MOVE_SATURATION_UP' <->'ZB_ZGP_COMMAND_MOVE_SATURATION_STOP' */

/* Known Profiles */
enum {
    ZB_ZGP_PROFILE_ID = 0xA1E0
};

/*---------------------------------------------------------------
 * Device IDs
 *---------------------------------------------------------------
 */
#define ZB_ZGP_DEVICE_PROXY                         (uint16_t)0x0060U
#define ZB_ZGP_DEVICE_PROXY_BASIC                   (uint16_t)0x0061U
#define ZB_ZGP_DEVICE_TARGET_PLUS                   (uint16_t)0x0062U
#define ZB_ZGP_DEVICE_TARGET                        (uint16_t)0x0063U
#define ZB_ZGP_DEVICE_COMMISSIONING_TOOL            (uint16_t)0x0064U
#define ZB_ZGP_DEVICE_COMBO                         (uint16_t)0x0065U
#define ZB_ZGP_DEVICE_COMBO_BASIC                   (uint16_t)0x0066U

/*---------------------------------------------------------------
 * Device Version
 *---------------------------------------------------------------
 */
#define ZB_ZGP_DEVICE_VER                           (uint8_t)0x00U

/*---------------------------------------------------------------
 * Cluster IDs
 *---------------------------------------------------------------
 */
#define ZB_ZGP_TYPE_DATA                            (uint8_t)0x00U
#define ZB_ZGP_TYPE_MAINTENANCE                     (uint8_t)0x01U

#define ZB_ZGP_APPID_SRCID                          (uint8_t)0x00U
#define ZB_ZGP_APPID_LPED                           (uint8_t)0x01U
#define ZB_ZGP_APPID_ENDPOINT                       (uint8_t)0x02U
/* Used to indicate invalid or unused entries. */
#define ZB_ZGP_APPID_INVALID                        (uint8_t)0xffU

#define ZB_ZGP_SRCID_UNSPECIFIED                    (uint32_t)0UL
#define ZB_ZGP_SRCID_BROADCAST                      (uint32_t)0xffffffffUL
#define ZB_ZGP_SRCID_MAX                            (uint32_t)0xfffffff8UL

#define ZB_ZGP_KEYTYPE_NONE                         (uint8_t)0x00U
#define ZB_ZGP_KEYTYPE_NWKKEY                       (uint8_t)0x01U /* ZigBee Network Key */
#define ZB_ZGP_KEYTYPE_GPD_GROUP                    (uint8_t)0x02U /* GPD Group Key */
#define ZB_ZGP_KEYTYPE_NWK_DERIVED                  (uint8_t)0x03U /* NWK-Key derived GPD Group Key: Kgp = HMAC(Knwk, "ZGP") */
#define ZB_ZGP_KEYTYPE_PRECONFIG                    (uint8_t)0x04U /* Pre-configured GPD security key. */
#define ZB_ZGP_KEYTYPE_INDIVIDUAL                   (uint8_t)0x07U /* Derived individual GPD key: Kgp = HMAC(K, ID) */

/* ZGP SecurityLevels */
#define ZB_ZGP_SECURITY_NONE                        (uint8_t)0x00U
#define ZB_ZGP_SECURITY_AUTH                        (uint8_t)0x02U
#define ZB_ZGP_SECURITY_ENCRYPT                     (uint8_t)0x03U

/* ZGP Security Status codes */
enum ZgpStatusCodeT {
    ZB_ZGP_STATUS_SUCCESS = 0x00,
    ZB_ZGP_STATUS_NO_SECURITY = 0x01,
    ZB_ZGP_STATUS_COUNTER_FAILURE = 0x02,
    ZB_ZGP_STATUS_AUTH_FAILURE = 0x03,
    ZB_ZGP_STATUS_UNPROCESSED = 0x04,
    /* Internal security status codes. */
    ZB_ZGP_STATUS_TX_THEN_DROP = 0x05,
    ZB_ZGP_STATUS_DROP_FRAME = 0x06,
    /* ZGP packet queue status codes. */
    ZB_ZGP_STATUS_TX_QUEUE_FULL = 0x07,
    ZB_ZGP_STATUS_ENTRY_REPLACED = 0x08,
    ZB_ZGP_STATUS_ENTRY_ADDED = 0x09,
    ZB_ZGP_STATUS_ENTRY_EXPIRED = 0x0a,
    ZB_ZGP_STATUS_ENTRY_REMOVED = 0x0b,
    ZB_ZGP_STATUS_INVALID_PARAMETER = 0x0c
};

/* GPDF Command Identifiers */
#define ZB_ZGP_COMMAND_IDENTIFY                     (uint8_t)0x00U
#define ZB_ZGP_COMMAND_RECALL_SCENE(_x_)            (uint8_t)(0x10U + (_x_))
#define ZB_ZGP_COMMAND_STORE_SCENE(_x_)             (uint8_t)(0x18U + (_x_))
#define ZB_ZGP_COMMAND_OFF                          (uint8_t)0x20U
#define ZB_ZGP_COMMAND_ON                           (uint8_t)0x21U
#define ZB_ZGP_COMMAND_TOGGLE                       (uint8_t)0x22U
#define ZB_ZGP_COMMAND_RELEASE                      (uint8_t)0x23U
#define ZB_ZGP_COMMAND_MOVE_UP                      (uint8_t)0x30U
#define ZB_ZGP_COMMAND_MOVE_DOWN                    (uint8_t)0x31U
#define ZB_ZGP_COMMAND_STEP_UP                      (uint8_t)0x32U
#define ZB_ZGP_COMMAND_STEP_DOWN                    (uint8_t)0x33U
#define ZB_ZGP_COMMAND_LEVEL_STOP                   (uint8_t)0x34U
#define ZB_ZGP_COMMAND_MOVE_UP_ONOFF                (uint8_t)0x35U
#define ZB_ZGP_COMMAND_MOVE_DOWN_ONOFF              (uint8_t)0x36U
#define ZB_ZGP_COMMAND_STEP_UP_ONOFF                (uint8_t)0x37U
#define ZB_ZGP_COMMAND_STEP_DOWN_ONOFF              (uint8_t)0x38U
#define ZB_ZGP_COMMAND_MOVE_HUE_STOP                (uint8_t)0x40U
#define ZB_ZGP_COMMAND_MOVE_HUE_UP                  (uint8_t)0x41U
#define ZB_ZGP_COMMAND_MOVE_HUE_DOWN                (uint8_t)0x42U
#define ZB_ZGP_COMMAND_STEP_HUE_UP                  (uint8_t)0x43U
#define ZB_ZGP_COMMAND_STEP_HUE_DOWN                (uint8_t)0x44U
#define ZB_ZGP_COMMAND_MOVE_SATURATION_STOP         (uint8_t)0x45U
#define ZB_ZGP_COMMAND_MOVE_SATURATION_UP           (uint8_t)0x46U
#define ZB_ZGP_COMMAND_MOVE_SATURATION_DOWN         (uint8_t)0x47U
#define ZB_ZGP_COMMAND_STEP_SATURATION_UP           (uint8_t)0x48U
#define ZB_ZGP_COMMAND_STEP_SATURATION_DOWN         (uint8_t)0x49U
#define ZB_ZGP_COMMAND_MOVE_COLOR                   (uint8_t)0x4aU
#define ZB_ZGP_COMMAND_STEP_COLOR                   (uint8_t)0x4bU
#define ZB_ZGP_COMMAND_LOCK_DOOR                    (uint8_t)0x50U
#define ZB_ZGP_COMMAND_UNLOCK_DOOR                  (uint8_t)0x51U
#define ZB_ZGP_COMMAND_PRESS_1_OF_1                 (uint8_t)0x60U
#define ZB_ZGP_COMMAND_RELEASE_1_OF_1               (uint8_t)0x61U
#define ZB_ZGP_COMMAND_PRESS_1_OF_2                 (uint8_t)0x62U
#define ZB_ZGP_COMMAND_RELEASE_1_OF_2               (uint8_t)0x63U
#define ZB_ZGP_COMMAND_PRESS_2_OF_2                 (uint8_t)0x64U
#define ZB_ZGP_COMMAND_RELEASE_2_OF_2               (uint8_t)0x65U
#define ZB_ZGP_COMMAND_SHORT_PRESS_1_OF_1           (uint8_t)0x66U
#define ZB_ZGP_COMMAND_SHORT_PRESS_1_OF_2           (uint8_t)0x67U
#define ZB_ZGP_COMMAND_SHORT_PRESS_2_OF_2           (uint8_t)0x68U
#define ZB_ZGP_COMMAND_ATTRIBUTE_REPORTING          (uint8_t)0xa0U
#define ZB_ZGP_COMMAND_MFR_REPORTING                (uint8_t)0xa1U
#define ZB_ZGP_COMMAND_MULTI_REPORTING              (uint8_t)0xa2U
#define ZB_ZGP_COMMAND_MFR_MULTI_REPORTING          (uint8_t)0xa3U
#define ZB_ZGP_COMMAND_REQUEST_ATTRIBUTES           (uint8_t)0xa4U
#define ZB_ZGP_COMMAND_READ_ATTRIBUTES_RESP         (uint8_t)0xa5U
#define ZB_ZGP_COMMAND_ZCL_TUNNELING                (uint8_t)0xa6U
#define ZB_ZGP_COMMAND_GPD_SENSOR                   (uint8_t)0xafU
#define ZB_ZGP_COMMAND_MFR_MIN                      (uint8_t)0xb0U
#define ZB_ZGP_COMMAND_MFR_MAX                      (uint8_t)0xbfU
#define ZB_ZGP_COMMAND_COMMISSIONING                (uint8_t)0xe0U
#define ZB_ZGP_COMMAND_DECOMMISSIONING              (uint8_t)0xe1U
#define ZB_ZGP_COMMAND_SUCCESS                      (uint8_t)0xe2U
#define ZB_ZGP_COMMAND_CHANNEL_REQUEST              (uint8_t)0xe3U
#define ZB_ZGP_COMMAND_e4                           (uint8_t)0xe4U
#define ZB_ZGP_COMMAND_e5                           (uint8_t)0xe5U
#define ZB_ZGP_COMMAND_e6                           (uint8_t)0xe6U
#define ZB_ZGP_COMMAND_e7                           (uint8_t)0xe7U
#define ZB_ZGP_COMMAND_e8                           (uint8_t)0xe8U
#define ZB_ZGP_COMMAND_e9                           (uint8_t)0xe9U
#define ZB_ZGP_COMMAND_ea                           (uint8_t)0xeaU
#define ZB_ZGP_COMMAND_eb                           (uint8_t)0xebU
#define ZB_ZGP_COMMAND_ec                           (uint8_t)0xecU
#define ZB_ZGP_COMMAND_ed                           (uint8_t)0xedU
#define ZB_ZGP_COMMAND_ee                           (uint8_t)0xeeU
#define ZB_ZGP_COMMAND_ef                           (uint8_t)0xefU
#define ZB_ZGP_COMMAND_RESERVED_MIN                 (uint8_t)0xf0U

#define ZB_ZGP_COMMAND_MASK_b0                      (uint8_t)0xb0U
#define ZB_ZGP_COMMAND_MASK_e0                      (uint8_t)0xe0U

#define ZB_ZGP_COMMAND_COMMISSIONING_REPLY          (uint8_t)0xf0U
#define ZB_ZGP_COMMAND_WRITE_ATTRIBUTES             (uint8_t)0xf1U
#define ZB_ZGP_COMMAND_READ_ATTRIBUTES              (uint8_t)0xf2U
#define ZB_ZGP_COMMAND_CHANNEL_CONFIGURATION        (uint8_t)0xf3U
#define ZB_ZGP_COMMAND_ZCL_TUNNELING2               (uint8_t)0xf6U

/*---------------------------------------------------------------
 * GP-DATA.request and GP-DATA.confirm
 *---------------------------------------------------------------
 */
#define ZB_ZGP_TXOPTIONS_QUEUE                      (uint8_t)0x01U
#define ZB_ZGP_TXOPTIONS_CSMA                       (uint8_t)0x02U
#define ZB_ZGP_TXOPTIONS_ACK                        (uint8_t)0x04U
#define ZB_ZGP_TXOPTIONS_TYPE                       (uint8_t)0x18U
#define ZB_ZGP_TXOPTIONS_DATA                       (uint8_t)(ZB_ZGP_TYPE_DATA << 3U)
#define ZB_ZGP_TXOPTIONS_MAINTENANCE                (uint8_t)(ZB_ZGP_TYPE_MAINTENANCE << 3U)
#define ZB_ZGP_TXOPTIONS_ENDPOINT                   (uint8_t)0x20U

struct ZbZgpDataReqT {
    uint8_t txOptions;
    uint8_t applicationId;
    uint32_t srcId;
    uint64_t dstAddr;
    uint8_t endpoint;
    uint8_t commandId;
    const void *asdu;
    uint8_t asduLength;
    uint32_t handle;
    uint16_t lifetime;
};

/* Same format as ZbNldeDataConfT */
struct ZbZgpDataConfT {
    uint32_t handle;
    enum ZgpStatusCodeT status;
};

/* Add items to the gpTxQueue */
void ZbZgpDataReqCallback(struct ZigBeeT *, struct ZbZgpDataReqT *,
    void (*callback)(struct ZbZgpDataConfT *, void *), void *arg);

/* Remove items from the gpTxQueue */
void ZbZgpDataPurge(struct ZigBeeT *zb, uint8_t appId, uint64_t gpdId, uint8_t endpoint);

/*---------------------------------------------------------------
 * GP-DATA.indication
 *---------------------------------------------------------------
 */
struct ZbZgpDataIndT {
    enum ZgpStatusCodeT status;
    int8_t rssi;
    uint8_t linkQuality;
    uint8_t seqNumber;
    uint8_t applicationId;
    uint8_t securityLevel;
    uint8_t keyType;
    uint32_t frameCounter;
    uint8_t rxAfterTx;
    uint8_t commissioning;
    uint32_t srcId;
    uint64_t srcAddr;
    uint8_t endpoint;
    uint8_t commandId;
    const void *asdu;
    uint8_t asduLength;
    uint8_t mic[4];
    uint8_t frameType;
};

/*---------------------------------------------------------------
 * Proxy table hacks
 *---------------------------------------------------------------
 */
int ZbZgpDecryptKey(struct ZigBeeT *zb, uint8_t appId, uint64_t gpdId, void *key, const void *mic);

uint8_t ZbZgpAddKey(struct ZigBeeT *zb, uint8_t applicationId, uint64_t gpdId, uint8_t endpoint,
    uint8_t level, uint8_t keytype, const void *key);

/* NOTE! This is only an empty server used for testing */
struct ZbZclClusterT * ZbZgpDeviceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/*lint -restore */
/*lint -restore */

#endif /* ZIGBEE_ZGP_H */
