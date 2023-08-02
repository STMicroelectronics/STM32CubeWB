/*****************************************************************************
 * @file    ble_l2cap_aci.h
 * @author  MDG
 * @brief   STM32WB BLE API (l2cap_aci)
 *          Auto-generated file: do not edit!
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLE_L2CAP_ACI_H__
#define BLE_L2CAP_ACI_H__


#include "ble_types.h"

/**
 * @brief ACI_L2CAP_CONNECTION_PARAMETER_UPDATE_REQ
 * Sends an L2CAP connection parameter update request from the Peripheral to
 * the Central.
 * An ACI_L2CAP_CONNECTION_UPDATE_RESP_EVENT event is raised when the Central
 * responds to the request (accepts or rejects).
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Latency Maximum Peripheral latency for the connection in number of
 *        connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout_Multiplier Defines connection timeout parameter in the
 *        following manner: Timeout Multiplier * 10ms.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_parameter_update_req( uint16_t Connection_Handle,
                                                      uint16_t Conn_Interval_Min,
                                                      uint16_t Conn_Interval_Max,
                                                      uint16_t Latency,
                                                      uint16_t Timeout_Multiplier );

/**
 * @brief ACI_L2CAP_CONNECTION_PARAMETER_UPDATE_RESP
 * Accepts or rejects a connection update. This command should be sent in
 * response to an ACI_L2CAP_CONNECTION_UPDATE_REQ_EVENT event from the
 * controller. The accept parameter has to be set if the connection parameters
 * given in the event are acceptable.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Latency Maximum Peripheral latency for the connection in number of
 *        connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout_Multiplier Defines connection timeout parameter in the
 *        following manner: Timeout Multiplier * 10ms.
 * @param Minimum_CE_Length Information parameter about the minimum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Maximum_CE_Length Information parameter about the maximum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Identifier Identifier received in ACI_L2CAP_Connection_Update_Req
 *        event.
 * @param Accept Specify if connection update parameters are acceptable or not.
 *        Values:
 *        - 0x00: Reject
 *        - 0x01: Accept
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_parameter_update_resp( uint16_t Connection_Handle,
                                                       uint16_t Conn_Interval_Min,
                                                       uint16_t Conn_Interval_Max,
                                                       uint16_t Latency,
                                                       uint16_t Timeout_Multiplier,
                                                       uint16_t Minimum_CE_Length,
                                                       uint16_t Maximum_CE_Length,
                                                       uint8_t Identifier,
                                                       uint8_t Accept );

/**
 * @brief ACI_L2CAP_COC_CONNECT
 * This command sends a Credit Based Connection Request packet on the specified
 * connection. See Bluetooth Core specification Vol.3 Part A.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param SPSM Simplified Protocol/Service Multiplexer.
 *        Values:
 *        - 0x0001 ... 0x00FF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 248
 * @param Initial_Credits Number of K-frames that can be received on the
 *        created channel(s) by the L2CAP layer entity sending this packet.
 *        Values:
 *        - 0 ... 65535
 * @param Channel_Number Number of channels to be created. If this parameter is
 *        set to 0, it requests the creation of one LE credit based connection-
 *        oriented channel. Otherwise, it requests the creation of one or more
 *        enhanced credit based connection-oriented channels.
 *        Values:
 *        - 0 ... 5
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_connect( uint16_t Connection_Handle,
                                  uint16_t SPSM,
                                  uint16_t MTU,
                                  uint16_t MPS,
                                  uint16_t Initial_Credits,
                                  uint8_t Channel_Number );

/**
 * @brief ACI_L2CAP_COC_CONNECT_CONFIRM
 * This command sends a Credit Based Connection Response packet. It must be
 * used upon receipt of a connection request through an
 * ACI_L2CAP_COC_CONNECT_EVENT event. See Bluetooth Core specification Vol.3
 * Part A.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 248
 * @param Initial_Credits Number of K-frames that can be received on the
 *        created channel(s) by the L2CAP layer entity sending this packet.
 *        Values:
 *        - 0 ... 65535
 * @param Result This parameter indicates the outcome of the request. A value
 *        of 0x0000 indicates success while a non-zero value indicates the
 *        request is refused.
 *        Values:
 *        - 0x0000 ... 0x000C
 * @param[out] Channel_Number Number of created channels. It is the length of
 *        Channel_Index_List.
 *        Values:
 *        - 0 ... 5
 * @param[out] Channel_Index_List List of channel indexes for which the
 *        primitive applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_connect_confirm( uint16_t Connection_Handle,
                                          uint16_t MTU,
                                          uint16_t MPS,
                                          uint16_t Initial_Credits,
                                          uint16_t Result,
                                          uint8_t* Channel_Number,
                                          uint8_t* Channel_Index_List );

/**
 * @brief ACI_L2CAP_COC_RECONF
 * This command sends a Credit Based Reconfigure Request packet on the
 * specified connection. See Bluetooth Core specification Vol.3 Part A.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 248
 * @param Channel_Number Number of created channels. It is the length of
 *        Channel_Index_List.
 *        Values:
 *        - 1 ... 5
 * @param Channel_Index_List List of channel indexes for which the primitive
 *        applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_reconf( uint16_t Connection_Handle,
                                 uint16_t MTU,
                                 uint16_t MPS,
                                 uint8_t Channel_Number,
                                 const uint8_t* Channel_Index_List );

/**
 * @brief ACI_L2CAP_COC_RECONF_CONFIRM
 * This command sends a Credit Based Reconfigure Response packet. It must be
 * used upon receipt of a Credit Based Reconfigure Request through an
 * ACI_L2CAP_COC_RECONF_EVENT event. See Bluetooth Core specification Vol.3
 * Part A.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Result This parameter indicates the outcome of the request. A value
 *        of 0x0000 indicates success while a non-zero value indicates the
 *        request is refused.
 *        Values:
 *        - 0x0000 ... 0x000C
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_reconf_confirm( uint16_t Connection_Handle,
                                         uint16_t Result );

/**
 * @brief ACI_L2CAP_COC_DISCONNECT
 * This command sends a Disconnection Request signaling packet on the specified
 * connection-oriented channel. See Bluetooth Core specification Vol.3 Part A.
 * The ACI_L2CAP_COC_DISCONNECT_EVENT event is received when the disconnection
 * of the channel is effective.
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_disconnect( uint8_t Channel_Index );

/**
 * @brief ACI_L2CAP_COC_FLOW_CONTROL
 * This command sends a Flow Control Credit signaling packet on the specified
 * connection-oriented channel. See Bluetooth Core specification Vol.3 Part A.
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @param Credits Number of credits the receiving device can increment,
 *        corresponding to the number of K-frames that can be sent to the peer
 *        device sending the Flow Control Credit packet.
 *        Values:
 *        - 1 ... 65535
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_flow_control( uint8_t Channel_Index,
                                       uint16_t Credits );

/**
 * @brief ACI_L2CAP_COC_TX_DATA
 * This command sends a K-frame packet on the specified connection-oriented
 * channel. See Bluetooth Core specification Vol.3 Part A.
 * Note: for the first K-frame of the SDU, the Information data shall contain
 * the L2CAP SDU Length coded on two octets followed by the K-frame information
 * payload. For the next K-frames of the SDU, the Information data shall only
 * contain the K-frame information payload.
 * The Length value must not exceed (BLE_CMD_MAX_PARAM_LEN - 3) i.e. 252 for
 * BLE_CMD_MAX_PARAM_LEN default value.
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @param Length Length of Data (in octets)
 * @param Data Information data
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_tx_data( uint8_t Channel_Index,
                                  uint16_t Length,
                                  const uint8_t* Data );


#endif /* BLE_L2CAP_ACI_H__ */
