/**
 * @file tal_event.c
 * @brief event, 基于timer queue、message queue、work queue实现的事件中心
 *
 * @copyright Copyright (c) tuya.inc 2023
 *
 */
#include "tuya_cloud_types.h"
#include "tal_event.h"
#include "tal_api.h"

STATIC EVENT_MANAGE_T g_event_manager = {0};

BOOL_T _event_name_is_valid(const char *name)
{
    if (!name) {
        return FALSE;
    }

    if (strlen(name) > EVENT_NAME_MAX_LEN || strlen(name) <= 0) {
        return FALSE;
    }

    return TRUE;
}

BOOL_T _event_desc_is_valid(const char *desc)
{
    if (!desc) {
        return FALSE;
    }

    if (strlen(desc) > EVENT_DESC_MAX_LEN || strlen(desc) <= 0) {
        return FALSE;
    }

    return TRUE;
}

EVENT_NODE_T *_event_node_create_init(const char *name)
{
    // allocate memory
    EVENT_NODE_T *event = tal_malloc(sizeof(EVENT_NODE_T));
    TUYA_CHECK_NULL_RETURN(event, NULL);
    memset(event, 0, sizeof(EVENT_NODE_T));

    // initialze the event node
    memcpy(event->name, name, strlen(name));
    event->name[strlen(name)] = '\0';
    INIT_LIST_HEAD(&event->subscribe_root);
    tal_mutex_create_init(&event->mutex);

    tal_mutex_lock(g_event_manager.mutex);

    // need check if there have free subscriber which subscribe this event
    struct tuya_list_head *free_pos = NULL;
    struct tuya_list_head *free_next = NULL;
    SUBSCRIBE_NODE_T *free_entry = NULL;
    tuya_list_for_each_safe(free_pos, free_next, &g_event_manager.free_subscribe_root) {
        // find by event name, and add it to subscribe list
        free_entry = tuya_list_entry(free_pos, SUBSCRIBE_NODE_T, node);
        if (0 == strcmp(free_entry->name, name)) {
            // del from free subscrbe list
            tuya_list_del(&free_entry->node);

            // add to different position according to the emergence flag
            if (free_entry->type == SUBSCRIBE_TYPE_EMERGENCY) {
                tuya_list_add(&free_entry->node, &event->subscribe_root);
            } else {
                tuya_list_add_tail(&free_entry->node, &event->subscribe_root);
            }
        }
    }

    // at last, need add this event to event manage root
    tuya_list_add_tail(&event->node, &g_event_manager.event_root);
    g_event_manager.event_cnt++;

    tal_mutex_unlock(g_event_manager.mutex);

    return event;
}

EVENT_NODE_T *_event_node_get(const char *name)
{
    // try to get event from event root
    EVENT_NODE_T *entry = NULL;
    struct tuya_list_head *pos = NULL;
    tuya_list_for_each(pos, &g_event_manager.event_root) {
        // find by name
        entry = tuya_list_entry(pos, EVENT_NODE_T, node);
        if (0 == strcmp(entry->name, name)) {
            return entry;
        }
    }

    return NULL;
}

SUBSCRIBE_NODE_T *_event_node_get_free_subscribe(SUBSCRIBE_NODE_T *subscribe)
{
    struct tuya_list_head *pos = NULL;
    SUBSCRIBE_NODE_T *entry = NULL;
    tuya_list_for_each(pos, &g_event_manager.free_subscribe_root) {
        // find by desc
        entry = tuya_list_entry(pos, SUBSCRIBE_NODE_T, node);
        if (0 == strcmp(entry->name, subscribe->name) &&
            0 == strcmp(entry->desc, subscribe->desc) &&
            entry->cb == subscribe->cb) {
            return entry;
        }
    }

    return NULL;
}

SUBSCRIBE_NODE_T *_event_node_get_subscribe(EVENT_NODE_T *event, SUBSCRIBE_NODE_T *subscribe)
{
    struct tuya_list_head *pos = NULL;
    SUBSCRIBE_NODE_T *entry = NULL;
    tuya_list_for_each(pos, &event->subscribe_root) {
        // find by desc
        entry = tuya_list_entry(pos, SUBSCRIBE_NODE_T, node);
        if (0 == strcmp(entry->desc, subscribe->desc) && entry->cb == subscribe->cb) {
            return entry;
        }
    }

    return NULL;
}

OPERATE_RET _event_node_dispatch(EVENT_NODE_T *event, void *data)
{
    OPERATE_RET rt = OPRT_OK;

    // dispatch in order
    struct tuya_list_head *p = NULL;
    struct tuya_list_head *n = NULL;
    SUBSCRIBE_NODE_T *entry = NULL;
    tuya_list_for_each_safe(p, n, &event->subscribe_root) {
        // find and call cb one by one
        entry = tuya_list_entry(p, SUBSCRIBE_NODE_T, node);
        if (entry->cb) {
            TUYA_CALL_ERR_LOG(entry->cb(data));
        }

        // one-time event should be removed after dispatch
        if (entry->type == SUBSCRIBE_TYPE_ONETIME) {
            tuya_list_del(&entry->node);
            tal_free(entry);
            entry = NULL;
        }
    }

    return rt;
}

