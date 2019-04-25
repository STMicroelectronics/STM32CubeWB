/**
  ******************************************************************************
  * @file    rsa_low_level.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides Low Level RSA operations
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
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_RSA_LOW_LEVEL_H__
#define __CRL_RSA_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup RSAlowlevel
    * @{
    */

#define RSAEP(P_pPubKey, P_pInput, P_pOutput, P_pMemBuf) RSAVP1((P_pPubKey),(P_pInput),(P_pOutput), (P_pMemBuf))   /*!< Wrapper for RSAVP1*/
#define RSADP(P_pPrivKey, P_pInput, P_pOutput, P_pMemBuf) RSASP1((P_pPrivKey),(P_pInput),(P_pOutput), (P_pMemBuf)) /*!< Wrapper for RSADP*/


  int32_t RSASP1(const RSAprivKey_stt *P_pPrivKey, const uint8_t * P_pEM, uint8_t *P_pOutput, membuf_stt *P_pMemBuf);
  int32_t RSAVP1(const RSApubKey_stt *P_pPubKey, const uint8_t * P_pSignature, uint8_t *P_pOutput, membuf_stt *P_pMemBuf);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_RSA_LOW_LEVEL_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
