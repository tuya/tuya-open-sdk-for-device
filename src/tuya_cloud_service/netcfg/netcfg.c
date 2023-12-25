#include "netcfg.h"
#include "tal_api.h"
#include "tuya_list.h"
#include "tal_workq_service.h"
#include "tal_event.h"

#define COUNT_OF(X)	((sizeof(X))/(sizeof(X[0])))

/*配网模式n是否在组合配网mode中存在*/
#define IS_SET(n,mode)	(((n)&(mode)) == (n))

typedef struct {
    LIST_HEAD listHead;
    int type;
} netcfg_session_t;

/*
配网模式切换初始化是否完成
*/
static bool isInited = false;
static netcfg_session_t *p_netcfg_session = NULL;

STATIC VOID __netcfg_start_msg_cb(VOID *msg)
{
    netcfg_handler_t * pHandler = NULL;
    pHandler = (netcfg_handler_t *)msg;

    int ret = pHandler->start(pHandler->type, pHandler->netcfg_finish_cb, pHandler->args);
    if (ret == OPRT_OK) {
        pHandler->isStarted = true;
    }
    PR_DEBUG("start 0x%x ret:%d", pHandler->type, ret);
}


/*
	配网注册接口，tuya smartcfg, ap netcfg, bt netcfg等都注册到该模块
	注册时，根据注册的配网类型，提前设置好模式，模式需要做的动作，比如：
	ez配网时，设置station, 执行动作：scan ap，并存储ap信息
	ap配网时，设置softap/stationap,执行动作：设置ap的ssid，并启动ap
*/
int netcfg_register(int type, netcfg_start_cb_t start, netcfg_stop_cb_t stop)
{
    //遍历listHead，查找是否有相同type的netcfg handler已经注册，如果存在，禁止重复注册
    P_LIST_HEAD pPos;
    netcfg_handler_t * phandler;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (phandler->type == type) {
            return OPRT_INVALID_PARM;
        }
    }

    //不存在type netcfg handler, 申请handler，并加入listHead
    NEW_LIST_NODE(netcfg_handler_t, phandler);
    if (phandler == NULL) {
        return OPRT_MALLOC_FAILED;
    }

    phandler->type = type;
    phandler->start = start;
    phandler->stop = stop;
    phandler->isStarted = false;
    phandler->netcfg_finish_cb = NULL;

    tuya_list_add(&phandler->node, & p_netcfg_session->listHead);

    return OPRT_OK;

}


int netcfg_unregister(int 	type)
{
    if (p_netcfg_session == NULL) {
        return OPRT_COM_ERROR;
    }
    P_LIST_HEAD pPos, pNext;
    netcfg_handler_t * phandler;
    tuya_list_for_each_safe(pPos, pNext, &(p_netcfg_session->listHead)) {
        phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (phandler->type == type) {
            DeleteNodeAndFree(phandler, node);
            return OPRT_OK;
        }
    }

    return OPRT_INVALID_PARM;

}

/*
	获取netcfg handler的接口
*/
netcfg_handler_t * netcfg_get_handler(int type)
{

    if (p_netcfg_session == NULL) {
        return NULL;
    }

    P_LIST_HEAD pPos;
    netcfg_handler_t * phandler;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (phandler->type == type) {
            return phandler;
        }
    }

    return NULL;
}

int netcfg_init(void)
{
    if (p_netcfg_session == NULL) {
        p_netcfg_session = tal_malloc(sizeof(netcfg_session_t));
        if (p_netcfg_session == NULL) {
            return OPRT_MALLOC_FAILED;
        }
        INIT_LIST_HEAD(&p_netcfg_session->listHead);
    }

    return OPRT_OK;
}

int netcfg_uninit(void)
{
    if (p_netcfg_session) {
        P_LIST_HEAD pPos, pNext;
        netcfg_handler_t * phandler;
        tal_workq_cancel(WORKQ_HIGHTPRI, __netcfg_start_msg_cb, NULL);

        tuya_list_for_each_safe(pPos, pNext, &(p_netcfg_session->listHead)) {
            phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
            if (phandler) {
                DeleteNodeAndFree(phandler, node);
            }
        }
        tal_free(p_netcfg_session);
        p_netcfg_session = NULL;
    }

    return OPRT_OK;
}

/*
	获取已经注册的配网组件总数
*/
int netcfg_get_register_count(void)
{

    if (p_netcfg_session == NULL) {
        return 0;
    }
    int count = 0;
    P_LIST_HEAD pPos;
    netcfg_handler_t * phandler;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (phandler) {
            count++;
        }
    }
    return count;
}

/*
	获取已经启动的配网组件总数
*/

