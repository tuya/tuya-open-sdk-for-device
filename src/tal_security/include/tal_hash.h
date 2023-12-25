/**
* @file tal_hash.h
* @brief Common process - adapter the hash api provide by OS
* @version 0.1
* @date 2022-04-22
*
* @copyright Copyright 2021-2025 Tuya Inc. All Rights Reserved.
*
*/

#ifndef __TAL_HASH_H__
#define __TAL_HASH_H__

#include "tkl_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    TKL_HASH_HANDLE ctx;
    UINT8_T ipad[64]; /*!< HMAC: inner padding */
    UINT8_T opad[64]; /*!< HMAC: outer padding */
}
tal_hash_mac_context_t;

/**
* @brief This function Create&initializes a sha256 context.
*
* @param[out] ctx: sha256 handle
*
* @note This API is used to create and init sha256.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_create_init( TKL_HASH_HANDLE *ctx );

/**
* @brief This function clears a sha256 context.
*
* @param[in] ctx: sha256 handle
*
* @note This API is used to release sha256.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_free( TKL_HASH_HANDLE ctx );

/**
* @brief This function starts a sha224 or sha256 checksum
 *                 calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[in] is224:    This determines which function to use. This must be
*                 either \c 0 for sha256, or \c 1 for sha224.
*
* @note This API is used to start sha256 or sha224.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_starts_ret( TKL_HASH_HANDLE ctx, INT32_T is224 );

/**
* @brief This function feeds an input buffer into an ongoing
*                 sha256 checksum calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[in] input:    The buffer holding the data. This must be a readable
*                 buffer of length \p ilen Bytes.
* @param[in] ilen:     The length of the input data in Bytes.
*
* @note This API is used to update sha256 or sha224.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_sha256_update_ret( TKL_HASH_HANDLE ctx,
                               const UINT8_T *input,
                               size_t ilen );

/**
* @brief This function finishes the sha256 operation, and writes
*                 the result to the output buffer.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[out] output:   The sha224 or sha256 checksum result.
*                 This must be a writable buffer of length \c 32 Bytes.
*
* @note This API is used to out sha256 result.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_sha256_finish_ret( TKL_HASH_HANDLE ctx,
                               UINT8_T output[32] );


/**
* @brief This function Create&initializes a md5 context.
*
* @param[out] ctx: md5 handle
*
* @note This API is used to create and init md5.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_md5_create_init( TKL_HASH_HANDLE *ctx );

/**
* @brief This function clears a md5 context.
*
* @param[in] ctx: md5 handle
*
* @note This API is used to release md5.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_md5_free( TKL_HASH_HANDLE ctx );

/**
* @brief This function starts a md5 checksum
 *                 calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
*
* @note This API is used to start md5.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_md5_starts_ret( TKL_HASH_HANDLE ctx);

/**
* @brief This function feeds an input buffer into an ongoing
*                 md5 checksum calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[in] input:    The buffer holding the data. This must be a readable
*                 buffer of length \p ilen Bytes.
* @param[in] ilen:     The length of the input data in Bytes.
*
* @note This API is used to update md5.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_md5_update_ret( TKL_HASH_HANDLE ctx,
                               const UINT8_T *input,
                               size_t ilen );

/**
* @brief This function finishes the md5 operation, and writes
*                 the result to the output buffer.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[out] output:   The sha224 or md5 checksum result.
*                 This must be a writable buffer of length \c 16 Bytes.
*
* @note This API is used to out md5 result.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_md5_finish_ret( TKL_HASH_HANDLE ctx,
                               UINT8_T output[16] );

/**
* @brief This function Create&initializes a sha1 context.
*
* @param[out] ctx: sha1 handle
*
* @note This API is used to create and init sha1.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_create_init( TKL_HASH_HANDLE *ctx );

/**
* @brief This function clears a sha1 context.
*
* @param[in] ctx: sha1 handle
*
* @note This API is used to release sha1.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_free( TKL_HASH_HANDLE ctx );

/**
* @brief This function starts a sha1 checksum
 *                 calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
*
* @note This API is used to start sha1.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_starts_ret( TKL_HASH_HANDLE ctx);

/**
* @brief This function feeds an input buffer into an ongoing
*                 sha1 checksum calculation.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[in] input:    The buffer holding the data. This must be a readable
*                 buffer of length \p ilen Bytes.
* @param[in] ilen:     The length of the input data in Bytes.
*
* @note This API is used to update sha1.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_sha1_update_ret( TKL_HASH_HANDLE ctx,
                               const UINT8_T *input,
                               size_t ilen );

/**
* @brief This function finishes the sha1 operation, and writes
*                 the result to the output buffer.
*
* @param[in] ctx: The context to use. This must be initialized.
* @param[out] output:   The sha224 or sha1 checksum result.
*                 This must be a writable buffer of length \c 16 Bytes.
*
* @note This API is used to out sha1 result.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_sha1_finish_ret( TKL_HASH_HANDLE ctx,
                               UINT8_T output[16] );
/**
 * @brief          This function calculates the SHA-224 or SHA-256
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SHA-256 result is calculated as
 *                 output = SHA-256(input buffer).
 *
 * @param[in] input:    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * @param[in] ilen:     The length of the input data in Bytes.
 * @param[out] output:   The SHA-224 or SHA-256 checksum result. This must
 *                 be a writable buffer of length \c 32 Bytes.
 * @param[in] is224:    Determines which function to use. This must be
 *                 either \c 0 for SHA-256, or \c 1 for SHA-224.
 *
 * @return         OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_sha256_ret( const UINT8_T *input,
                        size_t ilen,
                        UINT8_T output[32],
                        INT32_T is224 );

/**
 * @brief          Output = MD5( input buffer )
 *
 * @param[in] input:    buffer holding the data
 * @param[in] ilen:     length of the input data
 * @param[out] output:   MD5 checksum result
 *
 * @return         OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
OPERATE_RET tal_md5_ret( const UINT8_T *input,
                        size_t ilen,
                        UINT8_T output[16]);

/**
 * @brief          Output = sha1( input buffer )
 *
 * @param[in] input:    buffer holding the data
 * @param[in] ilen:     length of the input data
 * @param[out] output:   sha1 checksum result
 *
 * @return         OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
OPERATE_RET tal_sha1_ret( const UINT8_T *input,
                        size_t ilen,
                        UINT8_T output[20]);
/**
* @brief This function Create&initializes a sha256 maccontext.
*
* @param[out] hmac_handle: sha256_mac handle
*
* @note This API is used to create and init sha256_mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_mac_create_init( tal_hash_mac_context_t *hmac_handle );
/**
* @brief This function clears a sha256 mac context.
*
* @param[in] hmac_handle: sha256 mac handle
*
* @note This API is used to release sha256.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_mac_free( tal_hash_mac_context_t *hmac_handle );

/**
* @brief This function starts a sha256 mac checksum
*                 calculation.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[in] key:    key
* @param[in] keylen: keylen
*
* @note This API is used to start sha256 mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_mac_starts( tal_hash_mac_context_t *hmac_handle, const UINT8_T *key, size_t keylen);
/**
* @brief This function feeds an input buffer into an ongoing
*                 sha256 mac checksum calculation.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[in] input:    The buffer holding the data. This must be a readable
*                 buffer of length \p ilen Bytes.
* @param[in] ilen:     The length of the input data in Bytes.
*
* @note This API is used to update sha256 mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_mac_update( tal_hash_mac_context_t *hmac_handle, const UINT8_T *input, size_t ilen );
/**
* @brief This function finishes the sha256 mac operation, and writes
*                 the result to the output buffer.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[out] output:   The sha256 mac checksum result.
*                 This must be a writable buffer of length \c 32 Bytes.
*
* @note This API is used to out sha256 mac result.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha256_mac_finish( tal_hash_mac_context_t *hmac_handle, UINT8_T *output );

/**
 * @brief          This function calculates the SHA-256 MAC
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SHA-256 result is calculated as
 *                 output = SHA-256 MAC.
 *
 * @param[in] key:    The buffer holding the data. This must be a readable
 *                 buffer of length \p keylen Bytes.
 * @param[in] keylen:     The length of the key data in Bytes.* 
 * @param[in] input:    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * @param[in] ilen:     The length of the input data in Bytes.
 * @param[out] output:   The SHA-256 MAC checksum result. This must
 *                 be a writable buffer of length \c 32 Bytes.
 *
 * @return         OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac( const UINT8_T *key, size_t keylen,
                     const UINT8_T *input, size_t ilen,
                     UINT8_T *output );


/**
* @brief This function Create&initializes a sha1 maccontext.
*
* @param[out] hmac_handle: sha1_mac handle
*
* @note This API is used to create and init sha1_mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_mac_create_init( tal_hash_mac_context_t *hmac_handle );
/**
* @brief This function clears a sha1 mac context.
*
* @param[in] hmac_handle: sha1 mac handle
*
* @note This API is used to release sha1.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_mac_free( tal_hash_mac_context_t *hmac_handle );

/**
* @brief This function starts a sha1 mac checksum
*                 calculation.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[in] key:    key
* @param[in] keylen: keylen
*
* @note This API is used to start sha1 mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_mac_starts( tal_hash_mac_context_t *hmac_handle, const UINT8_T *key, size_t keylen);
/**
* @brief This function feeds an input buffer into an ongoing
*                 sha1 mac checksum calculation.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[in] input:    The buffer holding the data. This must be a readable
*                 buffer of length \p ilen Bytes.
* @param[in] ilen:     The length of the input data in Bytes.
*
* @note This API is used to update sha1 mac.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_mac_update( tal_hash_mac_context_t *hmac_handle, const UINT8_T *input, size_t ilen );
/**
* @brief This function finishes the sha1 mac operation, and writes
*                 the result to the output buffer.
*
* @param[in] hmac_handle: The context to use. This must be initialized.
* @param[out] output:   The sha1 mac checksum result.
*                 This must be a writable buffer of length \c 32 Bytes.
*
* @note This API is used to out sha1 mac result.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_sha1_mac_finish( tal_hash_mac_context_t *hmac_handle, UINT8_T *output );

/**
 * @brief          This function calculates the SHA-256 MAC
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SHA-256 result is calculated as
 *                 output = SHA-256 MAC.
 *
 * @param[in] key:    The buffer holding the data. This must be a readable
 *                 buffer of length \p keylen Bytes.
 * @param[in] keylen:     The length of the key data in Bytes.* 
 * @param[in] input:    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * @param[in] ilen:     The length of the input data in Bytes.
 * @param[out] output:   The SHA-256 MAC checksum result. This must
 *                 be a writable buffer of length \c 32 Bytes.
 *
 * @return         OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac( const UINT8_T *key, size_t keylen,
                     const UINT8_T *input, size_t ilen,
                     UINT8_T *output );

OPERATE_RET tal_sha256_self_test( INT32_T verbose );

OPERATE_RET tal_md5_self_test( INT32_T verbose );

OPERATE_RET tal_sha1_self_test( INT32_T verbose );

OPERATE_RET tal_sha256_mac_self_test( INT32_T verbose );

OPERATE_RET tal_sha1_mac_self_test( INT32_T verbose );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

