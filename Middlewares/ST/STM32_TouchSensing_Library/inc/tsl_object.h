/**
  ******************************************************************************
  * @file    tsl_object.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_object.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSL_OBJECT_H
#define __TSL_OBJECT_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_touchkey.h"
#include "tsl_linrot.h"

/* Exported types ------------------------------------------------------------*/

#define TSL_OBJ_TYPE_TKEY_MASK     (0x10) /**< TouchKey object mask */
#define TSL_OBJ_TYPE_LINROT_MASK   (0x20) /**< Linear and Rotary objects mask */
#define TSL_OBJ_TYPE_TRACKNAV_MASK (0x40) /**< TrackPad and NaviPad objects mask */

/** Contains all different kinds of sensors.
  */
typedef enum
{
  TSL_OBJ_TOUCHKEY   = (TSL_OBJ_TYPE_TKEY_MASK + 0),     /**< Normal TouchKey */
  TSL_OBJ_TOUCHKEYB  = (TSL_OBJ_TYPE_TKEY_MASK + 1),     /**< Basic TouchKey */
  TSL_OBJ_LINEAR     = (TSL_OBJ_TYPE_LINROT_MASK + 0),   /**< Normal Linear sensor */
  TSL_OBJ_LINEARB    = (TSL_OBJ_TYPE_LINROT_MASK + 1),   /**< Basic Linear sensor */
  TSL_OBJ_ROTARY     = (TSL_OBJ_TYPE_LINROT_MASK + 2),   /**< Normal Rotary sensor */
  TSL_OBJ_ROTARYB    = (TSL_OBJ_TYPE_LINROT_MASK + 3),   /**< Basic Rotary sensor */
  TSL_OBJ_TRACKPAD   = (TSL_OBJ_TYPE_TRACKNAV_MASK + 0), /**< TrackPad sensor */
  TSL_OBJ_NAVIPAD    = (TSL_OBJ_TYPE_TRACKNAV_MASK + 1)  /**< NaviPad sensor */
} TSL_ObjectType_enum_T;

/** Contains the definition of an Object.
  * Variables of this structure type can be placed in RAM or ROM.
  */
typedef struct
{
  TSL_ObjectType_enum_T  Type; /**< Object type */
  void *Elmt;                  /**< Pointer to the object  */
} TSL_Object_T;

/** Contains the definition of a Group of Objects.
  * Variables of this structure type must be placed in RAM only.
  */
typedef struct
{
  CONST TSL_Object_T     *p_Obj;         /**< Pointer to the first object */
  TSL_tNb_T              NbObjects;      /**< Number of objects in the group */
  TSL_tNb_T              StateMask;      /**< "OR" of all objects state mask */
  TSL_StateChange_enum_T Change;         /**< The State is different from the previous one */
  TSL_tIndex_T           ECS_exec;       /**< Flag for the ECS execution */
#if TSLPRM_ECS_DELAY > 0
  TSL_tIndex_T           ECS_wait;       /**< Flag for the ECS delay */
  TSL_tTick_ms_T         ECS_start_time; /**< Keep the time for the ECS delay */
#endif
} TSL_ObjectGroup_T;

/* Exported functions --------------------------------------------------------*/

void TSL_obj_GroupInit(TSL_ObjectGroup_T *objgrp);
void TSL_obj_GroupProcess(TSL_ObjectGroup_T *objgrp);
void TSL_obj_SetGlobalObj(CONST TSL_Object_T *pobj);

#endif /* __TSL_OBJECT_H */
