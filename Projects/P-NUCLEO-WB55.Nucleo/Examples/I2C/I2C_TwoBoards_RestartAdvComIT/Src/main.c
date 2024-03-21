/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_RestartAdvComIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32WBxx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer and with a repeated start condition between the transmit
  *          and receive process. 
  *          The communication is done using 2 Boards.
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
/* Uncomment this line to use the board as master, if not it is used as slave */
#define MASTER_BOARD
/**
  * @brief Defines related to Slave process
  */
#define SLAVE_CHIP_NAME     0
#define SLAVE_CHIP_REVISION 1
#define SLAVE_LAST_INFO     SLAVE_CHIP_REVISION

/**
  * @brief Defines related to Timeout to keep Leds status
  */
#define LED_STATUS_TIMEOUT  1000 /* 1 Second */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/**
  * @brief Variables related to Master process
  */
/* aCommandCode declaration array    */
/* [CommandCode][RequestSlaveAnswer] */
/* {CODE, YES/NO}                    */
const char* aCommandCode[4][4] = {
  {"CHIP_NAME", "YES"},
  {"CHIP_REVISION", "YES"},
  {"LOW_POWER", "NO"},
  {"WAKE_UP", "NO"}};
                  
uint8_t*     pMasterTransmitBuffer     = (uint8_t*)(&aCommandCode[0]);
uint8_t      ubMasterNbCommandCode     = sizeof(aCommandCode[0][0]);
uint8_t      aMasterReceiveBuffer[0xF] = {0};
__IO uint8_t ubMasterNbDataToReceive   = sizeof(aMasterReceiveBuffer);
__IO uint8_t ubMasterNbDataToTransmit  = 0;
uint8_t      ubMasterCommandIndex      = 0;
__IO uint8_t ubMasterReceiveIndex      = 0;

/**
  * @brief Variables related to Slave process
  */
const char* aSlaveInfo[]      = {
                  "STM32WB55xx",
                  "1.2.3"};

uint8_t       aSlaveReceiveBuffer[0xF]  = {0};
uint8_t*      pSlaveTransmitBuffer      = 0;
__IO uint8_t  ubSlaveNbDataToTransmit   = 0;
uint8_t       ubSlaveInfoIndex          = 0xFF;
__IO uint8_t  ubSlaveReceiveIndex       = 0;
uint32_t      uwTransferDirection       = 0;
__IO uint32_t uwTransferInitiated       = 0;
__IO uint32_t uwTransferEnded           = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferLength);
#if defined(__GNUC__) && defined(MASTER_BOARD) && !defined(__ARMCC_VERSION)
extern void initialise_monitor_handles(void); /*rtt*/
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
#if defined(__GNUC__) && defined(MASTER_BOARD) && !defined(__ARMCC_VERSION)
  initialise_monitor_handles();	/*rtt*/
#endif

#if (USE_VCP_CONNECTION == 1)
  COM_InitTypeDef COM_Init;
#endif /* USE_VCP_CONNECTION */

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
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED2 and LED3 */
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

#if (USE_VCP_CONNECTION == 1)
  /* Configure COM port */
  /* Put the UART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
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
#endif /* USE_VCP_CONNECTION */


