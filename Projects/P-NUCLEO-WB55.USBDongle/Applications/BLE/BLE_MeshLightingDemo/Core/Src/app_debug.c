/**
 ******************************************************************************
 * @file    app_debug.c
 * @author  MCD Application Team
 * @brief   Debug capabilities
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "app_debug.h"
#include "utilities_common.h"
#include "shci.h"
#include "tl.h"
#include "dbg_trace.h"

/* Private typedef -----------------------------------------------------------*/
typedef PACKED_STRUCT
{
  GPIO_TypeDef* port;
  uint16_t pin;
  uint8_t enable;
  uint8_t reserved;
} APPD_GpioConfig_t;

typedef PACKED_STRUCT
{
  uint8_t thread_config;
  uint8_t ble_config;
  uint8_t mac_802_15_4;
  uint8_t reserved;
} APPD_TracesConfig_t;

typedef PACKED_STRUCT
{
  uint8_t ble_dtb_cfg;
  uint8_t reserved[3];
} APPD_GeneralConfig_t;

/* Private defines -----------------------------------------------------------*/
#define GPIO_NBR_OF_RF_SIGNALS                  9
#define GPIO_CFG_NBR_OF_FEATURES                32
#define NBR_OF_TRACES_CONFIG_PARAMETERS         4
#define NBR_OF_GENERAL_CONFIG_PARAMETERS        4

/**
 * THIS SHALL BE SET TO A VALUE DIFFERENT FROM 0 ONLY ON REQUEST FROM ST SUPPORT
 */
#define BLE_DTB_CFG     0

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static APPD_TracesConfig_t APPD_TracesConfig={0, 0, 0, 0};
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static APPD_GeneralConfig_t APPD_GeneralConfig={BLE_DTB_CFG, {0, 0, 0}};

/**
 * THE DEBUG ON GPIO FOR CPU2 IS INTENDED TO BE USED ONLY ON REQUEST FROM ST SUPPORT
 * It provides timing information on the CPU2 activity.
 * All configuration of (port, pin) is supported for each features and can be selected by the user
 * depending on the availability
 */
static const APPD_GpioConfig_t aGpioConfigList[GPIO_CFG_NBR_OF_FEATURES] =
{
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
/* USER DEFINED IN M0 */
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
    { GPIOA, LL_GPIO_PIN_0, 0, 0},
};

/**
 * THE DEBUG ON GPIO FOR CPU2 IS INTENDED TO BE USED ONLY ON REQUEST FROM ST SUPPORT
 * This table is relevant only for BLE
 * It provides timing information on BLE RF activity.
 * New signals may be allocated at any location when requested by ST
 * The GPIO allocated to each signal depend on the BLE_DTB_CFG value and cannot be changed
 */
#if( BLE_DTB_CFG == 7)
static const APPD_GpioConfig_t aRfConfigList[GPIO_NBR_OF_RF_SIGNALS] =
{
    { GPIOB, LL_GPIO_PIN_2, 0, 0},      /* DTB10 */
    { GPIOB, LL_GPIO_PIN_7, 0, 0},      /* DTB11 */
    { GPIOA, LL_GPIO_PIN_8, 0, 0},      /* DTB12 */
    { GPIOA, LL_GPIO_PIN_9, 0, 0},      /* DTB13 */
    { GPIOA, LL_GPIO_PIN_10, 0, 0},     /* DTB14 */
    { GPIOA, LL_GPIO_PIN_11, 0, 0},     /* DTB15 */
    { GPIOB, LL_GPIO_PIN_8, 0, 0},      /* DTB16 */
    { GPIOB, LL_GPIO_PIN_11, 0, 0},     /* DTB17 */
    { GPIOB, LL_GPIO_PIN_10, 0, 0},     /* DTB18 */
};
#endif

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APPD_SetCPU2GpioConfig( void );
static void APPD_BleDtbCfg( void );

/* Functions Definition ------------------------------------------------------*/
void APPD_Init( void )
{
#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();
  HAL_DBGMCU_EnableDBGStopMode();

  /***************** ENABLE DEBUGGER *************************************/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);

#else
  GPIO_InitTypeDef gpio_config = {0};

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_ANALOG;

  gpio_config.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &gpio_config);
  __HAL_RCC_GPIOA_CLK_DISABLE();

  gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_3;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &gpio_config);
  __HAL_RCC_GPIOB_CLK_DISABLE();

  HAL_DBGMCU_DisableDBGSleepMode();
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

#endif /* (CFG_DEBUGGER_SUPPORTED == 1) */

