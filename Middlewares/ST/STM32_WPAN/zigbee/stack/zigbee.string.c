/* Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved. */

#include "zigbee.h"

static bool
char_is_space(uint8_t c)
{
    bool rc;

    switch (c) {
        case 0x09: /* '\t' */
        case 0x0a: /* '\n' */
        case 0x0b: /* '\v' */
        case 0x0c: /* '\f' */
        case 0x0d: /* '\r' */
        case 0x20: /* ' ' */
            rc = true;
            break;

        default:
            rc = false;
            break;
    }
    return (rc);
}

int
zb_hex_str_to_bin(const char *string, void *out, unsigned int maxlen)
{
    unsigned int i = 0;
    const uint8_t *str = (const uint8_t *)string;
    char c;

    if (str == NULL) {
        return 0;
    }

    while (i < maxlen) {
        if (*str == 0U) {
            return (int)i;
        }

        /* Ignore separators and whitespace. */
        c = (char)*str;
        if (char_is_space(*str) || (c == ':') || (c == '-') || (c == ',')) {
            str++;
            continue;
        }

        /* Convert first nibble. */
        if ((c >= 'a') && (c <= 'f')) {
            ((uint8_t *)out)[i] = (*str - (uint8_t)'a' + 10U) << 4;
        }
        else if ((c >= 'A') && (c <= 'F')) {
            ((uint8_t *)out)[i] = (*str - (uint8_t)'A' + 10U) << 4;
        }
        else if ((c >= '0') && (c <= '9')) {
            ((uint8_t *)out)[i] = ((uint8_t)(*str - (uint8_t)'0')) << 4;
        }
        else { /* Invalid nibble */
            return -1;
        }

        str++;

        if (*str == 0U) {
            return (int)i;
        }

        /* Ignore separators and whitespace. */
        c = (char)*str;
        if (char_is_space(*str) || (c == ':') || (c == '-') || (c == ',')) {
            str++;
            continue;
        }

        /* Convert the second nibble. */
        if ((c >= 'a') && (c <= 'f')) {
            ((uint8_t *)out)[i] |= (*str - (uint8_t)'a' + 10U);
        }
        else if ((c >= 'A') && (c <= 'F')) {
            ((uint8_t *)out)[i] |= (*str - (uint8_t)'A' + 10U);
        }
        else if ((c >= '0') && (c <= '9')) {
            ((uint8_t *)out)[i] |= (*str - (uint8_t)'0');
        }
        else { /* Invalid nibble */
            return -1;
        }
        str++;

        i++;
    } /* while */

    /* Done */
    return (int)i;
}

unsigned int
zb_hex_bin_to_str(const uint8_t *in_data, unsigned int in_len, char *out_str, unsigned int max_len,
    const char delimiter, unsigned int interval)
{
    unsigned int i, j;
    const char *hex = "0123456789abcdef";

    if (max_len == 0U) {
        return 0;
    }
    j = 0;
    for (i = 0; i < in_len; i++) {
        if ((interval != 0U) && (i != 0U) && ((i % interval) == 0U)) {
            if ((j + 2U) > max_len) {
                out_str[0] = (char)0;
                return 0;
            }
            out_str[j++] = delimiter;
        }
        if ((j + 3U) > max_len) {
            out_str[0] = (char)0;
            return 0;
        }
        out_str[j++] = hex[(in_data[i] >> 4) & 0x0fU];
        out_str[j++] = hex[in_data[i] & 0x0fU];
    }
    out_str[j] = (char)0;
    return j;
}
