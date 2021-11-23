/*****************************************************************************
 * @file    ee.h
 * @author  MCD Application Team
 * @brief   This file contains the interface of the EEPROM emulator
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

#ifndef EE_H__
#define EE_H__


/*
 * Description
 * -----------
 * the EE module implements an EEPROM emulator in one C file ("ee.c").
 * Its interface is defined below in this file ("ee.h").
 * Up to two independent banks can be configured.
 * Data granularity for store and read is one 32-bit word.
 * 
 * Configuration and dependencies
 * ------------------------------
 * the EE module includes a generic configuration header file "ee_cfg.h":
 * this file has to be defined by the user and must provide the following:
 * 
 * - Hardware flash driver definitions:
 *
 *      * HW_FLASH_PAGE_SIZE (in bytes)
 *
 *      * HW_FLASH_WIDTH (in bytes)
 *
 *      * int HW_FLASH_Write( uint32_t address,
 *                            uint64_t data );
 *
 *      * int HW_FLASH_Erase( uint32_t page,
 *                            uint16_t n,
 *                            int interrupt );
 *
 * - Optional configuration definitions:
 *
 *     * CFG_EE_BANK0_SIZE
 *       Size of the first bank in bytes (must be greater than 0).
 *       It must be a multiple of twice the page size.
 *       If not defined, it is set to twice the page size.
 *
 *     * CFG_EE_BANK0_MAX_NB
 *       Maximum number of (32-bit) data that can be stored in the first bank.
 *       This definition is only used during preprocessing for check.
 *
 *     * CFG_EE_BANK1_SIZE
 *       Size of the second bank in bytes (can be 0 if the bank is not used).
 *       It must be a multiple of twice the page size.
 *       If not defined, it is set to 0.
 *    
 *     * CFG_EE_BANK1_MAX_NB
 *       Maximum number of (32-bit) data that can be stored in the second bank.
 *       This definition is only used during preprocessing for check.
 *
 *     * CFG_EE_AUTO_CLEAN
 *       When set to 1, this setting forces EE_Clean to be called at end of
 *       EE_Write when needed.
 *
 *
 * Notes
 * -----
 * - a corrupted word in FLASH detected by the user software shall be set to 0.
 *   The EEPROM emulation software will then handle it properly.
 */


#include <stdint.h>



/* Definition of the functions return value */
enum
{
  EE_OK = 0,
  EE_NOT_FOUND,     /* read data is not found in flash */
  EE_CLEAN_NEEDED,  /* data is written but a "clean" is needed */
  EE_ERASE_ERROR,   /* an error occurs during flash erase */
  EE_WRITE_ERROR,   /* an error occurs during flash write */
  EE_STATE_ERROR    /* state of flash is incoherent (needs clean or format) */
};


/*
 * EE_Init
 *
 * Initialization of EEPROM emulation module. It must be called once at reset.
 *
 * format: 0 -> recover EE state from flash and restore the pages
 *              to a known good state in case of power loss.
 *         1 -> erase all the pages: starts from scratch.
 *              This format mode can be activated the very first time EEPROM
 *              emulation is used, to prepare flash pages for EEPROM emulation
 *              with empty EEPROM
 *
 * base_address: absolute start address of flash area used for EEPROM
 *               emulation. It must be a multiple of flash page size.
 *
 * return: EE_OK in case of success
 *         EE..._ERROR in case of error
 */

extern int EE_Init( int format,
                    uint32_t base_address );

/*
 * EE_Read
 *
 * Returns the last stored variable data, if found, which corresponds to
 * the passed virtual address
 *
 * bank:   index of the bank (0 or 1)
 *
 * addr:   variable virtual address
 *
 * data:   pointer to a 32-bit word (allocated by the caller) containing the
 *         variable value in case of success.
 *
 * return: EE_OK in case of success
 *         EE_NOT_FOUND in case this virtual address has never been written to
 *         EE..._ERROR in case of error
 */

extern int EE_Read( int bank, uint16_t addr, uint32_t* data );

/*
 * EE_Write
 *
 * Writes/updates variable data in EEPROM emulator.
 * Triggers internal pages transfer if flash pool is full.
 *
 * bank:   index of the bank (0 or 1)
 *
 * addr:   variable virtual address
 *
 * data:   32-bit data word to be written
 *
 * return: EE_OK in case of success
 *         EE_CLEAN_NEEDED if success but user must trigger flash cleanup
 *                         by calling EE_Clean()
 *         EE..._ERROR in case of error
 */

extern int EE_Write( int bank, uint16_t addr, uint32_t data );

/*
 * EE_Clean
 *
 * Erase obsolete pool of pages in polling or interrupt mode.
 * This function should be called when EE_Write() has returned EE_CLEAN_NEEDED
 * (and only in that case)
 * If interrupt mode is used, the user must declare a function with the
 * following prototype (see hw.h):
 *  void HWCB_FLASH_EndOfCleanup( void );
 * this function is called under FLASH IRQ handler at end of cleanup.
 *
 * bank:   index of the bank (0 or 1)
 *
 * interrupt: 0 -> polling mode
 *            1 -> interrupt mode
 *
 * return: EE_OK in case of success
 *         EE..._ERROR in case of error
 */

extern int EE_Clean( int bank, int interrupt );

/*
 * EE_Dump
 *
 * Dumps part of the EEPROM emulation content.
 * For the purpose of dumping the full content of the EEPROM, this function is
 * faster than calling several times the EE_Read() function.
 * Note 1: for speed purpose, CRC is not checked. If you need CRC to be
 *         checked, you must use EE_Read() instead.
 * Note 2: Some part of the output buffer can be not written if the
 *         corresponding variables have not been stored in the EEPROM.
 *
 * bank:   index of the bank (0 or 1)
 *
 * addr:   virtual address of the first variable to dump
 *
 * data:   pointer to a 32-bit buffer (allocated by the caller) where the
 *         variables from the EEPROM emulation are written.
 *         Variable from virtual address addr is stored in data[0],
 *         variable from virtual address (addr+1) is stored in data[1],
 *         ...
 *         variable from virtual address (addr+size-1) is stored in
 *         data[size-1].
 *
 * size:   number of consecutive variables to dump
 *
 * return: none
 */

extern void EE_Dump( int bank, uint16_t addr, uint32_t* data, uint16_t size );


#endif /* EE_H__ */
