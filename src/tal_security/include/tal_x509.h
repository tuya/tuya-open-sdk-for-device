/**
* @file tal_x509.h
* @brief Common process - adapter the tal_x509 api
* @version 0.1
* @date 2022-04-22
*
* @copyright Copyright 2021-2025 Tuya Inc. All Rights Reserved.
*
*/
#ifndef __TAL_X509_H__
#define __TAL_X509_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    X509_fingerprint_SHA1 = 0,
    X509_fingerprint_SHA256 = 1,
}X509_fingerprint;

typedef PVOID_T tuya_x509_struct;

BOOL_T tuya_x509_is_ca_pem_format(UINT8_T *buf, UINT32_T buflen);
OPERATE_RET tuya_x509_pem2der(UINT8_T *buf, UINT32_T buflen, UINT8_T **out, UINT32_T *outlen);
OPERATE_RET tuya_x509_get_serial( UINT8_T *buf, UINT32_T buflen,UINT8_T out[32], UINT32_T *outlen);
OPERATE_RET tuya_x509_get_fingerprint( UINT8_T *buf, UINT32_T buflen, X509_fingerprint type, UINT8_T out[64], UINT32_T *outlen );
OPERATE_RET tuya_x509_self_test( void );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

