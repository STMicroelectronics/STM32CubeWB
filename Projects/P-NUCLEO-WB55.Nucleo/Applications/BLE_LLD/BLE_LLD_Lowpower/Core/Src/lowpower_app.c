/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : lowpower_app.c
  * Description        : BLE LLD validation Application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "shci.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include "stm_logging.h"
#include "dbg_trace.h"
#include "ble_lld.h"
#include "app_ble_lld.h"
#include "lowpower_app.h"
#include "ring_buffer.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LOWPOWER_CHANNEL_1         8
#define LOWPOWER_CHANNEL_2         9
#define LOWPOWER_ID                0x5A964129
#define LOWPOWER_WAKEUP            9876
#define LOWPOWER_RECEIVE           19876
#define LOWPOWER_RECEIVE_ACK       576
#define BACK2BACK_TIME          50

/* Routines options */
#define LOWPOWER_PACKET_NUMBER_TX 50 // NB of Successif PACKET using HAL Send
#define LOWPOWER_PACKET_NUMBER_RX 40 // NB of Successif PACKET using HAL Receive
#define LOWPOWER_PACKET_STOP_RX   0   // Stop after RX
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void m0CmdProcess(void);

static void LP_BLE_Init(void);
static void LP_SendPacket(void);
static void LP_ReceivePacket(void);

static void Appli_m0CmdProcess_RadioStop(void);
static void Appli_m0CmdProcess_RadioEnd(void);
static void Appli_m0CmdProcess_RxAck(void);
static void Appli_m0CmdProcess_RxOk(void);
static void Appli_m0CmdProcess_RxAckEnd(void);
static void Appli_m0CmdProcess_RxOkEnd(void);

static void bleInit(void);
static void bleSend(void);
static void bleReceive(void);
/* USER CODE END PFP */

/* Private variables -----------------------------------------------*/
/* USER CODE BEGIN PV */
static bool      Toggle_in_progress = false; // used for TX/RX toggle
static uint32_t  number_of_TXRX=0;
static uint32_t  number_of_RXACK=0;

/* data buffer Tab to send TX */
static uint8_t lowpowertxBuffer[TXRX_BUF_SIZE] ={0x95,0xFF,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73,
0x8A,0xA8,0xBB,0xFF,0x8A,0xA8,0xA8,0xA8,
};

/* data buffer ACK Tab to send TX ACK after RX */
static uint8_t lowpowerAcktxBuffer[TXRX_BUF_SIZE] ={0x60,0x0D,
0x4a,0x75,0x6c,0x69,0x65,0x6e,0x2b,0x4d,0x61,0x72,0x69,0x6e,0x65,
};

/* data buffer Tab to receive RX */
static uint8_t lowpowerrxBuffer[TXRX_BUF_SIZE];

/* data buffer ACK Tab to receive RX ACK after TX */
static uint8_t lowpowerAckrxBuffer[TXRX_BUF_SIZE];

static ActionPacket lowpowerPacket[ACTION_PACKET_NB];

/* Hot config */
static uint32_t BLE_LLD_hot_ana_config_table[BLE_HOT_ANA_CONFIG_TABLE_LENGTH/4];

/* Timer for LED blink */
extern TIM_HandleTypeDef htim2;
uint8_t SwitchOffGPIO_timer_Id;
#define LED_ON_TIMEOUT            (0.5*1000*1000/CFG_TS_TICK_VAL) /**< 500ms */

/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN FD */

void LOWPOWER_APP_Init(void)
{
  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  CheckWirelessFirmwareInfo();

  /* Do not allow standby in the application */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  /* Disable low power mode for now, may be enable later depending on configuration */
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  /* Register tasks for event processing */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CMD_FROM_M0_TO_M4,     UTIL_SEQ_RFU, m0CmdProcess);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_INIT,          UTIL_SEQ_RFU, LP_BLE_Init);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_SENDPACKET,    UTIL_SEQ_RFU, LP_SendPacket);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_RECEIVEPACKET, UTIL_SEQ_RFU, LP_ReceivePacket);

  /* Register callbacks for radio events processing */
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_STOP,     Appli_m0CmdProcess_RadioStop);
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_END,      Appli_m0CmdProcess_RadioEnd);
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_RXACK,    Appli_m0CmdProcess_RxAck);
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_RXOK,     Appli_m0CmdProcess_RxOk);
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_RXACKEND, Appli_m0CmdProcess_RxAckEnd);
  BLE_LLD_PRX_ReplyTaskCbRegister(CMD_FROM_M0_RADIO_RXOKEND,  Appli_m0CmdProcess_RxOkEnd);

  BLE_LLD_PRX_Init(APP_BLE_LLD_SendCmdM0);

  APP_BLE_LLD_Init();

  UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_INIT, CFG_SCH_PRIO_0);

  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_On(LED_BLUE);
  BSP_LED_On(LED_RED);
  BSP_LED_On(LED_GREEN);
  #endif


  #if ( CFG_LPM_SUPPORTED == 1)
      HW_TS_Create(CFG_TIM_PROC_ID_ISR, &SwitchOffGPIO_timer_Id, hw_ts_SingleShot, Appli_TS_Callback);
  #else
      HAL_TIM_Base_Start_IT(&htim2);
  //UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_ENABLE );
  #endif
  


}