int netcfg_get_register_started_count(void)
{
    if (p_netcfg_session == NULL) {
        return 0;
    }
    int count = 0;
    P_LIST_HEAD pPos;
    netcfg_handler_t * phandler;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        phandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (phandler->isStarted) {
            count++;
        }
    }
    return count;
}
/*
TBD:
原计划配网使用方式：
netcfg_start(type1 | type2 |...)
废除 vendor 和 cfgtype的随意组合，来匹配list中配网type的方案
直接通过type 匹配list中的配网type
从使用者角度来说，需要
netcfg_start(type1)
netcfg_start(type2)
....

*/
int netcfg_start(int type, netcfg_finish_cb_t netcfg_finish_cb, void *args)
{
    netcfg_handler_t *pHandler = NULL;
    /*有注册配网，并且没有启动的配网时，执行模式切换处理*/
    if (!isInited) {
        // netcfg_mode_handler_process();
        isInited = true;
        // tal_event_publish(EVENT_WIFI_STATUS_UPDATE, NULL);
    }

    PR_DEBUG("netcfg module start type:0x%x", type);
    pHandler = netcfg_get_handler(type);
    if (pHandler == NULL) {
        PR_ERR("netcfg type 0x%x is not regist", type);
        return OPRT_INVALID_PARM;
    } else {
        pHandler->netcfg_finish_cb = netcfg_finish_cb;
        pHandler->args = args;
        tal_workq_schedule(WORKQ_HIGHTPRI, __netcfg_start_msg_cb, pHandler);
    }

    return OPRT_OK;
}

BOOL_T is_netcfg_inited(void)
{
    return isInited;
}


/*
	func desc:
	停止配网活动
	func params:
	type:
	0: 停止所有配网
	!=0 :停止指定配网

*/
int netcfg_stop(int type)
{

    int ret = OPRT_OK;
    netcfg_handler_t * pHandler = NULL;

    /*停止类型为type的配网*/
    if (type != 0) {
        pHandler = netcfg_get_handler(type);
        if (pHandler == NULL) {
            return OPRT_INVALID_PARM;
        } else {
            // CAPTURE_NETCFG_DATA(type, TY_DATA_NETCFG_STEP8);
            PR_DEBUG("netcfg module stop type:0x%x", type);
            if (pHandler->isStarted) {
                pHandler->isStarted = false;
                ret = pHandler->stop(type);
                if (ret != OPRT_OK) {
                    PR_ERR("netcfg module stop type:%d failed", type);
                    return ret;
                } else {
                    return OPRT_OK;
                }
            }
        }
    } else { /*停止所有的配网*/

        if (p_netcfg_session == NULL) {
            return OPRT_COM_ERROR;
        }
        P_LIST_HEAD pPos;
        tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
            pHandler = tuya_list_entry(pPos, netcfg_handler_t, node);
            if (pHandler->isStarted) {
                pHandler->isStarted = false;
                PR_DEBUG("netcfg module stop type:0x%x", pHandler->type);
                ret = pHandler->stop(pHandler->type);
                if (ret != OPRT_OK) {
                    PR_ERR("netcfg module stop type:0x%x fail", type);
                    continue;
                }
            }
        }
        /*destroy netcfg module*/
        netcfg_uninit();

    }
    return OPRT_OK;

}


/*
    func_desc:
    开启除了type之外所有配网
*/
int netcfg_start_other_all(int type)
{
    int ret = OPRT_OK;
    netcfg_handler_t * pHandler = NULL;
    if (p_netcfg_session == NULL) {
        return OPRT_COM_ERROR;
    }

    P_LIST_HEAD pPos;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        pHandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if ((pHandler->netcfg_finish_cb) && (!pHandler->isStarted) && (type != pHandler->type)) {
            PR_DEBUG("netcfg module start type:0x%x", pHandler->type);
            ret = netcfg_start(pHandler->type, pHandler->netcfg_finish_cb, pHandler->args);
            if (ret != OPRT_OK) {
                PR_ERR("netcfg module start type:0x%x fail", type);
                continue;
            }
        }
    }

    return OPRT_OK;
}

/*
	func desc:
	停止其他配网活动
	func params:
	type:
	!=0 :停止type外其他配网

*/
int netcfg_stop_other_all(int type)
{

    int ret = OPRT_OK;
    netcfg_handler_t * pHandler = NULL;

    if (p_netcfg_session == NULL) {
        return OPRT_COM_ERROR;
    }

    P_LIST_HEAD pPos;
    tuya_list_for_each(pPos, &(p_netcfg_session->listHead)) {
        pHandler = tuya_list_entry(pPos, netcfg_handler_t, node);
        if (pHandler->isStarted && (type != pHandler->type)) {
            pHandler->isStarted = false;
            PR_DEBUG("netcfg module stop type:0x%x", pHandler->type);
            ret = pHandler->stop(pHandler->type);
            if (ret != OPRT_OK) {
                PR_ERR("netcfg module stop type:0x%x fail", type);
                continue;
            }
        }
    }


    return OPRT_OK;
}

