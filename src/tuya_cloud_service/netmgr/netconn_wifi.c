/**
 * @file netconn_wifi.c
 * @brief wifi network config and connection manage
 * 
 * @copyright Copyright (c) 2023, all right reserved by Tuya Inc
 * 
 */
#include "netconn_wifi.h"
#include "tal_api.h"
#include "cJSON.h"
#include "ap_netcfg.h"

#ifdef ENABLE_BLUETOOTH
#include "ble_mgr.h"
#include "ble_netcfg.h"
#include "tuya_ble_service.h"
#endif

typedef enum {
    NETCONN_WIFI_MSG_CONNECT,
    NETCONN_WIFI_MSG_DISCONNECT,
    NETCONN_WIFI_MSG_SCAN,
} netmgr_wifi_msg_type_t;

typedef struct {
    int             type;
    netmgr_conn_wifi_t  *handle;
} netmgr_wifi_msg_t;


netmgr_conn_wifi_t s_netmgr_wifi = {
    .base= {
        .pri=0,
        .type=NETCONN_WIFI, 
        .open=netconn_wifi_open,
        .close=netconn_wifi_close,
        .get=netconn_wifi_get,
        .set=netconn_wifi_set
    },
    .ccode = {"CN"},
    .conn = {
        .table_size = NETCONN_WIFI_CONN_TABLE,
        .table = {1,3,5,10,15,20}
    }
};

STATIC VOID __netconn_wifi_connect_process(VOID *msg)
{
    netmgr_wifi_msg_t *wifi_msg = (netmgr_wifi_msg_t *)msg;
    netmgr_conn_wifi_t *wifi = wifi_msg->handle;

    switch (wifi_msg->type) {
    case NETCONN_WIFI_MSG_CONNECT:
        PR_DEBUG("wifi connnet %s",  wifi->conn.wifi_conn_info.ssid);
        tal_wifi_station_disconnect();
        tal_sw_timer_start(wifi->conn.timer, WIFI_CONN_TIMEOUT_MAX * 1000, TAL_TIMER_ONCE);
        wifi->conn.stat = NETCONN_WIFI_CONN_CHECK;
        tal_wifi_set_work_mode(WWM_STATION);
        tal_wifi_station_connect((SCHAR_T *)wifi->conn.wifi_conn_info.ssid, (SCHAR_T *)wifi->conn.wifi_conn_info.pswd);
        break;

    case NETCONN_WIFI_MSG_DISCONNECT:
        tal_sw_timer_stop(wifi->conn.timer);
        wifi->conn.count = 0;
        wifi->conn.stat  = NETCONN_WIFI_CONN_STOP;
        tal_wifi_station_disconnect();

        //! wait disconnect event
        tal_sw_timer_start(wifi->conn.timer, 1500, TAL_TIMER_ONCE);
        PR_DEBUG("auto connect stop %d", wifi->conn.stat);
        break;
    }

    tal_free(wifi_msg);
}

OPERATE_RET __netconn_wifi_connect(char *ssid, char *password)
{
    netmgr_conn_wifi_t *wifi = &s_netmgr_wifi;

    if (NETCONN_WIFI_CONN_LINKUP == wifi->conn.stat) {
        return OPRT_OK;
    }

    netmgr_wifi_msg_t *wifi_msg = tal_malloc(sizeof(netmgr_wifi_msg_t));
    if (NULL == wifi_msg) {
        return OPRT_MALLOC_FAILED;
    }

    wifi_msg->type = NETCONN_WIFI_MSG_CONNECT;
    wifi_msg->handle = wifi;
    return tal_workq_schedule(WORKQ_SYSTEM, __netconn_wifi_connect_process, wifi_msg);
}

