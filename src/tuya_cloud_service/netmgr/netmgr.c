
#include "netmgr.h"
#include "tal_api.h"
#include "tuya_slist.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_error_code.h"
#include "tuya_lan.h"

#ifdef  ENABLE_WIFI
#include "netconn_wifi.h"
extern netmgr_conn_wifi_t s_netmgr_wifi;
#endif

#ifdef ENABLE_WIRED
#include "netconn_wired.h"
extern netmgr_conn_wired_t s_netmgr_wired;
#endif

#ifdef ENABLE_BLUETOOTH
#include "ble_mgr.h"
#endif

typedef struct  {
    MUTEX_HANDLE lock;      // mutex
    BOOL_T inited;

    netmgr_type_e type;     // network manage type
    netmgr_type_e active;   // the connect now used
    netmgr_status_e status; // the network status

    netmgr_conn_base_t *conn[NETCONN_AUTO-1]; // connections
} netmgr_t;

STATIC netmgr_t s_netmgr ={0};

/**
 * @brief get active connection status and 
 * 
 * @return netconn_type_t: the connection should be used
 */
static netmgr_type_e __get_active_conn() 
{
    netmgr_t *netmgr = &s_netmgr;
    if (netmgr->type&NETCONN_WIFI && netmgr->type&NETCONN_WIRED) {
        netmgr_status_e wifi_status = NETMGR_LINK_DOWN;
        netmgr_status_e wired_status = NETMGR_LINK_DOWN;
        netmgr->conn[NETCONN_WIFI]->get(NETCONN_CMD_STATUS, &wifi_status);
        netmgr->conn[NETCONN_WIRED]->get(NETCONN_CMD_STATUS, &wired_status);
        
        if ((NETMGR_LINK_UP == wifi_status && NETMGR_LINK_UP == wired_status)
            || (NETMGR_LINK_UP != wifi_status && NETMGR_LINK_UP != wired_status)) {
            // all connection up or down, return the high priority connection, if same pririty, use wired connection first
            return (netmgr->conn[NETCONN_WIFI]->pri > netmgr->conn[NETCONN_WIRED]->pri) ? NETCONN_WIFI : NETCONN_WIRED;
        } else {
            // otherwise, return the up connection
            return (NETMGR_LINK_UP == wifi_status) ? NETCONN_WIFI : NETCONN_WIRED;
        }
    } else {
        return netmgr->type;
    }
}

/**
 * @brief connection event callback, called when connection event happed
 * 
 * @param event the connection event
 */
STATIC VOID __netmgr_event_cb(netmgr_type_e type, netmgr_status_e status)
{   
    // status unused
    (VOID)status;

    if (s_netmgr.type & type) {
        netmgr_status_e active_status = NETMGR_LINK_DOWN;
        netmgr_type_e active_conn = __get_active_conn();
        s_netmgr.conn[active_conn]->get(NETCONN_CMD_STATUS, &active_status);

        // both changed
        if (active_status != s_netmgr.status && active_conn != s_netmgr.active) {
            PR_DEBUG("netmgr changed active %d status %d, old active %d status %d", active_status, active_conn, s_netmgr.active, s_netmgr.status);
            s_netmgr.status = active_status;
            s_netmgr.active = active_conn;
        } else if (active_conn == s_netmgr.active) { 
            // active_status changed
            PR_DEBUG("netmgr status changed to %d, old %d, active %d", active_status, s_netmgr.status, s_netmgr.active);
            s_netmgr.status = active_status;
        } else if (active_status == s_netmgr.status) { 
            // active_conn changed
            PR_DEBUG("netmgr active changed to %d, old %d, status %d", active_conn, s_netmgr.active, s_netmgr.status);
            s_netmgr.active = active_conn;
        }
    }

    return;
}

OPERATE_RET __netmgr_conn_register(netmgr_type_e type, netmgr_conn_base_t *conn)
{
    s_netmgr.conn[type] = conn;
    s_netmgr.conn[type]->event_cb = __netmgr_event_cb;
    return s_netmgr.conn[type]->open(NULL);
}

