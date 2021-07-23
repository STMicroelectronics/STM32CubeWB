/******************************************************************************
 * @file    ble_gap_aci.c
 * @author  MCD
 * @brief   STM32WB BLE API (gap_aci)
 *          Auto-generated file: do not edit!
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

#include "ble_gap_aci.h"

tBleStatus aci_gap_set_non_discoverable( void )
{
  struct hci_request rq;
  tBleStatus status = 0;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x081;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_limited_discoverable( uint8_t Advertising_Type,
                                             uint16_t Advertising_Interval_Min,
                                             uint16_t Advertising_Interval_Max,
                                             uint8_t Own_Address_Type,
                                             uint8_t Advertising_Filter_Policy,
                                             uint8_t Local_Name_Length,
                                             const uint8_t* Local_Name,
                                             uint8_t Service_Uuid_length,
                                             const uint8_t* Service_Uuid_List,
                                             uint16_t Slave_Conn_Interval_Min,
                                             uint16_t Slave_Conn_Interval_Max )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_limited_discoverable_cp0 *cp0 = (aci_gap_set_limited_discoverable_cp0*)(cmd_buffer);
  aci_gap_set_limited_discoverable_cp1 *cp1 = (aci_gap_set_limited_discoverable_cp1*)(cmd_buffer + 1 + 2 + 2 + 1 + 1 + 1 + Local_Name_Length * (sizeof(uint8_t)));
  aci_gap_set_limited_discoverable_cp2 *cp2 = (aci_gap_set_limited_discoverable_cp2*)(cmd_buffer + 1 + 2 + 2 + 1 + 1 + 1 + Local_Name_Length * (sizeof(uint8_t)) + 1 + Service_Uuid_length * (sizeof(uint8_t)));
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Advertising_Type = Advertising_Type;
  index_input += 1;
  cp0->Advertising_Interval_Min = Advertising_Interval_Min;
  index_input += 2;
  cp0->Advertising_Interval_Max = Advertising_Interval_Max;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Advertising_Filter_Policy = Advertising_Filter_Policy;
  index_input += 1;
  cp0->Local_Name_Length = Local_Name_Length;
  index_input += 1;
  /* var_len_data input */
  {
    Osal_MemCpy( (void*)&cp0->Local_Name, (const void*)Local_Name, Local_Name_Length );
    index_input += Local_Name_Length;
    {
      cp1->Service_Uuid_length = Service_Uuid_length;
    }
    index_input += 1;
    Osal_MemCpy( (void*)&cp1->Service_Uuid_List, (const void*)Service_Uuid_List, Service_Uuid_length );
    index_input += Service_Uuid_length;
    {
      cp2->Slave_Conn_Interval_Min = Slave_Conn_Interval_Min;
    }
    index_input += 2;
    {
      cp2->Slave_Conn_Interval_Max = Slave_Conn_Interval_Max;
    }
    index_input += 2;
  }
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x082;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_discoverable( uint8_t Advertising_Type,
                                     uint16_t Advertising_Interval_Min,
                                     uint16_t Advertising_Interval_Max,
                                     uint8_t Own_Address_Type,
                                     uint8_t Advertising_Filter_Policy,
                                     uint8_t Local_Name_Length,
                                     const uint8_t* Local_Name,
                                     uint8_t Service_Uuid_length,
                                     const uint8_t* Service_Uuid_List,
                                     uint16_t Slave_Conn_Interval_Min,
                                     uint16_t Slave_Conn_Interval_Max )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_discoverable_cp0 *cp0 = (aci_gap_set_discoverable_cp0*)(cmd_buffer);
  aci_gap_set_discoverable_cp1 *cp1 = (aci_gap_set_discoverable_cp1*)(cmd_buffer + 1 + 2 + 2 + 1 + 1 + 1 + Local_Name_Length * (sizeof(uint8_t)));
  aci_gap_set_discoverable_cp2 *cp2 = (aci_gap_set_discoverable_cp2*)(cmd_buffer + 1 + 2 + 2 + 1 + 1 + 1 + Local_Name_Length * (sizeof(uint8_t)) + 1 + Service_Uuid_length * (sizeof(uint8_t)));
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Advertising_Type = Advertising_Type;
  index_input += 1;
  cp0->Advertising_Interval_Min = Advertising_Interval_Min;
  index_input += 2;
  cp0->Advertising_Interval_Max = Advertising_Interval_Max;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Advertising_Filter_Policy = Advertising_Filter_Policy;
  index_input += 1;
  cp0->Local_Name_Length = Local_Name_Length;
  index_input += 1;
  /* var_len_data input */
  {
    Osal_MemCpy( (void*)&cp0->Local_Name, (const void*)Local_Name, Local_Name_Length );
    index_input += Local_Name_Length;
    {
      cp1->Service_Uuid_length = Service_Uuid_length;
    }
    index_input += 1;
    Osal_MemCpy( (void*)&cp1->Service_Uuid_List, (const void*)Service_Uuid_List, Service_Uuid_length );
    index_input += Service_Uuid_length;
    {
      cp2->Slave_Conn_Interval_Min = Slave_Conn_Interval_Min;
    }
    index_input += 2;
    {
      cp2->Slave_Conn_Interval_Max = Slave_Conn_Interval_Max;
    }
    index_input += 2;
  }
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x083;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_direct_connectable( uint8_t Own_Address_Type,
                                           uint8_t Directed_Advertising_Type,
                                           uint8_t Direct_Address_Type,
                                           const uint8_t* Direct_Address,
                                           uint16_t Advertising_Interval_Min,
                                           uint16_t Advertising_Interval_Max )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_direct_connectable_cp0 *cp0 = (aci_gap_set_direct_connectable_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Directed_Advertising_Type = Directed_Advertising_Type;
  index_input += 1;
  cp0->Direct_Address_Type = Direct_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Direct_Address, (const void*)Direct_Address, 6 );
  index_input += 6;
  cp0->Advertising_Interval_Min = Advertising_Interval_Min;
  index_input += 2;
  cp0->Advertising_Interval_Max = Advertising_Interval_Max;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x084;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_io_capability( uint8_t IO_Capability )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_io_capability_cp0 *cp0 = (aci_gap_set_io_capability_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->IO_Capability = IO_Capability;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x085;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_authentication_requirement( uint8_t Bonding_Mode,
                                                   uint8_t MITM_Mode,
                                                   uint8_t SC_Support,
                                                   uint8_t KeyPress_Notification_Support,
                                                   uint8_t Min_Encryption_Key_Size,
                                                   uint8_t Max_Encryption_Key_Size,
                                                   uint8_t Use_Fixed_Pin,
                                                   uint32_t Fixed_Pin,
                                                   uint8_t Identity_Address_Type )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_authentication_requirement_cp0 *cp0 = (aci_gap_set_authentication_requirement_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Bonding_Mode = Bonding_Mode;
  index_input += 1;
  cp0->MITM_Mode = MITM_Mode;
  index_input += 1;
  cp0->SC_Support = SC_Support;
  index_input += 1;
  cp0->KeyPress_Notification_Support = KeyPress_Notification_Support;
  index_input += 1;
  cp0->Min_Encryption_Key_Size = Min_Encryption_Key_Size;
  index_input += 1;
  cp0->Max_Encryption_Key_Size = Max_Encryption_Key_Size;
  index_input += 1;
  cp0->Use_Fixed_Pin = Use_Fixed_Pin;
  index_input += 1;
  cp0->Fixed_Pin = Fixed_Pin;
  index_input += 4;
  cp0->Identity_Address_Type = Identity_Address_Type;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x086;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_authorization_requirement( uint16_t Connection_Handle,
                                                  uint8_t Authorization_Enable )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_authorization_requirement_cp0 *cp0 = (aci_gap_set_authorization_requirement_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Authorization_Enable = Authorization_Enable;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x087;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_pass_key_resp( uint16_t Connection_Handle,
                                  uint32_t Pass_Key )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_pass_key_resp_cp0 *cp0 = (aci_gap_pass_key_resp_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Pass_Key = Pass_Key;
  index_input += 4;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x088;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_authorization_resp( uint16_t Connection_Handle,
                                       uint8_t Authorize )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_authorization_resp_cp0 *cp0 = (aci_gap_authorization_resp_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Authorize = Authorize;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x089;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_init( uint8_t Role,
                         uint8_t privacy_enabled,
                         uint8_t device_name_char_len,
                         uint16_t* Service_Handle,
                         uint16_t* Dev_Name_Char_Handle,
                         uint16_t* Appearance_Char_Handle )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_init_cp0 *cp0 = (aci_gap_init_cp0*)(cmd_buffer);
  aci_gap_init_rp0 resp;
  Osal_MemSet( &resp, 0, sizeof(resp) );
  int index_input = 0;
  cp0->Role = Role;
  index_input += 1;
  cp0->privacy_enabled = privacy_enabled;
  index_input += 1;
  cp0->device_name_char_len = device_name_char_len;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08a;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &resp;
  rq.rlen = sizeof(resp);
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  if ( resp.Status )
    return resp.Status;
  *Service_Handle = resp.Service_Handle;
  *Dev_Name_Char_Handle = resp.Dev_Name_Char_Handle;
  *Appearance_Char_Handle = resp.Appearance_Char_Handle;
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_set_non_connectable( uint8_t Advertising_Event_Type,
                                        uint8_t Own_Address_Type )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_non_connectable_cp0 *cp0 = (aci_gap_set_non_connectable_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Advertising_Event_Type = Advertising_Event_Type;
  index_input += 1;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08b;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_set_undirected_connectable( uint16_t Advertising_Interval_Min,
                                               uint16_t Advertising_Interval_Max,
                                               uint8_t Own_Address_Type,
                                               uint8_t Adv_Filter_Policy )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_undirected_connectable_cp0 *cp0 = (aci_gap_set_undirected_connectable_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Advertising_Interval_Min = Advertising_Interval_Min;
  index_input += 2;
  cp0->Advertising_Interval_Max = Advertising_Interval_Max;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Adv_Filter_Policy = Adv_Filter_Policy;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08c;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_slave_security_req( uint16_t Connection_Handle )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_slave_security_req_cp0 *cp0 = (aci_gap_slave_security_req_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08d;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_update_adv_data( uint8_t AdvDataLen,
                                    const uint8_t* AdvData )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_update_adv_data_cp0 *cp0 = (aci_gap_update_adv_data_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->AdvDataLen = AdvDataLen;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->AdvData, (const void*)AdvData, AdvDataLen );
  index_input += AdvDataLen;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08e;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_delete_ad_type( uint8_t ADType )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_delete_ad_type_cp0 *cp0 = (aci_gap_delete_ad_type_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->ADType = ADType;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x08f;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_get_security_level( uint16_t Connection_Handle,
                                       uint8_t* Security_Mode,
                                       uint8_t* Security_Level )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_get_security_level_cp0 *cp0 = (aci_gap_get_security_level_cp0*)(cmd_buffer);
  aci_gap_get_security_level_rp0 resp;
  Osal_MemSet( &resp, 0, sizeof(resp) );
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x090;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &resp;
  rq.rlen = sizeof(resp);
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  if ( resp.Status )
    return resp.Status;
  *Security_Mode = resp.Security_Mode;
  *Security_Level = resp.Security_Level;
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_set_event_mask( uint16_t GAP_Evt_Mask )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_event_mask_cp0 *cp0 = (aci_gap_set_event_mask_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->GAP_Evt_Mask = GAP_Evt_Mask;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x091;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_configure_whitelist( void )
{
  struct hci_request rq;
  tBleStatus status = 0;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x092;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_terminate( uint16_t Connection_Handle,
                              uint8_t Reason )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_terminate_cp0 *cp0 = (aci_gap_terminate_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Reason = Reason;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x093;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_clear_security_db( void )
{
  struct hci_request rq;
  tBleStatus status = 0;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x094;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_allow_rebond( uint16_t Connection_Handle )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_allow_rebond_cp0 *cp0 = (aci_gap_allow_rebond_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x095;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_limited_discovery_proc( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_limited_discovery_proc_cp0 *cp0 = (aci_gap_start_limited_discovery_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Filter_Duplicates = Filter_Duplicates;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x096;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_general_discovery_proc( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_general_discovery_proc_cp0 *cp0 = (aci_gap_start_general_discovery_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Filter_Duplicates = Filter_Duplicates;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x097;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_name_discovery_proc( uint16_t LE_Scan_Interval,
                                              uint16_t LE_Scan_Window,
                                              uint8_t Peer_Address_Type,
                                              const uint8_t* Peer_Address,
                                              uint8_t Own_Address_Type,
                                              uint16_t Conn_Interval_Min,
                                              uint16_t Conn_Interval_Max,
                                              uint16_t Conn_Latency,
                                              uint16_t Supervision_Timeout,
                                              uint16_t Minimum_CE_Length,
                                              uint16_t Maximum_CE_Length )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_name_discovery_proc_cp0 *cp0 = (aci_gap_start_name_discovery_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Peer_Address_Type = Peer_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Peer_Address, (const void*)Peer_Address, 6 );
  index_input += 6;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Conn_Interval_Min = Conn_Interval_Min;
  index_input += 2;
  cp0->Conn_Interval_Max = Conn_Interval_Max;
  index_input += 2;
  cp0->Conn_Latency = Conn_Latency;
  index_input += 2;
  cp0->Supervision_Timeout = Supervision_Timeout;
  index_input += 2;
  cp0->Minimum_CE_Length = Minimum_CE_Length;
  index_input += 2;
  cp0->Maximum_CE_Length = Maximum_CE_Length;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x098;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_auto_connection_establish_proc( uint16_t LE_Scan_Interval,
                                                         uint16_t LE_Scan_Window,
                                                         uint8_t Own_Address_Type,
                                                         uint16_t Conn_Interval_Min,
                                                         uint16_t Conn_Interval_Max,
                                                         uint16_t Conn_Latency,
                                                         uint16_t Supervision_Timeout,
                                                         uint16_t Minimum_CE_Length,
                                                         uint16_t Maximum_CE_Length,
                                                         uint8_t Num_of_Whitelist_Entries,
                                                         const Whitelist_Entry_t* Whitelist_Entry )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_auto_connection_establish_proc_cp0 *cp0 = (aci_gap_start_auto_connection_establish_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Conn_Interval_Min = Conn_Interval_Min;
  index_input += 2;
  cp0->Conn_Interval_Max = Conn_Interval_Max;
  index_input += 2;
  cp0->Conn_Latency = Conn_Latency;
  index_input += 2;
  cp0->Supervision_Timeout = Supervision_Timeout;
  index_input += 2;
  cp0->Minimum_CE_Length = Minimum_CE_Length;
  index_input += 2;
  cp0->Maximum_CE_Length = Maximum_CE_Length;
  index_input += 2;
  cp0->Num_of_Whitelist_Entries = Num_of_Whitelist_Entries;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Whitelist_Entry, (const void*)Whitelist_Entry, Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t)) );
  index_input += Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t));
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x099;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_general_connection_establish_proc( uint8_t LE_Scan_Type,
                                                            uint16_t LE_Scan_Interval,
                                                            uint16_t LE_Scan_Window,
                                                            uint8_t Own_Address_Type,
                                                            uint8_t Scanning_Filter_Policy,
                                                            uint8_t Filter_Duplicates )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_general_connection_establish_proc_cp0 *cp0 = (aci_gap_start_general_connection_establish_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Type = LE_Scan_Type;
  index_input += 1;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Scanning_Filter_Policy = Scanning_Filter_Policy;
  index_input += 1;
  cp0->Filter_Duplicates = Filter_Duplicates;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09a;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_selective_connection_establish_proc( uint8_t LE_Scan_Type,
                                                              uint16_t LE_Scan_Interval,
                                                              uint16_t LE_Scan_Window,
                                                              uint8_t Own_Address_Type,
                                                              uint8_t Scanning_Filter_Policy,
                                                              uint8_t Filter_Duplicates,
                                                              uint8_t Num_of_Whitelist_Entries,
                                                              const Whitelist_Entry_t* Whitelist_Entry )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_selective_connection_establish_proc_cp0 *cp0 = (aci_gap_start_selective_connection_establish_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Type = LE_Scan_Type;
  index_input += 1;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Scanning_Filter_Policy = Scanning_Filter_Policy;
  index_input += 1;
  cp0->Filter_Duplicates = Filter_Duplicates;
  index_input += 1;
  cp0->Num_of_Whitelist_Entries = Num_of_Whitelist_Entries;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Whitelist_Entry, (const void*)Whitelist_Entry, Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t)) );
  index_input += Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t));
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09b;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_create_connection( uint16_t LE_Scan_Interval,
                                      uint16_t LE_Scan_Window,
                                      uint8_t Peer_Address_Type,
                                      const uint8_t* Peer_Address,
                                      uint8_t Own_Address_Type,
                                      uint16_t Conn_Interval_Min,
                                      uint16_t Conn_Interval_Max,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint16_t Minimum_CE_Length,
                                      uint16_t Maximum_CE_Length )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_create_connection_cp0 *cp0 = (aci_gap_create_connection_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->Peer_Address_Type = Peer_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Peer_Address, (const void*)Peer_Address, 6 );
  index_input += 6;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Conn_Interval_Min = Conn_Interval_Min;
  index_input += 2;
  cp0->Conn_Interval_Max = Conn_Interval_Max;
  index_input += 2;
  cp0->Conn_Latency = Conn_Latency;
  index_input += 2;
  cp0->Supervision_Timeout = Supervision_Timeout;
  index_input += 2;
  cp0->Minimum_CE_Length = Minimum_CE_Length;
  index_input += 2;
  cp0->Maximum_CE_Length = Maximum_CE_Length;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09c;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_terminate_gap_proc( uint8_t Procedure_Code )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_terminate_gap_proc_cp0 *cp0 = (aci_gap_terminate_gap_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Procedure_Code = Procedure_Code;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09d;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_connection_update( uint16_t Connection_Handle,
                                            uint16_t Conn_Interval_Min,
                                            uint16_t Conn_Interval_Max,
                                            uint16_t Conn_Latency,
                                            uint16_t Supervision_Timeout,
                                            uint16_t Minimum_CE_Length,
                                            uint16_t Maximum_CE_Length )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_connection_update_cp0 *cp0 = (aci_gap_start_connection_update_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Conn_Interval_Min = Conn_Interval_Min;
  index_input += 2;
  cp0->Conn_Interval_Max = Conn_Interval_Max;
  index_input += 2;
  cp0->Conn_Latency = Conn_Latency;
  index_input += 2;
  cp0->Supervision_Timeout = Supervision_Timeout;
  index_input += 2;
  cp0->Minimum_CE_Length = Minimum_CE_Length;
  index_input += 2;
  cp0->Maximum_CE_Length = Maximum_CE_Length;
  index_input += 2;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09e;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_send_pairing_req( uint16_t Connection_Handle,
                                     uint8_t Force_Rebond )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_send_pairing_req_cp0 *cp0 = (aci_gap_send_pairing_req_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Force_Rebond = Force_Rebond;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x09f;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_resolve_private_addr( const uint8_t* Address,
                                         uint8_t* Actual_Address )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_resolve_private_addr_cp0 *cp0 = (aci_gap_resolve_private_addr_cp0*)(cmd_buffer);
  aci_gap_resolve_private_addr_rp0 resp;
  Osal_MemSet( &resp, 0, sizeof(resp) );
  int index_input = 0;
  Osal_MemCpy( (void*)&cp0->Address, (const void*)Address, 6 );
  index_input += 6;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a0;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &resp;
  rq.rlen = sizeof(resp);
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  if ( resp.Status )
    return resp.Status;
  Osal_MemCpy( (void*)Actual_Address, (const void*)resp.Actual_Address, 6 );
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_set_broadcast_mode( uint16_t Advertising_Interval_Min,
                                       uint16_t Advertising_Interval_Max,
                                       uint8_t Advertising_Type,
                                       uint8_t Own_Address_Type,
                                       uint8_t Adv_Data_Length,
                                       const uint8_t* Adv_Data,
                                       uint8_t Num_of_Whitelist_Entries,
                                       const Whitelist_Entry_t* Whitelist_Entry )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_broadcast_mode_cp0 *cp0 = (aci_gap_set_broadcast_mode_cp0*)(cmd_buffer);
  aci_gap_set_broadcast_mode_cp1 *cp1 = (aci_gap_set_broadcast_mode_cp1*)(cmd_buffer + 2 + 2 + 1 + 1 + 1 + Adv_Data_Length * (sizeof(uint8_t)));
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Advertising_Interval_Min = Advertising_Interval_Min;
  index_input += 2;
  cp0->Advertising_Interval_Max = Advertising_Interval_Max;
  index_input += 2;
  cp0->Advertising_Type = Advertising_Type;
  index_input += 1;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Adv_Data_Length = Adv_Data_Length;
  index_input += 1;
  /* var_len_data input */
  {
    Osal_MemCpy( (void*)&cp0->Adv_Data, (const void*)Adv_Data, Adv_Data_Length );
    index_input += Adv_Data_Length;
    {
      cp1->Num_of_Whitelist_Entries = Num_of_Whitelist_Entries;
    }
    index_input += 1;
    Osal_MemCpy( (void*)&cp1->Whitelist_Entry, (const void*)Whitelist_Entry, Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t)) );
    index_input += Num_of_Whitelist_Entries * (sizeof(Whitelist_Entry_t));
  }
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a1;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_start_observation_proc( uint16_t LE_Scan_Interval,
                                           uint16_t LE_Scan_Window,
                                           uint8_t LE_Scan_Type,
                                           uint8_t Own_Address_Type,
                                           uint8_t Filter_Duplicates,
                                           uint8_t Scanning_Filter_Policy )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_start_observation_proc_cp0 *cp0 = (aci_gap_start_observation_proc_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->LE_Scan_Interval = LE_Scan_Interval;
  index_input += 2;
  cp0->LE_Scan_Window = LE_Scan_Window;
  index_input += 2;
  cp0->LE_Scan_Type = LE_Scan_Type;
  index_input += 1;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  cp0->Filter_Duplicates = Filter_Duplicates;
  index_input += 1;
  cp0->Scanning_Filter_Policy = Scanning_Filter_Policy;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a2;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_get_bonded_devices( uint8_t* Num_of_Addresses,
                                       Bonded_Device_Entry_t* Bonded_Device_Entry )
{
  struct hci_request rq;
  aci_gap_get_bonded_devices_rp0 resp;
  Osal_MemSet( &resp, 0, sizeof(resp) );
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a3;
  rq.rparam = &resp;
  rq.rlen = sizeof(resp);
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  if ( resp.Status )
    return resp.Status;
  *Num_of_Addresses = resp.Num_of_Addresses;
  Osal_MemCpy( (void*)Bonded_Device_Entry, (const void*)resp.Bonded_Device_Entry, *Num_of_Addresses * (sizeof(Bonded_Device_Entry_t)) );
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_is_device_bonded( uint8_t Peer_Address_Type,
                                     const uint8_t* Peer_Address )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_is_device_bonded_cp0 *cp0 = (aci_gap_is_device_bonded_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Peer_Address_Type = Peer_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Peer_Address, (const void*)Peer_Address, 6 );
  index_input += 6;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a4;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_numeric_comparison_value_confirm_yesno( uint16_t Connection_Handle,
                                                           uint8_t Confirm_Yes_No )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_numeric_comparison_value_confirm_yesno_cp0 *cp0 = (aci_gap_numeric_comparison_value_confirm_yesno_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Confirm_Yes_No = Confirm_Yes_No;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a5;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_passkey_input( uint16_t Connection_Handle,
                                  uint8_t Input_Type )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_passkey_input_cp0 *cp0 = (aci_gap_passkey_input_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Connection_Handle = Connection_Handle;
  index_input += 2;
  cp0->Input_Type = Input_Type;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a6;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_get_oob_data( uint8_t OOB_Data_Type,
                                 uint8_t* Address_Type,
                                 uint8_t* Address,
                                 uint8_t* OOB_Data_Len,
                                 uint8_t* OOB_Data )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_get_oob_data_cp0 *cp0 = (aci_gap_get_oob_data_cp0*)(cmd_buffer);
  aci_gap_get_oob_data_rp0 resp;
  Osal_MemSet( &resp, 0, sizeof(resp) );
  int index_input = 0;
  cp0->OOB_Data_Type = OOB_Data_Type;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a7;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &resp;
  rq.rlen = sizeof(resp);
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  if ( resp.Status )
    return resp.Status;
  *Address_Type = resp.Address_Type;
  Osal_MemCpy( (void*)Address, (const void*)resp.Address, 6 );
  *OOB_Data_Len = resp.OOB_Data_Len;
  Osal_MemCpy( (void*)OOB_Data, (const void*)resp.OOB_Data, 16 );
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_set_oob_data( uint8_t Device_Type,
                                 uint8_t Address_Type,
                                 const uint8_t* Address,
                                 uint8_t OOB_Data_Type,
                                 uint8_t OOB_Data_Len,
                                 const uint8_t* OOB_Data )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_set_oob_data_cp0 *cp0 = (aci_gap_set_oob_data_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Device_Type = Device_Type;
  index_input += 1;
  cp0->Address_Type = Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Address, (const void*)Address, 6 );
  index_input += 6;
  cp0->OOB_Data_Type = OOB_Data_Type;
  index_input += 1;
  cp0->OOB_Data_Len = OOB_Data_Len;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->OOB_Data, (const void*)OOB_Data, 16 );
  index_input += 16;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a8;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_add_devices_to_resolving_list( uint8_t Num_of_Resolving_list_Entries,
                                                  const Whitelist_Identity_Entry_t* Whitelist_Identity_Entry,
                                                  uint8_t Clear_Resolving_List )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_add_devices_to_resolving_list_cp0 *cp0 = (aci_gap_add_devices_to_resolving_list_cp0*)(cmd_buffer);
  aci_gap_add_devices_to_resolving_list_cp1 *cp1 = (aci_gap_add_devices_to_resolving_list_cp1*)(cmd_buffer + 1 + Num_of_Resolving_list_Entries * (sizeof(Whitelist_Identity_Entry_t)));
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Num_of_Resolving_list_Entries = Num_of_Resolving_list_Entries;
  index_input += 1;
  /* var_len_data input */
  {
    Osal_MemCpy( (void*)&cp0->Whitelist_Identity_Entry, (const void*)Whitelist_Identity_Entry, Num_of_Resolving_list_Entries * (sizeof(Whitelist_Identity_Entry_t)) );
    index_input += Num_of_Resolving_list_Entries * (sizeof(Whitelist_Identity_Entry_t));
    {
      cp1->Clear_Resolving_List = Clear_Resolving_List;
    }
    index_input += 1;
  }
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0a9;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_remove_bonded_device( uint8_t Peer_Identity_Address_Type,
                                         const uint8_t* Peer_Identity_Address )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_remove_bonded_device_cp0 *cp0 = (aci_gap_remove_bonded_device_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Peer_Identity_Address_Type = Peer_Identity_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Peer_Identity_Address, (const void*)Peer_Identity_Address, 6 );
  index_input += 6;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0aa;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_additional_beacon_start( uint16_t Adv_Interval_Min,
                                            uint16_t Adv_Interval_Max,
                                            uint8_t Adv_Channel_Map,
                                            uint8_t Own_Address_Type,
                                            const uint8_t* Own_Address,
                                            uint8_t PA_Level )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_additional_beacon_start_cp0 *cp0 = (aci_gap_additional_beacon_start_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Adv_Interval_Min = Adv_Interval_Min;
  index_input += 2;
  cp0->Adv_Interval_Max = Adv_Interval_Max;
  index_input += 2;
  cp0->Adv_Channel_Map = Adv_Channel_Map;
  index_input += 1;
  cp0->Own_Address_Type = Own_Address_Type;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Own_Address, (const void*)Own_Address, 6 );
  index_input += 6;
  cp0->PA_Level = PA_Level;
  index_input += 1;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0b0;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_additional_beacon_stop( void )
{
  struct hci_request rq;
  tBleStatus status = 0;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0b1;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

tBleStatus aci_gap_additional_beacon_set_data( uint8_t Adv_Data_Length,
                                               const uint8_t* Adv_Data )
{
  struct hci_request rq;
  uint8_t cmd_buffer[BLE_CMD_MAX_PARAM_LEN];
  aci_gap_additional_beacon_set_data_cp0 *cp0 = (aci_gap_additional_beacon_set_data_cp0*)(cmd_buffer);
  tBleStatus status = 0;
  int index_input = 0;
  cp0->Adv_Data_Length = Adv_Data_Length;
  index_input += 1;
  Osal_MemCpy( (void*)&cp0->Adv_Data, (const void*)Adv_Data, Adv_Data_Length );
  index_input += Adv_Data_Length;
  Osal_MemSet( &rq, 0, sizeof(rq) );
  rq.ogf = 0x3f;
  rq.ocf = 0x0b2;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if ( hci_send_req(&rq, FALSE) < 0 )
    return BLE_STATUS_TIMEOUT;
  return status;
}

