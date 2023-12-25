#include "tuya_cloud_types.h"
#include "mbedtls/md.h"
#include "mbedtls/pkcs5.h"
#include "tal_log.h"


int pbkdf2_sha256(const char *passphrase, size_t passphrase_len, const char *salt, size_t salt_len, int iterations, uint32_t key_length, unsigned char *buf, size_t buflen)
{
    mbedtls_md_context_t sha_ctx;
    const mbedtls_md_info_t *info_sha;
    int ret;

    if (NULL == passphrase || NULL == salt) {
        return -1;
    }

    if (buflen < key_length) {
        return -1;
    }

    mbedtls_md_init( &sha_ctx );

    info_sha = mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 );
    if (info_sha == NULL) {
        ret = -1;
        goto exit;
    }

    if ((ret = mbedtls_md_setup( &sha_ctx, info_sha, 1 ) ) != 0) {
        ret = -1;
        goto exit;
    }

    ret = mbedtls_pkcs5_pbkdf2_hmac( &sha_ctx, (const unsigned char*) passphrase, passphrase_len , 
            (const unsigned char*) salt, salt_len, iterations, key_length, buf );
    if (ret != 0) {
        ret = -1;
        goto exit;
    }

exit:
    mbedtls_md_free( &sha_ctx );

    return ret;
}

int ap_pbkdf2_cacl(char  *pin, char *uuid, uint8_t *buf, uint8_t buflen)
{
    return pbkdf2_sha256(pin, strlen(pin), uuid, strlen(uuid), 1024, 37, (unsigned char *)buf, buflen);
}

