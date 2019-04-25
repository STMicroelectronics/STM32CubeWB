/**
  ******************************************************************************
  * @file    bn.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Basic Functions for Big Numbers (Multi Precision Integers)
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
#ifndef __CRL_BN_H__
#define __CRL_BN_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup BN
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /**
    * @brief  Structure used to store a BigNum_stt * integer.
    */
  typedef struct
  {
    uint32_t *pmDigit;    /*!<  Used to represent the BigNum_stt * integer value; pmDigit[0] = least significant word. */
    uint16_t mNumDigits;  /*!<  Number of significant words of the vector pmDigit used to represent the actual value. */
    uint8_t mSize;        /*!<  Number of words allocated for the integer */
    int8_t mSignFlag;     /*!<  Is the integer mSignFlag: SIGN_POSITIVE positive, SIGN_NEGATIVE negative. */
  }
  BigNum_stt;

  /* Exported constants --------------------------------------------------------*/

#define NBIT_DIGIT        32u           /*!< Bit Size of a BigNum digit (should equal to CPU word size, works for 32). */
#define MSBIT_MASK        0x80000000u   /*!< Mask for the most significant bit in a digit */
#define MSBYTE_MASK       0xFF000000u   /*!< Mask for the most significant byte in a digit */
#define LSBIT_MASK        0x00000001u   /*!< Mask for the least significant bit in a digit */
#define MAX_DIGIT_VALUE   0xFFFFFFFFu   /*!< Mask for all bit to 1 in a word */
#define SIGN_POSITIVE     0             /*!< Used to denote a positive Big Number. */
#define SIGN_NEGATIVE     1             /*!< Used to denote a negative Big Number. */

  /* Exported functions ------------------------------------------------------- */
  /* Big Number Operations ******************************************************/

  /* Alloc big *****************************************************************/
  int32_t StAllocBig(BigNum_stt **P_ppBignum, \
                     int32_t P_wordSize,     \
                     membuf_stt *P_pMembuf);

  /* StFree big ****************************************************************/
  void StFreeBig    (BigNum_stt **P_pBignum, \
                     membuf_stt *P_pMembuf);

  /* x = initial_value. Sets the words x->pmDigit[1]_ _ _x->pmDigit[MAX_LEN_BIG-1] to 0. */
  void InitBig      (BigNum_stt * P_pBN,     \
                     int32_t P_initialValue);

  /* y=x. ***********************************************************************/
  int32_t CopyBig   (const BigNum_stt *P_pInputBN, \
                     BigNum_stt *P_pOutputBN);

  /* Returns 1 if x==0, 0 otherwise. ********************************************/
  int32_t IsZeroBig (const BigNum_stt *P_pInputBN);

  /* x=0. ***********************************************************************/
  void SetZeroBig   (BigNum_stt * P_pInputBN);

  /* Returns 1 if x>=0, returns 0 if x<0. ***************************************/
  int32_t SignBig   (const BigNum_stt *P_pInputBN);

  int32_t EvenBig   (const BigNum_stt *P_pBN);

  /* Compares two BigNum_stt * integer. *****************************************/
  int32_t CompareBig(const BigNum_stt *P_pInput1BN, \
                     const BigNum_stt *P_pInput2BN);

  /*  If x==0 returns 1. ********************************************************/
  int32_t CountBits (const BigNum_stt *P_pInputBN);

  int32_t CountBytes(const BigNum_stt * P_pInputBN);

  /* Returns the value of the pos-th bit of x. **********************************/
  uint16_t GiveBit  (const BigNum_stt *P_pBN, \
                     uint32_t P_pos);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_BN_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
