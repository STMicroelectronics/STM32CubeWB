/**
 * @file zigbee.security.h
 * @heading Security Utilities
 * @brief Zigbee security header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_SECURITY_H
#define ZIGBEE_SECURITY_H

#include <stdint.h>
#include <stdbool.h>

struct ZigBeeT;

/* Security Install Code Max Length (including CRC) */
#define ZB_SEC_INSTALL_CODE_MAX_LENGTH      18U

/* Key and Cipher strengths used by ZigBee. */
#define ZB_SEC_BLOCKSIZE                    16U
#define ZB_SEC_KEYSIZE                      ZB_SEC_BLOCKSIZE

/* ZB_SEC_KEYSTR_SIZE is a helper to know how much to allocate
 * for ascii string buffer. */
#define ZB_SEC_KEYSTR_SIZE      ((ZB_SEC_KEYSIZE * 2U) + ZB_SEC_KEYSIZE /* separators */ + 1U /* NULL */)

/*
 * +-----------------------------------------------------------------------------+
 * | Security      Security       Security       Data         Frame Integrity    |
 * | level         Level Sub-     Attributes     Encryption   (length M of MIC,  |
 * | identifier    Field                                      in number of       |
 * |               (Figure 18)                                octets)            |
 * +-----------------------------------------------------------------------------+
 * | 0x00          000          None           OFF           NO (M = 0)        |
 * | 0x01          001          MIC-32         OFF          YES (M=4)          |
 * | 0x02          010          MIC-64         OFF          YES (M=8)          |
 * | 0x03          011          MIC-128        OFF          YES (M=16)         |
 * | 0x04          100          ENC            ON            NO (M = 0)        |
 * | 0x05          101          ENC-MIC-32     ON           YES (M=4)          |
 * | 0x06          110          ENC-MIC-64     ON           YES (M=8)          |
 * | 0x07          111          ENC-MIC-128    ON           YES (M=16)         |
 * +-----------------------------------------------------------------------------+
 */
#define ZB_SEC_LEVEL_NONE                   0x00U
#define ZB_SEC_LEVEL_MIC32                  0x01U
#define ZB_SEC_LEVEL_MIC64                  0x02U
#define ZB_SEC_LEVEL_MIC128                 0x03U
#define ZB_SEC_LEVEL_ENC                    0x04U
#define ZB_SEC_LEVEL_ENC_MIC32              (uint8_t)(ZB_SEC_LEVEL_ENC | ZB_SEC_LEVEL_MIC32)
#define ZB_SEC_LEVEL_ENC_MIC64              (uint8_t)(ZB_SEC_LEVEL_ENC | ZB_SEC_LEVEL_MIC64)
#define ZB_SEC_LEVEL_ENC_MIC128             (uint8_t)(ZB_SEC_LEVEL_ENC | ZB_SEC_LEVEL_MIC128)

/* Macro checks security level if encryption is enabled */
#define ZB_SEC_ENCRYPTED(level)             ((level & ZB_SEC_LEVEL_ENC) != 0U)

/* Macro returns the length of the MIC, can be computed
 * as 4bytes * Floor((2 ^ (level - 1))), or:
 * 4 * 2 ^ (level - 1) rounded down to the nearest 4 bytes.
 *
 * See right-most column in table above.
 */
#define ZB_SEC_MIC_LENGTH(level)            ((2U << ((level) & 0x3U)) & ~0x3U)
#define ZB_SEC_MIC_LENGTH_5                 4U

/* The maximum possible MIC length. */
#define ZB_SEC_MAX_MIC_LENGTH               16U

/* The length of the CCM* nonce. */
#define ZB_SEC_NONCE_LENGTH                 13U

/* The maximum size of the auxiliary header. */
#define ZB_SEC_MAX_HEADER_SIZE              14U

