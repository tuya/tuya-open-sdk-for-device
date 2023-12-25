/**
 * @file tal_event.h
 * @brief tuya simple event module
 *
 * @copyright Copyright (c) tuya.inc 2023
 *
 */

#ifndef __TAL_EVENT_H__
#define __TAL_EVENT_H__

#include "tuya_cloud_types.h"
#include "tuya_list.h"
#include "tal_event_info.h"
#include "tal_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief max length of event name
 *
 */
#ifndef EVENT_NAME_MAX_LEN
#define EVENT_NAME_MAX_LEN (16)  // move to tuya_iot_config.h. use kconfig config. default is 16
#endif

/**
 * @brief max length of event description
 *
 */
#define EVENT_DESC_MAX_LEN (32)

/**
 * @brief subscriber type
 *
 */
typedef BYTE_T SUBSCRIBE_TYPE_E;
#define SUBSCRIBE_TYPE_NORMAL    0  // normal type, dispatch by the subscribe order, remove when unsubscribe
#define SUBSCRIBE_TYPE_EMERGENCY 1  // emergency type, dispatch first, remove when unsubscribe
#define SUBSCRIBE_TYPE_ONETIME   2  // one time type, dispatch by the subscribe order, remove after first time dispath

/**
 * @brief the event dispatch raw data
 *
 */
typedef struct {
    INT_T type;           // the data type
    INT_T len;            // the data length
    CHAR_T value[0];      // the data content
} EVENT_RAW_DATA_T;

/**
 * @brief event subscribe callback function type
 *
 */
typedef INT_T(*EVENT_SUBSCRIBE_CB)(VOID_T *data);

/**
 * @brief the subscirbe node
 *
 */
typedef struct {
    CHAR_T name[EVENT_NAME_MAX_LEN + 1];  // name, used to record the the event info
    CHAR_T desc[EVENT_DESC_MAX_LEN + 1];  // description, used to record the subscribe info
    SUBSCRIBE_TYPE_E type;              // the subscribe type
    EVENT_SUBSCRIBE_CB cb;              // the subscribe callback function
    struct tuya_list_head node;         // list node, used to attch to the event node
} SUBSCRIBE_NODE_T;

/**
 * @brief the event node
 *
 */
typedef struct {
    MUTEX_HANDLE mutex;                         // mutex, protection the event publish and subscribe

    CHAR_T name[EVENT_NAME_MAX_LEN + 1];          // name, the event name
    struct tuya_list_head node;                 // list node, used to attach to the event manage module
    struct tuya_list_head subscribe_root;       // subscibe root, used to manage the subscriber
} EVENT_NODE_T;

/**
 * @brief the event manage node
 *
 */
typedef struct {
    INT_T inited;
    MUTEX_HANDLE mutex;                             // mutex, used to protection event manage node
    INT_T event_cnt;                                  // current event number
    struct tuya_list_head event_root;               // event root, used to manage the event
    struct tuya_list_head free_subscribe_root;      // free subscriber list, used to manage the subscribe which not found the event
} EVENT_MANAGE_T;

/**
 * @brief event initialization
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_event_init(VOID);

/**
 * @brief: publish event
 *
 * @param[in] name: event name
 * @param[in] data: event data
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_event_publish(CONST CHAR_T* name, VOID_T *data);

/**
 * @brief: subscribe event
 *
 * @param[in] name: event name
 * @param[in] desc: subscribe description
 * @param[in] cb: subscribe callback function
 * @param[in] type: subscribe type
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_event_subscribe(CONST CHAR_T *name, CONST CHAR_T *desc, CONST EVENT_SUBSCRIBE_CB cb, SUBSCRIBE_TYPE_E type);

/**
 * @brief: unsubscribe event
 *
 * @param[in] name: event name
 * @param[in] desc: subscribe description
 * @param[in] cb: subscribe callback function
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_event_unsubscribe(CONST CHAR_T *name, CONST CHAR_T *desc, EVENT_SUBSCRIBE_CB cb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*__TAL_EVENT_H__ */


