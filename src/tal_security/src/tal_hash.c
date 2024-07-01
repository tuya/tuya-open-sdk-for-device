/**
 * @file tal_hash.c
 * @brief This file provides the implementations of SHA1, SHA224, SHA256, and
 * MD5 hash functions. It includes functions for creating, initializing,
 * updating, and finalizing hash computations, as well as a function for
 * directly computing the hash of a given input buffer. These functions serve as
 * a thin layer over the underlying hash implementation, providing a simplified
 * interface for hash operations.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_iot_config.h"
#include "tkl_memory.h"
#include "tal_hash.h"
#include "tal_log.h"

/**
 * @brief This function Create&initializes a sha256 context.
 *
 * @param[out] ctx: sha256 handle
 *
 * @note This API is used to create and init sha256.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_create_init(TKL_HASH_HANDLE *ctx)
{
    return tkl_sha256_create_init(ctx);
}

/**
 * @brief This function clears a sha256 context.
 *
 * @param[in] ctx: sha256 handle
 *
 * @note This API is used to release sha256.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_free(TKL_HASH_HANDLE ctx)
{
    return tkl_sha256_free(ctx);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_starts_ret(TKL_HASH_HANDLE ctx, int32_t is224)
{
    return tkl_sha256_starts_ret(ctx, is224);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_update_ret(TKL_HASH_HANDLE ctx, const uint8_t *input, size_t ilen)
{
    return tkl_sha256_update_ret(ctx, input, ilen);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_finish_ret(TKL_HASH_HANDLE ctx, uint8_t output[32])
{
    return tkl_sha256_finish_ret(ctx, output);
}

/**
 * @brief This function Create&initializes a md5 context.
 *
 * @param[out] ctx: md5 handle
 *
 * @note This API is used to create and init md5.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_create_init(TKL_HASH_HANDLE *ctx)
{
    return tkl_md5_create_init(ctx);
}

/**
 * @brief This function clears a md5 context.
 *
 * @param[in] ctx: md5 handle
 *
 * @note This API is used to release md5.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_free(TKL_HASH_HANDLE ctx)
{
    return tkl_md5_free(ctx);
}

/**
 * @brief This function starts a md5 checksum
 *                 calculation.
 *
 * @param[in] ctx: The context to use. This must be initialized.
 *
 * @note This API is used to start md5.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_starts_ret(TKL_HASH_HANDLE ctx)
{
    return tkl_md5_starts_ret(ctx);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_update_ret(TKL_HASH_HANDLE ctx, const uint8_t *input, size_t ilen)
{
    return tkl_md5_update_ret(ctx, input, ilen);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_finish_ret(TKL_HASH_HANDLE ctx, uint8_t output[16])
{
    return tkl_md5_finish_ret(ctx, output);
}

/**
 * @brief This function Create&initializes a sha1 context.
 *
 * @param[out] ctx: sha1 handle
 *
 * @note This API is used to create and init sha1.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_create_init(TKL_HASH_HANDLE *ctx)
{
    return tkl_sha1_create_init(ctx);
}

/**
 * @brief This function clears a sha1 context.
 *
 * @param[in] ctx: sha1 handle
 *
 * @note This API is used to release sha1.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_free(TKL_HASH_HANDLE ctx)
{
    return tkl_sha1_free(ctx);
}

/**
 * @brief This function starts a sha1 checksum
 *                 calculation.
 *
 * @param[in] ctx: The context to use. This must be initialized.
 *
 * @note This API is used to start sha1.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_starts_ret(TKL_HASH_HANDLE ctx)
{
    return tkl_sha1_starts_ret(ctx);
}

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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_update_ret(TKL_HASH_HANDLE ctx, const uint8_t *input, size_t ilen)
{
    return tkl_sha1_update_ret(ctx, input, ilen);
}

/**
 * @brief This function finishes the sha1 operation, and writes
 *                 the result to the output buffer.
 *
 * @param[in] ctx: The context to use. This must be initialized.
 * @param[out] output:   The sha224 or sha1 checksum result.
 *                 This must be a writable buffer of length \c 20 Bytes.
 *
 * @note This API is used to out sha1 result.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_finish_ret(TKL_HASH_HANDLE ctx, uint8_t output[20])
{
    return tkl_sha1_finish_ret(ctx, output);
}

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
 * @return         OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_ret(const uint8_t *input, size_t ilen, uint8_t output[32], int32_t is224)
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    TKL_HASH_HANDLE ctx = NULL;

    if ((ret = tal_sha256_create_init(&ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_starts_ret(ctx, is224)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_update_ret(ctx, input, ilen)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_finish_ret(ctx, output)) != OPRT_OK) {
        goto exit;
    }

exit:
    if (ctx) {
        ret |= tal_sha256_free(ctx);
    }

    return (ret);
}
/**
 * @brief          This function calculates the MD5
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The MD5 result is calculated as
 *                 output = MD5(input buffer).
 *
 * @param[in] input:    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * @param[in] ilen:     The length of the input data in Bytes.
 * @param[out] output:   The SHA-224 or MD5 checksum result. This must
 *                 be a writable buffer of length \c 16 Bytes.
 *
 * @return         OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_md5_ret(const uint8_t *input, size_t ilen, uint8_t output[16])
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    TKL_HASH_HANDLE ctx = NULL;

    if ((ret = tal_md5_create_init(&ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_md5_starts_ret(ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_md5_update_ret(ctx, input, ilen)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_md5_finish_ret(ctx, output)) != OPRT_OK) {
        goto exit;
    }

exit:
    if (ctx) {
        ret |= tal_md5_free(ctx);
    }

    return (ret);
}

/**
 * @brief          This function calculates the sha1
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The sha1 result is calculated as
 *                 output = sha1(input buffer).
 *
 * @param[in] input:    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * @param[in] ilen:     The length of the input data in Bytes.
 * @param[out] output:   The SHA-224 or sha1 checksum result. This must
 *                 be a writable buffer of length \c 20 Bytes.
 *
 * @return         OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_ret(const uint8_t *input, size_t ilen, uint8_t output[20])
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    TKL_HASH_HANDLE ctx = NULL;

    if ((ret = tal_sha1_create_init(&ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_starts_ret(ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_update_ret(ctx, input, ilen)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_finish_ret(ctx, output)) != OPRT_OK) {
        goto exit;
    }

exit:
    if (ctx) {
        ret |= tal_sha1_free(ctx);
    }

    return (ret);
}

/**
 * @brief This function Create&initializes a sha256 maccontext.
 *
 * @param[out] hmac_handle: sha256_mac handle
 *
 * @note This API is used to create and init sha256_mac.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac_create_init(tal_hash_mac_context_t *hmac_handle)
{
    if (hmac_handle == NULL) {
        return OPRT_INVALID_PARM;
    }

    memset(hmac_handle, 0, sizeof(tal_hash_mac_context_t));
    return tal_sha256_create_init(&(hmac_handle->ctx));
}
/**
 * @brief This function clears a sha256 mac context.
 *
 * @param[in] hmac_handle: sha256 mac handle
 *
 * @note This API is used to release sha256.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac_free(tal_hash_mac_context_t *hmac_handle)
{
    OPERATE_RET ret = OPRT_OK;

    if (hmac_handle != NULL) {
        ret = tal_sha256_free(hmac_handle->ctx);
        memset(hmac_handle, 0, sizeof(tal_hash_mac_context_t));
    }

    return ret;
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac_starts(tal_hash_mac_context_t *hmac_handle, const uint8_t *key, size_t keylen)
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    uint8_t sum[32];
    uint8_t *ipad, *opad;
    size_t i;
    TKL_HASH_HANDLE ctx;

    ctx = hmac_handle->ctx;

    if (keylen > 64) {
        if ((ret = tal_sha256_ret(key, keylen, sum, 0)) != OPRT_OK) {
            goto cleanup;
        }
        keylen = 32;
        key = sum;
    }

    ipad = hmac_handle->ipad;
    opad = hmac_handle->opad;

    memset(ipad, 0x36, sizeof(hmac_handle->ipad));
    memset(opad, 0x5C, sizeof(hmac_handle->opad));

    for (i = 0; i < keylen; i++) {
        ipad[i] = (uint8_t)(ipad[i] ^ key[i]);
        opad[i] = (uint8_t)(opad[i] ^ key[i]);
    }

    if ((ret = tal_sha256_starts_ret(ctx, 0)) != OPRT_OK) {
        goto cleanup;
    }

    if ((ret = tal_sha256_update_ret(ctx, ipad, 64)) != OPRT_OK) {
        goto cleanup;
    }

cleanup:
    memset(sum, 0, sizeof(sum));

    return (ret);
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac_update(tal_hash_mac_context_t *hmac_handle, const uint8_t *input, size_t ilen)
{
    return tal_sha256_update_ret(hmac_handle->ctx, input, ilen);
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac_finish(tal_hash_mac_context_t *hmac_handle, uint8_t *output)
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    uint8_t tmp[32];
    uint8_t *opad;
    TKL_HASH_HANDLE ctx = NULL;

    ctx = hmac_handle->ctx;
    opad = hmac_handle->opad;

    if ((ret = tal_sha256_finish_ret(hmac_handle->ctx, tmp)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_starts_ret(ctx, 0)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_update_ret(ctx, opad, 64)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_update_ret(ctx, tmp, 32)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha256_finish_ret(hmac_handle->ctx, output)) != OPRT_OK) {
        goto exit;
    }

    memset(tmp, 0, sizeof(tmp));

exit:

    return ret;
}
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
 * @return         OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha256_mac(const uint8_t *key, size_t keylen, const uint8_t *input, size_t ilen, uint8_t *output)
{
    tal_hash_mac_context_t hmac_handle;
    OPERATE_RET ret = OPRT_COM_ERROR;

    if ((ret = tal_sha256_mac_create_init(&hmac_handle)) != OPRT_OK) {
        goto cleanup;
    }

    if ((ret = tal_sha256_mac_starts(&hmac_handle, key, keylen)) != OPRT_OK) {
        goto cleanup;
    }
    if ((ret = tal_sha256_mac_update(&hmac_handle, input, ilen)) != OPRT_OK) {
        goto cleanup;
    }
    if ((ret = tal_sha256_mac_finish(&hmac_handle, output)) != OPRT_OK) {
        goto cleanup;
    }

cleanup:
    ret |= tal_sha256_mac_free(&hmac_handle);

    return (ret);
}

/**
 * @brief This function Create&initializes a sha1 maccontext.
 *
 * @param[out] hmac_handle: sha1_mac handle
 *
 * @note This API is used to create and init sha1_mac.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac_create_init(tal_hash_mac_context_t *hmac_handle)
{
    if (hmac_handle == NULL) {
        return OPRT_INVALID_PARM;
    }

    memset(hmac_handle, 0, sizeof(tal_hash_mac_context_t));
    return tal_sha1_create_init(&(hmac_handle->ctx));
}
/**
 * @brief This function clears a sha1 mac context.
 *
 * @param[in] hmac_handle: sha1 mac handle
 *
 * @note This API is used to release sha1.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac_free(tal_hash_mac_context_t *hmac_handle)
{
    OPERATE_RET ret = OPRT_OK;

    if (hmac_handle != NULL) {
        ret = tal_sha1_free(hmac_handle->ctx);
        memset(hmac_handle, 0, sizeof(tal_hash_mac_context_t));
    }

    return ret;
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac_starts(tal_hash_mac_context_t *hmac_handle, const uint8_t *key, size_t keylen)
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    uint8_t sum[20];
    uint8_t *ipad, *opad;
    size_t i;
    TKL_HASH_HANDLE ctx;

    ctx = hmac_handle->ctx;

    if (keylen > 64) {
        if ((ret = tal_sha1_ret(key, keylen, sum)) != OPRT_OK) {
            goto cleanup;
        }
        keylen = 20;
        key = sum;
    }

    ipad = hmac_handle->ipad;
    opad = hmac_handle->opad;

    memset(ipad, 0x36, sizeof(hmac_handle->ipad));
    memset(opad, 0x5C, sizeof(hmac_handle->opad));

    for (i = 0; i < keylen; i++) {
        ipad[i] = (uint8_t)(ipad[i] ^ key[i]);
        opad[i] = (uint8_t)(opad[i] ^ key[i]);
    }

    if ((ret = tal_sha1_starts_ret(ctx)) != OPRT_OK) {
        goto cleanup;
    }

    if ((ret = tal_sha1_update_ret(ctx, ipad, 64)) != OPRT_OK) {
        goto cleanup;
    }

cleanup:
    memset(sum, 0, sizeof(sum));

    return (ret);
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac_update(tal_hash_mac_context_t *hmac_handle, const uint8_t *input, size_t ilen)
{
    return tal_sha1_update_ret(hmac_handle->ctx, input, ilen);
}
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
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac_finish(tal_hash_mac_context_t *hmac_handle, uint8_t *output)
{
    OPERATE_RET ret = OPRT_COM_ERROR;
    uint8_t tmp[20];
    uint8_t *opad;
    TKL_HASH_HANDLE ctx = NULL;

    ctx = hmac_handle->ctx;
    opad = hmac_handle->opad;

    if ((ret = tal_sha1_finish_ret(hmac_handle->ctx, tmp)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_starts_ret(ctx)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_update_ret(ctx, opad, 64)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_update_ret(ctx, tmp, 20)) != OPRT_OK) {
        goto exit;
    }

    if ((ret = tal_sha1_finish_ret(hmac_handle->ctx, output)) != OPRT_OK) {
        goto exit;
    }

    memset(tmp, 0, sizeof(tmp));

exit:

    return ret;
}
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
 * @return         OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_sha1_mac(const uint8_t *key, size_t keylen, const uint8_t *input, size_t ilen, uint8_t *output)
{
    tal_hash_mac_context_t hmac_handle;
    OPERATE_RET ret = OPRT_COM_ERROR;

    if ((ret = tal_sha1_mac_create_init(&hmac_handle)) != OPRT_OK) {
        goto cleanup;
    }

    if ((ret = tal_sha1_mac_starts(&hmac_handle, key, keylen)) != OPRT_OK) {
        goto cleanup;
    }
    if ((ret = tal_sha1_mac_update(&hmac_handle, input, ilen)) != OPRT_OK) {
        goto cleanup;
    }
    if ((ret = tal_sha1_mac_finish(&hmac_handle, output)) != OPRT_OK) {
        goto cleanup;
    }

cleanup:
    ret |= tal_sha1_mac_free(&hmac_handle);

    return (ret);
}
#if defined(ENABLE_TAL_SECURITY_SELF_TEST)
/*
 * FIPS-180-2 test vectors
 */
