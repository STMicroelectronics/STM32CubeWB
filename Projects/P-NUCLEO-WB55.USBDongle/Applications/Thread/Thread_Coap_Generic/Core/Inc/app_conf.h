/**
  ******************************************************************************
 * @file    app_conf.h
 * @author  MCD Application Team
 * @brief   Application configuration file
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONF_H
#define APP_CONF_H

  /* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "hw_conf.h"
#include "hw_if.h"

/******************************************************************************
 * Transport Layer
 ******************************************************************************/
/**
 * Queue length of BLE Event
 * This parameter defines the number of asynchronous events that can be stored in the HCI layer before
 * being reported to the application. When a command is sent to the BLE core coprocessor, the HCI layer
 * is waiting for the event with the Num_HCI_Command_Packets set to 1. The receive queue shall be large
 * enough to store all asynchronous events received in between.
 * When CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE is set to 27, this allow to store three 255 bytes long asynchronous events
 * between the HCI command and its event.
 * This parameter depends on the value given to CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE. When the queue size is to small,
 * the system may hang if the queue is full with asynchronous events and the HCI layer is still waiting
 * for a CC/CS event, In that case, the notification TL_BLE_HCI_ToNot() is called to indicate
 * to the application a HCI command did not receive its command event within 30s (Default HCI Timeout).
 */
#define CFG_TL_EVT_QUEUE_LENGTH 5

/**
 * This parameter should be set to fit most events received by the HCI layer. It defines the buffer size of each element
 * allocated in the queue of received events and can be used to optimize the amount of RAM allocated by the Memory Manager.
 * It should not exceed 255 which is the maximum HCI packet payload size (a greater value is a lost of memory as it will
 * never be used)
 * It shall be at least 4 to receive the command status event in one frame.
 * The default value is set to 27 to allow receiving an event of MTU size in a single buffer. This value maybe reduced
 * further depending on the application.
 *
 */
#define CFG_TL_MOST_EVENT_PAYLOAD_SIZE 255   /**< Set to 255 with the memory manager and the mailbox */

#define TL_EVENT_FRAME_SIZE ( TL_EVT_HDR_SIZE + CFG_TL_MOST_EVENT_PAYLOAD_SIZE )

/******************************************************************************
 * UART interfaces
 ******************************************************************************/
/**
 * Select UART interfaces
 */
#define CFG_DEBUG_TRACE_UART      hw_lpuart1
#define CFG_CLI_UART                hw_uart1

/******************************************************************************
 * USB interface
 ******************************************************************************/
/**
 * Enable/Disable USB interface
 */
#define CFG_USB_INTERFACE_ENABLE    1

/******************************************************************************
 * Low Power
 ******************************************************************************/
/**
 *  When set to 1, the low power mode is enable
 *  When set to 0, the device stays in RUN mode
 */
#define CFG_LPM_SUPPORTED   0

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
#define CFG_HW_RESET_BY_FW        1

#define CFG_LED_SUPPORTED         1
#define CFG_BUTTON_SUPPORTED      1

/**
 * keep debugger enabled while in any low power mode when set to 1
 * should be set to 0 in production
 */
#define CFG_DEBUGGER_SUPPORTED    1

/**
 * When CFG_DEBUG_TRACE_FULL is set to 1, the trace are output with the API name, the file name and the line number
 * When CFG_DEBUG_TRACE_LIGTH is set to 1, only the debug message is output
 *
 * When both are set to 0, no trace are output
 * When both are set to 1,  CFG_DEBUG_TRACE_FULL is selected
 */
#define CFG_DEBUG_TRACE_LIGTH     1
#define CFG_DEBUG_TRACE_FULL      0

/* Define platform used: only ARM supported for TRACE trace */
#define CFG_PLATFORM_LINUX        (0x01)
#define CFG_PLATFORM_WINDOWS      (0x02)
#define CFG_PLATFORM_ARM          (0x03)

