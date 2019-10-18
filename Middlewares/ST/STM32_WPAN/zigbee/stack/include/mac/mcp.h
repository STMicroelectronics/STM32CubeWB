/* Copyright [2015 - 2018] Exegin Technologies Limited. All rights reserved. */
/*
 * This header file defines the MAC Control Protocol for communication with
 * IEEE 802.15.4 MAC devices. The MAC Control protocol is defined in
 * exegin-802.15.4-mac-control-protocol.pdf and assumes that the protocol messages
 * will be transmitted over a reliable channel that preserves message boundaries.
 *
 * Each message is composed of a common header, fixed message data, and may be
 * followed by TLVs.
 */
#ifndef MCP_H
#define MCP_H

#include <stdint.h>

/*lint -save -e9071 [ __mcp_bswap64() is reserved - 21.1 REQUIRED] */
#define __mcp__bswap64(x) ( \
        (((uint64_t)(x) << 56) & 0xff00000000000000ULL) | \
        (((uint64_t)(x) << 40) & 0x00ff000000000000ULL) | \
        (((uint64_t)(x) << 24) & 0x0000ff0000000000ULL) | \
        (((uint64_t)(x) << 8) & 0x000000ff00000000ULL) | \
        (((uint64_t)(x) >> 8) & 0x00000000ff000000ULL) | \
        (((uint64_t)(x) >> 24) & 0x0000000000ff0000ULL) | \
        (((uint64_t)(x) >> 40) & 0x000000000000ff00ULL) | \
        (((uint64_t)(x) >> 56) & 0x00000000000000ffULL) \
        )
/*lint -restore */

/*lint -save -e9071 [ __mcp_bswap32() is reserved - 21.1 REQUIRED] */
#define __mcp__bswap32(x) ( \
        (((uint32_t)(x) << 24) & 0xff000000U) | \
        (((uint32_t)(x) << 8) & 0x00ff0000) | \
        (((uint32_t)(x) >> 8) & 0x0000ff00) | \
        (((uint32_t)(x) >> 24) & 0x000000ff) \
        )
/*lint -restore */

/*lint -save -e9071 [ __mcp_bswap16() is reserved - 21.1 REQUIRED] */
#define __mcp__bswap16(x) ( \
        (((uint16_t)(x) << 8) & 0xff00U) | \
        (((uint16_t)(x) >> 8) & 0x00ff) \
        )
/*lint -restore */

/*lint -save -e9021 [ use of 'undef' is discouraged - 20.5 advisory] */
#ifdef __BYTE_ORDER__
# undef BYTE_ORDER
# if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define BYTE_ORDER  BIG_ENDIAN
# elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define BYTE_ORDER  LITTLE_ENDIAN
# else
#  error "Byte order unknown"
# endif
#elif !defined(BYTE_ORDER)
# include <endian.h>
# if !defined(BYTE_ORDER)
#  error "Don't know the byte-order. Please ensure BYTE_ORDER is defined or the appropriate header is included."
#endif
#endif
/*lint -restore */

#ifndef BIG_ENDIAN
# define BIG_ENDIAN  4321
#endif
#ifndef LITTLE_ENDIAN
# define LITTLE_ENDIAN  1234
#endif

#ifndef htole16
# if (BYTE_ORDER == BIG_ENDIAN)
#  define htole16(w)     __mcp__bswap16(w)
#  define le16toh(w)     __mcp__bswap16(w)
#  define htole32(w)     __mcp__bswap32(w)
#  define le32toh(w)     __mcp__bswap32(w)
#  define htole64(w)     __mcp__bswap64(w)
#  define le64toh(w)     __mcp__bswap64(w)
# else
#  define htole16(w)     (w)
#  define le16toh(w)     (w)
#  define htole32(w)     (w)
#  define le32toh(w)     (w)
#  define htole64(w)     (w)
#  define le64toh(w)     (w)
# endif
#endif

/* Deal with systems that don't define 64bit endian conversion macros. */
#ifndef htole64
# if (BYTE_ORDER == BIG_ENDIAN)
#  define htole64(w)     __mcp__bswap64(w)
#  define le64toh(w)     __mcp__bswap64(w)
# else
#  define htole64(w)     (w)
#  define le64toh(w)     (w)
# endif
#endif

/*
Provide macros for setting static/global data.
These are need, because some compilers map htole*() to actual functions.
*/
#ifndef const_htole16
# if (BYTE_ORDER == BIG_ENDIAN)
#  define const_htole16(w)  __mcp__bswap16(w)
#  define const_le16toh(w)  __mcp__bswap16(w)
#  define const_htole32(w)  __mcp__bswap32(w)
#  define const_le32toh(w)  __mcp__bswap32(w)
#  define const_htole64(w)  __mcp__bswap64(w)
#  define const_le64toh(w)  __mcp__bswap64(w)
# else
#  define const_htole16(w)  (w)
#  define const_le16toh(w)  (w)
#  define const_htole32(w)  (w)
#  define const_le32toh(w)  (w)
#  define const_htole64(w)  (w)
#  define const_le64toh(w)  (w)
# endif
#endif

/*
 * The size of a structure up to and including the specified field.
 * Used to specify the size of different versions of the structure.
 * The first term is the same as offsetof from stddef.h, but that header
 * cannot be included here because it disrupts the alignment checking.
 */
#define MCP_END_OF(_type_, _member_)    ((size_t)(&((_type_ *)0)->_member_) + sizeof(((_type_ *)0)->_member_))

/*---------------------------------------------------------
 * Common Header Definitions
 *---------------------------------------------------------
 */
/* The common MCP header fields. */
struct mcp_hdr {
    uint8_t cmd;
    uint8_t reserved;
    uint8_t seqno;
    uint8_t flags;
};

struct mcp_time {
    uint32_t sec;
    uint32_t nsec;
};

/* All messages and TLVs in MCP must be padded to a multiple of 8 bytes. */
#define MCP_ALIGN(_sz_) (((_sz_) + 7U) & ~0x7U)

/* MCP protocol version */
#define MCP_VERSION_MAJOR       3U
#define MCP_VERSION_MINOR       8U

/* MCP flag bits. */
#define MCP_FLAG_REQUEST        0x00U /* Direction to MAC. */
#define MCP_FLAG_CONFIRM        0x01U /* Reply to MCP_FLAG_REQUEST. */
#define MCP_FLAG_INDICATION     0x02U /* Asynchronous from MAC. */
#define MCP_FLAG_RESPONSE       0x03U /* Reply to MCP_FLAG_INDICATION. */
#define MCP_FLAG_DIRECTION(_x_) ((_x_) & 0x03U)
#define MCP_FLAG_BULK_END       0x40U /* Bulk Data: transfer is complete. */
#define MCP_FLAG_BULK_START     0x80U /* Bulk Data: start new transfer. */

