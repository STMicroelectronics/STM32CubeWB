/**
  ******************************************************************************
  * @file    sha256_sha224_transform.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   SHA-256 Update function
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
#ifndef __SHA256_TRANSFORM_H__
#define __SHA256_TRANSFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported functions ------------------------------------------------------- */
  /* SHA256 Update */
  void SHA256Update(HASHctx_stt* P_pSHA256ctx, \
                    const uint8_t* P_pInput,  \
                    uint32_t P_inputSize);

#ifdef __cplusplus
}
#endif

#endif  /*__SHA256_TRANSFORM_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