OPERATE_RET _event_node_add_free_subscribe(SUBSCRIBE_NODE_T* subscribe)
{
    OPERATE_RET rt = OPRT_OK;

    // existed, return ok, dont care, pretend to success
    SUBSCRIBE_NODE_T *new_entry = _event_node_get_free_subscribe(subscribe);
    if (new_entry) {
        return OPRT_OK;
    }

    // malloc a new entry and prepare to add
    new_entry = (SUBSCRIBE_NODE_T *)tal_malloc(sizeof(SUBSCRIBE_NODE_T));
    TUYA_CHECK_NULL_RETURN(new_entry, OPRT_MALLOC_FAILED);
    memcpy(new_entry, subscribe, sizeof(SUBSCRIBE_NODE_T));

    tuya_list_add_tail(&new_entry->node, &g_event_manager.free_subscribe_root);
    return rt;
}

OPERATE_RET _event_node_add_subscribe(EVENT_NODE_T *event, SUBSCRIBE_NODE_T* subscribe)
{
    OPERATE_RET rt = OPRT_OK;

    // existed, return ok, dont care, pretend to success
    SUBSCRIBE_NODE_T *new_entry = _event_node_get_subscribe(event, subscribe);
    if (new_entry) {
        return OPRT_OK;
    }

    // malloc a new entry and prepare to add
    new_entry = (SUBSCRIBE_NODE_T *)tal_malloc(sizeof(SUBSCRIBE_NODE_T));
    TUYA_CHECK_NULL_RETURN(new_entry, OPRT_MALLOC_FAILED);
    memcpy(new_entry, subscribe, sizeof(SUBSCRIBE_NODE_T));

    // try to add, if emergence, add to first, otherwise, add to tail
    if (subscribe->type == SUBSCRIBE_TYPE_EMERGENCY) {
        tuya_list_add(&new_entry->node, &event->subscribe_root);
    } else {
        tuya_list_add_tail(&new_entry->node, &event->subscribe_root);
    }

    return rt;
}

OPERATE_RET _event_node_del_free_subscribe(SUBSCRIBE_NODE_T* subscribe)
{
    OPERATE_RET rt = OPRT_OK;
    // not existed, return ok, dont care, pretend to success
    SUBSCRIBE_NODE_T *new_entry = _event_node_get_free_subscribe(subscribe);
    if (new_entry == NULL) {
        return OPRT_OK;
    }

    // dont forget remove and free
    tuya_list_del(&new_entry->node);
    tal_free(new_entry);
    new_entry = NULL;

    return rt;
}

OPERATE_RET _event_node_del_subscribe(EVENT_NODE_T *event, SUBSCRIBE_NODE_T* subscribe)
{
    OPERATE_RET rt = OPRT_OK;
    SUBSCRIBE_NODE_T *new_entry = NULL;
    // not existed, return ok, dont care, pretend to success
    new_entry = _event_node_get_subscribe(event, subscribe);
    if (new_entry == NULL) {
        return OPRT_OK;
    }

    // dont forget remove and free
    tuya_list_del(&new_entry->node);
    tal_free(new_entry);
    new_entry = NULL;
    return rt;
}

#if 0
int _ty_event_dump()
{
    if (g_event_manager.inited == 0) {
        return OPRT_OK;
    }

    struct tuya_list_head *e_pos = NULL;
    struct tuya_list_head *s_pos = NULL;
    EVENT_NODE_T *event = NULL;

    // event and subscribe
    PR_DEBUG("------------------------------------------------------------------");
    PR_DEBUG("name                desc                                cb");
    PR_DEBUG("------------------------------------------------------------------");
    tuya_list_for_each(e_pos, &g_event_manager.event_root) {
        event = tuya_list_entry(e_pos, EVENT_NODE_T, node);
        if (event) {
            SUBSCRIBE_NODE_T *subscribe = NULL;
            if (!tuya_list_empty(&event->subscribe_root)) {
                tuya_list_for_each(s_pos, &event->subscribe_root) {
                    subscribe = tuya_list_entry(s_pos, SUBSCRIBE_NODE_T, node);
                    if (subscribe) {
                        PR_DEBUG("%-16s    %-32s    0x%08x", subscribe->name, subscribe->desc, subscribe->cb);
                    }
                }
            }
        }
    }

    // free subscribe
    PR_DEBUG("------------------------free--------------------------------------");
    PR_DEBUG("name                desc                                cb");
    PR_DEBUG("------------------------------------------------------------------");
    SUBSCRIBE_NODE_T * subscribe = NULL;
    tuya_list_for_each(s_pos, &g_event_manager.free_subscribe_root) {
        subscribe = tuya_list_entry(s_pos, SUBSCRIBE_NODE_T, node);
        if (subscribe) {
            PR_DEBUG("%-16s    %-32s    0x%08x", subscribe->name, subscribe->desc, subscribe->cb);
        }
    }

    PR_DEBUG("\n");

    return OPRT_OK;
}
#endif

