/**
 ******************************************************************************
 * @file    hci_consth
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    04-September-2018
 * @brief   This file defines constants and functions for HCI layer.
 * 			See Bluetooth Core v 4.0, Vol. 2, Part E.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

#ifndef __HCI_CONST_H_
#define __HCI_CONST_H_

/* ----------------------------------- *
 *  Packed usage (compiler dependent)  *
 * ----------------------------------- */
#undef PACKED__
#undef PACKED_STRUCT

#if defined ( __CC_ARM )
  #if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #define PACKED__ __attribute__((packed))
    #define PACKED_STRUCT struct PACKED__
  #else
    #define PACKED__(TYPE) __packed TYPE
    #define PACKED_STRUCT PACKED__(struct)
  #endif
#elif defined   ( __GNUC__ )
  #define PACKED__ __attribute__((packed))
  #define PACKED_STRUCT struct PACKED__
#elif defined (__ICCARM__)
  #define PACKED_STRUCT __packed struct
#elif
  #define PACKED_STRUCT __packed struct
#endif


/*------------- Events -------------*/
/**
 * The event code in the @ref hci_event_pckt structure. If event code is EVT_VENDOR,
 * application can use @ref evt_blue_aci structure to parse the packet.
 */
#define EVT_VENDOR                      0xFF

#define EVT_CONN_COMPLETE               0x03
#define EVT_DISCONN_COMPLETE            0x05
#define EVT_LE_META_EVENT               0x3E
#define EVT_LE_CONN_UPDATE_COMPLETE     0x03
#define EVT_LE_CONN_COMPLETE            0x01
#define EVT_LE_ADVERTISING_REPORT       0x02
#define EVT_LE_PHY_UPDATE_COMPLETE      0x0C
#define EVT_LE_ENHANCED_CONN_COMPLETE	0x0A

typedef PACKED_STRUCT _hci_uart_pckt{
  uint8_t type;
  uint8_t data[1];
}  hci_uart_pckt;

typedef PACKED_STRUCT _hci_event_pckt{
  uint8_t         evt;
  uint8_t         plen;
  uint8_t         data[1];
}  hci_event_pckt;

typedef PACKED_STRUCT _evt_le_meta_event{
  uint8_t         subevent;
  uint8_t         data[1];
}  evt_le_meta_event;

/**
 * Vendor specific event for BLE core.
 */
typedef PACKED_STRUCT _evt_blue_aci{
  uint16_t ecode; /**< One of the BLE core event codes. */
  uint8_t  data[1];
} evt_blue_aci;



#endif /* __HCI_CONST_H_ */
