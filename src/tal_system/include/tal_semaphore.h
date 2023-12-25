/**
* @file tkl_semaphore.h
* @brief Common process - adapter the semaphore api provide by OS
*
* @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
*
*/
#ifndef __TAL_SEMAPHORE_H__
#define __TAL_SEMAPHORE_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PVOID_T SEM_HANDLE;

#define SEM_WAIT_FOREVER    0xFFFFffff

/**
* @brief Create semaphore
*
* @param[out] handle: semaphore handle
* @param[in] sem_cnt: semaphore init count
* @param[in] sem_max: semaphore max count
*
* @note This API is used for creating semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_semaphore_create_init(SEM_HANDLE *handle, UINT_T sem_cnt, UINT_T sem_max);

/**
* @brief Wait semaphore
*
* @param[in] handle: semaphore handle
* @param[in] timeout: wait timeout, SEM_WAIT_FOREVER means wait until get semaphore
*
* @note This API is used for waiting semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_semaphore_wait(SEM_HANDLE handle, UINT_T timeout);

/**
* @brief Post semaphore
*
* @param[in] handle: semaphore handle
*
* @note This API is used for posting semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_semaphore_post(SEM_HANDLE handle);

/**
* @brief Release semaphore
*
* @param[in] handle: semaphore handle
*
* @note This API is used for releasing semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_semaphore_release(SEM_HANDLE handle);


/**
* @brief Wait semaphore forever macro
*
* @param[in] handle: semaphore handle
*
* @note This API is used for waiting semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
#define tal_semaphore_wait_forever(__handle)                                \
    tal_semaphore_wait(__handle, SEM_WAIT_FOREVER)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

