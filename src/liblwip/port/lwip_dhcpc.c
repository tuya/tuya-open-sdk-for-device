/**
 * @file lwip_dhcpc.c
 * @brief DHCP client implementation for LwIP.
 *
 * This file provides the DHCP client functionality, allowing devices to obtain IP addresses
 * and other network configuration details from a DHCP server. It includes mechanisms for
 * IP address renewal and rebinding, as well as support for fast DHCP based on predefined
 * network parameters.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "ethernetif.h"
#include "lwip_dhcpc.h"
#include "lwip/tcpip.h"
#include "lwip_init.h"
#include "tal_wifi.h"
#include "tal_api.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
// Maximum number of DHCP retries
#define MAX_DHCP_TRIES 5

// Number of runs for the TUYA DHCP CLIENT thread
#define TUYA_DHCP_CLIENT_THREAD_TRY_NUM 5
// Wait time for the TUYA DHCP CLIENT thread while loop
#define TUYA_DHCP_CLIENT_THREAD_WAITTIMES 10

// To randomize the TCP/UDP port for the DHCP CLIENT on each IP address acquisition,
// place the port randomization functions from tcp.c/udp.c here
#ifndef TCP_LOCAL_PORT_RANGE_START
#define TCP_LOCAL_PORT_RANGE_START        0xc000
#define TCP_LOCAL_PORT_RANGE_END          0xffff
#define TCP_ENSURE_LOCAL_PORT_RANGE(port) ((u16_t)(((port) & ~TCP_LOCAL_PORT_RANGE_START) + TCP_LOCAL_PORT_RANGE_START))
#endif

#ifndef UDP_LOCAL_PORT_RANGE_START
#define UDP_LOCAL_PORT_RANGE_START        0xc000
#define UDP_LOCAL_PORT_RANGE_END          0xffff
#define UDP_ENSURE_LOCAL_PORT_RANGE(port) ((u16_t)(((port) & ~UDP_LOCAL_PORT_RANGE_START) + UDP_LOCAL_PORT_RANGE_START))
#endif

/***********************************************************
*************************variable define********************
***********************************************************/
extern u16_t tcp_port;
extern u16_t udp_port;

static THREAD_HANDLE tuya_dhcp_clinet_thrd = NULL;
static dhcpc_cb_t dhcpc_cb = NULL;

#if LWIP_CONFIG_FAST_DHCP
extern u8_t is_fast_dhcp;
extern ip_addr_t offered_ip_addr;
extern ip_addr_t offered_gw_addr;
extern ip_addr_t dns_srv;
#endif /* LWIP_CONFIG_FAST_DHCP */

/***********************************************************
*************************function define********************
***********************************************************/
// Randomize the starting ports for TCP and UDP for each DHCP CLIENT startup
#if LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS
/**
 * @brief random the tcp port
 *
 * @param   void
 * @return  void
 */
static void tcp_randomize_local_port(void)
{
    static u8_t done = 0;

    if (!done) {
        done = 1;
        LWIP_SRAND();
        tcp_port = LWIP_RAND() % (TCP_LOCAL_PORT_RANGE_END - TCP_LOCAL_PORT_RANGE_START) + TCP_LOCAL_PORT_RANGE_START;
    }
}

/**
 * @brief random the udp port
 *
 * @param   void
 * @return  void
 */
static void udp_randomize_local_port(void)
{
    static u8_t done = 0;

    if (!done) {
        done = 1;
        LWIP_SRAND();
        udp_port = LWIP_RAND() % (UDP_LOCAL_PORT_RANGE_END - UDP_LOCAL_PORT_RANGE_START) + UDP_LOCAL_PORT_RANGE_START;
    }
}
#endif

/**
 * @brief dhcp client start to get ipaddr
 *
 * @param[in]      idx     the netif to start dhcp client
 * @return  the state of DHCP, detail see "TUYA_DHCP_CLIENT_STATUS"
 */
u8_t TUYA_LwIP_DHCP_CLIENT(TUYA_NETIF_TYPE idx, const TUYA_DHCP_CLIENT_STATUS dhcp_state)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    u32_t IPaddress;
    u8_t iptab[4];
    u8_t DHCP_state;
    struct netif *pnetif = NULL;
    struct dhcp *dhcp = NULL;

    DHCP_state = dhcp_state;

    if (idx > (NETIF_NUM - 1)) {
        idx = NETIF_NUM - 1;
    }

    pnetif = tuya_ethernetif_get_netif_by_index(idx);
