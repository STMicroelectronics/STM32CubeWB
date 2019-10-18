/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef IEEE802154_H
#define IEEE802154_H

#include <stdint.h>
#include <string.h> /* size_t */

#define IEEE802154_PANID_BCAST                      0xffffU
#define IEEE802154_ADDR_SHORT_BCAST                 0xffffU
#define IEEE802154_ADDR_SHORT_NONE                  0xfffeU /* use extended addressing */
#define IEEE802154_ADDR_SIMPLE_NONE                 0xfeU
#define IEEE802154_ADDR_SIMPLE_BCAST                0xffU

/* IEEE 802.15.4 Frame Types */
#define IEEE802154_TYPE_BEACON                      0x0U
#define IEEE802154_TYPE_DATA                        0x1U
#define IEEE802154_TYPE_ACK                         0x2U
#define IEEE802154_TYPE_COMMAND                     0x3U
#define IEEE802154_TYPE_LLDN                        0x4U
#define IEEE802154_TYPE_MULTI                       0x5U

/* IEEE 802.15.4 Address Modes */
#define IEEE802154_MODE_NONE                        0x0U
#define IEEE802154_MODE_SIMPLE                      0x1U
#define IEEE802154_MODE_SHORT                       0x2U
#define IEEE802154_MODE_EXTENDED                    0x3U

/* IEEE 802.15.4 Frame Versions */
#define IEEE802154_VERSION_2003                     0x0U
#define IEEE802154_VERSION_2006                     0x1U
#define IEEE802154_VERSION_2015                     0x2U

#define IEEE802154_FCTRL_TYPE                       0x0007U
#define IEEE802154_FCTRL_TYPE_POS                   0
#define IEEE802154_FCTRL_SECURITY                   0x0008U
#define IEEE802154_FCTRL_SECURITY_POS               3
#define IEEE802154_FCTRL_PENDING                    0x0010U
#define IEEE802154_FCTRL_PENDING_POS                4
#define IEEE802154_FCTRL_ACK_REQ                    0x0020U
#define IEEE802154_FCTRL_ACK_REQ_POS                5
#define IEEE802154_FCTRL_PANID_COMP                 0x0040U /* was Intra-PAN */
#define IEEE802154_FCTRL_PANID_COMP_POS             6
#define IEEE802154_FCTRL_RESERVED                   0x0080U
#define IEEE802154_FCTRL_RESERVED_POS               7
#define IEEE802154_FCTRL_SEQNUM_COMP                0x0100U
#define IEEE802154_FCTRL_SEQNUM_COMP_POS            8
#define IEEE802154_FCTRL_IE_LIST                    0x0200U
#define IEEE802154_FCTRL_IE_LIST_POS                9
#define IEEE802154_FCTRL_DST_MODE                   0x0c00U
#define IEEE802154_FCTRL_DST_POS                    10
#define IEEE802154_FCTRL_DST_NONE                   ((uint16_t)IEEE802154_MODE_NONE << IEEE802154_FCTRL_DST_POS)
#define IEEE802154_FCTRL_DST_SIMPLE                 ((uint16_t)IEEE802154_MODE_SIMPLE << IEEE802154_FCTRL_DST_POS)
#define IEEE802154_FCTRL_DST_SHORT                  ((uint16_t)IEEE802154_MODE_SHORT << IEEE802154_FCTRL_DST_POS)
#define IEEE802154_FCTRL_DST_EXTENDED               ((uint16_t)IEEE802154_MODE_EXTENDED << IEEE802154_FCTRL_DST_POS)
#define IEEE802154_FCTRL_DST_MODE_GET(_fctrl_)      ((_fctrl_ & IEEE802154_FCTRL_DST_MODE) >> IEEE802154_FCTRL_DST_POS)
#define IEEE802154_FCTRL_VERSION                    0x3000U
#define IEEE802154_FCTRL_VERSION_POS                12
#define IEEE802154_FCTRL_VERSION_2003               ((uint16_t)IEEE802154_VERSION_2003 << IEEE802154_FCTRL_VERSION_POS)
#define IEEE802154_FCTRL_VERSION_2006               ((uint16_t)IEEE802154_VERSION_2006 << IEEE802154_FCTRL_VERSION_POS)
#define IEEE802154_FCTRL_VERSION_2015               ((uint16_t)IEEE802154_VERSION_2015 << IEEE802154_FCTRL_VERSION_POS)
#define IEEE802154_FCTRL_SRC_MODE                   0xc000U
#define IEEE802154_FCTRL_SRC_POS                    14
#define IEEE802154_FCTRL_SRC_NONE                   ((uint16_t)IEEE802154_MODE_NONE << IEEE802154_FCTRL_SRC_POS)
#define IEEE802154_FCTRL_SRC_SIMPLE                 ((uint16_t)IEEE802154_MODE_SIMPLE << IEEE802154_FCTRL_SRC_POS)
#define IEEE802154_FCTRL_SRC_SHORT                  ((uint16_t)IEEE802154_MODE_SHORT << IEEE802154_FCTRL_SRC_POS)
#define IEEE802154_FCTRL_SRC_EXTENDED               ((uint16_t)IEEE802154_MODE_EXTENDED << IEEE802154_FCTRL_SRC_POS)
#define IEEE802154_FCTRL_SRC_MODE_GET(_fctrl_)      ((_fctrl_ & IEEE802154_FCTRL_SRC_MODE) >> IEEE802154_FCTRL_SRC_POS)

