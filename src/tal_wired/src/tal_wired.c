/**
* @file tal_wired.c
* @brief Common process - wired abstration api implement
* @version 0.1
* @date 2020-11-09
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/
#include "tal_wired.h"
#include "tkl_init_wired.h"

// the tkl description, must be exist
extern TKL_WIRED_DESC_T c_wired_desc;

/**
 * @brief  get the link status of wired link
 *
 * @param[out]  is_up: the wired link status is up or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_wired_get_status(WIRED_STAT_E *stat)
{
    return tkl_wired_get_status(stat);
}

/**
 * @brief  set the status change callback
 *
 * @param[in]   cb: the callback when link status changed
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_wired_set_status_cb(TAL_WIRED_STATUS_CHANGE_CB cb)
{
    return tkl_wired_set_status_cb(cb);
}

/**
 * @brief  get the ip address of the wired link
 * 
 * @param[in]   ip: the ip address
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_wired_get_ip(NW_IP_S *ip)
{
    return tkl_wired_get_ip(ip);
}

/**
 * @brief  get the mac address of the wired link
 * 
 * @param[in]   mac: the mac address
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_wired_get_mac(NW_MAC_S *mac)
{    
    return tkl_wired_get_mac(mac);
}

/**
 * @brief  set the mac address of the wired link
 * 
 * @param[in]   mac: the mac address
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_wired_set_mac(CONST NW_MAC_S *mac)
{
    return tkl_wired_set_mac(mac);
}


