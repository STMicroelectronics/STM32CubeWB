/* Copyright [2009 - 2017] Exegin Technologies Limited. All rights reserved. */

#include "pletoh.h"

/*FUNCTION:-------------------------------------------------------------------
 *  NAME
 *      is_little_endian
 *  DESCRIPTION
 *      none
 *  PARAMETERS
 *      none
 *  RETURNS
 *      true if system is little-endian, false if big-endian
 *----------------------------------------------------------------------------
 */
bool
is_little_endian(void)
{
#if defined(_MSC_VER)
    /* Assume x86 Intel */
    return true;
#else
    short int number = 0x1;
    char *numPtr = (char *)&number;
    return (numPtr[0] == 1);
#endif
} /* is_little_endian */

/*----------------------------------------------------------------------------
 * Read Little Endian
 *----------------------------------------------------------------------------
 */
/* 2-byte */
uint16_t
pletoh16(const uint8_t *p)
{
    return p[0] | (p[1] << 8);
} /* pletoh16 */

/* 3-byte */
uint32_t
pletoh24(const uint8_t *p)
{
    return (uint32_t)pletoh16(p) | (uint32_t)(p[2]) << 16;
} /* pletoh24 */

/* 4-byte */
uint32_t
pletoh32(const uint8_t *p)
{
    return (uint32_t)pletoh16(p) | (uint32_t)pletoh16(p + 2) << 16;
} /* pletoh32 */

/* 5-byte */
uint64_t
pletoh40(const uint8_t *p)
{
    return (uint64_t)pletoh32(p) | (uint64_t)(p[4]) << 32;
} /* pletoh40 */

/* 6-byte */
uint64_t
pletoh48(const uint8_t *p)
{
    return pletoh40(p) | (uint64_t)(p[5]) << 40;
} /* pletoh48 */

/* 7-byte */
uint64_t
pletoh56(const uint8_t *p)
{
    return pletoh48(p) | (uint64_t)(p[6]) << 48;
} /* pletoh56 */

/* 8-byte */
uint64_t
pletoh64(const uint8_t *p)
{
    return pletoh32(p) | (uint64_t)pletoh32(p + 4) << 32;
} /* pletoh64 */

/*----------------------------------------------------------------------------
 * Write Little Endian
 *----------------------------------------------------------------------------
 */
/* 2-byte */
void
putle16(uint8_t *p, uint16_t value)
{
    p[0] = value >> 0;
    p[1] = value >> 8;
} /* putle16 */

/* 3-byte */
void
putle24(uint8_t *p, uint32_t value)
{
    putle16(p, value);
    p[2] = value >> 16;
} /* putle24 */

/* 4-byte */
void
putle32(uint8_t *p, uint32_t value)
{
    putle24(p, value);
    p[3] = value >> 24;
} /* putle32 */

/* 5-byte */
void
putle40(uint8_t *p, uint64_t value)
{
    putle32(p, value);
    p[4] = value >> 32;
} /* putle40 */

/* 6-byte */
void
putle48(uint8_t *p, uint64_t value)
{
    putle40(p, value);
    p[5] = value >> 40;
} /* putle48 */

/* 7-byte */
void
putle56(uint8_t *p, uint64_t value)
{
    putle48(p, value);
    p[6] = value >> 48;
} /* putle56 */

/* 8-byte */
void
putle64(uint8_t *p, uint64_t value)
{
    putle56(p, value);
    p[7] = value >> 56;
} /* putle64 */
