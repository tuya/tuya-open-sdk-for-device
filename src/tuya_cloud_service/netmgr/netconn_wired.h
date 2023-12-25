#ifndef __NETCONN_WIRED_H___
#define __NETCONN_WIRED_H___

#include "tuya_cloud_types.h"
#include "tal_wired.h"
#include "netmgr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the wired connect object
 * 
 */
typedef struct {
    netmgr_conn_base_t  base;
} netmgr_conn_wired_t;

/**
 * @brief open wired connection
 * 
 * @param config: wired connection config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_open(VOID *config);

/**
 * @brief close wired connection
 * 
 * @param config: wired connection config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_close(VOID);

/**
 * @brief update wired connection
 * 
 * @param config: the new config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_set(netmgr_conn_config_type_e cmd, VOID *param);

/**
 * @brief get wired connection attribte
 * 
 * @param type 
 * @param cmd 
 * @param param 
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_get(netmgr_conn_config_type_e cmd, VOID *param);

#ifdef __cplusplus
}
#endif


#endif