OPERATE_RET __netconn_wifi_disconnect(void)
{
    netmgr_conn_wifi_t *wifi = &s_netmgr_wifi;

    if (NETCONN_WIFI_CONN_STOP == wifi->conn.stat ||
        NETCONN_WIFI_CONN_REDAY == wifi->conn.stat) {
        return OPRT_OK;
    }

    netmgr_wifi_msg_t *wifi_msg = tal_malloc(sizeof(netmgr_wifi_msg_t));
    if (NULL == wifi_msg) {
        return OPRT_MALLOC_FAILED;
    }

    wifi_msg->type   = NETCONN_WIFI_MSG_DISCONNECT;
    wifi_msg->handle = wifi;

    //! 在工作队列执行
    return tal_workq_schedule(WORKQ_SYSTEM, __netconn_wifi_connect_process, wifi_msg);
}

STATIC VOID __netconn_wifi_event(WF_EVENT_E event, VOID *arg)
{
    netmgr_conn_wifi_t *wifi = &s_netmgr_wifi;
    PR_NOTICE("wifi status changed to %d, old stat: %d", event, wifi->conn.stat);

    tal_sw_timer_stop(wifi->conn.timer);
    if (event == WFE_CONNECTED) {
        PR_DEBUG("wifi connected in stat %d", wifi->conn.stat);
        wifi->conn.count = 0;
        wifi->conn.stat  = NETCONN_WIFI_CONN_LINKUP;
        wifi->base.status = NETMGR_LINK_UP;
    } else {
        //! faild or disconnect auto connect
        if (NETCONN_WIFI_CONN_CHECK == wifi->conn.stat ||
            NETCONN_WIFI_CONN_WAIT  == wifi->conn.stat) {
            PR_DEBUG("wifi connect wait %d-%d", wifi->conn.count, wifi->conn.table[wifi->conn.count]);
            tal_sw_timer_start(wifi->conn.timer, wifi->conn.table[wifi->conn.count] * 1000, TAL_TIMER_ONCE);
            if (wifi->conn.count < wifi->conn.table_size - 1) {
                wifi->conn.count++;
            }
            wifi->conn.stat = NETCONN_WIFI_CONN_WAIT;
        } else if (NETCONN_WIFI_CONN_LINKUP == wifi->conn.stat) {
            wifi->conn.stat = NETCONN_WIFI_CONN_REDAY;
            __netconn_wifi_connect(wifi->conn.wifi_conn_info.ssid, wifi->conn.wifi_conn_info.pswd);
        }

        wifi->base.status = NETMGR_LINK_DOWN;
    }

    // notify netmgr the wifi connection status changed
    if (wifi->base.event_cb) {
        wifi->base.event_cb(NETCONN_WIFI, wifi->base.status);
    }

    return;
}

STATIC VOID __netconn_wifi_conn_timer(TIMER_ID timer_id, void *arg)
{
    netmgr_conn_wifi_t *wifi = (netmgr_conn_wifi_t *)arg;

    PR_DEBUG("auto conn timeout cnt %d, stat %d", wifi->conn.count, wifi->conn.stat);
    if (NETCONN_WIFI_CONN_WAIT == wifi->conn.stat) {
        __netconn_wifi_connect(wifi->conn.wifi_conn_info.ssid, wifi->conn.wifi_conn_info.pswd);
        wifi->conn.stat = NETCONN_WIFI_CONN_CHECK;
    } else if (NETCONN_WIFI_CONN_STOP == wifi->conn.stat) {
        wifi->conn.stat = NETCONN_WIFI_CONN_REDAY;
    } else if (NETCONN_WIFI_CONN_CHECK == wifi->conn.stat) { 
        PR_DEBUG("wifi connect wait %d-%d", wifi->conn.count, wifi->conn.table[wifi->conn.count]);
        tal_sw_timer_start(wifi->conn.timer, wifi->conn.table[wifi->conn.count] * 1000, TAL_TIMER_ONCE);
        if (wifi->conn.count < wifi->conn.table_size - 1) {
            wifi->conn.count++;
        }
        wifi->conn.stat = NETCONN_WIFI_CONN_WAIT;
    }
}

