/**
 * @file tal_semaphore.h
 * @brief Provides semaphore management functions for Tuya IoT applications.
 *
 * This header file defines the interface for semaphore management in Tuya IoT
 * applications, including functions for creating, waiting, posting, and
 * releasing semaphores. Semaphores are synchronization primitives that are used
 * to manage concurrent access to shared resources by multiple threads. The API
 * abstracts the underlying operating system's semaphore mechanisms, providing a
 * simple and consistent interface for application developers.
 *
 * Semaphores are essential for ensuring thread safety and preventing race
 * conditions in multi-threaded environments. This file is part of the Tuya IoT
 * Development Platform and is intended for use in Tuya-based applications.
 *
 * @note This file is subject to the platform's license and copyright terms.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_SEMAPHORE_H__
#define __TAL_SEMAPHORE_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *SEM_HANDLE;

#define SEM_WAIT_FOREVER 0xFFFFffff

/**
 * @brief Create semaphore
 *
 * @param[out] handle: semaphore handle
 * @param[in] sem_cnt: semaphore init count
 * @param[in] sem_max: semaphore max count
 *
 * @note This API is used for creating semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_semaphore_create_init(SEM_HANDLE *handle, uint32_t sem_cnt, uint32_t sem_max);

/**
 * @brief Wait semaphore
 *
 * @param[in] handle: semaphore handle
 * @param[in] timeout: wait timeout, SEM_WAIT_FOREVER means wait until get
 * semaphore
 *
 * @note This API is used for waiting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_semaphore_wait(SEM_HANDLE handle, uint32_t timeout);

/**
 * @brief Post semaphore
 *
 * @param[in] handle: semaphore handle
 *
 * @note This API is used for posting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_semaphore_post(SEM_HANDLE handle);

/**
 * @brief Release semaphore
 *
 * @param[in] handle: semaphore handle
 *
 * @note This API is used for releasing semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_semaphore_release(SEM_HANDLE handle);

/**
 * @brief Wait semaphore forever macro
 *
 * @param[in] handle: semaphore handle
 *
 * @note This API is used for waiting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
#define tal_semaphore_wait_forever(__handle) tal_semaphore_wait(__handle, SEM_WAIT_FOREVER)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
