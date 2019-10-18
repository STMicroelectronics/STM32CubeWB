/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Touchlink cluster.
 *--------------------------------------------------------------------------
 */
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

#ifndef ZCL_TOUCHLINK_H
# define ZCL_TOUCHLINK_H

#include "zcl/zcl.h"

/* include TL utility cluster support functions. */
/* FIXME - disabled while adding Touchlink into stack */
/* #define TL_UTILITY */

/* Zigbee Information Field
 * See Figure 13-10. Format of the ZigBee Information Field */
/* Logical Type */
#define ZCL_TL_ZBINFO_TYPE_MASK                     0x03
#define ZCL_TL_ZBINFO_TYPE_COORD                    0x00
#define ZCL_TL_ZBINFO_TYPE_ROUTER                   0x01
#define ZCL_TL_ZBINFO_TYPE_END_DEVICE               0x02
/* Rx on when idle */
#define ZCL_TL_ZBINFO_RX_ON_IDLE                    0x04
/* Reserved */
#define ZCL_TL_ZBINFO_RESERVED                      0xf8

/* Internal use flags (don't get sent over-the-air)
 * FIXME - These used to be mixed with the ZCL_TL_ZBINFO_ flags, so for backward
 * compatibility make sure they don't conflict. */
#define ZCL_TL_FLAGS_IS_TARGET                      (uint8_t)0x10
#define ZCL_TL_FLAGS_USE_PERSIST                    (uint8_t)0x20
#define ZCL_TL_FLAGS_FACTORY_RESET                  (uint8_t)0x40

/*--------------------------------------------------------------------------
 * Warning! Deprecated API. Use ZbStartup with BDB_COMMISSION_MODE_TOUCHLINK instead.
 *--------------------------------------------------------------------------
 */
/* Deprecated! Touchlink should be started by setting the BDB_COMMISSION_MODE_TOUCHLINK
 * commissioning mode bit in the ZbStartupT configuration struct, filling in the
 * touchlink configuration in ZbStartupT, and calling ZbStartup. */
uint8_t ZbZclTouchlinkStart(struct ZbZclClusterT *clusterPtr,
    struct WpanPublicT *wpan, uint8_t flags,
    const void *persist_buf, unsigned int persist_len) ZB_F_DEPRECATED;

/* Deprecated! Use ZbStartup instead. */
uint8_t ZbZclTouchlinkScan(struct ZbZclClusterT *clusterPtr,
    void (*callback)(enum ZbBdbCommissioningStatusT status, void *arg), void *arg) ZB_F_DEPRECATED;

/* Deprecated! Use ZbStartup instead, with touchlink ZCL_TL_FLAGS_FACTORY_RESET flag bit set. */
uint8_t ZbZclTouchlinkFactoryReset(struct ZbZclClusterT *clusterPtr,
    void (*callback)(enum ZbBdbCommissioningStatusT status, void *arg), void *arg) ZB_F_DEPRECATED;

/* Deprecated! Use ZbStartupTouchlinkTargetStop instead. */
uint8_t ZbZclTouchlinkStop(struct ZbZclClusterT *clusterPtr) ZB_F_DEPRECATED;

/* Deprecated! ZbStartup will allocate the Touchlink Server cluster if necessary. */
struct ZbZclClusterT * ZbZclTouchlinkServerAlloc(struct ZigBeeT *zb, uint8_t endpoint) ZB_F_DEPRECATED;

/* Deprecated! ZbStartup will allocate the Identify Server cluster if not already
 * allocated on the endpoint chosen for Touchlink. */
void ZbZclTouchlinkServerConfigIdentify(struct ZbZclClusterT *clusterPtr,
    struct ZbZclClusterT *identifyCluster) ZB_F_DEPRECATED;

/* Deprecated! ZbStartup will allocate the Touchlink Client cluster if necessary. */
struct ZbZclClusterT * ZbZclTouchlinkClientAlloc(struct ZigBeeT *zb, uint8_t endpoint) ZB_F_DEPRECATED;

#endif /* __ZCL_ONOFF_H */