/* Key Modes */
#define IEEE802154_KEYID_IMPLICIT                   0x0U
#define IEEE802154_KEYID_DEFAULT                    0x1U
#define IEEE802154_KEYID_SHORT                      0x2U
#define IEEE802154_KEYID_EXTENDED                   0x3U

/* Security control field. */
#define IEEE802154_SCTRL_LEVEL                      0x07U
#define IEEE802154_SCTRL_ENCRYPT                    0x04U /* Security level >=4 enabled encryption */
#define IEEE802154_SCTRL_KEYID_POS                  3
#define IEEE802154_SCTRL_KEYID_MODE                 0x18U
#define IEEE802154_SCTRL_KEYID_IMPLICIT             (IEEE802154_KEYID_IMPLICIT << IEEE802154_SCTRL_KEYID_POS)
#define IEEE802154_SCTRL_KEYID_DEFAULT              (IEEE802154_KEYID_DEFAULT << IEEE802154_SCTRL_KEYID_POS)
#define IEEE802154_SCTRL_KEYID_SHORT                (IEEE802154_KEYID_SHORT << IEEE802154_SCTRL_KEYID_POS)
#define IEEE802154_SCTRL_KEYID_EXTENDED             (IEEE802154_KEYID_EXTENDED << IEEE802154_SCTRL_KEYID_POS)
#define IEEE802154_SCTRL_KEYID_GET(_sctrl_)         ((_sctrl_ & IEEE802154_SCTRL_KEYID_MODE) >> IEEE802154_SCTRL_KEYID_POS)
#define IEEE802154_SCTRL_FC_COMP                    0x20U
#define IEEE802154_SCTRL_FC_SIZE                    0x40U /* FIXME 1 - In 2015 spec, this is now IEEE802154_SCTRL_ASN_IN_NONCE */

/* Compute the MIC length from the security level. */
#define IEEE802154_MIC_LENGTH(_level_)              ((2U << ((_level_) & 0x3U)) & ~0x3U)

struct ieee802154_hdr {
    /* Frame header and addressing. */
    uint16_t fctrl;
    uint8_t seqnum;
    uint16_t dst_panid;
    uint64_t dst_addr;
    uint16_t src_panid;
    uint64_t src_addr;
    /* Security header fields. */
    uint8_t sctrl;
    uint64_t frame_counter;
    uint64_t key_source;
    uint8_t key_index;
    /* TODO: Header IE list? */
    /* Lengths used by security processing (not actually in the frame). */
    uint16_t auth_length;
    uint8_t mic_length;
};

/* Header parsing function. */
struct ieee802154_hdr * ieee802154_parse_header(struct ieee802154_hdr *hdr, const uint8_t *p, uint16_t len, const uint8_t **end);

