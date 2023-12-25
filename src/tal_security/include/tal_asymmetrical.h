/**
* @file tal_asymmetrical.h
* @brief Common process - adapter the asymmetrical api  provide by OS
* @version 0.1
* @date 2022-04-22
*
* @copyright Copyright 2021-2025 Tuya Inc. All Rights Reserved.
*
*/

#ifndef __TAL_ASYMMETRICAL_H__
#define __TAL_ASYMMETRICAL_H__

#include "tkl_asymmetrical.h"

#ifdef __cplusplus
    extern "C" {
#endif

#include "tuya_iot_config.h"
#include "tuya_tls.h"
#include "mbedtls/rsa.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"


#define OUTPUT_MODE_NONE               0
#define OUTPUT_MODE_PRIVATE            1
#define OUTPUT_MODE_PUBLIC             2

#define OUTPUT_FORMAT_PEM              0
#define OUTPUT_FORMAT_DER              1

int tal_gen_key(mbedtls_pk_type_t type,mbedtls_ecp_group_id grp_id, int rsa_keysize,mbedtls_pk_context *key);
int tal_Pk_Convert_Buf( mbedtls_pk_context *key, int output_mode,int output_format,uint8_t *output_buf, uint32_t output_buf_len);
int tal_Buf_Convert_Pk(const char *in_buf,int intput_mode,int input_format,mbedtls_pk_context *key);


int test_ty_asymmetric(void);


#ifdef __cplusplus
}
#endif

#endif