#if LWIP_CONFIG_FAST_DHCP
    if (DHCP_state == DHCP_START && !is_fast_dhcp) {
#else
    if (DHCP_state == DHCP_START) {
#endif
        ip_addr_set_zero(&pnetif->ip_addr);
        ip_addr_set_zero(&pnetif->netmask);
        ip_addr_set_zero(&pnetif->gw);
    }

    dhcp = ((struct dhcp *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP));
    if (!netif_is_up(pnetif)) { // netif should be set up before doing dhcp
                                // request (in lwip v2.0.0)
        netif_set_up(pnetif);
    }

    for (;;) {
        // tuya_cli_printf("\n\r
        // ========DHCP_state:%d============\n\r",DHCP_state);
        switch (DHCP_state) {
        case DHCP_START: {

#if DHCP_CREATE_RAND_XID && defined(LWIP_SRAND)
            /* For each system startup, fill in a random seed with different
             * system ticks. */
            LWIP_SRAND();
#endif /* DHCP_CREATE_RAND_XID && defined(LWIP_SRAND) */
            dhcp_start(pnetif);
            dhcp = ((struct dhcp *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP));
            IPaddress = 0;
            DHCP_state = DHCP_WAIT_ADDRESS;
        } break;

        case DHCP_WAIT_ADDRESS: {

#include "lwip/prot/dhcp.h"
            if ((dhcp_state_enum_t)dhcp->state == DHCP_STATE_OFF) {
                IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
                IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
                // os_printf(LM_APP, LL_INFO, "\n\rTUYA_LwIP_DHCP_CLIENT: dhcp
                // stop.");
                return DHCP_STOP;
            }

            /* Read the new IP address */
            IPaddress = ip_addr_get_ip4_u32(netif_ip_addr4(pnetif));
            if (IPaddress != 0) {
#if LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS
                tcp_randomize_local_port();
                udp_randomize_local_port();
#endif
                DHCP_state = DHCP_ADDRESS_ASSIGNED;

                /* Stop DHCP */
                // dhcp_stop(pnetif);  /* can not stop, need to renew, Robbie*/

                iptab[0] = (uint8_t)(IPaddress >> 24);
                iptab[1] = (uint8_t)(IPaddress >> 16);
                iptab[2] = (uint8_t)(IPaddress >> 8);
                iptab[3] = (uint8_t)(IPaddress);
                // os_printf(LM_APP, LL_INFO, "\n\rInterface %d IP address :
                // %d.%d.%d.%d", idx, iptab[3], iptab[2], iptab[1], iptab[0]);

                if (dhcpc_cb) {
                    dhcpc_cb(IPaddress, ip_addr_get_ip4_u32(netif_ip_netmask4(pnetif)),
                             ip_addr_get_ip4_u32(netif_ip_gw4(pnetif)));
                }

                return DHCP_ADDRESS_ASSIGNED;
            } else {
                /* DHCP timeout */
                if (dhcp->tries > MAX_DHCP_TRIES) {
                    DHCP_state = DHCP_TIMEOUT;
                    /* Stop DHCP */
                    dhcp_stop(pnetif);
#if LWIP_DHCPC_STATIC_IPADDR_ENABLE
                    /* Static address used */
                    IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
                    IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                    IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                    netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
                    iptab[0] = IP_ADDR3;
                    iptab[1] = IP_ADDR2;
                    iptab[2] = IP_ADDR1;
                    iptab[3] = IP_ADDR0;
#endif
                    // os_printf(LM_APP, LL_INFO, "\n\rInterface %d DHCP CLIENT
                    // timeout", idx); os_printf(LM_APP, LL_INFO, "\n\rStatic IP
                    // address : %d.%d.%d.%d", iptab[3], iptab[2], iptab[1],
                    // iptab[0]);
                    tuya_wpa_supp_evt_handle(WFE_CONNECT_FAILED, NULL);
                    return DHCP_TIMEOUT;
                }
            }
        } break;
        case DHCP_RELEASE_IP:
            // os_printf(LM_APP, LL_INFO, "\n\rTUYA_LwIP_DHCP_CLIENT: Release
            // ip");
            dhcp_release(pnetif);
            return DHCP_RELEASE_IP;
        case DHCP_STOP:
            // os_printf(LM_APP, LL_INFO, "\n\rTUYA_LwIP_DHCP_CLIENT: dhcp
            // stop.");
            dhcp_stop(pnetif);
            return DHCP_STOP;
        default:
            break;
        }
        tal_system_sleep(TUYA_DHCP_CLIENT_THREAD_WAITTIMES);
    }
}

