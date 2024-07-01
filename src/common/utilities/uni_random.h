/**
 * @file uni_random.h
 * @brief Header file for the universal random number generation utilities.
 * 
 * This file declares a set of functions for generating random numbers, strings,
 * and byte sequences. These functions are essential for operations requiring
 * randomness, such as security protocols, unique identifier generation, and
 * random data creation for testing purposes. The API provides a simple interface
 * for initializing the random number generator, generating random 32-bit integers,
 * creating random strings of specified length, generating a random number within
 * a specified range, and filling a buffer with random bytes.
 * 
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 * 
 */
#ifndef __UNI_RANDOM_H__
#define __UNI_RANDOM_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generates a random sequence of bytes.
 *
 * This function generates a random sequence of bytes and stores it in the specified output buffer.
 *
 * @param output The buffer to store the generated random bytes.
 * @param output_len The length of the output buffer.
 * @return The number of bytes generated, or a negative value if an error occurred.
 */
uint32_t uni_random(void);

/**
 * @brief Generates a random integer using the uni_random_bytes function.
 *
 * @return The generated random integer.
 */
int uni_random_init(void);

/**
 * @brief Generates a random string of characters.
 *
 * This function generates a random string of characters by selecting characters from a seed string.
 *
 * @param dst The destination buffer to store the generated string.
 * @param size The size of the destination buffer.
 * @return 0 if the string is generated successfully, otherwise an error code.
 */
int uni_random_string(char* dst, int size);

/**
 * @brief Generates a random number within a specified range.
 *
 * @param range The upper limit of the range (exclusive). If range is 0, the default range is 0xff.
 * @return The generated random number.
 */
int uni_random_range(unsigned int range);

/**
 * @brief Generates a random sequence of bytes.
 *
 * This function generates a random sequence of bytes and stores it in the specified output buffer.
 *
 * @param output The buffer to store the generated random bytes.
 * @param output_len The length of the output buffer.
 * @return The number of bytes generated, or a negative value if an error occurred.
 */
int uni_random_bytes(unsigned char *output, size_t output_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __UNI_RANDOM_H__ */
