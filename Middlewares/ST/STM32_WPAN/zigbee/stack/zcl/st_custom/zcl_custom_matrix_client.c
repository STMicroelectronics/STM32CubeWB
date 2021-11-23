/*****************************************************************************
 * @file    zcl_custom_matrix_client.c
 * @author  MCD Application Team
 * @brief   This file contains the implementation of the Custom Matrix
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
#include "zcl/st_custom/zcl.custom_matrix.h"

struct zcl_custom_matrix_client_cluster_t {
    struct ZbZclClusterT cluster;
};

/* Custom cluster long string allocation */
struct ZbZclClusterT *
ZbZcl_custom_matrix_ClientAlloc(struct ZigBeeT *zb, uint8_t endpoint)
{
    struct zcl_custom_matrix_client_cluster_t *clusterPtr;

    clusterPtr = ZbZclClusterAlloc(zb, sizeof(struct zcl_custom_matrix_client_cluster_t),\
              ZCL_CLUSTER_CUSTOM_MATRIX, endpoint, ZCL_DIRECTION_TO_CLIENT);
    if (clusterPtr == NULL) {
        return NULL;
    }
    
    /* This is the flags for Fragmentation */
    clusterPtr->cluster.txOptions |= (uint16_t)(ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY | ZB_APSDE_DATAREQ_TXOPTIONS_ACK | ZB_APSDE_DATAREQ_TXOPTIONS_FRAG);
    (void)ZbZclAttrIntegerWrite(&clusterPtr->cluster, ZCL_GLOBAL_ATTR_CLUSTER_REV, ZCL_CLUSTER_REVISION_ZCL6);

    ZbZclClusterAttach(&clusterPtr->cluster);
    return &clusterPtr->cluster;
}

/* Client command */
enum ZclStatusCodeT
ZbZclSet_custom_matrix_ClientCommand(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst, uint8_t pattern_code,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg)
{
    struct ZbZclClusterCommandReqT req;
    uint8_t payload[3];
    putle16(&payload[0], 1); // length
    payload[2]=pattern_code;
    
    (void)memset(&req, 0, sizeof(req));
    req.dst = *dst;
    req.cmdId = ZCL_SET_CUSTOM_MATRIX_COMMAND_REQ;
    req.noDefaultResp = ZCL_NO_DEFAULT_RESPONSE_TRUE;
    req.payload = payload;
    req.length = 3;//len + 2;
    ZbZclClusterCommandReq(cluster, &req, callback, arg);
    return ZCL_STATUS_SUCCESS;
}
