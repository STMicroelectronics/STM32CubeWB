/*****************************************************************************
 * @file    zcl_custom_matrix_server.c
 * @author  MCD Application Team
 * @brief   This file contains the implementation of the Custom Matrix
 *          cluster (server part).
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


#include "zigbee.h"
#include "zcl/zcl.h"
#include "zcl/st_custom/zcl.custom_matrix.h"

static enum ZclStatusCodeT
custom_matrix_command_handler(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr,\
    struct ZbApsdeDataIndT *dataIndPtr);

struct zcl_custom_matrix_server_cluster_t {
    struct ZbZclClusterT cluster;
    struct zcl_custom_matrix_server_callbacks_t callbacks;
};

struct ZbZclClusterT *
ZbZcl_custom_matrix_ServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,\
      struct zcl_custom_matrix_server_callbacks_t *callbacks, void *arg)
{
    struct zcl_custom_matrix_server_cluster_t *clusterPtr;

    clusterPtr = ZbZclClusterAlloc(zb, sizeof(struct zcl_custom_matrix_server_cluster_t),\
            ZCL_CLUSTER_CUSTOM_MATRIX, endpoint, ZCL_DIRECTION_TO_SERVER);
    if (clusterPtr == NULL) {
        return NULL;
    }

    clusterPtr->cluster.txOptions |= ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY;

    memset(&clusterPtr->callbacks, 0, sizeof(clusterPtr->callbacks));
    if (callbacks != NULL) {
        memcpy(&clusterPtr->callbacks, callbacks, sizeof(clusterPtr->callbacks));
    }

    clusterPtr->cluster.command = custom_matrix_command_handler;

    /* not used in our case */


 //   (void)ZbZclAttrIntegerWrite(&clusterPtr->cluster, ZCL_GLOBAL_ATTR_CLUSTER_REV, ZCL_CLUSTER_REVISION_ZCL6);

    ZbZclClusterSetCallbackArg(&clusterPtr->cluster, arg);

    ZbZclClusterAttach(&clusterPtr->cluster);
    return &clusterPtr->cluster;
}

static enum ZclStatusCodeT
custom_matrix_command_handler(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr,\
    struct ZbApsdeDataIndT *dataIndPtr)
{
    struct zcl_custom_matrix_server_cluster_t *custom_matrix_cluster = (struct zcl_custom_matrix_server_cluster_t *)clusterPtr;
    struct ZbZclAddrInfoT src_info;
    uint8_t cmd_id = zclHdrPtr->cmdId;
    enum ZclStatusCodeT return_status = ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;

    (void)memset(&src_info, 0, sizeof(src_info));
    src_info.addr = dataIndPtr->src;
    src_info.seqnum = zclHdrPtr->seqNum;
    //src_info.tx_options = dataIndPtr->securityStatus.;

    switch (cmd_id) {
        case ZCL_SET_CUSTOM_MATRIX_COMMAND_REQ:
        {
            struct set_custom_matrix_command_req_t req;
            memset(&req, 0, sizeof(req));

            if (custom_matrix_cluster->callbacks.set_custom_matrix_command == NULL) {
                return_status = ZCL_STATUS_UNSUPP_COMMAND;
                break;
            }
            req.pattern_code = dataIndPtr->asdu[2];
            //len = pletoh16(&dataIndPtr->asdu[0]);
            //memcpy(string, &dataIndPtr->asdu[2], len);
            //string[len] = '\0';
            //req.string = string;

            return_status = custom_matrix_cluster->callbacks.set_custom_matrix_command(clusterPtr, &req, &src_info, clusterPtr->app_cb_arg);
            break;
        }
        default:
            return_status = ZCL_STATUS_UNSUPP_COMMAND;
            break;
    }
    return return_status;
}

/* not used in our custom cluster */


enum ZclStatusCodeT
ZbZcl_custom_matrix_ServerSendCommandRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct custom_matrix_command_rsp_t *rsp)
{

    uint8_t rsp_payload[2];
    unsigned int length = 0U;
    struct ZbApsBufT bufv[1];
    dst_info->tx_options = ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY;
    /* Form the payload */
    putle16(&rsp_payload[length], rsp->one);
    length += 2U;

    bufv[0].data = rsp_payload;
    bufv[0].len = length;

    return ZbZclClusterCommandRsp(clusterPtr, dst_info, ZCL_CUSTOM_MATRIX_COMMAND_RSP, bufv, 1U);


}
