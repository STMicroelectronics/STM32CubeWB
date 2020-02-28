/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL DRLC cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZGP_PROXYBASIC_H
# define ZGP_PROXYBASIC_H

/*---------------------------------------------------------------
 * Dependencies
 *---------------------------------------------------------------
 */
#include "zigbee.h"
#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/* Maximum ZGP Message size (after fragmentation). */
#define ZB_ZGP_ASDU_LENGTH_GREEN_POWER              ZB_APS_CONST_MAX_PAYLOAD_SIZE

/* GP Proxy Basic Commands Received */
#define ZB_ZGP_GP_PAIRING                           0x01
#define ZB_ZGP_GP_PROXY_COMMISSIONING_MODE          0x02
#define ZB_ZGP_GP_RESPONSE                          0x06
#define ZB_ZGP_GP_SINK_TABLE_REQUEST                0x0a
#define ZB_ZGP_GP_PROXY_TABLE_REQUEST               0x0b

/* GP Proxy Basic Commands Sent */
#define ZB_ZGP_GP_NOTIFICATION                      0x00
#define ZB_ZGP_GP_COMMISSIONING_NOTIFICATION        0x04
#define ZB_ZGP_GP_SINK_TABLE_RESPONSE               0x0a
#define ZB_ZGP_GP_PROXY_TABLE_RESPONSE              0x0b

/* Proxy Basic Client attribute identifiers */
enum {
    ZCL_PROXYBASIC_CLI_ATTR_GPP_MAX_PROXY_TABLE_ENTRIES = 0x0010,
    ZCL_PROXYBASIC_CLI_ATTR_PROXY_TABLE = 0x0011,
#if 0
    ZCL_PROXYBASIC_CLI_ATTR_GPP_NOTIFICATION_RETRY_NUMBER = 0x0012,
    ZCL_PROXYBASIC_CLI_ATTR_GPP_NOTIFICATION_RETRY_TIMER = 0x0013,
    ZCL_PROXYBASIC_CLI_ATTR_GPP_MAX_SEARCH_COUNTER = 0x0014,
    ZCL_PROXYBASIC_CLI_ATTR_GPP_BLOCKED_GPDID = 0x0015,
#endif
    ZCL_PROXYBASIC_CLI_ATTR_GPP_FUNCTIONALITY = 0x0016,
    ZCL_PROXYBASIC_CLI_ATTR_GPP_ACTIVE_FUNCTIONALITY = 0x0017,
#if 0
    ZCL_PROXYBASIC_CLI_ATTR_GPP_SHARED_SECURITY_KEY_TYPE = 0x0020,
    ZCL_PROXYBASIC_CLI_ATTR_GPP_SHARED_SECURITY_KEY = 0x0021,
#endif
    ZCL_PROXYBASIC_CLI_ATTR_GPP_LINK_KEY = 0x0022,
#if 0
    ZCL_PROXYBASIC_CLI_ATTR_GPP_CLUSTER_REVISION = 0xFFFD,
#endif
};

/* Each proxy SHALL be able to support per Proxy Table entry, i.e. per GPD any of the following mini-mum configurations:
(i) at least 2 entries in the Lightweight sink address list and/or Full unicast sink address list,
(ii) at least 2 entries in the Sink group list and
(iii) at least 1 entry in the Lightweight sink address list or Full unicast sink address list and at least 1 entry in the Sink group list.
*/

#if 0 /* EXEGIN - NOT_INCLUDED */
#define ZB_ZGP_MAX_PROXY_TABLE_ENTRIES  5
#define ZB_ZGP_LIGHTWEIGHT_SINK_ADDRESS_LIST_ENTRIES 2
#define ZB_ZGP_SINK_GROUP_ENTRIES 2
#define ZB_ZGP_FULL_UNICAST_SINK_ADDRESS_LIST_ENTRIES 2

typedef struct ZbZgpLightweightSinkAddressT {
    uint64_t sink_ieee_addr;
    uint16_t sink_nwk_addr;
} ZbZgpLightweightSinkAddressT;

typedef struct ZbZgpSinkGroupT {
    uint16_t sink_group;
    uint16_t sink_alias;
} ZbZgpSinkGroupT;

