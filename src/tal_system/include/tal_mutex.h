/**
* @file tal_mutex.h
* @brief Common process - adapter the mutex api provide by OS
*
* @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
*
*/
#ifndef __TAL_MUTEX_H__
#define __TAL_MUTEX_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PVOID_T MUTEX_HANDLE;

/**
* @brief Create mutex
*
* @param[out] handle: mutex handle
*
* @note This API is used to create and init mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_mutex_create_init(MUTEX_HANDLE *handle);

/**
* @brief Lock mutex
*
* @param[in] handle: mutex handle
*
* @note This API is used to lock mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_mutex_lock(CONST MUTEX_HANDLE handle);


/**
* @brief Unlock mutex
*
* @param[in] handle: mutex handle
*
* @note This API is used to unlock mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_mutex_unlock(CONST MUTEX_HANDLE handle);

/**
* @brief Release mutex
*
* @param[in] mutexHandle: mutex handle
*
* @note This API is used to release mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_mutex_release(CONST MUTEX_HANDLE handle);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
