#include <string.h>
#include "tuya_list.h"
#include "tal_thread.h"
#include "tkl_thread.h"
#include "tal_mutex.h"
#include "tal_log.h"
#include "tal_memory.h"
#include "tal_system.h"
typedef struct {
    THREAD_HANDLE               thrdID;         
    INT_T                       thrdRunSta;     
    THREAD_FUNC_CB              pThrdFunc;      
    PVOID_T                     pThrdFuncArg;   
    UINT_T                      stackDepth;     
    THREAD_ENTER_CB             enter;          
    THREAD_EXIT_CB              exit;           
    CHAR_T                      thread_name[TAL_THREAD_MAX_NAME_LEN];
    LIST_HEAD node;
} THRD_MANAGE, *P_THRD_MANAGE;

typedef struct {
    LIST_HEAD                   list;
    MUTEX_HANDLE                mutex;
} DEL_THRD_MAG_S;

STATIC DEL_THRD_MAG_S *s_del_thrd_mag = NULL;
STATIC LIST_HEAD       s_all_thrd_mag;

STATIC VOID __WrapRunFunc(IN VOID *pArg);
STATIC VOID __inner_del_thread(IN THREAD_HANDLE thrdID);

STATIC OPERATE_RET __cr_and_init_del_thrd_mag(VOID)
{
    if (NULL != s_del_thrd_mag) {
        return OPRT_OK;
    }

    DEL_THRD_MAG_S *tmp_del_thrd_mag = tal_malloc(SIZEOF(DEL_THRD_MAG_S));
    if (NULL == tmp_del_thrd_mag) {
        PR_ERR("malloc fail");
        return OPRT_MALLOC_FAILED;
    }
    memset(tmp_del_thrd_mag, 0, SIZEOF(DEL_THRD_MAG_S));
    INIT_LIST_HEAD(&(tmp_del_thrd_mag->list));
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tal_mutex_create_init(&(tmp_del_thrd_mag->mutex));
    if(OPRT_OK != op_ret) {
        PR_ERR("Create Mutex err");
        tal_free(tmp_del_thrd_mag);
        return op_ret;
    }
    s_del_thrd_mag = tmp_del_thrd_mag;

    return OPRT_OK;
}

STATIC VOID __add_del_thrd_node(THRD_MANAGE *thrd)
{
    if (NULL == thrd || NULL == s_del_thrd_mag) {
        return;
    }
    tal_mutex_lock(s_del_thrd_mag->mutex);
    thrd->thrdRunSta = THREAD_STATE_STOP;
    tuya_list_add_tail(&(thrd->node),&(s_del_thrd_mag->list));
    tal_mutex_unlock(s_del_thrd_mag->mutex);
}

// need to port in different os
STATIC VOID __inner_del_thread(IN THREAD_HANDLE thrdID)
{
    PR_DEBUG("real delete thread:%p", thrdID);
    // delete thread process
    tkl_thread_release(thrdID);
}

STATIC VOID __free_all_del_thrd_node(VOID)
{
    if (NULL == s_del_thrd_mag) {
        return;
    }

    THRD_MANAGE *tmp_node = NULL;
    THRD_MANAGE *self_node = NULL;
    BOOL_T is_self = FALSE;
    THREAD_HANDLE thrdID = NULL;

    tal_mutex_lock(s_del_thrd_mag->mutex);

    LIST_HEAD *pos = NULL, *pNext = NULL;

    tuya_list_for_each_safe(pos, pNext, &(s_del_thrd_mag->list)) {
        PR_DEBUG("del list not empty...deleting %p, next %p", pos, pNext);
        tmp_node = tuya_list_entry(pos, THRD_MANAGE, node);
        if (THREAD_STATE_DELETE != tmp_node->thrdRunSta) {
            PR_DEBUG("Thread:%s is still running..", tmp_node->thread_name);
            continue; 
        }
        tkl_thread_is_self(tmp_node->thrdID, &is_self);
        PR_DEBUG("Final Free Thread:%s, is_self:%d", tmp_node->thread_name, is_self);
        DeleteNode(tmp_node,node);
        if (is_self) {
            self_node = tmp_node;
            PR_DEBUG("delay to delete thread self");
        } else {
            PR_DEBUG("delete thread not self");
            thrdID = tmp_node->thrdID;
            tal_free(tmp_node);
            __inner_del_thread(thrdID);
        }
    }
    tal_mutex_unlock(s_del_thrd_mag->mutex);

    if (is_self) {
        PR_DEBUG("finally delete thread self");
        thrdID = self_node->thrdID;
        tal_free(self_node);
        __inner_del_thread(thrdID);
    }
}