typedef struct ZbZgpProxyTableEntryT {
    uint8_t in_use;
    uint16_t options; // 16-bit bitmap
    union {
        uint32_t GPD_ID_SrcID;
        uint64_t GPD_ID_IEEE;
    }; /* Anonymous Union */
       /* uint32_t GPD_ID; */ // Unsigned 32-bit integer/IEEE address
    uint8_t endpoint; // Unsigned 8-bit integer
    uint16_t GPD_assigned_alias; // Unsigned 16-bit integer
    uint8_t Security_Options; // 8-bit bitmap
    uint32_t GPD_security_frame_counter; // Unsigned 32-bit Integer
    uint8_t GPD_key[ZB_SEC_KEYSIZE]; // Security key
    uint8_t lightweight_sink_address_list_len;
    ZbZgpLightweightSinkAddressT lightweight_sink_address_list[ZB_ZGP_LIGHTWEIGHT_SINK_ADDRESS_LIST_ENTRIES]; // sequence of octets 2 entries A.3.4.2.2.2
    uint8_t sink_group_list_len;
    ZbZgpSinkGroupT sink_group_list[ZB_ZGP_SINK_GROUP_ENTRIES]; // list sequence of octets 2 entries A.3.4.2.2.2
    uint8_t groupcast_radius; // Unsigned 8-bit integer
    uint8_t search_counter; // Unsigned 8-bit integer
    uint16_t extended_options; // 16-bit bitmap
    uint8_t full_unicast_sink_address_list_len;
    ZbZgpLightweightSinkAddressT full_unicast_sink_address_list[ZB_ZGP_FULL_UNICAST_SINK_ADDRESS_LIST_ENTRIES]; // sequence of octets  2 entries A.3.4.2.2.2
} ZbZgpProxyTableEntryT;

#endif

/* Proxy Table in_use field states */

enum {
    ZCL_PROXYBASIC_PT_ENTRY_FREE = 0x0000,
    ZCL_PROXYBASIC_PT_ENTRY_IN_USE,
    ZCL_PROXYBASIC_PT_ENTRY_IN_COMMISSIONING,
};

/* Proxy Table Options Bit Masks */

#define ZGP_PT_APPLICATIONID       0x0007
#define ZGP_PT_APPLICATIONID_SRCID 0x0000
#define ZGP_PT_APPLICATIONID_IEEE  0x0002
#define ZGP_PT_ENTRYACTIVE         0x0008
#define ZGP_PT_ENTRYVALID          0x0010
#define ZGP_PT_SEQUENCE_NUMBER_CAPABILITIES 0x0020
#define ZGP_PT_LIGHTWEIGHT_UNICAST_GPS 0x0040
#define ZGP_PT_DERIVED_GROUP_GPS       0x0080
#define ZGP_PT_COMMISSIONED_GROUP_GPS  0x0100
#define ZGP_PT_FIRSTTOFORWARD          0x0200
#define ZGP_PT_INRANGE                 0x0400
#define ZGP_PT_GPD_FIXED               0x0800
#define ZGP_PT_HASALLUNICASTROUTES     0x1000
#define ZGP_PT_ASSIGNEDALIAS           0x2000
#define ZGP_PT_SECURITYUSE             0x4000
#define ZGP_PT_OPTIONS_EXTENSION       0x8000

/* Proxy table Options_Extension Bit Mask */

#define ZGP_PT_FULL_UNICAST_GPS 0x0001

/* Proxy Table Request Options Bit Mask */

#define ZGP_PT_REQUEST_TYPE       0x0018
#define ZGP_PT_REQUEST_TYPE_GPID  0x0000
#define ZGP_PT_REQUEST_TYPE_INDEX 0x0008

/* Proxy Table Security Options Bit Mask */

#define ZGP_PT_SECURITYLEVEL      0x0003
#define ZGP_PT_NO_SECURITY                                               0x0000
#define ZGP_PT_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC_ONLY                0x0002
#define ZGP_PT_ENCRYPTION_AND_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC      0x0003
#define ZGP_PT_SECURITYKEYTYPE    0x001C
#define ZGP_PT_NO_KEY                                0x0000
#define ZGP_PT_ZIGBEE_NWK_KEY                        0x0004
#define ZGP_PT_GPD_GROUP_KEY                         0x0008
#define ZGP_PT_NWK_KEY_DERIVED_GPD_GROUP_KEY         0x000C
#define ZGP_PT_INDIVIDUAL_OUT_OF_THE_BOX_GPD_KEY     0x0010
#define ZGP_PT_DERIVED_INDIVIDUAL_GPD_KEY            0x001C

/* Pairing Command Options Bit Mask */

