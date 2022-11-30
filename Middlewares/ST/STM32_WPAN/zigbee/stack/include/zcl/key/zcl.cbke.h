/* Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 * Copyright (c) 2013-2014, Exegin Technologies Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Exegin Technologies Limited nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *--------------------------------------------------------------------------
 */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the Certificate
 *      Based Key Exchange suite to the ZigBee
 *      Key Establishment cluster.
 *--------------------------------------------------------------------------
 */

#ifndef ZCL_CBKE_H
#  define ZCL_CBKE_H

#include "zcl/zcl.h"
#include "zcl/key/zcl.cbke.h"
#include "zcl/key/zcl.key.h"

/*---------------------------------------------------------------
 * Constants
 *---------------------------------------------------------------
 */
#define CURVE_IDENTIFIER_SECT163K1          0x00U /* HACK: Not actually defined in the spec, but used for legacy certs. */
#define CURVE_IDENTIFIER_SECT283K1          0x0DU
#define HASH_IDENTIFIER_AES_MMO             0x08U
#define CERT_VALID_INFINITY                 0xffffffffU
#define CBKE2_KEY_USAGE_AGREEMENT           0x08U
#define CBKE2_DIGITAL_SIGNATURE             0x80U

#define CBKE_EPHEMERAL_MAX_TIME             0xfeU
#define CBKE_CONFIRM_MAX_TIME               0xfeU

#define CBKE_V1_EPHEMERAL_DEFAULT_TIME      11U
#define CBKE_V1_CONFIRM_DEFAULT_TIME        16U

#define CBKE_V2_EPHEMERAL_DEFAULT_TIME      22U
#define CBKE_V2_CONFIRM_DEFAULT_TIME        32U

/*---------------------------------------------------------------
 * Structures
 *---------------------------------------------------------------
 */
struct ZbZclCertificateT {
    uint8_t type;
    uint64_t serial;
    uint8_t curve;
    uint8_t hash;
    uint64_t issuer;
    uint64_t validFrom;
    uint32_t validTo;
    uint64_t subject;
    uint8_t keyUsage;
    unsigned int publicLen;
    uint8_t publicKey[CBKE2_CERT_PUBLIC_KEY_SIZE];
};

struct ZbZclKeClusterT;

/*---------------------------------------------------------------
 * Function Declarations
 *---------------------------------------------------------------
 */
enum ZclStatusCodeT ZbZclKeAddCbke(struct ZbZclClusterT *clusterPtr, const struct ZbZclCbkeInfoT *info);
enum ZclStatusCodeT ZbZclKeAddCbke2(struct ZbZclClusterT *clusterPtr, const struct ZbZclCbke2InfoT *info);
struct ZbZclCertificateT * ZbZclParseCertificate(struct ZbZclCertificateT *dst, const void *src, unsigned int len);
enum ZbZclKeyStatusT ZbZclCbke2ReconstPrivateKey(struct ZigBeeT *zb, const uint8_t *icu,
    const uint8_t *se, const uint8_t *du, uint8_t *wu);

/* Helper function to reconstruct a private key. */
uint8_t ZbZclCbkeReconstPrivateKey(const unsigned char *icu, const unsigned char *se, const unsigned char *du, unsigned char *wu);

struct ZbZclKeSuiteT * ZbZclKeMatchSuite(struct ZbZclKeClusterT *kePtr, uint16_t suite_mask);
bool ZbZclSuiteIsSupported(struct ZbZclClusterT *cluster, uint16_t suite);

#endif /* _ZCL_CBKE_H */