#if(CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif

  APPD_SetCPU2GpioConfig( );
  APPD_BleDtbCfg( );

  return;
}

void APPD_EnableCPU2( void )
{
  SHCI_C2_DEBUG_Init_Cmd_Packet_t DebugCmdPacket =
  {
    {{0,0,0}},                            /**< Does not need to be initialized */
    {(uint8_t *)aGpioConfigList,
    (uint8_t *)&APPD_TracesConfig,
    (uint8_t *)&APPD_GeneralConfig,
    GPIO_CFG_NBR_OF_FEATURES,
    NBR_OF_TRACES_CONFIG_PARAMETERS,
    NBR_OF_GENERAL_CONFIG_PARAMETERS}
  };

  /**< Traces channel initialization */
  TL_TRACES_Init( );

  /** GPIO DEBUG Initialization */
  SHCI_C2_DEBUG_Init( &DebugCmdPacket  );

  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void APPD_SetCPU2GpioConfig( void )
{
  GPIO_InitTypeDef gpio_config = {0};
  uint8_t local_loop;
  uint16_t gpioa_pin_list;
  uint16_t gpiob_pin_list;
  uint16_t gpioc_pin_list;

  gpioa_pin_list = 0;
  gpiob_pin_list = 0;
  gpioc_pin_list = 0;

  for(local_loop = 0 ; local_loop < GPIO_CFG_NBR_OF_FEATURES; local_loop++)
  {
    if( aGpioConfigList[local_loop].enable != 0)
    {
      switch((uint32_t)aGpioConfigList[local_loop].port)
      {
        case (uint32_t)GPIOA:
            gpioa_pin_list |= aGpioConfigList[local_loop].pin;
          break;

        case (uint32_t)GPIOB:
            gpiob_pin_list |= aGpioConfigList[local_loop].pin;
          break;

        case (uint32_t)GPIOC:
            gpioc_pin_list |= aGpioConfigList[local_loop].pin;
          break;

        default:
          break;
      }
    }
  }

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  if(gpioa_pin_list != 0)
  {
    gpio_config.Pin = gpioa_pin_list;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_C2GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpio_config);
    HAL_GPIO_WritePin(GPIOA, gpioa_pin_list, GPIO_PIN_RESET);
  }

  if(gpiob_pin_list != 0)
  {
    gpio_config.Pin = gpiob_pin_list;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_C2GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &gpio_config);
    HAL_GPIO_WritePin(GPIOB, gpiob_pin_list, GPIO_PIN_RESET);
  }

  if(gpioc_pin_list != 0)
  {
    gpio_config.Pin = gpioc_pin_list;
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_C2GPIOC_CLK_ENABLE();
    HAL_GPIO_Init(GPIOC, &gpio_config);
    HAL_GPIO_WritePin(GPIOC, gpioc_pin_list, GPIO_PIN_RESET);
  }
  
  return;
}

static void APPD_BleDtbCfg( void )
{
#if (BLE_DTB_CFG != 0)
  GPIO_InitTypeDef gpio_config = {0};
  uint8_t local_loop;
  uint16_t gpioa_pin_list;
  uint16_t gpiob_pin_list;

  gpioa_pin_list = 0;
  gpiob_pin_list = 0;

  for(local_loop = 0 ; local_loop < GPIO_NBR_OF_RF_SIGNALS; local_loop++)
  {
    if( aRfConfigList[local_loop].enable != 0)
    {
      switch((uint32_t)aRfConfigList[local_loop].port)
      {
        case (uint32_t)GPIOA:
            gpioa_pin_list |= aRfConfigList[local_loop].pin;
          break;

        case (uint32_t)GPIOB:
            gpiob_pin_list |= aRfConfigList[local_loop].pin;
          break;

        default:
          break;
      }
    }
  }

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_AF_PP;
  gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.Alternate = GPIO_AF6_RF_DTB7;

  if(gpioa_pin_list != 0)
  {
    gpio_config.Pin = gpioa_pin_list;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_C2GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpio_config);
  }

  if(gpiob_pin_list != 0)
  {
    gpio_config.Pin = gpiob_pin_list;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_C2GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &gpio_config);
  }
#endif

  return;
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
*************************************************************/
#if(CFG_DEBUG_TRACE != 0)
void DbgOutputInit( void )
{
  HW_UART_Init(CFG_DEBUG_TRACE_UART);
  return;
}


void DbgOutputTraces(  uint8_t *p_data, uint16_t size, void (*cb)(void) )
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);

  return;
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