#define CFG_PLATFORM_TYPE         (CFG_PLATFORM_ARM)

/**
 * Enable or Disable traces in application
 */
#define APP_DBG_EN              1

#if (APP_DBG_EN != 0)
#define APP_DBG_MSG             PRINT_MESG_DBG
#else
#define APP_DBG_MSG             PRINT_NO_MESG
#endif

#if (( CFG_DEBUG_TRACE_FULL == 1 ) || ( CFG_DEBUG_TRACE_LIGTH == 1 ))
#define CFG_DEBUG_TRACE      1

#undef CFG_LPM_SUPPORTED
#define CFG_LPM_SUPPORTED         0

#undef CFG_DEBUGGER_SUPPORTED
#define CFG_DEBUGGER_SUPPORTED    1

#undef CFG_PLATFORM_TYPE
#define CFG_PLATFORM_TYPE         (CFG_PLATFORM_ARM)

#else
#define CFG_DEBUG_TRACE      0
#endif



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
#if (CFG_FULL_LOW_POWER == 1)
#define CFG_LED_SUPPORTED         0
#define CFG_BUTTON_SUPPORTED      0
#else
#define CFG_LED_SUPPORTED         1
#define CFG_BUTTON_SUPPORTED      1
#endif /* CFG_FULL_LOW_POWER */
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
  CFG_TASK_COAP_MSG_BUTTON,
  CFG_TASK_MSG_FROM_M0_TO_M4,
  CFG_TASK_SEND_CLI_TO_M0,
  CFG_TASK_SYSTEM_HCI_ASYNCH_EVT,
  CFG_TASK_TRACE,
  CFG_TASK_AMM_BCKGND,
#if (CFG_USB_INTERFACE_ENABLE != 0)
  CFG_TASK_VCP_SEND_DATA,
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
  CFG_TASK_NBR  /**< Shall be last in the list */
} CFG_IdleTask_Id_t;

/* Scheduler types and defines        */
/*------------------------------------*/
#define TASK_COAP_MSG_BUTTON        (1U << CFG_TASK_COAP_MSG_BUTTON)
#define TASK_MSG_FROM_M0_TO_M4      (1U << CFG_TASK_MSG_FROM_M0_TO_M4)


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
  CFG_EVT_ACK_FROM_M0_EVT,
  CFG_EVT_SYNCHRO_BYPASS_IDLE,
  CFG_EVT_TIMER
} CFG_IdleEvt_Id_t;

#define EVENT_ACK_FROM_M0_EVT        (1U << CFG_EVT_ACK_FROM_M0_EVT)
#define EVENT_SYNCHRO_BYPASS_IDLE    (1U << CFG_EVT_SYNCHRO_BYPASS_IDLE)
#define EVENT_TIMER                  (1U << CFG_EVT_TIMER)
/******************************************************************************
 * Configure Log level for Application
 ******************************************************************************/
#define APPLI_CONFIG_LOG_LEVEL          LOG_LEVEL_INFO
#define APPLI_PRINT_FILE_FUNC_LINE      0

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
    CFG_LPM_APP_THREAD,
  /* USER CODE BEGIN CFG_LPM_Id_t */

  /* USER CODE END CFG_LPM_Id_t */
} CFG_LPM_Id_t;

/******************************************************************************
 * OTP manager
 ******************************************************************************/
#define CFG_OTP_BASE_ADDRESS    OTP_AREA_BASE

#define CFG_OTP_END_ADRESS      OTP_AREA_END_ADDR

/******************************************************************************
 * MEMORY MANAGER
 ******************************************************************************/
#define CFG_AMM_VIRTUAL_MEMORY_NUMBER               (1U)
#define CFG_AMM_VIRTUAL_APP_TRACE                   (1U)
#define CFG_AMM_VIRTUAL_APP_TRACE_BUFFER_SIZE       (DBG_TRACE_MSG_QUEUE_SIZE / 4U)

#endif /* APP_CONF_H */