static const uint8_t sha256_test_buf[3][57] = {
    {"abc"}, {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}, {""}};

static const size_t sha256_test_buflen[3] = {3, 56, 1000};

static const uint8_t sha256_test_sum[6][32] = {
    /*
     * SHA-224 test vectors
     */
    {0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22, 0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2,
     0x55, 0xB3, 0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7, 0xE3, 0x6C, 0x9D, 0xA7},
    {0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC, 0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89,
     0x01, 0x50, 0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19, 0x52, 0x52, 0x25, 0x25},
    {0x20, 0x79, 0x46, 0x55, 0x98, 0x0C, 0x91, 0xD8, 0xBB, 0xB4, 0xC1, 0xEA, 0x97, 0x61,
     0x8A, 0x4B, 0xF0, 0x3F, 0x42, 0x58, 0x19, 0x48, 0xB2, 0xEE, 0x4E, 0xE7, 0xAD, 0x67},

    /*
     * SHA-256 test vectors
     */
    {0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
     0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD},
    {0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
     0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1},
    {0xCD, 0xC7, 0x6E, 0x5C, 0x99, 0x14, 0xFB, 0x92, 0x81, 0xA1, 0xC7, 0xE2, 0x84, 0xD7, 0x3E, 0x67,
     0xF1, 0x80, 0x9A, 0x48, 0xA4, 0x97, 0x20, 0x0E, 0x04, 0x6D, 0x39, 0xCC, 0xC7, 0x11, 0x2C, 0xD0}};

