/*****************************************************************************
 * @file    ee.c
 * @author  MCD Application Team
 * @brief   This file contains the implementation of the EEPROM emulator
 *          for the STM32WB platform.
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#include "ee_cfg.h"
#include "ee.h"

/*****************************************************************************/

/* Page header size in bytes (4 words used to save page state) */
#define EE_HEADER_SIZE             (4 * HW_FLASH_WIDTH)

/* Maximum number of elements by page */
#define EE_NB_MAX_ELT \
          ((HW_FLASH_PAGE_SIZE - EE_HEADER_SIZE) / HW_FLASH_WIDTH)

/* Flash words special values */
#define EE_ERASED                  0xFFFFFFFFFFFFFFFFULL
#define EE_PROGRAMMED              0x5555555555555555ULL

/* No page define */
#define EE_PAGE_NOT_FOUND          0xFFFFFFFFUL

/* Element tag in flash */
#define EE_TAG                     0x8000UL

/* Page state definition */
enum
{
  EE_STATE_ERASED  = 0,   /* page is erased */
  EE_STATE_RECEIVE = 1,   /* used during transfer for new page */
  EE_STATE_ACTIVE  = 2,   /* page contains valid data and is not full */
  EE_STATE_VALID   = 3,   /* page contains valid data and is full */
  EE_STATE_ERASING = 4,   /* used during transfer for old pages */
};

/* Macro to get flash address from page index */
#define EE_FLASH_ADDR( pv, p ) \
          ((pv)->address + ((p) * HW_FLASH_PAGE_SIZE))

/* Macro to get flash page from page index */
#define EE_FLASH_PAGE( pv, p ) \
          ((((pv)->address - HW_FLASH_ADDRESS) / HW_FLASH_PAGE_SIZE) + (p))

/* Macro to get first page index of following pool, among circular pool list */
#define EE_NEXT_POOL( pv ) \
           (((pv)->current_write_page < (pv)->nb_pages) ? (pv)->nb_pages : 0)

/* Check Configuration */
#ifndef CFG_EE_BANK0_SIZE
#define CFG_EE_BANK0_SIZE          (2 * HW_FLASH_PAGE_SIZE)
#endif
#ifndef CFG_EE_BANK1_SIZE
#define CFG_EE_BANK1_SIZE          0
#endif
#if (CFG_EE_BANK0_SIZE & ((2 * HW_FLASH_PAGE_SIZE) - 1))
#error EE: wrong value of CFG_EE_BANK0_SIZE
#endif
#if (CFG_EE_BANK1_SIZE & ((2 * HW_FLASH_PAGE_SIZE) - 1))
#error EE: wrong value of CFG_EE_BANK1_SIZE
#endif
#if ((CFG_EE_BANK0_MAX_NB > \
      EE_NB_MAX_ELT * CFG_EE_BANK0_SIZE / (2 * HW_FLASH_PAGE_SIZE)) || \
     (CFG_EE_BANK0_MAX_NB > 0x4000U))
#error EE: CFG_EE_BANK0_MAX_NB too big
#endif
#if ((CFG_EE_BANK1_SIZE > 0) && \
     ((CFG_EE_BANK1_MAX_NB > \
       EE_NB_MAX_ELT * CFG_EE_BANK1_SIZE / (2 * HW_FLASH_PAGE_SIZE)) || \
      (CFG_EE_BANK1_MAX_NB > 0x4000U)))
#error EE: CFG_EE_BANK1_MAX_NB too big
#endif
#if (HW_FLASH_WIDTH != 8)
#error EE: this module only works for a 64-bit flash
#endif

/* Macro to get a 64-bit pointer from an address represented as an integer */
#ifndef EE_PTR
#define EE_PTR( x )               ((uint64_t*)(uintptr_t)(x))
#endif /* !EE_PTR */

/* Macro used for debug (empty by default) */
#ifndef EE_DBG
#define EE_DBG( x )
#endif /* !EE_DBG */

/* Definition of global variables structure */
typedef struct
{
  /* Absolute base address of the bank in Flash memory */
  uint32_t address;

  /* Total number of pages in a pool (constant) */
  uint8_t  nb_pages;

  /* Current write page (can be ACTIVE or RECEIVE state) */
  uint8_t  current_write_page;

  /* Total number of elements written in valid and active pages */
  uint16_t nb_written_elements;

  /* Write position inside the current write page */
  uint16_t next_write_offset;

} EE_var_t;

