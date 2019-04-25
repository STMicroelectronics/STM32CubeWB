/**
  ******************************************************************************
  * @file    ecc.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides Elliptic Curve Cryptography (ECC) primitives
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_ECC_H__
#define __CRL_ECC_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "../Common_ecc_rsa/MATH/math.h"

  /** @addtogroup ECC
    * @{
    */

  /**
    * @brief  Structure continaing the BigNum_stt that describes the parameters of an Elliptic Curve and its generator
    */
  typedef struct
  {
    BigNum_stt *pmA;          /*!< Parameter a of the curve equation. */
    BigNum_stt *pmB;          /*!< Parameter b of the curve equation. */
    BigNum_stt *pmP;          /*!< Modulus p. */
    BigNum_stt *pmN;          /*!< Order of the curve n. */
    BigNum_stt *pmGx;         /*!< Coordinate pmX of curve's generator */
    BigNum_stt *pmGy;         /*!< Coordinate pmY of curve's generator*/
  }
  intEC_stt;


  /**
    * @brief  Structure that keeps the Elliptic Curve Parameter
    */
  typedef struct
  {
    const uint8_t *pmA;  /*!< pointer to paramter "a" */

    int32_t mAsize;      /*!< size of paramter "a" */

    const uint8_t *pmB;  /*!< pointer to paramter "b" */

    int32_t mBsize;      /*!< size of paramter "b" */

    const uint8_t *pmP;  /*!<pointer to paramter "p" */

    int32_t mPsize;      /*!<size of paramter "p" */

    const uint8_t *pmN;  /*!< pointer to paramter "n" */

    int32_t mNsize;      /*!< size of paramter "n" */

    const uint8_t *pmGx; /*!< pointer to x coordinate of generator point */

    int32_t mGxsize;     /*!< size of x coordinate of generator point */

    const uint8_t *pmGy; /*!< pointer to y coordinate of generator point */

    int32_t mGysize;     /*!< size of y coordinate of generator point */

    intEC_stt *pmInternalEC;  /*!< Pointer to internal structure for handling the parameters */
  }
  EC_stt;


  /**
    * @brief  Enumeration to specify the possible flags for an Elliptic Curve Point
    */
  typedef enum ECPntFlags_e
  {

    E_POINT_GENERAL = 0,    /*!< The point is not normalized (Coordinate Z != 1) */

    E_POINT_NORMALIZED = 1, /*!< The point is normalized (Coordinate Z == 1)*/

    E_POINT_INFINITY = 2,   /*!< The point is the O point */

    E_POINT_MONTY = 4       /*!< The point's coordinates are expressed in Montgomery domain */
  } ECPntFlags_et;

  /**
    * @brief  Object used to store an elliptic curve point.
    */
  typedef struct
  {

    BigNum_stt *pmX ;     /*!< pmX coordinate. */

    BigNum_stt *pmY ;     /*!< pmY coordinate. */

    BigNum_stt *pmZ ;     /*!< pmZ coordinate, used in projective representations. */

    ECPntFlags_et mFlag;  /*!< Point Flag, allowed values are: \n
                             * - flag=CRL_EPOINT_GENERAL for a point which may have pmZ different from 1
                             * - flag=CRL_EPOINT_NORMALIZED for a point which has pmZ equal to 1
                             * - flag=CRL_EPOINT_INFINITY to denote the infinity point
                            */
  }
  ECpoint_stt;

  /**
    * @brief   Enumeration for the coordinates of an elliptic curve point
    */
  typedef enum ECcoordinate_e
  {
    E_ECC_POINT_COORDINATE_X = 0,  /*!< Coordinate X */

    E_ECC_POINT_COORDINATE_Y = 1,   /*!< Coordinate Y */

    E_ECC_POINT_COORDINATE_Z = 2,   /*!< Coordinate Z */
  } ECcoordinate_et;


  /**
    * @brief   Object used to store an ECC private key
    */
  typedef struct
  {
    BigNum_stt *pmD;   /*!<  BigNum Representing the Private Key */
  }
  ECCprivKey_stt;

  /**
    * @brief   Object used to store an ECDSA signature
    */
  typedef struct
  {
    /** R */
    BigNum_stt *pmR ;  /*!< pointer to paramter R*/
    /** S */
    BigNum_stt *pmS ; /*!< pointer to paramter S*/
  }
  ECDSAsignature_stt;


  /**
    * @brief  Enumeration for the values inside the ECDSA signature
    */
  typedef enum ECDSAsignValues_e
  {
    E_ECDSA_SIGNATURE_R_VALUE = 0,  /*!<  Value R  */
    E_ECDSA_SIGNATURE_S_VALUE = 1,  /*!<  Value S */
  } ECDSAsignValues_et;

  /**
    * @brief  Structure used in ECDSA signature verification function
    */
  typedef struct
  {

    ECpoint_stt *pmPubKey;  /*!<  Pointer to the ECC Public Key used in the verification */

    EC_stt      *pmEC;      /*!<  Pointer to Elliptic Curve parameters */
  }
  ECDSAverifyCtx_stt;


  /**
    * @brief  Structure used in ECDSA signature generation function
    */
  typedef struct
  {

    ECCprivKey_stt *pmPrivKey;  /*!<  Pointer to the ECC Private Key used in the verification */

    EC_stt         *pmEC;       /*!<  Pointer to Elliptic Curve parameters */

    RNGstate_stt   *pmRNG;      /*!<  Pointer to an Initialized Random Engine Status */
  }
  ECDSAsignCtx_stt;

  /* Exported functions ------------------------------------------------------- */
  int32_t ECCinitEC(EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECCfreeEC(EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECCinitPoint(ECpoint_stt **P_ppECPnt, const EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECCfreePoint(ECpoint_stt **P_pECPnt, membuf_stt *P_pMemBuf);
  int32_t ECCsetPointCoordinate(ECpoint_stt *P_pECPnt,
                                ECcoordinate_et P_Coordinate,
                                const uint8_t *P_pCoordinateValue,
                                int32_t P_coordinateSize);
  int32_t ECCgetPointCoordinate(const ECpoint_stt *P_pECPnt,
                                ECcoordinate_et P_Coordinate,
                                uint8_t *P_pCoordinateValue,
                                int32_t *P_pCoordinateSize);
  int32_t ECCgetPointFlag(const ECpoint_stt *P_pECPnt);
    void ECCsetPointFlag(ECpoint_stt *P_pECPnt, 
                         ECPntFlags_et P_newFlag);
  int32_t ECCsetPointGenerator(ECpoint_stt *P_pPoint, const EC_stt *P_pECctx);
  int32_t ECCcopyPoint(const ECpoint_stt *P_pOriginalPoint, ECpoint_stt *P_pCopyPoint);
  int32_t ECCinitPrivKey(ECCprivKey_stt **P_ppECCprivKey, const EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECCfreePrivKey(ECCprivKey_stt **P_pECCprivKey, membuf_stt *P_pMemBuf);
  int32_t ECCsetPrivKeyValue(ECCprivKey_stt *P_pECCprivKey,
                             const uint8_t *P_pPrivateKey,
                             int32_t P_privateKeySize);
  int32_t ECCgetPrivKeyValue(const ECCprivKey_stt *P_pECCprivKey,
                             uint8_t *P_pPrivateKey,
                             int32_t *P_pPrivateKeySize);
  int32_t ECCscalarMul(const ECpoint_stt *P_pECbasePnt,
                       const ECCprivKey_stt *P_pECCprivKey,
                       ECpoint_stt *P_pECresultPnt,
                       const EC_stt *P_pECctx,
                       membuf_stt *P_pMemBuf);
  int32_t ECDSAinitSign(ECDSAsignature_stt **P_ppSignature, const EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECDSAfreeSign(ECDSAsignature_stt **P_pSignature, membuf_stt *P_pMemBuf);
  int32_t ECDSAsetSignature(ECDSAsignature_stt *P_pSignature,
                            ECDSAsignValues_et P_RorS,
                            const uint8_t *P_pValue,
                            int32_t P_valueSize);
  int32_t ECDSAgetSignature(const ECDSAsignature_stt *P_pSignature,
                            ECDSAsignValues_et P_RorS,
                            uint8_t *P_pValue,
                            int32_t *P_pValueSize);
  int32_t ECDSAverify(const uint8_t      *P_pDigest,
                      int32_t             P_digestSize,
                      const ECDSAsignature_stt   *P_pSignature,
                      const ECDSAverifyCtx_stt *P_pVerifyCtx,
                      membuf_stt *P_pMemBuf);
  int32_t ECCvalidatePubKey(const ECpoint_stt *pECCpubKey, const EC_stt *P_pECctx, membuf_stt *P_pMemBuf);
  int32_t ECCkeyGen(ECCprivKey_stt *P_pPrivKey,
                    ECpoint_stt    *P_pPubKey,
                    RNGstate_stt * P_pRandomState,
                    const EC_stt    *P_pECctx,
                    membuf_stt *P_pMemBuf);
  int32_t ECDSAsign(const uint8_t         *P_pDigest,
                    int32_t                P_digestSize,
                    const ECDSAsignature_stt *P_pSignature,
                    const ECDSAsignCtx_stt *P_pSignCtx,
                    membuf_stt *P_pMemBuf);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __ECC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