#define ZGP_PAIR_APPLICATIONID       0x000007
#define ZGP_PAIR_APPLICATIONID_SRCID 0x000000
#define ZGP_PAIR_APPLICATIONID_IEEE  0x000002
#define ZGP_PAIR_ADD_SINK            0x000008
#define ZGP_PAIR_REMOVE_GPD          0x000010
#define ZGP_PAIR_COMMUNICATION_MODE  0x000060
#define ZGP_PAIR_FULL_UNICAST_FORWARDING_OF_THE_GP_NOTIFICATION                                               0x000000
#define ZGP_PAIR_GROUPCAST_FORWARDING_OF_THE_GP_NOTIFICATION_COMMAND_TO_DGROUPID                              0x000020
#define ZGP_PAIR_GROUPCAST_FORWARDING_OF_THE_GP_NOTIFICATION_COMMAND_TO_PRE_COMMISSIONED_GROUPID              0x000040
#define ZGP_PAIR_UNICAST_FORWARDING_OF_THE_GP_NOTIFICATION_COMMAND_BY_THE_LIGHTWEIGHT_UNICAST_FUNCTIONALITY   0x000060
#define ZGP_PAIR_GPD_FIXED           0x000080
#define ZGP_PAIR_GPD_MAC_SEQUENCE_NUMBER_CAPABILITIES       0x000100
#define ZGP_PAIR_SECURITYLEVEL       0x000600
#define ZGP_PAIR_NO_SECURITY                                               0x000000
#define ZGP_PAIR_DEPRECIATED_SECUIRITY_LEVEL                               0x000200
#define ZGP_PAIR_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC_ONLY                0x000400
#define ZGP_PAIR_ENCRYPTION_AND_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC      0x000600
#define ZGP_PAIR_SECURITYKEYTYPE            0x003800
#define ZGP_PAIR_NO_KEY                                0x000000
#define ZGP_PAIR_ZIGBEE_NWK_KEY                        0x000800
#define ZGP_PAIR_GPD_GROUP_KEY                         0x001000
#define ZGP_PAIR_NWK_KEY_DERIVED_GPD_GROUP_KEY         0x001800
#define ZGP_PAIR_INDIVIDUAL_OUT_OF_THE_BOX_GPD_KEY     0x002000
#define ZGP_PAIR_DERIVED_INDIVIDUAL_GPD_KEY            0x003800
#define ZGP_PAIR_GPD_SECURITY_FRAME_COUNTER_PRESENT    0x004000
#define ZGP_PAIR_GPD_SECURITY_KEY_PRESENT    0x008000
#define ZGP_PAIR_ASSIGNED_ALIAS_PRESENT      0x010000
#define ZGP_PAIR_FORWARDING_RADIUS_PRESENT   0x020000
#define ZGP_PAIR_RESERVED                    0xFC0000

/* Proxy Commissioning Mode Options Bit Masks */

#define ZGP_PCOMM_ACTION 0x01
#define ZGP_PCOMM_EXIT_MODE 0x0E
#define ZGP_PCOMM_EXIT_MODE_ON_COMMISSIONINGWINDOW_EXPIRATION 0x02
#define ZGP_PCOMM_EXIT_MODE_ON_FIRST_PAIRING_SUCCESS 0x04
#define ZGP_PCOMM_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT 0x08
#define ZGP_PCOMM_CHANNEL_PRESENT 0x10
#define ZGP_PCOMM_UNICAST_COMMUNICATION 0x20
#define ZGP_PCOMM_RESERVED  0xC0

/* Response Command Options Bit Masks*/

#define ZGP_RESPONSE_APPLICATIONID       0x0007
#define ZGP_RESPONSE_APPLICATIONID_SRCID 0x0000
#define ZGP_RESPONSE_APPLICATIONID_IEEE  0x0002
#define ZGP_RESPONSE_TRANSMIT_ON_ENDPOINT_MATCH  0x0008
#define ZGP_RESPONSE_RESERVED            0x00F0

/* Commissioning Notification Bit Masks */

