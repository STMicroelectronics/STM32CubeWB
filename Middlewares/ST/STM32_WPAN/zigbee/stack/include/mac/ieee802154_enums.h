/* Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved. */

#ifndef IEEE802154_ENUMS_H
#define IEEE802154_ENUMS_H

#include "mcp_enums.h" /* MAC Status and Enumerations */

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

#endif