/* Masks for the Security control header fields. (section 4.5.1)*/
#define ZB_SEC_SECCTRL_MASK_LEVEL           (uint8_t)0x07U /* Bits 0-2 */
#define ZB_SEC_SECCTRL_MASK_KEYID           (uint8_t)0x18U /* Bits 3-4 */
#define ZB_SEC_SECCTRL_MASK_EXTNONCE        (uint8_t)0x20U /* Bits 5 */
#define ZB_SEC_SECCTRL_MASK_RESERVED        (uint8_t)0x60U /* Bits 6-7 */

/* Offsets of the Security control header fields. */
#define ZB_SEC_SECCTRL_OFFSET_LEVEL         0U
#define ZB_SEC_SECCTRL_OFFSET_KEYID         3U
#define ZB_SEC_SECCTRL_OFFSET_EXTNONCE      5U

/* Key Ids (Frame Control Field). */
enum ZbSecHdrKeyIdT {
    ZB_SEC_KEYID_LINK = 0x00,
    ZB_SEC_KEYID_NETWORK = 0x01,
    ZB_SEC_KEYID_TRANSPORT = 0x02,
    ZB_SEC_KEYID_KEYLOAD = 0x03,
    /* Exegin add-on - not for over-the-air */
    ZB_SEC_KEYID_BOTH_LINK_NETWORK = 0xfe, /* For Update Device (send two) */
    ZB_SEC_KEYID_DEFAULT = 0xff
};

/* Maximum value for a frame counter. */
#define ZB_SEC_MAX_FRAME_COUNTER            0xffffffffU

/* Frame Counter Resets are controlled much like a lollipop counter, and require
 * the 'new' value to be near zero to guard against replay attacks. */
#define ZB_FRAME_COUNTER_RESET_MAX          256U

/* Key Type Enumerations (Primitives and over-the-air). */
enum ZbSecKeyTypeT {
    /* Reserved -- was Trust-Center master key */
    ZB_SEC_KEYTYPE_STANDARD_NWK = 0x01, /* Standard network key */
    /* 0x02 -- Reserved -- was Application master key */
    ZB_SEC_KEYTYPE_APP_LINK = 0x03, /* Application link key */
    ZB_SEC_KEYTYPE_TC_LINK = 0x04 /* Trust-Center link key */
        /* 0x05 -- Reserved -- was High security network key */
};

/*---------------------------------------------------------------
 * Flags to indicate encryption used. Loosely based on enum ZbSecKeyTypeT.
 *---------------------------------------------------------------
 */
#define ZB_SEC_ENCRYPT_TYPE_LINK_FLAG       0x80U

enum ZbSecEncryptT {
    /* No encryption used */
    ZB_SEC_ENCRYPT_TYPE_NONE = 0x00,
    /* Encrypted with standard network key */
    ZB_SEC_ENCRYPT_TYPE_STANDARD_NWK = 0x01,
    /* Link keys */
    /* ZB_SEC_ENCRYPT_TYPE_LINK_FLAG = ZB_SEC_ENCRYPT_TYPE_LINK_FLAG, */
    /* Application link key */
    ZB_SEC_ENCRYPT_TYPE_APP_LINK = 0x83, /* ZB_SEC_ENCRYPT_TYPE_LINK_FLAG | 0x03U */
    /* Trust-Center link key */
    ZB_SEC_ENCRYPT_TYPE_TC_LINK = 0x84, /* ZB_SEC_ENCRYPT_TYPE_LINK_FLAG | 0x04U */
    /* Preconfigured Global Trust-Center link key */
    ZB_SEC_ENCRYPT_TYPE_GLOBAL_TC_LINK = 0x90, /* ZB_SEC_ENCRYPT_TYPE_LINK_FLAG | 0x10U */
    /* Distributed Global Trust-Center link key */
    ZB_SEC_ENCRYPT_TYPE_DISTRIB_TC_LINK = 0xa0 /* ZB_SEC_ENCRYPT_TYPE_LINK_FLAG | 0x20U */
};

/*---------------------------------------------------------------
 * CBKE Certificate Formats
 *---------------------------------------------------------------
 */
