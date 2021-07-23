/* Copyright [2016 - 2021] Exegin Technologies Limited. All rights reserved. */

#ifndef IEEE802154_CRC_H
#define IEEE802154_CRC_H

/* 802.15.4 two-byte FCS */
#define WPAN_CRC_POLYNOMIAL                     0x1021U
#define WPAN_CRC_INITIAL                        0x0000U
#define WPAN_CRC_XOROUT                         0x0000U

/* For COBS (MAC dongles) */
#define PPPINITFCS16    0xffff /* Initial FCS value */
#define PPPGOODFCS16    0xf0b8 /* Good final FCS value */

uint16_t WpanCrc(uint16_t crc, const void *dataPtr, unsigned int dataLen);

#endif
