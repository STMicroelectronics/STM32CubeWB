/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/* API between MAC and NHLE */

/* Warning 656: Arithmetic operation uses (compatible) enum's */
/* Note 9027: Unpermitted operand to operator '+' [MISRA 2012 Rule 10.1, required] */
/* Note 9029: Mismatched essential type categories for binary operator [MISRA 2012 Rule 10.4, required] */
/*lint -save -e656 -e9027 -e9029 */

#ifndef IEEE802154_API_H
#define IEEE802154_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#include "ieee802154.h"
#include "mcp.h"
#include "mcp_enums.h"
#include "llist.h"
#include "pletoh.h"
#if 0
#include "phy.h" /* FIXME - move phy.h out of radio.git and into it's own repo like we do with MCP? */
#endif

#ifndef PHY_RSSI_INVALID
#define PHY_RSSI_INVALID                    (-128)
#endif

#ifndef PHY_TX_POWER_MAX
/* The maximum (default) transmit power currently supported.
 * A value of 14 dBm aligns with the current GB-868 maximum. */
#define PHY_TX_POWER_MAX                    14 /* dBm */
#define PHY_TX_POWER_MIN                    (-20) /* dBm */
#endif

#if defined(__GNUC__)
# define WPAN_WARN_UNUSED __attribute__((warn_unused_result))
#else
# define WPAN_WARN_UNUSED
#endif

#ifndef MIN
# define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

struct WpanPublicT;

/* FIXME 2 - Make IEEE802154_MODE_ defines an enum. */
typedef uint8_t WpanAddrModeT; /* e.g. IEEE802154_MODE_SHORT */

/*----------------------------------------------------------------------------
 * Channels and Channel Masks
 *----------------------------------------------------------------------------
 */
/* Maximum number of channels for a given 32-bit channel mask. */
#define WPAN_CHANNELS_PER_MASK                  32U
/* Maximum channel value. logicalChannel is a 16-bit value. Keep it within
 * that type's range. 0xffff is otherwise arbitrary. */
#define WPAN_CHANNELS_MAX                       0xffffU

/* Maximum channels per page and channel mask, assuming 32-bit channel mask
 * and top 5-bits are reserved for page.
 *
 * Note that page bits are not an 802.15.4 concept, so the page is never
 * encoded in the channel mask for the MAC API. There is always a separate
 * channel page parameter (channelPage) from the channel or channel mask. */
#define WPAN_PAGE_CHANNELS_MAX                  27U
#define WPAN_PAGE_CHANNELMASK_ALL               0x07FFFFFFU /* Channels 0 to 26 */

/* Specific channel masks */
#define WPAN_CHANNELMASK_868MHZ                 0x00000001U /* Channel 0 */
#define WPAN_CHANNELMASK_915MHZ                 0x000007FEU /* Channels 1-10 */
#define WPAN_CHANNELMASK_2400MHZ                0x07FFF800U /* Channels 11-26 */

/* GB-868 channel masks (863 to 876 MHz) */
#define WPAN_CHANNELMASK_PAGE28                 0x07FFFFFFU /* Channels 0 to 26 */
#define WPAN_CHANNELMASK_PAGE29                 0x000000FFU /* Channels 27 to 34  (0 to 7) */
#define WPAN_CHANNELMASK_PAGE30                 0x00003FFFU /* Channels 35 to 48  (0 to 13) */
/* GB-868 channel masks (915 to 921 MHz) */
#define WPAN_CHANNELMASK_PAGE31                 0x00001FFFU /* Channels 0 to 12 */

/* Virtual channel page (BDL/SCoP). Arbitrarily chosen. */
#define WPAN_CHANNELPAGE_VIRTUAL_MIN            16U
#define WPAN_CHANNELPAGE_VIRTUAL_MAX            17U
#define WPAN_CHANNELMASK_VIRTUAL                WPAN_PAGE_CHANNELMASK_ALL /* Channels 0 to 26 */

#define WPAN_CHANNELPAGE_SUN                    9U

/*----------------------------------------------------------------------------
 * Packet Sizes
 *----------------------------------------------------------------------------
 */
#define WPAN_CONST_MAX_PHY_PACKET_SIZE          127U

/* Minimum v0 and v1 Beacon overhead, assuming:
 *   - extended addressing (typically short, but be on the safe side)
 *   - no security
 *   - GTS count = 0
 *   - no pending addresses */
#define WPAN_CONST_MIN_BEACON_OVERHEAD          (17U + 2U /* FCS */) /* 19 */

/* Maximum v0 and v1 Beacon overhead, assuming:
 *   - extended addressing
 *   - no security
 *   - GTS count = 0
 *   - 7 extended pending addresses (56 bytes) */
#define WPAN_CONST_MAX_BEACON_OVERHEAD          (17U + 56U + 2U) /* 75*/

/* For the maximum beacon payload length, assume minimum overhead */
#define WPAN_CONST_MAX_BEACON_PAYLOAD_LENGTH    (WPAN_CONST_MAX_PHY_PACKET_SIZE - WPAN_CONST_MIN_BEACON_OVERHEAD) /* 108 */

/* For ZigBee PRO, ZB_NWK_CONST_MAC_FRAME_OVERHEAD is 11. */
#define WPAN_CONST_MPDU_OVERHEAD                11U /* minimum overhead is 9 */

#define WPAN_CONST_MAX_MAC_PAYLOAD_SIZE         (WPAN_CONST_MAX_PHY_PACKET_SIZE - WPAN_CONST_MPDU_OVERHEAD)
#define WPAN_IE_MAX_SIZE                        WPAN_CONST_MAX_MAC_PAYLOAD_SIZE