uint16_t ieee802154_write_header(const struct ieee802154_hdr *hdr, uint8_t *q, uint16_t maxlen);

int ieee802154_addr_match(const struct ieee802154_hdr *hdr, uint16_t panid, uint64_t ext_addr, uint16_t short_addr, uint8_t simple_addr, uint8_t coordinator);

/* IE's are identified by a 16-bit header. */
#define IEEE802154_HIE_LENGTH                       0x007fU
#define IEEE802154_HIE_ID                           0x7f80U
#define IEEE802154_HIE_ID_OFFSET                    7U
#define IEEE802154_PIE_LENGTH                       0x07ffU
#define IEEE802154_PIE_ID                           0x7800U
#define IEEE802154_PIE_ID_OFFSET                    11U
#define IEEE802154_IE_TYPE                          0x8000U
#define IEEE802154_IE_HEADER                        0U
#define IEEE802154_IE_PAYLOAD                       IEEE802154_IE_TYPE
#define IEEE802154_IE_MIN_SPACE                     2U

/* Read a 16-bit IE header from memory. */
#define IEEE802154_IE_PARSE(_p_) \
    (((const uint8_t *)(_p_))[0] | (((uint16_t)((const uint8_t *)(_p_))[1]) << 8))

/* Read the IE type and ID from memory. */
#define IEEE802154_IE_ISTYPE(_p_) \
    (((uint16_t)((const uint8_t *)(_p_))[1] << 8) & IEEE802154_IE_TYPE)
#define IEEE802154_IE_VALUE(_p_) \
    (IEEE802154_IE_PARSE(_p_) & ((uint16_t)(IEEE802154_IE_TYPE | \
                                            ((IEEE802154_IE_ISTYPE(_p_) != 0U) ? IEEE802154_PIE_ID : IEEE802154_HIE_ID))))

/* Read the IE length from memory. */
#define IEEE802154_IE_LENGTH(_p_) \
    (((((uint16_t)((const uint8_t *)(_p_))[1] << 8) & IEEE802154_IE_TYPE) != 0U) ? \
     (IEEE802154_IE_PARSE(_p_) & IEEE802154_PIE_LENGTH) : \
     ((size_t)((const uint8_t *)(_p_))[0] & IEEE802154_HIE_LENGTH))
#define IEEE802154_IE_SPACE(_p_)                    (IEEE802154_IE_LENGTH(_p_) + IEEE802154_IE_MIN_SPACE)
#define IEEE802154_IE_DATA(_p_) \
    /*lint -save -e9087 [ cast obj A <-> obj B - 11.3 REQUIRED. */ \
    ((const void *)((const uint8_t *)(&_p_[IEEE802154_IE_MIN_SPACE])))

/* Set an IE header into memory. */
#define IEEE802154_IE_SET(_p_, _id_, _len_) \
    do { \
        ((uint8_t *)(_p_))[0] = (((_id_) | (_len_)) >> 0) & 0xff; \
        ((uint8_t *)(_p_))[1] = (((_id_) | (_len_)) >> 8) & 0xff; \
    } while (0)

/* Encode ID's into a type/value pair */
#define IEEE802154_HIE_VALUE(_id_)                  (((uint16_t)(_id_) << IEEE802154_HIE_ID_OFFSET) | IEEE802154_IE_HEADER)
#define IEEE802154_PIE_VALUE(_id_)                  (((uint16_t)(_id_) << IEEE802154_PIE_ID_OFFSET) | IEEE802154_IE_PAYLOAD)