OPERATE_RET tal_thread_create_and_start(THREAD_HANDLE          *handle,
                                        CONST THREAD_ENTER_CB   enter,
                                        CONST THREAD_EXIT_CB    exit,
                                        CONST THREAD_FUNC_CB    func,
                                        CONST PVOID_T           func_args,
                                        CONST THREAD_CFG_T     *cfg)
{
    if(NULL == s_del_thrd_mag) {
        PR_TRACE("Init Thread Del Mgr");
        __cr_and_init_del_thrd_mag();
        INIT_LIST_HEAD(&s_all_thrd_mag);
    }

    if (!handle || !func) {
        PR_ERR("Para null");
        return OPRT_INVALID_PARM;
    }
    THRD_MANAGE *pMgr = (P_THRD_MANAGE)tal_malloc(sizeof(THRD_MANAGE));
    if(pMgr == NULL) {
        PR_ERR("Malloc err");
        return OPRT_MALLOC_FAILED;
    }
    memset(pMgr, 0, SIZEOF(THRD_MANAGE));
    INIT_LIST_HEAD(&(pMgr->node));

    pMgr->thrdRunSta    = THREAD_STATE_EMPTY;
    pMgr->pThrdFunc     = func;
    pMgr->pThrdFuncArg  = func_args;
    pMgr->enter         = enter;
    pMgr->exit          = exit;
    strncpy(pMgr->thread_name, cfg->thrdname, TAL_THREAD_MAX_NAME_LEN - 1);
    *handle             = pMgr;
    pMgr->stackDepth    = cfg->stackDepth;

    tal_mutex_lock(s_del_thrd_mag->mutex);
    tuya_list_add_tail(&(pMgr->node),&s_all_thrd_mag);
    tal_mutex_unlock(s_del_thrd_mag->mutex);

    INT_T opRet;
    opRet = tkl_thread_create(&(pMgr->thrdID), 
                            cfg->thrdname, 
                            cfg->stackDepth, 
                            cfg->priority, 
                            __WrapRunFunc, 
                            pMgr);
    if (opRet != 0) {
        PR_ERR("Create Thrd Fail:%d", opRet);
        tal_mutex_lock(s_del_thrd_mag->mutex);
        tuya_list_del(&(pMgr->node));
        tal_mutex_unlock(s_del_thrd_mag->mutex);
        tal_free(pMgr);
       *handle = NULL;
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }

    STATIC INT_T stack_cnt = 0;
    stack_cnt += cfg->stackDepth;    
    PR_INFO("thread_create name:%s,stackDepth:%d,totalstackDepth:%d,priority:%d", cfg->thrdname, cfg->stackDepth, stack_cnt, cfg->priority);

    return OPRT_OK;
}

