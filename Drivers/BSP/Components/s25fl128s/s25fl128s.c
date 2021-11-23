/**
  ******************************************************************************
  * @file    s25fl128s.c
  * @author  MCD Application Team
  * @brief   This file provides the S25FL128S QSPI drivers.
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

/* Includes ------------------------------------------------------------------*/
#include "s25fl128s.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup S25FL128S S25FL128S
  * @{
  */

/** @defgroup S25FL128S_Private_Define S25FL128S Private Define
 * @{
 */
/* To avoid compiling issues for projects using previous version */
#ifndef S25FL128S_DUMMY_CYCLES_READ_DUAL_INOUT
#define S25FL128S_DUMMY_CYCLES_READ_DUAL_INOUT 4U
#endif

#ifndef S25FL128S_DUMMY_CYCLES_READ_QUAD_INOUT
#define S25FL128S_DUMMY_CYCLES_READ_QUAD_INOUT 6U
#endif
/**
 * @}
 */

/** @defgroup S25FL128S_Exported_Functions S25FL128S Exported Functions
  * @{
  */

/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval QSPI memory status
  */
int32_t S25FL128S_GetFlashInfo(S25FL128S_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize = S25FL128S_FLASH_SIZE;
  pInfo->EraseSectorSize = S25FL128S_SECTOR_SIZE;
  pInfo->EraseSectorsNumber = (S25FL128S_FLASH_SIZE/S25FL128S_SECTOR_SIZE);
  pInfo->ProgPageSize = S25FL128S_PAGE_SIZE;
  pInfo->ProgPagesNumber = (S25FL128S_FLASH_SIZE/S25FL128S_PAGE_SIZE);

  return S25FL128S_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_WriteEnable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  UNUSED(Mode); /* The command Write Enable is always 1-0-0 */

  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = S25FL128S_SR1_WREN;
  s_config.Mask            = S25FL128S_SR1_WREN;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
  s_command.Instruction    = S25FL128S_READ_STATUS_REG1_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(Ctx, &s_command, &s_config, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  This function reset the (WEL) Write Enable Latch bit.
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_WriteDisable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef     s_command;

  UNUSED(Mode); /* The command Write Disable is always 1-0-0 */

  /* Initialize the read ID command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_WRITE_DISABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  *         SPI/QPI; 1-1-1/1-1-4
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ 256
  * @retval QSPI memory status
  */
int32_t S25FL128S_PageProgram(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  /* Setup program command */
  switch(Mode)
  {
  case S25FL128S_SPI_1I4O_MODE :              /* 1-1-4 program commands */
    s_command.Instruction     = S25FL128S_QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD;
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case S25FL128S_SPI_MODE :                   /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.Instruction     = S25FL128S_PAGE_PROG_4_BYTE_ADDR_CMD;
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    break;
  }

  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.Address           = WriteAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = Size;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(Ctx, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Polling WIP(Write In Progress) bit become to 0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_AutoPollingMemReady(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  UNUSED(Mode); /* The command Read Status Register-1 is always 1-0-1 */

  /* Configure automatic polling mode to wait for memory ready */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_READ_STATUS_REG1_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match              = 0;
  s_config.Mask               = S25FL128S_SR1_WIP;
  s_config.MatchMode          = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize    = 1;
  s_config.Interval           = 0x10;
  s_config.AutomaticStop      = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling(Ctx, &s_command, &s_config, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_Enter4BytesAddressMode(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;
  uint8_t reg1;

  /* Initialize the read bank register command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_READ_BANK_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, &reg1, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Enable write operations */
  if( S25FL128S_WriteEnable(Ctx,Mode)!=S25FL128S_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Update Bank address register (with 4byte addressing bit) */
  s_command.Instruction = S25FL128S_WRITE_BANK_REG_CMD;
  MODIFY_REG(reg1, S25FL128S_BA_EXTADD, S25FL128S_BA_EXTADD);

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Transmission of the data Status Register 1 */
  if (HAL_QSPI_Transmit(Ctx, &reg1, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Reads an amount of data from the QSPI memory in STR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval QSPI memory status
  */
int32_t S25FL128S_ReadSTR(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  switch(Mode)
  {
  case S25FL128S_SPI_1I2O_MODE :           /* 1-1-2 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;

  case S25FL128S_SPI_2IO_MODE :           /* 1-2-2 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ_DUAL_INOUT;
    break;

  case S25FL128S_SPI_1I4O_MODE :           /* 1-1-4 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;

  case S25FL128S_SPI_4IO_MODE :           /* 1-4-4 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ_QUAD_INOUT;
    break;

  case S25FL128S_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default:
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;
  }

  /* Initialize the read command */
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.Address           = ReadAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.NbData            = Size;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  *         S25FL128S support 4K, 64K size block erase commands.
  *         SPI; 1-0-0/1-1-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  BlockAddress Block address to erase
  * @param  BlockSize 4K or 64K
  * @retval QSPI memory status
  */
int32_t S25FL128S_BlockErase(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint32_t BlockAddress, S25FL128S_Erase_t BlockSize)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The Erase commands are always 1-1-0 or 1-0-0 */

  /* Setup erase command */
  switch(BlockSize)
  {
  case S25FL128S_ERASE_64K :
    s_command.Instruction     = S25FL128S_SECTOR_ERASE_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize     = QSPI_ADDRESS_32_BITS;
    s_command.Address         = BlockAddress;
    break;

  case S25FL128S_ERASE_CHIP :
    s_command.Instruction     = S25FL128S_BULK_ERASE_ALTERNATE_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_NONE;
  break;

  case S25FL128S_ERASE_4K :
  default :
    s_command.Instruction     = S25FL128S_SUBSECTOR_ERASE_4_BYTE_ADDR_CMD_4K;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize     = QSPI_ADDRESS_32_BITS;
    s_command.Address         = BlockAddress;
    break;
  }

  /* Initialize the erase command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if(HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Whole chip erase.
  *         SPI; 1-0-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_ChipErase(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Chip Erase is always 1-0-0 */

  /* Initialize the erase command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.Instruction       = S25FL128S_BULK_ERASE_ALTERNATE_CMD;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Reads an amount of data from the QSPI memory on STR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_EnableMemoryMappedModeSTR(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  switch(Mode)
  {
  case S25FL128S_SPI_1I2O_MODE :           /* 1-1-2 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;

  case S25FL128S_SPI_2IO_MODE :           /* 1-2-2 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ_DUAL_INOUT;
    break;

  case S25FL128S_SPI_1I4O_MODE :           /* 1-1-4 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;

  case S25FL128S_SPI_4IO_MODE :           /* 1-4-4 read commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ_QUAD_INOUT;
    break;

  case S25FL128S_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default:
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = S25FL128S_FAST_READ_4_BYTE_ADDR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    s_command.DummyCycles     = S25FL128S_DUMMY_CYCLES_READ;
    break;
  }

  s_command.DummyCycles        = S25FL128S_DUMMY_CYCLES_READ;
  s_command.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
  s_command.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;
  /* Configure the command for the read instruction */
  s_command.AddressSize        = QSPI_ADDRESS_32_BITS;
  s_command.DdrMode            = QSPI_DDR_MODE_DISABLE;
  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0x00;

  if (HAL_QSPI_MemoryMapped(Ctx, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Read Flash Status register value
  *         SPI; 1-0-1
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  Value to read from status register
  */
int32_t S25FL128S_ReadStatusRegister(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *Value)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Read Status Register-1 is always 1-0-1 */

  /* Initialize the reading of status register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_READ_STATUS_REG1_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, Value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Flash reset enable command
  *         SPI; 1-0-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_ResetEnable(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Software Reset is always 1-0-0 */

  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_SOFTWARE_RESET_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Flash reset memory command
  *         SPI; 1-0-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_ResetMemory(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Software Reset is always 1-0-0 */

  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_SOFTWARE_RESET_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  *         SPI; 1-0-1
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  ID  Flash ID
  * @retval QSPI memory status
  */
int32_t S25FL128S_ReadID(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *ID)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Read Identification is always 1-0-1 */

  /* Initialize the read ID command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_READ_ID_CMD2;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.NbData            = 3;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, ID, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Program/Erases suspend. Interruption Program/Erase operations.
  *         After the device has entered Erase-Suspended mode,
  *         system can read any address except the block/sector being Program/Erased.
  *         SPI; 1-0-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash moder
  * @retval QSPI memory status
  */
int32_t S25FL128S_ProgEraseSuspend(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Program Erase Suspend is always 1-0-0 */

  /* Initialize the S25FL128S_PROG_ERASE_SUSPEND_CMD command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_PROG_ERASE_SUSPEND_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Program/Erases resume.
  *         SPI; 1-0-0
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_ProgEraseResume(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  UNUSED(Mode); /* The command Program Erase Resume is always 1-0-0 */

  /* Initialize the S25FL128S_PROG_ERASE_RESUME_CMD command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = S25FL128S_PROG_ERASE_RESUME_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return S25FL128S_ERROR;
  }

  return S25FL128S_OK;
}

/**
  * @brief  Enter deep sleep
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @retval QSPI memory status
  */
int32_t S25FL128S_EnterDeepPowerDown(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode)
{
  UNUSED(Ctx);
  UNUSED(Mode);

  /* No Deep Power Down command for this memory */
  return S25FL128S_ERROR;
}

/**
  * @brief  Reads an amount of SFDP data from the QSPI memory.
  *         SFDP : Serial Flash Discoverable Parameter
  * @param  Ctx QSPI handle
  * @param  Mode Flash mode
  * @param  pData Pointer to data to be read
  *         ReadAddr Read start address
  *         Size Size of data to read in Byte
  * @retval QSPI memory status
  */
int32_t S25FL128S_ReadSFDP(QSPI_HandleTypeDef *Ctx, S25FL128S_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  UNUSED(Ctx);
  UNUSED(Mode);
  UNUSED(pData);
  UNUSED(ReadAddr);
  UNUSED(Size);

  /* No Serial Flash Discoverable Parameter command for this memory */
  return S25FL128S_ERROR;
 }

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

