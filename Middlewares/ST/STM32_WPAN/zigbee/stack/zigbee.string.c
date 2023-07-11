/* Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved. */

#include "zigbee.h"

/*FUNCTION:-------------------------------------------------------------------
 *  NAME
 *      char_is_space
 *  DESCRIPTION
 *      Checks if character is one of the standard white-space characters:
 *
 *      ' '     (0x20)  space (SPC)
 *      '\t'    (0x09)  horizontal tab (TAB)
 *      '\n'    (0x0a)  newline (LF)
 *      '\v'    (0x0b)  vertical tab (VT)
 *      '\f'    (0x0c)  feed (FF)
 *      '\r'    (0x0d)  carriage return (CR)
 *
 *  PARAMETERS
 *      none
 *  RETURNS
 *      none
 *----------------------------------------------------------------------------
 */
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

/*FUNCTION:------------------------------------------------------
 *  NAME
 *      zb_hex_str_to_bin
 *  DESCRIPTION
 *      A helper for parsing hex strings to binary blobs.
 *
 *      Parses a hexadecimal string into memory. Will skip over
 *      separator characters. Will stop when maxlen bytes
 *      have been written, or a NULL character is encountered.
 *  PARAMETERS
 *      str             ; Input string.
 *      out             ; Output pointer.
 *      maxlen          ; Size of memory that can be written to.
 *  RETURNS
 *      int             ; Number of bytes written.
 *---------------------------------------------------------------
 */
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

/*FUNCTION:------------------------------------------------------
 *  NAME
 *      zb_hex_bin_to_str
 *  DESCRIPTION
 *      Convert an array of bytes to a null terminated string of
 *      hex. For an array of length bytes, without delimiters the
 *      buffer must be at least 2 * length + 1 . With delimiters
 *      add the number of delimiters.
 *  PARAMETERS
 *      output     buffer to store string, WARNING must be at
 *                 least: 2 * length + 1 + #delimiters in length!
 *      data       data to convert
 *      length     number of bytes of data to convert
 *      delimiter  character to use as a delimiter
 *      interval   space between delimiters or zero for none
 *  RETURNS
 *      output string length
 *---------------------------------------------------------------
 */
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
