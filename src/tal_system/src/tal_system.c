#include "tkl_system.h"
#include "tkl_memory.h"
#include "tal_system.h"
#include "tal_sleep.h"
#include "tal_log.h"
#include "tal_memory.h"

VOID_T *tal_malloc(SIZE_T size)
{
    if (0 == size) {
        return NULL;
    }

    VOID_T *ptr = NULL;
    ptr = tkl_system_malloc(size);
    if (NULL == ptr) {
        PR_ERR("0x%x malloc failed:0x%x free:0x%x",
            __builtin_return_address(0), size, tal_system_get_free_heap_size());
    }

    return ptr;
}

VOID_T tal_free(VOID_T* ptr)
{
    if (NULL == ptr) {
        return;
    }

    tkl_system_free(ptr);
}

VOID_T *tal_calloc(SIZE_T nitems, SIZE_T size)
{
    return tkl_system_calloc(nitems, size);
}

VOID_T *tal_realloc(VOID_T* ptr, SIZE_T size)
{
    return tkl_system_realloc(ptr, size);
}


VOID_T tal_system_sleep(UINT_T time_ms)
{
    tkl_system_sleep(time_ms);
}

VOID_T tal_system_reset(VOID_T)
{
    tkl_system_reset();
}

INT_T tal_system_get_free_heap_size(VOID_T)
{
    return tkl_system_get_free_heap_size();
}

SYS_TICK_T tal_system_get_tick_count(VOID_T)
{
    return tkl_system_get_tick_count();
}

SYS_TIME_T g_sys_time_offset = 0; // used for debug
SYS_TIME_T tal_system_get_millisecond(VOID_T)
{
    return tkl_system_get_millisecond() + g_sys_time_offset;
}

INT_T tal_system_get_random(UINT_T range)
{
    return tkl_system_get_random(range);
}


TUYA_RESET_REASON_E tal_system_get_reset_reason(CHAR_T** describe)
{
    return tkl_system_get_reset_reason(describe);
}

OPERATE_RET tal_system_get_cpu_info(TUYA_CPU_INFO_T **cpu_ary, INT_T *cpu_cnt)
{
    return tkl_system_get_cpu_info(cpu_ary, cpu_cnt);
}