/*
 * Checkup routine
 */
OPERATE_RET tal_sha256_self_test(int32_t verbose)
{
    OPERATE_RET ret = OPRT_OK;
    int32_t i, j, k, buflen;
    uint8_t *buf;
    uint8_t sha256sum[32];
    TKL_HASH_HANDLE ctx = NULL;

    buf = tal_malloc(1024);
    if (NULL == buf) {
        if (verbose != 0) {
            PR_DEBUG("Buffer allocation failed\n");
        }

        return (1);
    }

    tal_sha256_create_init(&ctx);

    for (i = 0; i < 6; i++) {
        j = i % 3;
        k = i < 3;

        if (verbose != 0) {
            PR_DEBUG("  SHA-%d test #%d: ", 256 - k * 32, j + 1);
        }

        if ((ret = tal_sha256_starts_ret(ctx, k)) != 0) {
            goto fail;
        }

        if (j == 2) {
            memset(buf, 'a', buflen = 1000);

            for (j = 0; j < 1000; j++) {
                ret = tal_sha256_update_ret(ctx, buf, buflen);
                if (ret != 0) {
                    goto fail;
                }
            }

        } else {
            ret = tal_sha256_update_ret(ctx, sha256_test_buf[j], sha256_test_buflen[j]);
            if (ret != 0) {
                goto fail;
            }
        }

        if ((ret = tal_sha256_finish_ret(ctx, sha256sum)) != 0) {
            goto fail;
        }

        if (memcmp(sha256sum, sha256_test_sum[i], 32 - k * 4) != 0) {
            ret = 1;
            goto fail;
        }

        if (verbose != 0) {
            PR_DEBUG("passed\n");
        }
    }

    if (verbose != 0) {
        PR_DEBUG("\n");
    }

    goto exit;

fail:
    if (verbose != 0) {
        PR_DEBUG("failed\n");
    }

exit:
    tal_sha256_free(ctx);
    tal_free(buf);

    return (ret);
}