OPERATE_RET netmgr_init(netmgr_type_e type)
{
    OPERATE_RET rt = OPRT_OK;

    TUYA_CALL_ERR_RETURN(tal_mutex_create_init(&s_netmgr.lock));
    s_netmgr.status = NETMGR_LINK_DOWN;
    s_netmgr.type = type;

#ifdef ENABLE_WIRED    
    if (type & NETCONN_WIRED) {
        __netmgr_conn_register(NETCONN_WIRED, (netmgr_conn_base_t *)&s_netmgr_wired);
    }
#endif

#ifdef ENABLE_WIFI
    if (type & NETCONN_WIFI) {
        __netmgr_conn_register(NETCONN_WIFI, (netmgr_conn_base_t *)&s_netmgr_wifi);
    }
#endif
    s_netmgr.active = __get_active_conn();
    s_netmgr.inited = TRUE;

    tuya_lan_init(tuya_iot_client_get());

#ifdef ENABLE_BLUETOOTH
    tuya_ble_init(&(tuya_ble_cfg_t) {
        .client      = tuya_iot_client_get(),
        .device_name = "TYBLE"
        });
#endif    

    return rt;
}

OPERATE_RET netmgr_conn_set(netmgr_type_e type, netmgr_conn_config_type_e cmd, void *param)
{
    if (!s_netmgr.inited)
        return OPRT_RESOURCE_NOT_READY;

    PR_TRACE("netmgr conn %d set %d para %p", type, cmd, param);
    OPERATE_RET rt = OPRT_OK;
    if (NETCONN_AUTO == type) {
        TUYA_CALL_ERR_RETURN(s_netmgr.conn[s_netmgr.active]->set(cmd, param));
    } else if (s_netmgr.type&type) {
        TUYA_CALL_ERR_RETURN(s_netmgr.conn[type]->set(cmd, param));
    }

    return rt;
}

OPERATE_RET netmgr_conn_get(netmgr_type_e type, netmgr_conn_config_type_e cmd, void *param)
{
    if (!s_netmgr.inited)
        return OPRT_RESOURCE_NOT_READY;

    PR_TRACE("netmgr conn %d get %d para %p", type, cmd, param);
    OPERATE_RET rt = OPRT_OK;
    if (NETCONN_AUTO == type) {
        TUYA_CALL_ERR_RETURN(s_netmgr.conn[s_netmgr.active]->get(cmd, param));
    } else if (s_netmgr.type&type) {
        TUYA_CALL_ERR_RETURN(s_netmgr.conn[type]->get(cmd, param));
    }

    return rt;
}

VOID netmgr_cmd(INT_T argc, CHAR_T *argv[])
{
    if (!s_netmgr.inited) {
        PR_INFO("network not ready!");
        return;
    }

    if (argc > 3) {
        PR_INFO("usage: netmgr [wifi|wired|switch] [donw/up]");
        return;
    }
    
    if (argc == 1) {
        // dump network connection
        PR_NOTICE("netmgr active %d, status %d", s_netmgr.active, s_netmgr.status);
        PR_NOTICE("---------------------------------------");
        if (s_netmgr.type&NETCONN_WIFI) {
            PR_NOTICE("type %d pri %d status %d", s_netmgr.conn[NETCONN_WIFI]->type, s_netmgr.conn[NETCONN_WIFI]->pri, s_netmgr.conn[NETCONN_WIFI]->status);
        }
        if (s_netmgr.type&NETCONN_WIRED) {
            PR_NOTICE("type %d pri %d status %d", s_netmgr.conn[NETCONN_WIRED]->type, s_netmgr.conn[NETCONN_WIRED]->pri, s_netmgr.conn[NETCONN_WIRED]->status);
        }
    } else {
        if (0 == strcmp(argv[1], "wifi")) {  
            if (s_netmgr.type&NETCONN_WIFI) {
                PR_INFO("usage: netmgr [wifi] [donw/up/scan]");
            } else if (0 == strcmp(argv[2], "up")) {
                // netconn_wifi_connect(netmgr->info.ssid, netmgr->info.password);
            } else if (0 == strcmp(argv[2], "down")) {
                // netconn_wifi_disconnect();
            } else if (0 == strcmp(argv[2], "scan")) {
#ifdef ENABLE_WIFI        
                AP_IF_S *aplist;
                UINT_T  num;
                tal_wifi_all_ap_scan(&aplist, &num);    
#endif            
            } else {
                PR_INFO("usage: netmgr [wifi] [donw/up/scan]");
            }
        } else if (0 == strcmp(argv[1], "wired")) {
            if (!(s_netmgr.type&NETCONN_WIRED)) {
                PR_INFO("usage: netmgr [wired] [donw/up]");
            }else if (0 == strcmp(argv[2], "up")) {
                // TBD..
            } else if (0 == strcmp(argv[2], "down")) {
                // TBD
            } else {
                PR_INFO("usage: netmgr wire [donw/up]");
            }
        } else if (0 == strcmp(argv[1], "switch")) {

        } else {
            PR_INFO("usage: netmgr [wifi|wired|switch] [donw|up]");
        }
    }

    return;
}
