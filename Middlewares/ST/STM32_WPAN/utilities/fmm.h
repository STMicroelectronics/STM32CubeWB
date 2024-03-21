/**
 ******************************************************************************
 * @file    fmm.h
 * @author  MCD Application Team
 * @brief   Header for Fifo based Memory Manager
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMM_H
#define __FMM_H

/* Includes ------------------------------------------------------------------*/
/* Exported defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef uint8_t   FMM_Mem_Id;
typedef uint32_t*  pFMM_MemAdd;
typedef void ( *pFMM_Callback )( void );

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
FMM_Init( FMM_Mem_Id Mem_id, pFMM_MemAdd pMemAdd, uint16_t MemSize );
pFMM_MemAdd FMM_GetBuffer( FMM_Mem_Id Mem_id, uint16_t Size, pFMM_Callback Callback );
void FMM_UpdateBufferSize( pFMM_MemAdd pMemAdd, uint16_t Size);
void FMM_ReleaseBuffer( pFMM_MemAdd pMemAdd );
void FMM_Notification( pFMM_Callback pCallback );

#endif /*__FMM_H */