/*
 * RFC 1321 test vectors
 */
static const uint8_t md5_test_buf[7][81] = {{""},
                                            {"a"},
                                            {"abc"},
                                            {"message digest"},
                                            {"abcdefghijklmnopqrstuvwxyz"},
                                            {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"},
                                            {"1234567890123456789012345678901234567890123456789012345678901234567890123"
                                             "4567890"}};

static const size_t md5_test_buflen[7] = {0, 1, 3, 14, 26, 62, 80};

static const uint8_t md5_test_sum[7][16] = {
    {0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E},
    {0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61},
    {0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72},
    {0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0},
    {0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B},
    {0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F},
    {0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A}};

/*
 * Checkup routine
 */
OPERATE_RET tal_md5_self_test(int32_t verbose)
{
    int32_t i;
    OPERATE_RET ret = OPRT_OK;
    uint8_t md5sum[16];

    for (i = 0; i < 7; i++) {
        if (verbose != 0) {
            PR_DEBUG("  MD5 test #%d: ", i + 1);
        }

        ret = tal_md5_ret(md5_test_buf[i], md5_test_buflen[i], md5sum);
        if (ret != 0) {
            goto fail;
        }

        if (memcmp(md5sum, md5_test_sum[i], 16) != 0) {
            ret = 1;
            goto fail;
        }

        if (verbose != 0) {
            PR_DEBUG("passed\n");
        }
    }

    if (verbose != 0) {
        PR_DEBUG("\n");
    }

    goto exit;

fail:
    if (verbose != 0) {
        PR_DEBUG("failed\n");
    }

exit:
    return (ret);
}

/*
 * FIPS-180-1 test vectors
 */
static const uint8_t sha1_test_buf[3][57] = {
    {"abc"}, {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}, {""}};

static const size_t sha1_test_buflen[3] = {3, 56, 1000};

static const uint8_t sha1_test_sum[3][20] = {{0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
                                              0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D},
                                             {0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
                                              0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1},
                                             {0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E,
                                              0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F}};

/*
 * Checkup routine
 */
OPERATE_RET tal_sha1_self_test(int verbose)
{
    int32_t i, j, buflen, ret = 0;
    uint8_t buf[1024];
    uint8_t sha1sum[20];
    TKL_HASH_HANDLE ctx = NULL;

    tal_sha1_create_init(&ctx);
    /*
     * SHA-1
     */
    for (i = 0; i < 3; i++) {
        if (verbose != 0) {
            PR_DEBUG("  SHA-1 test #%d: ", i + 1);
        }

        if ((ret = tal_sha1_starts_ret(ctx)) != 0) {
            goto fail;
        }

        if (i == 2) {
            memset(buf, 'a', buflen = 1000);

            for (j = 0; j < 1000; j++) {
                ret = tal_sha1_update_ret(ctx, buf, buflen);
                if (ret != 0) {
                    goto fail;
                }
            }
        } else {
            ret = tal_sha1_update_ret(ctx, sha1_test_buf[i], sha1_test_buflen[i]);
            if (ret != 0) {
                goto fail;
            }
        }

        if ((ret = tal_sha1_finish_ret(ctx, sha1sum)) != 0) {
            goto fail;
        }

        if (memcmp(sha1sum, sha1_test_sum[i], 20) != 0) {
            ret = 1;
            goto fail;
        }

        if (verbose != 0) {
            PR_DEBUG("passed\n");
        }
    }

    if (verbose != 0) {
        PR_DEBUG("\n");
    }

    goto exit;

fail:
    if (verbose != 0) {
        PR_DEBUG("failed\n");
    }

exit:
    tal_sha1_free(ctx);

    return (ret);
}

/*
 * RFC 4231 test vectors
 */
static const uint8_t sha256_mac_test_key[7][131] = {
    {0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
     0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b},
    {"Jefe"},
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
    {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
     0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19},
    {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
     0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c},
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}};

