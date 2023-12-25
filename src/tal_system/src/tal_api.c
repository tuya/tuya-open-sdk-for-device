#include "tkl_semaphore.h"
#include "tkl_mutex.h"
#include "tkl_ota.h"
#include "tkl_queue.h"
#include "tal_mutex.h"
#include "tal_semaphore.h"
#include "tal_queue.h"
#include "tal_ota.h"

//! sem
OPERATE_RET tal_semaphore_create_init(SEM_HANDLE *handle, UINT_T sem_cnt, UINT_T sem_max)
{
    return tkl_semaphore_create_init(handle, sem_cnt, sem_max);
}

OPERATE_RET tal_semaphore_wait(SEM_HANDLE handle, UINT_T timeout)
{
    return tkl_semaphore_wait(handle, timeout);
}

OPERATE_RET tal_semaphore_post(SEM_HANDLE handle)
{
    return tkl_semaphore_post(handle);
}

OPERATE_RET tal_semaphore_release(SEM_HANDLE handle)
{
    return tkl_semaphore_release(handle);
}

//! mutex
OPERATE_RET tal_mutex_create_init(MUTEX_HANDLE *handle)
{
    return tkl_mutex_create_init(handle);
}

OPERATE_RET tal_mutex_lock(CONST MUTEX_HANDLE handle)
{
    return tkl_mutex_lock(handle);
}

OPERATE_RET tal_mutex_unlock(CONST MUTEX_HANDLE handle)
{
    return tkl_mutex_unlock(handle);
}

OPERATE_RET tal_mutex_release(CONST MUTEX_HANDLE handle)
{
    return tkl_mutex_release(handle);
}

//! ota
OPERATE_RET tal_ota_get_ability(UINT_T *image_size, TUYA_OTA_TYPE_E *type)
{
    return tkl_ota_get_ability(image_size, type);
}

OPERATE_RET tal_ota_start_notify(UINT_T image_size, TUYA_OTA_TYPE_E type, TUYA_OTA_PATH_E path)
{
    return tkl_ota_start_notify(image_size, type, path);
}

OPERATE_RET tal_ota_data_process(TUYA_OTA_DATA_T *pack, UINT_T* remain_len)
{
    return tkl_ota_data_process(pack, remain_len);
}

OPERATE_RET tal_ota_end_notify(BOOL_T reset)
{
    return tkl_ota_end_notify(reset);
}

//! queue
OPERATE_RET tal_queue_create_init(QUEUE_HANDLE *queue, INT_T msgsize, INT_T msgcount)
{
    return tkl_queue_create_init(queue, msgsize, msgcount);
}

OPERATE_RET tal_queue_post(QUEUE_HANDLE queue, VOID_T *data, UINT_T timeout)
{
    return tkl_queue_post(queue, data, timeout);
}

OPERATE_RET tal_queue_fetch(QUEUE_HANDLE queue, VOID_T *msg, UINT_T timeout)
{
    return tkl_queue_fetch(queue, msg, timeout);
}

VOID_T tal_queue_free(QUEUE_HANDLE queue)
{
    tkl_queue_free(queue);
}