OPERATE_RET __netconn_wifi_info_set(netconn_wifi_info_t *info)
{
    CHAR_T netinfo[128];
    sprintf(netinfo, "{\"s\":\"%s\",\"p\":\"%s\"}", info->ssid, info->pswd);
    PR_DEBUG("netinfo %s", netinfo);

    return tal_kv_set("netinfo", (const uint8_t *)netinfo, strlen(netinfo));
}

OPERATE_RET __netconn_wifi_info_get(netconn_wifi_info_t *info)
{
    OPERATE_RET rt = 0;
    size_t   length = 0;
    uint8_t *netinfo  = NULL;

    TUYA_CALL_ERR_RETURN(tal_kv_get("netinfo", &netinfo, &length));
    
    cJSON *json = cJSON_Parse((const char *)netinfo);
    TUYA_CHECK_NULL_GOTO(json, err_exit);
    PR_DEBUG("netinfo %s", cJSON_PrintUnformatted(json));

    cJSON *s = cJSON_GetObjectItem(json, "s");
    TUYA_CHECK_NULL_GOTO(s, err_exit);

    cJSON *p = cJSON_GetObjectItem(json, "p");
    TUYA_CHECK_NULL_GOTO(p, err_exit);

    strcpy(info->ssid, s->valuestring);
    strcpy(info->pswd, p->valuestring);
    cJSON_Delete(json);
    tal_kv_free(netinfo);
    return OPRT_OK;

err_exit:
    if (netinfo) {
        tal_kv_free(netinfo);
        netinfo = NULL;
    }

    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }
    return OPRT_CJSON_PARSE_ERR;    
}

OPERATE_RET __netconn_wifi_netcfg_finish(INT_T type, netcfg_info_t *info)
{
    netmgr_conn_wifi_t *netmgr_wifi = &s_netmgr_wifi;

    // save wifi info
    netmgr_wifi->netcfg.netcfg_finish = TRUE;
    memcpy(netmgr_wifi->conn.wifi_conn_info.token, info->token, info->t_len);
    memcpy(netmgr_wifi->conn.wifi_conn_info.ssid, info->ssid, info->s_len);
    memcpy(netmgr_wifi->conn.wifi_conn_info.pswd, info->passwd, info->p_len);
    __netconn_wifi_info_set(&netmgr_wifi->conn.wifi_conn_info);
    PR_NOTICE("netcfg finished,  ssid %s, passwd %s, token %s", netmgr_wifi->conn.wifi_conn_info.ssid, netmgr_wifi->conn.wifi_conn_info.pswd, netmgr_wifi->conn.wifi_conn_info.token);
    // notify netcfg finished and save 
    tal_semaphore_post(netmgr_wifi->netcfg.netcfg_sem);

    return OPRT_OK;
}

