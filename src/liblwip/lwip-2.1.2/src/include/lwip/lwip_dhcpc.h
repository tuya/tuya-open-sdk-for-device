
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
    DHCP_START = 0,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED,
    DHCP_RELEASE_IP,
    DHCP_STOP,
    DHCP_TIMEOUT
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
