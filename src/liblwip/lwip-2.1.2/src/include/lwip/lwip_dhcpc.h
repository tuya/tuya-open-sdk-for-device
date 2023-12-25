/**
 * @file lwip_dhcpc.h
 * @brief LWIP dhcp client初始化封装
 *
 * @copyright copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */

#ifndef __LWIP_DHCPC_H
#define __LWIP_DHCPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/tcpip.h"
#include "lwip/init.h" //for lwip version control
#include "lwip/err.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
/* state machine of dhcp client */
typedef enum {
    DHCP_START = 0,          //启动DHCP CLIENT
    DHCP_WAIT_ADDRESS,       //IP地址获取中
    DHCP_ADDRESS_ASSIGNED,   //已经获取到IP地址
    DHCP_RELEASE_IP,         //释放IP地址
    DHCP_STOP,               //停止DHCP CLIENT
    DHCP_TIMEOUT             //DHCP获取IP地址超时
} TUYA_DHCP_CLIENT_STATUS;

#if LWIP_CONFIG_FAST_DHCP
typedef struct ty_dhcpc_params_ {
    unsigned char fast_dhcp;
    ip_addr_t  ip;
    ip_addr_t  mask;
    ip_addr_t  gw;
	ip_addr_t  dns_srv[DNS_MAX_SERVERS];
} ty_dhcpc_params_s;
#endif /* LWIP_CONFIG_FAST_DHCP */

typedef void (*dhcpc_cb_t)(unsigned int ip, unsigned int mask, unsigned int gw);

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/


/**
 * @brief start dhcp client thread to get ip for WIFI STATION mode
 *
 * @param     void
 * @return    OPRT_OS_ADAPTER_OK:success other:fail
 */
#if LWIP_CONFIG_FAST_DHCP
int tuya_dhcp_client_start(ty_dhcpc_params_s *dhcpc_params);
#else
int tuya_dhcp_client_start(void);
#endif /* LWIP_CONFIG_FAST_DHCP */
void dhcpc_set_new_lease_cb(dhcpc_cb_t cb);

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_DHCPC_H */