#define MCP_CMD_NOP             0x00U
/* MCPS Primitives */
#define MCP_MCPS_DATA           0x01U
#define MCP_MCPS_PURGE          0x02U
/* MLME Primitives */
#define MCP_MLME_RESET          0x03U
#define MCP_MLME_GET            0x04U
#define MCP_MLME_SET            0x05U
#define MCP_MLME_START          0x06U
#define MCP_MLME_ASSOCIATE      0x07U
#define MCP_MLME_DISASSOCIATE   0x08U
#define MCP_MLME_SCAN           0x09U
#define MCP_MLME_BEACON         0x0aU
#define MCP_MLME_COMM           0x0bU
#define MCP_MLME_POLL           0x0cU
#define MCP_MLME_RX_ENABLE      0x0dU
#define MCP_MLME_SYNC           0x0eU
#define MCP_MLME_ORPHAN         0x0fU
/* IEEE 802.15.4e Primitives */
#define MCP_MLME_SET_SLOTFRAME  0x20U
#define MCP_MLME_SET_LINK       0x21U
#define MCP_MLME_TSCH_MODE      0x22U
#define MCP_MLME_BCNREQ         0x23U
#define MCP_MLME_GETPWRTBL      0x24U
#define MCP_MLME_SETPWRTBL      0x25U
#define MCP_MLME_DUTYCYCLE      0x26U
/* Wi-SUN Specific Primitives */
#define MCP_WISUN_START         0xE0U
/* Vendor Specific Primitives */
#define MCP_MLME_EVENT          0xDDU
#define MCP_VENDOR_COMMAND      0xDEU
/* IEEE 802.15.4 PHY Primitives */
#define MCP_PD_DATA             0xDFU
/* Device-control Primitives */
#define MCP_DEVICE_REGISTER     0xfaU
#define MCP_DEVICE_RESET        0xfbU
#define MCP_DEVICE_START        0xfcU
#define MCP_DEVICE_IDENT        0xfdU
#define MCP_DEVICE_DATA         0xfeU
#define MCP_DEVICE_ERROR        0xffU

/*---------------------------------------------------------
 * Common MCP Constants
 *---------------------------------------------------------
 */

/* Addressing Modes */
#define MCP_ADDRMODE_NONE       0U
#define MCP_ADDRMODE_SHORT      2U
#define MCP_ADDRMODE_LONG       3U

/* Special addresses */
#define MCP_SHORTADDR_NONE      0xfffeU
#define MCP_SHORTADDR_BCAST     0xffffU
#define MCP_PANID_BCAST         0xffffU

/* Bit-mapped Options */
#define MCP_TXOPT_ACKREQ        0x0001U
#define MCP_TXOPT_USEGTS        0x0002U
#define MCP_TXOPT_INDIRECT      0x0004U
#define MCP_TXOPT_NO_SEQNO      0x0008U
#define MCP_TXOPT_NO_PANID      0x0010U
#define MCP_TXOPT_MULTIPURPOSE  0x0020U
#define MCP_OPT_LQI_SCAN        0x0040U

#define MCP_RXOPT_MODE_SWITCH   0x0001U
#define MCP_RXOPT_NO_SEQNO      0x0008U
#define MCP_RXOPT_NO_PANID      0x0010U

/*---------------------------------------------------------
 * MCP Message Primitive Definitions
 *---------------------------------------------------------
 */
/* MCPS-DATA.request */
struct mcp_datareq {
    uint8_t cmd; /* MCP_MCPS_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t dst_panid;
    uint8_t dst_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t dst_addr;
    uint16_t handle;
    uint16_t txopt; /* e.g. MCP_TXOPT_ACKREQ */
    uint16_t data_rate;
    uint16_t tag;
    /* Payload TLV may follow as the last TLV. */
};

