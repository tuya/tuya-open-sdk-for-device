/**
 * @file mix_method.c
 * @brief Collection of utility functions for string manipulation,
 * encoding/decoding, and other miscellaneous operations.
 *
 * This file contains implementations of various utility functions including but
 * not limited to string comparison, lower case conversion, and memory
 * management wrappers. These functions are designed to facilitate common
 * operations required across different modules of the application, providing a
 * centralized and reusable codebase.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#define __MIX_METHOD_GLOBALS
#include "mix_method.h"
#include "tal_memory.h"
#include "mbedtls/base64.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define __tolower(c)                 ((('A' <= (c)) && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))
#define TY_BASE64_BUF_LEN_CALC(slen) (((slen) / 3 + ((slen) % 3 != 0)) * 4 + 1) // 1 for '\0'

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/**
 * @brief Compare two strings lexicographically.
 *
 * This function compares the two strings `src` and `dst` lexicographically.
 * It returns an integer less than, equal to, or greater than zero if `src` is
 * found, respectively, to be less than, to match, or be greater than `dst`.
 *
 * @param src The first string to be compared.
 * @param dst The second string to be compared.
 * @return An integer less than, equal to, or greater than zero if `src` is
 * found, respectively, to be less than, to match, or be greater than `dst`.
 */
int uni_strcmp(const char *src, const char *dst)
{
    int ret = 0;

    while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst) {
        ++src, ++dst;
    }

    if (ret < 0) {
        ret = -1;
    } else if (ret > 0) {
        ret = 1;
    }

    return (ret);
}

/**
 * @brief Converts an ASCII character to its corresponding hexadecimal value.
 *
 * This function takes an ASCII character as input and returns its corresponding
 * hexadecimal value. The input character must be a valid hexadecimal digit
 * (0-9, a-f, A-F). If the input character is not a valid hexadecimal digit, the
 * function returns 0.
 *
 * @param asccode The ASCII character to be converted to hexadecimal.
 * @return The hexadecimal value of the input character.
 */
unsigned char asc2hex(char asccode)
{
    unsigned char ret;

    if ('0' <= asccode && asccode <= '9') {
        ret = asccode - '0';
    } else if ('a' <= asccode && asccode <= 'f') {
        ret = asccode - 'a' + 10;
    } else if ('A' <= asccode && asccode <= 'F') {
        ret = asccode - 'A' + 10;
    } else {
        ret = 0;
    }

    return ret;
}

/**
 * Converts ASCII characters to hexadecimal representation.
 *
 * @param hex The output buffer to store the hexadecimal representation.
 * @param ascs The input buffer containing ASCII characters.
 * @param srclen The length of the input buffer.
 */
void ascs2hex(unsigned char *hex, unsigned char *ascs, int srclen)
{
    unsigned char l4, h4;
    int i, lenstr;
    lenstr = srclen;

    if (lenstr % 2) {
        lenstr -= (lenstr % 2);
    }

    if (lenstr == 0) {
        return;
    }

    for (i = 0; i < lenstr; i += 2) {
        h4 = asc2hex(ascs[i]);
        l4 = asc2hex(ascs[i + 1]);
        hex[i / 2] = (h4 << 4) + l4;
    }
}

/**
 * Converts an array of hexadecimal values to a string representation.
 *
 * @param pbDest The destination buffer to store the converted string.
 * @param pbSrc The source buffer containing the hexadecimal values.
 * @param nLen The length of the source buffer.
 */
void hex2str(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++) {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) {
            ddh = ddh + 7;
        }
        if (ddl > 57) {
            ddl = ddl + 7;
        }
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

/**
 * @brief Converts an array of bytes to a string representation.
 *
 * This function takes an array of bytes `pbSrc` and converts it to a string
 * representation in the `pbDest` array. The length of the input array is
 * specified by `nLen`. The resulting string will be twice the length of the
 * input array, as each byte is represented by two characters in the string.
 *
 * The `upper` parameter determines whether the resulting string should use
 * uppercase or lowercase characters for hexadecimal digits. If `upper` is
 * `true`, uppercase characters will be used; if `upper` is `false`, lowercase
 * characters will be used.
 *
 * @param pbDest Pointer to the destination array where the resulting string
 * will be stored.
 * @param pbSrc Pointer to the source array of bytes to be converted.
 * @param nLen Length of the source array.
 * @param upper Flag indicating whether to use uppercase or lowercase characters
 * in the resulting string.
 *
 * @return None.
 */
