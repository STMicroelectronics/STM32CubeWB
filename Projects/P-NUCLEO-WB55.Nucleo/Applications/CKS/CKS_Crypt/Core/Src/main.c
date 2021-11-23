/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    CRYP_CKS/src/main.c
* @author  MCD Application Team
* @brief   CKS examples
*
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
#include "main.h"
#include "app_common.h"
#include "app_cks.h"
#include "tl.h"
#include "shci_tl.h"

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Reset_IPCC( void );

/* IPCC section */
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4U*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4U))
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U]; //CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE=16

/* Private functions prototypes-----------------------------------------------*/
static void appe_Tl_Init( void );
static volatile uint8_t FLAG_WAIT_CPU2_RDY=1;
static volatile uint8_t FLAG_WAIT_RESP=0;
static void Error_Handler(void);

int main(void)
{
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
 
  /* MCU Configuration--------------------------------------------------------*/
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  Reset_IPCC();
  
  /* Configure the system clock */
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Initialize all transport layers and wakeup CPU2 */
  appe_Tl_Init();	

  if (HAL_OK==CKS_Test())
  {
    BSP_LED_On(LED1); // Test OK.
  }
  else
  {
    BSP_LED_On(LED3); // Test fails
  }
  /* Infinite loop */
  while(1)
  {
  }
}

/*************************************************************
*
* LOCAL FUNCTIONS
*
*************************************************************/

static void appe_Tl_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
  /**< Reference table initialization */
  TL_Init();
  
  /**< System channel initialization */
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = NULL;
  shci_init(NULL, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();
  
  // Check channel 2
  while(FLAG_WAIT_CPU2_RDY==1); 
  return;
}

/*************************************************************
*
* MailBox FUNCTIONS
*
*************************************************************/
void shci_notify_asynch_evt(void* pdata)
{
  FLAG_WAIT_CPU2_RDY=0;
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  FLAG_WAIT_RESP=1;
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  while(FLAG_WAIT_RESP==0);
  FLAG_WAIT_RESP=0;
  return;
}

/**
* @brief System Clock Configuration
* @retval None
*/
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI1
    |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
    |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
      |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

}

/*************************************************************
*
* LOCAL FUNCTIONS
*
*************************************************************/


static void Reset_IPCC( void )
{
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);
  
  LL_C1_IPCC_ClearFlag_CHx(
                           IPCC,
                           LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                             | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  LL_C2_IPCC_ClearFlag_CHx(
                           IPCC,
                           LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                             | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  LL_C1_IPCC_DisableTransmitChannel(
                                    IPCC,
                                    LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                                      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  LL_C2_IPCC_DisableTransmitChannel(
                                    IPCC,
                                    LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                                      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  LL_C1_IPCC_DisableReceiveChannel(
                                   IPCC,
                                   LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                                     | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  LL_C2_IPCC_DisableReceiveChannel(
                                   IPCC,
                                   LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
                                     | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);
  
  return;
}

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
static void Error_Handler(void)
{
  while(1);
}