/*****************************************************************************/

/* Local functions */

static void EE_Reset( EE_var_t* pv, uint32_t address, uint8_t nb_pages );

static int EE_Recovery( EE_var_t* pv );

static int EE_Transfer( EE_var_t* pv, uint16_t addr, uint32_t page );

static int EE_WriteEl( EE_var_t* pv, uint16_t addr, uint32_t data );

static int EE_ReadEl( const EE_var_t* pv,
                      uint16_t addr, uint32_t* data, uint32_t page );

static int EE_SetState( const EE_var_t* pv, uint32_t page, uint32_t state );

static uint32_t EE_GetState( const EE_var_t* pv, uint32_t page );

static uint16_t EE_Crc( uint64_t v );

/*****************************************************************************/

/* Global variables */

EE_var_t EE_var[CFG_EE_BANK1_SIZE ? 2 : 1];

/*****************************************************************************/

int EE_Init( int format, uint32_t base_address )
{
  int status;
  uint16_t total_nb_pages;

  /* Reset global variables of both banks */

  EE_Reset( &EE_var[0],
            base_address,
            CFG_EE_BANK0_SIZE / (2 * HW_FLASH_PAGE_SIZE) );

  if ( CFG_EE_BANK1_SIZE )
  {
    EE_Reset( &EE_var[1],
              base_address + CFG_EE_BANK0_SIZE,
              CFG_EE_BANK1_SIZE / (2 * HW_FLASH_PAGE_SIZE) );
  }

  /* If format mode is set, start from scratch */

  if ( format )
  {
    /* Force erase of all pages */
    total_nb_pages =
      2 * (EE_var[0].nb_pages + (CFG_EE_BANK1_SIZE ? EE_var[1].nb_pages : 0));

    if ( HW_FLASH_Erase( EE_FLASH_PAGE( EE_var, 0 ), total_nb_pages, 0 ) != 0 )
    {
      return EE_ERASE_ERROR;
    }

    /* Set first page of each pool in ACTIVE State */
    status = EE_SetState( &EE_var[0], 0, EE_STATE_ACTIVE );

    if ( CFG_EE_BANK1_SIZE && (status == EE_OK) )
    {
      status = EE_SetState( &EE_var[1], 0, EE_STATE_ACTIVE );
    }

    return status;
  }

  /* else, try to recover the EEPROM emulation state from flash */

  status = EE_Recovery( &EE_var[0] );

  if ( CFG_EE_BANK1_SIZE && (status == EE_OK) )
  {
    status = EE_Recovery( &EE_var[1] );
  }

  return status;
}

/*****************************************************************************/

int EE_Read( int bank, uint16_t addr, uint32_t* data )
{
  EE_var_t *pv = &EE_var[CFG_EE_BANK1_SIZE && bank];;

  /* Read element starting from active page */
  return EE_ReadEl( pv, addr, data, pv->current_write_page );
}

/*****************************************************************************/

int EE_Write( int bank, uint16_t addr, uint32_t data )
{
  EE_var_t *pv = &EE_var[CFG_EE_BANK1_SIZE && bank];;
  uint32_t page;

  /* Check if current pool is full */
  if ( pv->nb_written_elements < EE_NB_MAX_ELT * pv->nb_pages )
  {
    /* If not full, write the virtual address and value in the EEPROM */
    return EE_WriteEl( pv, addr, data );
  }

  EE_DBG( EE_2 );

  /* If full, we need to write in other pool and perform pool transfer */
  page = EE_NEXT_POOL( pv );

  /* Check next page state: it must be ERASED */
  if ( EE_GetState( pv, page ) != EE_STATE_ERASED )
  {
    return EE_STATE_ERROR;
  }

  /* Mark the ERASED page at RECEIVE state */
  if ( EE_SetState( pv, page, EE_STATE_RECEIVE ) != EE_OK )
  {
    return EE_WRITE_ERROR;
  }

  EE_DBG( EE_3 );

  /* Reset global variables */
  pv->current_write_page = page;
  pv->nb_written_elements = 0;
  pv->next_write_offset = EE_HEADER_SIZE;

  /* Write the variable passed as parameter in the new active page */
  if ( EE_WriteEl( pv, addr, data ) != EE_OK )
  {
    return EE_WRITE_ERROR;
  }

  EE_DBG( EE_4 );

  /* Set the previous ACTIVE pool to ERASING and copy the latest written
     values to the new pool */
  if ( EE_Transfer( pv, addr, page ) != EE_OK )
  {
    return EE_WRITE_ERROR;
  }

  EE_DBG( EE_5 );

#if CFG_EE_AUTO_CLEAN == 0

  /* A clean is required */
  return EE_CLEAN_NEEDED;

#else /* CFG_EE_AUTO_CLEAN */

  return EE_Clean( bank, 0 );

#endif /* CFG_EE_AUTO_CLEAN */
}

