/**
 * @file tal_mutex.h
 * @brief Provides mutex (mutual exclusion) management functions for Tuya IoT
 * applications.
 *
 * This header file defines the interface for mutex management in Tuya IoT
 * applications, including functions for creating, locking, unlocking, and
 * releasing mutexes. These functions abstract the underlying operating system's
 * mutex mechanisms, providing a simple and consistent API for synchronization
 * across different platforms. The mutex management functions are essential for
 * ensuring thread safety and preventing race conditions in multi-threaded
 * environments.
 *
 * The API supports creating a mutex handle, locking and unlocking mutexes for
 * critical sections, and releasing mutex resources when they are no longer
 * needed. This facilitates the development of reliable and concurrent
 * applications on the Tuya IoT platform.
 *
 * @note This file is part of the Tuya IoT Development Platform and is intended
 * for use in Tuya-based applications. It is subject to the platform's license
 * and copyright terms.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_MUTEX_H__
#define __TAL_MUTEX_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *MUTEX_HANDLE;

/**
 * @brief Create mutex
 *
 * @param[out] handle: mutex handle
 *
 * @note This API is used to create and init mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_mutex_create_init(MUTEX_HANDLE *handle);

/**
 * @brief Lock mutex
 *
 * @param[in] handle: mutex handle
 *
 * @note This API is used to lock mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_mutex_lock(const MUTEX_HANDLE handle);

/**
 * @brief Unlock mutex
 *
 * @param[in] handle: mutex handle
 *
 * @note This API is used to unlock mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_mutex_unlock(const MUTEX_HANDLE handle);

/**
 * @brief Release mutex
 *
 * @param[in] mutexHandle: mutex handle
 *
 * @note This API is used to release mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_mutex_release(const MUTEX_HANDLE handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
