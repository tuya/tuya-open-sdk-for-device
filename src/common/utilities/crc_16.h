/**
 * @file crc_16.h
 * @brief Header file for CRC-16 checksum calculation function.
 *
 * @copyright Copyright 2016-2021 Tuya Inc. All Rights Reserved.
 **/
  
#ifndef _CRC_16_H
#define _CRC_16_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __CRC_16_GLOBALS
#define __CRC_16_MODULE_EXT
#else
#define __CRC_16_MODULE_EXT extern
#endif

/**
 * @brief Calculates the CRC-16 checksum for the given data.
 *
 * @param data The data for which the CRC-16 checksum needs to be calculated.
 * @param size The size of the data in bytes.
 * @return The calculated CRC-16 checksum.
 */
__CRC_16_MODULE_EXT
unsigned short get_crc_16(unsigned char *data, unsigned short size);

#ifdef __cplusplus
}
#endif
#endif
