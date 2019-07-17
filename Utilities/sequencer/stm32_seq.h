/**
 ******************************************************************************
 * @file    stm32_seq.h
 * @author  MCD Application Team
 * @brief   sequencer interface
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_SEQ_H
#define __STM32_SEQ_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
  typedef uint32_t  UTIL_SEQ_bm_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/**
 * This should be used in the application
 *
 *    UTIL_SEQ_RegTask( task_id_bm, UTIL_SEQ_RFU, task );;
 *
 * This provides a default value for unused parameter
 *
 */
#define UTIL_SEQ_RFU 0

/**
 * This should be used in the application
 * while(1)
 * {
 *    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
 * }
 *
 * This informs the sequencer that all tasks registered shall be considered
 *
 */
#define UTIL_SEQ_DEFAULT         (~0)

/* Exported functions ------------------------------------------------------- */
/**
 * @brief  This API Initializes the SEQ resources.
 * @param  None
 * @retval None
 */
void UTIL_SEQ_Init( void );

/**
 * @brief  This API Un-Initializes the SEQ resources.
 * @param  None
 * @retval None
 */
void UTIL_SEQ_DeInit( void );



/**
 * @brief This is called by the sequencer in critical section (PRIMASK bit) when
 *          - there is no more tasks to be executed
 *          AND
 *          - there is no pending event or the pending event is still not set
 *        The application should enter low power mode in this function
 *        When this function is not implemented by the application, the sequencer keeps running a while loop (RUN MODE)
 *
 * @param  None
 * @retval None
 */
void UTIL_SEQ_Idle( void );

/**
 * @brief This is called by the sequencer outside critical section just before calling UTIL_SEQ_Idle( )
 *        UTIL_SEQ_PreIdle() is considered as the last task executed before calling UTIL_SEQ_Idle( )
 *        In case a task or an event is set from an interrupt handler just after UTIL_SEQ_PreIdle() is called,
 *        UTIL_SEQ_Idle() will not be called.
 *
 * @param  None
 * @retval None
 */
void UTIL_SEQ_PreIdle( void );

/**
 * @brief This is called by the sequencer outside critical section either
 *        - after calling UTIL_SEQ_Idle( )
 *        OR
 *        - after calling UTIL_SEQ_PreIdle( ) in case UTIL_SEQ_Idle() in case a task or an event has been
 *          requested after UTIL_SEQ_PreIdle() has been called.
 *
 *        Note: UTIL_SEQ_PostIdle() is always called if UTIL_SEQ_PreIdle() has been called and never called otherwise
 *
 * @param  None
 * @retval None
 */
void UTIL_SEQ_PostIdle( void );

/**
 * @brief  This requests the sequencer to execute all pending tasks using round robin mechanism.
 *         When no task are pending, it calls UTIL_SEQ_Idle();
 *         This function should be called in a while loop in the application
 *
 * @param  mask_bm: this is the list of task (bit mapping) that is be kept in the sequencer list
 * @retval None
 */
void UTIL_SEQ_Run( UTIL_SEQ_bm_t mask_bm );

/**
 * @brief This registers a task in the sequencer.
 *
 * @param  task_id_bm: The Id of the task
 *         It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @param flags: Flags are reserved param for future use
 * @param task: Reference of the function to be executed
 *
 * @retval None
 */
void UTIL_SEQ_RegTask( UTIL_SEQ_bm_t task_id_bm, uint32_t flags, void (*task)( void ) );

/**
 * @brief  Request a task to be executed
 *
 * @param  task_id_bm: The Id of the task
 *         It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @param  task_prio: The priority of the task
 *         It shall an number from  0 (high priority) to 31 (low priority)
 *         The priority is checked each time the sequencer needs to select a new task to execute
 *         It does not permit to preempt a running task with lower priority
 * @retval None
 */
void UTIL_SEQ_SetTask( UTIL_SEQ_bm_t task_id_bm , uint32_t task_prio );

/**
 * @brief Prevents a task to be called by the sequencer even when set with UTIL_SEQ_SetTask()
 *        By default, all tasks are executed by the sequencer when set with UTIL_SEQ_SetTask()
 *        When a task is paused, it is moved out from the sequencer list
 *
 * @param  task_id_bm: The Id of the task
 *         It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @retval None
 */
void UTIL_SEQ_PauseTask( UTIL_SEQ_bm_t task_id_bm );

/**
 * @brief Allows a task to be called by the sequencer if set with UTIL_SEQ_SetTask()
 *        By default, all tasks are executed by the sequencer when set with UTIL_SEQ_SetTask()
 *        This is used in relation with UTIL_SEQ_PauseTask() to get back in the sequencer list a task that has been
 *        moved out
 *
 * @param  task_id_bm: The Id of the task
 *         It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @retval None
 */
void UTIL_SEQ_ResumeTask( UTIL_SEQ_bm_t task_id_bm );

/**
 * @brief It sets an event that is waited with UTIL_SEQ_WaitEvt()
 *
 * @param evt_id_bm
 *        It shall be a bit mapping where only 1 bit is set
 * @retval None
 */
void UTIL_SEQ_SetEvt( UTIL_SEQ_bm_t evt_id_bm );

/**
 * @brief This API may be used to clear the event before calling UTIL_SEQ_WaitEvt()
 *        This API may be useful when the UTIL_SEQ_SetEvt() is called several time to notify the same event.
 *        Due to Software Architecture where the timings are hard to control, this may be an unwanted case.
 *
 * @param evt_id_bm
 *        It shall be a bit mapping where only 1 bit is set
 * @retval None
 */
void UTIL_SEQ_ClrEvt( UTIL_SEQ_bm_t evt_id_bm );

/**
 * @brief It waits for a specific event to be set. The sequencer loops UTIL_SEQ_EvtIdle() until the event is set
 *        When called recursively, it acts as a First in / Last out mechanism. The sequencer waits for the
 *        last event requested to be set even though one of the already requested event has been set.
 *
 * @param evt_id_bm
 *        It shall be a bit mapping where only 1 bit is set
 * @retval None
 */
void UTIL_SEQ_WaitEvt( UTIL_SEQ_bm_t evt_id_bm );

/**
 * @brief This API returns whether the waited event is pending or not
 *        It is useful only when the UTIL_SEQ_EvtIdle() is overloaded by the application. In that case, when the low
 *        power mode needs to be executed, the application shall first check whether the waited event is pending
 *        or not. Both the event checking and the low power mode processing should be done in critical section
 *
 * @param   None
 * @retval  0 when the waited event is not there or the evt_id when the waited event is pending
 */
UTIL_SEQ_bm_t UTIL_SEQ_IsEvtPend( void );

/**
 * @brief The sequencer loops in that function until the waited event is set
 *        The application may either enter low power mode or call UTIL_SEQ_Run()
 *        When not implemented by the application, it calls UTIL_SEQ_Run(0) which means all tasks are removed from
 *        sequencer list and only UTIL_SEQ_Idle() is called. In that case, only low power mode is executed.
 *
 * @param  task_id_bm: The task id that is currently running. When task_id_bm = 0, it means UTIL_SEQ_WaitEvt( )
 *                     has been called outside a registered task (ie at startup before UTIL_SEQ_Run( ) has been called
 * @param  evt_waited_bm: The event id that is waited.
 * @retval None
 */
void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm );

#ifdef __cplusplus
}
#endif

#endif /*__STM32_SEQ_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
