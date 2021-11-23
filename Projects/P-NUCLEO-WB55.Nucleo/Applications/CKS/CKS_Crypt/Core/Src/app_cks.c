/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file   CRYP_CKS/src/app_cks.c
* @author  MCD Application Team
* @brief   This example provides an example of CKS feature.
*          This feature allows to store AES keys in CPU2 area.
           Keys cannot be accessed by user application running on CPU1
           They are loaded when requested by CPU2 in the unreadable key register of AES1 IP
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_cks.h"
/* Next file is used when keys are provisionned by STM32CubeProgrammer */
#include "Keys_index.h"

#define AES_TEXT_SIZE 16
#define TIMEOUT_VALUE 0xFF

/* WRITE_KEYS will call API to store customer Key
* Alternativ is the use of STM32CubeProgrammer -wusrkey option
* CKS_LOCK_TEST show how to prevent the re-use key
*/
 #define WRITE_KEYS
 //#define CKS_LOCK_TEST

/* if WRITE_KEYS is enabled these indexes are overwritten */
uint8_t key_simple_128_idx=KEY_SIMPLE_128_IDX;
uint8_t key_simple_256_idx=KEY_SIMPLE_256_IDX;
uint8_t key_encrypted_128_idx=KEY_ENCRYPTED_128_IDX;
uint8_t key_encrypted_256_idx=KEY_ENCRYPTED_256_IDX;

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp1; /* AES1 IP is used */

#ifdef WRITE_KEYS
/* Master Key always set in index=0 */
static const uint8_t pKeyMaster[16] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};

/* 128 bits keys. In clear format and encrypted by master key */
static const uint8_t pKeySimple_128[16] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
static const uint8_t pKeyEncrypted_128[16] = {
  0x5f,0x95, 0x4f,0xc4,0xf6, 0x8b, 0x74, 0xba,
  0x4c, 0x1f, 0x90, 0x57, 0xb0, 0xc4, 0x09, 0x8a
};

/* 256 bits keys. In clear format and encrypted by master key */
static const uint8_t pKeySimple_256[32] = {
  0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
  0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
  0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7,
  0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4
};
static const uint8_t pKeyEncrypted_256[32]  = {
  0x14, 0xd6, 0xb1, 0xc2, 0xcb, 0xef, 0xd7, 0xa2,
  0xcc, 0x9b, 0x2b, 0x2f, 0x3c, 0x76, 0x31, 0x37,
  0x73, 0xd7, 0xcb, 0x4d, 0x27, 0x54 ,0x2f, 0x63,
  0x75, 0x4f, 0x70, 0x9b, 0x51, 0x33, 0x9c, 0xc6
};

/* Initialization vectors */
static const uint8_t MasterIV[12] =   {0x00, 0x01, 0x02, 0x03,0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
#endif /* WRITE_KEYS */

uint32_t AESIV[4] =   {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};
/* Plaintext */
uint32_t aPlaintextCBC[AES_TEXT_SIZE] =
{ 0xE2BEC16B ,0x969F402E ,0x117E3DE9 ,0x2A179373 ,
0x578A2DAE ,0x9CAC031E ,0xAC6FB79E ,0x518EAF45 ,
0x461CC830 ,0x11E45CA3 ,0x19C1FBE5 ,0xEF520A1A ,
0x45249FF6 ,0x179B4FDF ,0x7B412BAD ,0x10376CE6};

/*AES_CBC*/ 
/* Expected text: Encrypted Data with AES 128 Mode CBC */
uint32_t aEncryptedtextCBC128[AES_TEXT_SIZE] =
{0xACAB4976 ,0x46B21981 ,0x9B8EE9CE ,0x7D19E912 ,
0x9BCB8650 ,0xEE197250 ,0x3A11DB95 ,0xB2787691 ,
0xB8D6BE73 ,0x3B74C1E3 ,0x9EE61671 ,0x16952222 ,
0xA1CAF13F ,0x09AC1F68 ,0x30CA0E12 ,0xA7E18675};

/* Expected text: Encrypted Data with AES 256 Mode CBC */
uint32_t aEncryptedtextCBC256[AES_TEXT_SIZE] =
{0x044C8CF5 ,0xBAF1E5D6 ,0xFBAB9E77 ,0xD6FB7B5F ,
0x964EFC9C ,0x8D80DB7E ,0x7B779F67 ,0x7D2C70C6 ,
0x6933F239 ,0xCFBAD9A9 ,0x63E230A5 ,0x61142304 ,
0xE205EBB2 ,0xFCE99BC3 ,0x07196CDA ,0x1B9D6A8C};

/* Used for storing the encrypted text */
uint32_t aEncryptedtext[AES_TEXT_SIZE];
/* Used for storing the decrypted text */
uint32_t aDecryptedtext[AES_TEXT_SIZE];

/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef CKS_SimpleKeys(void);
static HAL_StatusTypeDef CKS_EncryptedKeys(void);
static int data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size);

