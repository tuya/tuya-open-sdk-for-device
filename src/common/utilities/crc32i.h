/**
 * @file crc32i.h
 * @brief Header file for CRC32I hash algorithm functions.
 * 
 * This file declares the functions for initializing, updating, finalizing,
 * and calculating the CRC32I hash value. The CRC32I hash algorithm is utilized
 * for generating a cyclic redundancy check value for a block of data. This is
 * particularly useful for error-checking and ensuring data integrity. The functions
 * provided allow for both incremental and single-step CRC32I hash calculation.
 * 
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 * 
 */

#ifndef __CR_CRC32I_H__
#define __CR_CRC32I_H__

/**
 * @brief Initializes the CRC32I hash algorithm.
 *
 * @return The initial value for the CRC32I hash algorithm.
 */
unsigned int hash_crc32i_init(void);

/**
 * @brief Updates the CRC32I hash value with the given data.
 *
 * This function updates the CRC32I hash value by incorporating the data
 * provided. The hash value is updated in a streaming fashion, allowing
 * for incremental updates.
 *
 * @param hash The current CRC32I hash value.
 * @param data Pointer to the data to be incorporated into the hash.
 * @param size The size of the data in bytes.
 *
 * @return The updated CRC32I hash value.
 */
unsigned int hash_crc32i_update(unsigned int hash, const void *data,
                                unsigned int size);
/**
 * @brief Finishes the CRC32I hash calculation and returns the final hash value.
 *
 * This function is used to finalize the CRC32I hash calculation and obtain the final hash value.
 *
 * @param hash The intermediate hash value obtained from the CRC32I calculation.
 * @return The final CRC32I hash value.
 */
unsigned int hash_crc32i_finish(unsigned int hash);

/**
 * @brief Calculates the CRC32I hash value for the given data.
 *
 * @param data The pointer to the data.
 * @param size The size of the data in bytes.
 * @return The CRC32I hash value.
 */
unsigned int hash_crc32i_total(const void* data, unsigned int size);

#endif