/* Field sizes for the elliptic curve (NIST-K163, aka SECT-163K1) */
#define CBKE_PRIVATE_KEY_SIZE               21U /* sizeof(2^163) */
#define CBKE_COMPRESSED_PUBLIC_KEY_SIZE     (CBKE_PRIVATE_KEY_SIZE + 1U)
#define CBKE_UNCOMPRESSED_PUBLIC_KEY_SIZE   (2 * CBKE_PRIVATE_KEY_SIZE + 1U)
#define CBKE_SHARED_SECRET_SIZE             CBKE_PRIVATE_KEY_SIZE

/* Field sizes for the elliptic curve (NIST-K283, aka SECT-283K1) */
#define CBKE2_PRIVATE_KEY_SIZE              36U /* sizeof(2^283) */
#define CBKE2_COMPRESSED_PUBLIC_KEY_SIZE    (CBKE2_PRIVATE_KEY_SIZE + 1U)
#define CBKE2_UNCOMPRESSED_PUBLIC_KEY_SIZE  (2U * CBKE2_PRIVATE_KEY_SIZE + 1U)
#define CBKE2_SHARED_SECRET_SIZE            CBKE2_PRIVATE_KEY_SIZE

/* Size and layout of the CBKE certificate. */
#define CBKE_CERT_SUBJECT_OFFSET            CBKE_COMPRESSED_PUBLIC_KEY_SIZE
#define CBKE_CERT_SUBJECT_SIZE              8U
#define CBKE_CERT_ISSUER_OFFSET             (CBKE_CERT_SUBJECT_OFFSET + CBKE_CERT_SUBJECT_SIZE)
#define CBKE_CERT_ISSUER_SIZE               8U
#define CBKE_CERT_DATA_OFFSET               (CBKE_CERT_ISSUER_OFFSET + CBKE_CERT_ISSUER_SIZE)
#define CBKE_CERT_DATA_SIZE                 10U
#define CBKE_CERTIFICATE_SIZE               (CBKE_CERT_DATA_OFFSET + CBKE_CERT_DATA_SIZE)

/* Size and layout of the CBKE2 certificate. */
#define CBKE2_CERT_TYPE_OFFSET              0U
#define CBKE2_CERT_TYPE_SIZE                1U
#define CBKE2_CERT_TYPE                     0x00U
#define CBKE2_CERT_SERIAL_OFFSET            (CBKE2_CERT_TYPE_SIZE)
#define CBKE2_CERT_SERIAL_SIZE              8U
#define CBKE2_CERT_CURVE_OFFSET             (CBKE2_CERT_SERIAL_OFFSET + CBKE2_CERT_SERIAL_SIZE)
#define CBKE2_CERT_CURVE_SIZE               1U
#define CBKE2_CERT_CURVE                    CURVE_IDENTIFIER_SECT283K1
#define CBKE2_CERT_HASH_OFFSET              (CBKE2_CERT_CURVE_OFFSET + CBKE2_CERT_CURVE_SIZE)
#define CBKE2_CERT_HASH_SIZE                1U
#define CBKE2_CERT_HASH                     HASH_IDENTIFIER_AES_MMO
#define CBKE2_CERT_ISSUER_OFFSET            (CBKE2_CERT_HASH_OFFSET + CBKE2_CERT_HASH_SIZE)
#define CBKE2_CERT_ISSUER_SIZE              8U
#define CBKE2_CERT_VALID_FROM_OFFSET        (CBKE2_CERT_ISSUER_OFFSET + CBKE2_CERT_ISSUER_SIZE)
#define CBKE2_CERT_VALID_FROM_SIZE          5U
#define CBKE2_CERT_VALID_TO_OFFSET          (CBKE2_CERT_VALID_FROM_OFFSET + CBKE2_CERT_VALID_FROM_SIZE)
#define CBKE2_CERT_VALID_TO_SIZE            4U
#define CBKE2_CERT_SUBJECT_OFFSET           (CBKE2_CERT_VALID_TO_OFFSET + CBKE2_CERT_VALID_TO_SIZE)
#define CBKE2_CERT_SUBJECT_SIZE             8U
#define CBKE2_CERT_KEY_USAGE_OFFSET         (CBKE2_CERT_SUBJECT_OFFSET + CBKE2_CERT_SUBJECT_SIZE)
#define CBKE2_CERT_KEY_USAGE_SIZE           1U
#define CBKE2_CERT_KEY_USAGE                KEY_USAGE_AGREEMENT
#define CBKE2_CERT_PUBLIC_KEY_OFFSET        (CBKE2_CERT_KEY_USAGE_OFFSET + CBKE2_CERT_KEY_USAGE_SIZE)
#define CBKE2_CERT_PUBLIC_KEY_SIZE          37U
#define CBKE2_CERTIFICATE_SIZE              (CBKE2_CERT_PUBLIC_KEY_OFFSET + CBKE2_CERT_PUBLIC_KEY_SIZE)