/**
 * @brief dhcp client start to get ipaddr
 *
 * @param[in]      pArg     input arg
 * @return  void
 */
static void tuya_dhcp_client_thread(void *pArg)
{
    u8_t ret = 0;
    u8_t remain = TUYA_DHCP_CLIENT_THREAD_TRY_NUM;
#if LWIP_CONFIG_FAST_DHCP
    ty_dhcpc_params_s *dhcp_params = (ty_dhcpc_params_s *)pArg;
    if (NULL != dhcp_params) {
        is_fast_dhcp = dhcp_params->fast_dhcp;
        //#if LWIP_IPV6
        //#else
        offered_ip_addr.addr = dhcp_params->ip.addr;
        offered_gw_addr.addr = dhcp_params->gw.addr;
        //#endif
    }
#endif /* LWIP_CONFIG_FAST_DHCP */

    while (remain) {
        ret = TUYA_LwIP_DHCP_CLIENT(NETIF_STA_IDX, DHCP_START);
        if (ret == DHCP_ADDRESS_ASSIGNED) {
            break;
        }
        remain--;
    }

    if (0 == remain) {
        tuya_wpa_supp_evt_handle(WFE_CONNECT_FAILED, NULL);
    } else {
        tuya_wpa_supp_evt_handle(WFE_CONNECTED, NULL);
    }
    tal_thread_delete(tuya_dhcp_clinet_thrd);
    tuya_dhcp_clinet_thrd = NULL;
#if LWIP_CONFIG_FAST_DHCP
    is_fast_dhcp = 0;
    offered_ip_addr.addr = 0;
    offered_gw_addr.addr = 0;
    tal_free(pArg);
#endif /* LWIP_CONFIG_FAST_DHCP */
}

/**
 * @brief start dhcp client thread to get ip for WIFI STATION mode
 *
 * @param      void
 * @return    OPRT_OK:success other:fail
 */
#if LWIP_CONFIG_FAST_DHCP
int tuya_dhcp_client_start(ty_dhcpc_params_s *dhcpc_params)
#else
int tuya_dhcp_client_start(void)
#endif /* LWIP_CONFIG_FAST_DHCP */
{
    int op_ret = OPRT_OK;

    THREAD_CFG_T thread_cfg = {
        .stackDepth = DHCPC_THREAD_STACKSIZE,
        .priority = DHCPC_THREAD_PRIO,
        .thrdname = "tuya_dhcp_client",
    };
#if LWIP_CONFIG_FAST_DHCP
    ty_dhcpc_params_s *pdhcpc_params;
#endif /* LWIP_CONFIG_FAST_DHCP */

    if (NULL == tuya_dhcp_clinet_thrd) {
#if LWIP_CONFIG_FAST_DHCP
        pdhcpc_params = (ty_dhcpc_params_s *)tal_malloc(sizeof(ty_dhcpc_params_s));
        if (NULL == pdhcpc_params) {
            return OPRT_MALLOC_FAILED;
        }
        memcpy(pdhcpc_params, dhcpc_params, sizeof(ty_dhcpc_params_s));
        op_ret = tal_thread_create_and_start(&tuya_dhcp_clinet_thrd, NULL, NULL, tuya_dhcp_client_thread, pdhcpc_params,
                                             &thread_cfg);
#else
        op_ret =
            tal_thread_create_and_start(&tuya_dhcp_clinet_thrd, NULL, NULL, tuya_dhcp_client_thread, NULL, &thread_cfg);
#endif /* LWIP_CONFIG_FAST_DHCP */
        if (OPRT_OK != op_ret) {
#if LWIP_CONFIG_FAST_DHCP
            tal_free(pdhcpc_params);
#endif /* LWIP_CONFIG_FAST_DHCP */
            return op_ret;
        }
    }
    return op_ret;
}

void dhcpc_set_new_lease_cb(dhcpc_cb_t cb)
{
    dhcpc_cb = cb;
}
