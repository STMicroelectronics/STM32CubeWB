/**
  ******************************************************************************
  * @file    stm32wb5mm_dk.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage:
  *          - LEDs, push-buttons and COM ports available on STM32WB5MM-DK
  *            board (MB1292) from STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk.h"

#if (PA7_AUTO_MANAGEMENT == 1)
#include "stm32wb5mm_dk_bus.h"
#endif /* PA7_AUTO_MANAGEMENT == 1*/

#if (USE_BSP_COM_FEATURE == 1)
#if (USE_COM_LOG == 1)

#if defined(__ICCARM__)
#include <LowLevelIOInterface.h>
#endif /* __ICCARM__ */

#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32WB5MM_DK STM32WB5MM-DK
  * @{
  */

/** @defgroup STM32WB5MM_DK_COMMON STM32WB5MM_DK COMMON
  * @brief This file provides set of firmware functions to manage Leds, push-buttons
  *        and COM ports available on STM32WB5MM_DK board from STMicroelectronics.
  * @{
  */

/** @defgroup STM32WB5MM_DK_COMMON_Private_Defines STM32WB5MM-DK COMMON Private Defines
  * @{
  */
#if (USE_BSP_COM_FEATURE == 1)
#if (USE_COM_LOG == 1)
#define COM_POLL_TIMEOUT     1000
#endif /* (USE_COM_LOG == 1) */
#endif /* (USE_BSP_COM_FEATURE == 1) */

#if (USE_COM_LOG == 1)
/**
  * @brief  Redirect console output to COM
  */
#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#elif defined (__CC_ARM) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6 */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#endif /* __ICCARM__ */

#endif /* USE_COM_LOG */

#define DELAY                       1u
#define T_CYCLE_0                   4u
#define T_CYCLE_1                   1u
#define WRITE_COMMAND               0x3Au
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_Private_TypesDefinitions STM32WB5MM-DK COMMON Private Types Definitions
  * @{
  */
typedef void (* BSP_EXTI_LineCallback)(void);
/**
  * @}
  */


/** @defgroup STM32WB5MM_DK_COMMON_Private_FunctionPrototypes STM32WB5MM-DK COMMON Private Function Prototypes
  * @{
  */
static void BUTTON_USER1_EXTI_Callback(void);
static void BUTTON_USER2_EXTI_Callback(void);

#if (USE_BSP_COM_FEATURE == 1)
static void COM1_MspInit(UART_HandleTypeDef *huart);
static void COM1_MspDeInit(UART_HandleTypeDef *huart);
#endif /* (USE_BSP_COM_FEATURE == 1) */

static int32_t PWM_LED_WriteData(aPwmLedGsData_TypeDef aPwmLedGsData);
static void    PWM_LED_SendBit(uint8_t bit);
static void    PWM_LED_SenByte(uint8_t byte);
static void    PWM_LED_Wait(uint32_t NbCycles);

static void TIM17_MspInit(TIM_HandleTypeDef *htim);
static void TIM17_MspDeInit(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_Exported_Variables STM32WB5MM-DK COMMON Exported Variables
  * @{
  */
EXTI_HandleTypeDef hpb_exti[BUTTONn];
#if (USE_BSP_COM_FEATURE == 1)
UART_HandleTypeDef hcom_uart[COMn];
#endif /* USE_BSP_COM_FEATURE */

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_Private_Variables STM32WB5MM-DK COMMON Private Variables
  * @{
  */
static GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {BUTTON_USER1_GPIO_PORT, BUTTON_USER2_GPIO_PORT};

static const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_USER1_PIN, BUTTON_USER2_PIN};

static const IRQn_Type BUTTON_IRQn[BUTTONn] = {BUTTON_USER1_EXTI_IRQn, BUTTON_USER2_EXTI_IRQn};

#if (USE_BSP_COM_FEATURE == 1)
static USART_TypeDef*  COM_USART[COMn] = {COM1_UART};

#if (USE_COM_LOG == 1)
static COM_TypeDef COM_ActiveLogPort = COM1;
#endif /* USE_COM_LOG == 1 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsComMspCbValid[COMn] = {0};
#endif
#endif /* (USE_BSP_COM_FEATURE == 1) */


static TIM_HandleTypeDef PwmLed_TimerHandle = {0};

static __IO uint32_t CycleCount = 0;

static aPwmLedGsData_TypeDef PWM_LED_GSDATA = {0}; 

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
static uint32_t IsPwmLedMspCbValid = 0;
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS == 1 */
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_Exported_Functions STM32WB5MM-DK COMMON Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32WB5MM_DK BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_GetVersion(void)
{
  return ((int32_t)STM32WB5MM_DK_BSP_VERSION);
}

/** @defgroup STM32WB5MM_DK_COMMON_PWM_LED_Functions STM32WB5MM-DK COMMON PWM LED Functions
  * @{
  */

/**
  * @brief  Configure PWM LED Driver.
  * @retval BSP error code
  */
int32_t BSP_PWM_LED_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  
  GPIO_InitTypeDef  gpio_config = {0};
  
  PWM_LED_SDI_GPIO_CLK_ENABLE();
  gpio_config.Pin       = PWM_LED_SDI_GPIO_PIN;
  gpio_config.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_config.Pull      = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_LED_SDI_GPIO_PORT, &gpio_config);
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_RESET);
  
  PWM_LED_SELECT_GPIO_CLK_ENABLE();
  gpio_config.Pin       = PWM_LED_SELECT_GPIO_PIN;
  gpio_config.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_config.Pull      = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_LED_SELECT_GPIO_PORT, &gpio_config);
  HAL_GPIO_WritePin(PWM_LED_SELECT_GPIO_PORT, PWM_LED_SELECT_GPIO_PIN, GPIO_PIN_RESET);

  PwmLed_TimerHandle.Instance = PWM_LED_TIM;
  
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    if(IsPwmLedMspCbValid == 0U)
    {
      if(BSP_PWM_LED_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#else
    /* TIM17 MSP initialization */
    TIM17_MspInit(&PwmLed_TimerHandle);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS == 1*/
      
  PwmLed_TimerHandle.Init.Prescaler = (PWM_LED_TIM_GET_COUNTER_CLK_FREQ()/1000000) -1;
  PwmLed_TimerHandle.Init.Period = (PWM_LED_TIM_COUNTER_FREQ/PWM_LED_TIM_FREQ) - 1;
  if (HAL_TIM_Base_Init(&PwmLed_TimerHandle) != HAL_OK) 
  {
    ret = BSP_ERROR_NO_INIT;
  }

  return ret;
}

/**
  * @brief  DeInitialize PWM LED Driver.
  * @retval BSP error code
  */
int32_t BSP_PWM_LED_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  HAL_GPIO_DeInit(PWM_LED_SELECT_GPIO_PORT, PWM_LED_SELECT_GPIO_PIN);
  HAL_GPIO_DeInit(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN);
  __HAL_TIM_DISABLE_IT(&PwmLed_TimerHandle, TIM_IT_UPDATE);
  if (HAL_TIM_PWM_DeInit(&PwmLed_TimerHandle) != HAL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
#else
    /* TIM17 MSP de-initialization */
    TIM17_MspDeInit(&PwmLed_TimerHandle);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS == 1*/
#if (PA7_AUTO_MANAGEMENT == 1)
    BSP_PWM_LED_Pin_AutoManagement();
#endif /* PA7_AUTO_MANAGEMENT == 1*/
  
  return ret;
}

/**
  * @brief  Set the GS data (PWM Control) for each output.
  * @param  aPwmLedGsData GS Data array (one element per output)
  * @retval BSP error code
  */
int32_t BSP_PWM_LED_On(aPwmLedGsData_TypeDef aPwmLedGsData)
{
  return PWM_LED_WriteData(aPwmLedGsData);
}

/**
  * @brief  Turn each output Off.
  * @retval BSP error code
  */
int32_t BSP_PWM_LED_Off(void)
{
  aPwmLedGsData_TypeDef aPwmLedGsData = {PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF};
  return PWM_LED_WriteData(aPwmLedGsData);
}

/**
  * @brief  Toggle the selected LED.
  * @param  aPwmLedGsData GS (Gray Scale) data
  * @retval BSP error code
  */
int32_t BSP_PWM_LED_Toggle(aPwmLedGsData_TypeDef aPwmLedGsData)
{
  if (BSP_PWM_LED_GetState() == 0U)
  {
    return PWM_LED_WriteData(aPwmLedGsData);
  }
  else
  {
    return BSP_PWM_LED_Off();
  }
}

/**
  * @brief  Indicate whether outputs are turned on or off.
  * @retval 0 means off, 1 means on
  */
int32_t BSP_PWM_LED_GetState(void)
{
  return (((PWM_LED_GSDATA[PWM_LED_RED] == PWM_LED_GSDATA_OFF)
        && (PWM_LED_GSDATA[PWM_LED_GREEN] == PWM_LED_GSDATA_OFF)
        && (PWM_LED_GSDATA[PWM_LED_BLUE] == PWM_LED_GSDATA_OFF)) ? 0U : 1U);
}

/**
  * @brief  BSP PWM LED interrupt handler.
  * @retval None
  */
void BSP_PWM_LED_IRQHandler(void)
{
  __HAL_TIM_CLEAR_FLAG(&PwmLed_TimerHandle, TIM_IT_UPDATE);
  CycleCount++;
}

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default PWM LED Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_PWM_LED_RegisterDefaultMspCallbacks(void)
{
  BSP_PWM_LED_Cb_t CallBacks;
  
  CallBacks.pMspInitCb = TIM17_MspInit;
  CallBacks.pMspDeInitCb = TIM17_MspDeInit;
  
  return BSP_PWM_LED_RegisterMspCallbacks(&CallBacks);
}

/**
  * @brief Register Application PWM LED Msp Callbacks
  * @param Callbacks     pointer to PWM LED MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_PWM_LED_RegisterMspCallbacks(BSP_PWM_LED_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;
  
  __HAL_TIM_RESET_HANDLE_STATE(&PwmLed_TimerHandle);
  
  /* Register MspInit/MspDeInit Callbacks */
  if (HAL_TIM_RegisterCallback(&PwmLed_TimerHandle, HAL_TIM_BASE_MSPINIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if (HAL_TIM_RegisterCallback(&PwmLed_TimerHandle, HAL_TIM_BASE_MSPDEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsPwmLedMspCbValid = 1U;
  }
  
  /* BSP status */
  return ret;
}
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS == 1 */

#if (PA7_AUTO_MANAGEMENT == 1)
/**
  * @brief Reconfigure the GPIO PA7 Pin for SPI1 MOSI Pin to be able to continue sending message on LCD.
  * @note This API is helpful to manage the hardware limitation available on STM32WB5MM-DK board: PA7 Pin is 
  * shared between LCD MOSI PIN and RGB LED.
  * @retval BSP error code
  */
void BSP_PWM_LED_Pin_AutoManagement(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure = {0};
  /* configure SPIx MOSI for LCD */
  GPIO_InitStructure.Pin       = BUS_SPI1_MOSI_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Alternate = BUS_SPI1_AF;
  HAL_GPIO_Init(BUS_SPI1_GPIO_PORTA, &GPIO_InitStructure);
}
#endif /* PA7_AUTO_MANAGEMENT == 1*/
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_BUTTON_Functions STM32WB5MM-DK COMMON BUTTON Functions
  * @{
  */

/**
  * @brief  Configure Button GPIO and EXTI Line.
  * @param  Button Specifies the Button to be configured.
  *   This parameter should be:
  *     @arg BUTTON_USER1
  *     @arg BUTTON_USER2
  * @param  ButtonMode Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval BSP error code
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure = {0};
  static BSP_EXTI_LineCallback button_callback[BUTTONn] = {BUTTON_USER1_EXTI_Callback, BUTTON_USER2_EXTI_Callback};
  static uint32_t button_interrupt_priority[BUTTONn] = {BSP_BUTTON_USERx_IT_PRIORITY, BSP_BUTTON_USERx_IT_PRIORITY};
  static const uint32_t button_exti_line[BUTTONn] = {BUTTON_USER1_EXTI_LINE, BUTTON_USER2_EXTI_LINE};

  /* Enable the BUTTON Clock */
  BUTTON_USERx_GPIO_CLK_ENABLE(Button);

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
  }
  else /* (ButtonMode == BUTTON_MODE_EXTI) */
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Mode = GPIO_MODE_IT_FALLING;

    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);

    (void)HAL_EXTI_GetHandle(&hpb_exti[Button], button_exti_line[Button]);
    (void)HAL_EXTI_RegisterCallback(&hpb_exti[Button], HAL_EXTI_COMMON_CB_ID, button_callback[Button]);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), button_interrupt_priority[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInitialize Push Button.
  * @param  Button  Button to be configured
  *   This parameter should be:
  *     @arg BUTTON_USER1
  *     @arg BUTTON_USER2
  * @note BSP_PB_DeInit() does not disable the GPIO clock
  * @retval BSP error code
  */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
  HAL_NVIC_DisableIRQ((BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], BUTTON_PIN[Button]);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Return the selected Button state.
  * @param  Button  Specifies the Button to be checked.
  *   This parameter should be:
  *     @arg BUTTON_USER1
  *     @arg BUTTON_USER2
  * @retval 0 means released, 1 means pressed.
  */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return (int32_t)HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  BSP Push Button callback
  * @param  Button: Specifies the Button to be checked.
  *         This parameter can be one of following parameters:
  *           @arg BUTTON_SW1
  *           @arg BUTTON_SW2
  * @retval None.
  */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);

  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */
}

/**
  * @brief  BSP PB interrupt handler.
  * @param  Button Button where interrupt occurs.
  *   This parameter should be:
  *     @arg BUTTON_USER1
  * @retval None.
  */
void BSP_PB_IRQHandler(Button_TypeDef Button)
{
  HAL_EXTI_IRQHandler(&hpb_exti[Button]);
}

/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @defgroup STM32WB5MM_DK_COMMON_COM_Functions COMMON COM Functions
  * @{
  */

/**
  * @brief  Configure COM port.
  * @param  COM COM port to be configured.
  *         This parameter can be COM1
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified USART peripheral.
  * @retval BSP error code
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM > COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* Init the UART Msp */
    COM1_MspInit(&hcom_uart[COM]);
#else
    if (IsComMspCbValid[COM] == 0U)
    {
      if (BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */

    if (MX_LPUART1_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
    {
      return BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DeInit COM port.
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* USART configuration */
    hcom_uart[COM].Instance = COM_USART[COM];

#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    COM1_MspDeInit(&hcom_uart[COM]);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */

    if (HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
    {
      return BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Configure COM port.
  * @param  huart USART handle
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified USART peripheral.
  * @retval HAL error code
  */
__weak HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init)
{
  /* USART configuration */
  huart->Instance          = COM_USART[COM1];
  huart->Init.BaudRate     = COM_Init->BaudRate;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.Parity       = (uint32_t)COM_Init->Parity;
  huart->Init.WordLength   = (uint32_t)COM_Init->WordLength;
  huart->Init.StopBits     = (uint32_t)COM_Init->StopBits;
  huart->Init.HwFlowCtl    = (uint32_t)COM_Init->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;

  return HAL_UART_Init(huart);
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default COM Msp Callbacks
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register default MspInit/MspDeInit Callback */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, COM1_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, COM1_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief Register COM Msp Callback registering
  * @param  COM COM port to be configured.
  *          This parameter can be COM1
  * @param Callbacks     pointer to COM1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG == 1)
/**
  * @brief  Select the active COM port.
  * @param  COM COM port to be activated.
  *          This parameter can be COM1
  * @retval BSP status
  */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{
  if (COM_ActiveLogPort != COM)
  {
    COM_ActiveLogPort = COM;
  }
  return BSP_ERROR_NONE;
}

#if defined(__ICCARM__)
/**
  * @brief  Retargets the C library __write function to the IAR function iar_fputc.
  * @param  file: file descriptor.
  * @param  ptr: pointer to the buffer where the data is stored.
  * @param  len: length of the data to write in bytes.
  * @retval length of the written data in bytes.
  */
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    iar_fputc((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /* __ICCARM__ */

/**
  * @brief Retargets the C library msg_info function to the USART.
  * @param None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the serial port and Loop until the end of transmission */
  while (HAL_OK != HAL_UART_Transmit(&hcom_uart [COM_ActiveLogPort], (uint8_t *) &ch, 1, COM_POLL_TIMEOUT))
  {
    ;
  }
  return ch;
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. read a character to the USART and Loop until the end of transmission */
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&hcom_uart [COM_ActiveLogPort], (uint8_t *)&ch, 1, COM_POLL_TIMEOUT))
  {
    ;
  }
  
  /* echo this character on console */
  (void) HAL_UART_Transmit(&hcom_uart [COM_ActiveLogPort], (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif /* USE_COM_LOG == 1 */

/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_COMMON_Private_Functions STM32WB5MM-DK COMMON Private Functions
  * @{
  */

/**
  * @brief  BUTTON1 EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_USER1_EXTI_Callback(void)
{
  BSP_PB_Callback(BUTTON_USER1);
}

/**
  * @brief  BUTTON2 EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_USER2_EXTI_Callback(void)
{
  BSP_PB_Callback(BUTTON_USER2);
}

#if (USE_BSP_COM_FEATURE == 1)
/**
  * @brief  Initializes COM1 MSP.
  * @param  huart UART handle
  * @retval BSP status
  */
static void COM1_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* Enable GPIO clock */
  COM1_TX_GPIO_CLK_ENABLE();
  COM1_RX_GPIO_CLK_ENABLE();

  /* Enable USART clock */
  COM1_CLK_ENABLE();

  /* Configure USART Tx as alternate function */
  gpio_init_structure.Pin       = COM1_TX_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Alternate = COM1_TX_AF;
  HAL_GPIO_Init(COM1_TX_GPIO_PORT, &gpio_init_structure);

  /* Configure USART Rx as alternate function */
  gpio_init_structure.Pin = COM1_RX_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = COM1_RX_AF;
  HAL_GPIO_Init(COM1_RX_GPIO_PORT, &gpio_init_structure);
}

/**
  * @brief  DeInitialize COM1 MSP part
  * @param  huart UART handle
  * @retval BSP status
  */
static void COM1_MspDeInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* COM GPIO pin configuration */
  gpio_init_structure.Pin  = COM1_TX_PIN;
  HAL_GPIO_DeInit(COM1_TX_GPIO_PORT, gpio_init_structure.Pin);

  gpio_init_structure.Pin  = COM1_RX_PIN;
  HAL_GPIO_DeInit(COM1_RX_GPIO_PORT, gpio_init_structure.Pin);

  /* Disable USART clock */
  COM1_CLK_DISABLE();
}
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @brief  Write GS data into the PWM LED driver through a single-wire interface
  * @param  aPwmLedGsData GS (Gray Scale) data
  * @retval BSP status
  */
static int32_t PWM_LED_WriteData(aPwmLedGsData_TypeDef aPwmLedGsData)
{
  if (aPwmLedGsData == NULL)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  __HAL_TIM_ENABLE_IT(&PwmLed_TimerHandle, TIM_IT_UPDATE);
  
  /* Start time base */
  if (HAL_TIM_Base_Start(&PwmLed_TimerHandle) != HAL_OK)
  {
    return BSP_ERROR_UNKNOWN_FAILURE;   
  }
  
  /* Enable Grayscale (GS) Control */
  HAL_GPIO_WritePin(PWM_LED_SELECT_GPIO_PORT, PWM_LED_SELECT_GPIO_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(PWM_LED_SELECT_GPIO_PORT, PWM_LED_SELECT_GPIO_PIN, GPIO_PIN_SET);
  
  /* TCycle measurement sequence */
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_RESET);
  PWM_LED_Wait(DELAY);
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_SET);
  PWM_LED_Wait(T_CYCLE_0);
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_RESET);
  PWM_LED_Wait(DELAY);
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_SET);
  PWM_LED_Wait(T_CYCLE_0);
  
  /* Write command */
  PWM_LED_SenByte(WRITE_COMMAND);
  
  /* Write the GS data */
  for (uint8_t gsdata = 0; gsdata < PWM_LED_NB; gsdata++)
  {
    PWM_LED_GSDATA[gsdata] = aPwmLedGsData[gsdata];
    PWM_LED_SenByte(PWM_LED_GSDATA[gsdata]);
  }
  
  /* Disable Grayscale (GS) Control */
  HAL_GPIO_WritePin(PWM_LED_SELECT_GPIO_PORT, PWM_LED_SELECT_GPIO_PIN, GPIO_PIN_RESET);
  
  /* Stop time base */
  if (HAL_TIM_Base_Stop(&PwmLed_TimerHandle) != HAL_OK)
  {
    return BSP_ERROR_UNKNOWN_FAILURE;   
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  Data 0/1 write sequence
  * @param  bit
  * @retval None
  */
static void PWM_LED_SendBit(uint8_t bit)
{
  /* Start next cycle */
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_SET);
  PWM_LED_Wait(DELAY); 
  HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_RESET);
  PWM_LED_Wait(DELAY);
  
  if (bit)
  {
    HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_SET);
    PWM_LED_Wait(DELAY);
    HAL_GPIO_WritePin(PWM_LED_SDI_GPIO_PORT, PWM_LED_SDI_GPIO_PIN, GPIO_PIN_RESET);
    PWM_LED_Wait(T_CYCLE_1);
  }
  else
  {
    PWM_LED_Wait(T_CYCLE_0);
  }
}

/**
  * @brief  Byte write Sequence
  * @param  byte
  * @retval None
  */
static void PWM_LED_SenByte(uint8_t byte)
{
    PWM_LED_SendBit(byte & (1<<7));
    PWM_LED_SendBit(byte & (1<<6));
    PWM_LED_SendBit(byte & (1<<5));
    PWM_LED_SendBit(byte & (1<<4));
    PWM_LED_SendBit(byte & (1<<3));
    PWM_LED_SendBit(byte & (1<<2));
    PWM_LED_SendBit(byte & (1<<1));
    PWM_LED_SendBit(byte & (1<<0));
}

/**
  * @brief  Byte write Sequence
  * @param  NbCycles Number of cycles to wait for
  * @retval None
  */
static void PWM_LED_Wait(uint32_t NbCycles)
{
  uint32_t start = CycleCount;
  
  while ((CycleCount - start) < NbCycles)
  {
  }
}

/**
  * @brief  Timer MSP initialization
  * @param  htim Timer instance
  * @retval None
  */
void TIM17_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == PWM_LED_TIM)
  {
    PWM_LED_TIM_CLOCK_ENABLE();
    
    HAL_NVIC_SetPriority(PWM_LED_TIM_UP_IRQN, PWM_LED_CLOCK_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(PWM_LED_TIM_UP_IRQN);
  }
}

/**
  * @brief  Timer MSP de-initialization
  * @param  htim Timer instance
  * @retval None
  */
void TIM17_MspDeInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == PWM_LED_TIM)
  {
    HAL_NVIC_DisableIRQ(PWM_LED_TIM_UP_IRQN);
    PWM_LED_TIM_CLOCK_DISABLE();
  }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