/*****************************************************************************/

int EE_Clean( int bank, int interrupt )
{
  EE_var_t *pv = &EE_var[CFG_EE_BANK1_SIZE && bank];
  uint32_t page;

  /* Get first page of unused pool */
  page = EE_NEXT_POOL( pv );

  /* At least, the first page of the pool should be in ERASING state */
  if ( EE_GetState( pv, page ) != EE_STATE_ERASING )
  {
    return EE_STATE_ERROR;
  }

  EE_DBG( EE_1 );

  /* Erase all the pages of the pool */
  if ( HW_FLASH_Erase( EE_FLASH_PAGE( pv, page ), pv->nb_pages, interrupt )
       != 0 )
  {
    return EE_ERASE_ERROR;
  }

  return EE_OK;
}

/*****************************************************************************/

void EE_Dump( int bank, uint16_t addr, uint32_t* data, uint16_t size )
{
  EE_var_t *pv = &EE_var[CFG_EE_BANK1_SIZE && bank];;
  uint32_t flash_addr, end_flash_addr, word, idx;
  uint64_t el;

  /* Parse all elements from active pool of flash */
  flash_addr = pv->address;
  end_flash_addr = pv->nb_pages * HW_FLASH_PAGE_SIZE;
  if ( pv->current_write_page >= pv->nb_pages )
    flash_addr += end_flash_addr;
  end_flash_addr += flash_addr;

  for ( ; flash_addr < end_flash_addr; flash_addr += HW_FLASH_WIDTH )
  {
    /* Read one element from flash */
    el = *EE_PTR( flash_addr );
    word = (uint32_t)el;

    /* Consider only valid word */
    if ( (word >> 30) == (EE_TAG >> 14) )
    {
      /* Check variable index (addr, idx, size <= 0x4000) */
      idx = ((uint32_t)((word << 2) >> 18)) - addr;
      if ( idx < size )
      {
        /* Write in the data buffer the variable data */
        data[idx] = (uint32_t)(el >> 32);
      }
    }
  }
}

/*****************************************************************************/

static void EE_Reset( EE_var_t* pv, uint32_t address, uint8_t nb_pages )
{
  /* Reset global variables of the bank */
  pv->address = address;
  pv->nb_pages = nb_pages;
  pv->current_write_page = 0;
  pv->nb_written_elements = 0;
  pv->next_write_offset = EE_HEADER_SIZE;
}

/*****************************************************************************/