/* Route button event to processing task */
void Appli_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case BUTTON_SW1_PIN:
      //UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_INIT, CFG_SCH_PRIO_0);
      break;

    case BUTTON_SW2_PIN:
      //UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0);
      break;

    case BUTTON_SW3_PIN:
      //UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_RECEIVEPACKET, CFG_SCH_PRIO_0);
      break;

    default:
      break;

  }
  return;
}

void Appli_TIM_IC_CaptureCallback(void)
{
}

static void m0CmdProcess(void)
{
  BLE_LLD_PRX_ReplyTaskCbDispatch();
}

static void Appli_m0CmdProcess_RadioStop(void)
{
  logUart("Radio STOP : RXACK:%d RX:%d",number_of_RXACK ,number_of_TXRX);
  number_of_RXACK=0;
  number_of_TXRX=0;
}
static void Appli_m0CmdProcess_RadioEnd(void)
{
  #if (CFG_LPM_SUPPORTED == 1U)
    HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
    UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_ENABLE );
  #else
    HAL_TIM_Base_Start_IT(&htim2);
  #endif  
  number_of_RXACK=0;
  number_of_TXRX=0;
  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_Off(LED_BLUE);
  #endif
}

static void Appli_m0CmdProcess_RxAck(void)
{
  number_of_RXACK++;
  logUart("Radio RXACK %d",number_of_RXACK);
  //BSP_LED_Off(LED_BLUE);
}

static void Appli_m0CmdProcess_RxOk(void)
{
  number_of_TXRX++;
  logUart("Radio RX %d",number_of_TXRX);
  //BSP_LED_Off(LED_BLUE);
}

static void Appli_m0CmdProcess_RxAckEnd(void)
{
  number_of_RXACK++;
  #if (CFG_LPM_SUPPORTED == 1U)
    HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
  #else
    HAL_TIM_Base_Start_IT(&htim2);
  #endif
  logUart("Radio END: RXACK:%d",number_of_RXACK);
  number_of_RXACK=0;

}

static void Appli_m0CmdProcess_RxOkEnd(void)
{
  number_of_TXRX++;
  #if (CFG_LPM_SUPPORTED == 1U)
    HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
  #else
    HAL_TIM_Base_Start_IT(&htim2);
  #endif
  logUart("Radio END: RX:%d",number_of_TXRX);
  number_of_TXRX=0;
}

/* Appli custom functions */
static void LP_BLE_Init(void)
{
      bleInit();
}

static void LP_SendPacket(void)
{
      #if (CFG_LED_SUPPORTED == 1U)
      BSP_LED_On(LED_RED);
      BSP_LED_Off(LED_GREEN);
      #endif

      bleSend();
}

static void LP_ReceivePacket(void)
{
      #if (CFG_LED_SUPPORTED == 1U)
      BSP_LED_On(LED_GREEN);
      BSP_LED_Off(LED_RED);
      #endif
      bleReceive();
}

/* ---------------------------- LLD specific code ----------------------------*/
static void bleInit(void)
{
  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_Off(LED_BLUE);
  #endif
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  BLE_LLD_Init(HS_STARTUP_TIME, 1, BLE_LLD_hot_ana_config_table, ENABLE);

  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_On(LED_BLUE);
  BSP_LED_Off(LED_BLUE);
  #endif
  uint8_t map[5]= {0xFF,0xFF,0xFF,0xFF,0xFF};

  BLE_LLD_SetChannelMap(STATE_MACHINE_0, &map);
  BLE_LLD_SetChannel(STATE_MACHINE_0,LOWPOWER_CHANNEL_2,0);
  BLE_LLD_SetTxAttributes(STATE_MACHINE_0, LOWPOWER_ID, 0x555555,0);
  BLE_LLD_SetTx_Rx_Phy(STATE_MACHINE_0, TX_PHY_2MBPS, RX_PHY_2MBPS);
  BLE_LLD_SetBackToBackTime(BACK2BACK_TIME);

  BLE_LLD_SetChannelMap(STATE_MACHINE_3, &map);
  BLE_LLD_SetChannel(STATE_MACHINE_3, LOWPOWER_CHANNEL_2, 0);
  BLE_LLD_SetTxAttributes(STATE_MACHINE_3, ~LOWPOWER_ID, 0x555555,0);
  BLE_LLD_SetTx_Rx_Phy(STATE_MACHINE_3, TX_PHY_2MBPS, RX_PHY_2MBPS);
  BLE_LLD_SetBackToBackTime(BACK2BACK_TIME);
      
     
  #if (CFG_LPM_SUPPORTED == 1U)
    HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
  #else
    HAL_TIM_Base_Start_IT(&htim2);
  #endif

}

