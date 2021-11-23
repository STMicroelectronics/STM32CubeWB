/* Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved. */

#include "zcl/se/zcl.price.h"

#ifndef ZCL_PRICE_ATTR_GBCS_LIST_NAME
#define ZCL_PRICE_ATTR_GBCS_LIST_NAME               zcl_price_server_gbcs_attr_list
#endif

#ifdef ZCL_PRICE_ATTR_GBCS_CALLBACK
#ifndef ZCL_PRICE_ATTR_GBCS_FLAGS
/* If callback is not NULL, Should define one or both of ZCL_ATTR_FLAG_CB_READ | ZCL_ATTR_FLAG_CB_WRITE */
#error "If ZCL_PRICE_ATTR_GBCS_CALLBACK defined, must also define ZCL_PRICE_ATTR_GBCS_FLAGS. E.g. ZCL_ATTR_FLAG_CB_READ | ZCL_ATTR_FLAG_CB_WRITE"
#endif
#else
#define ZCL_PRICE_ATTR_GBCS_CALLBACK                NULL
#endif

/* By default, disable reporting */
#ifndef ZCL_PRICE_ATTR_GBCS_FLAGS
#define ZCL_PRICE_ATTR_GBCS_FLAGS                   ZCL_ATTR_FLAG_NONE
#define ZCL_PRICE_ATTR_GBCS_REPORT_INTVL_MIN        0x0000U
#define ZCL_PRICE_ATTR_GBCS_REPORT_INTVL_MAX        0xffffU
#endif

/* Use this as-is to append the GBCS Metering Attributes to the
 * Metering Server. Or use this as a template to create the
 * attribute's as your application requires. */

/* NOTE: Use ZCL_ATTR_LIST_LEN(list) to get the length of the list (number of attributes) */

const struct ZbZclAttrT ZCL_PRICE_ATTR_GBCS_LIST_NAME[] = {
    /* CommodityType (Read-only) */
    {
        ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE, ZCL_DATATYPE_ENUMERATION_8BIT, ZCL_PRICE_ATTR_GBCS_FLAGS, 0,
        ZCL_PRICE_ATTR_GBCS_CALLBACK, {0, 0},
        {ZCL_PRICE_ATTR_GBCS_REPORT_INTVL_MIN, ZCL_PRICE_ATTR_GBCS_REPORT_INTVL_MAX}
    },
};