OPERATE_RET __netconn_activate_token_get(CONST tuya_iot_config_t *config, tuya_binding_info_t* binding)
{
    int rt = OPRT_OK;

    netmgr_conn_wifi_t *netmgr_wifi = &s_netmgr_wifi;
    tal_semaphore_create_init(&netmgr_wifi->netcfg.netcfg_sem, 0, 1);

    // init netcfg
    netcfg_init();
    if (netmgr_wifi->netcfg.netcfg_mode & TUYA_NETMGR_NETCFG_AP) {
        ap_netcfg_init(&netmgr_wifi->netcfg.netcfg_args);
        netcfg_start(NETCFG_TUYA_WIFI_AP, __netconn_wifi_netcfg_finish, NULL);
    }

#ifdef ENABLE_BLUETOOTH    
    if (netmgr_wifi->netcfg.netcfg_mode & TUYA_NETMGR_NETCFG_BLE) {
        ble_netcfg_init(&netmgr_wifi->netcfg.netcfg_args);
        netcfg_start(NETCFG_TUYA_BLE, __netconn_wifi_netcfg_finish, NULL);
    }
#endif

    // wait until netcfg timeout
    tal_semaphore_wait(netmgr_wifi->netcfg.netcfg_sem, (netmgr_wifi->netcfg.netcfg_timeout < 60*1000) ? SEM_WAIT_FOREVER : netmgr_wifi->netcfg.netcfg_timeout);

    // netcfg success
    if (netmgr_wifi->netcfg.netcfg_finish) {
        memcpy(binding->region,     netmgr_wifi->conn.wifi_conn_info.token, REGION_LEN);
        memcpy(binding->token,      netmgr_wifi->conn.wifi_conn_info.token+REGION_LEN, TOKEN_LEN);
        memcpy(binding->regist_key, netmgr_wifi->conn.wifi_conn_info.token+REGION_LEN+TOKEN_LEN, REGIST_KEY_LEN);

        // wifi connect
        __netconn_wifi_connect(netmgr_wifi->conn.wifi_conn_info.ssid, netmgr_wifi->conn.wifi_conn_info.pswd);
    } else {
        PR_NOTICE("netcfg timeout!");
    }

    // stop all netcfg 
    netcfg_stop(NETCFG_STOP_ALL_CFG_MODULE);
    tal_semaphore_release(netmgr_wifi->netcfg.netcfg_sem);

    return rt;
}

OPERATE_RET netconn_wifi_open(VOID *config)
{
    netmgr_conn_wifi_t *netmgr_wifi = &s_netmgr_wifi;
    OPERATE_RET rt = OPRT_OK;

    // init            
    TUYA_CALL_ERR_RETURN(tal_wifi_init(__netconn_wifi_event));
    TUYA_CALL_ERR_RETURN(tal_wifi_set_country_code(netmgr_wifi->ccode));

    // create a connect timer --- it will triggered by netcfg or set ssid&pswd
    TUYA_CALL_ERR_RETURN(tal_sw_timer_create(__netconn_wifi_conn_timer, netmgr_wifi, &netmgr_wifi->conn.timer));

    tuya_iot_client_t *client = tuya_iot_client_get(); 
    if (client->is_activated) {
        tal_wifi_lp_disable();
        __netconn_wifi_info_get(&netmgr_wifi->conn.wifi_conn_info);
        __netconn_wifi_connect(netmgr_wifi->conn.wifi_conn_info.ssid, netmgr_wifi->conn.wifi_conn_info.pswd);

        // after reset, need to get a new token
        tuya_iot_token_get_port_register(client, __netconn_activate_token_get);
    }

    return rt;
}

OPERATE_RET netconn_wifi_close(VOID)
{
    return OPRT_OK;
}

