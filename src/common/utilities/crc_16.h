/**
 * @file crc_16.h
 * @brief 
 * @author nzy
 * @version 1.0.0
 * @date 2017-02-22
 */

#ifndef _CRC_16_H
    #define _CRC_16_H

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __CRC_16_GLOBALS
    #define __CRC_16_MODULE_EXT
#else
    #define __CRC_16_MODULE_EXT extern
#endif

/**
 * @brief get_crc_16 
 *
 * @param[in] data
 * @param[in] size
 *
 * @return unsigned short
 */
__CRC_16_MODULE_EXT \
unsigned short get_crc_16(unsigned char *data,unsigned short size);

#ifdef __cplusplus
}
#endif
#endif


