/**
 * @file crc_16.c
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */
#define __CRC_16_GLOBALS
#include "crc_16.h"

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/
/**
 * @brief Calculates the CRC-16 checksum for the given data.
 *
 * @param data The data for which the CRC-16 checksum needs to be calculated.
 * @param size The size of the data in bytes.
 * @return The calculated CRC-16 checksum.
 */
unsigned short get_crc_16(unsigned char *data, unsigned short size)
{
    unsigned short poly[2] = {0, 0xa001}; // 0x8005 <==> 0xa001
    unsigned short crc = 0xffff;
    int i, j;

    for (j = size; j > 0; j--) {
        unsigned char ds = *data++;

        for (i = 0; i < 8; i++) {
            crc = (crc >> 1) ^ poly[(crc ^ ds) & 1];
            ds = ds >> 1;
        }
    }

    return crc;
}
