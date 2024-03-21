/**
 ******************************************************************************
 * @file    fmm.c
 * @author  MCD Application Team
 * @brief   Fifo based Memory Manager
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include "fmm.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  FREE,
  RESERVED,
  CLOSED
} BufferStatus_e;

typedef  PACKED_STRUCT {
  uint8_t     BufferStatus;
  FMM_Mem_Id  MemId;
  uint16_t    BufferSize;
} TL_PacketHeader_t;

typedef struct{
  TL_PacketHeader_t   pHeadMemory;
  TL_PacketHeader_t   pEndMemory;
  TL_PacketHeader_t   pCurFreeMemory;
  pFMM_Callback     Callback;
  uint16_t          MemReqCallback;
  uint16_t          CurFreeSize;
  uint16_t          HeadFreeSize;
} MemManager_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MemManager_t MemManager[FMM_MGR_NBR];

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint16_t GetMemAvailable( FMM_Mem_Id Mem_id );
static void CheckCallback( FMM_Mem_Id Mem_id );
static void UpdatePtr( FMM_Mem_Id Mem_id, TL_PacketHeader_t *pPtr, uint16_t *pFreeSizeValue );

/* Functions Definition ------------------------------------------------------*/

FMM_Init( FMM_Mem_Id Mem_id, pFMM_MemAdd pMemAdd, uint16_t MemSize )
{
  MemManager[Mem_id].pCurFreeMemory.BufferStatus = FREE;
  MemManager[Mem_id].pHeadMemory = pMemAdd;
  MemManager[Mem_id].pEndMemory = ((uint8_t*)pMemAdd) + 4*DIVF( MemSize, 4 );
  MemManager[Mem_id].pCurFreeMemory = pMemAdd;
  MemManager[Mem_id].CurFreeSize = ( 4*DIVF( MemSize, 4 ) - sizeof(TL_PacketHeader_t) );
  MemManager[Mem_id].HeadFreeSize = 0;
  MemManager[Mem_id].pCurFreeMemory.MemId = Mem_id;
  MemManager[Mem_id].MemReqCallback = 0;

  return;
}

void FMM_ReleaseBuffer( pFMM_MemAdd pMemAdd )
{
  TL_PacketHeader_t *pbuffer;
  FMM_Mem_Id mem_id;
  uint8_t *pend_head_mem;

  pbuffer = ((TL_PacketHeader_t *)pMemAdd) - 1;
  mem_id = pbuffer->MemId;

  pbuffer->BufferStatus = FREE;

  UpdatePtr( mem_id, MemManager[mem_id].pHeadMemory, MemManager[mem_id].HeadFreeSize );

  pend_head_mem = ((uint8_t*)MemManager[mem_id].pHeadMemory) + MemManager[mem_id].HeadFreeSize + sizeof( TL_PacketHeader_t );

  if (pend_head_mem == ((uint8_t*)MemManager[mem_id].pEndMemory))
  {
    MemManager[mem_id].HeadFreeSize = 0;
    MemManager[mem_id].CurFreeSize = (MemManager[mem_id].pHeadMemory - MemManager[mem_id].pEndMemory) - 1;
    MemManager[mem_id].pCurFreeMemory = MemManager[mem_id].pHeadMemory;
    if (MemManager[mem_id].MemReqCallback)
    {
      MemManager[mem_id].MemReqCallback = 0;
      FMM_Notification(MemManager[mem_id].Callback);
    }
  }
  else
  {
    UpdatePtr( mem_id, MemManager[mem_id].pCurFreeMemory, MemManager[mem_id].CurFreeSize );
    CheckCallback( mem_id );
  }


  return;
}

