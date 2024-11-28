/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_conf.h
  * Description        : Application configuration file for STM32WPAN Middleware.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONF_H
#define APP_CONF_H

#include "hw.h"
#include "hw_conf.h"
#include "hw_if.h"

/******************************************************************************
 * Application Config
 ******************************************************************************/
#define CFG_HS_STARTUP_TIME 0x0099 /* Start up time of the high speed oscillator in system time units (625/256us) */

#define CFG_BACK2BACK_TIME 200 /* Back to back time (us) */

/******************************************************************************
 * Transport Layer
 ******************************************************************************/
/**
 * Queue length of M0 traces/log messages
 * This parameter defines the number of asynchronous events that can be stored in the M0 test FW before
 * being reported to the M4 which will re-build traces/log messages before to send them to UART.
 * This parameter is combined with the CFG_TL_MOST_EVENT_PAYLOAD_SIZE to calculate the queue size needed by M0 for traces.
 */
#define CFG_TL_EVT_QUEUE_LENGTH 20
/**
 * TL_EVENT_FRAME_SIZE is the size of the packets transferred between M0 and M4 through IPCC.
 * Note 1 : thoose packets are first put in a trace queue (see DBG_TRACE_MSG_QUEUE_SIZE) before being sent to UART
 * Note 2 : Queue size must be higher than a M0 trace buffer max size (see LOG_BUFFER_SIZE_MAX in M0 FW)
 */
#define CFG_TL_MOST_EVENT_PAYLOAD_SIZE 255

#define TL_EVENT_FRAME_SIZE ( TL_EVT_HDR_SIZE + CFG_TL_MOST_EVENT_PAYLOAD_SIZE )

/******************************************************************************
 * UART interfaces
 ******************************************************************************/
/**
 * Select UART interfaces
 */
#define CFG_DEBUG_TRACE_UART    hw_lpuart1
#define CFG_UART    hw_uart1

/******************************************************************************
 * USB interface
 ******************************************************************************/

/**
 * Enable/Disable USB interface
 */
#define CFG_USB_INTERFACE_ENABLE    0

/******************************************************************************
 * Low Power
 *
 *  When BLE_LLD_LP is set to 1:
 *  the system is configured in low power mode CFG_LPM_SUPPORTED
 *
 *  When BLE_LLD_LP is not set, the low power mode is not activated
 *
 ******************************************************************************/
#define CFG_LPM_SUPPORTED   0

#ifdef BLE_LLD_LP
#undef CFG_LPM_SUPPORTED
#define CFG_LPM_SUPPORTED   1
#endif

#define CFG_PWR_MODE_STOP      LL_PWR_MODE_STOP2

/******************************************************************************
 * Timer Server
 ******************************************************************************/
/**
 *  CFG_RTC_WUCKSEL_DIVIDER:  This sets the RTCCLK divider to the wakeup timer.
 *  The higher is the value, the better is the power consumption and the accuracy of the timerserver
 *  The lower is the value, the finest is the granularity
 *
 *  CFG_RTC_ASYNCH_PRESCALER: This sets the asynchronous prescaler of the RTC. It should as high as possible ( to output
 *  clock as low as possible) but the output clock should be equal or higher frequency compare to the clock feeding
 *  the wakeup timer. A lower clock speed would impact the accuracy of the timer server.
 *
 *  CFG_RTC_SYNCH_PRESCALER: This sets the synchronous prescaler of the RTC.
 *  When the 1Hz calendar clock is required, it shall be sets according to other settings
 *  When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0x7FFF (MAX VALUE)
 *
 *  CFG_RTCCLK_DIVIDER_CONF:
 *  Shall be set to either 0,2,4,8,16
 *  When set to either 2,4,8,16, the 1Hhz calendar is supported
 *  When set to 0, the user sets its own configuration
 *
 *  The following settings are computed with LSI as input to the RTC
 */
#define CFG_RTCCLK_DIVIDER_CONF 0

#if (CFG_RTCCLK_DIVIDER_CONF == 0)
/**
 * Custom configuration
 * It does not support 1Hz calendar
 * It divides the RTC CLK by 16
 */
#define CFG_RTCCLK_DIV  (16)
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#define CFG_RTC_ASYNCH_PRESCALER (CFG_RTCCLK_DIV - 1)
#define CFG_RTC_SYNCH_PRESCALER (0x7FFF)

#else

#if (CFG_RTCCLK_DIVIDER_CONF == 2)
/**
 * It divides the RTC CLK by 2
 */
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 4)
/**
 * It divides the RTC CLK by 4
 */
#define CFG_RTC_WUCKSEL_DIVIDER (2)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 8)
/**
 * It divides the RTC CLK by 8
 */
#define CFG_RTC_WUCKSEL_DIVIDER (1)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 16)
/**
 * It divides the RTC CLK by 16
 */
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#endif

#define CFG_RTCCLK_DIV              CFG_RTCCLK_DIVIDER_CONF
#define CFG_RTC_ASYNCH_PRESCALER    (CFG_RTCCLK_DIV - 1)
#define CFG_RTC_SYNCH_PRESCALER     (DIVR( LSE_VALUE, (CFG_RTC_ASYNCH_PRESCALER+1) ) - 1 )

#endif

