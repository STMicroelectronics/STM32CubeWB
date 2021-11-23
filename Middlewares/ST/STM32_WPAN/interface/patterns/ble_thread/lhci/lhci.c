/**
 ******************************************************************************
 * @file    lhci.c
 * @author  MCD Application Team
 * @brief   HCI command for the system channel
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
#include "ble_common.h"

#include "tl.h"
#include "mbox_def.h"
#include "lhci.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
extern uint32_t FW_Version;

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
void LHCI_C1_Write_Register( TL_CmdPacket_t *pcmd )
{
  LHCI_C1_Write_Register_cmd_t * p_param;
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK(); /**< backup PRIMASK bit */

  p_param = (LHCI_C1_Write_Register_cmd_t *) pcmd->cmdserial.cmd.payload;

  switch (p_param->busw)
  {
  case LHCI_8bits:
    __disable_irq();

    *(uint8_t *) (p_param->add) = ((*(uint8_t *) (p_param->add)) & (~(p_param->mask)))
        		      | (p_param->val & p_param->mask);

    __set_PRIMASK(primask_bit); /**< Restore PRIMASK bit*/
    break;

  case LHCI_16bits:
    __disable_irq();

    *(uint16_t *) (p_param->add) = ((*(uint16_t *) (p_param->add)) & (~(p_param->mask)))
        		      | (p_param->val & p_param->mask);

    __set_PRIMASK(primask_bit); /**< Restore PRIMASK bit*/
    break;

  default: /**< case SHCI_32BITS */
    __disable_irq();

    *(uint32_t *) (p_param->add) = ((*(uint32_t *) (p_param->add)) & (~(p_param->mask)))
        		      | (p_param->val & p_param->mask);

    __set_PRIMASK(primask_bit); /**< Restore PRIMASK bit*/
    break;
  }

  ((TL_EvtPacket_t*) pcmd)->evtserial.type = TL_LOCRSP_PKT_TYPE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.evtcode = TL_BLEEVT_CC_OPCODE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.plen = TL_EVT_CS_PAYLOAD_SIZE;
  ;
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->cmdcode = LHCI_OPCODE_C1_WRITE_REG;
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload[0]= 0x00;
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->numcmd = 1;

  return;
}

void LHCI_C1_Read_Register( TL_CmdPacket_t *pcmd )
{
  LHCI_C1_Read_Register_cmd_t * p_param;
  uint32_t rsp_val;
  uint8_t busw;

  p_param = (LHCI_C1_Read_Register_cmd_t *) pcmd->cmdserial.cmd.payload;
  busw = p_param->busw;

  switch (busw)
  {
  case LHCI_8bits:
    rsp_val = *(uint8_t *) (p_param->add);
    break;

  case LHCI_16bits:
    rsp_val = *(uint16_t *) (p_param->add);
    break;

  default: /**< case SHCI_32BITS */
    rsp_val = *(uint32_t *) (p_param->add);
    break;
  }

  ((TL_EvtPacket_t*) pcmd)->evtserial.type = TL_LOCRSP_PKT_TYPE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.evtcode = TL_BLEEVT_CC_OPCODE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.plen = TL_EVT_HDR_SIZE + sizeof(LHCI_C1_Read_Register_ccrp_t);
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->cmdcode = LHCI_OPCODE_C1_READ_REG;
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->numcmd = 1;
  ((LHCI_C1_Read_Register_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->status =
      0x00;

  memcpy(
      (void*)&(((LHCI_C1_Read_Register_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->val),
      &rsp_val,
      4);

  return;
}

void LHCI_C1_Read_Device_Information( TL_CmdPacket_t *pcmd )
{
  uint32_t ipccdba;
  MB_RefTable_t * p_ref_table;

  ipccdba = READ_BIT(FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA);
  p_ref_table = (MB_RefTable_t*)((ipccdba<<2) + SRAM2A_BASE);

  ((TL_EvtPacket_t*) pcmd)->evtserial.type = TL_LOCRSP_PKT_TYPE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.evtcode = TL_BLEEVT_CC_OPCODE;
  ((TL_EvtPacket_t*) pcmd)->evtserial.evt.plen = TL_EVT_HDR_SIZE + sizeof(LHCI_C1_Device_Information_ccrp_t);
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->cmdcode = LHCI_OPCODE_C1_DEVICE_INF;
  ((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->numcmd = 1;


  /**
   * status
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->status =
      0x00;

  /**
   * revision id
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->rev_id =
      (uint16_t)LL_DBGMCU_GetRevisionID();

  /**
   * device code id
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->dev_code_id =
      (uint16_t)LL_DBGMCU_GetDeviceID();

  /**
   * package type
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->package_type =
      (uint8_t)LL_GetPackageType();

  /**
   * device type id
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->device_type_id =
      (uint8_t)LL_FLASH_GetDeviceID();

  /**
   * st company id
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->st_company_id =
      LL_FLASH_GetSTCompanyID();

  /**
   * UID64
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->uid64 =
      LL_FLASH_GetUDN();

  /**
   * UID96_0
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->uid96_0 =
      LL_GetUID_Word0();

  /**
   * UID96_1
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->uid96_1 =
      LL_GetUID_Word1();

  /**
   * UID96_2
   */
  ((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->uid96_2 =
      LL_GetUID_Word2();

  /**
   * SafeBootInf
   */
  memcpy(
      &(((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->SafeBootInf),
      &(p_ref_table->p_device_info_table->SafeBootInfoTable),
      sizeof(MB_SafeBootInfoTable_t));

  /**
   * FusInf
   */
  memcpy(
      &(((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->FusInf),
      &(p_ref_table->p_device_info_table->FusInfoTable),
      sizeof(MB_FusInfoTable_t));

  /**
   * WirelessFwInf
   */
  memcpy(
      &(((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->WirelessFwInf),
      &(p_ref_table->p_device_info_table->WirelessFwInfoTable),
      sizeof(MB_WirelessFwInfoTable_t));

  /**
   * AppFwInf
   */
  (((LHCI_C1_Device_Information_ccrp_t *) (((TL_CcEvt_t *) (((TL_EvtPacket_t*) pcmd)->evtserial.evt.payload))->payload))->AppFwInf) =
      FW_Version;

  return;
}