/* Header IE types. */
#define IEEE802154_HIE_UNMANAGED_MIN                IEEE802154_HIE_VALUE(0x00U)
#define IEEE802154_HIE_UNMANAGED_MAX                IEEE802154_HIE_VALUE(0x19U)
#define IEEE802154_HIE_LE_CSL                       IEEE802154_HIE_VALUE(0x1aU)
#define IEEE802154_HIE_LE_RIT                       IEEE802154_HIE_VALUE(0x1bU)
#define IEEE802154_HIE_DSME_PAN_DESC                IEEE802154_HIE_VALUE(0x1cU)
#define IEEE802154_HIE_RZ_TIME                      IEEE802154_HIE_VALUE(0x1dU)
#define IEEE802154_HIE_ACK_TIME_CORR                IEEE802154_HIE_VALUE(0x1eU)
#define IEEE802154_HIE_GROUP_ACK                    IEEE802154_HIE_VALUE(0x1fU)
#define IEEE802154_HIE_LLDN_INFO                    IEEE802154_HIE_VALUE(0x20U)
#define IEEE802154_HIE_SIMPLIFIED_SUPERFRAME        IEEE802154_HIE_VALUE(0x23U)
#define IEEE802154_HIE_TERM_PAYLOAD                 IEEE802154_HIE_VALUE(0x7eU) /* Payload IE list to follow */
#define IEEE802154_HIE_TERM_UNFORMATTED             IEEE802154_HIE_VALUE(0x7fU) /* Unformatted payload to follow. */

/* Payload IE group values. */
#define IEEE802154_PIE_ESDU                         IEEE802154_PIE_VALUE(0x0U)
#define IEEE802154_PIE_MLME                         IEEE802154_PIE_VALUE(0x1U)
#define IEEE802154_PIE_VENDOR_SPECIFIC              IEEE802154_PIE_VALUE(0x2U)
#define IEEE802154_PIE_RESERVED_MIN                 IEEE802154_PIE_VALUE(0x3U)
#define IEEE802154_PIE_RESERVED_MAX                 IEEE802154_PIE_VALUE(0xeU)
#define IEEE802154_PIE_TERM                         IEEE802154_PIE_VALUE(0xfU)

/* IE parser helper functions */
const void * ieee802154_ie_start(const void *ie, size_t len);
const void * ieee802154_ie_next(const void *ie, size_t len);
unsigned int ieee802154_ie_list_length(const void *ie, size_t len);

/*---------------------------------------------------------------
 * Nested MLME IE
 *---------------------------------------------------------------
 */
/* Nested MLME IE header */
#define IEEE802154_MIE_SHORT_LENGTH                 0x00ffU
#define IEEE802154_MIE_SHORT_ID                     0x7f00U
#define IEEE802154_MIE_SHORT_OFFSET                 8U
#define IEEE802154_MIE_LONG_LENGTH                  0x07ffU
#define IEEE802154_MIE_LONG_ID                      0x7800U
#define IEEE802154_MIE_LONG_OFFSET                  11U
#define IEEE802154_MIE_TYPE                         0x8000U
#define IEEE802154_MIE_TYPE_SHORT                   0U
#define IEEE802154_MIE_TYPE_LONG                    IEEE802154_MIE_TYPE

#define IEEE802154_MIE_SET(_p_, _id_, _len_)        IEEE802154_IE_SET(_p_, _id_, _len_)

#define IEEE802154_MIE_ISTYPE(_p_) \
    (((uint16_t)((const uint8_t *)(_p_))[1] << 8) & IEEE802154_MIE_TYPE)
#define IEEE802154_MIE_VALUE(_p_) \
    (IEEE802154_IE_PARSE(_p_) & (IEEE802154_MIE_TYPE | \
                                 ((IEEE802154_MIE_ISTYPE(_p_) != 0U) ? IEEE802154_MIE_LONG_ID : IEEE802154_MIE_SHORT_ID)))

/* Read the IE length from memory. */
#define IEEE802154_MIE_LENGTH(_p_) \
    (uint16_t)(((((uint16_t)((const uint8_t *)(_p_))[1] << 8) & IEEE802154_MIE_TYPE) != 0U) ? \
               (IEEE802154_IE_PARSE(_p_) & IEEE802154_MIE_LONG_LENGTH) : ((const uint8_t *)(_p_))[0])

#define IEEE802154_MIE_SPACE(_p_)                   (IEEE802154_MIE_LENGTH(_p_) + 2U)
#define IEEE802154_MIE_DATA(_p_)                    IEEE802154_IE_DATA(_p_)

