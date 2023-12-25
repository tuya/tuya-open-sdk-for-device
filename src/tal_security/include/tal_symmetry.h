/**
* @file tal_symmetry.h
* @brief Common process - adapter the symmetry api provide by OS
* @version 0.1
* @date 2022-04-22
*
* @copyright Copyright 2021-2025 Tuya Inc. All Rights Reserved.
*
*/
 
#ifndef __TAL_SYMMETRY_H__
#define __TAL_SYMMETRY_H__

#include "tkl_symmetry.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{    
    SYMMETRY_DECRYPT = 0,
    SYMMETRY_ENCRYPT = 1,
}TAL_SYMMETRY_CRYPT_MODE;

/**
* @brief This function Create&initializes a aes context.
*
* @param[out] ctx: aes handle
*
* @note This API is used to create and init aes.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_create_init( TKL_SYMMETRY_HANDLE *ctx );

/**
* @brief This function releases and clears the specified AES context.
*
* @param[in] ctx: The AES context to clear.
*
* @note This API is used to release aes.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_free( TKL_SYMMETRY_HANDLE ctx );

/**
* @brief This function sets the encryption key.
*
* @param[in] ctx: The AES context to which the key should be bound.
 *                 It must be initialized.
* @param[in] key:  The encryption key..
* @param[in] keybits:  The size of data passed in bits. Valid options are:
*                 <ul><li>128 bits</li>
*                 <li>192 bits</li>
*                 <li>256 bits</li></ul>
*
* @note This API is used to set aes key.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_setkey_enc( TKL_SYMMETRY_HANDLE ctx, UINT8_T *key,
                    UINT32_T keybits );

/**
* @brief This function sets the decryption key.
*
* @param[in] ctx: The AES context to which the key should be bound.
 *                 It must be initialized.
* @param[in] key:  The decryption key..
* @param[in] keybits:  The size of data passed in bits. Valid options are:
*                 <ul><li>128 bits</li>
*                 <li>192 bits</li>
*                 <li>256 bits</li></ul>
*
* @note This API is used to set aes key.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_setkey_dec( TKL_SYMMETRY_HANDLE ctx, UINT8_T *key,
                    UINT32_T keybits );

/**
* @brief This function performs an AES encryption or decryption operation.
*
* @param[in] ctx:  The AES context to use for encryption or decryption.
 *                 It must be initialized and bound to a key.
* @param[in] mode     The AES operation: 
* @param[in] length   The length of the input data in Bytes. This must be a
*                 multiple of the block size (\c 16 Bytes).
* @param[in] input    The buffer holding the input data.
*                 It must be readable and of size \p length Bytes.
* @param[in] output   The buffer where the output data will be written.
*                 It must be writeable and of size \p length Bytes.
* @note This function operates on full blocks, that is, the input size
*         must be a multiple of the AES block size of \c 16 Bytes.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_crypt_ecb( TKL_SYMMETRY_HANDLE ctx,
                    INT32_T mode,
                    size_t length,
                    UINT8_T *input,
                    UINT8_T *output );

/**
* @brief This function performs an AES-CBC encryption or decryption operation
*         on full blocks.
*
*         It performs the operation defined in the \p mode
*         parameter (encrypt/decrypt), on the input data buffer defined in
*         the \p input parameter.
*
*         It can be called as many times as needed, until all the input
*         data is processed. tal_aes_init(), and either
*         tal_aes_setkey_enc() or tal_aes_setkey_dec() must be called
*         before the first call to this API with the same context.
* 
* @param[in] ctx:  The AES context to use for encryption or decryption.
 *                 It must be initialized and bound to a key.
* @param[in] mode     The AES operation: 
* @param[in] length   The length of the input data in Bytes. This must be a
*                 multiple of the block size (\c 16 Bytes).
* @param[in] iv       Initialization vector (updated after use).
*                 It must be a readable and writeable buffer of \c 16 Bytes.
* @param[in] input    The buffer holding the input data.
*                 It must be readable and of size \p length Bytes.
* @param[in] output   The buffer where the output data will be written.
*                 It must be writeable and of size \p length Bytes.
*
* @note This function operates on full blocks, that is, the input size
*         must be a multiple of the AES block size of \c 16 Bytes.
* @note Upon exit, the content of the IV is updated so that you can
*         call the same function again on the next
*         block(s) of data and get the same result as if it was
*         encrypted in one call. This allows a "streaming" usage.
*         If you need to retain the contents of the IV, you should
*         either save it manually or use the cipher module instead.
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tal_aes_crypt_cbc( TKL_SYMMETRY_HANDLE ctx,
                    INT32_T mode,
                    size_t length,
                    UINT8_T iv[16],
                    UINT8_T *input,
                    UINT8_T *output );



OPERATE_RET  tal_aes_crypt_ctr(TKL_SYMMETRY_HANDLE ctx,
                    size_t length,                    
                    size_t * nc_off, 
                    UINT8_T nonce_counter[16],
                    UINT8_T stream_block [16],
                    UINT8_T *input,
                    UINT8_T *output );

OPERATE_RET  tal_aes128_ecb_encode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *ec_data,
                    UINT8_T *key );

OPERATE_RET  tal_aes128_ecb_decode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *dec_data,
                    UINT8_T *key );

OPERATE_RET  tal_aes128_cbc_encode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *key,
                    UINT8_T *iv,
                    UINT8_T *ec_data );

OPERATE_RET  tal_aes128_cbc_decode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *key,
                    UINT8_T *iv,
                    UINT8_T *dec_data );

OPERATE_RET  tal_aes256_cbc_encode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *key,
                    UINT8_T *iv,
                    UINT8_T *ec_data );

OPERATE_RET  tal_aes256_cbc_decode_raw(UINT8_T *data,                    
                    size_t len,
                    UINT8_T *key,
                    UINT8_T *iv,
                    UINT8_T *dec_data );

OPERATE_RET  tal_aes256_ctr_raw(UINT8_T *input,                    
                    size_t len, 
                    UINT8_T *key,
                    size_t * nc_off, 
                    UINT8_T nonce_counter[16],
                    UINT8_T stream_block [16],                    
                    UINT8_T *output );

UINT32_T tal_pkcs7padding_buffer(UINT8_T *p_buffer, UINT32_T length);

INT32_T tal_aes_get_actual_length(IN UINT8_T *dec_data, IN UINT32_T dec_data_len);

OPERATE_RET tal_aes128_ecb_encode(IN UINT8_T *data, IN UINT32_T len, \
                              OUT UINT8_T **ec_data, OUT UINT32_T *ec_len, \
                              IN UINT8_T *key);

OPERATE_RET tal_aes128_ecb_decode(IN UINT8_T *data, IN UINT32_T len, \
                              OUT UINT8_T **dec_data, OUT UINT32_T *dec_len, \
                              IN UINT8_T *key);

OPERATE_RET tal_aes128_cbc_encode(IN UINT8_T *data, IN UINT32_T len, \
                              IN UINT8_T *key, IN UINT8_T *iv, \
                              OUT UINT8_T **ec_data, OUT UINT32_T *ec_len);

OPERATE_RET tal_aes128_cbc_decode(IN UINT8_T *data, IN UINT32_T len, \
                              IN UINT8_T *key, IN UINT8_T *iv, \
                              OUT UINT8_T **dec_data, OUT UINT32_T *dec_len);

OPERATE_RET tal_aes_free_data(IN UINT8_T *data);

OPERATE_RET tal_aes_self_test( INT32_T verbose );

#ifdef __cplusplus
} // extern "C"
#endif

#endif

