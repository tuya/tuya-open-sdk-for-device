#include "tal_sleep.h"
#include "tal_mutex.h"
#include "tal_log.h"
#include "tkl_sleep.h"

OPERATE_RET tal_cpu_sleep_mode_set(BOOL_T enable, TUYA_CPU_SLEEP_MODE_E mode)
{
    return tkl_cpu_sleep_mode_set(enable, mode);

}
typedef struct {
    BOOL_T          lp_enable;
    BYTE_T          lp_mode_cnt;
    MUTEX_HANDLE    lp_mutex;
    UINT_T          lp_disable_cnt;
} TAL_CPU_T;

STATIC TAL_CPU_T s_tal_cpu = {0};

VOID_T tal_cpu_set_lp_mode(BOOL_T lp_enable)
{
    //function can be called only once
    PR_DEBUG("cpu_set_lp_mode_cnt:%d", s_tal_cpu.lp_mode_cnt);
    if (s_tal_cpu.lp_mode_cnt > 0) {
        return;
    }
    if (lp_enable) {
        tal_cpu_sleep_mode_set(TRUE, TUYA_CPU_SLEEP);
    }
    PR_DEBUG("set cpu lp mode:%d", lp_enable);
    s_tal_cpu.lp_enable = lp_enable;
    s_tal_cpu.lp_mode_cnt++;
}

BOOL_T tal_cpu_get_lp_mode(VOID_T)
{
    return s_tal_cpu.lp_enable;
}

STATIC OPERATE_RET tal_cpu_lp_init_mutex(VOID_T)
{
    if (NULL != s_tal_cpu.lp_mutex) {
        return OPRT_OK;
    }

    OPERATE_RET op_ret = tal_mutex_create_init(&s_tal_cpu.lp_mutex);
    if (OPRT_OK != op_ret) {
        PR_ERR("create mutex fail");
    }

    return op_ret;
}

//cpu lowpower feature api
OPERATE_RET tal_cpu_lp_enable(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;
    if (!tal_cpu_get_lp_mode()) {
        PR_DEBUG("can not enable, lowpower disabled");
        return OPRT_COM_ERROR;
    }
    op_ret = tal_cpu_lp_init_mutex();
    if (OPRT_OK != op_ret) {
        return op_ret;
    }
    tal_mutex_lock(s_tal_cpu.lp_mutex);
    if (s_tal_cpu.lp_disable_cnt > 0) {
        s_tal_cpu.lp_disable_cnt--;
    }
    PR_DEBUG("<tal_cpu_lp> disable_cnt:%d", s_tal_cpu.lp_disable_cnt);
    if (!s_tal_cpu.lp_disable_cnt) {
        op_ret = tal_cpu_sleep_mode_set(TRUE, TUYA_CPU_SLEEP);
    }
    tal_mutex_unlock(s_tal_cpu.lp_mutex);
    if (OPRT_OK != op_ret) {
        PR_ERR("cpu_lp_enable: set cpu lp mode fail(%d)", op_ret);
    }

    return op_ret;
}

OPERATE_RET tal_cpu_lp_disable(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;
    if (!tal_cpu_get_lp_mode()) {
        PR_DEBUG("cpu has been disabled");
        return OPRT_OK;
    }
    op_ret = tal_cpu_lp_init_mutex();
    if (OPRT_OK != op_ret) {
        return op_ret;
    }
    tal_mutex_lock(s_tal_cpu.lp_mutex);
    if (!s_tal_cpu.lp_disable_cnt++) {
        op_ret = tal_cpu_sleep_mode_set(FALSE, TUYA_CPU_SLEEP);
    }
    PR_DEBUG("<tal_cpu_lp>  disable_cnt:%d", s_tal_cpu.lp_disable_cnt);
    tal_mutex_unlock(s_tal_cpu.lp_mutex);

    if (OPRT_OK != op_ret) {
        PR_ERR("tuya_cpu_lp_disable: set cpu lp mode fail(%d)", op_ret);
    }

    return op_ret;
}