static const size_t sha256_mac_test_keylen[7] = {20, 4, 20, 25, 20, 131, 131};

static const uint8_t sha256_mac_test_buf[7][152] = {
    {"Hi There"},
    {"what do ya want for nothing?"},
    {0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
     0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
     0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd},
    {0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
     0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
     0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd},
    {"Test With Truncation"},
    {"Test Using Larger Than Block-Size Key - Hash Key First"},
    {"This is a test using a larger than block-size key and a larger than "
     "block-size data. The key needs to be hashed before being used by the "
     "HMAC algorithm."}};

static const size_t sha256_mac_test_buflen[7] = {8, 28, 50, 50, 20, 54, 152};

static const uint8_t sha256_mac_test_sum[7][32] = {
    {0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b,
     0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7},
    {0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7,
     0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83, 0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43},
    {0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46, 0x85, 0x4d, 0xb8, 0xeb, 0xd0, 0x91, 0x81, 0xa7,
     0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22, 0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe},
    {0x82, 0x55, 0x8a, 0x38, 0x9a, 0x44, 0x3c, 0x0e, 0xa4, 0xcc, 0x81, 0x98, 0x99, 0xf2, 0x08, 0x3a,
     0x85, 0xf0, 0xfa, 0xa3, 0xe5, 0x78, 0xf8, 0x07, 0x7a, 0x2e, 0x3f, 0xf4, 0x67, 0x29, 0x66, 0x5b},
    {0xa3, 0xb6, 0x16, 0x74, 0x73, 0x10, 0x0e, 0xe0, 0x6e, 0x0c, 0x79, 0x6c, 0x29, 0x55, 0x55, 0x2b},
    {0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f,
     0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14, 0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54},
    {0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb, 0x27, 0x63, 0x5f, 0xbc, 0xd5, 0xb0, 0xe9, 0x44,
     0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93, 0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2}};

