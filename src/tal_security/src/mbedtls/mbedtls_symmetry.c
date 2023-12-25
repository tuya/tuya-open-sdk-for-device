/**
 * @file mbedtls_symmetry.c
 * @brief This is aes implementation based on mbedtls
 * @version 1.0
 * @date 2022-04-22
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */


/***********************************************************************
 ** INCLUDE                                                           **
 **********************************************************************/
#include "tuya_iot_config.h"
#include "tkl_memory.h"
#include "tkl_symmetry.h"
#if !defined(ENABLE_PLATFORM_AES)
#include "mbedtls/aes.h"
#endif
/***********************************************************************
 ** CONSTANT ( MACRO AND ENUM )                                       **
 **********************************************************************/


/***********************************************************************
 ** STRUCT                                                            **
 **********************************************************************/


/***********************************************************************
 ** VARIABLE                                                          **
 **********************************************************************/


/***********************************************************************
 ** FUNCTON                                                           **
 **********************************************************************/


#if !defined(ENABLE_PLATFORM_AES)

/**
* @brief This function Create&initializes a aes context.
*
* @param[out] ctx: aes handle
*
* @note This API is used to create and init aes.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tkl_aes_create_init( TKL_SYMMETRY_HANDLE *ctx ){

    mbedtls_aes_context *mbedtls_aes_ctx;

    mbedtls_aes_ctx =(mbedtls_aes_context *) tkl_system_malloc(sizeof(mbedtls_aes_context));    
    mbedtls_aes_init(mbedtls_aes_ctx);
    *ctx = mbedtls_aes_ctx;

    return OPRT_OK;
}
/**
* @brief This function releases and clears the specified AES context.
*
* @param[in] ctx: The AES context to clear.
*
* @note This API is used to release aes.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET  tkl_aes_free( TKL_SYMMETRY_HANDLE ctx )
{
    if(ctx != NULL){
        mbedtls_aes_free( (mbedtls_aes_context *) ctx );
        tkl_system_free(ctx);
    }

    return OPRT_OK;
}
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
OPERATE_RET  tkl_aes_setkey_enc( TKL_SYMMETRY_HANDLE ctx, const UINT8_T *key,
                    UINT32_T keybits )
{
    if( mbedtls_aes_setkey_enc( (mbedtls_aes_context *)ctx, key,keybits ) != 0)
        return OPRT_COM_ERROR;

    return OPRT_OK;
}
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
OPERATE_RET  tkl_aes_setkey_dec( TKL_SYMMETRY_HANDLE ctx, const UINT8_T *key,
                    UINT32_T keybits )
{
    if( mbedtls_aes_setkey_dec( (mbedtls_aes_context *)ctx, key,keybits ) != 0)
        return OPRT_COM_ERROR;

    return OPRT_OK;
}
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
OPERATE_RET  tkl_aes_crypt_ecb( TKL_SYMMETRY_HANDLE ctx,
                    INT32_T mode,
                    size_t length,
                    const UINT8_T *input,
                    UINT8_T *output )
{
    UINT32_T i;

    if( (length % 16) != 0)
        return OPRT_INVALID_PARM;

    for(i = 0;i < length;i += 16){
        if( mbedtls_aes_crypt_ecb( (mbedtls_aes_context *)ctx,mode,input + i,output + i) != 0)
            return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
* @brief This function performs an AES-CBC encryption or decryption operation
*         on full blocks.
*
*         It performs the operation defined in the \p mode
*         parameter (encrypt/decrypt), on the input data buffer defined in
*         the \p input parameter.
*
*         It can be called as many times as needed, until all the input
*         data is processed. tkl_aes_init(), and either
*         tkl_aes_setkey_enc() or tkl_aes_setkey_dec() must be called
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
OPERATE_RET  tkl_aes_crypt_cbc( TKL_SYMMETRY_HANDLE ctx,
                    INT32_T mode,
                    size_t length,
                    UINT8_T iv[16],
                    const UINT8_T *input,
                    UINT8_T *output )
{
    if( mbedtls_aes_crypt_cbc( (mbedtls_aes_context *)ctx,mode,length,iv,input,output ) != 0)
        return OPRT_COM_ERROR;

    return OPRT_OK;
}

#endif