/* MCPS-DATA.confirm */
struct mcp_dataconf {
    uint8_t cmd; /* MCP_MCPS_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t panid;
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t status;
    uint64_t src_addr;
    struct mcp_time timestamp;
    uint16_t handle;
    uint8_t retries;
    uint8_t backoffs;
    uint8_t tx_flags;
    uint8_t rsv;
    uint16_t tag;
};

#define MCP_TX_FLAGS_Q_DEFAULT      0x00U
#define MCP_TX_FLAGS_Q_PEER         0x01U

/* MCPS-DATA.indication */
struct mcp_dataind {
    uint8_t cmd; /* MCP_MCPS_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t src_panid;
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t dst_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t src_addr;
    uint64_t dst_addr;
    uint16_t dst_panid;
    uint16_t rxopt; /* MCP_RXOPT_... */
    struct mcp_time timestamp;
    uint16_t data_rate;
    uint8_t dsn;
    int8_t rssi;
    /* Payload TLV may follow as the last TLV. */
};

/* MCPS-PURGE.request */
struct mcp_purgereq {
    uint8_t cmd; /* MCP_MCPS_PURGE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t handle;
    uint8_t rsv[2];
};

/* MCPS-PURGE.confirm */
struct mcp_purgeconf {
    uint8_t cmd; /* MCP_MCPS_PURGE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t handle;
    uint8_t status;
    uint8_t rsv[1];
};

/*---------------------------------------------------------
 * MLME Message Primitive Definitions
 *---------------------------------------------------------
 */
/* MLME-RESET.request */
struct mcp_resetreq {
    uint8_t cmd; /* MCP_MLME_RESET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t default_pib;
    uint8_t rsv[3];
};

/* MLME-RESET.confirm */
struct mcp_resetconf {
    uint8_t cmd; /* MCP_MLME_RESET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-GET.request */
struct mcp_getreq {
    uint8_t cmd; /* MCP_MLME_GET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
    /* Attribute TLV is mandatory. */
};

/* MLME-GET.confirm */
struct mcp_getconf {
    uint8_t cmd; /* MCP_MLME_GET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t rsv[4];
    /* Attribute TLV is mandatory. */
};

/* MLME-SET.request */
struct mcp_setreq {
    uint8_t cmd; /* MCP_MLME_SET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
    /* Attribute TLV is mandatory. */
};

/* MLME-SET.confirm */
struct mcp_setconf {
    uint8_t cmd; /* MCP_MLME_SET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t rsv[4];
    /* Attribute TLVs are mandatory ONLY for attributes that failed (sizeof(value)==0 for those TLVs) */
};

/* MLME-START.request */
struct mcp_startreq {
    uint8_t cmd; /* MCP_MLME_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint32_t time;
    uint16_t options; /* e.g. MCP_START_PAN_COORD */
    uint16_t panid;
    uint16_t channel;
    uint8_t page;
    uint8_t bo; /* Beacon Order */
    uint8_t sfo; /* Superframe Order */
    uint8_t rsv[7];
};

/* MLME-START.request options */
#define MCP_START_PAN_COORD         0x0001U
#define MCP_START_BATT_EXT          0x0002U
#define MCP_START_COORD_REALIGN     0x0004U

/* MLME-START.confirm */
struct mcp_startconf {
    uint8_t cmd; /* MCP_MLME_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-ASSOCIATE.request */
struct mcp_assocreq {
    uint8_t cmd; /* MCP_MLME_ASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t panid;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t rsv0;
    uint64_t addr;
    uint16_t channel;
    uint8_t page;
    uint8_t capability; /* MCP_ASSOC_CAP_... */
    uint32_t rsv1;
};

/* MLME-ASSOCIATE.request capabilities */
#define MCP_ASSOC_CAP_ALT_COORD     0x01U /* Deprecated by IEEE 802.15.4-2015 */
#define MCP_ASSOC_CAP_DEV_TYPE      0x02U /* 0 = RFD, 1 = FFD */
#define MCP_ASSOC_CAP_PWR_SRC       0x04U /* 0 = battery, 1 = mains */
#define MCP_ASSOC_CAP_RXONIDLE      0x08U
#define MCP_ASSOC_CAP_ASSOC_TYPE    0x10U /* 0 = normal, 1 = fast association */
/* Reserved bit: 0x20 */
#define MCP_ASSOC_CAP_SECURITY      0x40U
#define MCP_ASSOC_CAP_ALLOC_ADDR    0x80U /* 1 = coordinator (i.e. parent) allocates short address, 0 = otherwise */

/* MLME-ASSOCIATE.confirm */
struct mcp_assocconf {
    uint8_t cmd; /* MCP_MLME_ASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t addr;
    uint8_t status;
    uint8_t rsv;
};

/* MLME-ASSOCIATE.indication */
struct mcp_assocind {
    uint8_t cmd; /* MCP_MLME_ASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t capability;
    uint8_t rsv[3];
    uint64_t addr;
};

/* MLME-ASSOCIATE.response */
struct mcp_assocrsp {
    uint8_t cmd; /* MCP_MLME_ASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_RESPONSE */
    uint16_t addr16;
    uint8_t status; /* MCP_ASSOC_RSP_STATUS */
    uint8_t rsv;
    uint64_t addr64;
};

/* MLME-ASSOCIATE.response status */
#define MCP_ASSOC_RSP_STATUS_SUCCESS        0x00U
#define MCP_ASSOC_RSP_STATUS_AT_CAPACITY    0x01U
#define MCP_ASSOC_RSP_STATUS_DENIED         0x02U

/* MLME-DISASSOCIATE.request */
struct mcp_disassocreq {
    uint8_t cmd; /* MCP_MLME_DISASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t panid;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t reason;
    uint64_t addr;
    uint16_t txopt; /* e.g. MCP_TXOPT_ACKREQ */
    uint8_t rsv[6];
};

/* MLME-DISASSOCIATE.confirm */
struct mcp_disassocconf {
    uint8_t cmd; /* MCP_MLME_DISASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t panid;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t status;
    uint64_t addr;
};

/* MLME-DISASSOCIATE.indication */
struct mcp_disassocind {
    uint8_t cmd; /* MCP_MLME_DISASSOCIATE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t rsv[3];
    uint8_t reason;
    uint64_t addr;
};

/* MLME-SCAN.request */
struct mcp_scanreq {
    uint8_t cmd; /* MCP_MLME_SCAN */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t type; /* e.g. MCP_SCAN_ENERGY */
    uint8_t page;
    uint16_t ch_offset;
    uint32_t ch_mask;
    uint16_t options; /* MCP_TXOPT_..., MCP_OPT_LQI_SCAN */
    uint8_t duration;
    uint8_t rsv;
};
#define MCP_SCAN_ENERGY             0U
#define MCP_SCAN_ACTIVE             1U
#define MCP_SCAN_PASSIVE            2U
#define MCP_SCAN_ORPHAN             3U
#define MCP_SCAN_ENHANCED           4U
#define MCP_SCAN_RIT_PASSIVE        5U

/* MLME-SCAN.confirm */
struct mcp_scanconf {
    uint8_t cmd; /* MCP_MLME_SCAN */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t type; /* e.g. MCP_SCAN_ENERGY */
    uint16_t ch_offset;
    uint32_t unscanned;
    uint8_t page;
    uint8_t rsv[3];
    /* Scan results will be returned as TLVs. */
};

/* MLME-BEACON-NOTIFY.indication */
struct mcp_beaconind {
    uint8_t cmd; /* MCP_MLME_BEACON */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t type; /* e.g. MCP_BEACON_DEFAULT */
    uint8_t bsn;
    uint8_t rsv[2];
    /* PAN Descriptor TLV is mandatory. (struct mcp_pandesc) */
    /* Beacon payload may be present as payload TLV. (struct mcp_payload) */
};
#define MCP_BEACON_DEFAULT  0U
#define MCP_BEACON_ENHANCED 1U /* IEEE 802.15.4e amendment */

/* MLME-BEACON.request (IEEE 802.15.4e amendment) */
struct mcp_beaconreq {
    uint8_t cmd; /* MCP_MLME_BEACON */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t type;
    uint8_t page;
    uint16_t channel;
    uint8_t sfo;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint16_t panid;
    uint16_t txopt; /* e.g. MCP_TXOPT_ACKREQ */
    int8_t txpower; /* dBm */
    uint8_t rsv[1];
    uint64_t addr;
};

/* MLME-BEACON.confirm (IEEE 802.15.4e amendment) */
struct mcp_beaconconf {
    uint8_t cmd; /* MCP_MLME_BEACON */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-BEACON-REQUEST.indication */
struct mcp_bcnreqind {
    uint8_t cmd; /* MCP_MLME_BCNREQ */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t type;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint16_t panid;
    uint64_t addr;
    int8_t rssi;
    uint8_t rsv[7];
};

#define MCP_BCNREQIND_SIZE_V1_0   (MCP_END_OF(struct mcp_bcnreqind, addr))
#define MCP_BCNREQIND_SIZE_V3_8   (MCP_END_OF(struct mcp_bcnreqind, rsv)) /* Added rssi (and trailing rsv) */

/* MLME-BEACON-REQUEST.request */
struct mcp_bcnreqreq {
    uint8_t cmd; /* MCP_MLME_BCNREQ */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t options;
    uint8_t rsv[2];
};

/* MLME-BEACON-REQUEST.confirm */
struct mcp_bcnreqconf {
    uint8_t cmd; /* MCP_MLME_BCNREQ */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-COMM-STATUS.indication */
struct mcp_commind {
    uint8_t cmd; /* MCP_MLME_COMM */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t panid;
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t dst_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t src_addr;
    uint64_t dst_addr;
    uint8_t status;
    uint8_t rsv[7];
};

/* MLME-POLL.request */
struct mcp_pollreq {
    uint8_t cmd; /* MCP_MLME_POLL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t panid;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t rsv;
    uint64_t addr;
};

/* MLME-POLL.confirm */
struct mcp_pollconf {
    uint8_t cmd; /* MCP_MLME_POLL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-POLL.indication (ZigBee PRO R21 extension) */
struct mcp_pollind {
    uint8_t cmd; /* MCP_MLME_POLL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t rsv;
    uint16_t panid;
    uint64_t addr;
};

/* MLME-RX-ENABLE.request */
struct mcp_rxenreq {
    uint8_t cmd; /* MCP_MLME_RX_ENABLE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint32_t on_time;
    uint32_t on_duration;
    uint8_t defer;
    uint8_t rsv[3];
};

/* MLME-RX-ENABLE.confirm */
struct mcp_rxenconf {
    uint8_t cmd; /* MCP_MLME_RX_ENABLE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-SYNC-LOSS.indication */
struct mcp_syncind {
    uint8_t cmd; /* MCP_MLME_SYNC */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t panid;
    uint16_t channel;
    uint8_t page;
    uint8_t reason;
    uint8_t rsv[6];
};

/* MLME-SYNC.request */
struct mcp_syncreq {
    uint8_t cmd; /* MCP_MLME_SYNC */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t channel;
    uint8_t page;
    uint8_t track;
};

/* MLME-ORPHAN.indication */
struct mcp_orphanind {
    uint8_t cmd; /* MCP_MLME_ORPHAN */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t rsv[4];
    uint64_t addr;
};

/* MLME-ORPHAN.response */
struct mcp_orphanrsp {
    uint8_t cmd; /* MCP_MLME_ORPHAN */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t addr16;
    uint8_t member;
    uint8_t rsv;
    uint64_t addr64;
};

/* MLME-GET-POWER-INFORMATION-TABLE.request */
struct mcp_getpowertblreq {
    uint8_t cmd; /* MCP_MLME_GETPWRTBL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t dst_addr16; /* 0xfffe (IEEE802154_ADDR_SHORT_NONE) = empty/invalid */
    uint8_t rsv[2];
    uint64_t dst_addr64; /* 0 = empty/invalid */
};

/* MLME-GET-POWER-INFORMATION-TABLE.confirm */
struct mcp_getpowertblconf {
    uint8_t cmd; /* MCP_MLME_GETPWRTBL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    int8_t tx_power; /* TX Power Level for this destination */
    uint16_t dst_addr16; /* 0xfffe (IEEE802154_ADDR_SHORT_NONE) = empty/invalid */
    uint64_t dst_addr64; /* 0 = empty/invalid */
    int8_t last_rssi; /* For debugging only. Last known RSSI measurement from this device */
    uint8_t is_nhle_negot; /* Has txPower been modified by the NHLE */
    uint8_t rsv[6];
};

/* MLME-SET-POWER-INFORMATION-TABLE.request */
struct mcp_setpowertblreq {
    uint8_t cmd; /* MCP_MLME_SETPWRTBL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t dst_addr16; /* 0xfffe (IEEE802154_ADDR_SHORT_NONE) = empty/invalid */
    int8_t tx_power; /* TX Power Level for this destination */
    int8_t last_rssi; /* For debugging only. Last known RSSI measurement from this device */
    uint64_t dst_addr64; /* 0 = empty/invalid */
    uint8_t is_nhle_negot; /* Has txPower been modified by the NHLE */
    uint8_t rsv[7];
};

/* MLME-SET-POWER-INFORMATION-TABLE.confirm */
struct mcp_setpowertblconf {
    uint8_t cmd; /* MCP_MLME_SETPWRTBL */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

#define MCP_DUTYCYCLE_STATUS_NORMAL            0U
#define MCP_DUTYCYCLE_STATUS_LIMITED           1U
#define MCP_DUTYCYCLE_STATUS_CRITICAL          2U
#define MCP_DUTYCYCLE_STATUS_SUSPENDED         3U

/* MLME-DUTY-CYCLE-MODE.indication */
struct mcp_dutycycleind {
    uint8_t cmd; /* MCP_MLME_DUTYCYCLE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t mode; /* e.g. MCP_DUTYCYCLE_STATUS_NORMAL */
    uint8_t rsv[3];
};

#define MCP_TSCH_OPERATION_ADD      0x00
#define MCP_TSCH_OPERATION_DELETE   0x01
#define MCP_TSCH_OPERATION_MODIFY   0x02
#define MCP_TSCH_OPERATION_PUBLIC   0x80

#define MCP_TSCH_LINK_TX        0x0001
#define MCP_TSCH_LINK_RX        0x0002
#define MCP_TSCH_LINK_SHARED    0x0004
#define MCP_TSCH_LINK_TIMEKEEP  0x0008
#define MCP_TSCH_LINK_PRIORITY  0x0010
#define MCP_TSCH_LINK_ADVERTISE 0x0020

/* MLME-SET-SLOTFRAME.request */
struct mcp_sframereq {
    uint8_t cmd; /* MCP_MLME_SET_SLOTFRAME */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t handle;
    uint8_t operation; /* MCP_TSCH_OPERATION_... */
    uint16_t size;
};

/* MLME-SET-SLOTFRAME.confirm */
struct mcp_sframeconf {
    uint8_t cmd; /* MCP_MLME_SET_SLOTFRAME */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t handle;
    uint8_t status;
    uint8_t rsv[2];
};

/* MLME-SET-LINK.request */
struct mcp_linkreq {
    uint8_t cmd; /* MCP_MLME_SET_LINK */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t handle;
    uint8_t operation; /* MCP_TSCH_OPERATION_... */
    uint8_t slotframe;
    uint16_t timeslot;
    uint16_t offset;
    uint16_t type; /* MCP_TSCH_LINK_... */
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t rsv[1];
    uint64_t addr;
    uint64_t lifetime; /* Number of timeslots until the link expires */
};

#define MCP_LINKREQ_SIZE_V1_0   (MCP_END_OF(struct mcp_linkreq, addr))
#define MCP_LINKREQ_SIZE_V3_2   (MCP_END_OF(struct mcp_linkreq, lifetime)) /* Added lifetime */

/* MLME-SET-LINK.confirm */
struct mcp_linkconf {
    uint8_t cmd; /* MCP_MLME_SET_LINK */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t handle;
    uint8_t slotframe;
    uint8_t status;
};

#define MCP_TSCH_MODE_ENABLE    0x01
#define MCP_TSCH_MODE_CCA       0x02

/* MLME-TSCH-MODE.request */
struct mcp_tschreq {
    uint8_t cmd; /* MCP_MLME_TSCH_MODE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    struct mcp_time timestamp;
    uint32_t asn; /* Low 32-bits of ASN. */
    uint8_t asnhi; /* High 8 bits of ASN */
    uint8_t mode; /* MCP_TSCH_MODE_... */
    uint8_t rsv[6];
};

/* MLME-TSCH-MODE.confirm */
struct mcp_tschconf {
    uint8_t cmd; /* MCP_MLME_TSCH_MODE */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t mode; /* MCP_TSCH_MODE_... */
    uint8_t status;
    uint8_t rsv[2];
};

/* WS-START.request */
struct mcp_wsstartreq {
    uint8_t cmd; /* MCP_WISUN_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t ch_function;
    uint8_t reg_domain;
    uint8_t op_class;
    uint8_t rsv[1];
    uint8_t netname[32];
    /* TLVs may follow to define excluded channel ranges. */
};

/* WS-START.confirm */
struct mcp_wsstartconf {
    uint8_t cmd; /* MCP_WISUN_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* MLME-EVENT.indication */
struct mcp_eventind {
    uint8_t cmd; /* MCP_MLME_EVENT */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t event; /* mcp_EVENT_* code */
    uint8_t rsv[2];
};

/* MLME-EVENT.indication for mcp_EVENT_LINK_EXPIRED */
struct mcp_eventind_link {
    struct mcp_eventind event;
    uint16_t link_handle;
    uint8_t slotframe;
    uint8_t rsv[5];
};

/*---------------------------------------------------------
 * IEEE 802.15.4-2015 Primitives
 *---------------------------------------------------------
 */
/* VENDOR_COMMAND.request */
struct mcp_vendorreq {
    uint8_t cmd; /* MCP_VENDOR_COMMAND */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t dst_panid;
    uint8_t dst_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t dst_addr;
    uint8_t oui[3];
    uint8_t rsv[5];
    /* Payload TLV may follow as the last TLV. */
};

/* VENDOR_COMMAND.confirm */
struct mcp_vendorconf {
    uint8_t cmd; /* MCP_VENDOR_COMMAND */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint16_t panid;
    uint64_t src_addr;
    struct mcp_time timestamp;
};

/* VENDOR_COMMAND.indication */
struct mcp_vendorind {
    uint8_t cmd; /* MCP_VENDOR_COMMAND */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint16_t src_panid;
    uint8_t src_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t dst_mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t src_addr;
    uint64_t dst_addr;
    uint16_t dst_panid;
    uint8_t dsn;
    int8_t rssi;
    struct mcp_time timestamp;
    uint8_t oui[3];
    uint8_t rsv[1];
    /* Payload TLV may follow as the last TLV. */
};

/*---------------------------------------------------------
 * IEEE 802.15.4 PHY Primitives
 *---------------------------------------------------------
 */
/* PD-DATA.request */
struct mcp_phydatareq {
    uint8_t cmd; /* MCP_PD_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
    /* Payload TLV is mandatory. */
};

/* PD-DATA.confirm */
struct mcp_phydataconf {
    uint8_t cmd; /* MCP_PD_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
    /* Payload TLV is mandatory. */
};

/* PD-DATA.indication */
struct mcp_phydataind {
    uint8_t cmd; /* MCP_PD_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    struct mcp_time sof_ts;
    int8_t rssi;
    uint8_t fcslen;
    uint16_t data_rate;
    struct mcp_time eof_ts;
    /* Payload TLV may follow as the last TLV. */
};

/*---------------------------------------------------------
 * Device Control Primitive Definitions
 *---------------------------------------------------------
 */
/* DEVICE-IDENTIFIER.request */
struct mcp_identreq {
    uint8_t cmd; /* MCP_DEVICE_IDENT */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
};

/* DEVICE-IDENTIFIER.confirm */
struct mcp_identconf {
    uint8_t cmd; /* MCP_DEVICE_IDENT */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t if_major;
    uint8_t if_minor;
    uint8_t fw_major;
    uint8_t fw_minor;
    uint8_t fw_rev;
    uint8_t rsv;
    uint16_t mac_rev;
    uint32_t phy_amend; /* MCP_AMEND_...*/
    uint32_t hw_id;
    uint32_t hw_rev;
    char pname[8]; /* Equivalent to license product string */
};
#define MCP_AMEND_802154e  0x0001
#define MCP_AMEND_802154g  0x0002

/* DEVICE-START.request */
struct mcp_devstartreq {
    uint8_t cmd; /* MCP_DEVICE_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
};

/* DEVICE-START.confirm */
struct mcp_devstartconf {
    uint8_t cmd; /* MCP_DEVICE_START */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t status;
    uint8_t rsv[3];
};

/* DEVICE-RESET.request */
struct mcp_devresetreq {
    uint8_t cmd; /* MCP_DEVICE_RESET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
};

/* DEVICE-RESET.confirm */
struct mcp_devresetconf {
    uint8_t cmd; /* MCP_DEVICE_RESET */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint8_t reason; /* MCP_RESET_... */
    uint8_t rsv[3];
};
#define MCP_RESET_UNKNOWN   0
#define MCP_RESET_POWERON   1
#define MCP_RESET_SOFT      2

/* DEVICE-REGISTERS.request */
struct mcp_regreq {
    uint8_t cmd; /* MCP_DEVICE_REGISTER */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint16_t offset;
    uint16_t count;
};

/* DEVICE-REGISTERS.confirm */
struct mcp_regconf {
    uint8_t cmd; /* MCP_DEVICE_REGISTER */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t next;
    uint8_t rsv[2];
    /* Read registers returned as TLVs. */
};

/* DEVICE-DATA.request */
struct mcp_devdatareq {
    uint8_t cmd; /* MCP_DEVICE_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_REQUEST */
    uint8_t rsv[4];
    /* Raw payload */
};

/* DEVICE-DATA.confirm */
struct mcp_devdataconf {
    uint8_t cmd; /* MCP_DEVICE_DATA */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_CONFIRM */
    uint16_t window; /* number of bytes able to receive */
    uint8_t rsv[2];
};

/* ERROR.indication */
struct mcp_error {
    uint8_t cmd; /* MCP_DEVICE_ERROR */
    uint8_t hdr_rsv;
    uint8_t seqno;
    uint8_t flags; /* MCP_FLAG_INDICATION */
    uint8_t err_cmd; /* The input command that generated the error */
    uint8_t rsv;
    uint8_t err_code; /* MCP_ERROR_... */
    uint8_t err_flags; /* The input command's flags */
};
#define MCP_ERROR_NONE          0
#define MCP_ERROR_GENERAL       1
#define MCP_ERROR_UNSUPPORTED   2
#define MCP_ERROR_MALFORMED     3
#define MCP_ERROR_OVERFLOW      4

/*---------------------------------------------------------
 * TLV Definitions
 *---------------------------------------------------------
 */
/* Internal TLV header structure, just used to keep the macros concise. */
struct mcp_tlv_hdr {
    uint8_t type;
    uint8_t len;
    uint16_t datalen; /* When type == MCP_TLV_PAYLOAD, length of data following this TLV header. */
    uint8_t rsv[4];
};

#define MCP_TLV_LEN(_sz_) \
    /*lint -save -e778 [!MISRA - constant exp. evals to 0 in'/'] */ \
    ((uint8_t)(((_sz_) - 1U) / 8U))  \
    /*lint -restore  */

#define MCP_TLV_TYPE(_p_)       (((struct mcp_tlv_hdr *)(_p_))->type)
#define MCP_TLV_SIZE(_p_)       __mcp_tlv_size(_p_)
#define MCP_TLV_SPACE(_len_)    (((_len_) + 7) & ~7u)
#define MCP_TLV_NEXT(_p_)       ((void *)((uint8_t *)(_p_) + MCP_TLV_SIZE(_p_)))

/*
 * TLV Iteration Macro:
 * _p_ is a pointer variable, it will be set to the current TLV in the loop body.
 * _start_ is a pointer to the start of the TLVs to iterate over, it should remain constant while iterating.
 * _len_ is the length of the TLV data to iterate over, it should remain constant while iterating.
 */
#define MCP_TLV_FOREACH(_p_, _start_, _len_) \
    for ((_p_) = (void *)(_start_); \
         ((uint8_t *)(_p_) + 8) <= ((uint8_t *)(_start_) + (_len_))    \
         && (uint8_t *)MCP_TLV_NEXT(_p_) <= ((uint8_t *)(_start_) + (_len_)); \
         (_p_) = MCP_TLV_NEXT(_p_))

/* MCP TLV type codes. */
#define MCP_TLV_PAD         0 /* 0x00 */
#define MCP_TLV_PAYLOAD     1 /* 0x01 - When present, always the final TLV. */
#define MCP_TLV_ENERGYLIST  2 /* 0x02 */
#define MCP_TLV_PANDESC     3 /* 0x03 */
#define MCP_TLV_SECURITY    4 /* 0x04 */
#define MCP_TLV_RANGING     5 /* 0x05 */
#define MCP_TLV_UWB         6 /* 0x06 */
#define MCP_TLV_HIE         7 /* 0x07 */
#define MCP_TLV_PIE         8 /* 0x08 */
#define MCP_TLV_BCN_BITMAP  9 /* 0x09 */
#define MCP_TLV_HOPPING     10 /* 0x0a */
#define MCP_TLV_MPM_SCAN    11 /* 0x0b */
#define MCP_TLV_EBEACON     12 /* 0x0c */
#define MCP_TLV_MODE_SWITCH 13 /* 0x0d */
#define MCP_TLV_ATTRIBUTE   14 /* 0x0e */
#define MCP_TLV_REGISTER    15 /* 0x0f */
#define MCP_TLV_PENDADDR    16 /* 0x10 */
#define MCP_TLV_LINKQUALITY 17 /* 0x11 */
#define MCP_TLV_MIE         18 /* 0x12 */
#define MCP_TLV_TSCH_DIAG   19 /* 0x13 */
#define MCP_TLV_PHY_DIAG    20 /* 0x14 */

#define MCP_TLV_IS_IE(_p_) ( \
        MCP_TLV_TYPE(_p_) == MCP_TLV_HIE    \
        || MCP_TLV_TYPE(_p_) == MCP_TLV_PIE    \
        || MCP_TLV_TYPE(_p_) == MCP_TLV_MIE \
        )

#define MCP_TLV_IS_VENDOR_IE(_p_) ( \
        (MCP_TLV_TYPE(_p_) == MCP_TLV_HIE && ((struct mcp_ie *)(_p_))->id == mcp_IEEE802154_HIE_VENDOR_SPECIFIC)    \
        || (MCP_TLV_TYPE(_p_) == MCP_TLV_PIE && ((struct mcp_ie *)(_p_))->id == mcp_IEEE802154_PIE_VENDOR_SPECIFIC)    \
        || (MCP_TLV_TYPE(_p_) == MCP_TLV_MIE && ((struct mcp_ie *)(_p_))->id == mcp_IEEE802154_MIE_VENDOR_SPECIFIC) \
        )

#ifdef __cplusplus
extern "C" {
#endif

static inline unsigned int
__mcp_tlv_size(const void *tlv)
{
    const struct mcp_tlv_hdr *t = tlv;
    unsigned int sz;

    switch (t->type) {
        case MCP_TLV_PAD:
            sz = 1U;
            break;

        case MCP_TLV_PAYLOAD:
            sz = (unsigned int)le16toh(t->datalen) + 8U;
            break;

        default:
            sz = ((unsigned int)t->len + 1U) * 8U;
            break;
    }
    return sz;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

/*
 * Payload TLV object
 *
 * When present, the Payload TLV MUST be the final TLV and MUST set the length
 * to zero. The payload data follows immediately after the payload TLV and until
 * the end of the encapsulating message.
 */
struct mcp_payload {
    uint8_t type; /* MCP_TLV_PAYLOAD */
    uint8_t len; /* MUST be zero. */
    uint16_t datalen;
    uint8_t rsv[4];
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t data[];
    /*lint -restore */
};

/* EnergyList TLV Object */
struct mcp_energylist {
    uint8_t type; /* MCP_TLV_ENERGYLIST */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_energylist) + num_channels) */
    uint16_t ch_offset; /* Channel Number (N) = index_into_energy[] + ch_offset */
    uint8_t rsv[4];
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t energy[]; /* In dB above IEEE specified radio sensitivity. */
    /*lint -restore */
};

/* PAN Descriptor TLV Object */
struct mcp_pandesc {
    uint8_t type; /* MCP_TLV_PANDESC */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_pandesc)) + ... */
    uint16_t channel;
    uint16_t panid;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint8_t options; /* e.g. MCP_PANDESC_OPT_GTS_PERMIT */
    uint64_t addr;
    uint16_t sframe; /* e.g. MCP_PANDESC_SFRAME_ASSOC_PERMIT */
    uint8_t page;
    int8_t rssi;
    struct mcp_time timestamp;
    uint8_t rsv[4];
    /* Security and UWB TLV may follow as sub-tlvs. */
};

/* PAN Descriptor Superframe Specification */
#define MCP_PANDESC_SFRAME_BCN_ORD          0xF000U
#define MCP_PANDESC_SFRAME_SF_ORD           0x0F00U
#define MCP_PANDESC_SFRAME_CAP_SLOT         0x00F0U
#define MCP_PANDESC_SFRAME_BATT_EXT         0x0008U
#define MCP_PANDESC_SFRAME_PAN_COORD        0x0002U
#define MCP_PANDESC_SFRAME_ASSOC_PERMIT     0x0001U

/* PAN Descriptor Options field */
#define MCP_PANDESC_OPT_GTS_PERMIT          0x01

/* Security Info TLV Object. */
struct mcp_security {
    uint8_t type; /* MCP_TLV_SECURITY */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_security)) */
    uint8_t status;
    uint8_t level;
    uint8_t keymode; /* MCP_KEYMODE... */
    uint8_t keyindex;
    uint8_t rsv[2];
    uint64_t keysrc;
};
#define MCP_KEYMODE_NONE     0
#define MCP_KEYMODE_DEFAULT  1
#define MCP_KEYMODE_SHORT    2
#define MCP_KEYMODE_LONG     3

/* Ranging TLV Object */
struct mcp_ranging {
    uint8_t type; /* MCP_TLV_RANGING */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_ranging)) */
    uint8_t status;
    uint8_t fom;
    uint32_t start;
    uint32_t stop;
    uint32_t interval;
    uint32_t offset;
    uint8_t rsv[4];
};

/* UWB TLV Object */
struct mcp_uwb {
    uint8_t type; /* MCP_TLV_UWB */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_uwb)) */
    uint8_t category;
    uint8_t prf;
    uint32_t repetition;
    uint32_t codelist;
    uint8_t rsv[4];
};

/* Header IE, Payload IE and Nested IE Objects */
/*
 * Note that the Payload IE with a Group ID of 1 (the MLME IE) is a special
 * case. The IE contains nested TLVs, which are parsed into their own TLVs of
 * type MCP_TLV_MIE.
 */
struct mcp_ie {
    uint8_t type; /* MCP_TLV_HIE, MCP_TLV_PIE or MCP_TLV_MIE */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_ie) + ielen) */
    uint16_t ielen;
    uint8_t id;
    uint8_t oui[3]; /* Only for Vendor-specific IEs; else reserved. */
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t data[]; /* The IE content follows. */
    /*lint -restore */
};

/* Beacon Bitmap TLV Object */
struct mcp_bbits {
    uint8_t type; /* MCP_TLV_BCN_BITMAP */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_bbits) + sizeof(bits)) */
    uint8_t rsv[2];
    uint16_t index;
    uint16_t bitslen; /* number of 8bit groups (bytes) */
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t bits[];
    /*lint -restore */
};

/* Channel Hopping TLV Object */
/* TODO: Still to be defined. */

/* MPM Scan TLV Object */
struct mcp_mpmscan {
    uint8_t type; /* MCP_TLV_MPM_SCAN */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_mpmscan)) */
    uint8_t rsv;
    uint8_t scan; /* TODO: Enumerations. */
    uint32_t duration;
};

/* Enhanced Beacon TLV Object */
struct mcp_ebeacon {
    uint8_t type; /* MCP_TLV_EBEACON */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_ebeacon)) */
    uint8_t rsv;
    uint8_t offset;
    uint32_t order;
};

/* Mode Switch TLV Object */
struct mcp_modesw {
    uint8_t type; /* MCP_TLV_MODE_SWITCH */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_modesw)) */
    uint16_t phytype;
    uint16_t phymode;
    uint8_t rsv;
    uint8_t param;
};

/* Attribute TLV Object */
struct mcp_attr {
    uint8_t type; /* MCP_TLV_ATTRIBUTE */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_attr) + sizeof(value)) */
    uint16_t id; /* PIBAttributeID */
    uint16_t index; /* PIBAttributeIndex */
    uint8_t status; /* Only applicable in *.confirm for attributes that failed. */
    uint8_t padlen; /* Length of padding for unstructured blob attributes. */
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t value[];
    /*lint -restore */
};

