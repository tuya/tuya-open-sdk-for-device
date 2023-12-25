/**
 * @file tal_queue.h
 * @brief  Common process - Initialization
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TAL_QUEUE_H__
#define __TAL_QUEUE_H__

#include "tuya_cloud_types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef PVOID_T QUEUE_HANDLE;

/**
 * @brief Create message queue
 *
 * @param[in] msgsize message size
 * @param[in] msgcount message number
 * @param[out] queue the queue handle created
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_queue_create_init(QUEUE_HANDLE *queue, INT_T msgsize, INT_T msgcount);

/**
 * @brief post a message to the message queue
 *
 * @param[in] queue the handle of the queue
 * @param[in] data the data of the message
 * @param[in] timeout timeout time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_queue_post(QUEUE_HANDLE queue, VOID_T *data, UINT_T timeout);

/**
 * @brief fetch message from the message queue
 *
 * @param[in] queue the message queue handle
 * @param[inout] msg the message fetch form the message queue
 * @param[in] timeout timeout time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_queue_fetch(QUEUE_HANDLE queue, VOID_T *msg, UINT_T timeout);

/**
 * @brief free the message queue
 *
 * @param[in] queue the message queue handle
 *
 * @return VOID_T
 */
VOID_T tal_queue_free(QUEUE_HANDLE queue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