void byte2str(unsigned char *pbDest, unsigned char *pbSrc, int nLen, bool_t upper)
{
    char to_lower[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char to_upper[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    char *tmp = upper ? to_upper : to_lower;

    int i;
    for (i = 0; i < nLen; i++) {
        pbDest[i * 2] = tmp[((pbSrc[i] & 0xF0) >> 4)];
        pbDest[i * 2 + 1] = tmp[(pbSrc[i] & 0x0F)];
    }

    pbDest[nLen * 2] = '\0';
    return;
}

/**
 * @brief Finds the position of a character in a string, starting from the end.
 *
 * @param str The input string to search in.
 * @param revr_index The reverse index to start searching from (starting from
 * 0).
 * @param ch The character to find.
 * @return The position of the character in the string, or -1 if the string is
 * NULL, the reverse index is out of range, or the character is not found.
 *         Returns -2 if the string is empty.
 */
int str_revr_find_ch(const char *str, const int revr_index, const char ch)
{
    if (NULL == str) {
        return -1;
    }

    int len = strlen(str);
    if (revr_index >= len) {
        return -1;
    }

    int i = 0;
    for (i = (len - 1 - revr_index); i >= 0; i--) {
        if (str[i] == ch) {
            return i;
        }
    }

    return -2;
}

/**
 * @brief Sorts an array of bytes in ascending or descending order.
 *
 * @param is_ascend Flag indicating whether to sort in ascending order (1) or
 * descending order (0).
 * @param buf The array of bytes to be sorted.
 * @param buf_cnt The number of elements in the array.
 */
void byte_sort(unsigned char is_ascend, unsigned char *buf, int buf_cnt)
{
    int i, j;
    unsigned char tmp = 0;
    for (j = 1; j < buf_cnt; j++) {
        for (i = 0; i < buf_cnt - j; i++) {
            if (is_ascend) {
                if (buf[i] > buf[i + 1]) {
                    tmp = buf[i];
                    buf[i] = buf[i + 1];
                    buf[i + 1] = tmp;
                }
            } else {
                if (buf[i] < buf[i + 1]) {
                    tmp = buf[i];
                    buf[i] = buf[i + 1];
                    buf[i + 1] = tmp;
                }
            }
        }
    }
}

/**
 * @brief Duplicates a string.
 *
 * This function duplicates the input string and returns a pointer to the
 * duplicated string.
 *
 * @param str The input string to be duplicated.
 * @return A pointer to the duplicated string, or NULL if the input string is
 * NULL or memory allocation fails.
 */
char *mm_strdup(const char *str)
{
    if ((void *)0 == str) {
        return (void *)0;
    }

    char *tmp = tal_malloc(strlen(str) + 1);
    if ((void *)0 == tmp) {
        return (void *)0;
    }
    memcpy(tmp, str, strlen(str));
    tmp[strlen(str)] = '\0';

    return tmp;
}

/**
 * @brief Checks if a version string is valid.
 *
 * This function checks if a version string is valid based on the following
 * criteria:
 * - The version string must not be NULL.
 * - The version string must not exceed 10 characters in length.
 * - The version string must not be empty.
 * - The version string must have at most 2 dot separators.
 * - Each section of the version string must consist of at most 8 digits.
 * - Version format: xx.xx.xx / xx.xx / xxxx
 *
 * @param ver The version string to be checked.
 * @return 1 if the version string is valid, 0 otherwise.
 */
int is_valid_version(char *ver)
{
    int section_num_count = 0; // Number of digits in each section
    int dot = 0;               // Number of dot separators
    char *last = NULL;

    if (ver == NULL) {
        return 0;
    }
    if (strlen(ver) > 10 || strlen(ver) == 0) { // Maximum length of version string is 10 characters
        return 0;
    }

    while (*ver) {
        if (*ver == '.') {
            if (last == NULL || (*last) == '.') {
                return 0;
            }

            dot++;
            if (dot > 2) {
                return 0;
            }

            section_num_count = 0;
        } else if (*ver >= '0' && *ver <= '9') {
            section_num_count++;
            if (section_num_count > 8) { // One possibility is that the version is named based on
                                         // time, e.g., 20190121, where each section can have a
                                         // maximum length of 8 digits
                return 0;
            }
        } else {
            return 0;
        }

        last = ver;
        ver++;
    }

    if (last != NULL && *last == '.') {
        return 0;
    }

    return 1;
}

/**
 * @brief Converts a string representation of a MAC address to its corresponding
 * byte array.
 *
 * This function takes a string representation of a MAC address and converts it
 * to its corresponding byte array format. The input string must be either 12
 * characters long (without colons) or 17 characters long (with colons). The
 * resulting byte array will be stored in the 'mac' parameter.
 *
 * @param str The string representation of the MAC address.
 * @param mac The byte array to store the converted MAC address.
 * @return 0 if the conversion is successful, -1 otherwise.
 */
int string2mac(const char *str, char mac[6])
{
    int len = strlen(str);
    int i = 0;
    int count = 0;

    if (12 != len && 17 != len) {
        return -1;
    }

    while (i < 6) {
        mac[i] = ((asc2hex(str[count]) << 4) & 0xF0) + (asc2hex(str[count + 1]) & 0x0F);
        count = count + 2;
        if (len == 17) {
            count++;
        }
        i++;
    }

    return 0;
}

/**
 * @brief Converts a version string to an integer representation.
 *
 * The version string should be in the format "x.y.z", where x, y, and z are
 * decimal numbers. Each number is converted to a byte and combined into a
 * single integer value.
 *
 * @param versionString The version string to convert.
 *
 * @return The integer representation of the version string.
 *         If the version string is empty or invalid, returns OPRT_INVALID_PARM.
 */
int versionString2int(const char *versionString)
{
    int len = strlen(versionString);
    if (len <= 0) {
        return OPRT_INVALID_PARM;
    }
    static unsigned char v = 0;
    int i = 0;
    int intOfString = 0;
    while (i < len) {
        if (versionString[i] == '.') {
            i++;
            intOfString <<= 8;
            v = 0;
            continue;
        } else {
            unsigned char d = asc2hex(versionString[i]);
            v = v * 10 + d;
            if (versionString[i + 1] == '.' || versionString[i + 1] == '\0') {
                intOfString |= v;
            }
        }
        i++;
    }

    v = 0;
    return intOfString;
}

/**
 * @brief Checks if a character is an alphabetic character.
 *
 * This function checks if the given character is an alphabetic character (A-Z
 * or a-z).
 *
 * @param c The character to be checked.
 * @return Returns the character if it is an alphabetic character, otherwise
 * returns 0.
 */
int tuya_isalpha(int c)
{
    if (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))) {
        return c;
    } else {
        return 0;
    }
}

