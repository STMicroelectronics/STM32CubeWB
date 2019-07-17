/**
 ******************************************************************************
 * @file    stm32_seq.c
 * @author  MCD Application Team
 * @brief   Simple sequencer implementation
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
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32_seq.h"
#include "utilities_conf.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
uint32_t priority;
uint32_t round_robin;
} UTIL_SEQ_Priority_t;

/* Private defines -----------------------------------------------------------*/
#define UTIL_SEQ_NO_BIT_SET   (0)
#define UTIL_SEQ_ALL_BIT_SET    (~0)

#ifndef UTIL_SEQ_INIT_CRITICAL_SECTION
  #define UTIL_SEQ_INIT_CRITICAL_SECTION( )
#endif

#ifndef UTIL_SEQ_ENTER_CRITICAL_SECTION
  #define UTIL_SEQ_ENTER_CRITICAL_SECTION( )   UTILS_ENTER_CRITICAL_SECTION( )
#endif

#ifndef UTIL_SEQ_EXIT_CRITICAL_SECTION
  #define UTIL_SEQ_EXIT_CRITICAL_SECTION( )     UTILS_EXIT_CRITICAL_SECTION( )
#endif

/*default number of task is default 32 (maximum), can be reduced by redefining in utilities_conf.h*/
#ifndef UTIL_SEQ_CONF_TASK_NBR 
	#define UTIL_SEQ_CONF_TASK_NBR  (32)
#endif 

#if UTIL_SEQ_CONF_TASK_NBR > 32
#error "UTIL_SEQ_CONF_PRIO_NBR must be less of equal then 32"
#endif
  
#ifndef UTIL_SEQ_CONF_PRIO_NBR 
  #define UTIL_SEQ_CONF_PRIO_NBR  (2)
#endif

#ifndef UTIL_SEQ_MEMSET8
#define UTIL_SEQ_MEMSET8( dest, value, size )   UTILS_MEMSET8( dest, value, size )
#endif

/* Private variables ---------------------------------------------------------*/

static UTIL_SEQ_bm_t TaskSet = UTIL_SEQ_NO_BIT_SET;
static UTIL_SEQ_bm_t TaskMask = UTIL_SEQ_ALL_BIT_SET;
static UTIL_SEQ_bm_t SuperMask = UTIL_SEQ_ALL_BIT_SET;
static UTIL_SEQ_bm_t EvtSet = UTIL_SEQ_NO_BIT_SET;
static UTIL_SEQ_bm_t EvtWaited = UTIL_SEQ_NO_BIT_SET;
static uint32_t CurrentTaskIdx = 0;
static void (*TaskCb[UTIL_SEQ_CONF_TASK_NBR])( void );
static UTIL_SEQ_Priority_t TaskPrio[UTIL_SEQ_CONF_PRIO_NBR] = { 0 };

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t bit_position(uint32_t value);

/* Functions Definition ------------------------------------------------------*/
void UTIL_SEQ_Init( void )
{
  TaskSet = UTIL_SEQ_NO_BIT_SET;
  TaskMask = UTIL_SEQ_ALL_BIT_SET;
  SuperMask = UTIL_SEQ_ALL_BIT_SET;
  EvtSet = UTIL_SEQ_NO_BIT_SET;
  EvtWaited = UTIL_SEQ_NO_BIT_SET;
  CurrentTaskIdx = 0;
  UTIL_SEQ_MEMSET8(TaskCb, 0, sizeof(TaskCb));
  UTIL_SEQ_MEMSET8(TaskPrio, 0, sizeof(TaskPrio));
  UTIL_SEQ_INIT_CRITICAL_SECTION( );
}

void UTIL_SEQ_DeInit( void )
{
}

/**
 * This function can be nested.
 * That is the reason why many variables that are used only in that function are declared static.
 * Note: These variables could have been declared static in the function.
 */