/* Nested MLME IE types. */
#define IEEE802154_MIE_SHORT_VALUE(_id_)            (((uint16_t)(_id_) << IEEE802154_MIE_SHORT_OFFSET) | IEEE802154_MIE_TYPE_SHORT)
#define IEEE802154_MIE_LONG_VALUE(_id_)             (((uint16_t)(_id_) << IEEE802154_MIE_LONG_OFFSET) | IEEE802154_MIE_TYPE_LONG)
#define IEEE802154_MIE_TSCH_SYNC                    IEEE802154_MIE_SHORT_VALUE(0x1aU)
#define IEEE802154_MIE_TSCH_LINK                    IEEE802154_MIE_SHORT_VALUE(0x1bU)
#define IEEE802154_MIE_TSCH_TIMESLOT                IEEE802154_MIE_SHORT_VALUE(0x1cU)
#define IEEE802154_MIE_HOP_TIMING                   IEEE802154_MIE_SHORT_VALUE(0x1dU)
#define IEEE802154_MIE_EB_FILTER                    IEEE802154_MIE_SHORT_VALUE(0x1eU)
#define IEEE802154_MIE_METRICS_SINGLE               IEEE802154_MIE_SHORT_VALUE(0x1fU)
#define IEEE802154_MIE_METRICS_ALL                  IEEE802154_MIE_SHORT_VALUE(0x20U)
#define IEEE802154_MIE_UNMANAGED_MIN                IEEE802154_MIE_SHORT_VALUE(0x40U)
#define IEEE802154_MIE_UNMANAGED_MAX                IEEE802154_MIE_SHORT_VALUE(0x7fU)
#define IEEE802154_MIE_HOP_SEQUENCE                 IEEE802154_MIE_LONG_VALUE(0x9U)

#define IEEE802154_METRIC_OCTETS                    0x01
#define IEEE802154_METRIC_RETRY                     0x02
#define IEEE802154_METRIC_MULTIPLE_RETRY            0x03
#define IEEE802154_METRIC_TX_FAIL                   0x04
#define IEEE802154_METRIC_TX_SUCCESS                0x05
#define IEEE802154_METRIC_FCS_ERROR                 0x06
#define IEEE802154_METRIC_SECURITY_FAILURE          0x07
#define IEEE802154_METRIC_DUPLICATE_FRAME           0x08
#define IEEE802154_METRIC_RX_SUCCESS                0x09
#define IEEE802154_METRIC_NACK                      0x0a
#define IEEE802154_METRIC_MAX                       0x0a

/* Nested MLME IE parser helper functions. */
const void * ieee802154_mie_start(const void *ie, size_t len);
const void * ieee802154_mie_next(const void *ie, size_t len);

/*----------------------------------------------------------------------------
 * Enhanced Beacon Filter Header (IEEE802154_MIE_EB_FILTER)
 *----------------------------------------------------------------------------
 */
#define IEEE802154_EBF_HDR_PERMITJOIN               (uint8_t)0x01U
#define IEEE802154_EBF_HDR_LQI                      (uint8_t)0x02U
#define IEEE802154_EBF_HDR_PERCENT                  (uint8_t)0x04U
#define IEEE802154_EBF_HDR_PIB_MASK                 (uint8_t)0x18U
#define IEEE802154_EBF_HDR_RESERVED_MASK            (uint8_t)0xE0U
#define IEEE802154_EBF_HDR_PIB_ENTRIES(_hdr_)       (((_hdr_) & IEEE802154_EBF_HDR_PIB_MASK) >> 3)

/*---------------------------------------------------------------
 * Vendor Specific OUIs that we support
 *---------------------------------------------------------------
 */
#define IEEE802154_VENDOR_OUI_ZIGBEE                0x4A191BU /* ZigBee Alliance CID */

/*---------------------------------------------------------------
 * Vendor Specific OUI: ZigBee (IEEE802154_VENDOR_OUI_ZIGBEE)
 *---------------------------------------------------------------
 */
/* Nested ZigBee IE header */
#define IEEE802154_ZIE_LENGTH_MASK                  0x003fU
#define IEEE802154_ZIE_ID_MASK                      0xffc0U
#define IEEE802154_ZIE_ID_OFFSET                    6U
#define IEEE802154_ZIE_PUT_ID(_id_)                 (((_id_) << IEEE802154_ZIE_ID_OFFSET))
#define IEEE802154_ZIE_GET_ID_MASK(_p_)             (IEEE802154_IE_PARSE(_p_) & IEEE802154_ZIE_ID_MASK)