static int EE_Recovery( EE_var_t* pv )
{
  uint32_t page, first_page, state, prev_state, flash_addr, i;

  /* Search all pages for a reliable RECEIVE page then ACTIVE page */
  for ( state = EE_STATE_RECEIVE; state <= EE_STATE_ACTIVE; state++ )
  {
    for ( page = 0; page < 2UL * pv->nb_pages; page++ )
    {
      if ( state != EE_GetState( pv, page ) )
        continue;

      if ( (page == 0) || (page == pv->nb_pages) )
      {
        /* Check if state is reliable by checking state of next page */
        if ( EE_GetState( pv, page + 1 ) != EE_STATE_ERASED )
          continue;
      }
      else
      {
        prev_state = EE_GetState( pv, page - 1 );

        if ( prev_state != state )
        {
          /* Check if state is reliable by checking state of previous page */
          if ( prev_state != EE_STATE_VALID )
            continue;
        }
        else
        {
          /* If page and previous page are the same, mark previous as VALID */
          if ( EE_SetState( pv, page - 1, EE_STATE_VALID ) != EE_OK )
          {
            return EE_WRITE_ERROR;
          }
        }
      }

      /* Update write page */
      pv->current_write_page = page;

      /* Count elements already in ACTIVE or RECEIVE page */
      flash_addr = EE_FLASH_ADDR( pv, page ) + EE_HEADER_SIZE;
      for ( i = 0; i < EE_NB_MAX_ELT; i++ )
      {
        /* Check if current element is valid */
        if ( *EE_PTR( flash_addr ) == EE_ERASED )
          break;

        /* Update global variables accordingly */
        pv->nb_written_elements++;
        pv->next_write_offset += HW_FLASH_WIDTH;

        /* Next element address */
        flash_addr += HW_FLASH_WIDTH;
      }

      /* Count elements already transferred in previous pool pages */
      while ( !((page == 0) || (page == pv->nb_pages)) )
      {
        /* Update number of elements written in pool */
        pv->nb_written_elements += EE_NB_MAX_ELT;

        page--;
      }

      /* If we have found a RECEIVE page, it means that pool transfer
         has been interrupted by reset */
      if ( state == EE_STATE_RECEIVE )
      {
        /* Resume pool transfer */
        if ( EE_Transfer( pv, EE_TAG, page ) != EE_OK )
        {
          return EE_WRITE_ERROR;
        }
      }

      /* RECEIVE/ACTIVE page found, check if some erasing is needed */

      /* Get first page of unused pool */
      first_page = EE_NEXT_POOL( pv );

      /* Erase all the pages not already erased in the pool */
      for ( page = first_page; page < first_page + pv->nb_pages; page++ )
      {
        if ( EE_GetState( pv, page ) != EE_STATE_ERASED )
        {
          if ( HW_FLASH_Erase( EE_FLASH_PAGE( pv, page ), 1, 0 ) != 0 )
          {
            return EE_ERASE_ERROR;
          }
        }
      }

      return EE_OK;
    }
  }

  /* No true RECEIVE or ACTIVE page has been found */
  return EE_STATE_ERROR;
}

/*****************************************************************************/

static int EE_Transfer( EE_var_t* pv, uint16_t addr, uint32_t page )
{
  uint32_t state, var, data, last_page;

  /* Input "page" is the first page of the new pool;
     We compute "last_page" as the last page of the old pool to be set
     in ERASING state (all pages in old pool are assumed to be either VALID
     or ACTIVE, except in case of recovery, where some pages may be already
     in ERASING state.
     However, in case of recovery, we do not not need to set ERASING,
     as initialization phase erases the unactive pool. */
  last_page =
    (page < pv->nb_pages) ? (2 * pv->nb_pages - 1) : (pv->nb_pages - 1);

  if ( addr != EE_TAG )
  {
    /* Loop on all old pool pages in descending order */
    page = last_page;
    while ( 1 )
    {
      state = EE_GetState( pv, page );

      if ( (state == EE_STATE_ACTIVE) || (state == EE_STATE_VALID) )
      {
        /* Set page state to ERASING */
        if ( EE_SetState( pv, page, EE_STATE_ERASING ) != EE_OK )
        {
          return EE_WRITE_ERROR;
        }
      }

      EE_DBG( EE_6 );

      /* Check if start of pool is reached */
      if ( (page == 0) || (page == pv->nb_pages) )
        break;

      page--;
    }
  }

  /* Now, we can copy variables from one pool to the other */

  for ( var = 0; var < EE_NB_MAX_ELT * pv->nb_pages; var++ )
  {
    /* Check each variable except the one passed as parameter
       (and except the ones already transferred in case of recovery) */
    if  ( (var != addr) &&
          ((addr != EE_TAG) ||
           (EE_ReadEl( pv, var, &data, pv->current_write_page ) != EE_OK)) )
    {
      /* Read the last variable update */
      if ( EE_ReadEl( pv, var, &data, last_page ) == EE_OK )
      {
        EE_DBG( EE_7 );

        /* In case variable corresponding to the virtual address was found,
           copy the variable to the new active page */
        if ( EE_WriteEl( pv, var, data ) != EE_OK )
        {
          return EE_WRITE_ERROR;
        }
      }
    }
  }

  /* Transfer is now done, mark the receive state page as active */
  return EE_SetState( pv, pv->current_write_page, EE_STATE_ACTIVE );
}

/*****************************************************************************/

