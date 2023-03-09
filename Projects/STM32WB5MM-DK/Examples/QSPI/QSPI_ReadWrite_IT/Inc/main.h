/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    QSPI/QSPI_ReadWrite_IT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32wb5mm_dk.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
/* S25FL128S Spansion memory */

/* Size of the flash */
#define QSPI_FLASH_SIZE                          24
#define QSPI_PAGE_SIZE                           256

/* Read Device Identification */
#define READ_EMS_CMD                             0x90
#define READ_ID_CMD                              0x9F
#define READ_ES_CMD                              0xAB

/* Register Access */
#define READ_STATUS_REG_CMD                      0x05
#define READ_STATUS_REG_2_CMD                    0x07
#define READ_CFG_REG_CMD                         0x35
#define WRITE_REG_CMD                            0x01
#define CLEAR_STATUS_REG_CMD                     0x30

#define WRITE_DISABLE_CMD                        0x04
#define WRITE_ENABLE_CMD                         0x06

#define READ_AUTOBOOT_REG_CMD                    0x14
#define WRITE_AUTOBOOT_REG_CMD                   0x15

#define READ_BANK_REG_CMD                        0x16
#define WRITE_BANK_REG_CMD                       0x17
#define ACCESS_BANK_REG_CMD                      0xB9

#define READ_DATA_LEARNING_REG                   0x41
#define WRITE_NONVOL_DATA_LEARNING_REG           0x43
#define WRITE_VOL_DATA_LEARNING_REG              0x4A

/* Read Flash Array */
#define READ_CMD                                 0x03
#define READ_4_BYTE_ADDR_CMD                     0x13

#define FAST_READ_CMD                            0x0B
#define FAST_READ_4_BYTE_ADDR_CMD                0x0C
#define FAST_READ_DDR_CMD                        0x0D
#define FAST_READ_DDR_4_BYTE_ADDR_CMD            0x0E

#define DUAL_OUT_FAST_READ_CMD                   0x3B
#define DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD       0x3C

#define QUAD_OUT_FAST_READ_CMD                   0x6B
#define QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD       0x6C

#define DUAL_INOUT_FAST_READ_CMD                 0xBB
#define DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD     0xBC
#define DUAL_INOUT_FAST_READ_DDR_CMD             0xBD
#define DUAL_INOUT_FAST_READ_DDR_4_BYTE_ADDR_CMD 0xBE

#define QUAD_INOUT_FAST_READ_CMD                 0xEB
#define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD     0xEC
#define QUAD_INOUT_FAST_READ_DDR_CMD             0xED
#define QUAD_INOUT_FAST_READ_DDR_4_BYTE_ADDR_CMD 0xEE

/* Program Flash Array */
#define PAGE_PROG_CMD                            0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD                0x12

#define QUAD_IN_FAST_PROG_CMD                    0x32
#define QUAD_IN_FAST_PROG_2_CMD                  0x38
#define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD        0x34

#define PROG_SUS_CMD                             0x85
#define PROG_RES_CMD                             0x8A

/* Erase Flash Array */
#define PARAM_ERASE_CMD                          0x20
#define PARAM_ERASE_4_BYTE_ADDR_CMD              0x21

#define BULK_ERASE_CMD                           0x60
#define BILK_ERASE_2_CMD                         0xC7

#define SECTOR_ERASE_CMD                         0xD8
#define SECTOR_ERASE_4_BYTE_ADDR_CMD             0xDC

#define ERASE_SUS_CMD                            0x75
#define ERASE_RES_CMD                            0x7A

/* One Time Program Array */
#define OTP_PROG_CMD                             0x42
#define OTP_READ_CMD                             0x4B

/* Advanced Sector Protection */
#define DYB_READ_CMD                             0xE0
#define DYB_WRITE_CMD                            0xE1

#define PPB_READ_CMD                             0xE2
#define PPB_PROG_CMD                             0xE3
#define PPB_ERASE_CMD                            0xE4

#define ASP_READ_CMD                             0x2B
#define ASP_PROG_CMD                             0x2F

#define PPB_LOCK_BIT_READ_CMD                    0xA7
#define PPB_LOCK_BIT_WRITE_CMD                   0xA6

#define PASSWORD_READ_CMD                        0xE7
#define PASSWORD_PROG_CMD                        0xE8
#define PASSWORD_UNLOCK_CMD                      0xE9

/* Reset */
#define SW_RESET_CMD                             0xF0
#define MODE_BIT_RESET_CMD                       0xFF

#define RESET_MEMORY_CMD                         SW_RESET_CMD

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ                  8
#define DUMMY_CLOCK_CYCLES_READ_INOUT_DUAL       0
#define DUMMY_CLOCK_CYCLES_READ_INOUT_QUAD       4

#define DUMMY_CLOCK_CYCLES_READ_DDR              2
#define DUMMY_CLOCK_CYCLES_READ_DUAL_DDR         4
#define DUMMY_CLOCK_CYCLES_READ_QUAD_DDR         6

/* Values for the memory ready register */
#define MEMORY_READY_MATCH_VALUE                 0x00
#define MEMORY_READY_MASK_VALUE                  0x01
#define MEMORY_READY_INSTRUCTION                 READ_STATUS_REG_CMD

#define MEMORY_READY_ALTERNATE_MATCH_VALUE       MEMORY_READY_MATCH_VALUE
#define MEMORY_READY_ALTERNATE_MASK_VALUE        MEMORY_READY_MASK_VALUE
#define MEMORY_READY_ALTERNATE_INSTRUCTION       MEMORY_READY_INSTRUCTION

#define MEMORY_READY_DUAL_FLASH_MATCH_VALUE      0x0000
#define MEMORY_READY_DUAL_FLASH_MASK_VALUE       0x0101
#define MEMORY_READY_DUAL_FLASH_INSTRUCTION      READ_STATUS_REG_CMD

#define QSPI_ALTERNATE_BYTE_PATTERN              0xB0 /* Should be different to 0xAx and not complementary between MSB and LSB part */

/* End address of the QSPI memory */
#define QSPI_END_ADDR              (1 << QSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
