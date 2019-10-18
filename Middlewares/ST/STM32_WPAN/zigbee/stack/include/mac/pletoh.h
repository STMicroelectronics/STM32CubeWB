/* Copyright [2009 - 2018] Exegin Technologies Limited. All rights reserved. */

#ifndef PLETOH_H_
#define PLETOH_H_

#include <stdint.h>
#include <stdbool.h>

bool is_little_endian(void);

/*----------------------------------------------------------------------------
 * Read Little Endian
 *----------------------------------------------------------------------------
 */
/* 2-byte */
uint16_t pletoh16(const uint8_t *p);

/* 3-byte */
uint32_t pletoh24(const uint8_t *p);

/* 4-byte */
uint32_t pletoh32(const uint8_t *p);

/* 5-byte */
uint64_t pletoh40(const uint8_t *p);

/* 6-byte */
uint64_t pletoh48(const uint8_t *p);

/* 7-byte */
uint64_t pletoh56(const uint8_t *p);

/* 8-byte */
uint64_t pletoh64(const uint8_t *p);

/*----------------------------------------------------------------------------
 * Write Little Endian
 *----------------------------------------------------------------------------
 */
/* 2-byte */
void putle16(uint8_t *p, uint16_t value);

/* 3-byte */
void putle24(uint8_t *p, uint32_t value);

/* 4-byte */
void putle32(uint8_t *p, uint32_t value);

/* 5-byte */
void putle40(uint8_t *p, uint64_t value);

/* 6-byte */
void putle48(uint8_t *p, uint64_t value);

/* 7-byte */
void putle56(uint8_t *p, uint64_t value);

/* 8-byte */
void putle64(uint8_t *p, uint64_t value);

#endif /* _PLETOH_H_ */
