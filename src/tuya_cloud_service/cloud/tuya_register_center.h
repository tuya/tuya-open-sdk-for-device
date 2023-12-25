/**
 * @file tuya_register_center.h
 * @brief Tuya Register Center
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright 2022 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_REGISTER_CENTER_H__
#define __TUYA_REGISTER_CENTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "cJSON.h"

/**
 * Register Center Source
 */
typedef enum {
    RCS_APP,
    RCS_MF,
    RCS_CODE,
    RCS_MAX
} RCS_E;

/**
 * Definition of Register Center
 */
typedef struct {
    RCS_E source;
    BOOL_T pub;
    CHAR_T *code;
    CHAR_T *url0;
    CHAR_T *urlx;
    BYTE_T *ca_cert;
    UINT_T ca_cert_len;
} register_center_t;

/**
 * @brief Init and load register center
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_register_center_init(VOID);

/**
 * @brief Save register center
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_register_center_save(RCS_E source, cJSON *rcs);

/**
 * @brief Get register center
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_register_center_get(register_center_t *rcs);

/**
 * @brief Update ca cert of register center
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_register_center_update(BYTE_T *ca_cert, UINT_T ca_cert_len);

/**
 * @brief Reset register center
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_register_center_reset(VOID);

/**
 * @brief get register center url
 *
 * @return url address
 */
CHAR_T* tuya_register_center_get_url(VOID);

#ifdef __cplusplus
}
#endif

#endif //__TUYA_REGISTER_CENTER_H__
