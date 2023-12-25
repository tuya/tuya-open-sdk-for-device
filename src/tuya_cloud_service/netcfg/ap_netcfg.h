#ifndef _AP_NETCFG_H_
#define _AP_NETCFG_H_

#include "netcfg.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief register netcfg to netcfg module
 *
 * @param[in] netcfg_policy type
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int ap_netcfg_init(netcfg_args_t *netcfg);

#ifdef __cplusplus
}
#endif
#endif
