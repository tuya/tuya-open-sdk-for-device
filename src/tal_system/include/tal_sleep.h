/**
 * @file tal_sleep.h
 * @brief This is tal_sleep file
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_SLEEP_H__
#define __TAL_SLEEP_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/


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
 * @brief sleep callback register
 *
 * @param[in] sleep_cb:  sleep callback
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_cpu_sleep_callback_register(TUYA_SLEEP_CB_T *sleep_cb);

/**
 * @brief allow to sleep
 *
 * @param[in] none
 *
 * @return none
 */
VOID_T tal_cpu_allow_sleep(VOID_T);

/**
 * @brief force wakeup
 *
 * @param[in] none
 *
 * @return none
 */
VOID_T tal_cpu_force_wakeup(VOID_T);

/**
 * @brief set cpu lowpower mode
 *
 * @param[in] lp_enable
 *
 * @return none
 */
VOID_T tal_cpu_set_lp_mode(BOOL_T lp_enable);

/**
 * @brief get cpu lowpower mode
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
BOOL_T tal_cpu_get_lp_mode(VOID_T);

/**
 * @brief cpu lowpower enable
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_cpu_lp_enable(VOID_T);

/**
 * @brief cpu lowpower disable
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_cpu_lp_disable(VOID_T);


#ifdef __cplusplus
}
#endif

#endif /* __TAL_SLEEP_H__ */

