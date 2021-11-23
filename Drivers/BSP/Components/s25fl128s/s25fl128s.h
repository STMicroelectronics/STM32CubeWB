/**
  ******************************************************************************
  * @file    s25fl128s.h
  * @author  MCD Application Team
  * @brief   This file contains all the description of the S25FL128S QSPI memory.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef S25FL128S_H
#define S25FL128S_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "s25fl128s_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup S25FL128S
  * @brief     This file provides a set of definitions for the Spansion
  *            S25FL128S memory (configuration, commands, registers).
  * @{
  */

/** @defgroup S25FL128S_Exported_Constants
  * @{
  */

/* S25FL128SAGMFI01 Spansion Memory */
/**
  * @brief  S25FL128S Configuration
  */

#define S25FL128S_BLOCK_64K                   (uint32_t) (64 * 1024)        /* 256 blocks of 64KBytes  */
#define S25FL128S_SECTOR_4K                   (uint32_t) (4  * 1024)        /* 4096 sectors of 4KBytes */

#define S25FL128S_FLASH_SIZE                            0x1000000 /* 128 MBits => 16MBytes */
#define S25FL128S_SECTOR_SIZE                           0x1000    /* 4096 sectors of 4kBytes */
#define S25FL128S_PAGE_SIZE                             0x100     /* 65536 pages of 256 bytes */

#define S25FL128S_BULK_ERASE_MAX_TIME                   330000
#define S25FL128S_SECTOR_ERASE_MAX_TIME                 130

/**
  * @brief  S25FL128S Error codes
  */
#define S25FL128S_OK                (0)
#define S25FL128S_ERROR             (-1)

/**
  * @brief  S25FL128S Commands
  */
/* Reset Operations */
#define S25FL128S_SOFTWARE_RESET_CMD                    0xF0
#define S25FL128S_MODE_BIT_RESET_CMD                    0xFF

/* Identification Operations */
#define S25FL128S_READ_ID_CMD                           0x90
#define S25FL128S_READ_ID_CMD2                          0x9F
#define S25FL128S_READ_ELECTRONIC_SIGNATURE             0xAB
#define S25FL128S_READ_SERIAL_FLASH_DISCO_PARAM_CMD     0x5A

/* Register Operations */
#define S25FL128S_READ_STATUS_REG1_CMD                  0x05
#define S25FL128S_READ_STATUS_REG2_CMD                  0x07
#define S25FL128S_READ_CONFIGURATION_REG1_CMD           0x35
#define S25FL128S_WRITE_STATUS_CMD_REG_CMD              0x01
#define S25FL128S_WRITE_DISABLE_CMD                     0x04
#define S25FL128S_WRITE_ENABLE_CMD                      0x06
#define S25FL128S_CLEAR_STATUS_REG1_CMD                 0x30
#define S25FL128S_READ_AUTOBOOT_REG_CMD                 0x14
#define S25FL128S_WRITE_AUTOBOOT_REG_CMD                0x15
#define S25FL128S_READ_BANK_REG_CMD                     0x16
#define S25FL128S_WRITE_BANK_REG_CMD                    0x17
#define S25FL128S_ACCESS_BANK_REG_CMD                   0xB9
#define S25FL128S_READ_DATA_LEARNING_PATTERN_CMD        0x41
#define S25FL128S_PGM_NV_DATA_LEARNING_REG_CMD          0x43
#define S25FL128S_WRITE_VOL_DATA_LEARNING_REG_CMD       0x4A

/* Read Operations */
#define S25FL128S_READ_CMD                              0x03
#define S25FL128S_READ_4_BYTE_ADDR_CMD                  0x13
#define S25FL128S_FAST_READ_CMD                         0x0B
#define S25FL128S_FAST_READ_4_BYTE_ADDR_CMD             0x0C
#define S25FL128S_FAST_READ_DDR_CMD                     0x0D
#define S25FL128S_FAST_READ__DDR_4_BYTE_ADDR_CMD        0x0E
#define S25FL128S_DUAL_OUT_FAST_READ_CMD                0x3B
#define S25FL128S_DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD    0x3C
#define S25FL128S_QUAD_OUT_FAST_READ_CMD                0x6B
#define S25FL128S_QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD    0x6C
#define S25FL128S_DUAL_INOUT_FAST_READ_CMD              0xBB
#define S25FL128S_DUAL_INOUT_FAST_READ_DTR_CMD          0xBD
#define S25FL128S_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD  0xBC
#define S25FL128S_DDR_DUAL_INOUT_READ_4_BYTE_ADDR_CMD   0xBE
#define S25FL128S_QUAD_INOUT_FAST_READ_CMD              0xEB
#define S25FL128S_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD  0xEC
#define S25FL128S_QUAD_INOUT_FAST_READ_DDR_CMD          0xED
#define S25FL128S_QUAD_INOUT_READ_DDR_4_BYTE_ADDR_CMD   0xEE