OPERATE_RET netconn_wifi_set(netmgr_conn_config_type_e cmd, VOID *param)
{
    netmgr_conn_wifi_t *netmgr_wifi = &s_netmgr_wifi;
    tuya_iot_client_t *client = tuya_iot_client_get(); 
    OPERATE_RET rt = OPRT_OK;

    switch (cmd) {
    case NETCONN_CMD_PRI: // set pri will cause status change to reneg the active connection
        netmgr_wifi->base.pri = *(INT_T*)param;
        netmgr_wifi->base.event_cb(NETCONN_WIFI, netmgr_wifi->base.status);
        break;
    case NETCONN_CMD_MAC: // set mac to the station
        TUYA_CALL_ERR_RETURN(tal_wifi_set_mac(WF_STATION, (NW_MAC_S*)param));
        break;
    case NETCONN_CMD_SSID_PSWD: // set ssid&paswd will cause wifi disconnect&connect
        memcpy(&netmgr_wifi->conn.wifi_conn_info, (netconn_wifi_info_t*)param, sizeof(netconn_wifi_info_t));
        __netconn_wifi_connect(netmgr_wifi->conn.wifi_conn_info.ssid, netmgr_wifi->conn.wifi_conn_info.pswd);
        break;
    case NETCONN_CMD_COUNTRYCODE:
        memcpy(netmgr_wifi->ccode, (CHAR_T*)param,  strlen((CHAR_T*)param));
        TUYA_CALL_ERR_RETURN(tal_wifi_set_country_code(netmgr_wifi->ccode));
        break;
    case NETCONN_CMD_NETCFG:
        if (!client->is_activated) {
            netconn_wifi_netcfg_t *netcfg = (netconn_wifi_netcfg_t *)param;
            netmgr_wifi->netcfg.netcfg_mode = netcfg->netcfg_mode;
            netmgr_wifi->netcfg.netcfg_finish = FALSE;
            netmgr_wifi->netcfg.netcfg_timeout = netcfg->netcfg_timeout ;
            memcpy(&netmgr_wifi->netcfg.netcfg_args, &netcfg->netcfg_args, sizeof(netcfg_args_t));
            if (netmgr_wifi->netcfg.netcfg_mode&NETCFG_TUYA_BLE || netmgr_wifi->netcfg.netcfg_mode&NETCFG_TUYA_WIFI_AP) {
                tuya_iot_token_get_port_register(client, __netconn_activate_token_get);
            }
        }
        break;
    case NETCONN_CMD_CLOSE:
        __netconn_wifi_disconnect();
        break;

    case NETCONN_CMD_RESET:
        tal_kv_del("netinfo");
        netmgr_wifi->conn.stat = NETCONN_WIFI_CONN_STOP;
        memset(&netmgr_wifi->conn.wifi_conn_info, 0, sizeof(netmgr_wifi->conn.wifi_conn_info));
        tal_wifi_station_disconnect();
        break;

    default:
        return OPRT_NOT_SUPPORTED;
    }

    return OPRT_OK;
}

OPERATE_RET netconn_wifi_get(netmgr_conn_config_type_e cmd, void *param)
{
    netmgr_conn_wifi_t *netmgr_wifi = &s_netmgr_wifi;
    OPERATE_RET rt = OPRT_OK;

    switch (cmd) {
    case NETCONN_CMD_PRI: // set pri will cause status change to reneg the active connection
        netmgr_wifi->base.pri = *(INT_T*)param;
        netmgr_wifi->base.event_cb(NETCONN_WIFI, netmgr_wifi->base.status);
        break;
    case NETCONN_CMD_MAC: // set mac to the station
        TUYA_CALL_ERR_RETURN(tal_wifi_get_mac(WF_STATION, (NW_MAC_S*)param));
        break;
    case NETCONN_CMD_SSID_PSWD: // set ssid&paswd will cause wifi disconnect&connect
        memcpy((netconn_wifi_info_t*)param, &netmgr_wifi->conn.wifi_conn_info, sizeof(netconn_wifi_info_t));
        break;
    case NETCONN_CMD_COUNTRYCODE:
        memcpy((CHAR_T*)param, netmgr_wifi->ccode, strlen(netmgr_wifi->ccode));
        break;
    case NETCONN_CMD_IP:
        TUYA_CALL_ERR_RETURN(tal_wifi_get_ip(WF_STATION, (NW_IP_S*)param));
        break;    
    case NETCONN_CMD_NETCFG: {
        netconn_wifi_netcfg_t *netcfg = (netconn_wifi_netcfg_t *)param;
        netcfg->netcfg_mode = netmgr_wifi->netcfg.netcfg_mode;
        netcfg->netcfg_finish = netmgr_wifi->netcfg.netcfg_finish;
        netcfg->netcfg_timeout = netmgr_wifi->netcfg.netcfg_timeout;
        memcpy(&netcfg->netcfg_args, &netmgr_wifi->netcfg.netcfg_args, sizeof(netcfg_args_t));
        }
        break;
    case NETCONN_CMD_STATUS:
        *(netmgr_status_e*)param = netmgr_wifi->base.status;
        break;        
    default:
        return OPRT_NOT_SUPPORTED;
    }

    return OPRT_OK;
}