/*
 * Checkup routine
 */
OPERATE_RET tal_sha256_mac_self_test(int32_t verbose)
{
    int32_t i;
    OPERATE_RET ret = OPRT_OK;
    uint8_t sha256_mac[32];
    uint32_t len;

    for (i = 0; i < 7; i++) {
        if (verbose != 0) {
            PR_DEBUG("  SHA256 MAC test #%d: ", i + 1);
        }

        ret = tal_sha256_mac(sha256_mac_test_key[i], sha256_mac_test_keylen[i], sha256_mac_test_buf[i],
                             sha256_mac_test_buflen[i], sha256_mac);
        if (ret != 0) {
            goto fail;
        }

        len = 32;
        if (i == 4) {
            len = 16;
        }
        if (memcmp(sha256_mac, sha256_mac_test_sum[i], len) != 0) {
            ret = 1;
            goto fail;
        }

        if (verbose != 0) {
            PR_DEBUG("passed\n");
        }
    }

    if (verbose != 0) {
        PR_DEBUG("\n");
    }

    goto exit;

fail:
    if (verbose != 0) {
        PR_DEBUG("failed\n");
    }

exit:
    return (ret);
}

/*
 * RFC 2202 test vectors
 */
static const uint8_t sha1_mac_test_key[7][131] = {
    {0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
     0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b},
    {"Jefe"},
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
    {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
     0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19},
    {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
     0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c},
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
     0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
    {
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    }};