/* Program Operations */
#define S25FL128S_PAGE_PROG_CMD                         0x02
#define S25FL128S_PAGE_PROG_4_BYTE_ADDR_CMD             0x12
#define S25FL128S_QUAD_IN_FAST_PROG_CMD                 0x32
#define S25FL128S_QUAD_IN_FAST_PROG_ALTERNATE_CMD       0x38
#define S25FL128S_QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD     0x34
#define S25FL128S_PROGRAM_SUSPEND_CMD                   0x85
#define S25FL128S_PROGRAM_RESUME_CMD                    0x8A

/* Erase Operations */
#define S25FL128S_SUBSECTOR_ERASE_CMD_4K                0x20
#define S25FL128S_SUBSECTOR_ERASE_4_BYTE_ADDR_CMD_4K    0x21
#define S25FL128S_SECTOR_ERASE_CMD                      0xD8
#define S25FL128S_SECTOR_ERASE_4_BYTE_ADDR_CMD          0xDC
#define S25FL128S_BULK_ERASE_CMD                        0x60
#define S25FL128S_BULK_ERASE_ALTERNATE_CMD              0xC7
#define S25FL128S_PROG_ERASE_SUSPEND_CMD                0x75
#define S25FL128S_PROG_ERASE_RESUME_CMD                 0x7A

/* One-Time Programmable Operations */
#define S25FL128S_PROG_OTP_ARRAY_CMD                    0x42
#define S25FL128S_READ_OTP_ARRAY_CMD                    0x4B

/* Deep Power Operations */
#define S25FL128S_ENTER_DEEP_POWER_DOWN                 0xB9

/* Advanced Sector Protection Operations */
#define S25FL128S_READ_DYB_CMD                          0xE0
#define S25FL128S_WRITE_DYB_CMD                         0xE1
#define S25FL128S_READ_PPB_CMD                          0xE2
#define S25FL128S_PROGRAM_PPB_CMD                       0xE3
#define S25FL128S_ERASE_PPB_CMD                         0xE4
#define S25FL128S_READ_ASP_CMD                          0x2B
#define S25FL128S_PROGRAM_ASP_CMD                       0x2F
#define S25FL128S_READ_PPB_LOCKBIT_CMD                  0xA7
#define S25FL128S_WRITE_PPB_LOCKBIT_CMD                 0xA6
#define S25FL128S_READ_PASSWORD_CMD                     0xE7
#define S25FL128S_PROGRAM_PASSWORD_CMD                  0xE8
#define S25FL128S_UNLOCK_PASSWORD_CMD                   0xE9

/**
  * @brief  S25FL128S Registers
  */
/* Status Register-1 */
#define S25FL128S_SR1_WIP                               ((uint8_t)0x01)      /*!< Write in progress, device busy */
#define S25FL128S_SR1_WREN                              ((uint8_t)0x02)      /*!< Write Registers, program or commands are accepted */
#define S25FL128S_SR1_BP0                               ((uint8_t)0x04)      /*!< Sector0 protected from Program or Erase */
#define S25FL128S_SR1_BP1                               ((uint8_t)0x08)      /*!< Sector1 protected from Program or Erase */
#define S25FL128S_SR1_BP2                               ((uint8_t)0x10)      /*!< Sector2 protected from Program or Erase */
#define S25FL128S_SR1_ERERR                             ((uint8_t)0x20)      /*!< Erase error */
#define S25FL128S_SR1_PGERR                             ((uint8_t)0x40)      /*!< Program error */
#define S25FL128S_SR1_SRWD                              ((uint8_t)0x80)      /*!< Status Register Write Disable */

/* Status Register-2 */
#define S25FL128S_SR2_PS                                ((uint8_t)0x01)      /*!< Program in Suspend mode */
#define S25FL128S_SR2_ES                                ((uint8_t)0x02)      /*!< Erase Suspend Mode */

/* Configuration Register CR1 */
#define S25FL128S_CR1_FREEZE                            ((uint8_t)0x01)      /*!< Block protection and OTP locked */
#define S25FL128S_CR1_QUAD                              ((uint8_t)0x02)      /*!< Quad mode enable */
#define S25FL128S_CR1_BPNV                              ((uint8_t)0x08)      /*!< BP2-0 bits of Status Reg are volatile */
#define S25FL128S_CR1_TBPROT                            ((uint8_t)0x20)      /*!< BPstarts at bottom */
#define S25FL128S_CR1_LC_MASK                           ((uint8_t)0xC0)      /*!< Latency Code mask */
#define S25FL128S_CR1_LC0                               ((uint8_t)0x00)      /*!< Latency Code = 0 */
#define S25FL128S_CR1_LC1                               ((uint8_t)0x40)      /*!< Latency Code = 1 */
#define S25FL128S_CR1_LC2                               ((uint8_t)0x80)      /*!< Latency Code = 2 */
#define S25FL128S_CR1_LC3                               ((uint8_t)0xC0)      /*!< Latency Code = 3 */