/* Nested ZigBee IE types */
#define IEEE802154_ZIE_MASK_REJOIN                  IEEE802154_ZIE_PUT_ID((uint16_t)0x00U)
#define IEEE802154_ZIE_MASK_TXPOWER                 IEEE802154_ZIE_PUT_ID((uint16_t)0x01U)
#define IEEE802154_ZIE_MASK_BEACONPAYLOAD           IEEE802154_ZIE_PUT_ID((uint16_t)0x02U)

/* Read the IE length from memory. */
#define IEEE802154_ZIE_LENGTH(_p_) \
    ((uint16_t)(((const uint8_t *)(_p_))[0]) & IEEE802154_ZIE_LENGTH_MASK)

#define IEEE802154_ZIE_SPACE(_p_)                   (IEEE802154_ZIE_LENGTH(_p_) + 2U)
#define IEEE802154_ZIE_DATA(_p_)                    IEEE802154_IE_DATA(_p_)

/* Nested ZigBee IE parser helper functions. */
const void * ieee802154_zie_start(const void *ie, size_t len);
const void * ieee802154_zie_next(const void *ie, size_t len);

/*---------------------------------------------------------------
 * IEEE 802154 command types
 *---------------------------------------------------------------
 */
#define IEEE802154_CMD_ASSOCIATE_REQ                0x01
#define IEEE802154_CMD_ASSOCIATE_RESP               0x02
#define IEEE802154_CMD_DISASSOCIATE                 0x03
#define IEEE802154_CMD_DATA_REQ                     0x04
#define IEEE802154_CMD_PANID_CONFLICT               0x05
#define IEEE802154_CMD_ORPHAN_NOTIFY                0x06
#define IEEE802154_CMD_BEACON_REQ                   0x07
#define IEEE802154_CMD_REALIGNMENT                  0x08
#define IEEE802154_CMD_GTS_REQ                      0x09
#define IEEE802154_CMD_LL_DISCOVER                  0x0d
#define IEEE802154_CMD_LL_CONFIG_STATUS             0x0e
#define IEEE802154_CMD_LL_CONFIG_REQ                0x0f
#define IEEE802154_CMD_LL_CTS_GROUP                 0x10
#define IEEE802154_CMD_LL_RTS                       0x11
#define IEEE802154_CMD_LL_CTS                       0x12
#define IEEE802154_CMD_DSME_ASSOC_REQ               0x13
#define IEEE802154_CMD_DSME_ASSOC_RESP              0x14
#define IEEE802154_CMD_DSME_GTS_REQ                 0x15
#define IEEE802154_CMD_DSME_GTS_REPLY               0x16
#define IEEE802154_CMD_DSME_GTS_NOTIFY              0x17
#define IEEE802154_CMD_DSME_INFO_REQ                0x18
#define IEEE802154_CMD_DSME_INFO_REPLY              0x19
#define IEEE802154_CMD_DSME_BEACON_ALLOC            0x1a
#define IEEE802154_CMD_DSME_BEACON_COLLISION        0x1b
#define IEEE802154_CMD_DSME_LINK_STATUS             0x1c
#define IEEE802154_CMD_AMCA_BEACON_REQ              0x1d
#define IEEE802154_CMD_AMCA_HELLO                   0x1e
#define IEEE802154_CMD_AMCA_CHANNEL_PROBE           0x1f
#define IEEE802154_CMD_LE_RIT_DATA_REQ              0x20

#define IEEE802154_CCM_BLOCK_SIZE                   16U
#define IEEE802154_CCM_NONCE_SIZE                   13U

/* IEEE 802.15.4 packet decryption function. */
int ieee802154_decrypt(const struct ieee802154_hdr *hdr, uint8_t *p, size_t len,
    const void *nonce, void (*cipher)(void *out, const void *in, void *ctx), void *ctx);

#endif /* _IEEE802154_H */
