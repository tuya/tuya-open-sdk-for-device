/**
 * @file uni_random.c
 * @brief Provides functions for generating random numbers and strings.
 *
 * This file contains implementations of functions for generating random bytes,
 * integers, and strings using the underlying Tuya TLS library's random
 * function. It includes functionality to generate a random integer within a
 * specified range, a completely random 32-bit unsigned integer, and a random
 * string composed of hexadecimal characters. These utilities are essential for
 * operations requiring randomness, such as generating unique identifiers or
 * keys.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_tls.h"
#include "stdlib.h"

/**
 * @brief Generates a random sequence of bytes.
 *
 * This function generates a random sequence of bytes and stores it in the
 * specified output buffer.
 *
 * @param output The buffer to store the generated random bytes.
 * @param output_len The length of the output buffer.
 * @return The number of bytes generated, or a negative value if an error
 * occurred.
 */
int uni_random_bytes(unsigned char *output, size_t output_len)
{
    return tuya_tls_random(output, output_len);
}

/**
 * @brief Generates a random integer using the uni_random_bytes function.
 *
 * @return The generated random integer.
 */
int uni_random_int(void)
{
    unsigned int output;
    uni_random_bytes((unsigned char *)&output, sizeof(output));
    return output & RAND_MAX;
}

/**
 * @brief Generates a random number within a specified range.
 *
 * @param range The upper limit of the range (exclusive). If range is 0, the
 * default range is 0xff.
 * @return The generated random number.
 */
int uni_random_range(unsigned int range)
{
    if (range == 0) {
        range = 0xff;
    }

    return uni_random_int() % range;
}

/**
 * @brief Generates a random 32-bit unsigned integer.
 *
 * @return The generated random number.
 */
uint32_t uni_random(void)
{
    uint32_t output;
    uni_random_bytes((unsigned char *)&output, sizeof(output));
    return output;
}

/**
 * @brief Generates a random string of characters.
 *
 * This function generates a random string of characters by selecting characters
 * from a seed string.
 *
 * @param dst The destination buffer to store the generated string.
 * @param size The size of the destination buffer.
 * @return 0 if the string is generated successfully, otherwise an error code.
 */
int uni_random_string(char *dst, int size)
{
    int i = 0;
    char const *seed = "0123456789abcdef";

    for (i = 0; i < size; ++i) {
        dst[i] = seed[uni_random_range(16)];
    }

    return 0;
}