#ifdef MASTER_BOARD

  /* Configure User push-button (SW1) */
  BSP_PB_Init(BUTTON_SW1,BUTTON_MODE_GPIO);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifdef MASTER_BOARD

    /* Wait for User push-button (SW1) press before starting the Communication */
    while (BSP_PB_GetState(BUTTON_SW1) != GPIO_PIN_RESET)
    {
    }
  
    /* Wait for User push-button (SW1) release before starting the Communication */
    while (BSP_PB_GetState(BUTTON_SW1) != GPIO_PIN_SET)
    {
    }

    /* The board sends the message and expects to receive it back if necessary. */

    /* If Master no request a Slave answer, Run master in transmitter mode only. */
    if(strncmp(aCommandCode[ubMasterCommandIndex][1], "NO", 2) == 0)
    {
      /*##-2- Start the transmission process #####################################*/  
      /* Master prepare and send the transmission buffer ("pMasterTransmitBuffer") 
         through a "New" communication frame. The communication will be stopped at
         the end of transmission process thanks to "I2C_FIRST_AND_LAST_FRAME" option usage. */
      pMasterTransmitBuffer    = (uint8_t*)(aCommandCode[ubMasterCommandIndex][0]);
      ubMasterNbDataToTransmit = strlen((char *)(aCommandCode[ubMasterCommandIndex][0]));

      /* Handle I2C events (Master Transmit only) */
      do
      {
        if(HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1, (uint16_t)(I2C_ADDRESS),  pMasterTransmitBuffer, ubMasterNbDataToTransmit, I2C_FIRST_AND_LAST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-3- Wait for the end of the transfer #################################*/  
        /*  Before starting a new communication transfer, you need to check the current   
            state of the peripheral; if it is busy you need to wait for the end of current
            transfer before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the 
            transfer, but application may perform other tasks while transfer operation
            is ongoing. */  
        while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        {
        } 

        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
           Master restarts communication */
      }
      while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);

      /*##-4- Monitor Status through Terminal I/O ##############################*/  
      /* Display through external Terminal IO the Command Code acknowledge by Slave device */
      printf("Slave goes to %s.\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]));
    }
    /* Else Master request a Slave answer, Run master in transmitter mode then receiver mode. */
    else
    {
      /*##-2- Start the transmission process #####################################*/  
      /* Master prepare and send the transmission buffer ("pMasterTransmitBuffer") 
         through a "New" communication frame. The communication will not stopped thanks
         to "I2C_FIRST_FRAME" option usage. This will allow to generate a restart condition
         after change the I2C peripheral from transmission process to reception process */
      pMasterTransmitBuffer    = (uint8_t*)(aCommandCode[ubMasterCommandIndex][0]);
      ubMasterNbDataToTransmit = strlen((char *)(aCommandCode[ubMasterCommandIndex][0]));

      /* Handle I2C events (Master Transmit only) */
      do
      {
        if(HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1, (uint16_t)(I2C_ADDRESS),  pMasterTransmitBuffer, ubMasterNbDataToTransmit, I2C_FIRST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-3- Wait for the end of the transfer #################################*/  
        /*  Before starting a new communication transfer, you need to check the current   
            state of the peripheral; if it is busy you need to wait for the end of current
            transfer before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the 
            transfer, but application may perform other tasks while transfer operation
            is ongoing. */  
        while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        {
        } 

        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
           Master restarts communication */
      }
      while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);

      /*##-4- Put I2C peripheral in reception process ###########################*/  
      /* Master generate a restart condition and then change the I2C peripheral 
         from transmission process to reception process, to retrieve information
         data from Slave device. */
      do
      {
        if(HAL_I2C_Master_Sequential_Receive_IT(&hi2c1, (uint16_t)(I2C_ADDRESS), aMasterReceiveBuffer, strlen((char *)(aSlaveInfo[ubMasterCommandIndex])), I2C_LAST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-5- Wait for the end of the transfer #################################*/  
        /*  Before starting a new communication transfer, you need to check the current   
            state of the peripheral; if it is busy you need to wait for the end of current
            transfer before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the 
            transfer, but application may perform other tasks while transfer operation
            is ongoing. */  
        while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        {
        } 

        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
           Master restarts communication */
      }
      while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);

      /*##-6- Monitor Status through Terminal I/O ##############################*/  
      /* Display through external Terminal IO the Slave Answer received */
      printf("%s : %s\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]), (char*)aMasterReceiveBuffer);
    }

  
    /* Prepare Index to send next command code */
    ubMasterCommandIndex++;
    if(ubMasterCommandIndex >= ubMasterNbCommandCode)
    {
      ubMasterCommandIndex = 0;
    }
    
    /* For User help, keep Leds status until timeout */
    HAL_Delay(LED_STATUS_TIMEOUT);

    /* Then Clear and Reset process variables, arrays and Leds status, for next transfer */
    FlushBuffer8(aMasterReceiveBuffer, COUNTOF(aMasterReceiveBuffer));
    ubMasterNbDataToTransmit = 0;
    ubMasterReceiveIndex     = 0;
    BSP_LED_Off(LED2);
    
#else /* SLAVE_BOARD */
    
    /*##-2- Put I2C peripheral in Listen address match code process ##########*/  
    /* This action will allow I2C periphal to able to treat Master request when
       necessary depending of transfer direction requested by Master */
    if(HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK)
    {
      /* Transfer error in reception process */
      Error_Handler();
    }
    
    /*##-3- Wait for a new frame communication with a Master #################*/  
    /*  Before starting a transfer, you need to wait a Master request event.
        For simplicity reasons, this example is just waiting till an Address callback event,
       but application may perform other tasks while transfer operation is ongoing. */  
    while(uwTransferInitiated != 1)
    {
    }

    /*##-4- Wait for the end of the frame communication ######################*/  
    /*  Before ending a transfer, you need to wait a Master end event.
        For simplicity reasons, this example is just waiting till an Stop condition event,
        but application may perform other tasks while transfer operation is ongoing. */  
    while(uwTransferEnded != 1)
    {
    }
    
    /* For User help, keep Leds status until timeout */
    HAL_Delay(LED_STATUS_TIMEOUT);

    /*##-5- Clear, reset process variables, arrays and Leds status ###########*/  
    FlushBuffer8(aSlaveReceiveBuffer, COUNTOF(aSlaveReceiveBuffer));
    uwTransferInitiated = 0;
    uwTransferEnded = 0;
    ubSlaveReceiveIndex = 0;
    ubSlaveInfoIndex = 0xFF;
    BSP_LED_Off(LED2);
#endif /* MASTER_BOARD */
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = I2C_ADDRESS;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** I2C Enable Fast Mode Plus
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
#ifdef MASTER_BOARD
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 on: Transfer in transmission process is correct */
  BSP_LED_On(LED2);
}
#else
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 off: Transfer in transmission process is correct */
  BSP_LED_Off(LED2);
}
#endif /* MASTER_BOARD */

/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
#ifdef MASTER_BOARD
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 off: Transfer in reception process is correct */
  BSP_LED_Off(LED2);
}
#else
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 on: Transfer in reception process is correct */
  BSP_LED_On(LED2);

  /* Check Command code receive previously */
  /* If data received match with a Internal Command Code, set the associated index */
  /* Which will use for Transmission process if requested by Master */
  if(strcmp((char *)(aSlaveReceiveBuffer), (char *)(aCommandCode[0][0])) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_NAME;
  }
  else if(strcmp((char *)(aSlaveReceiveBuffer), (char *)(aCommandCode[1][0])) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_REVISION;
  }
  else
  {
    if(HAL_I2C_Slave_Sequential_Receive_IT(I2cHandle, &aSlaveReceiveBuffer[ubSlaveReceiveIndex], 1, I2C_FIRST_FRAME) != HAL_OK)
    {
      Error_Handler();
    }
    ubSlaveReceiveIndex++;
  }
}
#endif /* MASTER_BOARD */