#define ZGP_COMMISSIONING_NOTIFICATION_APPLICATIONID       0x0007
#define ZGP_COMMISSIONING_NOTIFICATION_APPLICATIONID_SRCID 0x0000
#define ZGP_COMMISSIONING_NOTIFICATION_APPLICATIONID_IEEE  0x0002
#define ZGP_COMMISSIONING_NOTIFICATION_RXAFTERTX           0x0008
#define ZGP_COMMISSIONING_NOTIFICATION_SECURITYLEVEL       0x0030
#define ZGP_COMMISSIONING_NOTIFICATION_NO_SECURITY                                               0x0000
#define ZGP_COMMISSIONING_NOTIFICATION_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC_ONLY                0x0020
#define ZGP_COMMISSIONING_NOTIFICATION_ENCRYPTION_AND_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC      0x0030
#define ZGP_COMMISSIONING_NOTIFICATION_SECURITYKEYTYPE                       0x01C0
#define ZGP_COMMISSIONING_NOTIFICATION_NO_KEY                                0x0000
#define ZGP_COMMISSIONING_NOTIFICATION_ZIGBEE_NWK_KEY                        0x0040
#define ZGP_COMMISSIONING_NOTIFICATION_GPD_GROUP_KEY                         0x0080
#define ZGP_COMMISSIONING_NOTIFICATION_NWK_KEY_DERIVED_GPD_GROUP_KEY         0x00C0
#define ZGP_COMMISSIONING_NOTIFICATION_INDIVIDUAL_OUT_OF_THE_BOX_GPD_KEY     0x0100
#define ZGP_COMMISSIONING_NOTIFICATION_DERIVED_INDIVIDUAL_GPD_KEY            0x01C0
#define ZGP_COMMISSIONING_NOTIFICATION_SECURITY_PROCESSING_FAILED            0x0200
#define ZGP_COMMISSIONING_NOTIFICATION_BIDIRECTIONAL_CAPABILITY              0x0400
#define ZGP_COMMISSIONING_NOTIFICATION_PROXY_INFO_PRESENT                    0x0800

/* Notification Bit Masks */

#define ZGP_NOTIFICATION_APPLICATIONID       0x0007
#define ZGP_NOTIFICATION_APPLICATIONID_SRCID 0x0000
#define ZGP_NOTIFICATION_APPLICATIONID_IEEE  0x0002
#define ZGP_NOTIFICATION_ALSO_UNICAST        0x0008
#define ZGP_NOTIFICATION_ALSO_DERIVED_GROUP  0x0010
#define ZGP_NOTIFICATION_ALSO_COMMISSIONED_GROUP 0x0020
#define ZGP_NOTIFICATION_SECURITYLEVEL       0x00C0
#define ZGP_NOTIFICATION_NO_SECURITY                                               0x0000
#define ZGP_NOTIFICATION_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC_ONLY                0x0040
#define ZGP_NOTIFICATION_ENCRYPTION_AND_FULL_4B_FRAME_COUNTER_AND_FULL_4B_MIC      0x00C0
#define ZGP_NOTIFICATION_SECURITYKEYTYPE     0x0700
#define ZGP_NOTIFICATION_NO_KEY                                0x0000
#define ZGP_NOTIFICATION_ZIGBEE_NWK_KEY                        0x0100
#define ZGP_NOTIFICATION_GPD_GROUP_KEY                         0x0200
#define ZGP_NOTIFICATION_NWK_KEY_DERIVED_GPD_GROUP_KEY         0x0300
#define ZGP_NOTIFICATION_INDIVIDUAL_OUT_OF_THE_BOX_GPD_KEY     0x0400
#define ZGP_NOTIFICATION_DERIVED_INDIVIDUAL_GPD_KEY            0x0700
#define ZGP_NOTIFICATION_RXAFTERTX           0x0800
#define ZGP_NOTIFICATION_GPTXQUEUEFULL       0x1000
#define ZGP_NOTIFICATION_BIDIRECTIONAL_CAPABILITY 0x2000
#define ZGP_NOTIFICATION_PROXY_INFO_PRESENT  0x4000

/* Commissioing States */

enum {
    ZCL_PROXYBASIC_CLI_OPERATIONAL = 0x0000,
    ZCL_PROXYBASIC_CLI_WAIT_FOR_CHANNEL_REQ0,
    ZCL_PROXYBASIC_CLI_WAIT_FOR_CHANNEL_REQ1,
    ZCL_PROXYBASIC_CLI_WAIT_FOR_COMMISSIONING0,
    ZCL_PROXYBASIC_CLI_WAIT_FOR_COMMISSIONING1,
    ZCL_PROXYBASIC_CLI_WAIT_FOR_SUCCESS,
    ZCL_PROXYBASIC_CLI_WAIT_PAIRING,
};

#endif /* __ZGP_PROXYBASIC_H */
