/**
 ******************************************************************************
 * @file    lhci.h
 * @author  MCD Application Team
 * @brief   HCI command for the system channel
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
#ifndef __LHCI_H
#define __LHCI_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  typedef enum
  {
    LHCI_8bits =  1,
    LHCI_16bits = 2,
    LHCI_32bits = 4,
  } LHCI_Busw_t;

#define LHCI_OGF                        ( 0x3F )
#define LHCI_OCF_BASE                   ( 0x160 )

#define LHCI_OCF_C1_WRITE_REG           ( LHCI_OCF_BASE + 0 )
#define LHCI_OPCODE_C1_WRITE_REG        (( LHCI_OGF << 10) + LHCI_OCF_C1_WRITE_REG)
  typedef PACKED_STRUCT
  {
    LHCI_Busw_t     busw;
    uint32_t        mask;
    uint32_t        add;
    uint32_t        val;
  } LHCI_C1_Write_Register_cmd_t;

#define LHCI_OCF_C1_READ_REG            ( LHCI_OCF_BASE + 1 )
#define LHCI_OPCODE_C1_READ_REG         (( LHCI_OGF << 10) + LHCI_OCF_C1_READ_REG)
  typedef PACKED_STRUCT
  {
    LHCI_Busw_t     busw;
    uint32_t        add;
  } LHCI_C1_Read_Register_cmd_t;

  typedef PACKED_STRUCT
  {
    uint8_t         status;
    uint32_t        val;
  } LHCI_C1_Read_Register_ccrp_t;

#define LHCI_OCF_C1_DEVICE_INF          ( LHCI_OCF_BASE + 2 )
#define LHCI_OPCODE_C1_DEVICE_INF       (( LHCI_OGF << 10) + LHCI_OCF_C1_DEVICE_INF)
  typedef PACKED_STRUCT
  {
    uint8_t                   	status;
    uint16_t					rev_id;			/* from DBGMCU_ICODE */
    uint16_t					dev_code_id;	/* from DBGMCU_ICODE */
    uint8_t                   	package_type;	/* from package data register */
    uint8_t                   	device_type_id; /* from FLASH UID64 */
    uint32_t                   	st_company_id;  /* from FLASH UID64 */
    uint32_t                   	uid64;  		/* from FLASH UID64 */
    uint32_t                   	uid96_0;      	/* from Unique device ID register */
    uint32_t                   	uid96_1;      	/* from Unique device ID register */
    uint32_t                   	uid96_2;      	/* from Unique device ID register */
    MB_SafeBootInfoTable_t    	SafeBootInf;
    MB_FusInfoTable_t         	FusInf;
    MB_WirelessFwInfoTable_t  	WirelessFwInf;
    uint32_t                  	AppFwInf;
  } LHCI_C1_Device_Information_ccrp_t;

  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /**
   * @brief Write a register in the device
   *
   * @param  None
   * @retval None
   */
  void LHCI_C1_Write_Register( TL_CmdPacket_t *pcmd );

  /**
   * @brief Read a register in the device
   *
   * @param  None
   * @retval None
   */
  void LHCI_C1_Read_Register( TL_CmdPacket_t *pcmd );

  /**
   * @brief Read the Device Information
   *
   * @param  None
   * @retval None
   */
  void LHCI_C1_Read_Device_Information( TL_CmdPacket_t *pcmd  );

  #ifdef __cplusplus
}
#endif

#endif /*__LHCI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
