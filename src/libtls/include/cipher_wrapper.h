#ifndef __CRYPTO_WRAPPER_H_
#define __CRYPTO_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "mbedtls/platform.h"
#include "mbedtls/cipher.h"
#include "mbedtls/md.h"

typedef struct {
    unsigned char *key;
    unsigned char *nonce;
    unsigned char *ad;
    unsigned char *data;
    size_t key_len;
    size_t nonce_len;
    size_t ad_len;
    size_t data_len;
    mbedtls_cipher_type_t cipher_type;
} cipher_params_t;

int mbedtls_cipher_auth_encrypt_wrapper(const cipher_params_t *input, unsigned char *output, size_t *olen,
                                        unsigned char *tag, size_t tag_len);

int mbedtls_cipher_auth_decrypt_wrapper(const cipher_params_t *input, unsigned char *output, size_t *olen,
                                        unsigned char *tag, size_t tag_len);

int mbedtls_message_digest(mbedtls_md_type_t md_type, const uint8_t *input, size_t ilen, uint8_t *digest);

int mbedtls_message_digest_hmac(mbedtls_md_type_t md_type, const uint8_t *key, size_t keylen, const uint8_t *input,
                                size_t ilen, uint8_t *digest);

#ifdef __cplusplus
}
#endif
#endif