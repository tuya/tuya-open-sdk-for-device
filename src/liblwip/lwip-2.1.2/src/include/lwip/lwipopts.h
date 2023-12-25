/**
 * @file lwipopts.h
 * @brief LWIP功能和资源配置
 *
 * @copyright copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
#ifndef LWIP_HDR_LWIPOPTS_H
#define LWIP_HDR_LWIPOPTS_H

//TUYA 平台宏
#include "tuya_iot_config.h"


/*
   ----------------------------------------------
   ---------- TUYA定制性能优化 options ----------
   ----------------------------------------------
*/
//TCP头申请不到PBUF_RAM空间后，使用PBUF_POOL空间。打开该宏可能导致数据包丢包（一般驱动适配层用POOL）
//#define LWIP_TCP_HEAD_ALLOC_REUSE_POLL      0

/*
tuya 线程的优先级功能，在应用发送消息后，会把当前应用的优先级升到最高，收到消息后再恢复到
正常，用于加快应用速度。防止广播包过多的时候，TCPIP线程一直处理广播，应用层消息得不到处理。

比如：涂鸦MCU仿真助手开开机的时候会发很多命令，会导致“__mq_ctrl_task”执行时间不足，recv得不
到及时调度，导致发送失败。此时可以开启次功能，解决次问题，或在“sf_obj_dp_report_async”中加
入“tuya_hal_system_sleep(100)”，减慢MCU仿真助手的调度频率，让mqtt线程可以及时调度。
*/
//#define LWIP_TUYA_APP_TCPIP_THREAD_SWITCH   1

//#define IPEF_TEST_OPTIMIZE   0

/*
   ----------------------------------------------
   --------------- 连接资源 options -------------
   ----------------------------------------------
*/
//LWIP内核UDP个数
//#define MEMP_NUM_UDP_PCB                6
//LWIP内核已完成连接的活跃TCP个数
//#define MEMP_NUM_TCP_PCB                10
//LWIP内核处于listen状态的TCP个数
//#define MEMP_NUM_TCP_PCB_LISTEN         5
//NETCONN的个数，位于socket下一层
//#define MEMP_NUM_NETCONN                (MEMP_NUM_UDP_PCB+MEMP_NUM_TCP_PCB+MEMP_NUM_TCP_PCB_LISTEN)


/*
   ----------------------------------------------
   ------------- 数据缓存资源 options -----------
   ----------------------------------------------
*/
//数据包缓存大小，如果发送的数据较多，且发送速度较大，需要修改此值
//#if IPEF_TEST_OPTIMIZE
//#define MEM_SIZE                        (64*512)
//#else
//#define MEM_SIZE                        (48*512)
//#endif


//PBUF数据的缓存个数，所有数据包共用，如果发送的数据较多，且发送速度较大，需要修改此值
//#define MEMP_NUM_PBUF                   100

//TCP发送缓存队列中TCP分配的个数
//#define MEMP_NUM_TCP_SEG                20

//数据包分片重组缓存个数
//#define IP_REASS_MAX_PBUFS              10


//POOL资源的个数，一般用在底层驱动收包
//#define PBUF_POOL_SIZE                  (2*IP_REASS_MAX_PBUFS)
//单个POOL池子的大小
//#define PBUF_POOL_BUFSIZE               500
//#define PBUF_LINK_ENCAPSULATION_HLEN    400

//TCP发送缓存，单位字节
//#define TCP_SND_BUF                     (5*TCP_MSS)

//TCP发送缓存，单位个
//#define TCP_SND_QUEUELEN                (4*TCP_SND_BUF/TCP_MSS)

//NETCON层接收数据包的缓存个数（UDP使用中，如果对端发送快于本地接收，这个缓存小会丢掉）
//#define MEMP_NUM_NETBUF                 8