/**
 * @brief Converts an uppercase character to lowercase.
 *
 * @param c The character to be converted.
 * @return The lowercase equivalent of the input character if it is an uppercase
 * character, otherwise the input character itself.
 */
int tuya_tolower(int c)
{
    return ((c >= 'A') && (c <= 'Z')) ? (c - 'A' + 'a') : c;
}

/**
 * @brief Converts a lowercase character to uppercase.
 *
 * This function takes a character as input and converts it to uppercase if it
 * is a lowercase character. If the input character is already uppercase or not
 * a letter, it is returned as is.
 *
 * @param c The character to be converted.
 * @return The converted character.
 */
int tuya_toupper(int c)
{
    return ((c >= 'a') && (c <= 'z')) ? (c - 'a' + 'A') : c;
}

/**
 * @brief Encodes binary data into base64 format.
 *
 * @param bindata The binary data to be encoded.
 * @param base64 The buffer to store the base64 encoded data.
 * @param binlength The length of the binary data.
 * @return A pointer to the base64 encoded data.
 */
char *tuya_base64_encode(const unsigned char *bindata, char *base64, int binlength)
{
    size_t dlen, olen;
    dlen = TY_BASE64_BUF_LEN_CALC(binlength);
    mbedtls_base64_encode((unsigned char *)base64, dlen, &olen, bindata, binlength);
    return base64;
}

/**
 * @brief Decodes a base64 encoded string.
 *
 * This function decodes the given base64 encoded string and stores the result
 * in the provided buffer.
 *
 * @param base64 The base64 encoded string to decode.
 * @param bindata The buffer to store the decoded data.
 * @return The length of the decoded data.
 */
int tuya_base64_decode(const char *base64, unsigned char *bindata)
{
    size_t olen = 0;

    mbedtls_base64_decode(bindata, strlen(base64), &olen, (unsigned char *)base64, strlen(base64));

    return olen;
}