static void bleSend(void)
{
  lowpowerPacket[APACKET_0].StateMachineNo = STATE_MACHINE_0;
  lowpowerPacket[APACKET_0].ActionTag = AT_TXRX | AT_PLL_TRIG | AT_TIMER_WAKEUP | AT_RELATIVE;
  lowpowerPacket[APACKET_0].WakeupTime = LOWPOWER_WAKEUP;
  lowpowerPacket[APACKET_0].data = lowpowertxBuffer;
  lowpowerPacket[APACKET_0].next_true = APACKET_1;
  lowpowerPacket[APACKET_0].next_false = APACKET_NULL;
  lowpowerPacket[APACKET_0].condRoutine = condCase_Done;
  lowpowerPacket[APACKET_0].dataRoutine = DATA_ROUT_LL_TXMULTIACK;
  lowpowerPacket[APACKET_0].actionPacketNb = APACKET_0;
  BLE_LLD_SetReservedArea(&lowpowerPacket[APACKET_0]);

  lowpowerPacket[APACKET_1].StateMachineNo = STATE_MACHINE_3;
  lowpowerPacket[APACKET_1].ActionTag = 0;
  lowpowerPacket[APACKET_1].WakeupTime = LOWPOWER_WAKEUP;
  lowpowerPacket[APACKET_1].ReceiveWindowLength = LOWPOWER_RECEIVE;
  lowpowerPacket[APACKET_1].data = lowpowerAckrxBuffer;
  lowpowerPacket[APACKET_1].next_true = APACKET_NULL;
  lowpowerPacket[APACKET_1].next_false = APACKET_0;
  lowpowerPacket[APACKET_1].condRoutine = condCase_Done;
  lowpowerPacket[APACKET_1].dataRoutine = DATA_ROUT_LL_TXMULTIACK;
  lowpowerPacket[APACKET_1].actionPacketNb = APACKET_1;
  BLE_LLD_SetReservedArea(&lowpowerPacket[APACKET_1]);

  BLE_LLD_PRX_SetdataRoutineMultiOptions(LOWPOWER_PACKET_NUMBER_TX,LOWPOWER_PACKET_STOP_RX);
  BLE_LLD_MakeActionPacketPending(&lowpowerPacket[APACKET_0]);
}

static void bleReceive(void)
{
  lowpowerPacket[APACKET_5].StateMachineNo = STATE_MACHINE_0;
  lowpowerPacket[APACKET_5].ActionTag = AT_PLL_TRIG | AT_TIMER_WAKEUP | AT_RELATIVE;
  lowpowerPacket[APACKET_5].WakeupTime = LOWPOWER_WAKEUP;
  lowpowerPacket[APACKET_5].ReceiveWindowLength = LOWPOWER_RECEIVE;
  lowpowerPacket[APACKET_5].data = lowpowerrxBuffer;
  lowpowerPacket[APACKET_5].next_true = APACKET_1;
  lowpowerPacket[APACKET_5].next_false = APACKET_NULL;
  lowpowerPacket[APACKET_5].condRoutine = condCase_Rx;
  lowpowerPacket[APACKET_5].dataRoutine = DATA_ROUT_LL_RXMULTIACK;
  lowpowerPacket[APACKET_5].actionPacketNb = APACKET_5;
  BLE_LLD_SetReservedArea(&lowpowerPacket[APACKET_5]);

  lowpowerPacket[APACKET_1].StateMachineNo = STATE_MACHINE_3;
  lowpowerPacket[APACKET_1].ActionTag = AT_TXRX;
  lowpowerPacket[APACKET_1].WakeupTime = LOWPOWER_WAKEUP;
  lowpowerPacket[APACKET_1].data = lowpowerAcktxBuffer;
  lowpowerPacket[APACKET_1].next_true = APACKET_NULL;
  lowpowerPacket[APACKET_1].next_false = APACKET_5;
  lowpowerPacket[APACKET_1].condRoutine = condCase_Done;
  lowpowerPacket[APACKET_1].dataRoutine = DATA_ROUT_LL_RXMULTIACK;
  lowpowerPacket[APACKET_1].actionPacketNb = APACKET_1;
  BLE_LLD_SetReservedArea(&lowpowerPacket[APACKET_1]);

  BLE_LLD_PRX_SetdataRoutineMultiOptions(LOWPOWER_PACKET_NUMBER_RX,LOWPOWER_PACKET_STOP_RX);
  BLE_LLD_MakeActionPacketPending(&lowpowerPacket[APACKET_5]);
}

void Appli_TIM_PeriodElapsedCallback(void)
{
  HAL_TIM_Base_Stop_IT(&htim2);
  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_On(LED_BLUE);
  #endif

  if(Toggle_in_progress){
      Toggle_in_progress = false;
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0);
  } else {
      Toggle_in_progress = true;
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_RECEIVEPACKET, CFG_SCH_PRIO_0);
  }
}

void Appli_TS_Callback(void)
{
  #if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_On(LED_BLUE);
  #endif
  
  if(Toggle_in_progress){
      Toggle_in_progress = false;
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0);
  } else {
      Toggle_in_progress = true;
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_RECEIVEPACKET, CFG_SCH_PRIO_0);
  }
}
/* USER CODE END FD */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
/* USER CODE END FD_WRAP_FUNCTIONS */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
