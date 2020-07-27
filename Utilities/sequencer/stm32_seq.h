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
#ifndef STM32_SEQ_H
#define STM32_SEQ_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
  
/** @defgroup SEQUENCER sequencer utilities
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup SEQUENCER_Exported_type SEQUENCER exported types
 *  @{
 */
/**
 *  @brief  bit mapping of the task.
 *  this value is used to represent a list of task (each corresponds to a task).
 */

typedef uint32_t UTIL_SEQ_bm_t;

/**
  * @}
 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup SEQUENCER_Exported_const SEQUENCER exported constants
 *  @{
 */

/**
 * @brief This provides a default value for unused parameter
 *
 */
#define UTIL_SEQ_RFU 0

/**
 * @brief Default value used to start the scheduling.
 *
 * This informs the sequencer that all tasks registered shall be considered
 *
 * @note
 * This should be used in the application\n
 * while(1)\n
 * {\n
 *    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );\n
 * }\n
 *
 */
#define UTIL_SEQ_DEFAULT         (~0U)

/**
  * @}
 */

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup SEQUENCER_Exported_function SEQUENCER exported functions
 *  @{
 */

/**
 * @brief  This function initializes the sequencer resources.
 *
 */
void UTIL_SEQ_Init( void );

/**
 * @brief  This function un-initializes the sequencer resources.
 *
 */
void UTIL_SEQ_DeInit( void );

/**
 * @brief This function is called by the sequencer in critical section (PRIMASK bit) when
 *          - there are no more tasks to be executed
 *          AND
 *          - there are no pending event or the pending event is still not set
 * @note  The application should enter low power mode in this function
 *        When this function is not implemented by the application, the sequencer keeps running a while loop (RUN MODE)
 *
 */
void UTIL_SEQ_Idle( void );

/**
 * @brief This function is called by the sequencer outside critical section just before calling UTIL_SEQ_Idle( )
 *        UTIL_SEQ_PreIdle() is considered as the last task executed before calling UTIL_SEQ_Idle( )
 *        In case a task or an event is set from an interrupt handler just after UTIL_SEQ_PreIdle() is called,
 *        UTIL_SEQ_Idle() will not be called.
 *
 */
void UTIL_SEQ_PreIdle( void );

/**
 * @brief This function is called by the sequencer outside critical section either
 *        - after calling UTIL_SEQ_Idle( )
 *        OR
 *        - after calling UTIL_SEQ_PreIdle( ) without call to UTIL_SEQ_Idle() due to an incoming task set or event
 *          requested after UTIL_SEQ_PreIdle() has been called.
 *
 *        Note: UTIL_SEQ_PostIdle() is always called if UTIL_SEQ_PreIdle() has been called and never called otherwise
 *
 */
void UTIL_SEQ_PostIdle( void );

/**
 * @brief This function requests the sequencer to execute all pending tasks using round robin mechanism.
 *        When no task are pending, it calls UTIL_SEQ_Idle();
 *        This function should be called in a while loop in the application
 *
 * @param Mask_bm list of task (bit mapping) that is be kept in the sequencer list.
 *
 */
void UTIL_SEQ_Run( UTIL_SEQ_bm_t Mask_bm );

/**
 * @brief This function registers a task in the sequencer.
 *
 * @param TaskId_bm The Id of the task
 * @param Flags Flags are reserved param for future use
 * @param Task Reference of the function to be executed
 *
 */
void UTIL_SEQ_RegTask( UTIL_SEQ_bm_t TaskId_bm, uint32_t Flags, void (*Task)( void ) );

/**
 * @brief This function requests a task to be executed
 *
 * @param TaskId_bm The Id of the task
 *        It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @param Task_Prio The priority of the task
 *        It shall a number from  0 (high priority) to 31 (low priority)
 *        The priority is checked each time the sequencer needs to select a new task to execute
 *        It does not permit to preempt a running task with lower priority
 *
 */
void UTIL_SEQ_SetTask( UTIL_SEQ_bm_t TaskId_bm , uint32_t Task_Prio );

/**
 * @brief This function checks if a task could be scheduled.
 *
 * @param TaskId_bm The Id of the task
 *        It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 * @retval 0 if not 1 if true
 */
uint32_t UTIL_SEQ_IsSchedulableTask( UTIL_SEQ_bm_t TaskId_bm);

/**
 * @brief This function prevents a task to be called by the sequencer even when set with UTIL_SEQ_SetTask()
 *        By default, all tasks are executed by the sequencer when set with UTIL_SEQ_SetTask()
 *        When a task is paused, it is moved out from the sequencer list
 *
 * @param TaskId_bm The Id of the task
 *        It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 *
 */
void UTIL_SEQ_PauseTask( UTIL_SEQ_bm_t TaskId_bm );

/**
 * @brief This function allows to know if the task has been put in pause.
 *        By default, all tasks are executed by the sequencer when set with UTIL_SEQ_SetTask()
 *        The exit of the pause shall be done by the function UTIL_SEQ_ResumeTask.
 *
 * @param TaskId_bm The Id of the task
 *        It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 *
 */
uint32_t UTIL_SEQ_IsPauseTask( UTIL_SEQ_bm_t TaskId_bm );

/**
 * @brief This function allows again a task to be called by the sequencer if set with UTIL_SEQ_SetTask()
 *        This is used in relation with UTIL_SEQ_PauseTask()
 *
 * @param TaskId_bm The Id of the task
 *        It shall be (1<<task_id) where task_id is the number assigned when the task has been registered
 *
 */
void UTIL_SEQ_ResumeTask( UTIL_SEQ_bm_t TaskId_bm );

/**
 * @brief This function sets an event that is waited with UTIL_SEQ_WaitEvt()
 *
 * @param EvtId_bm event id bit mask
 *
 * @note an event shall be a 32 bit mapping where only 1 bit is set
 *
 */
void UTIL_SEQ_SetEvt( UTIL_SEQ_bm_t EvtId_bm );

/**
 * @brief This function may be used to clear the event before calling UTIL_SEQ_WaitEvt()
 *        This API may be useful when the UTIL_SEQ_SetEvt() is called several time to notify the same event.
 *        Due to Software Architecture where the timings are hard to control, this may be an unwanted case.
 *
 * @param EvtId_bm event id bm
 *        It shall be a bit mapping where only 1 bit is set
 *
 */
void UTIL_SEQ_ClrEvt( UTIL_SEQ_bm_t EvtId_bm );

/**
 * @brief This function waits for a specific event to be set. The sequencer loops UTIL_SEQ_EvtIdle() until the event is set
 *        When called recursively, it acts as a First in / Last out mechanism. The sequencer waits for the
 *        last event requested to be set even though one of the already requested event has been set.
 *
 * @param EvtId_bm event id bit mask
 *        It shall be a bit mapping where only 1 bit is set
 *
 */
void UTIL_SEQ_WaitEvt( UTIL_SEQ_bm_t EvtId_bm );

/**
 * @brief This function returns whether the waited event is pending or not
 *        It is useful only when the UTIL_SEQ_EvtIdle() is overloaded by the application. In that case, when the low
 *        power mode needs to be executed, the application shall first check whether the waited event is pending
 *        or not. Both the event checking and the low power mode processing should be done in critical section
 *
 * @retval  0 when the waited event is not there or the evt_id when the waited event is pending
 */
UTIL_SEQ_bm_t UTIL_SEQ_IsEvtPend( void );

/**
 * @brief This function loops until the waited event is set
 * @param  TaskId_bm The task id that is currently running. When task_id_bm = 0, it means UTIL_SEQ_WaitEvt( )
 *                     has been called outside a registered task (ie at startup before UTIL_SEQ_Run( ) has been called
 * @param  EvtWaited_bm The event id that is waited.
 *
 * @note
 *        When not implemented by the application, it calls UTIL_SEQ_Run(~TaskId_bm) which means the waited
 *        task is suspended until the waited event and the other tasks are running or the application enter
 *        low power mode.
 *        Else the user can redefine his own function for example call sequencer UTIL_SEQ_Run(0) to suspend all
 *        the task and let the sequencer enter the low power mode.
 */
void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t TaskId_bm, UTIL_SEQ_bm_t EvtWaited_bm );

/**
  * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__STM32_SEQ_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
