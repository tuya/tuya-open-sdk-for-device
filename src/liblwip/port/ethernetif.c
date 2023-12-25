/**
 * @file ethernetif.c
 * @brief LWIP网口相关
 *
 * @copyright copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"
#include "lwip/icmp.h"
#include "lwip/inet.h"
#include "netif/etharp.h"
#include "lwip/err.h"
#include "ethernetif.h"
#include "lwip_init.h"
#include "lwip/ethip6.h" //Add for ipv6
//#include "tal_network.h"
//#include "tuya_os_adapter_errcode.h"
#include "tkl_lwip.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#if LWIP_TUYA_PACKET_PRINT
#define TUYA_PACKET_PRINT(pbuf) tuya_ethernetif_packet_print(pbuf)
#else
#define TUYA_PACKET_PRINT(pbuf)
#endif

/***********************************************************
*************************variable define********************
***********************************************************/
/* network interface structure */
struct netif xnetif[NETIF_NUM];

#if LWIP_TUYA_PACKET_PRINT
/***********************************************************
*************************function define********************
***********************************************************/
/**
 * @brief ethernetif packet print, enable/disable by LWIP_TUYA_PACKET_PRINT
 *
 * @param[in]       p       the packet of pbuf
 * @return  void
 */
static void tuya_ethernetif_packet_print(struct pbuf *p)
{
    u32_t i, timeout, hour, minute, second, msecond;
    struct pbuf *q;

    timeout = sys_now() % 86400000;
    hour = timeout / 1000 / 60 / 60;
    minute = (timeout / 1000 / 60) % 60;
    second = (timeout / 1000) % 60;
    msecond = timeout % 1000;
    printf("+---------+---------------+----------+\r\n");
    printf("%02d:%02d:%02d,%d,000   ETHER\r\n", hour, minute , second, msecond);
    printf("|0   |");
    for (q = p; q != NULL; q = q->next) {
        for (i = 0; i < q->len; i++) {
            printf("%02x|", ((u8_t *)q->payload)[i]);
        }
    }
    printf("\r\n\n\n");
}
#endif /* LWIP_TUYA_PACKET_PRINT */

/**
 * @brief get netif by index
 *
 * @param[in]       net_if_idx    the num of netif index
 * @return  NULL: get netif fail   other: the point of netif
 */
struct netif *tuya_ethernetif_get_netif_by_index(const TUYA_NETIF_TYPE net_if_idx)
{
    if (net_if_idx > (NETIF_NUM - 1)) {
        return NULL;
    }

    return &xnetif[net_if_idx];
}

/**
 * @brief netif broadcast enable/disable
 *
 * @param[in]      enable    (1:enable the broadcast  0:disable the broadcast)
 * @return  void
 */
void tuya_ethernetif_broadcast_set(const TUYA_NETIF_TYPE net_if_idx, BOOL_T enable)
{
    struct netif *pnetif = NULL;

    pnetif = tuya_ethernetif_get_netif_by_index(net_if_idx);

    if(enable == TRUE) {
        pnetif->flags |= NETIF_FLAG_BROADCAST;
    } else {
        pnetif->flags &= ~NETIF_FLAG_BROADCAST;
    }
}

/**
 * @brief set netif ipaddr from lwip
 *
 * @param[in]       net_if_idx    index of netif
 * @param[out]      ip            ip of netif(ip gateway mask)
 * @return  void
 */
void tuya_ethernetif_set_ip(const TUYA_NETIF_TYPE net_if_idx, NW_IP_S *ip)
{
    struct netif *pnetif = NULL;
    u32_t ip_addr = 0;
    u32_t gw = 0;
    u32_t mask = 0;

    pnetif = tuya_ethernetif_get_netif_by_index(net_if_idx);

    ip_addr = inet_addr(ip->ip);
    gw = inet_addr(ip->gw);
    mask = inet_addr(ip->mask);

    IP4_ADDR(&pnetif->ip_addr, ip_addr&0xff, (ip_addr>>8)&0xff, (ip_addr>>16)&0xff, (ip_addr>>24)&0xff);
    IP4_ADDR(&pnetif->gw, gw&0xff, (gw>>8)&0xff, (gw>>16)&0xff, (gw>>24)&0xff);
    IP4_ADDR(&pnetif->netmask, mask&0xff, (mask>>8)&0xff, (mask>>16)&0xff, (mask>>24)&0xff);
}

/**
 * @brief get netif ipaddr from lwip
 *
 * @param[in]       net_if_idx    index of netif
 * @param[out]      ip            ip of netif(ip gateway mask)
 * @return  void
 */
