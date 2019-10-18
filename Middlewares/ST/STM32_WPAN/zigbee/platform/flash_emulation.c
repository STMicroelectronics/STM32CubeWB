#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "stm32wbxx.h"
#include "stm32wbxx_hal_conf.h"
#include "stm32_wpan_common.h"
#include "flash_emulation.h"

static inline uint32_t
mapAddress(uint32_t aAddress)
{
    return (aAddress + TMP_STORAGE_BUF_ADDR);
} /* mapAddress */

uint32_t
utilsFlashGetSize(void)
{
    return TMP_STORAGE_BUF_SIZE;
} /* utilsFlashGetSize */

/**
 * Write flash. The write operation only clears bits, but never set bits.
 *
 * The flash address starts from 0, and this function maps the input address to the physical address of flash for writing.
 * 0 is always mapped to the beginning of one flash page.
 * The input address should never be mapped to the firmware space or any other protected flash space.
 *
 * @param[in]  aAddress  The start address of the flash to write.
 * @param[in]  aData     The pointer of the data to write.
 * @param[in]  aSize     The size of the data to write.
 *
 * @returns The actual size of octets write to flash.
 *          It is expected the same as aSize, and may be less than aSize.
 *          0 indicates that something wrong happens when writing.
 */
uint32_t
utilsFlashWrite(uint32_t aAddress, uint8_t *aData, uint32_t aSize)
{
    uint32_t nbOfBytesToWrite = MIN(aSize, TMP_STORAGE_BUF_SIZE);

    memcpy((uint8_t *)mapAddress(aAddress), aData, nbOfBytesToWrite);

    return nbOfBytesToWrite;
} /* utilsFlashWrite */

/**
 * Read flash.
 *
 * The flash address starts from 0, and this function maps the input address to the physical address of flash for reading.
 * 0 is always mapped to the beginning of one flash page.
 * The input address should never be mapped to the firmware space or any other protected flash space.
 *
 * @param[in]   aAddress  The start address of the flash to read.
 * @param[Out]  aData     The pointer of buffer for reading.
 * @param[in]   aSize     The size of the data to read.
 *
 * @returns The actual size of octets read to buffer.
 *          It is expected the same as aSize, and may be less than aSize.
 *          0 indicates that something wrong happens when reading.
 */
uint32_t
utilsFlashRead(uint32_t aAddress, uint8_t *aData, uint32_t aSize)
{
    uint32_t nbOfBytesToRead = 0;

    /* Check the number of bytes to read */
    if ((aAddress + aSize) > TMP_STORAGE_BUF_SIZE) {
        nbOfBytesToRead = TMP_STORAGE_BUF_SIZE - aAddress;
    }
    else {
        nbOfBytesToRead = aSize;
    }

    /* Perform the copy */
    memcpy(aData, (uint8_t *)mapAddress(aAddress), nbOfBytesToRead);

    return nbOfBytesToRead;
} /* utilsFlashRead */