static int EE_WriteEl( EE_var_t* pv, uint16_t addr, uint32_t data )
{
  uint32_t page, flash_addr;
  uint64_t el;

  /* It is assumed here that the current pool is not full
     and that free pages in this pool are in ERASED state */

  /* Check if active page is full */
  if ( pv->next_write_offset >= HW_FLASH_PAGE_SIZE )
  {
    /* Get current active page */
    page = pv->current_write_page;

    /* Set new page as was previous one (active or receive) */
    if ( EE_SetState( pv, page + 1, EE_GetState( pv, page ) ) != EE_OK )
    {
      return EE_WRITE_ERROR;
    }

    EE_DBG( EE_8 );

    /* Set current page in valid state */
    if ( EE_SetState( pv, page, EE_STATE_VALID ) != EE_OK )
    {
      return EE_WRITE_ERROR;
    }

    /* Update global variables to use next page */
    pv->current_write_page = page + 1;
    pv->next_write_offset = EE_HEADER_SIZE;
  }

  /* Build element to be written in flash */
  if ( addr == EE_TAG )
  {
    el = 0ULL;
  }
  else
  {
    /* Build element from virtual addr and data, plus CRC */
    el = ((((uint64_t)data) << 32) | ((EE_TAG | (addr & 0x3FFFUL)) << 16));
    el |= EE_Crc( el );
  }

  /* Compute write address */
  flash_addr =
    EE_FLASH_ADDR( pv, pv->current_write_page ) + pv->next_write_offset;

  /* Write element in flash */
  if ( HW_FLASH_Write( flash_addr, el ) != 0 )
  {
    return EE_WRITE_ERROR;
  }

  /* Increment global variables relative to write operation done */
  pv->next_write_offset += HW_FLASH_WIDTH;
  pv->nb_written_elements++;

  return EE_OK;
}

/*****************************************************************************/

static int EE_ReadEl( const EE_var_t* pv,
                      uint16_t addr, uint32_t* data, uint32_t page )
{
  uint32_t flash_addr, offset;
  uint64_t el;

  /* Search variable in the pool (in decreasing page order from "page") */
  while ( 1 )
  {
    /* Check each page address starting from end */
    flash_addr = EE_FLASH_ADDR( pv, page );
    for ( offset = HW_FLASH_PAGE_SIZE - HW_FLASH_WIDTH;
          offset >= EE_HEADER_SIZE; offset -= HW_FLASH_WIDTH )
    {
      /* Read one element from flash */
      el = *EE_PTR( flash_addr + offset );

      /* Compare the read address with the input address and check CRC:
         in case of failed CRC, data is corrupted and has to be skipped */
      if ( (el != EE_ERASED) && (el != 0ULL) &&
           (((el & 0x3FFFFFFFUL) >> 16) == addr) &&
           (EE_Crc( el ) == (uint16_t)el) )
      {
        /* Get variable data */
        *data = (uint32_t)(el >> 32);

        /* Variable is found */
        return EE_OK;
      }
    }

    /* Check if start of pool is reached */
    if ( (page == 0) || (page == pv->nb_pages) )
    {
      /* Variable is not found */
      return EE_NOT_FOUND;
    }

    page--;
  }
}

/*****************************************************************************/

static int EE_SetState( const EE_var_t* pv, uint32_t page, uint32_t state )
{
  uint32_t flash_addr;

  flash_addr = EE_FLASH_ADDR( pv, page ) + ((state - 1) * HW_FLASH_WIDTH);

  EE_DBG( EE_0 );

  /* Set new page state inside page header */
  if ( HW_FLASH_Write( flash_addr, EE_PROGRAMMED ) != 0 )
  {
    return EE_WRITE_ERROR;
  }

  return EE_OK;
}

/*****************************************************************************/

static uint32_t EE_GetState( const EE_var_t* pv, uint32_t page )
{
  uint32_t state, flash_addr;

  flash_addr = EE_FLASH_ADDR( pv, page ) + EE_HEADER_SIZE;

  for ( state = EE_STATE_ERASING; state > EE_STATE_ERASED; state-- )
  {
    flash_addr -= HW_FLASH_WIDTH;

    /* If page header word is not ERASED, return word index as page state */
    if ( *EE_PTR( flash_addr ) != EE_ERASED )
      break;
  }

  return state;
}

/*****************************************************************************/

static uint16_t EE_Crc( uint64_t v )
{
  uint32_t x, crc = 0;

#define EE_CRC_STEP( n ) x = ((crc >> 8) ^ (uint8_t)(v >> n)) & 0xFFUL; \
                         x ^= x >> 4; \
                         crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x

  EE_CRC_STEP( 16 );
  EE_CRC_STEP( 24 );
  EE_CRC_STEP( 32 );
  EE_CRC_STEP( 40 );
  EE_CRC_STEP( 48 );
  EE_CRC_STEP( 56 );

  return (uint16_t)crc;
}

/*****************************************************************************/
