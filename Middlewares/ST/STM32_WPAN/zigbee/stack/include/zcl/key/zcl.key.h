/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      The public header file for the Key Establishment
 *      cluster of the ZCL.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_KEY_H
# define ZCL_KEY_H

#include "zcl/zcl.h"

/* The slack time is added to the remote side's advertised timeout values.
 * It accounts for network latency, etc. */
#define ZCL_KEY_TIMEOUT_SLACK_SEC                   10U

/* Attirbute identifiers. */
#define ZCL_KEY_ATTR_SUITE                          0x0000U

/* Command identifiers. */
enum {
    ZCL_KEY_COMMAND_INITIATE_KE = 0x00,
    ZCL_KEY_COMMAND_EPHEMERAL_DATA = 0x01,
    ZCL_KEY_COMMAND_CONFIRM_KEY = 0x02,
    ZCL_KEY_COMMAND_TERMINATE_KE = 0x03
};

/* Key Establishment suite bits */
#define ZCL_KEY_SUITE_CBKE_NONE                     0x0000U
#define ZCL_KEY_SUITE_CBKE_ECMQV                    0x0001U
#define ZCL_KEY_SUITE_CBKE2_ECMQV                   0x0002U
/* Exegin Custom for testing - SKKE over ZCL */
#define ZCL_KEY_SUITE_SKKE                          0x8000U

/* Key Establishment status codes. */
enum ZbZclKeyStatusT {
    ZCL_KEY_STATUS_SUCCESS = 0x00,
    ZCL_KEY_STATUS_UNKNOWN_ISSUER = 0x01,
    ZCL_KEY_STATUS_BAD_KEY_CONFIRM = 0x02,
    ZCL_KEY_STATUS_BAD_MESSAGE = 0x03,
    ZCL_KEY_STATUS_NO_RESOURCES = 0x04,
    ZCL_KEY_STATUS_UNSUPPORTED_SUITE = 0x05,
    ZCL_KEY_STATUS_INVALID_CERTIFICATE = 0x06, /* SE 1.2 */
    ZCL_KEY_STATUS_TIMEOUT = 0x07, /* Exegin extension */
    ZCL_KEY_STATUS_MATCH_DESC_FAILED = 0x08 /* Exegin extension */
};

/* Callback functions types */
typedef void (*ZbZclKeConfirmFuncT)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT status, void *arg);

struct ZbZclKeSuiteT;

/* Functions */
struct ZbZclClusterT * ZbZclKeAlloc(struct ZigBeeT *zb, uint8_t endpoint, ZbZclKeConfirmFuncT responderConfirmCb, void *responderConfirmArg);
enum ZclStatusCodeT ZbZclKeAddSuite(struct ZbZclClusterT *clusterPtr, struct ZbZclKeSuiteT *newSuitePtr);
void ZbZclKeDeleteAllSuites(struct ZbZclClusterT *clusterPtr);
uint16_t ZbZclKeSuiteGetSupported(struct ZbZclClusterT *clusterPtr);
enum ZclStatusCodeT ZbZclKeNegotiate(struct ZigBeeT *zb, uint64_t partner, uint8_t endpoint, uint16_t suite, ZbZclKeConfirmFuncT confirmFunc, void *confirmArg);
const char * ZbZclKeStatusToStr(uint8_t statusCode);

/* ZbZclKeWithDevice peforms all the steps required for CBKE between devices, including a Trust Center.
 * Note, CBKE with a Trust Center is already handled by the ZbStartup code, including Trust Cetner Swap Out (TCSO)
 *
 * If aps_req_key is true, and partnerAddr is not the TC, then an APS Request Key is sent to the TC with the partner
 * address set to the partnerAddr paramter.
 *
 * The callback is called once KE is done, or there was an error, only if ZbZclKeWithDevice returns ZCL_STATUS_SUCCESS. */
enum ZclStatusCodeT ZbZclKeWithDevice(struct ZigBeeT *zb, uint64_t partnerAddr, bool aps_req_key,
    void (*callback)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg), void *arg);

#endif /* ZCL_KEY_H */
