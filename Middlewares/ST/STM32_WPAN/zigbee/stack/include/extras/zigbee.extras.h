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

/*-----------------------------------------------------------------------------
 *  Blocking Functions (CONFIG_ZB_BLOCKING_CALLS)
 *-----------------------------------------------------------------------------
 */
void ZbNldeDataReqWait(struct ZigBeeT *zb, struct ZbNldeDataReqT *req, struct ZbNldeDataConfT *conf);
void ZbNldeFrameReqWait(struct ZigBeeT *zb, struct ZbNldeFrameReqT *req, ZbNldeFrameConfT *conf);
void ZbNlmeNetDiscWait(struct ZigBeeT *zb, struct ZbNlmeNetDiscReqT *req, struct ZbNlmeNetDiscConfT *conf);
void ZbNlmeNetFormWait(struct ZigBeeT *zb, struct ZbNlmeNetFormReqT *req, struct ZbNlmeNetFormConfT *conf);
void ZbNlmeJoinWait(struct ZigBeeT *zb, struct ZbNlmeJoinReqT *req, struct ZbNlmeJoinConfT *conf);
void ZbNlmeLeaveWait(struct ZigBeeT *zb, struct ZbNlmeLeaveReqT *leaveReqPtr, struct ZbNlmeLeaveConfT *leaveConfPtr);
void ZbNlmeRouteDiscWait(struct ZigBeeT *zb, struct ZbNlmeRouteDiscReqT *req, struct ZbNlmeRouteDiscConfT *conf);

void ZbZdoNwkAddrWait(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req, struct ZbZdoNwkAddrRspT *rsp);
void ZbZdoIeeeAddrWait(struct ZigBeeT *zb, struct ZbZdoIeeeAddrReqT *req, struct ZbZdoIeeeAddrRspT *rsp);
void ZbZdoNodeDescWait(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req, struct ZbZdoNodeDescRspT *rsp);
void ZbZdoPowerDescWait(struct ZigBeeT *zb, struct ZbZdoPowerDescReqT *req, struct ZbZdoPowerDescRspT *rsp);
void ZbZdoSimpleDescWait(struct ZigBeeT *zb, struct ZbZdoSimpleDescReqT *req, struct ZbZdoSimpleDescRspT *rsp);
void ZbZdoActiveEpWait(struct ZigBeeT *zb, struct ZbZdoActiveEpReqT *req, struct ZbZdoActiveEpRspT *rsp);
void ZbZdoMatchDescWait(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req, struct ZbZdoMatchDescRspT *rsp);
void ZbZdoBindWait(struct ZigBeeT *zb, struct ZbZdoBindReqT *req, struct ZbZdoBindRspT *rsp);
void ZbZdoUnbindWait(struct ZigBeeT *zb, struct ZbZdoBindReqT *req, struct ZbZdoBindRspT *rsp);
void ZbZdoLqiWait(struct ZigBeeT *zb, struct ZbZdoLqiReqT *req, struct ZbZdoLqiRspT *rsp);
void ZbZdoRtgWait(struct ZigBeeT *zb, struct ZbZdoRtgReqT *req, struct ZbZdoRtgRspT *rsp);
void ZbZdoMgmtBindWait(struct ZigBeeT *zb, struct ZbZdoMgmtBindReqT *req, struct ZbZdoMgmtBindRspT *rsp);
void ZbZdoPermitJoinWait(struct ZigBeeT *zb, struct ZbZdoPermitJoinReqT *req, struct ZbZdoPermitJoinRspT *rsp);
void ZbZdoNwkUpdateWait(struct ZigBeeT *zb, struct ZbZdoNwkUpdateReqT *req, struct ZbZdoNwkUpdateNotifyT *rsp);
void ZbZdoLeaveWait(struct ZigBeeT *zb, struct ZbZdoLeaveReqT *req, struct ZbZdoLeaveRspT *rsp);

#endif
