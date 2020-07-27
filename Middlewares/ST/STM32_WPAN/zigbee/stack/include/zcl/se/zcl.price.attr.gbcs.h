/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#include "zcl/se/zcl.price.h"

#ifdef ZCL_PRICE_ATTR_GBCS_CALLBACK
/* Optional attribute flags. If providing an attribute callback (), then need to
 * specify the appropriate flags: (ZCL_ATTR_FLAG_CB_READ | ZCL_ATTR_FLAG_CB_WRITE)
 */
# ifndef ZCL_PRICE_ATTR_GBCS_FLAGS
#  error "Must define ZCL_PRICE_ATTR_GBCS_FLAGS if defining ZCL_PRICE_ATTR_GBCS_CALLBACK"
# endif
#else
# define ZCL_PRICE_ATTR_GBCS_CALLBACK       NULL
# define ZCL_PRICE_ATTR_GBCS_FLAGS          ZCL_ATTR_FLAG_NONE
#endif

/* Use this as-is to append the GBCS Metering Attributes to the
 * Metering Server. Or use this as a template to create the
 * attribute's as your application requires. */

const struct ZbZclAttrT zcl_price_server_gbcs_attr_list[] = {
    /* CommodityType (Read-only) */
    {
        ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE, ZCL_DATATYPE_ENUMERATION_8BIT, ZCL_PRICE_ATTR_GBCS_FLAGS, 0,
        ZCL_PRICE_ATTR_GBCS_CALLBACK, {0, 0}, {0, 0}
    },
};

const unsigned int ZCL_PRICE_SVR_GBCS_ATTR_LIST_LEN = ZCL_ATTR_LIST_LEN(zcl_price_server_gbcs_attr_list);