void UTIL_SEQ_Run( UTIL_SEQ_bm_t mask_bm )
{
  uint32_t counter;
  UTIL_SEQ_bm_t current_task_set;
  UTIL_SEQ_bm_t super_mask_backup;

  /**
   *  When this function is nested, the mask to be applied cannot be larger than the first call
   *  The mask is always getting smaller and smaller
   *  A copy is made of the mask set by UTIL_SEQ_Run() in case it is called again in the task
   */
  super_mask_backup = SuperMask;
  SuperMask &= mask_bm;

  /**
   * There are two independent mask to check:
   * TaskMask that comes from UTIL_SEQ_PauseTask() / UTIL_SEQ_ResumeTask
   * SuperMask that comes from UTIL_SEQ_Run
   * If the waited event is there, exit from  UTIL_SEQ_Run() to return to the
   * waiting task
   */
  while( (TaskSet & TaskMask & SuperMask) && (!(EvtSet & EvtWaited)) )
  {
    counter = 0;
    /**
     * When a flag is set, the associated bit is set in TaskPrio[counter].priority mask depending
     * on the priority parameter given from UTIL_SEQ_SetTask()
     * The while loop is looking for a flag set from the highest priority maskr to the lower
     */
    while(!(TaskPrio[counter].priority & TaskMask & SuperMask))
    {
      counter++;
    }

    current_task_set = TaskPrio[counter].priority & TaskMask & SuperMask;

    /**
     * The round_robin register is a mask of allowed flags to be evaluated.
     * The concept is to make sure that on each round on UTIL_SEQ_Run(), if two same flags are always set,
     * the sequencer does not run always only the first one.
     * When a task has been executed, The flag is removed from the round_robin mask.
     * If on the next UTIL_SEQ_RUN(), the two same flags are set again, the round_robin mask will mask out the first flag
     * so that the second one can be executed.
     * Note that the first flag is not removed from the list of pending task but just masked by the round_robin mask
     *
     * In the check below, the round_robin mask is reinitialize in case all pending tasks haven been executed at least once
     */
    if (!(TaskPrio[counter].round_robin & current_task_set))
    {
      TaskPrio[counter].round_robin = UTIL_SEQ_ALL_BIT_SET;
    }

    /** Read the flag index of the task to be executed
	 *  Once the index is read, the associated task will be executed even though a higher priority stack is requested
	 *  before task execution.
	 */
    CurrentTaskIdx = bit_position(current_task_set & TaskPrio[counter].round_robin);

    /** remove from the roun_robin mask the task that has been selected to be executed */
    TaskPrio[counter].round_robin &= ~(1 << (CurrentTaskIdx));

    UTIL_SEQ_ENTER_CRITICAL_SECTION( );
    /** remove from the list or pending task the one that has been selected to be executed */
    TaskSet &= ~(1 << (CurrentTaskIdx));
    /** remove from all priority mask the task that has been selected to be executed */
    for (counter = UTIL_SEQ_CONF_PRIO_NBR; counter; counter--)
    {
      TaskPrio[counter - 1].priority &= ~(1 << (CurrentTaskIdx));
    }
    UTIL_SEQ_EXIT_CRITICAL_SECTION( );
    /** Execute the task */
    TaskCb[CurrentTaskIdx]( );
  }

  UTIL_SEQ_PreIdle( );
  
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );
  if (!((TaskSet & TaskMask & SuperMask) || (EvtSet & EvtWaited)))
  {
    UTIL_SEQ_Idle( );
  }
  UTIL_SEQ_EXIT_CRITICAL_SECTION( );
  
  UTIL_SEQ_PostIdle( );

  /** restore the mask from UTIL_SEQ_Run() */
  SuperMask = super_mask_backup;

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_RegTask( UTIL_SEQ_bm_t task_id_bm , uint32_t flags, void (*task)( void ) )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  TaskCb[bit_position(task_id_bm)] = task;

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_SetTask( UTIL_SEQ_bm_t task_id_bm , uint32_t task_prio )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  TaskSet |= task_id_bm;
  TaskPrio[task_prio].priority |= task_id_bm;

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_PauseTask( UTIL_SEQ_bm_t task_id_bm )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  TaskMask &= (~task_id_bm);

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_ResumeTask( UTIL_SEQ_bm_t task_id_bm )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  TaskMask |= task_id_bm;

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_SetEvt( UTIL_SEQ_bm_t evt_id_bm )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  EvtSet |= evt_id_bm;

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_ClrEvt( UTIL_SEQ_bm_t evt_id_bm )
{
  UTIL_SEQ_ENTER_CRITICAL_SECTION( );

  EvtSet &= (~evt_id_bm);

  UTIL_SEQ_EXIT_CRITICAL_SECTION( );

  return;
}

/**
 *  this function can be nested
 */
void UTIL_SEQ_WaitEvt( UTIL_SEQ_bm_t evt_id_bm )
{
  UTIL_SEQ_bm_t event_waited_id_backup;
  UTIL_SEQ_bm_t current_task_id_bm;

  /** store in local the current_task_id_bm as the global variable CurrentTaskIdx
   *  may be overwritten in case there are nested call of UTIL_SEQ_Run()
   */
  current_task_id_bm = (1 << CurrentTaskIdx);

  /** backup the event id that was currently waited */
  event_waited_id_backup = EvtWaited;
  EvtWaited = evt_id_bm;
  /**
   *  wait for the new event
   *  note: that means that if the previous waited event occurs, it will not exit
   *  the while loop below.
   *  The system is waiting only for the last waited event.
   *  When it will go out, it will wait again fro the previous one.
   *  It case it occurs while waiting for the second one, the while loop will exit immediately
   */
  while((EvtSet & EvtWaited) == 0)
  {
    UTIL_SEQ_EvtIdle(current_task_id_bm, EvtWaited);
  }
  EvtSet &= (~EvtWaited);
  EvtWaited = event_waited_id_backup;

  return;
}

/**
 *  this function can be nested
 */
UTIL_SEQ_bm_t UTIL_SEQ_IsEvtPend( void )
{
  return (EvtSet & EvtWaited);
}

__WEAK void UTIL_SEQ_EvtIdle( uint32_t UTIL_SEQ_bm_t, uint32_t evt_waited_bm )
{
  /**
   * Execute sequencer if not implemented by the application
   * By default, all tasks are moved out of the sequencer
   * Only UTIL_SEQ_Idle() will be called in critical section
   */
  UTIL_SEQ_Run( UTIL_SEQ_NO_BIT_SET );

  return;
}

__WEAK void UTIL_SEQ_Idle( void )
{
  /**
   * Stay in run mode if not implemented by the application
   */
  return;
}

__WEAK void UTIL_SEQ_PreIdle( void )
{
  /**
   * Unless specified by the application, there is nothing to be done
   */
  return;
}

__WEAK void UTIL_SEQ_PostIdle( void )
{
  /**
   * Unless specified by the application, there is nothing to be done
   */
  return;
}

#if( __CORTEX_M == 0)
static const uint8_t clz_table_4bit[16] = { 4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };	
static uint32_t bit_position(uint32_t value)
{

  uint32_t n = 0;

  if ((value & 0xFFFF0000) == 0)  { n  = 16; value <<= 16;  }
  if ((value & 0xFF000000) == 0)  { n +=  8; value <<=  8;  }
  if ((value & 0xF0000000) == 0)  { n +=  4; value <<=  4;  }

  n += (uint32_t)clz_table_4bit[value >> (32-4)];

  return (31-n);
}
#else
static uint32_t bit_position(uint32_t value)
{
  return (31 -__CLZ( value ));
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