/*----------------------------------------------------------------------------
 * Packet Durations (symbols)
 *----------------------------------------------------------------------------
 */
#define WPAN_CONST_BASE_SLOT_DURATION           60U
#define WPAN_CONST_NUM_SUPERFRAME_SLOTS         16U
#define WPAN_CONST_UNIT_BACKOFF_PERIOD          20U
#define WPAN_CONST_TURNAROUND_TIME              12U
#define WPAN_CONST_BASE_SUPERFRAME_DURATION     (WPAN_CONST_BASE_SLOT_DURATION * WPAN_CONST_NUM_SUPERFRAME_SLOTS)

/* macResponseWaitTime - multiples of aBaseSuperframeDuration (960) symbols. */
#define WPAN_CONST_RESPONSE_WAIT_DEFAULT        32U /* 30,720 symbols (@ 62,500 sym/sec = 492 mS) */

/*----------------------------------------------------------------------------
 * Superframe Specification field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_SUPERFRAME_MASK_BEACON             0x000fU
#define WPAN_SUPERFRAME_MASK_SUPERFRAME         0x00f0U
#define WPAN_SUPERFRAME_MASK_FINAL_CAP          0x0f00U
#define WPAN_SUPERFRAME_MASK_BATTERY_EXT        0x1000U
#define WPAN_SUPERFRAME_MASK_RESERVED           0x2000U
#define WPAN_SUPERFRAME_MASK_PAN_COORD          0x4000U
#define WPAN_SUPERFRAME_MASK_PERMIT_JOIN        0x8000U

#define WPAN_SUPERFRAME_OFFSET_BEACON           0U
#define WPAN_SUPERFRAME_OFFSET_SUPERFRAME       4U
#define WPAN_SUPERFRAME_OFFSET_FINAL_CAP        8U
#define WPAN_SUPERFRAME_OFFSET_BATTERY_EXT      12U
#define WPAN_SUPERFRAME_OFFSET_PAN_COORD        14U
#define WPAN_SUPERFRAME_OFFSET_PERMIT_JOIN      15U

/*----------------------------------------------------------------------------
 * GTS Specification field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_GTS_MASK_COUNT                     0x07U
#define WPAN_GTS_MASK_PERMIT                    0x80U
#define WPAN_GTS_OFFSET_COUNT                   0U
#define WPAN_GTS_OFFSET_PERMIT                  7U

/*----------------------------------------------------------------------------
 * Pending Address field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_PENDADDR_MASK_SHORT                0x07U
#define WPAN_PENDADDR_MASK_LONG                 0x70U
#define WPAN_PENDADDR_MASK_RESERVED             0x88U

#define WPAN_PENDADDR_OFFSET_SHORT              0U
#define WPAN_PENDADDR_OFFSET_LONG               4U

/*----------------------------------------------------------------------------
 * Transmit Power
 *----------------------------------------------------------------------------
 */
/* Transmit power format. */
#define WPAN_TXPOWER_POWER_MASK                 0x3fU
#define WPAN_TXPOWER_POWER_MAX                  31
#define WPAN_TXPOWER_POWER_MIN                  -32
#define WPAN_TXPOWER_TOLERANCE_MASK             0xc0U
#define WPAN_TXPOWER_TOLERANCE_1DB              0x00U
#define WPAN_TXPOWER_TOLERANCE_3DB              0x40U
#define WPAN_TXPOWER_TOLERANCE_6DB              0x80U

/* Sign-extend the 6bit transmit power value to a signed integer. */
#define WPAN_TXPOWER_AS_INT(_v_)                ((int)(((_v_) & 0x3f) ^ ~0x1f) - ~0x1f)

/*----------------------------------------------------------------------------
 * Join Policy
 *----------------------------------------------------------------------------
 */
/* Warning, the MAC does not define these values. ZigBee R22 defines these
 * values as shown below. */
enum WpanJoinPolicyT {
    WPAN_JOIN_POLICY_ALL = 0, /* 0x00 = ALL_JOIN */
    WPAN_JOIN_POLICY_IEEELIST, /* 0x01 = IEEELIST_JOIN */
    WPAN_JOIN_POLICY_NONE /* 0x02 = NO_JOIN */
};

/*---------------------------------------------------------
 * Diagnostics Information structure
 *---------------------------------------------------------
 */
/* mcp_macDiagClusterInfo */
/* Make sure this struct is self-packing and aligned to 8-byte boundaries for transfering via MCP. */
struct mac_diag_info {
    uint32_t mac_rx_bcast; /* ZCL_DIAG_ATTR_MAC_RX_BCAST */
    uint32_t mac_tx_bcast; /* ZCL_DIAG_ATTR_MAC_TX_BCAST */
    uint32_t mac_rx_ucast; /* ZCL_DIAG_ATTR_MAC_RX_UCAST */
    uint32_t mac_tx_ucast; /* ZCL_DIAG_ATTR_MAC_TX_UCAST */
    uint16_t mac_tx_ucast_retry; /* ZCL_DIAG_ATTR_MAC_TX_UCAST_RETRY */
    uint16_t mac_tx_ucast_fail; /* ZCL_DIAG_ATTR_MAC_TX_UCAST_FAIL */
    uint16_t phy_to_mac_queue_lim_reached; /* ZCL_DIAG_ATTR_PHY_MAC_QUEUE_LIM */
    uint16_t packet_validate_drop_count; /* ZCL_DIAG_ATTR_PACKET_VAL_DROP_COUNT (Note, also in zb_diag_info) */
    uint16_t avg_mac_retry_per_aps_msg_sent; /* ZCL_DIAG_ATTR_AVG_MAC_RETRY_PER_APS_MSG */
    uint8_t last_message_lqi; /* ZCL_DIAG_ATTR_LAST_MSG_LQI */
    int8_t last_message_rssi; /* ZCL_DIAG_ATTR_LAST_MSG_RSSI */
};

