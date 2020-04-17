/**
  ******************************************************************************
  * @file    aes.h
  * @author  MCD Application Team
  * @brief   Container for the AES functionalities
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Common/aes_low_level.h"
#include "Common/aes_common.h"


#ifdef INCLUDE_ECB
#include "ECB/aes_ecb.h"
#endif

#ifdef INCLUDE_CBC
#include "CBC/aes_cbc.h"
#endif

#ifdef INCLUDE_CTR
#include "CTR/aes_ctr.h"
#endif

#ifdef INCLUDE_CFB
#include "CFB/aes_cfb.h"
#endif

#ifdef INCLUDE_OFB
#include "OFB/aes_ofb.h"
#endif

#ifdef INCLUDE_XTS
#include "XTS/aes_xts.h"
#endif

#ifdef INCLUDE_GCM
#include "GCM/aes_gcm.h"
#endif

#ifdef INCLUDE_CCM
#include "CCM/aes_ccm.h"
#endif

#ifdef INCLUDE_CMAC
#include "CMAC/aes_cmac.h"
#endif

#ifdef INCLUDE_KEY_WRAP
#include "KEYWRAP/aes_keywrap.h"
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
