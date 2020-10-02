/**
 * file zcl.touchlink.h
 * brief Deprecated Touchlink API - Excluded from DOxygen documentation
 * author Exegin Technologies Limited
 * copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 *
 * The functions in this header file are deprecated
 */

#ifndef ZCL_TOUCHLINK_H
#define ZCL_TOUCHLINK_H

/* PICS.ZCL.Touchlink
 * TC.S | True
 * TC.C | True
 *
 * Server Attributes
 * TC.S.Afffd | True
 *
 * Commands Received
 * TC.S.C00.Rsp | True
 * TC.S.C02.Rsp | True
 * TC.S.C06.Rsp | True
 * TC.S.C07.Rsp | True
 * TC.S.C10.Rsp | True
 * TC.S.C12.Rsp | True
 * TC.S.C14.Rsp | True
 * TC.S.C16.Rsp | True
 * TC.S.C41.Rsp | False
 * TC.S.C42.Rsp | False
 *
 * Commands Generated
 * TC.S.C01.Tx | True
 * TC.S.C03.Tx | True
 * TC.S.C11.Tx | True
 * TC.S.C13.Tx | True
 * TC.S.C15.Tx | True
 * TC.S.C40.Tx | False
 * TC.S.C41.Tx | False
 * TC.S.C42.Tx | False
 *
 *
 * Client Attributes
 * TC.C.Afffd | True
 *
 * Commands Received
 * TC.C.C01.Rsp | True
 * TC.C.C03.Rsp | True
 * TC.C.C11.Rsp | True
 * TC.C.C13.Rsp | True
 * TC.C.C15.Rsp | True
 * TC.C.C40.Rsp | False
 * TC.C.C41.Rsp | False
 * TC.C.C42.Rsp | False
 *
 * Commands Generated
 * TC.C.C00.Tx | True
 * TC.C.C02.Tx | True
 * TC.C.C06.Tx | True
 * TC.C.C07.Tx | True
 * TC.C.C10.Tx | True
 * TC.C.C12.Tx | True
 * TC.C.C14.Tx | True
 * TC.C.C16.Tx | True
 * TC.C.C41.Tx | False
 * TC.C.C42.Tx | False
 */

#include "zcl/zcl.h"

/* include TL utility cluster support functions. */
/* EXEGIN - disabled while adding Touchlink into stack */
/* #define TL_UTILITY */

/* Zigbee Information Field
 * See Figure 13-10. Format of the ZigBee Information Field */
/* Logical Type */
#define ZCL_TL_ZBINFO_TYPE_MASK                     0x03U
#define ZCL_TL_ZBINFO_TYPE_COORD                    0x00U
#define ZCL_TL_ZBINFO_TYPE_ROUTER                   0x01U
#define ZCL_TL_ZBINFO_TYPE_END_DEVICE               0x02U
/* Rx on when idle */
#define ZCL_TL_ZBINFO_RX_ON_IDLE                    0x04U
/* Reserved */
#define ZCL_TL_ZBINFO_RESERVED                      0xf8U

/* Internal use flags (don't get sent over-the-air)
 * EXEGIN - These used to be mixed with the ZCL_TL_ZBINFO_ flags, so for backward
 * compatibility make sure they don't conflict. */
#define ZCL_TL_FLAGS_IS_TARGET                      0x10U
#define ZCL_TL_FLAGS_USE_PERSIST                    0x20U
#define ZCL_TL_FLAGS_FACTORY_RESET                  0x40U

#endif