struct ZbZclCbkePrivateT {
    uint8_t privateKey[CBKE_PRIVATE_KEY_SIZE];
    uint8_t publicCaKey[CBKE_COMPRESSED_PUBLIC_KEY_SIZE];
};

struct ZbZclCbkeInfoT {
    struct ZbZclCbkePrivateT keys;
    uint8_t cert[CBKE_CERTIFICATE_SIZE];
    uint8_t ephemeralTime; /* In seconds. If 0, CBKE_V1_EPHEMERAL_DEFAULT_TIME is used */
    uint8_t confirmTime; /* In seconds. If 0, CBKE_V1_CONFIRM_DEFAULT_TIME is used */
};

struct ZbZclCbke2PrivateT {
    uint8_t privateKey[CBKE2_PRIVATE_KEY_SIZE];
    uint8_t publicCaKey[CBKE2_COMPRESSED_PUBLIC_KEY_SIZE];
};

struct ZbZclCbke2InfoT {
    struct ZbZclCbke2PrivateT keys;
    uint8_t cert[CBKE2_CERTIFICATE_SIZE];
    uint8_t ephemeralTime; /* In seconds. If 0, CBKE_V2_EPHEMERAL_DEFAULT_TIME is used */
    uint8_t confirmTime; /* In seconds. If 0, CBKE_V2_CONFIRM_DEFAULT_TIME is used */
};

/*---------------------------------------------------------------
 * Auxiliary Frame Functions
 *---------------------------------------------------------------
 */
/* Security Control Field of the Auxiliary Header */
struct ZbSecAuxHdrCtrlT {
    uint8_t secLevel;
    /**< Security level of the frame. */
    enum ZbSecHdrKeyIdT keyId;
    /**< Key used to encrypt the frame. */
    bool extNonce;
    /**< Extended nonce present. */
};

/* Structure containing the fields stored in the Aux Header */
struct ZbSecAuxHdrT {
    struct ZbSecAuxHdrCtrlT securityCtrl;
    uint32_t frameCounter;
    uint64_t srcExtAddr; /* Present if securityCtrl.extNonce */
    uint8_t keySeqno; /* Present if securityCtrl.keyId = 1 (Network Key) */
};

/**
 * Parses the auxiliary header into the provided structure.
 * @param data Packet buffer
 * @param dataLen Packet length
 * @param auxHdrPtr Pointer to ZbSecAuxHdrT structure to parse data into.
 * @return Number of bytes in the header, or <0 on error.
 */
int ZbSecParseAuxHdr(const uint8_t *data, unsigned int dataLen, struct ZbSecAuxHdrT *auxHdrPtr);

/**
 * Append an auxiliary security header to a packet
 * @param data Packet buffer
 * @param dataLen Packet length
 * @param auxHdrPtr auxiliary header struct to use
 * @return Number of bytes written, or <0 on error.
 */
int ZbSecAppendAuxHdr(uint8_t *data, unsigned int dataLen, struct ZbSecAuxHdrT *auxHdrPtr);

