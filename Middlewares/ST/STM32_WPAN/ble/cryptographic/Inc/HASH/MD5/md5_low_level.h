/**
  ******************************************************************************
  * @file    md5_low_level.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   MD5 core functions
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
#ifndef __MD5_LOW_LEVEL_H__
#define __MD5_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /* MD5 - Initialize new context */
  void crl_u_MD5_Init   (MD5ctx_stt *P_pMD5ctx);
  /* MD5 Update function */
  void crl_u_MD5_Update (MD5ctx_stt *P_pMD5ctx,  \
                         const uint8_t *P_pInput, \
                         uint32_t P_inputSize);
  /* MD5 finalization function */
  void crl_u_MD5_Finish (MD5ctx_stt *P_pMD5ctx, \
                         uint8_t *P_pDigest);


#ifdef __cplusplus
}
#endif

#endif  /*__MD5_LOW_LEVEL_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