/* Register TLV Object */
struct mcp_register {
    uint8_t type; /* MCP_TLV_REGISTER */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_register) + strlen(name) + 1) */
    uint16_t offset;
    uint32_t value;
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    char name[]; /* Null terminated string. */
    /*lint -restore */
};

/* Pending Address TLV object. */
struct mcp_pendaddr {
    uint8_t type; /* MCP_TLV_PENDADDR */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_pendaddr) + sizeof(addrlist)) */
    uint8_t mode;
    uint8_t rsv[5];
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint64_t addrlist[];
    /*lint -restore */
};

/* Link Quality TLV Object. */
struct mcp_linkquality {
    uint8_t type; /* MCP_TLV_LINKQUALITY */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_linkquality)) */
    uint8_t lqi; /* link quality indication */
    uint8_t rsv[5];
};

/* TSCH Diagnostics TLV Object. */
struct mcp_tschdiag {
    uint8_t type; /* MCP_TLV_TSCH_DIAG */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_tschdiag)) */
    uint8_t rsv[6];
    uint64_t asn; /* Current ASN */
    struct mcp_time slotstart; /* Time at start of timeslot */
};

/* PHY Diagnostics TLV Object. */
struct mcp_phydiag {
    uint8_t type; /* MCP_TLV_PHY_DIAG */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_phydiag)) */
    uint8_t page;
    uint8_t rsv0;
    uint16_t channel;
    uint16_t phyband;
    uint16_t phytype;
    uint8_t rsv1[6];
};

