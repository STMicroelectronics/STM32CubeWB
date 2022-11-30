/*****************************************************************************
 * @file    zcl.custom_matrix.h
 * @author  MCD Application Team
 * @brief   This file contains the interface of the Custom Matrix cluster.
 *
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


#ifndef ZCL_CUSTOM_MATRIX_H
#define ZCL_CUSTOM_MATRIX_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the Custom Matrix cluster.
 *--------------------------------------------------------------------------
 */

/* Custom Cluster ID */
#define ZCL_CLUSTER_CUSTOM_MATRIX  0x80E1

/* Attribute Identifiers */
enum {
    ZCL_CUSTOM_MATRIX_ATTR = 0x0000,
     };

/* Commands */
#define ZCL_SET_CUSTOM_MATRIX_COMMAND_REQ          0x00
#define ZCL_CUSTOM_MATRIX_COMMAND_RSP              0x01

/* Request structures */
struct set_custom_matrix_command_req_t {
    uint8_t pattern_code;
};


/* Response Structures */
struct custom_matrix_command_rsp_t {
    uint16_t one;
};


enum ZclStatusCodeT
ZbZclSet_custom_matrix_ClientCommand(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst, uint8_t pattern_code,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Callbacks */
struct zcl_custom_matrix_server_callbacks_t {
    enum ZclStatusCodeT (*set_custom_matrix_command)(struct ZbZclClusterT *clusterPtr, \
        struct set_custom_matrix_command_req_t *cmd_req, struct ZbZclAddrInfoT *src_info, void *arg);
//    enum ZclStatusCodeT (*get_custom_ls_command)(struct ZbZclClusterT *clusterPtr, \
//        struct set_custom_ls_command_req_t *cmd_req, struct ZbZclAddrInfoT *src_info, void *arg);
};

/* Allocation Functions */
struct ZbZclClusterT * ZbZcl_custom_matrix_ClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZcl_custom_matrix_ServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, \
          struct zcl_custom_matrix_server_callbacks_t *callbacks, void *arg);

#endif /* ZCL_CUSTOM_MATRIX_H */