/*
   ----------------------------------------------
   ---------- 操作系统调度资源 options ----------
   ----------------------------------------------
*/
//如果如果发送数据较多较快，需要加大以下的邮箱个数
//TCPIP邮箱的深度
//#define TCPIP_MBOX_SIZE                 6
//UDP接收邮箱深度
//#define DEFAULT_UDP_RECVMBOX_SIZE       6
//TCP接收邮箱深度
//#define DEFAULT_TCP_RECVMBOX_SIZE       6
//RAW接收邮箱深度
//#define DEFAULT_RAW_RECVMBOX_SIZE       6
//TCP accept邮箱深度
//#define DEFAULT_ACCEPTMBOX_SIZE         6

//LWIP定时器超时资源个数
//#define MEMP_NUM_SYS_TIMEOUT            10



/*
   ----------------------------------------------
   ------- TUYA固定业务（不需修改）options ------
   ----------------------------------------------
*/
//TCPIP线程名字
#define TCPIP_THREAD_NAME              "TUYA_TCPIP"
//是否给TCPIP线程加锁
//#define LWIP_TCPIP_CORE_LOCKING         0
//TCPIP线程是否支持超时
#define LWIP_TCPIP_TIMEOUT              1

//随机数生成，用于随机化TCP/UDP的端口号，保证每次开机后的端口号都不一样
#define LWIP_RAND()                     tkl_system_get_random(0xFFFFFFFF)
#define LWIP_SRAND()                    

//其他线程大小，不需要关心
#define DEFAULT_THREAD_STACKSIZE        500


/* ---------- MTU options ---------- */
#define LWIP_TUYA_MTU                   1500

/* ---------- TCP options ---------- */
#define LWIP_TCP                        1

//是否支持KEEPALIVE保活
#define LWIP_TCP_KEEPALIVE              1
//发送超时
//#define LWIP_SO_SNDTIMEO                1
//接收超时
//#define LWIP_SO_RCVTIMEO                1
//TCP最大分片个数
#define TCP_MSS                         (LWIP_TUYA_MTU - 40)
//TCP窗口大小
#define TCP_WND                         (5*TCP_MSS)

/* ---------- EAPOL ---------------- */
//#define LWIP_EAPOL_SUPPORT              1

/* ---------- UDP options ---------- */
#define LWIP_UDP                        1

/* ---------- ICMP options ---------- */
#define LWIP_ICMP                       1

/* ---------- ARP options ----------- */
#define LWIP_ARP                        1

/* ---------- DHCP options ---------- */
#define LWIP_DHCP                       1
//tuya 这里由底层文件上提到该头文件，改为用户可配置
#define DHCP_COARSE_TIMER_SECS          60
//tuya 增加DHCP请求的seconds_elapsed，用于标记客户端首次向服务器发出请求后的时间，按照协议标准需要这么做(从rtk上移植)
#define LWIP_DHCP_SECONDS_ELAPSE        1
//DHCP请求transaction identifier 随机化
#define DHCP_CREATE_RAND_XID            1


/* ---------- DNS options ---------- */
#define LWIP_DNS                        1

/* ---------- SO_REUSE options --------- */
#define SO_REUSE                        1

/* Support Multicast */
#define LWIP_IGMP                       1

/* ---------RAW option--------- */
#define LWIP_RAW                        1

/* ---------IPv6 option--------- */
//#define LWIP_IPV6                       0

/* ---------Socket options ---------*/
#define LWIP_SOCKET                     1
//tuya 立即发送RST断开连接
//#define LWIP_SO_LINGER                  1
//tuya 是否不等待unsend和unacked，直接发送RST关闭连接，需要配合socket so_linger机制
//#define LWIP_TUYA_SO_LINGER_RST         1
//使用LWIP提供的errno值
//#define LWIP_PROVIDE_ERRNO              1
//socket号从1开始
#define LWIP_SOCKET_OFFSET              1

/* ---------Enable Netconn API--------- */
#define LWIP_NETCONN                    1

/* ------initial port before dhcp------ */
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS  1