/*---------------------------------------------------------
 * Structured Attribute Definitions
 *---------------------------------------------------------
 */

/* All scalar attributes use a 32-bit word or 64-bit double word. */
/* Other attribute structures are defined below. */
struct mcp_scalar {
    struct mcp_attr attr;
    /*lint -save -e658 [!MISRA - anonymous union assumed ] */
    union {
        uint32_t value;
        uint64_t value64;
    };
    /*lint -restore */
};

/* Statistics (ID==0x68). */
struct mcp_statistics {
    struct mcp_attr attr;
    uint32_t tx_success; /* Total packets transmitted successfully */
    uint32_t tx_fail; /* Total packets attempted to transmit but failed */
    uint32_t tx_cca_fail; /* TX failures because of CCA failures */
    uint32_t tx_no_ack_fail; /* TX failures because no ack received (macTxFailCount) */
    uint32_t tx_ftype[4]; /* TX Frame types */
    uint32_t rx_success; /* Total packets received correctly (macRxSuccessCount) */
    uint32_t rx_fail; /* Total packets received but failed to parse */
    uint32_t rx_malformed; /* Packets which could not be parsed correctly */
    uint32_t rx_security; /* Security related errors (macSecurityFailure) */
    uint32_t rx_duplicate; /* Duplicate packets dropped (macDuplicateFrameCount) */
    uint32_t tx_no_retries; /* Packets transmitted and acked with no retries (macTxSuccessCount) */
    uint32_t rx_ftype[4]; /* RX Frame types */
    uint32_t tx_one_retry; /* TX acked after one retry (macRetryCount) */
    uint32_t tx_multi_retries; /* TX acked after more than one retry (macMultipleRetryCount) */
    uint32_t sync_beacon; /* Resync triggered by mismatch with beacn ASN */
    uint32_t sync_latency; /* Resync triggered by timeslot latency */
    uint32_t sync_timeslots; /* Number of timeslots missed during timer latency */
    uint32_t rsv;
};

