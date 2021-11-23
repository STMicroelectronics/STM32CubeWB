/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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

/** @addtogroup STM32WLxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
BSP_DemoTypedef  BSP_examples[]=
{
  {LCD_demo, "OLED SCREEN Demo", 0},
  {Temperature_Test, "Temperature Demo", 1},
  {Gyro_Test, "Gyroscope Demo", 2},
  {Accelero_Test, "Accelerometer Demo", 3},
  {Proximity_Test, "Proximity Demo", 4},
  {QSPI_demo, "QSPI Demo", 5},
  {Audio_Record_Demo, "Audio Record Demo", 6},
  {Led_demo, "Led Demo", 7}
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
__IO uint32_t ButtonState = 0;
__IO uint8_t NbLoop = 1;
/* Exported variables --------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32WBxx HAL library initialization:
  - Configure the Flash prefetch, Flash preread and Buffer caches
  - Systick timer is configured by default as source of time base, but user 
  can eventually implement his proper time base source (a general purpose 
  timer for example or other time source), keeping in mind that Time base 
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
  handled in milliseconds basis.
  - Low Level Initialization
  */
  HAL_Init();
  
  /* Configure the System clock to have a frequency of 48 MHz */
  SystemClock_Config();
  
  /* Configure the User Button 1 in EXTI Mode */
  BSP_PB_Init(BUTTON_USER1, BUTTON_MODE_EXTI);
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  Display_DemoDescription();
  
  while (1)
  {
    if(ButtonState == 1)
    {
      HAL_Delay(400);
      ButtonState = 0;
      BSP_examples[DemoIndex++].DemoFunc();

      HAL_Delay(100);

      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        NbLoop++;
        DemoIndex = 0;
      }
      Display_DemoDescription();
    }
  }
}

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
    |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
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
  /** Initializes the peripherals clocks 
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_USB;
  
  PeriphClkInitStruct.PLLSAI1.PLLN = 86;
  PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV7;
  PeriphClkInitStruct.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLR = RCC_PLLR_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);  
  HAL_PWREx_EnableVddUSB();
}

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[64];
  uint32_t x_size;
  uint32_t y_size;

  /* Set LCD Foreground Layer  */
  UTIL_LCD_SetFuncDriver(&LCD_Driver); /* SetFunc before setting device */
  UTIL_LCD_SetDevice(0);            /* SetDevice after funcDriver is set */
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_DisplayOn(0);
  BSP_LCD_Refresh(0);
  
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  UTIL_LCD_SetFont(&Font12);
  
  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
  UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"STM32WB5MM-DK", CENTER_MODE);
  BSP_LCD_Refresh(0);
  HAL_Delay(1000);

  UTIL_LCD_DisplayStringAt(0, 12, (uint8_t *)"Press user button1", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"to start :", CENTER_MODE);
  sprintf(desc,"%s", BSP_examples[DemoIndex].DemoName);
  UTIL_LCD_DisplayStringAt(0, 36, (uint8_t *)desc, CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 48, (uint8_t *)"example", CENTER_MODE);
  BSP_LCD_Refresh(0);
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
uint32_t Serial_Scanf(uint32_t value)
{
  uint16_t tmp = 0;
  
  tmp = getchar();
  while ( getchar() != '\n' );
  
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %lu \n", value);
    return 0xFF;
  }
  return tmp;
}

/**
  * @brief  Check for user input
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  return ButtonState;
}

/**
  * @brief  Button Callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if(Button == BUTTON_USER1)
  {
    ButtonState = 1;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{  
  while(1) 
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
