/**
 ******************************************************************************
 * @file    app_conf.h
 * @author  MCD Application Team
 * @brief   Application configuration file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONF_H
#define APP_CONF_H

  /* Includes ------------------------------------------------------------------*/
#include "stm32wbxx.h"
#include "stm32wbxx_ll_exti.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_ipcc.h"
#include "stm32wbxx_ll_bus.h"
#include "stm32wbxx_ll_pwr.h"
#include "stm32wbxx_ll_cortex.h"
#include "stm32wbxx_ll_utils.h"
#include "hw.h"

/******************************************************************************
 * Debug
 ******************************************************************************/
/**
 * When set, this resets some hw resources to set the device in the same state than the power up
 * The FW resets only register that may prevent the FW to run properly
 *
 * This shall be set to 0 in a final product
 *
 */
#define CFG_HW_RESET_BY_FW        1

#define CFG_LED_SUPPORTED         1

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

/******************************************************************************
 * Configure Log level for Application
 ******************************************************************************/
#define APPLI_CONFIG_LOG_LEVEL          LOG_LEVEL_INFO
#define APPLI_PRINT_FILE_FUNC_LINE      0

/******************************************************************************
 * UART interfaces
 ******************************************************************************/



#define DBG_TRACE_UART_CFG      hw_lpuart1
#define UART_CLI                hw_uart1


/******************************************************************************
 * Transport Layer
 ******************************************************************************/
/**
 * Queue length of BLE Event
 * This parameter defines the number of asynchronous events that can be stored in the HCI layer before
 * being reported to the application. When a command is sent to the BLE core coprocessor, the HCI layer
 * is waiting for the event with the Num_HCI_Command_Packets set to 1. The receive queue shall be large
 * enough to store all asynchronous events received in between.
 * When CFG_TL_MOST_EVENT_PAYLOAD_SIZE is set to 27, this allow to store three 255 bytes long asynchronous events
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

#endif /* APP_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