/* AutoBoot Register */
#define S25FL128S_AB_EN                                 ((uint32_t)0x00000001) /*!< AutoBoot Enabled     */
#define S25FL128S_AB_SD_MASK                            ((uint32_t)0x000001FE) /*!< AutoBoot Start Delay mask */
#define S25FL128S_AB_SA_MASK                            ((uint32_t)0xFFFFFE00) /*!< AutoBoot Start Address mask */

/* Bank Address Register */
#define S25FL128S_BA_BA24                               ((uint8_t)0x01)      /*!< A24 for 512 Mb device */
#define S25FL128S_BA_BA25                               ((uint8_t)0x02)      /*!< A25 for 512 Mb device */
#define S25FL128S_BA_EXTADD                             ((uint8_t)0x80)      /*!< 4 bytes addressing required from command */

/* ASP Register */
#define S25FL128S_ASP_PSTMLB                            ((uint16_t)0x0002)   /*!< Persistent protection mode not permanently enabled */
#define S25FL128S_ASP_PWSMLB                            ((uint16_t)0x0003)   /*!< Password protection mode not permanently enabled */

/* PPB Lock Register */
#define S25FL128S_PPBLOCK                               ((uint8_t)0x01)      /*!< PPB array may be programmed or erased */

/**
  * @}
  */

/** @defgroup S25FL128S_Exported_Types S25FL128S Exported Types
  * @{
  */

typedef struct {
  uint32_t FlashSize;                     /*!< Size of the flash                         */
  uint32_t EraseSectorSize;               /*!< Size of sectors for the erase operation   */
  uint32_t EraseSectorsNumber;            /*!< Number of sectors for the erase operation */
  uint32_t ProgPageSize;                  /*!< Size of pages for the program operation   */
  uint32_t ProgPagesNumber;               /*!< Number of pages for the program operation */
} S25FL128S_Info_t;

typedef enum
{
  S25FL128S_SPI_MODE = 0,                 /*!< 1-1-1 commands, Power on H/W default setting */
  S25FL128S_SPI_1I2O_MODE,                /*!< 1-1-2 read commands                          */
  S25FL128S_SPI_2IO_MODE,                 /*!< 1-2-2 read commands                          */
  S25FL128S_SPI_1I4O_MODE,                /*!< 1-1-4 read commands                          */
  S25FL128S_SPI_4IO_MODE,                 /*!< 1-4-4 read commands                          */
  S25FL128S_DPI_MODE,                     /*!< 2-2-2 commands                               */
  S25FL128S_QPI_MODE                      /*!< 4-4-4 commands                               */
} S25FL128S_Interface_t;

typedef enum
{
  S25FL128S_STR_TRANSFER = 0,             /*!< Single Transfer Rate */
} S25FL128S_Transfer_t;

typedef enum
{
  S25FL128S_DUALFLASH_DISABLE = 0,        /*!< Single flash mode    */
} S25FL128S_DualFlash_t;

typedef enum
{
  S25FL128S_ERASE_4K = 0,                 /*!< 4K size Sector erase */
  S25FL128S_ERASE_64K,                    /*!< 64K size Block erase */
  S25FL128S_ERASE_CHIP                    /*!< Whole chip erase     */
} S25FL128S_Erase_t;

/**
  * @}
  */

/** @defgroup S25FL128S_Exported_Functions
  * @{
  */

int32_t S25FL128S_GetFlashInfo(S25FL128S_Info_t *pInfo);
int32_t S25FL128S_AutoPollingMemReady(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_Enter4BytesAddressMode(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
/* Register/Setting Commands *************************************************/
int32_t S25FL128S_WriteEnable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_BlockErase(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint32_t BlockAddress, S25FL128S_Erase_t BlockSize);
int32_t S25FL128S_ChipErase(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_PageProgram(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t S25FL128S_ReadSTR(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t S25FL128S_ReadStatusRegister(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *Value);
int32_t S25FL128S_EnableMemoryMappedModeSTR(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_WriteDisable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_ReadID(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *ID);
int32_t S25FL128S_ResetMemory(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_ResetEnable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_EnterDeepPowerDown(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_ProgEraseResume(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_ProgEraseSuspend(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode);
int32_t S25FL128S_ReadSFDP(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* S25FL128S_H */
