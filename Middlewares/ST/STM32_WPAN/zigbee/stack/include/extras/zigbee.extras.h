/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZIGBEE_EXTRAS_H
#define ZIGBEE_EXTRAS_H

#include "zigbee.h"

const char * ZbStatusToStr(enum ZbStatusCodeT statusCode);
const char * ZbBdbCommissStatusToStr(enum ZbBdbCommissioningStatusT status);
const char * ZbApsDeviceStatusToStr(enum ZbApsmeDeviceStatusT status);

/* NWK functions required for zigbee testing */
void ZbNwkAddrSetNextChildAddr(struct ZigBeeT *zb, uint16_t nextChildAddr);
bool ZbNwkTestSendUnknownCommand(struct ZigBeeT *zb, uint16_t nwkDstAddr,
    uint8_t nwkProtoVer);

/* APS functions required for zigbee testing */
void ZbApsmeRemoveDeviceReq_NoApsSec(struct ZigBeeT *zb, struct ZbApsmeRemoveDeviceReqT *req);

/* ZDO functions required for zigbee testing */
void ZbZdoNwkAddrReq_NoSec(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req);
void ZbZdoNwkAddrReq_InterPan(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req, uint16_t panid);
void ZbZdoNodeDescReq_NoSec(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req);
void ZbZdoNodeDescReq_nwk_ext_bit(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req, uint64_t dstExtAddr);
void ZbZdoDiscoveryCacheReq(struct ZigBeeT *zb, uint16_t dstAddr);
void ZbZdoUnsolicitedEnhUpdateNotify(struct ZigBeeT *zb, uint16_t dstAddr);

/* ZDO string conversion helper functions */
const char * ZbZdoNodeTypeToStr(const uint8_t type);
void ZbZdoNodeBandsStr(const uint8_t band, char *buf, const unsigned int max_len);

#endif
