#ifndef __NETCONN_WIFI_H___
#define __NETCONN_WIFI_H___

#include "tuya_cloud_types.h"
#include "netmgr.h"
#include "netcfg.h"
#include "tal_wifi.h"
#include "tal_semaphore.h"
#include "tuya_iot.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief wifi network configure type
 * 
 */
typedef enum {
    TUYA_NETMGR_NETCFG_AP   = 1 << 0,       // configure wifi according ble
    TUYA_NETMGR_NETCFG_BLE  = 1 << 1,       // configure wifi according wifi ap
} netmgr_netcfg_mode_t;

/**
 * @brief wifi connection event
 * 
 */
typedef enum {
    NETCONN_NETCFG_START = 1,
    NETCONN_NETCFG_DONE,
    NETCONN_NETCFG_TIMEOUT,
    NETCONN_WIFI_CONN_START,
    NETCONN_WIFI_CONN,
    NETCONN_WIFI_CONN_FAILED,
    NETCONN_WIFI_DISCONN,
} netconn_wifi_event_e;

/**
 * @brief wifi reconnection status
 * 
 */
typedef enum {
    NETCONN_WIFI_CONN_REDAY,
    NETCONN_WIFI_CONN_CHECK,
    NETCONN_WIFI_CONN_LINKUP,
    NETCONN_WIFI_CONN_WAIT,
    NETCONN_WIFI_CONN_STOP,
} netconn_wifi_conn_status_e;

/**
 * @brief wifi connection reconnect configure
 * 
 */
#define NETCONN_WIFI_CONN_TABLE 6
#define WIFI_CONN_TIMEOUT_MAX   20
typedef struct {
    CHAR_T  ssid[WIFI_SSID_LEN+1];  // wifi ap ssid
    CHAR_T  pswd[WIFI_PASSWD_LEN+1];// wifi passwd    
    CHAR_T  token[BIND_TOKEN_LEN+1];// wifi bind token
} netconn_wifi_info_t;

/**
 * @brief wifi connect/reconnect configure
 * 
 */
typedef struct {
    netconn_wifi_conn_status_e  stat;
    UINT32_T                    count;
    UINT32_T                    table_size;
    UINT32_T                    table[NETCONN_WIFI_CONN_TABLE];
    TIMER_ID                    timer;
    
    netconn_wifi_info_t         wifi_conn_info;                     // the connected wifi info
} netconn_wifi_conn_t;

/**
 * @brief  wifi connection netcfg configure
 * 
 */
typedef struct {
    SEM_HANDLE          netcfg_sem;             // wifi netcfg timeout semaphore

    BOOL_T              netcfg_finish;          // wifi netcfg finish flag
    UINT32_T            netcfg_mode;            // wifi netcfg mode
    UINT32_T            netcfg_timeout;         // wifi netcfg timeout in ms, min is 60*1000
    netcfg_args_t       netcfg_args;            // wifi netcfg parameters, only support qrcode scan now
} netconn_wifi_netcfg_t;

/**
 * @brief network connection wifi configure
 * 
 */
typedef struct {
    netmgr_conn_base_t   base;                 // connection base, keep first

    CHAR_T ccode[COUNTRY_CODE_LEN+1];
    netconn_wifi_netcfg_t   netcfg;                 // wifi netcfg configure
    netconn_wifi_conn_t     conn;                   // wifi connect/reconnect configure
} netmgr_conn_wifi_t;


/**
 * @brief open a wifi connection
 * 
 * @param config 
 * @return OPERATE_RET
 */
OPERATE_RET netconn_wifi_open(VOID *config);

/**
 * @brief close a wifi connection
 * 
 * @param config 
 * @return netconn_wifi_config_t* 
 */
OPERATE_RET netconn_wifi_close(VOID);

/**
 * @brief update wifi connection
 * 
 * @param config: the new config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wifi_set(netmgr_conn_config_type_e cmd, void *param);

/**
 * @brief get wifi connection attribte
 * 
 * @param type 
 * @param cmd 
 * @param param 
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wifi_get(netmgr_conn_config_type_e cmd, void *param);

#ifdef __cplusplus
}
#endif


#endif
