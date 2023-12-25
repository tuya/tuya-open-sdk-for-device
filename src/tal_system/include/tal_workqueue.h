/**
 * @file tal_workqueue.h
 * @brief tuya workqueue module
 * 
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 * 
 */
#ifndef __TAL_WORKQUEUE_H__
#define __TAL_WORKQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tal_thread.h"

typedef enum {
    LOOP_ONCE,
    LOOP_CYCLE
}LOOP_TYPE;

typedef VOID_T* WORKQUEUE_HANDLE;
typedef VOID_T (*WORKQUEUE_CB)(VOID_T *data);

typedef struct {
    WORKQUEUE_CB cb;
    VOID_T *data;
}WORK_ITEM_T;
typedef BOOL_T (*WORKQUEUE_TRAVERSE_CB)(WORK_ITEM_T*item, VOID_T *ctx);

/**
 * @brief create and initialize a workqueue which runs in thread context
 * 
 * @param[in] queue_len the maximum number of items that the workqueue can contain
 * @param[in] thread_cfg thread param
 * @param[out] handle the workqueue handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_create(CONST UINT16_T queue_len, THREAD_CFG_T *thread_cfg, WORKQUEUE_HANDLE *handle);

/**
 * @brief put work task in workqueue
 *
 * @param[in] handle the workqueue handle
 * @param[in] cb the work callback
 * @param[in] data the work data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_schedule(WORKQUEUE_HANDLE handle, WORKQUEUE_CB cb, VOID_T *data);

/**
 * @brief put work task in workqueue, instant will be dequeued first
 *
 * @param[in] handle the workqueue handle
 * @param[in] cb the work callback
 * @param[in] data the work data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_schedule_instant(WORKQUEUE_HANDLE handle, WORKQUEUE_CB cb, VOID_T *data);

/**
 * @brief cancel work task in workqueue
 *
 * @param[in] handle the workqueue handle
 * @param[in] cb the work callback
 * @param[in] data the work data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_cancel(WORKQUEUE_HANDLE handle, WORKQUEUE_CB cb, VOID_T *data);

/**
 * @brief traverse the queue with specific callback
 *
 * @param[in] handle the workqueue handle
 * @param[in] cb the traverse callback
 * @param[in] ctx the traverse context
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_traverse(WORKQUEUE_HANDLE handle, WORKQUEUE_TRAVERSE_CB cb, VOID_T *ctx);

/**
 * @brief get the workqueue item number
 *
 * @param[in] handle the workqueue handle
 *
 * @return the current item counts 
 */
UINT16_T tal_workqueue_get_num(WORKQUEUE_HANDLE handle);

/**
 * @brief release the workqueue
 *
 * @param[in] handle the workqueue handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_release(WORKQUEUE_HANDLE handle);

/**
 * @brief get thread handle of the workqueue
 *
 * @param[in] handle the workqueue handle
 *
 * @return thread handle
 */
THREAD_HANDLE tal_workqueue_get_thread(WORKQUEUE_HANDLE handle);

typedef VOID_T* DELAYED_WORK_HANDLE;

/**
 * @brief init delayed work in workqueue
 *
 * @param[in] handle the workqueue handle
 * @param[in] cb the work callback
 * @param[in] data the work data
 * @param[out] delayed_work handle of delayed work
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_init_delayed(WORKQUEUE_HANDLE handle, WORKQUEUE_CB cb, VOID_T *data,
    DELAYED_WORK_HANDLE *delayed_work);

/**
 * @brief put work in workqueue after delay
 *
 * @param[in] delayed_work handle of delayed work
 * @param[in] interval number of ms to wait or 0 for immediate execution
 * @param[in] type see @LOOP_TYPE
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_start_delayed(DELAYED_WORK_HANDLE delayed_work,
    TIME_MS interval, LOOP_TYPE type);

/**
 * @brief stop delayed work
 *
 * @param[in] delayed_work handle of delayed work
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_stop_delayed(DELAYED_WORK_HANDLE delayed_work);

/**
 * @brief cancel delayed work
 *
 * @param[in] delayed_work handle of delayed work
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_workqueue_cancel_delayed(DELAYED_WORK_HANDLE delayed_work);

#ifdef __cplusplus
}
#endif
#endif