/**
* @brief  Test Encryption and decryption with keys in clear format or encrypted by master key
* @retval HAL_StatusTypeDef
*/

HAL_StatusTypeDef CKS_Test (void)
{ 
  if (HAL_ERROR==CKS_SimpleKeys()) return(HAL_ERROR);
  
  if (HAL_ERROR==CKS_EncryptedKeys()) return(HAL_ERROR);
  
  return(HAL_OK);
}

/**
* @brief  Test Encryption and decryption with keys in clear format
* @brief  Key size of 128 and 256 bits
* @retval HAL_StatusTypeDef
*/
static HAL_StatusTypeDef CKS_SimpleKeys(void)
{
  /**************************************************
  * STORE: Simple keys 128 and 256 bits 
  **************************************************/
#ifdef WRITE_KEYS
  SHCI_C2_FUS_StoreUsrKey_Cmd_Param_t CKS_param;
  // Store simple key 128 bits
  CKS_param.KeyType=KEYTYPE_SIMPLE;
  CKS_param.KeySize=16; // En byte
  memcpy(CKS_param.KeyData, (uint8_t *) pKeySimple_128, 16);
  SHCI_C2_FUS_StoreUsrKey(&CKS_param, &key_simple_128_idx );
  printf("Key index:%i\r\n", key_simple_128_idx);
  
  // Store key 256 bits
  CKS_param.KeyType=KEYTYPE_SIMPLE;
  CKS_param.KeySize=32; // En byte
  memcpy(CKS_param.KeyData, (uint8_t *) pKeySimple_256, 32);
  SHCI_C2_FUS_StoreUsrKey(&CKS_param, &key_simple_256_idx );
  printf("Key index:%i\r\n", key_simple_256_idx);
#endif
  /**************************************************
  * TEST : SIMPLE KEYS. Encryption and edcryption
  **************************************************/
  /* Load 128 bit key with CKS */
  hcryp1.Instance = AES1;
  
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
  hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
  hcryp1.Init.Algorithm     = CRYP_AES_CBC;
  hcryp1.Init.pKey          = NULL; /* Key will be provided by CKS service */
  hcryp1.Init.pInitVect     = AESIV;
  
  if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* Key is loaded in AES1 Key register by CPU2*/ 
  SHCI_C2_FUS_LoadUsrKey(key_simple_128_idx); 
  
  /* Start encrypting aPlaintextECB, the cypher data is available in aEncryptedtext */
  if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  } 
  else 
  {  /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aEncryptedtext, aEncryptedtextCBC128, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  
#ifdef CKS_LOCK_TEST
  /* Locks use of previous key */
  SHCI_C2_FUS_LockUsrKey(key_simple_128_idx);
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  /* Next command shall do nothing*/
  SHCI_C2_FUS_LoadUsrKey(key_simple_128_idx);
  /* Next decryption based on key_simple_128_idx will fail */
#endif /* CKS_LOCK_TEST */
  
  /* Decryption */
  /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCBC128, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  /* DeInit */
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* Load 256 bit key with CKS */
  hcryp1.Instance = AES1;
  hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
  hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
  hcryp1.Init.pKey          = NULL; /* Key will be provided by CKS service */
  hcryp1.Init.Algorithm     = CRYP_AES_CBC;
  hcryp1.Init.pInitVect     = AESIV;
  
  if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  SHCI_C2_FUS_LoadUsrKey(key_simple_256_idx);
  
  /* Encrypt 256bits */
  if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {  
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aEncryptedtext, aEncryptedtextCBC256, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCBC256, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  
  
  /* DeInit */
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK;
  
}

static HAL_StatusTypeDef CKS_EncryptedKeys(void)
{
  
  
  /**************************************************
  * STORE: Master key, encrypted keys
  **************************************************/
#ifdef WRITE_KEYS
  uint8_t key_idx_master;
  SHCI_C2_FUS_StoreUsrKey_Cmd_Param_t CKS_param;
  CKS_param.KeyType=KEYTYPE_MASTER;
  CKS_param.KeySize=16; // En byte
  memcpy(CKS_param.KeyData, (uint8_t *) pKeyMaster, 16);
  SHCI_C2_FUS_StoreUsrKey(&CKS_param, &key_idx_master );
  printf("Key index:%i\r\n", key_idx_master);
  
  // Store encrypted key 128 bits
  CKS_param.KeyType=KEYTYPE_ENCRYPTED;
  CKS_param.KeySize=16; // En byte
  memcpy(CKS_param.KeyData, (uint8_t *) pKeyEncrypted_128, 16);
  memcpy((uint8_t *)(CKS_param.KeyData+16), (uint8_t *) MasterIV, 12);
  SHCI_C2_FUS_StoreUsrKey(&CKS_param, &key_encrypted_128_idx );
  printf("Key index:%i\r\n", key_encrypted_128_idx);
  
  // Store encrypted key 256 bits
  CKS_param.KeyType=KEYTYPE_ENCRYPTED;
  CKS_param.KeySize=32; // En byte
  memcpy(CKS_param.KeyData, (uint8_t *) pKeyEncrypted_256 , 32);
  memcpy((uint8_t *)(CKS_param.KeyData+32), (uint8_t *) MasterIV, 12);
  SHCI_C2_FUS_StoreUsrKey(&CKS_param, &key_encrypted_256_idx );
  printf("Key index:%i\r\n", key_encrypted_256_idx);
#endif  
  /***************************************************************
  * TEST : SIMPLE KEYS. Encryption and edcryption with 128bit key
  ****************************************************************/  
  /* Load 128 bit key with CKS */
  hcryp1.Instance = AES1;
  
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
  hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
  hcryp1.Init.Algorithm     = CRYP_AES_CBC;
  hcryp1.Init.pKey          = NULL; /* Key will be provided by CKS service */
  hcryp1.Init.pInitVect     = AESIV;
  
  if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  SHCI_C2_FUS_LoadUsrKey(key_encrypted_128_idx);
  
  /* Encryption */
  /* Start encrypting aPlaintextECB, the cypher data is available in aEncryptedtext */
  if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  } 
  else 
  {  /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aEncryptedtext, aEncryptedtextCBC128, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  /* Decryption */
  /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCBC128, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  
  /* DeInit */
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /***************************************************************
  * TEST : SIMPLE KEYS. Encryption and edcryption with 256bit key
  ****************************************************************/  
  /* Load 256 bit key with CKS */
  hcryp1.Instance = AES1;
  hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
  hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
  hcryp1.Init.pKey          = NULL; /* Key will be provided by CKS service */
  hcryp1.Init.Algorithm     = CRYP_AES_CBC;
  hcryp1.Init.pInitVect     = AESIV;
  
  if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  SHCI_C2_FUS_LoadUsrKey(key_encrypted_256_idx);
  
  /* Encrypt 256bits */
  if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {  
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aEncryptedtext, aEncryptedtextCBC256, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCBC256, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    return HAL_ERROR;
  }
  else
  {
    /* Check the encrypted text with the expected one *************************/ 
    if (data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE)!=0)
    { 
      return HAL_ERROR;
    }
  }
  
  
  /* DeInit */
  if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK; 
}

/**
* @brief  buffer data comparison
* @param
* @retval None
*/
static int data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size)
{
  /*  Before starting a new process, you need to check the current state of the peripheral;
  if it is busy you need to wait for the end of current transfer before starting a new one.
  For simplicity reasons, this example is just waiting till the end of the
  process, but application may perform other tasks while transfer operation
  is ongoing. */
  while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY)
  {
  }
  return(memcmp(EncryptedText, RefText, Size*4));
  
}