OPERATE_RET tal_event_init(VOID)
{
    OPERATE_RET rt = OPRT_OK;

    if (g_event_manager.inited == TRUE) {
        return OPRT_OK;
    }

    // due to the event api maybe called before iot param init,
    // we will add os adapter and base layer init here to make it success

    INIT_LIST_HEAD(&g_event_manager.event_root);
    INIT_LIST_HEAD(&g_event_manager.free_subscribe_root);
    tal_mutex_create_init(&g_event_manager.mutex);
    g_event_manager.event_cnt = 0;
    g_event_manager.inited = TRUE;

    //create this two event first, in case no RAM when health check need reboot
    // _event_node_create_init(EVENT_REBOOT_REQ);
    // _event_node_create_init(EVENT_REBOOT_ACK);

    return rt;
}

OPERATE_RET tal_event_publish(CONST CHAR_T* name, VOID_T *data)
{
    if (g_event_manager.inited != TRUE) {
        tal_event_init();
    }

    if (!_event_name_is_valid(name)) {
        return OPRT_BASE_EVENT_INVALID_EVENT_NAME;
    }

    OPERATE_RET rt = OPRT_OK;
    // try to get event, if not exist, create and init.
    EVENT_NODE_T *event = _event_node_get(name);
    if (!event) {
        event = _event_node_create_init(name);
        TUYA_CHECK_NULL_RETURN(event, OPRT_MALLOC_FAILED);
    }

    // to keep the consistency, dispatch will done in mutex lock
    tal_mutex_lock(event->mutex);
    // try to dispatch event to all subscribe
    // if one of the subscribe failed, it will continue but will return failed to record the execute status
    TUYA_CALL_ERR_LOG(_event_node_dispatch(event, data));

    tal_mutex_unlock(event->mutex);

    return rt;
}

OPERATE_RET tal_event_subscribe(CONST CHAR_T *name, CONST CHAR_T *desc, CONST EVENT_SUBSCRIBE_CB cb, SUBSCRIBE_TYPE_E type)
{
    if (g_event_manager.inited != TRUE) {
        tal_event_init();
    }

    if (!_event_desc_is_valid(desc)) {
        return OPRT_BASE_EVENT_INVALID_EVENT_DESC;
    }

    if (!_event_name_is_valid(name)) {
        return OPRT_BASE_EVENT_INVALID_EVENT_NAME;
    }

    OPERATE_RET rt = OPRT_OK;
    SUBSCRIBE_NODE_T subscribe = {{0}};
    subscribe.cb = cb;
    subscribe.type = type;
    memcpy(subscribe.name, name, strlen(name));
    subscribe.name[strlen(name)] = '\0';
    memcpy(subscribe.desc, desc, strlen(desc));
    subscribe.desc[strlen(desc)] = '\0';

    EVENT_NODE_T *event = _event_node_get(name);
    if (!event) {
        // if not found the event, add to the free list
        tal_mutex_lock(g_event_manager.mutex);
        TUYA_CALL_ERR_LOG(_event_node_add_free_subscribe(&subscribe));
        tal_mutex_unlock(g_event_manager.mutex);
    } else {
        // if found the event, add to the subscribe list
        tal_mutex_lock(event->mutex);
        TUYA_CALL_ERR_LOG(_event_node_add_subscribe(event, &subscribe));
        tal_mutex_unlock(event->mutex);
    }

    return rt;
}


OPERATE_RET tal_event_unsubscribe(CONST CHAR_T *name, CONST CHAR_T *desc, EVENT_SUBSCRIBE_CB cb)
{
    if (g_event_manager.inited != TRUE) {
        tal_event_init();
    }

    if (!_event_desc_is_valid(desc)) {
        return OPRT_BASE_EVENT_INVALID_EVENT_DESC;
    }

    if (!_event_name_is_valid(name)) {
        return OPRT_BASE_EVENT_INVALID_EVENT_NAME;
    }

    OPERATE_RET rt = OPRT_OK;
    SUBSCRIBE_NODE_T subscribe = {{0}};
    subscribe.cb = cb;
    memcpy(subscribe.name, name, strlen(name));
    subscribe.name[strlen(name)] = '\0';
    memcpy(subscribe.desc, desc, strlen(desc));
    subscribe.desc[strlen(desc)] = '\0';

    EVENT_NODE_T *event = _event_node_get(name);
    if (!event) {
        // if not found the event, del from the free list
        tal_mutex_lock(g_event_manager.mutex);
        TUYA_CALL_ERR_LOG(_event_node_del_free_subscribe(&subscribe));
        tal_mutex_unlock(g_event_manager.mutex);
    } else {
        // if found the event, del from the subscribe list
        tal_mutex_lock(event->mutex);
        TUYA_CALL_ERR_LOG(_event_node_del_subscribe(event, &subscribe));
        tal_mutex_unlock(event->mutex);
    }


    return rt;
}

