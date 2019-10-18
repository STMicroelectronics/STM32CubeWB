/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef M4_LOCAL_ZIGBEE_H
# define M4_LOCAL_ZIGBEE_H

/* This file is for the M4 side only, and is mainly for the cluster code that
 * can be compiled with the zigbee stack, or separate like in this case. */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* size_t */
#include <stdlib.h> /* rand */
#include <stdint.h>

#ifndef AES_BLOCK_SIZE
# define AES_BLOCK_SIZE                     16U
#endif

struct ZigBeeT;

/*---------------------------------------------------------------
 * critical lock (Should not be needed on this platform)
 *---------------------------------------------------------------
 */
#define ZbEnterCritical(_zb_)
#define ZbExitCritical(_zb_)

/*---------------------------------------------------------------
 * memory allocation
 *---------------------------------------------------------------
 */
void * zb_heap_alloc(struct ZigBeeT *zb, size_t sz, const char *funcname, unsigned int linenum);
void zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *funcname, unsigned int linenum);
#define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, "", 0)
#define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, "", 0)

/*---------------------------------------------------------------
 * memset, memcpy, memmove, strcpy replacements
 *---------------------------------------------------------------
 */
void * ZbMemSet(void *s, int c, size_t n);
void * ZbMemCpy(void *dest, const void *src, size_t n);
void * ZbMemMove(void *dest, const void *src, size_t n);
char * ZbStrCpy(char *dest, const char *src);

/*---------------------------------------------------------------
 * String conversion
 *---------------------------------------------------------------
 */
int zb_hex_str_to_bin(const char *string, void *out, unsigned int maxlen);
unsigned int zb_hex_bin_to_str(const uint8_t *in_data, unsigned int in_len, char *out_str, unsigned int max_len,
    const char delimiter, unsigned int interval);

/*---------------------------------------------------------------
 * random value
 *---------------------------------------------------------------
 */
/* On M4, this doesn't need to be very random. Not used with security. */
#define ZbPortGetRand(zb)                   rand()

/*---------------------------------------------------------------
 * AES Hashing
 *---------------------------------------------------------------
 */
typedef struct {
    uint8_t m[AES_BLOCK_SIZE];
    uint8_t hash[AES_BLOCK_SIZE];
    uint8_t key[AES_BLOCK_SIZE];
    uint8_t length;
} ZbHashT;

/* Matyas-Meyer-Oseas hash function. */
void ZbHashInit(ZbHashT *h);
void ZbHashAdd(ZbHashT *h, const void *data, uint32_t len);
void ZbHashByte(ZbHashT *h, uint8_t byte);
void ZbHashDigest(ZbHashT *h, void *digest);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* M4_LOCAL_ZIGBEE_H */
