/* Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_PAYLOAD_H
# define ZCL_PAYLOAD_H

#include "zcl/zcl.h"

/*lint -save -e9021 [ useof 'undef' is discouraged <Rule 20.5, ADVISORY> ] */
/*lint -save -e9016 [ pointer  arithmetic <Rule 18.4, ADVISORY> ] */

/*---------------------------------------------------------------
 * ZCL Payload Helpers
 *---------------------------------------------------------------
 */
#define ZCL_BUILDER_CHECK_LEN(size)                 \
    do {                                            \
        if (payload == NULL) {                      \
            return -1;                              \
        }                                           \
        if (index == NULL) {                        \
            return -1;                              \
        }                                           \
        if ((*index + (size)) > capacity) {         \
            return -1;                              \
        }                                           \
    } while (false)

#define ZCL_BUILDER_TEMPLATE(size, append_func)     \
    do {                                            \
        ZCL_BUILDER_CHECK_LEN(size);                \
        append_func;                                \
        *index += (size);                           \
        return (int)*index;                         \
    } while (false)

/*
 * Helper Functions for constructing a ZCL payload
 */
static inline int
zb_zcl_append_uint8(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint8_t value)
{
    ZCL_BUILDER_TEMPLATE(1U, payload[*index] = value);
}

static inline int
zb_zcl_append_uint16(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint16_t value)
{
    ZCL_BUILDER_TEMPLATE(2U, putle16(payload + *index, value));
}

static inline int
zb_zcl_append_uint24(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint32_t value)
{
    ZCL_BUILDER_TEMPLATE(3U, putle24(payload + *index, value));
}

static inline int
zb_zcl_append_uint32(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint32_t value)
{
    ZCL_BUILDER_TEMPLATE(4U, putle32(payload + *index, value));
}

static inline int
zb_zcl_append_uint40(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint64_t value)
{
    ZCL_BUILDER_TEMPLATE(5U, putle40(payload + *index, value));
}

static inline int
zb_zcl_append_uint48(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint64_t value)
{
    ZCL_BUILDER_TEMPLATE(6U, putle48(payload + *index, value));
}

static inline int
zb_zcl_append_uint56(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint64_t value)
{
    ZCL_BUILDER_TEMPLATE(7U, putle56(payload + *index, value));
}

static inline int
zb_zcl_append_uint64(uint8_t *payload, const unsigned int capacity, unsigned int *index, uint64_t value)
{
    ZCL_BUILDER_TEMPLATE(8U, putle64(payload + *index, value));
}

static inline int
zb_zcl_append_uint8_array(uint8_t *payload, const unsigned int capacity, unsigned int *index,
    const uint8_t *value, const unsigned int value_length)
{
    unsigned i;

    ZCL_BUILDER_CHECK_LEN(value_length);
    /* ZbMemCpy is not portable (internal stack use only), so do a manual copy here. */
    for (i = 0; i < value_length; i++) {
        payload[*index + i] = value[i];
    }
    *index += value_length;
    return (int)*index;
}

/* prevent accidental reuse */
#undef ZCL_BUILDER_TEMPLATE

#define ZCL_PARSER_TEMPLATE(size, extract_func)     \
    do {                                            \
        if (payload == NULL) {                      \
            return -1;                              \
        }                                           \
        if (index == NULL) {                        \
            return -1;                              \
        }                                           \
        if ((*index + (size)) > length) {           \
            return -1;                              \
        }                                           \
        *value = extract_func;                      \
        *index += (size);                           \
        return (int)*index;                         \
    } while (false)

/*
 * Helper Functions for processing a ZCL payload
 */
static inline int
zb_zcl_parse_uint8(const uint8_t *payload, const unsigned int length, unsigned int *index, uint8_t *value)
{
    ZCL_PARSER_TEMPLATE(1U, payload[*index]);
}

static inline int
zb_zcl_parse_uint16(const uint8_t *payload, const unsigned int length, unsigned int *index, uint16_t *value)
{
    ZCL_PARSER_TEMPLATE(2U, pletoh16(payload + *index));
}

static inline int
zb_zcl_parse_uint24(const uint8_t *payload, const unsigned int length, unsigned int *index, uint32_t *value)
{
    ZCL_PARSER_TEMPLATE(3U, pletoh24(payload + *index));
}

static inline int
zb_zcl_parse_uint32(const uint8_t *payload, const unsigned int length, unsigned int *index, uint32_t *value)
{
    ZCL_PARSER_TEMPLATE(4U, pletoh32(payload + *index));
}

static inline int
zb_zcl_parse_uint40(const uint8_t *payload, const unsigned int length, unsigned int *index, uint64_t *value)
{
    ZCL_PARSER_TEMPLATE(5U, pletoh40(payload + *index));
}

static inline int
zb_zcl_parse_uint48(const uint8_t *payload, const unsigned int length, unsigned int *index, uint64_t *value)
{
    ZCL_PARSER_TEMPLATE(6U, pletoh48(payload + *index));
}

static inline int
zb_zcl_parse_uint56(const uint8_t *payload, const unsigned int length, unsigned int *index, uint64_t *value)
{
    ZCL_PARSER_TEMPLATE(7U, pletoh56(payload + *index));
}

static inline int
zb_zcl_parse_uint64(const uint8_t *payload, const unsigned int length, unsigned int *index, uint64_t *value)
{
    ZCL_PARSER_TEMPLATE(8U, pletoh64(payload + *index));
}

/* prevent accidental reuse */
#undef ZCL_PARSER_TEMPLATE

/*lint -restore */
/*lint -restore */
#endif /* __ZCL_PAYLOAD_H */
