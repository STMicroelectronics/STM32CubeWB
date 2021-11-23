/*****************************************************************************
 * @file    zcl_custom_ls_client.c
 * @author  MCD Application Team
 * @brief   This file contains the implementation of the Custom Long String
 *          cluster (client part).
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <string.h>

#include "zigbee.h"
#include "zcl/zcl.h"
#include "zcl/st_custom/zcl.custom_ls.h"

struct zcl_custom_ls_client_cluster_t {
    struct ZbZclClusterT cluster;
};

/* Custom cluster long string allocation */
struct ZbZclClusterT *
ZbZcl_custom_ls_ClientAlloc(struct ZigBeeT *zb, uint8_t endpoint)
{
    struct zcl_custom_ls_client_cluster_t *clusterPtr;

    clusterPtr = ZbZclClusterAlloc(zb, sizeof(struct zcl_custom_ls_client_cluster_t),
              ZCL_CLUSTER_CUSTOM_LS, endpoint, ZCL_DIRECTION_TO_CLIENT);
    if (clusterPtr == NULL) {
        return NULL;
    }

    /* This is the flags for Fragmentation */
    clusterPtr->cluster.txOptions |= (uint16_t)(ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY | ZB_APSDE_DATAREQ_TXOPTIONS_ACK | ZB_APSDE_DATAREQ_TXOPTIONS_FRAG);
    //ZbZclClusterSetTxOptions(&clusterPtr->cluster, ZB_APSDE_DATAREQ_TXOPTIONS_FRAG);
    (void)ZbZclAttrIntegerWrite(&clusterPtr->cluster, ZCL_GLOBAL_ATTR_CLUSTER_REV, 1);

    ZbZclClusterAttach(&clusterPtr->cluster);
    return &clusterPtr->cluster;
}

/* Client command */
enum ZclStatusCodeT
ZbZclSet_custom_ls_ClientCommand(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst, char *string,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg)
{
    struct ZbZclClusterCommandReqT req;
    uint8_t payload[2050];
    uint16_t len;

    len = (uint16_t)strlen(string);
    if (len > 2048) {
      return ZCL_STATUS_INVALID_VALUE;
    }

    putle16(&payload[0], len);
    memcpy(&payload[2], string, len);

    (void)memset(&req, 0, sizeof(req));
    req.dst = *dst;
    req.cmdId = ZCL_SET_CUSTOM_LS_COMMAND_REQ;
    req.noDefaultResp = /*ZCL_NO_DEFAULT_RESPONSE_FALSE*/ZCL_NO_DEFAULT_RESPONSE_TRUE;
    req.payload = payload;
    req.length = len + 2;
    ZbZclClusterCommandReq(cluster, &req, callback, arg);
    return ZCL_STATUS_SUCCESS;
}