static const size_t sha1_mac_test_keylen[7] = {20, 4, 20, 25, 20, 80, 80};

static const uint8_t sha1_mac_test_buf[7][152] = {
    {"Hi There"},
    {"what do ya want for nothing?"},
    {0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
     0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
     0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd},
    {0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
     0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
     0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd},
    {"Test With Truncation"},
    {"Test Using Larger Than Block-Size Key - Hash Key First"},
    {"Test Using Larger Than Block-Size Key and Larger Than One Block-Size "
     "Data"}};

static const size_t sha1_mac_test_buflen[7] = {8, 28, 50, 50, 20, 54, 73};

static const uint8_t sha1_mac_test_sum[7][20] = {{0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64, 0xe2, 0x8b,
                                                  0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e, 0xf1, 0x46, 0xbe, 0x00},
                                                 {0xef, 0xfc, 0xdf, 0x6a, 0xe5, 0xeb, 0x2f, 0xa2, 0xd2, 0x74,
                                                  0x16, 0xd5, 0xf1, 0x84, 0xdf, 0x9c, 0x25, 0x9a, 0x7c, 0x79},
                                                 {0x12, 0x5d, 0x73, 0x42, 0xb9, 0xac, 0x11, 0xcd, 0x91, 0xa3,
                                                  0x9a, 0xf4, 0x8a, 0xa1, 0x7b, 0x4f, 0x63, 0xf1, 0x75, 0xd3},
                                                 {0x4c, 0x90, 0x07, 0xf4, 0x02, 0x62, 0x50, 0xc6, 0xbc, 0x84,
                                                  0x14, 0xf9, 0xbf, 0x50, 0xc8, 0x6c, 0x2d, 0x72, 0x35, 0xda},
                                                 {0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2,
                                                  0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04},
                                                 {0xaa, 0x4a, 0xe5, 0xe1, 0x52, 0x72, 0xd0, 0x0e, 0x95, 0x70,
                                                  0x56, 0x37, 0xce, 0x8a, 0x3b, 0x55, 0xed, 0x40, 0x21, 0x12},
                                                 {0xe8, 0xe9, 0x9d, 0x0f, 0x45, 0x23, 0x7d, 0x78, 0x6d, 0x6b,
                                                  0xba, 0xa7, 0x96, 0x5c, 0x78, 0x08, 0xbb, 0xff, 0x1a, 0x91}};

/*
 * Checkup routine
 */
OPERATE_RET tal_sha1_mac_self_test(int32_t verbose)
{
    int32_t i;
    OPERATE_RET ret = OPRT_OK;
    uint8_t sha1_mac[20];
    uint32_t len;

    for (i = 0; i < 7; i++) {
        if (verbose != 0) {
            PR_DEBUG("  sha1 MAC test #%d: ", i + 1);
        }

        ret = tal_sha1_mac(sha1_mac_test_key[i], sha1_mac_test_keylen[i], sha1_mac_test_buf[i], sha1_mac_test_buflen[i],
                           sha1_mac);
        if (ret != 0) {
            goto fail;
        }

        len = 20;
        if (memcmp(sha1_mac, sha1_mac_test_sum[i], len) != 0) {
            PR_DEBUG("%d ,%d\r\n", sha1_mac_test_keylen[i], sha1_mac_test_buflen[i]);
            PR_DEBUG("0x%02x ,0x%02x\r\n", sha1_mac[0], sha1_mac[19]);
            ret = 1;
            goto fail;
        }

        if (verbose != 0) {
            PR_DEBUG("passed\n");
        }
    }

    if (verbose != 0) {
        PR_DEBUG("\n");
    }

    goto exit;

fail:
    if (verbose != 0) {
        PR_DEBUG("failed\n");
    }

exit:
    return (ret);
}
#endif