/** tick timer value in us */
#define CFG_TS_TICK_VAL           DIVR( (CFG_RTCCLK_DIV * 1000000), LSE_VALUE )

typedef enum
{
  CFG_TIM_PROC_ID_ISR,
} CFG_TimProcID_t;

/******************************************************************************
 * Debug
 ******************************************************************************/
/**
 * When set, this resets some hw resources to put the device in the same state as at power up.
 * It resets only register that may prevent the FW to run properly.
 *
 * This shall be set to 0 in a final product
 *
 */
#define CFG_HW_RESET_BY_FW         1

/**
 * keep debugger enabled while in any low power mode when set to 1
 * should be set to 0 in production
 */
#define CFG_DEBUGGER_SUPPORTED    1

/*****************************************************************************
 * Traces
 * Enable or Disable traces in application
 * When CFG_DEBUG_TRACE is set, traces are activated
 *
 * Note : Refer to utilities_conf.h file in order to details
 *        the level of traces : CFG_DEBUG_TRACE_FULL or CFG_DEBUG_TRACE_LIGHT
 *****************************************************************************/
#define CFG_DEBUG_TRACE    1

/**
 * When CFG_DEBUG_TRACE_FULL is set to 1, the trace are output with the API name, the file name and the line number
 * When CFG_DEBUG_TRACE_LIGHT is set to 1, only the debug message is output
 *
 * When both are set to 0, no trace are output
 * When both are set to 1,  CFG_DEBUG_TRACE_FULL is selected
 */
#define CFG_DEBUG_TRACE_LIGHT     1
#define CFG_DEBUG_TRACE_FULL      0

#if (( CFG_DEBUG_TRACE != 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ) && (CFG_DEBUG_TRACE_FULL == 0))
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     1
#endif

#if ( CFG_DEBUG_TRACE == 0 )
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     0
#endif

/**
 * When not set, the traces is looping on sending the trace over UART
 */
#define DBG_TRACE_USE_CIRCULAR_QUEUE 1

/**
 * max buffer Size to queue data traces and max data trace allowed.
 * Only Used if DBG_TRACE_USE_CIRCULAR_QUEUE is defined
 */
#define DBG_TRACE_MSG_QUEUE_SIZE 4096
#define MAX_DBG_TRACE_MSG_SIZE 1024

/******************************************************************************
 * Configure Log level for Application
 ******************************************************************************/
#define APPLI_CONFIG_LOG_LEVEL    LOG_LEVEL_INFO
#define APPLI_PRINT_FILE_FUNC_LINE    0

/* USER CODE BEGIN Defines */
/******************************************************************************
 * User interaction
 * When CFG_LED_SUPPORTED is set, LEDS are activated if requested
 * When CFG_BUTTON_SUPPORTED is set, the push button are activated if requested
 ******************************************************************************/
#define CFG_LED_SUPPORTED         1
#define CFG_BUTTON_SUPPORTED      1

/* USER CODE END Defines */

/******************************************************************************
 * Scheduler
 ******************************************************************************/
  /**
   * This is the list of task id required by the application
   * Each Id shall be in the range 0..31
   */

typedef enum
{
  CFG_TASK_CMD_FROM_M0_TO_M4,
  CFG_TASK_SEND_CLI_TO_M0,
  CFG_TASK_SEND_TO_M0,
/* USER CODE BEGIN IdleTask */
     CFG_TASK_HAL_BLE_ENCRYPT,
     CFG_TASK_HAL_BLE_STARTTONE,
     CFG_TASK_HAL_BLE_STOPTONE,
/* USER CODE END IdleTask */
  CFG_TASK_SYSTEM_HCI_ASYNCH_EVT,
  CFG_TASK_PROCESS_UART_RX_BUFFER,
  CFG_TASK_PROCESS_UART_RX_IT,
  CFG_TASK_PROCESS_UART_TX_IT,
  CFG_TASK_NBR  /**< Shall be last in the list */
} CFG_IdleTask_Id_t;

/**
 * This is the list of priority required by the application
 * Each Id shall be in the range 0..31
 */
typedef enum
{
    CFG_SCH_PRIO_0,
    CFG_SCH_PRIO_1,
    CFG_SCH_PRIO_NBR,
} CFG_SCH_Prio_Id_t;

/**
 * This is a bit mapping over 32bits listing all events id supported in the application
 */
typedef enum
{
  CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP,
  CFG_EVT_RECEIVE_RSPACKEVT,
  CFG_EVT_RECEIVE_ENDPACKEVT,
} CFG_IdleEvt_Id_t;

/******************************************************************************
 * LOW POWER
 ******************************************************************************/
/**
 * Supported requester to the MCU Low Power Manager - can be increased up  to 32
 * It lists a bit mapping of all user of the Low Power Manager
 */
typedef enum
{
    CFG_LPM_APP,
    CFG_LPM_APP_BLE_LLD,
  /* USER CODE BEGIN CFG_LPM_Id_t */

  /* USER CODE END CFG_LPM_Id_t */
} CFG_LPM_Id_t;

/******************************************************************************
 * OTP manager
 ******************************************************************************/
#define CFG_OTP_BASE_ADDRESS    OTP_AREA_BASE

#define CFG_OTP_END_ADRESS      OTP_AREA_END_ADDR

#endif /*APP_CONF_H */

