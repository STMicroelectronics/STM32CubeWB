/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the CRYP
  *          peripheral to encrypt and decrypt data using AES algorithm in 
  *          chaining modes (ECB, CBC, CTR) and with all key sizes.
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Private define ------------------------------------------------------------*/


#define AES_TEXT_SIZE     16

#define ECB               1
#define CBC               2
#define CTR               3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp1;
__ALIGN_BEGIN static const uint32_t pKeyAES1[4] __ALIGN_END = {
                            0x2B7E1516,0x28AED2A6,0xABF71588,0x09CF4F3C};
DMA_HandleTypeDef hdma_aes1_in;
DMA_HandleTypeDef hdma_aes1_out;

/* USER CODE BEGIN PV */

/* Key size 256 bits */
 uint32_t aAES256key[8] = {0x603DEB10, 0x15CA71BE, 0x2B73AEF0, 0x857D7781,
                          0x1F352C07, 0x3B6108D7, 0x2D9810A3, 0x0914DFF4};

/* Initialization vector */
uint32_t AESIV[4] =   {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};
uint32_t AESIV_CTR[4] =   {0xF0F1F2F3 , 0xF4F5F6F7 , 0xF8F9FAFB , 0xFCFDFEFF};

/* Plaintext */
uint32_t aPlaintextECB[AES_TEXT_SIZE] =
                        { 0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,
                          0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                          0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                          0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

uint32_t aPlaintextCBC[AES_TEXT_SIZE] =
                        { 0xE2BEC16B ,0x969F402E ,0x117E3DE9 ,0x2A179373 ,
                          0x578A2DAE ,0x9CAC031E ,0xAC6FB79E ,0x518EAF45 ,
                          0x461CC830 ,0x11E45CA3 ,0x19C1FBE5 ,0xEF520A1A ,
                          0x45249FF6 ,0x179B4FDF ,0x7B412BAD ,0x10376CE6};

uint32_t aPlaintextCTR[AES_TEXT_SIZE] =
                        { 0x477D83D6 ,0x69F90274 ,0x887EBC97 ,0x54E8C9CE ,
                          0xEA51B475 ,0x3935C078 ,0x35F6ED79 ,0x8A71F5A2 ,
                          0x6238130C ,0x88273AC5 ,0x9883DFA7 ,0xF74A5058,
                          0xA224F96F ,0xE8D9F2FB ,0xDE82D4B5 ,0x08EC3667};

/* AES_ECB */ 
/* Expected text: Encrypted Data with AES 128 Mode ECB */
uint32_t aEncryptedtextECB128[AES_TEXT_SIZE] =
                          {0x3AD77BB4 ,0x0D7A3660 ,0xA89ECAF3 ,0x2466EF97 ,
                           0xF5D3D585 ,0x03B9699D ,0xE785895A ,0x96FDBAAF ,
                           0x43B1CD7F ,0x598ECE23 ,0x881B00E3 ,0xED030688 ,
                           0x7B0C785E ,0x27E8AD3F ,0x82232071 ,0x04725DD4};

/*AES_CBC*/ 
/* Expected text: Encrypted Data with AES 128 Mode CBC */
uint32_t aEncryptedtextCBC128[AES_TEXT_SIZE] =
                           {0xACAB4976 ,0x46B21981 ,0x9B8EE9CE ,0x7D19E912 ,
                           0x9BCB8650 ,0xEE197250 ,0x3A11DB95 ,0xB2787691 ,
                           0xB8D6BE73 ,0x3B74C1E3 ,0x9EE61671 ,0x16952222 ,
                           0xA1CAF13F ,0x09AC1F68 ,0x30CA0E12 ,0xA7E18675};

/*AES_CTR*/  
/* Expected text: Encrypted Data with AES 128 Mode CTR */
uint32_t aEncryptedtextCTR128[AES_TEXT_SIZE] =
                          {0x8986B2E1 ,0x64C7046D ,0x2616F7D8 ,0x736DB099 ,
                          0xD66F6019 ,0xFFBF0E9E ,0xDE18E861 ,0xFFBFFF9D ,
                          0x7CFB275A ,0x7ACBABDB ,0x4090F2DA ,0xD57C0DB0 ,
                          0x5BB8C078 ,0x8BC07DF4 ,0x050E849E ,0x773900CF};

/* Expected text: Encrypted Data with AES 256 Mode ECB */
uint32_t aEncryptedtextECB256[AES_TEXT_SIZE] =
                                   {0xF3EED1BD ,0xB5D2A03C ,0x064B5A7E ,0x3DB181F8 ,
                                    0x591CCB10 ,0xD410ED26 ,0xDC5BA74A ,0x31362870 ,
                                    0xB6ED21B9 ,0x9CA6F4F9 ,0xF153E7B1 ,0xBEAFED1D ,
                                    0x23304B7A ,0x39F9F3FF ,0x067D8D8F ,0x9E24ECC7};

/* Expected text: Encrypted Data with AES 256 Mode CBC */
uint32_t aEncryptedtextCBC256[AES_TEXT_SIZE] =
                         {0x044C8CF5 ,0xBAF1E5D6 ,0xFBAB9E77 ,0xD6FB7B5F ,
                          0x964EFC9C ,0x8D80DB7E ,0x7B779F67 ,0x7D2C70C6 ,
                          0x6933F239 ,0xCFBAD9A9 ,0x63E230A5 ,0x61142304 ,
                          0xE205EBB2 ,0xFCE99BC3 ,0x07196CDA ,0x1B9D6A8C};

/* Expected text: Encrypted Data with AES 256 Mode CTR */
uint32_t aEncryptedtextCTR256[AES_TEXT_SIZE] =
                         {0xC8C37806 ,0xA591EAEE ,0x20AFE5ED ,0x144BCFDD ,
                          0x53C7C22F ,0x59AD46B2 ,0x09972153 ,0xA3AF5353 ,
                          0x5B0C90D4 ,0x3297BC45 ,0x5DE80E17 ,0xB11921B4 ,
                          0xB1A393FB ,0x65B55E6D ,0x10BB43C8,0x65829EA2};

/* Used for storing the encrypted text */
uint32_t aEncryptedtext[AES_TEXT_SIZE];

/* Used for storing the decrypted text */
uint32_t aDecryptedtext[AES_TEXT_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_DMA_Init(void);
static void MX_AES1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

static void Display_PlainData_ECB(uint32_t datalength);
static void Display_PlainData_CBC(uint32_t datalength);
static void Display_PlainData_CTR(uint32_t datalength);
static void Display_CypherData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
void SystemClock_Config(void);
void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size);
/* Private functions ---------------------------------------------------------*/
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
extern void initialise_monitor_handles(void);
#endif

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
  initialise_monitor_handles(); 
#endif
#if (USE_VCP_CONNECTION == 1)
  COM_InitTypeDef COM_Init;
#endif

  /* STM32WBxx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_AES1_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
#if (USE_VCP_CONNECTION == 1)
  /* Configure COM port */
  COM_Init.BaudRate   = 115200;
  COM_Init.WordLength = COM_WORDLENGTH_8B;
  COM_Init.StopBits   = COM_STOPBITS_1;
  COM_Init.Parity     = COM_PARITY_NONE;
  COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &COM_Init) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if (BSP_COM_SelectLogPort(COM1) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
#endif

  /*##- Configure the CRYP peripheral ######################################*/
  /* Set the common CRYP parameters */
  hcryp1.Instance = AES1;
 


  /* Display Plain Data*/
  Display_PlainData_ECB(AES_TEXT_SIZE);
    
  /* Display Cypher Data*/
  Display_CypherData(AES_TEXT_SIZE);




    /******************************************************************************/
    /*                             AES mode ECB                                   */
    /******************************************************************************/

    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }

    /*=====================================================
        Encryption ECB mode
    ======================================================*/

    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_32B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm     = CRYP_AES_ECB;
    hcryp1.Init.pKey          = (uint32_t *)pKeyAES1;
    hcryp1.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;  

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    /* Start encrypting aPlaintextECB, the cypher data is available in aEncryptedtext */
    if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextECB, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(ECB, 128, AES_TEXT_SIZE);
    }
    else
    {
      /* Processing Error */
      Error_Handler();
    }
    
    /* Compare the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextECB128, AES_TEXT_SIZE);
  

    /*****************  AES 256   ****************/
    hcryp1.Init.DataType      = CRYP_DATATYPE_32B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Encrypt_IT(&hcryp1, aPlaintextECB, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(ECB, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextECB256, AES_TEXT_SIZE);
    
    


    /*=====================================================
        Decryption ECB mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_32B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm     = CRYP_AES_ECB;
    hcryp1.Init.pKey          = (uint32_t *)pKeyAES1;

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextECB128, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(ECB, 128, AES_TEXT_SIZE);
    }
    else
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
  data_cmp(aDecryptedtext, aPlaintextECB, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/
    hcryp1.Init.DataType      = CRYP_DATATYPE_32B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Decrypt_IT(&hcryp1, aEncryptedtextECB256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(ECB, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintextECB, AES_TEXT_SIZE);
    
    


    /******************************************************************************/
    /*                             AES mode CBC                                   */
    /******************************************************************************/

    /* Display Plain Data*/
    Display_PlainData_CBC(AES_TEXT_SIZE);
    /*=====================================================
        Encryption CBC mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm     = CRYP_AES_CBC;
    hcryp1.Init.pKey          = (uint32_t *)pKeyAES1;
    hcryp1.Init.pInitVect     = AESIV;

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    /* Start encrypting aPlaintextECB, the cypher data is available in aEncryptedtext */
    if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(CBC, 128, AES_TEXT_SIZE);
    }
    else  
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextCBC128, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Encrypt_IT(&hcryp1, aPlaintextCBC, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(CBC, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextCBC256, AES_TEXT_SIZE);
    
    


    /*=====================================================
        Decryption CBC mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm     = CRYP_AES_CBC;
    hcryp1.Init.pKey          = (uint32_t *)pKeyAES1;
    hcryp1.Init.pInitVect     = AESIV;

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCBC128, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(CBC, 128, AES_TEXT_SIZE);
    }
    else   
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    hcryp1.Init.DataType      = CRYP_DATATYPE_8B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Decrypt_IT(&hcryp1, aEncryptedtextCBC256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(CBC, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintextCBC, AES_TEXT_SIZE);
    
    


    /******************************************************************************/
    /*                             AES mode CTR                                   */
    /******************************************************************************/

    /* Display Plain Data*/
    Display_PlainData_CTR(AES_TEXT_SIZE);

    /*=====================================================
        Encryption CTR mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_1B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm = CRYP_AES_CTR;
    hcryp1.Init.pKey      = (uint32_t *)pKeyAES1;
    hcryp1.Init.pInitVect = AESIV_CTR; 

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    /* Start encrypting aPlaintextCTR, the cypher data is available in aEncryptedtext */
    if (HAL_CRYP_Encrypt(&hcryp1, aPlaintextCTR, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(CTR, 128, AES_TEXT_SIZE);
    }
    else     
    {
      /* Processing Error */
      Error_Handler();
    }
    
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextCTR128, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    hcryp1.Init.DataType      = CRYP_DATATYPE_1B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Encrypt_IT(&hcryp1, aPlaintextCTR, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(CTR, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextCTR256, AES_TEXT_SIZE);
    
    

    /*=====================================================
        Decryption CTR mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&hcryp1) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    hcryp1.Init.DataType      = CRYP_DATATYPE_1B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_128B;
    hcryp1.Init.Algorithm = CRYP_AES_CTR;
    hcryp1.Init.pKey      = (uint32_t *)pKeyAES1;
    hcryp1.Init.pInitVect = AESIV_CTR;

    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    /* Start decrypting aCiphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYP_Decrypt(&hcryp1, aEncryptedtextCTR128, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(CTR, 128, AES_TEXT_SIZE);
    }
    else  
    {
      /* Processing Error */
      Error_Handler();

    }

    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintextCTR, AES_TEXT_SIZE);
  

    /*****************  AES 256   ****************/
    hcryp1.Init.DataType      = CRYP_DATATYPE_1B;
    hcryp1.Init.KeySize       = CRYP_KEYSIZE_256B;  
    hcryp1.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYP_Decrypt_IT(&hcryp1, aEncryptedtextCTR256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(CTR, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintextCTR, AES_TEXT_SIZE);
    
    



  printf("===================================================\n ");
  printf("\n\r ECB, CBC and CTR encryptions/decryptions done.\n ");
  printf("No issue detected.\n ");
  
  BSP_LED_On(LED1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)

  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief AES1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES1_Init(void)
{

  /* USER CODE BEGIN AES1_Init 0 */

  /* USER CODE END AES1_Init 0 */

  /* USER CODE BEGIN AES1_Init 1 */

  /* USER CODE END AES1_Init 1 */
  hcryp1.Instance = AES1;
  hcryp1.Init.DataType = CRYP_DATATYPE_32B;
  hcryp1.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp1.Init.pKey = (uint32_t *)pKeyAES1;
  hcryp1.Init.Algorithm = CRYP_AES_ECB;
  hcryp1.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp1.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  if (HAL_CRYP_Init(&hcryp1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES1_Init 2 */

  /* USER CODE END AES1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/* USER CODE BEGIN 4 */



/**
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData_ECB(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aPlaintextECB;

  printf("\n\r =============================================================\n\r");
  printf(" ================= Crypt Using HW Crypto  ====================\n\r");
  printf(" =============================================================\n\r");
  printf(" -----------------------------------------------\n\r");
  printf(" Plain Data (Input data for AES ECB encryption):\n\r");
  printf(" -----------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData_CBC(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aPlaintextCBC;

  printf("\n\r =============================================================\n\r");
  printf(" ================= Crypt Using HW Crypto  ====================\n\r");
  printf(" =============================================================\n\r");
  printf(" -----------------------------------------------\n\r");
  printf(" Plain Data (Input data for AES CBC encryption):\n\r");
  printf(" -----------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData_CTR(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aPlaintextCTR;

  printf("\n\r =============================================================\n\r");
  printf(" ================= Crypt Using HW Crypto  ====================\n\r");
  printf(" =============================================================\n\r");
  printf(" -----------------------------------------------\n\r");
  printf(" Plain Data (Input data for AES CTR encryption):\n\r");
  printf(" -----------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}


/**
  * @brief  Display Cypher data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_CypherData(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aEncryptedtextECB128;

  printf("\n\r =============================================================\n\r");
  printf(" ------------------------------------------------\n\r");
  printf(" Cypher Data (Input data for AES 128 decryption):\n\r");
  printf(" ------------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Encrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aEncryptedtext;

  printf("\n\r =======================================\n\r");
  printf(" Encrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);

    count++;
    if (count == 16)
    {
      count = 0;
      printf(" Block %ld \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Decrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;
  uint8_t * ptr = (uint8_t *)aDecryptedtext;

  printf("\n\r =======================================\n\r");
  printf(" Decrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength*4; BufferCounter++)
  {
    printf("[0x%02X]", *ptr++);
    count++;

    if (count == 16)
    {
      count = 0;
      printf(" Block %ld \n\r", BufferCounter / 16);
    }
  }
}


/**
  * @brief  buffer data comparison
  * @param
  * @retval None
  */
void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size)
{
  /*  Before starting a new process, you need to check the current state of the peripheral;
      if it is busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      process, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_CRYP_GetState(&hcryp1) != HAL_CRYP_STATE_READY)
  {
  }

  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(EncryptedText, RefText, Size*4) != 0)
  {
    Error_Handler();
  }
  else
  {
    /* Right encryption */
  }
}
  


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  printf("\n\r Error Detected...\n ");
  
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
