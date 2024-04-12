/**
 * @file netmgr.h
 * @author network manage module
 * 
 * @copyright Copyright (c) 2023, allright reserved by Tuya Inc
 * 
 */
#ifndef __NETMGR_H___
#define __NETMGR_H___

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief network connection type
 * 
 */
typedef enum {
    NETCONN_WIFI    = 1 << 0,
    NETCONN_WIRED   = 1 << 1,
    NETCONN_AUTO    = 1 << 2,
} netmgr_type_e;

/**
 * @brief the network link event
 * 
 */
typedef enum {
    NETMGR_LINK_DOWN,       // network was disconnected
    NETMGR_LINK_UP,         // network was connected
    NETMGR_LINK_UP_SWITH,   // network was connected but connection changed
} netmgr_status_e;

typedef enum {  
    NETCONN_CMD_PRI,            // INT_T
    NETCONN_CMD_IP,             // NW_IP_S 
    NETCONN_CMD_MAC,            // NW_MAC_S
    NETCONN_CMD_STATUS,         // netmgr_type_e
    NETCONN_CMD_SSID_PSWD,      // netconn_wifi_info_t
    NETCONN_CMD_COUNTRYCODE,    // "US"/"CN"/"EU"/"JP"
    NETCONN_CMD_NETCFG,         // netconn_wifi_netcfg_t
    NETCONN_CMD_SET_STATUS_CB,  // user define status callback instead of the default
    NETCONN_CMD_CLOSE,          // close network connection
    NETCONN_CMD_RESET,          // close network connection
} netmgr_conn_config_type_e;

/**
 * @brief the device network config
 * 
 */
typedef struct {   
    uint8_t pri;
    netmgr_type_e type; 
    netmgr_status_e status;

    OPERATE_RET (*open)(VOID *config);
    OPERATE_RET (*close)(VOID);   
    OPERATE_RET (*set)(netmgr_conn_config_type_e cmd, VOID *param);  
    OPERATE_RET (*get)(netmgr_conn_config_type_e cmd, VOID *param);      
    VOID (*event_cb)(netmgr_type_e type, netmgr_status_e event);   
} netmgr_conn_base_t;

/**
 * @brief network manage init
 * 
 * @param config all network connections
 * @return OPERATE_RET 
 */
OPERATE_RET netmgr_init(netmgr_type_e type);

/**
 * @brief set network connection attribute
 * 
 * @param type connection type
 * @param cmd attribute type
 * @param param input attribute
 * @return OPERATE_RET 
 */
OPERATE_RET netmgr_conn_get(netmgr_type_e type, netmgr_conn_config_type_e cmd, void *param);


/**
 * @brief get network connection attribute
 * 
 * @param type connection type
 * @param cmd attribute type
 * @param param output attribute
 * @return OPERATE_RET 
 */
OPERATE_RET netmgr_conn_set(netmgr_type_e type, netmgr_conn_config_type_e cmd, void *param);

#ifdef __cplusplus
}
#endif


#endif