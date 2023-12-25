/**
 * @file tuya_lan_sock.h
 * @author fangweng.hu@tuya.com
 * @brief TUYA lan sock
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_LAN_SOCK_H__
#define __TUYA_LAN_SOCK_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief sock read handler
 *
 * @param[in] sock fd
 * @param[in] sock_ctx user data
 *
 */
typedef VOID (*sloop_sock_read)(INT_T sock);

/**
 * @brief pre select handler
 *
 */
typedef VOID (*sloop_sock_pre_select)();

/**
 * @brief sock err handler
 *
 * @param[in] sock fd
 * @param[in] sock_ctx user data
 *
 */
typedef VOID (*sloop_sock_err)(INT_T sock);

/**
 * @brief sock loop thread quit handler
 *
 */
typedef VOID (*sloop_sock_quit)();

/**
 * @brief reg sock info
 *
 */
typedef struct sloop_sock_t {
    INT_T sock;
    sloop_sock_pre_select pre_select;
    sloop_sock_read read;
    sloop_sock_err err;
    sloop_sock_quit quit;
} sloop_sock_t;

/**
 * @brief register sock
 *
 * @param[in] sock_info reg sock info
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_reg_lan_sock(sloop_sock_t sock_info);

/**
 * @brief unregister sock
 *
 * @param[in] sock fd
 *
 * @note The sock will closed internal, user no need closed manually
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_unreg_lan_sock(INT_T sock);

/**
 * @brief set sock loop disable
 *
 */
VOID tuya_sock_loop_disable();

/**
 * @brief get sock loop terminate vaule
 *
 * @return terminate value
 *
 */
INT_T tuya_get_sock_loop_terminate();

/**
 * @brief dump lan sock info
 *
 */
VOID tuya_dump_lan_sock_reader();

/**
 * @brief sock loop init
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_sock_loop_init(VOID);

#ifdef __cplusplus
}
#endif
#endif //__TUYA_LAN_SOCK_H__

