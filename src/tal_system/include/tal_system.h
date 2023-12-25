/**
 * @file tal_system.h
 * @brief This is tal_system file
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_SYSTEM_H__
#define __TAL_SYSTEM_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/
/**
 * @brief enter critical macro
 */
#define TAL_ENTER_CRITICAL()        \
    UINT32_T __irq_mask;              \
    __irq_mask = tal_system_enter_critical()

/**
 * @brief exit critical macro
 */
#define TAL_EXIT_CRITICAL()         \
    tal_system_exit_critical(__irq_mask)

/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/


/***********************************************************************
 ********************* variable ****************************************
 **********************************************************************/


/***********************************************************************
 ********************* function ****************************************
 **********************************************************************/

/**
 * @brief system enter critical
 *
 * @param[in]   none
 * @return  irq mask
 */
UINT32_T tal_system_enter_critical(VOID_T);

/**
 * @brief system exit critical
 *
 * @param[in]   irq_mask: irq mask
 * @return  none
 */
VOID_T tal_system_exit_critical(UINT32_T irq_mask);

/**
 * @brief This API is used for system sleep.
 *
 * @param[in] time_ms: time in MS
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tal_system_sleep(UINT32_T time_ms);

/**
 * @brief tal_system_reset
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tal_system_reset(VOID_T);

/**
 * @brief tal_system_get_tick_count
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
SYS_TICK_T tal_system_get_tick_count(VOID_T);

/**
 * @brief tal_system_get_millisecond
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
SYS_TIME_T tal_system_get_millisecond(VOID_T);

/**
 * @brief Get system random data
 *
 * @param[in] range: random from 0  to range
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT32_T tal_system_get_random(UINT32_T range);

/**
 * @brief tal_system_get_reset_reason
 *
 * @param[in] describe: point to reset reason describe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
TUYA_RESET_REASON_E tal_system_get_reset_reason(CHAR_T** describe);

/**
 * @brief This API is used for system delay.
 *
 * @param[in] time_ms: time in MS
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tal_system_delay(UINT32_T time_ms);

/**
 * @brief This API is used for system cpu info get.
 *
 * @param[in] **cpu_ary: info of cpus
 * @param[in] *cpu_cnt: num of cpu
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_system_get_cpu_info(TUYA_CPU_INFO_T **cpu_ary, INT32_T *cpu_cnt);


#ifdef __cplusplus
}
#endif

#endif /* __TAL_SYSTEM_H__ */