/*
   ----------------------------------------------
   ----------------- 调试 options ---------------
   ----------------------------------------------
*/
//tuya TUYA底层驱动数据包wireshark打印
#define LWIP_TUYA_PACKET_PRINT          0
    
    
//LWIP_DEBUG同时管理了LWIP_NOASSERT（没有断言）和LWIP_STATS，注释掉“#define LWIP_DEBUG”可以节约20K空间
//#define LWIP_DEBUG                      0
// Debug Options
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define IGMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define TIMERS_DEBUG                LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define SLIP_DEBUG                  LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF
#define AUTOIP_DEBUG                LWIP_DBG_OFF
#define DNS_DEBUG                   LWIP_DBG_OFF
#define IP6_DEBUG                   LWIP_DBG_OFF
    
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define UDP_LPC_EMAC                LWIP_DBG_OFF
#define ETHEAPOL_DEBUG              LWIP_DBG_ON
    
    
#ifdef LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK         1
#define MEMP_SANITY_CHECK           1
#define LWIP_DBG_TYPES_ON           LWIP_DBG_ON
#define LWIP_DBG_MIN_LEVEL          LWIP_DBG_LEVEL_ALL
#else
//tuya 8720cf如果需要用到断言的话，需要去掉"cc.h"中的"#define LWIP_PLATFORM_ASSERT(x) //do { if(!(x)) while(1); } while(0)"
#define LWIP_NOASSERT               0
#define LWIP_STATS                  0
#endif
    
//用于TUYA平台导入测试计数，用于监控状态
#if LWIP_STATS
//TCPIP线程统计计数需要用更大的线程
#define TCPIP_THREAD_STACKSIZE          (4096*2)
    
#define LINK_STATS                      1
#define ETHARP_STATS                    1
#define IP_STATS                        1
#define IPFRAG_STATS                    1
#define ICMP_STATS                      1
#define IGMP_STATS                      1
#define UDP_STATS                       1
#define TCP_STATS                       1
#define MEM_STATS                       1
#define MEMP_STATS                      1
#define SYS_STATS                       1
#define LWIP_STATS_DISPLAY              1
#define IP6_STATS                       1
#define ICMP6_STATS                     1
#define IP6_FRAG_STATS                  1
#define MLD6_STATS                      1
#define ND6_STATS                       1
#define MIB2_STATS                      1
#define MIB2_STATS                      1
    
//TUYA网口层监控函数，需要在驱动层适配
#define TUYA_ETHERNETIF_STATS           1
#endif


/*
   ----------------------------------------------
   --------------- LWIP 内部头文件 --------------
   ----------------------------------------------
*/
#include "lwip/init.h"


/*
   ----------------------------------------------
   -------------- 原厂适配 options --------------
   ----------------------------------------------
*/
//TCPIP线程的大小
//#define TCPIP_THREAD_STACKSIZE          (1024*4)
//TCPIP线程的优先级，此处的“11”来源于freertos的configMAX_PRIORITIES，由于SDK不能包含操作系统，此处用数值代替
//#define TCPIP_THREAD_PRIO               (11 - 2)

//DHCP CLIENT线程的大小
//#define DHCPC_THREAD_STACKSIZE          (1024*2)

//DHCP CLIENT线程优先级
//#define DHCPC_THREAD_PRIO               5

//使用信号量代替锁
#define LWIP_COMPAT_MUTEX               1

//字节对齐方式
#define MEM_ALIGNMENT                   4

#define LWIP_CHKSUM_ALGORITHM           3

#define LWIP_NETIF_API                  1

//#define LWIP_TX_PBUF_ZERO_COPY 		1

//#define LWIP_DHCP_CHECK_LINK_UP         0

//#define CONFIG_TUYA_SOCK_SHIM 1

//#define LWIP_NETIF_HOSTNAME 1

//#define LWIP_CHKSUM(buf, len) tkl_ethernetif_ip_chksum(buf, len)

#define LWIP_HOOK_IP4_ROUTE_SRC(s, d)         (void *)ip4_route_src_hook(s, d)

#define LWIP_DHCP_DISCOVER_RETRY_INTERVAL_1S 1

//#define SOCK_API_SYNC 1

//#define LWIP_NETCONN_SEM_PER_THREAD 1

#define MEMP_MEM_MALLOC 1

//#define LWIP_DHCPC_STATIC_IPADDR_ENABLE 0

#define LWIP_CONFIG_FAST_DHCP 1

#endif /* LWIP_HDR_LWIPOPTS_H */