#ifndef MASTER_BOARD
/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection: Master request Transfer Direction (Write/Read), value of @ref I2C_XferOptions_definition
  * @param  AddrMatchCode: Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  if(AddrMatchCode == (uint16_t)(I2C_ADDRESS))
  {
    uwTransferInitiated = 1;
    uwTransferDirection = TransferDirection;

    /* First of all, check the transfer direction to call the correct Slave Interface */
    if(uwTransferDirection == I2C_DIRECTION_TRANSMIT)
    {
      if(HAL_I2C_Slave_Sequential_Receive_IT(&hi2c1, &aSlaveReceiveBuffer[ubSlaveReceiveIndex], 1, I2C_FIRST_FRAME) != HAL_OK)
      {
        Error_Handler();
      }
      ubSlaveReceiveIndex++;
    }
    else
    {
      pSlaveTransmitBuffer = (uint8_t*)(aSlaveInfo[ubSlaveInfoIndex]);
      ubSlaveNbDataToTransmit = strlen((char *)(aSlaveInfo[ubSlaveInfoIndex]));

      if(HAL_I2C_Slave_Sequential_Transmit_IT(&hi2c1, pSlaveTransmitBuffer, ubSlaveNbDataToTransmit, I2C_LAST_FRAME) != HAL_OK)
      {
        Error_Handler();
      }
    }
  }
  else
  {
    /* Call Error Handler, Wrong Address Match Code */
    Error_Handler();
  }
}

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  uwTransferEnded = 1;
}
#endif

/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    /* Turn Off LED2 */
    BSP_LED_Off(LED2);

    Error_Handler();
  }
}

/**
  * @brief  Flush 8-bit buffer.
  * @param  pBuffer1: pointer to the buffer to be flushed.
  * @param  BufferLength: buffer's length
  * @retval None
  */
static void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferLength)
{
  uint8_t Index = 0;
  
  for (Index = 0; Index < BufferLength; Index++)
  {
    pBuffer1[Index] = 0;
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
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
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