/*---------------------------------------------------------------
 * MCP Error
 *---------------------------------------------------------------
 */
void wpan_mcp_error_init(struct mcp_error *error, uint8_t seqno);

/*---------------------------------------------------------------
 * Attribute Payload Types
 *---------------------------------------------------------------
 */
/* Scalar */
/* struct mcp_scalar */

/* Payload */
struct wpan_attr_payload {
    struct mcp_attr attr; /* len = MCP_TLV_LEN(sizeof(struct mcp_attr)) */
    struct mcp_payload payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/*---------------------------------------------------------------
 * MLME-GET
 *---------------------------------------------------------------
 */
/* MLME-GET.request */
struct wpan_get_req {
    struct mcp_getreq req;
    struct mcp_attr attr;
};

/* MLME-GET.confirm */
/*
{
    struct mcp_getconf conf;
    struct wpan_attr_payload payload;
};
or
{
    struct mcp_getconf conf;
    struct mcp_scalar payload;
};
*/

void wpan_get_req_init(struct wpan_get_req *req);
void wpan_get_conf_scalar_init(struct mcp_getconf *conf, uint16_t id, uint8_t seqno);
void wpan_get_conf_payload_init(struct mcp_getconf *conf, uint16_t id, uint8_t seqno);

bool WPAN_WARN_UNUSED wpan_get_uint64(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint64_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint16(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint16_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint8(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint8_t *value, uint16_t index);
#if 0 /* not required for this port */
#define WPAN_HAVE_LICENSE
bool WPAN_WARN_UNUSED wpan_get_license(struct WpanPublicT *publicPtr, struct wpan_license *license);
#endif

/*---------------------------------------------------------------
 * MLME-SET
 *---------------------------------------------------------------
 */

/* MLME-SET.request */
/* {
 *     struct mcp_setreq req;
 *     struct mcp_scalar scalar;
 * }
 *   or
 * {
 *      struct mcp_setreq req;
 *      struct wpan_attr_payload payload;
 * }
 */

/* MLME-SET.confirm */
struct wpan_set_conf {
    struct mcp_setconf conf;
    struct mcp_attr attr;
};

void wpan_set_req_scalar_init(struct mcp_setreq *req, uint16_t id);
void wpan_set_req_payload_init(struct mcp_setreq *req, uint16_t id);
void wpan_set_conf_init(struct wpan_set_conf *conf, uint16_t id, uint8_t seqno);

bool WPAN_WARN_UNUSED wpan_set_uint64(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint64_t value64, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint16(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint16_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint8(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint8_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_bcnpayload(struct WpanPublicT *publicPtr, uint8_t *buf, uint16_t len);

/*---------------------------------------------------------------
 * MLME-GET/SET TX Power
 *---------------------------------------------------------------
 */
bool WpanGetTxPower(struct WpanPublicT *publicPtr, int *powerPtr, uint8_t *tolerancePtr);
bool WpanSetTxPower(struct WpanPublicT *publicPtr, int power, uint8_t tolerance);

/*---------------------------------------------------------
 * MCPS-PURGE
 *---------------------------------------------------------
 */
/*
 * The MCPS-PURGE service is used to cancel a pending MCPS-DATA.request that
 * has not yet been transmitted. The pending transation should be identified
 * by the MSDU handle provided in the MCPS-DATA.request. If the MSDU handle
 * not unique, then the behavior is unspecified.
 *
 * This function is optional, and does not need to be implemented on all
 * devices. Although, it is recommended for all devices that make use of the
 * indirect transmission service.
 */

/* MCPS-PURGE.request */
/* struct mcp_purgereq */

/* MCPS-PURGE.confirm */
/* struct mcp_purgeconf */

void wpan_purge_req_init(struct mcp_purgereq *req);
void wpan_purge_conf_init(struct mcp_purgeconf *conf, uint8_t seqno);
bool wpan_purge_req(struct WpanPublicT *publicPtr, struct mcp_purgereq *req, struct mcp_purgeconf *conf);

/*---------------------------------------------------------------
 * MLME-DATA
 *---------------------------------------------------------------
 */
/*
 * The MCPS-DATA.indication service is used to pass received packets to the
 * NHLE. And will be called for any received data packet that passes
 * address recognition.
 *
 * The MCPS-DATA.request service is used to begin the transmission of a data
 * packet. The payload buffer only needs to be valid for the duration of the
 * function call, and is not required to be allocated from the heap. The
 * MCPS-DATA.confirm service returns the results of the transmission attempt
 * and is returned asynchronously via a callback function.
 *
 * These functions are mandatory, and are the "meat and potatoes" of the
 * IEEE 802.15.4 device. For most applications, the acknowledgment and
 * indirect services must be supported (ZigBee). GTS transmissions are only
 * required for beacon-enabled PAN's.
 *
 * The MSDU handle is an 8-bit parameter specified by the upper layers to
 * associate a request with the asynchronous response. Earlier version. Due to
 * the 8-bit size, pointers may not be used here, but table indices are fine.
 *
 * For indirect and GTS transmissions, the packet will not be transmitted
 * immediately and may need to wait for a long duration. If necessary, the NHLE
 * can cancel the pending packet using the MCPS-PURGE service below. Likewise,
 * it is recommended that the MSDU handle should be unique within a single
 * device.
 */

/* MCPS-DATA.request */
struct wpan_data_req {
    struct mcp_datareq req;
    /* FIXME - make security optional */
    struct mcp_security security;
    struct mcp_payload payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/* MCPS-DATA.confirm */
/* struct mcp_dataconf */

/* MCPS-DATA.indication */
struct wpan_data_ind {
    struct mcp_dataind ind;
    struct mcp_linkquality lqi;
    /* FIXME - make security optional */
    struct mcp_security security;
    struct mcp_payload payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

void wpan_data_req_init(struct wpan_data_req *req);
void wpan_data_conf_init(struct mcp_dataconf *conf, uint8_t seqno);
void wpan_data_ind_init(struct wpan_data_ind *ind);

bool wpan_data_req(struct WpanPublicT *publicPtr, struct wpan_data_req *req);

/*---------------------------------------------------------------
 * MLME-RESET
 *---------------------------------------------------------------
 */
/* MLME-RESET.request */
/* struct mcp_resetreq */

/* MLME-RESET.confirm */
/* struct mcp_resetconf */

void wpan_reset_req_init(struct mcp_resetreq *req);
void wpan_reset_conf_init(struct mcp_resetconf *conf, uint8_t seqno);
bool wpan_reset_req(struct WpanPublicT *publicPtr, struct mcp_resetreq *req, struct mcp_resetconf *conf);

/*---------------------------------------------------------------
 * MLME-POLL
 *---------------------------------------------------------------
 */
/*
 * The MLME-POLL service is used to retrieve pending packets from a device's
 * parent.Issuing this request will transmit a data request command to the
 * specified device, and wait for an acknowledgement. If the frame-pending
 * bit in the acknowledgement is not set, then this will return a status of
 * NO_DATA.
 *
 * Strictly according to the specification, this should also wait for the
 * parent to dequeue and transmit the pending frames before returning SUCCESS
 * but in reality most devices just return after receiving the ACK.
 *
 * This function is mandatory for all deivce types, but is used most requently
 * deivces with macRxOnWhenidle set to FALSE.
 */
/* MLME-POLL.request */
struct wpan_poll_req {
    struct mcp_pollreq req;
    struct mcp_security security;
};

/* MLME-POLL.confirm */
/* struct mcp_pollconf */

/* MLME-POLL.indication */
/* struct mcp_pollind */

void wpan_poll_req_init(struct wpan_poll_req *req);
void wpan_poll_conf_init(struct mcp_pollconf *conf, uint8_t seqno);
void wpan_poll_ind_init(struct mcp_pollind *ind);
bool wpan_poll_req(struct WpanPublicT *publicPtr, struct wpan_poll_req *req);

/*---------------------------------------------------------------
 * MLME-START
 *---------------------------------------------------------------
 */
/*
 * The MLME-START service is used to begin operating as a full-function device, prior
 * to receiving an MLME-START.request, the device should not respond transmit beacon
 * frames, nor should it respond to beacon request commands.
 *
 * On full-function devices, this service may also be used to change the active channel
 * and beacon-timing parameters. (Note that for RFD's there is no service to change
 * channels, and the device should directly set the phyCurrentChannel PIB attribute (if
 * unsupported, orphan scanning is likely the only method left to rejoin a moved PAN).
 *
 * If the coordRealignment parameter is set to true, then device will also broadcast a
 * coordinator realignment command prior to changing its parameters.
 *
 * This service is mandatory for full-function devices, and is never used for end
 * devices.
 */
/* MLME-START.request */
/* struct mcp_startreq */

/* MLME-START.confirm */
/* struct mcp_startconf */

void wpan_start_req_init(struct mcp_startreq *req);
void wpan_start_conf_init(struct mcp_startconf *conf, uint8_t seqno);
bool wpan_start_req(struct WpanPublicT *publicPtr, struct mcp_startreq *req, struct mcp_startconf *conf);

/*---------------------------------------------------------------
 * MLME-RX-ENABLE
 *---------------------------------------------------------------
 */
/*
 * The MLME-RX-ENABLE service is used to enable (or disable) the receiver for a
 * period of time. This function should return immediately without waiting for
 * the receiver to be turned on.
 *
 * This function has no effect when macRxOnWhenIdle is true. Otherwise, the
 * receipt of this function should supersede all previous requests. Likewise,
 * setting rxOnDuration to 0 will disable the receiver if currently enabled.
 *
 * The deferPermit and rxOnTime parameters are ignored unless operating on a
 * beacon enabled network, in which case they specify which superframe this
 * command applies to, and when in the superframe to enable the receiver.
 *
 * On non-beacon enabled networks, the enable time takes effect immediately.
 */

/* MLME-RX-ENABLE.request */
/* struct mcp_rxenreq; */

/* MLME-RX-ENABLE.confirm */
/* struct mcp_rxenconf; */

void wpan_rx_enable_req_init(struct mcp_rxenreq *req);
void wpan_rx_enable_conf_init(struct mcp_rxenconf *conf, uint8_t seqno);
bool wpan_rx_enable_req(struct WpanPublicT *publicPtr, struct mcp_rxenreq *req, struct mcp_rxenconf *conf);

/*---------------------------------------------------------------
 * MLME-ASSOCIATE
 *---------------------------------------------------------------
 */
/*
 * The MLME-ASSOCIATE service is used to facilitate the MAC-layer
 * association procedure, and consists of four primitives:
 * MLME-ASSOCIATE.request - Requests the device to associate to a parent.
 * MLME-ASSOCIATE.confirm - Returns the result of an association attempt.
 * MLME-ASSOCIATE.indication - Notifies the NHLE that a device requested assoication.
 * MLME-ASSOCIATE.response - Requests that the device respond to an association request.
 *
 * MLME-ASSOCIATE.request and MLME-ASSOCIATE.confirm are implemented
 * with a blocking function, and are mandatory for all IEEE 802.15.4
 * devices.
 *
 * MLME-ASSOCIATE.indication is passed from the driver to the NHLE using
 * a callback function. The NHLE should not expect the
 * MLME-ASSOCIATE.indication callback to be executed from a thread-safe
 * context, the exact context is device dependent. After receiving this
 * indication, the NHLE must respond with an MLME-ASSOCIATE.response
 * within macResponseWaitTime.
 *
 * MLME-ASSOCIATE.response is implemented as a blocking function, returning
 * an MLME-COMM-STATUS.indication after the device successfully polls for the
 * association response command, or the indirect transmission expires.
 *
 * MLME-ASSOCIATE.indication and MLME-ASSOCIATE.response are mandatory for
 * full function devices (ZigBee coordinators and routers), and is optional
 * for reduced function devices (ZigBee end devices).
 *
 */

/* MLME-ASSOCIATE.request */
struct wpan_assoc_req {
    struct mcp_assocreq req;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.confirm */
struct wpan_assoc_conf {
    struct mcp_assocconf conf;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.indication */
struct wpan_assoc_ind {
    struct mcp_assocind ind;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.response */
struct wpan_assoc_rsp {
    struct mcp_assocrsp rsp;
    struct mcp_security security;
};

void wpan_assoc_req_init(struct wpan_assoc_req *req);
void wpan_assoc_conf_init(struct wpan_assoc_conf *conf, uint8_t seqno);
void wpan_assoc_ind_init(struct wpan_assoc_ind *ind);
void wpan_assoc_rsp_init(struct wpan_assoc_rsp *rsp);
bool wpan_assoc_req(struct WpanPublicT *publicPtr, struct wpan_assoc_req *req);
bool wpan_assoc_rsp(struct WpanPublicT *publicPtr, struct wpan_assoc_rsp *rsp);

#if 0 /* disassociate is not used and is now deprecated */
/*---------------------------------------------------------------
 * MLME-DISASSOCIATE
 *---------------------------------------------------------------
 */
/*
 * The MLME-DISASSOCIATE service is used to remove a device from a PAN to
 * which it is currently associated.
 *
 * Because the MLME-DISASSOCIATE.confirm is returned after an indirect
 * transmission, the status is instead returned using the MLME-COMM-STATUS
 * service.
 *
 * This function is optional, and is not currently used by anything.
 */
/* MLME-DISASSOCIATE.request */
typedef struct {
    WpanAddrModeT deviceMode;
    uint16_t devicePanId;
    uint64_t deviceAddr;
    uint8_t disassociateReason;
    uint8_t txIndirect;
    struct mcp_security security;
} WpanDisassociateReqT;

/* MLME-DISASSOCIATE.confirm */
typedef struct {
    struct WpanPublicT *publicPtr;
    uint8_t status;
    WpanAddrModeT deviceMode;
    uint16_t devicePanId;
    uint64_t deviceAddr;
} WpanDisassociateConfT;

/* MLME-DISASSOCIATE.indication */
typedef struct {
    struct WpanPublicT *publicPtr;
    uint64_t deviceAddress;
    uint8_t disassociateReason;
    struct mcp_security security;
} WpanDisassociateIndT;

bool WpanDisassociateReq(struct WpanPublicT *publicPtr, WpanDisassociateReqT *req);
#endif

/*---------------------------------------------------------------
 * MLME-ORPHAN
 *---------------------------------------------------------------
 */
/* MLME-ORPHAN.indication */
struct wpan_orphan_ind {
    struct mcp_orphanind ind;
    struct mcp_security security;
};

/* MLME-ORPHAN.response */
struct wpan_orphan_rsp {
    struct mcp_orphanrsp rsp;
    struct mcp_security security;
};

void wpan_orphan_ind_init(struct wpan_orphan_ind *ind);
void wpan_orphan_rsp_init(struct wpan_orphan_rsp *rsp);
bool wpan_orphan_rsp(struct WpanPublicT *publicPtr, struct wpan_orphan_rsp *rsp);

/*---------------------------------------------------------
 * MLME-COMM-STATUS
 *---------------------------------------------------------
 */
/* MLME-COMM-STATUS.indication */
struct wpan_comm_status_ind {
    struct mcp_commind ind;
    struct mcp_security security;
};

void wpan_comm_status_ind_init(struct wpan_comm_status_ind *ind);

/*---------------------------------------------------------------
 * MLME-SCAN
 *---------------------------------------------------------------
 */
/*
 * The MLME-SCAN service is used to facilitate the scanning of channels for a
 * variety of purposes, and is implemented as a blocking function call (note
 * that this can potentially block for up to 250 seconds per channel!).
 *
 * Every scan type can specify a range of channels to scna using the scanChannels
 * bitmask, and a duration to scan the channel for. The actual time spent on each
 * channel (in symbols) is given by the forumla:
 *      aBaseSuperframeDuration * ((1ul << scanDuration) - 1)
 *
 * If for whatever reason, the device is unable to scan any channel, then it should
 * set the corresponding bits in the unscannedChannels bitmask found in the
 * MCPS-SCAN.confirm.
 *
 * Energy scans are performed by periodically sampling the channel energy level and
 * returning a list of the maximum energy level sampled on each channel.
 *
 * Active and passive scans are used to detect IEEE 802.15.4 networks operating on
 * the local chnanels by searching for beacon frames. The scan is performed by
 * listening on each channel for beacons, and returning them to the NHLE using the
 * PAN descriptor list in the MLME-SCAN.confirm (if macAutoRequest is true), or via
 * the MLME-BEACON-NOTIFY.indication service (if macAutoReqyest is FALSE, or if the
 * beacon has a non-zero payload length). The only difference between an active and
 * passive scan is that the active scan will first transmit a beacon request command
 * before listening for beacon frames. Active scans should be used to lcoate non-
 * beaconing networks, and passive scans for beacon-enabled networks.
 *
 * Orphan scans are used to implement the orphan rejoin procedure. An orphaned
 * device will attempt to locate its parent by iterating through the scan channel mask
 * and issuing orphan notification commands on each channel. If the devices's parent
 * detects this command is should respond with a coordinator realignment command and
 * the orphanded device should consider itself associated to the PAN again.
 */

/* Maximum valid scan duration */
#define WPAN_SCAN_DURATION_MAX                  14U

/* Filter fields of the Enhanced beacon request command. */
#define WPAN_SCAN_FILTER_PERMIT_JOIN            0x01U
#define WPAN_SCAN_FILTER_LINK_QUALITY           0x02U
#define WPAN_SCAN_FILTER_PERCENT                0x04U
#define WPAN_SCAN_FILTER_PAYLOAD                0x80U

/* MLME-SCAN.request (legacy 802.15.4-2003) */
struct wpan_scan_req {
    struct mcp_scanreq req;
    struct mcp_security security;
    struct mcp_payload pie; /* FIXME - convert to mcp_ie? */
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/* MLME-SCAN.confirm */
struct wpan_scan_conf {
    struct mcp_scanconf conf;
    /* zigbee doesn't use pan descriptor */
    struct mcp_energylist energy;
    uint8_t buf[WPAN_CHANNELS_PER_MASK];
};

void wpan_scan_req_init(struct wpan_scan_req *req);
void wpan_scan_conf_init(struct wpan_scan_conf *conf, uint8_t seqno);
bool wpan_scan_req(struct WpanPublicT *publicPtr, struct wpan_scan_req *req);

/*---------------------------------------------------------------
 * MLME-BEACON
 *---------------------------------------------------------------
 */
struct wpan_pan_desc {
    struct mcp_pandesc pandesc;
    /* FIXME - make the following optional and arbitrary order */
    struct mcp_linkquality lqi;
    struct mcp_security security;
};

/* MLME-BEACON-REQUST.indication */
struct wpan_beacon_request_ind {
    struct mcp_bcnreqind ind;
    /* use IE TLVs? For zigbee, this is easier for now. */
    struct mcp_payload ie_payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/* MLME-BEACON.request */
/* struct mcp_beaconreq */

/* MLME-BEACON.confirm */
/* struct mcp_beaconconf */

/*
 * The MLME-BEACON-NOTIFY.indication service is used to report information about
 * received beacons. This is implemented by a callback function, and may be called
 * during active scans, or on receipt of any beacon with a non-zero payload length.
 *
 * If macAutoRequest is set to FALSE, then the driver will NOT automatically
 * respond to beacons that advertise pending frames for the device. And the device
 * will issue the MLME-BEACON-NOTIFY.indication so the NHLE can manually query
 * its parent for pending packets.
 */

/* MLME-BEACON-NOTIFY.indication */
struct wpan_beacon_notify_ind {
    struct mcp_beaconind ind;
    /* FIXME - make the following optional and arbitrary order */
    struct wpan_pan_desc pandesc;
#if 0 /* pending addresses are not used by zigbee */
    struct mcp_pendaddr pendaddr;
#endif
    struct mcp_payload payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

void wpan_pan_desc_init(struct wpan_pan_desc *pandesc);
void wpan_pan_desc_set_sframe(struct mcp_pandesc *pandesc, uint16_t sframe);
uint16_t wpan_pan_desc_get_sframe(struct mcp_pandesc *pandesc);

void wpan_beacon_request_ind_init(struct wpan_beacon_request_ind *ind);
void wpan_beacon_req_init(struct mcp_beaconreq *req);
void wpan_beacon_conf_init(struct mcp_beaconconf *conf, uint8_t seqno);
void wpan_beacon_notify_ind_init(struct wpan_beacon_notify_ind *ind);
bool wpan_beacon_req(struct WpanPublicT *publicPtr, struct mcp_beaconreq *req, struct mcp_beaconconf *conf);

/*----------------------------------------------------------------------------
 * TX Power Table Configuration
 *----------------------------------------------------------------------------
 */
#define TX_POWER_CTRL_MAX                   PHY_TX_POWER_MAX
#define TX_POWER_CTRL_MIN                   PHY_TX_POWER_MIN
#define TX_POWER_CTRL_DELTA_MAX             6
#define TX_POWER_CTRL_DELTA_MIN             (-6)

void wpan_getpowertblreq_init(struct mcp_getpowertblreq *req);
void wpan_getpowertblconf_init(struct mcp_getpowertblconf *conf, uint8_t seqno);

/* Note: to clear the Power Information Table, do a MLME-SET-POWER-INFORMATION-TABLE.request
 * with dst_addr16 = IEEE802154_ADDR_SHORT_NONE, and dst_addr64 = 0. */
void wpan_setpowertblreq_init(struct mcp_setpowertblreq *req);
void wpan_setpowertblconf_init(struct mcp_setpowertblconf *conf, uint8_t seqno);

/*---------------------------------------------------------------
 * Duty Cycle Mode Indication
 *---------------------------------------------------------------
 */
void wpan_dutycycle_ind_init(struct mcp_dutycycleind *ind);

/*--------------------------------------------------------------------------
 * MAC Instance
 *--------------------------------------------------------------------------
 */
#define WPAN_DEVICE_NAME_MAXSZ                  16
#define WPAN_DEVICE_DESC_MAXSZ                  64

struct WpanPublicT {
    /* Link list linking (only for NHLE to use) */
    struct WpanPublicT *next;
    struct WpanPublicT *prev;

    /* Interface Description */
    char ifcName[WPAN_DEVICE_NAME_MAXSZ]; /* e.g. "wpan0", etc... */
    char ifcDesc[WPAN_DEVICE_DESC_MAXSZ]; /* e.g. "ChipCon CC2420 2.4GHz Transceiver" */
    bool isIpBridge;

    /*---------------------------------------------------------------
     * Serial-IO Device Management API
     *---------------------------------------------------------------
     */
    const struct WpanSioVersionT *sio_version_info;
    void (*sio_close)(struct WpanPublicT *publicPtr, bool doReset);
    /* Wait-Event and Kick MAC functions */
    int (*sio_get_fd)(struct WpanPublicT *publicPtr);
    /* process returns false if the communication link has been broken (e.g. dongle unplugged).
     * The application should call dev->close() on the device and detach it from any NHLE. */
    bool WPAN_WARN_UNUSED (*sio_process)(struct WpanPublicT *publicPtr);

    /*---------------------------------------------------------------
     * MAC Request API
     *---------------------------------------------------------------
     */
    void (*mcpsPurgeReq)(struct WpanPublicT *publicPtr, struct mcp_purgereq *req, struct mcp_purgeconf *conf);
    void (*mcpsDataReq)(struct WpanPublicT *publicPtr, struct wpan_data_req *req);

    /*
     * The MLME-RESET service is used to return the device to its initial state
     * and is implemented as a blocking function call. When called, this function
     * should perform a hardware reset, clear all pending frames, and cease the
     * the transmission of beacons.
     *
     * When handling an MLME-RESET, the following attributes require special
     * handling:
     *  - macBsn and macDsn should be set to randomized values.
     *  - macExtendedAddress (if implemented) should not be modified, regardless of
     *          the value of setDefaultPib. The device should only change its
     *          extended address when explicitly instructed to do so by the NHLE.
     */
    void (*mlmeResetReq)(struct WpanPublicT *publicPtr, struct mcp_resetreq *req, struct mcp_resetconf *conf);

    /*
     * The MLME-SET service is provided to write status and configuration
     * data to an IEEE 802.15.4 device.
     *
     * All attributes are written in the host byte order. If the attribute
     * length does not match the size exactly then an error status will be
     * returned. In the event of an error, no change will be made to the
     * device configuration. The index parameter is ignored for scalar
     * attributes.
     *
     * A default implementation of this function is also provided, it does nothing
     * more than write the attribute into a cache of the PIB table located in the
     * device handle structure.
     *
     * MLME-SET is mandatory for all IEEE 802.15.4 devices.
     */
    void (*mlmeSetReq)(struct WpanPublicT *publicPtr, struct mcp_setreq *req, struct wpan_set_conf *conf);

    /*
     * The MLME-GET service is provided to read status and configuration
     * data from an IEEE 802.15.4 device, and has been modified slightly
     * to avoid requiring heap memory when returning an attribute to the
     * caller.
     *
     * The MLME-GET.request primitive specifies the attribute and an optional
     * index if the attribute is a table. The index is ignored for all scalar
     * attributes. The MLME-GET.request also specifies a buffer (and buffer
     * length) to write the attribute value into.
     *
     * All attributes are written in the host byte order starting at the
     * beginning of the buffer. If the attribute is longer than the buffer
     * then an error status will be returned. Refer to the definition of
     * WpanMacPibT for the size of each attribute.
     *
     * In addition, a helper function is provided to allow the NHLE to perform
     * a scalar MLME-GET without having to put the WpanGetReqT and WpanGetConfT
     * structures on the stack.
     *
     * A default implementation of this function is also provided, it does nothing
     * more than read the attribute frame a cache of the PIB table located in the
     * device handle structure.
     *
     * MLME-GET is mandatory for all IEEE 802.15.4 devices.
     */
    void (*mlmeGetReq)(struct WpanPublicT *publicPtr, struct wpan_get_req *req, struct mcp_getconf *conf);

    void (*mlmePollReq)(struct WpanPublicT *publicPtr, struct wpan_poll_req *req);
#if 0 /* for beacon-enabled networks only */
    void (*mlmeSyncReq)(struct WpanPublicT *, WpanSyncReqT *);
#endif
    void (*mlmeStartReq)(struct WpanPublicT *publicPtr, struct mcp_startreq *req, struct mcp_startconf *conf);
    /* MLME-RX-ENABLE.request
     *      req->deferPermit is ignored.
     *      req->rxOntime is ignored
     *      req->rxOnDuration is treated as a boolean value.
     *           0 = disable receiver, and go to sleep if supported
     *          >0 = wake up and/or enable receiver */
    void (*mlmeRxEnableReq)(struct WpanPublicT *publicPtr, struct mcp_rxenreq *req, struct mcp_rxenconf *conf);
    void (*mlmeAssociateReq)(struct WpanPublicT *publicPtr, struct wpan_assoc_req *req);
    void (*mlmeAssociateResp)(struct WpanPublicT *publicPtr, struct wpan_assoc_rsp *req);
#if 0 /* disassociate is not used and is now deprecated */
    void (*mlmeDisassociateReq)(struct WpanPublicT *publicPtr, WpanDisassociateReqT *req);
#endif
    void (*mlmeOrphanResp)(struct WpanPublicT *publicPtr, struct wpan_orphan_rsp *rsp);
    void (*mlmeScanReq)(struct WpanPublicT *publicPtr, struct wpan_scan_req *req);
    void (*mlmeBeaconReq)(struct WpanPublicT *publicPtr, struct mcp_beaconreq *req, struct mcp_beaconconf *conf);
    void (*mlmeGetPowerTblReq)(struct WpanPublicT *publicPtr, struct mcp_getpowertblreq *req, struct mcp_getpowertblconf *conf);
    void (*mlmeSetPowerTblReq)(struct WpanPublicT *publicPtr, struct mcp_setpowertblreq *req, struct mcp_setpowertblconf *conf);

    /*---------------------------------------------------------------
     * NHLE Callbacks: minimal API required
     *---------------------------------------------------------------
     */
    void (*mcpsDataConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_dataconf *conf);
    void (*mcpsDataInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_data_ind *ind);
    void (*mlmeAssociateConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_assoc_conf *conf);
    void (*mlmeAssociateInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_assoc_ind *ind);
#if 0 /* disassociate is not used and is now deprecated */
    void (*mlmeDisassociateConf)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanDisassociateConfT *conf);
    void (*mlmeDisassociateInd)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanDisassociateIndT *ind);
#endif
    void (*mlmeBeaconNotifyInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_notify_ind *ind);

    /* The MLME-ORPHAN service is used to respond to devices performing orphan
     * scans. The MLME-ORPAH.indication is implemented as a callback function
     * set by the NHLE and is called on recieipt of any orphan notification
     * commands.
     *
     * If the device is a parent of the orphaned child, then it should respond
     * to the MLME-ORPAN.indication by calling the MLME-ORPAN.response function
     * to reassociate the child to this PAN. If the orphaned device is not a
     * a child, then no action should be taken by the NHLE.
     *
     * This service is optional, but recommended for full function devices.
     */
    void (*mlmeOrphanInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_orphan_ind *ind);

    void (*mlmeCommStatusInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_comm_status_ind *ind);
    void (*mlmeScanConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_scan_conf *conf);
#if 0 /* for beacon-enabled networks only */
    void (*mlmeSyncLossInd)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanSyncLossIndT *);
#endif
    void (*mlmePollConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_pollconf *conf);
    void (*mlmePollInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_pollind *ind);

    /*---------------------------------------------------------------
     * NHLE Callbacks: required for R22 GB-868 (Sub-GHz, Enhanced Beacons, Duty Cycle)
     *---------------------------------------------------------------
     */
    void (*mlmeBeaconRequestInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_request_ind *ind);
    void (*mlmeEnhBeaconNotifyInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_notify_ind *ind);
    /* LBT/DC: MLME-DUTYCYCLE-MODE.indication */
    void (*mlmeDutyCycleModeInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_dutycycleind *ind);

    /*---------------------------------------------------------------
     * Private Data
     *---------------------------------------------------------------
     */
    void *nhleInstance; /* NHLE's private data */
    void *priv; /* MAC's private data */
};

/*---------------------------------------------------------------
 * Serial-IO Device Management Wrappers
 *---------------------------------------------------------------
 */
bool WpanSerialProcess(struct WpanPublicT *publicPtr);
int WpanSerialFd(struct WpanPublicT *publicPtr);
void WpanSerialClose(struct WpanPublicT *publicPtr, bool doReset);

/*---------------------------------------------------------------
 * IEEE 802.15.4 CRC
 *---------------------------------------------------------------
 */
#define WPAN_CRC_POLYNOMIAL                     0x1021
#define WPAN_CRC_INITIAL                        0x0000
#define WPAN_CRC_XOROUT                         0x0000

uint16_t WpanCrc(uint16_t crc, const void *dataPtr, unsigned int dataLen);

/*--------------------------------------------------------------------------
 * Enhanced Beacon Request and Enhanced Beacon IE Parser
 *--------------------------------------------------------------------------
 */
struct WpanIe {
    uint16_t len;
    const uint8_t *data;
};

/* FIXME 2 - rename to something specific to EBR/EB for ZigBee PRO? */
struct WpanIeOpts {
    struct WpanIe ebf; /* MLME sub-IE: Enhanced beacon request filtering */
    struct WpanIe rejoin; /* ZigBee IE: Rejoin */
    struct WpanIe txPower; /* ZigBee IE: TX Power */
    struct WpanIe beaconPayload; /* ZigBee IE: beacon payload + superframe */
};

int WpanIeParseOpts(struct WpanIeOpts *opts, const void *ie, size_t len);

/*--------------------------------------------------------------------------
 * GB-868 - FIXME 1 - move to separate header file?
 *--------------------------------------------------------------------------
 */
/* Helper to convert IE TX Power value to integer */
int8_t WpanEnhBeaconIeToTxPower(int8_t ieTxPower, int rssi);

/* Helper function to convert an RSSI measurement to an LQI value. */
uint8_t WpanRssiToLqi(int8_t rssi);
int8_t WpanLqiToRssi(uint8_t lqi);

/* The ideal measured RSSI value we want to be operating at is
 * 20 dB above the -99 dBm floor. */
#ifndef WPAN_GB868_IDEAL_RX_RSSI
# define WPAN_GB868_IDEAL_RX_RSSI           (-79)
#endif

#ifndef WPAN_GB868_CCA_THRESHOLD
/* aLBTThresholdLevelLp */
# define WPAN_GB868_CCA_THRESHOLD           (-87)
#endif

/*---------------------------------------------------------------
 * ieee802154_misc.
 *---------------------------------------------------------------
 */
bool WpanIsBdl(struct WpanPublicT *publicPtr);
bool WpanIsVirtual(struct WpanPublicT *publicPtr);
bool WpanIsIpBridge(struct WpanPublicT *publicPtr);

uint32_t WpanGetChannelMaskByPage(uint8_t page);
uint8_t WpanGetNumChannelsFromMask(uint32_t mask, uint8_t *first_channel);
uint8_t WpanTxPowerGetToleranceInt(uint8_t txpower);
const char * WpanStatusToStr(enum mcp_status status);
const char * WpanDutyCycleStatusToStr(uint8_t status);

#endif /* IEEE802154_API_H */

/*lint -restore */
