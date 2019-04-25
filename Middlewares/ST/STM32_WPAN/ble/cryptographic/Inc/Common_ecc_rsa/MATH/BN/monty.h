/**
  ******************************************************************************
  * @file    monty.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides Montgomery Operations
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
#ifndef __CRL_MONTY_H__
#define __CRL_MONTY_H__

/* When multiplications are required the routines called are: MultiplyBig() and SquareBig().
*  The implementation of these two functions depends by the value of the constant
*  BASIC_MUL_BIG defined in MulDiv.h. */

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup Monty
    * @{
    */

  /* Exported types ------------------------------------------------------------*/

  /**
    * @brief  Structure used to store Montgomery domain parameters.
    */
  typedef struct
  {
    const BigNum_stt *pmModulusBN;  /*!< The modulus on which are reffered the parameters. */
    BigNum_stt *pmR2;               /*!< The Montgomery Parameter */
    uint32_t mModInvLSW;            /*!<  The least significant word of -(modulus^(-1)) (mod MontRadix). */
    membuf_stt *pmMemBuf;
  }
  montyParams_stt;

  /* Exported functions ------------------------------------------------------- */

  /* Montgomery Modular Operations **********************************************/

  /* Comment:  given an integer x in Zm, the Montgomery residue (o M-residue) is:
  *            xR (mod m), get as: MontyExit(A = x*R^2,T) or
  *             MontyMul(x,R^2,Residuo_x).                                      */

  /* Initializes Montgomery parameters according to the modulus m. (different implementation) */
  int32_t MontyInit (const BigNum_stt *P_pModulusBN, \
                     montyParams_stt *P_pMontyParams, \
                     membuf_stt *P_pMemBuf);
  \
  /* Free R^2 inside the montgomery parameters contained in a montyParams_stt structure */
  void MontyKillR2  (montyParams_stt * P_pMontyParams);

  /* Sets to zero all Montgomery parameters. ************************************/
  void MontyKillAll (montyParams_stt *P_pMontyParams);

  /* y=x(R^-1) (mod m); x from the Montgomery domain it is carried to integer modulo m. */
  int32_t MontyExit (const BigNum_stt *P_pMontyBN,         \
                     BigNum_stt *P_pNormalBN,               \
                     const montyParams_stt *P_pMontyParams);
  \

  /* Montgomery cios multiplication *********************************************/
  int32_t MontyMul (const BigNum_stt * P_pFactor1BN,       \
                    const BigNum_stt * P_pFactor2BN,       \
                    BigNum_stt * P_pProductBN,             \
                    const montyParams_stt *P_pMontyParams);
  \

  /* Exported macros ------------------------------------------------------- */
  /* Squaring in monty domain ***************************************************/
#define MontySqr(P_Base,P_Square,P_pMontyParams) MontyMul((P_Base),(P_Base),(P_Square),(P_pMontyParams))

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_MONTY_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
