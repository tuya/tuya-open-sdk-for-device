/**
 * @file lwip_init.c
 * @brief TUYA LWIP初始化
 *
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
//#include "oshal.h"

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "ethernetif.h"
#include "lwip_init.h"
#include "lwip/tcpip.h"
#ifdef TUYA_SDK_CLI_ADAPTER
#include "tuya_cli_adapt.h"
#endif
#include "tal_wifi.h"
#include "tal_log.h"

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/**
 * @brief tuya lwip init
 *
 * @param     void
 * @return    void
 */
extern void cli_printf(const char *f, ...);

void TUYA_LwIP_Init(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    s8_t idx = 0;
    struct netif *pnetif = NULL;

    //wifi的STATIO/AP必须对应LWIP的netif0/1
    if((NETIF_AP_IDX != WF_AP)||(NETIF_STA_IDX != WF_STATION)){
        //os_printf(LM_APP, LL_INFO, "wifi station/ap is not match netif0/1\n");
    }

    //lwip内核和资源初始化
    tcpip_init(NULL, NULL);

    //初始化netif，设置IP地址和名称
    for (idx = 0; idx < NETIF_NUM; idx++) {
#if LWIP_DHCPC_STATIC_IPADDR_ENABLE         
        if (idx == 0) {
            IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
            IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
        } else {
            IP4_ADDR(ip_2_ip4(&ipaddr), AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
            IP4_ADDR(ip_2_ip4(&netmask), AP_NETMASK_ADDR0, AP_NETMASK_ADDR1 , AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
            IP4_ADDR(ip_2_ip4(&gw), AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);
        }
#else
        IP4_ADDR(ip_2_ip4(&ipaddr), 0, 0, 0, 0);
        IP4_ADDR(ip_2_ip4(&netmask), 0, 0, 0, 0);
        IP4_ADDR(ip_2_ip4(&gw), 0, 0, 0, 0);
#endif
        pnetif = tuya_ethernetif_get_netif_by_index(idx);
        pnetif->name[0] = 'r';
        pnetif->name[1] = '0' + idx;

        netif_add(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw), NULL, &tuya_ethernetif_init, &tcpip_input);

        PR_DEBUG("interface %d is initialized", idx);
    }

    //设置STATION模式的netif为默认netif
    netif_set_default(tuya_ethernetif_get_netif_by_index(NETIF_STA_IDX));

    /***************************************/
    /* 注意: netif_set_up/down在wifi中操作 */
    /***************************************/
    for (idx = 0; idx < NETIF_NUM; idx++) {
        pnetif = tuya_ethernetif_get_netif_by_index(idx);
        if (pnetif) {
            netif_set_up(pnetif);
        }
    }
}