#define MCP_STATISTICS_SIZE_V1_0    (MCP_END_OF(struct mcp_statistics, rx_ftype))
#define MCP_STATISTICS_SIZE_V3_4    (MCP_END_OF(struct mcp_statistics, tx_multi_retries)) /* Removed rsv, added tx_no_retries, tx_one_retry, tx_multi_retries */
#define MCP_STATISTICS_SIZE_V3_5    (MCP_END_OF(struct mcp_statistics, rsv)) /* Added sync_beacon, sync_latency, sync_timeslot */

/* Counters as id/count pair. */
struct mcp_id_counter {
    uint32_t id;
    uint32_t count;
};
/* PHY Statistics (ID==0x6c). */
struct mcp_counters {
    struct mcp_attr attr;
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    struct mcp_id_counter entry[];
    /*lint -restore */
};

/* Security Level Descriptor Attribute (ID==0x75) */
struct mcp_seclevel {
    struct mcp_attr attr;
    uint64_t usage;
    uint64_t override;
    uint8_t minimum;
    uint8_t rsv[7];
};

/* Security KeyDescriptor Attribute (ID==0x71) */
struct mcp_keydesc {
    struct mcp_attr attr;
    uint64_t usage;
    uint8_t keymode; /* MCP_KEYMODE_... */
    uint8_t keyindex;
    uint16_t panid; /* PANID for implicit lookups using MCP_KEYMODE_NONE */
    uint8_t rsv[4];
    uint64_t keysrc; /* Key source for explicit lookups, and address for implicit lookups. */
    uint8_t key[16];
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t devices[]; /* Variable sized bit mask */
    /*lint -restore */
};

