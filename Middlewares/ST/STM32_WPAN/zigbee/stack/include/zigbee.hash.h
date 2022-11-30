/**
 * @file zigbee.hash.h
 * @heading Hash Utilities
 * @brief ZigBee AES Hash Implementation
 * @author Exegin Technologies
 * @copyright Copyright [2020 - 2022] Exegin Technologies Limited. All rights reserved.
 *
 * This file defines the Exegin specific ZigBee AES Hash structure and also provides
 * the declaration of Hashing API's. Customers with their own hashing implementation
 * can simply replace this file with their variant. In effect it helps to plug
 * customer specific hashing implementation into ZSDK stack.
 */

#ifndef ZIGBEE_HASH_H
# define ZIGBEE_HASH_H

/*---------------------------------------------------------------
 * AES Hashing
 *---------------------------------------------------------------
 */
#ifndef AES_BLOCK_SIZE
# define AES_BLOCK_SIZE                     16U
#endif

struct ZbHash {
    uint8_t m[AES_BLOCK_SIZE];
    uint8_t hash[AES_BLOCK_SIZE];
    uint8_t key[AES_BLOCK_SIZE];
    uint8_t length;
};

/* Matyas-Meyer-Oseas hash function. */
void ZbHashInit(struct ZbHash *h);
void ZbHashAdd(struct ZbHash *h, const void *data, uint32_t len);
void ZbHashByte(struct ZbHash *h, uint8_t data);
void ZbHashDigest(struct ZbHash *h, void *digest);

/* HMAC hash function (based on AES-MMO) */
void ZbHmacInit(struct ZbHash *h, const void *key, uint32_t len);
#define ZbHmacAdd(hash, data, len)   ZbHashAdd(hash, data, len)
#define ZbHmacByte(hash, byte)         ZbHashByte(hash, byte)
void ZbHmacDigest(struct ZbHash *h, void *digest);

#endif /* ZIGBEE_HASH_H */