STATIC VOID __WrapRunFunc(IN PVOID_T pArg)
{
    __free_all_del_thrd_node();

    P_THRD_MANAGE pThrdManage = (P_THRD_MANAGE)pArg;
	
#if OPERATING_SYSTEM == SYSTEM_LINUX 
    tkl_thread_set_self_name(pThrdManage->thread_name);
#endif
    if(pThrdManage->enter) {
        PR_DEBUG("enter Thread:%s func call", pThrdManage->thread_name);
        pThrdManage->enter();
    }
    PR_DEBUG("Thread:%s Exec Start. Set to Running Stat", pThrdManage->thread_name);
    pThrdManage->thrdRunSta = THREAD_STATE_RUNNING;
    pThrdManage->pThrdFunc(pThrdManage->pThrdFuncArg);
    // must call <DeleteThrdHandle> to delete thread
    THREAD_STATE_E status = THREAD_STATE_EMPTY;
    do {
        tal_mutex_lock(s_del_thrd_mag->mutex);
        status = pThrdManage->thrdRunSta;
        tal_mutex_unlock(s_del_thrd_mag->mutex);
        tal_system_sleep(10);
    }while(status != THREAD_STATE_STOP);

    if (pThrdManage->exit) { // exit noraml
        PR_DEBUG("exit Thread:%s func call", pThrdManage->thread_name);
        pThrdManage->exit();
    }
    PR_DEBUG("Thread:%s Exec Finish. Set to Del Stat", pThrdManage->thread_name);
    tal_mutex_lock(s_del_thrd_mag->mutex);    
    pThrdManage->thrdRunSta = THREAD_STATE_DELETE;
    tal_mutex_unlock(s_del_thrd_mag->mutex);
    __free_all_del_thrd_node();
}

THREAD_STATE_E tal_thread_get_state(CONST THREAD_HANDLE handle)
{
    if (NULL == handle) {
        PR_ERR("Para null");
        return -1;
    }
    P_THRD_MANAGE pThrdManage = (P_THRD_MANAGE)handle;

    return pThrdManage->thrdRunSta;
}

OPERATE_RET tal_thread_delete(CONST THREAD_HANDLE handle)
{
    if (NULL == handle) {
        PR_ERR("Para null");
        return OPRT_INVALID_PARM;
    }
    P_THRD_MANAGE pThrdManage = (P_THRD_MANAGE)handle;
    PR_DEBUG("Del Thrd:%s", pThrdManage->thread_name);
    tal_mutex_lock(s_del_thrd_mag->mutex);
    if (THREAD_STATE_EMPTY == pThrdManage->thrdRunSta) {
        tal_mutex_unlock(s_del_thrd_mag->mutex);
        return OPRT_COM_ERROR;
    }
    tuya_list_del(&(pThrdManage->node));
    tal_mutex_unlock(s_del_thrd_mag->mutex);
    __add_del_thrd_node(pThrdManage);

    return OPRT_OK;
}

OPERATE_RET tal_thread_is_self(CONST THREAD_HANDLE handle, BOOL_T *bl)
{
    if (NULL == handle || NULL == bl) {
        return OPRT_INVALID_PARM;
    }

    P_THRD_MANAGE pThrdManage = (P_THRD_MANAGE)handle;
    BOOL_T is_self = false;
    int ret = tkl_thread_is_self(pThrdManage->thrdID, &is_self);
    if (ret != 0) {
        return ret;
    }

    if (is_self) {
        *bl = TRUE;
    } else {
        *bl = FALSE;
    }

    return OPRT_OK;
}

OPERATE_RET tal_thread_diagnose(CONST THREAD_HANDLE handle)
{
    OPERATE_RET ret = OPRT_OK;

    if (NULL == handle) {
        return OPRT_INVALID_PARM;
    }

    P_THRD_MANAGE pThrdManage = (P_THRD_MANAGE)handle;
    ret = tkl_thread_diagnose(pThrdManage->thrdID);

    return ret;
}

VOID tal_thread_dump_watermark(VOID)
{
    if(!s_del_thrd_mag) {
        return;
    }

    LIST_HEAD *pos = NULL;
    THRD_MANAGE *tmp_node = NULL;
    UINT_T watermark = 0;
    INT_T op_ret;
    
    tal_mutex_lock(s_del_thrd_mag->mutex);
    tuya_list_for_each(pos, &s_all_thrd_mag) {
        tmp_node = tuya_list_entry(pos, THRD_MANAGE, node);
        op_ret = tkl_thread_get_watermark(tmp_node->thrdID, &watermark);
        if(OPRT_OK == op_ret) {
            PR_DEBUG("thread[%-16s] stack[%5d] free[%5d]",
                tmp_node->thread_name, tmp_node->stackDepth, watermark);
        } else {
            break;
        }
    }
    tal_mutex_unlock(s_del_thrd_mag->mutex);
}
