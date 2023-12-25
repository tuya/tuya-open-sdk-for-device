
#include "netconn_wired.h"
#include "tal_api.h"
#include "tal_wired.h"

netmgr_conn_wired_t s_netmgr_wired = {
    .base={
        .pri=1,
        .type=NETCONN_WIRED,
        .open=netconn_wired_open,
        .close=netconn_wired_close,
        .get=netconn_wired_get,
        .set=netconn_wired_set
    }
};

/**
 * @brief a callback used to process the lowlayer event
 * 
 * @param event the tal wired event
 * @param arg 
 * @return STATIC 
 */
STATIC VOID  __netconn_wired_event(WIRED_STAT_E event)
{
    netmgr_conn_wired_t *netmgr_wired = &s_netmgr_wired;

    PR_NOTICE("wired status changed to %d, old stat: %d", event, netmgr_wired->base.status);
    netmgr_wired->base.status = (event == TKL_WIRED_LINK_UP)?NETMGR_LINK_UP:NETMGR_LINK_DOWN;

    // notify netmgr
    if (netmgr_wired->base.event_cb) {
        netmgr_wired->base.event_cb(NETCONN_WIRED, netmgr_wired->base.status);
    }

    return;
}

/**
 * @brief open wired connection
 * 
 * @param config: wired connection config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_open(VOID *config)
{
    OPERATE_RET rt = OPRT_OK;
    netmgr_conn_wired_t *netmgr_wired = &s_netmgr_wired;

    // open wired connection, default disconnect
    // memcpy(&netmgr_wired->config, config, sizeof(netmgr_wired->config));
    netmgr_wired->base.status = NETMGR_LINK_DOWN;
    TUYA_CALL_ERR_RETURN(tal_wired_set_status_cb(__netconn_wired_event));

    return rt;
}

/**
 * @brief close wired connection
 * 
 * @param config: wired connection config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_close()
{
    return OPRT_OK;
}

/**
 * @brief update wired connection
 * 
 * @param config: the new config
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_set(netmgr_conn_config_type_e cmd, void *param)
{
    netmgr_conn_wired_t *netmgr_wired = &s_netmgr_wired;
    OPERATE_RET rt = OPRT_OK;

    switch (cmd) {
    case NETCONN_CMD_PRI:
        netmgr_wired->base.pri = *(INT_T*)param;
        netmgr_wired->base.event_cb(NETCONN_WIRED, netmgr_wired->base.status);
        break;
    case NETCONN_CMD_MAC:
        TUYA_CALL_ERR_RETURN(tal_wired_set_mac((NW_MAC_S*)param));
        break;
    default:
        return OPRT_NOT_SUPPORTED;
    }

    return OPRT_OK;
}

/**
 * @brief get wired connection attribte
 * 
 * @param type 
 * @param cmd 
 * @param param 
 * @return OPERATE_RET 
 */
OPERATE_RET netconn_wired_get(netmgr_conn_config_type_e cmd, void *param)
{
    netmgr_conn_wired_t *netmgr_wired = &s_netmgr_wired;
    OPERATE_RET rt = OPRT_OK;
    
    switch (cmd) {
    case NETCONN_CMD_PRI:
        *(INT_T*)param = netmgr_wired->base.pri;
        break;
    case NETCONN_CMD_IP:        
        TUYA_CALL_ERR_RETURN(tal_wired_get_ip((NW_IP_S*)param));
        break;
    case NETCONN_CMD_MAC:
        TUYA_CALL_ERR_RETURN(tal_wired_get_mac((NW_MAC_S*)param));
        break;
    case NETCONN_CMD_STATUS:
        *(netmgr_status_e*)param = netmgr_wired->base.status;
        break;
    case NETCONN_CMD_CLOSE:
        break;
    default:
        return OPRT_NOT_SUPPORTED;
    }

    return OPRT_OK;    
}