pFMM_MemAdd FMM_GetBuffer( FMM_Mem_Id Mem_id, uint16_t Size, pFMM_Callback Callback )
{
  pFMM_MemAdd retvalue;

  if( ( 4*DIVC( Size, 4 ) ) <= GetMemAvailable( Mem_id ))
  {
    if ( MemManager[Mem_id].pCurFreeMemory.BufferStatus != FREE )
    {
      FMM_UpdateBufferSize( MemManager[Mem_id].pCurFreeMemory,  MemManager[Mem_id].pCurFreeMemory.BufferSize);
    }
    MemManager[Mem_id].pCurFreeMemory.BufferStatus = RESERVED;
    MemManager[Mem_id].pCurFreeMemory.BufferSize = ( 4*DIVC( Size, 4 ) );
    MemManager[Mem_id].CurFreeSize -= ( 4*DIVC( Size, 4 ) );
    retvalue = (pFMM_MemAdd)(MemManager[Mem_id].pCurFreeMemory + 1) ;
  }
  else
  {
    MemManager[Mem_id].MemReqCallback = ( 4*DIVC( Size, 4 ) );
    MemManager[Mem_id].Callback = Callback;
    retvalue = 0;
  }

  return ( retvalue);
}

void FMM_UpdateBufferSize( pFMM_MemAdd pMemAdd, uint16_t Size)
{
  TL_PacketHeader_t *pbuffer;
  FMM_Mem_Id mem_id;

  pbuffer = ((TL_PacketHeader_t *)pMemAdd) - 1;
  mem_id = pbuffer->MemId;

  if(pbuffer->BufferStatus == RESERVED)
  {
    pbuffer->BufferStatus = CLOSED;
    MemManager[mem_id].CurFreeSize += (pbuffer->BufferSize +  ( (4*DIVC( Size, 4 ) ));
    pbuffer->BufferSize = ( 4*DIVC( Size, 4 ) );

    if(MemManager[mem_id].CurFreeSize)
    {
      (uint8_t*)MemManager[mem_id].pCurFreeMemory += ( 4*DIVC( Size, 4 ) );
      MemManager[mem_id].pCurFreeMemory.BufferStatus = FREE;
    }
    else
    {
      MemManager[mem_id].pCurFreeMemory =  MemManager[mem_id].pHeadMemory;
      MemManager[mem_id].CurFreeSize = MemManager[mem_id].HeadFreeSize;
      MemManager[mem_id].HeadFreeSize = 0;
    }
  }

  return;
}

/**********************************************************************************************************************/

static uint16_t GetMemAvailable( FMM_Mem_Id Mem_id )
{
  uint16_t ret_size;
  uint8_t *pend_cur_free_mem;

  pend_cur_free_mem = ((uint8_t*)MemManager[Mem_id].pCurFreeMemory) + MemManager[Mem_id].CurFreeSize + sizeof( TL_PacketHeader_t );

  if (pend_cur_free_mem == ((uint8_t*)MemManager[Mem_id].pEndMemory))
  {
    ret_size = MAX( MemManager[Mem_id].CurFreeSize, MemManager[Mem_id].HeadFreeSize );
  }
  else
  {
    ret_size = MemManager[Mem_id].CurFreeSize;
  }

  return ( ret_size );
}

static void CheckCallback( FMM_Mem_Id Mem_id )
{
  uint16_t mem_available;

  if(MemManager[Mem_id].MemReqCallback)
  {
    mem_available = GetMemAvailable( Mem_id );
    if(MemManager[Mem_id].MemReqCallback <= mem_available)
    {
      MemManager[Mem_id].MemReqCallback = 0;
      FMM_Notification(MemManager[Mem_id].Callback);
    }
  }
  return;
}

static void UpdatePtr( FMM_Mem_Id Mem_id, TL_PacketHeader_t *pPtr, uint16_t *pFreeSizeValue )
{
  TL_PacketHeader_t *pmem;
  uint8_t update_done = FALSE;

  while(update_done == FALSE)
  {
    (uint8_t*)pmem = ((uint8_t*)pPtr) + (*pFreeSizeValue);

    if ((pmem + sizeof( TL_PacketHeader_t )) != ((uint8_t*)MemManager[Mem_id].pEndMemory))
    {
      if(pmem->BufferStatus == FREE)
      {
        (*pFreeSizeValue) += pmem->BufferSize;
      }
      else
      {
        update_done = TRUE;
      }
    }
    else
    {
      update_done = TRUE;
    }
  }

  return;
}