/* Security Device Descriptor (ID==0x73)*/
struct mcp_keydevice {
    struct mcp_attr attr;
    uint64_t addr64;
    uint16_t panid;
    uint16_t addr16;
    uint32_t counter;
    uint32_t exempt;
    uint8_t rsv[4];
};

/* SunPageDescriptor Attribute (ID==0x206) */
struct mcp_sunpage_desc {
    struct mcp_attr attr;
    uint32_t bands; /* Bit-map of supported frequency bands. */
    uint32_t modes; /* Bit-map of supported rate-modes. */
};

/* SunPageEntry Attribute (ID==0x207) */
struct mcp_sunpage_entry {
    struct mcp_attr attr;
    uint16_t band;
    uint16_t phytype;
    uint16_t phymode;
    uint16_t rsv;
};

/* TSCH Slotframe Table Entry Attribute (ID=0x404) */
struct mcp_tsch_slotframe {
    struct mcp_attr attr;
    uint16_t size;
    uint8_t handle;
    uint8_t visibility; /* MCP_SLOTFRAME_VISIB_... */
    uint8_t rsv[4];
};

#define MCP_SLOTFRAME_VISIB_UNKNOWN   0x00 /* Default indicating older version. */
#define MCP_SLOTFRAME_VISIB_PRIVATE   0x01
#define MCP_SLOTFRAME_VISIB_PUBLIC    0x02