/**
 * Builds the CCM* nonce from the source address, frame counter and security control bits.
 * The nonce buffer must be at least ZB_SEC_NONCE_LENGTH in size.
 * @param nonce Nonce buffer (output)
 * @param extAddr Source extended address
 * @param frameCounter Frame counter
 * @param secCtrl Security control field
 * @return None
 */
void ZbSecMakeNonce(uint8_t *nonce, uint64_t extAddr, uint32_t frameCounter, uint8_t secCtrl);

/*---------------------------------------------------------------
 * Security Transformations
 *---------------------------------------------------------------
 */

/**
 * Performs an AES MMO hash on the selected data
 * @param zb Zigbee stack instance
 * @param data data to hash
 * @param length length of data to hash
 * @param digest hash must be AES_BLOCK_SIZE in size!
 * @return
 */
bool ZbAesMmoHash(uint8_t const *data, const unsigned int length, uint8_t *digest);

/**
 * Performs the Keyed Hash function for Message Authentication. The HMAC operation described
 * in section B.1.4, and specified by FIPS pub 198. This is used to trasmute the link keys into
 * key-load and key-transport keys.
 * @param key The encryption key to use
 * @param input The input data byte
 * @param keyOut The resultant transformed key
 * @return None
 */
void ZbSecKeyTransform(uint8_t *key, uint8_t input, uint8_t *keyOut);

/**
 * Adds a device key-pair to the stack as a Trust Center Link Key type.
 * This API is typically used to add link keys to the TC for devices
 * that need to join the network.
 * @param zb Zigbee stack instance
 * @param extAddr The EUI64 address of the device matching the link key
 * @param key The link key, of length ZB_SEC_KEYSIZE, to add
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZbSecAddDeviceLinkKeyByKey(struct ZigBeeT *zb, uint64_t extAddr,
    uint8_t *key);

/**
 * Adds a device key-pair to the stack as a Trust Center Link Key type.
 * The key being added is defined by the Install Code provided to this function.
 * This API is typically used to add link keys to the TC for devices
 * that need to join the network.
 * @param zb Zigbee stack instance
 * @param extAddr The EUI64 address of the device matching the link key
 * @param ic The install code, including the trailing 2-octet CRC.
 * @param len Length of the Install Code in bytes
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZbSecAddDeviceLinkKeyByInstallCode(struct ZigBeeT *zb, uint64_t extAddr,
    uint8_t *ic, unsigned int len);

/*---------------------------------------------------------------
 * ECDSA Signature Validation (e.g. for OTA Upgrade)
 *---------------------------------------------------------------
 */
enum ZbSecEcdsaSigType {
    ZB_SEC_ECDSA_SIG_SUITE_1,
    ZB_SEC_ECDSA_SIG_SUITE_2
};

#define ZB_SEC_CRYPTO_SUITE_V2_CERT_LEN             74U
#define ZB_SEC_CRYPTO_SUITE_V2_SIG_LEN              80U /* IEEE[8] r[36] s[36] */

/**
 * Elliptic Curve Digital Signature Algorithm (ECDSA) validator.
 * @param zb Zigbee stack instance
 * @param sig_type Signature suite type (e.g. ZB_SEC_ECDSA_SIG_SUITE_1)
 * @param ca_pub_key_array Public key
 * @param ca_pub_key_len Public key length
 * @param certificate Length must be ZB_SEC_CRYPTO_SUITE_V2_CERT_LEN (74 bytes)
 * @param signature Length must be ZB_SEC_CRYPTO_SUITE_V2_SIG_LEN (80 bytes)
 * @param image_digest Length is AES_BLOCK_SIZE (16 bytes)
 * @param cert_digest Length is AES_BLOCK_SIZE (16 bytes)
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZbSecEcdsaValidate(struct ZigBeeT *zb, enum ZbSecEcdsaSigType sig_type,
    const uint8_t *ca_pub_key_array, unsigned int ca_pub_key_len,
    const uint8_t *certificate, const uint8_t *signature,
    const uint8_t *image_digest, const uint8_t *cert_digest);

#endif