void tuya_ethernetif_get_ip(const TUYA_NETIF_TYPE net_if_idx, NW_IP_S *ip)
{
    u32_t ip_address = 0;
    struct netif *pnetif = NULL;

    pnetif = tuya_ethernetif_get_netif_by_index(net_if_idx);

    // ip
    ip_address = pnetif->ip_addr.addr;
    sprintf(ip->ip, "%d.%d.%d.%d", (unsigned char)(ip_address), (unsigned char)(ip_address >> 8), \
            (unsigned char)(ip_address >> 16), (unsigned char)(ip_address >> 24));

    // gw
    ip_address = pnetif->gw.addr;
    sprintf(ip->gw, "%d.%d.%d.%d", (unsigned char)(ip_address), (unsigned char)(ip_address >> 8), \
            (unsigned char)(ip_address >> 16), (unsigned char)(ip_address >> 24));

    // submask
    ip_address = pnetif->netmask.addr;
    sprintf(ip->mask, "%d.%d.%d.%d", (unsigned char)(ip_address), (unsigned char)(ip_address >> 8), \
            (unsigned char)(ip_address >> 16), (unsigned char)(ip_address >> 24));
}

/**
 * @brief set netif's mac
 *
 * @param[in]       net_if_idx    index of netif
 * @param[in]       mac           mac to set
 * @return  int    OPRT_OK:success   other:fail
 */
int tuya_ethernetif_mac_set(const TUYA_NETIF_TYPE net_if_idx, NW_MAC_S *mac)
{
    struct netif *pnetif = NULL;
    u32_t i = 0;

    if(MAC_ADDR_LEN != NETIF_MAX_HWADDR_LEN){
        return OPRT_OS_ADAPTER_NOT_SUPPORTED;
    }

    pnetif = tuya_ethernetif_get_netif_by_index(net_if_idx);

    for (i = 0; i < MAC_ADDR_LEN; i++) {
        pnetif->hwaddr[i] = mac->mac[i];
    }

    return OPRT_OK;
}

/**
 * @brief get netif's mac
 *
 * @param[in]       net_if_idx    index of netif
 * @param[out]      mac           mac to set
 * @return  int    OPRT_OK:success   other:fail
 */
int tuya_ethernetif_mac_get(const TUYA_NETIF_TYPE net_if_idx, NW_MAC_S *mac)
{
    struct netif *pnetif = NULL;
    u32_t i = 0;

    if(MAC_ADDR_LEN != NETIF_MAX_HWADDR_LEN){
        return OPRT_OS_ADAPTER_NOT_SUPPORTED;
    }

    pnetif = tuya_ethernetif_get_netif_by_index(net_if_idx);

    for (i = 0; i < MAC_ADDR_LEN; i++) {
        mac->mac[i] = pnetif->hwaddr[i];
    }

    return OPRT_OK;
}

#if not_yet
/**
 * @brief netif check(check netif is up/down and ip is valid)
 *
 * @param   void
 * @return  int    OPRT_OK:netif is up and ip is valid
 */
int tuya_ethernetif_station_state_get(void)
{
    struct netif *pnetif = NULL;
    
    pnetif = tuya_ethernetif_get_netif_by_index(NETIF_STA_IDX);

    if (!netif_is_up(pnetif)) {
        return OPRT_OS_ADAPTER_COM_ERROR;
    }

    if (ip4_addr_isany_val(*(netif_ip_addr4(pnetif)))) {
        return OPRT_OS_ADAPTER_COM_ERROR;
    }

    return OPRT_OK;
}
#endif

/**
 * @brief ethernetif int
 *
 * @param[in]      netif     the netif to be inited
 * @return  void
 */
static void tuya_ethernet_init(struct netif *netif)
{

    /* set netif MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set netif maximum transfer unit */
    netif->mtu = LWIP_TUYA_MTU;

    /* Accept broadcast address and ARP traffic */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

#if LWIP_IGMP
    /* make LwIP_Init do igmp_start to add group 224.0.0.1 */
    netif->flags |= NETIF_FLAG_IGMP;
#endif

    /* Wlan interface is initialized later */
    tkl_ethernetif_init(netif);
}

/**
 * @brief ethernet interface sendout the pbuf packet
 *
 * @param[in]      netif     the netif to which to be inited
 * @return  err_t  SEE "err_enum_t" in "lwip/err.h" to see the lwip err(ERR_OK: SUCCESS other:fail)
 */
err_t tuya_ethernetif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    if (netif->name[1] == '0') {
        netif->hostname = "lwip0";
    } else if (netif->name[1] == '1') {
        netif->hostname = "lwip1";
    }
#endif /* LWIP_NETIF_HOSTNAME */

    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif
    //netif->linkoutput = tuya_ethernetif_output;
    netif->linkoutput = (netif_linkoutput_fn)tkl_ethernetif_output;
    
    /* initialize the hardware */
    tuya_ethernet_init(netif);

    etharp_init();

    return ERR_OK;
}

int tuya_ethernetif_get_ifindex_by_mac(NW_MAC_S *mac, TUYA_NETIF_TYPE *net_if_idx)
{
    int i;
    struct netif *netif;

    if (NULL == mac || NULL == net_if_idx) {
        return -1;
    }

    for (i = 0; i < NETIF_NUM; i++) {
        netif = &xnetif[i];
        if (NULL == netif) {
            continue;
        }
    
        if (memcmp(netif->hwaddr, mac->mac, 6) == 0) {
            *net_if_idx = i;
            break;
        }
    }

    return 0;
}