/* TSCH Link Table Entry Attribute (ID==0x406) */
struct mcp_tsch_link {
    struct mcp_attr attr;
    uint16_t handle;
    uint16_t type;
    uint16_t timeslot;
    uint16_t offset;
    uint32_t rsv;
    uint16_t size; /* From matching macSlotframeTable entry. */
    uint8_t slotframe;
    uint8_t mode; /* e.g. MCP_ADDRMODE_SHORT */
    uint64_t addr;
    uint64_t lifetime; /* Number of timeslots until the link expires */
};

/* License Challenge Attribute (ID==0x6C) */
struct mcp_license_challenge {
    struct mcp_attr attr;
    /*lint -save -e9038 [flexible array member declared 18.7 REQUIRED] */
    uint8_t challenge[];
    /*lint -restore */
};

/* License Certificate Attribute (ID==0x6D) */
/*
 * TODO: Check if the license fields are big-endian or little-endian. We may
 * need to document this for backwards compatibility. Changing endianness would
 * require re-signing all the existing licenses.
 */
struct mcp_license {
    struct mcp_attr attr;
    uint64_t addr; /* First address covered by this license. */
    uint16_t cflags; /* Certificate flags. */
    uint8_t range; /* Number of addresses covered by this license. */
    uint8_t rsv;
    uint32_t pen; /* Manufacturer IANA PEN */
    uint8_t pname[8]; /* Product name. */
    uint32_t pflags; /* Product flags. */
    uint32_t xflags; /* Extra flags (reserved). */
    uint8_t pubx[32]; /* Device public key (X-coordinate). */
    uint8_t r[32]; /* ECDSA (P256) Signature component 'r' */
    uint8_t s[32]; /* ECDSA (P256) Signature component 's' */
};

#endif /* MCP_H */
