/**
* @file tal_network.h
* @brief Common process - Initialization
* @version 0.1
* @date 2020-11-09
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/
#ifndef __TAL_NETWORK_H__
#define __TAL_NETWORK_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* tuya sdk definition of 127.0.0.1 */
#define TY_IPADDR_LOOPBACK     ((UINT32_T)0x7f000001UL)
/* tuya sdk definition of 0.0.0.0 */
#define TY_IPADDR_ANY          ((UINT32_T)0x00000000UL)
/* tuya sdk definition of 255.255.255.255 */
#define TY_IPADDR_BROADCAST    ((UINT32_T)0xffffffffUL)

/**
* @brief Get error code of network
*
* @param void
*
* @note This API is used for getting error code of network.
*
* @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
*/
TUYA_ERRNO tal_net_get_errno(VOID);

/**
* @brief Add file descriptor to set
*
* @param[in] fd: file descriptor
* @param[in] fds: set of file descriptor
*
* @note This API is used to add file descriptor to set.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_fd_set(INT_T fd, TUYA_FD_SET_T* fds);

/**
* @brief Clear file descriptor from set
*
* @param[in] fd: file descriptor
* @param[in] fds: set of file descriptor
*
* @note This API is used to clear file descriptor from set.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_fd_clear(INT_T fd, TUYA_FD_SET_T* fds);

/**
* @brief Check file descriptor is in set
*
* @param[in] fd: file descriptor
* @param[in] fds: set of file descriptor
*
* @note This API is used to check the file descriptor is in set.
*
* @return TRUE or FALSE
*/
OPERATE_RET tal_net_fd_isset(INT_T fd, TUYA_FD_SET_T* fds);

/**
* @brief Clear all file descriptor in set
*
* @param[in] fds: set of file descriptor
*
* @note This API is used to clear all file descriptor in set.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_fd_zero(TUYA_FD_SET_T* fds);

//Add file descriptor to set
#define TAL_FD_SET(n,p)     tal_net_fd_set(n, p)
//Clear file descriptor from set
#define TAL_FD_CLR(n, p)    tal_net_fd_clear(n, p)
//Check file descriptor is in set
#define TAL_FD_ISSET(n,p)   tal_net_fd_isset(n,p)
//Clear all descriptor in set
#define TAL_FD_ZERO(p)      tal_net_fd_zero(p)

/**
* @brief Get available file descriptors
*
* @param[in] maxfd: max count of file descriptor
* @param[out] readfds: a set of readalbe file descriptor
* @param[out] writefds: a set of writable file descriptor
* @param[out] errorfds: a set of except file descriptor
* @param[in] ms_timeout: time out
*
* @note This API is used to get available file descriptors.
*
* @return >0 the count of available file descriptors, <=0 error.
*/
INT_T tal_net_select(CONST INT_T maxfd, TUYA_FD_SET_T *readfds, TUYA_FD_SET_T *writefds,
                                TUYA_FD_SET_T *errorfds, CONST UINT_T ms_timeout);

/**
* @brief Get no block file descriptors
*
* @param[in] fd: file descriptor
*
* @note This API is used to get no block file descriptors.
*
* @return >0 the count of no block file descriptors, <=0 error.
*/
INT_T tal_net_get_nonblock(CONST INT_T fd);

/**
* @brief Set block flag for file descriptors
*
* @param[in] fd: file descriptor
* @param[in] block: block flag
*
* @note This API is used to set block flag for file descriptors.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_block(CONST INT_T fd, CONST BOOL_T block);

/**
* @brief Close file descriptors
*
* @param[in] fd: file descriptor
*
* @note This API is used to close file descriptors.
*
* @return 0 on success. Others on error, please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_close(CONST INT_T fd);

/**
* @brief Create a tcp/udp socket
*
* @param[in] type: protocol type, tcp or udp
*
* @note This API is used for creating a tcp/udp socket.
*
* @return file descriptor
*/
INT_T tal_net_socket_create(CONST TUYA_PROTOCOL_TYPE_E type);

/**
* @brief Connect to network
*
* @param[in] fd: file descriptor
* @param[in] addr: address information of server
* @param[in] port: port information of server
*
* @note This API is used for connecting to network.
*
* @return 0 on success. Others on error, please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_connect(CONST INT_T fd, CONST TUYA_IP_ADDR_T addr, CONST UINT16_T port);

/**
* @brief Connect to network with raw data
*
* @param[in] fd: file descriptor
* @param[in] p_socket: raw socket data
* @param[in] len: data lenth
*
* @note This API is used for connecting to network with raw data.
*
* @return 0 on success. Others on error, please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_connect_raw(CONST INT_T fd, VOID_T *p_socket, CONST INT_T len);

/**
* @brief Bind to network
*
* @param[in] fd: file descriptor
* @param[in] addr: address information of server
* @param[in] port: port information of server
*
* @note This API is used for binding to network.
*
* @return 0 on success. Others on error, please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_bind(CONST INT_T fd, CONST TUYA_IP_ADDR_T addr, CONST UINT16_T port);

/**
* @brief Listen to network
*
* @param[in] fd: file descriptor
* @param[in] backlog: max count of backlog connection
*
* @note This API is used for listening to network.
*
* @return 0 on success. Others on error, please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_listen(CONST INT_T fd, CONST INT_T backlog);

/**
* @brief Send data to network
*
* @param[in] fd: file descriptor
* @param[in] buf: send data buffer
* @param[in] nbytes: buffer lenth
*
* @note This API is used for sending data to network
*
* @return >0 on num of send, <0 please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_send(CONST INT_T fd, CONST VOID_T *buf, CONST UINT_T nbytes);

/**
* @brief Send data to specified server
*
* @param[in] fd: file descriptor
* @param[in] buf: send data buffer
* @param[in] nbytes: buffer lenth
* @param[in] addr: address information of server
* @param[in] port: port information of server
*
* @note This API is used for sending data to network
*
* @return >0 on num of send, <0 please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_send_to(CONST INT_T fd, CONST VOID_T *buf, CONST UINT_T nbytes,
                                 CONST TUYA_IP_ADDR_T addr, CONST UINT16_T port);

/**
* @brief Accept the coming socket connection of the server fd
*
* @param[in] fd: file descriptor
* @param[in] addr: address information of server
* @param[in] port: port information of server
*
* @note This API is used for accepting the coming socket connection of the server fd.
*
* @return >0 the file descriptor, <=0 means failed
*/
INT_T tal_net_accept(CONST INT_T fd, TUYA_IP_ADDR_T *addr, UINT16_T *port);

/**
* @brief Receive data from network
*
* @param[in] fd: file descriptor
* @param[in] buf: receive data buffer
* @param[in] nbytes: buffer lenth
*
* @note This API is used for receiving data from network
*
* @return >0 on num of recv, <0 please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_recv(CONST INT_T fd, VOID_T *buf, CONST UINT_T nbytes);

/**
* @brief Receive data from network with need size
*
* @param[in] fd: file descriptor
* @param[in] buf: receive data buffer
* @param[in] nbytes: buffer lenth
* @param[in] nd_size: the need size
*
* @note This API is used for receiving data from network with need size
*
* @return >0 on success. Others on error
*/
INT_T tal_net_recv_nd_size(CONST INT_T fd, VOID_T *buf, CONST UINT_T buf_size, CONST UINT_T nd_size);

/**
* @brief Receive data from specified server
*
* @param[in] fd: file descriptor
* @param[in] buf: receive data buffer
* @param[in] nbytes: buffer lenth
* @param[in] addr: address information of server
* @param[in] port: port information of server
*
* @note This API is used for receiving data from specified server
*
* @return >0 on num of recv, <0 please refer to the error no of the target system
*/
TUYA_ERRNO tal_net_recvfrom(CONST INT_T fd, VOID_T *buf, CONST UINT_T nbytes,
                                   TUYA_IP_ADDR_T *addr, UINT16_T *port);

/**
* @brief Set timeout option of socket fd
*
* @param[in] fd: file descriptor
* @param[in] ms_timeout: timeout in ms
* @param[in] type: transfer type, receive or send
*
* @note This API is used for setting timeout option of socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_timeout(CONST INT_T fd, CONST INT_T ms_timeout, CONST TUYA_TRANS_TYPE_E type);

/**
* @brief Set buffer_size option of socket fd
*
* @param[in] fd: file descriptor
* @param[in] buf_size: buffer size in byte
* @param[in] type: transfer type, receive or send
*
* @note This API is used for setting buffer_size option of socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_bufsize(CONST INT_T fd, CONST INT_T buf_size, CONST TUYA_TRANS_TYPE_E type);

/**
* @brief Enable reuse option of socket fd
*
* @param[in] fd: file descriptor
*
* @note This API is used to enable reuse option of socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_reuse(CONST INT_T fd);

/**
* @brief Disable nagle option of socket fd
*
* @param[in] fd: file descriptor
*
* @note This API is used to disable nagle option of socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_disable_nagle(CONST INT_T fd);

/**
* @brief Enable broadcast option of socket fd
*
* @param[in] fd: file descriptor
*
* @note This API is used to enable broadcast option of socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_broadcast(CONST INT_T fd);

/**
* @brief Get address information by domain
*
* @param[in] domain: domain information
* @param[in] addr: address information
*
* @note This API is used for getting address information by domain.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_gethostbyname(CONST CHAR_T *domain, TUYA_IP_ADDR_T *addr);

/**
* @brief Set keepalive option of socket fd to monitor the connection
*
* @param[in] fd: file descriptor
* @param[in] alive: keepalive option, enable or disable option
* @param[in] idle: keep idle option, if the connection has no data exchange with the idle time(in seconds), start probe.
* @param[in] intr: keep interval option, the probe time interval.
* @param[in] cnt: keep count option, probe count.
*
* @note This API is used to set keepalive option of socket fd to monitor the connection.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_set_keepalive(INT_T fd, CONST BOOL_T alive, CONST UINT_T idle, CONST UINT_T intr, CONST UINT_T cnt);

/**
* @brief Get ip address by socket fd
*
* @param[in] fd: file descriptor
* @param[out] addr: ip address
*
* @note This API is used for getting ip address by socket fd.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_get_socket_ip(INT_T fd, TUYA_IP_ADDR_T *addr);


/**
* @brief Change ip string to address
*
* @param[in] ip_str: ip string
*
* @note This API is used to change ip string to address.
*
* @return ip address
*/
TUYA_IP_ADDR_T tal_net_str2addr(CONST CHAR_T *ip_str);

/**
* @brief Change ip address to string
*
* @param[in] ipaddr: ip address
*
* @note This API is used to change ip address(in host byte order) to string(in IPv4 numbers-and-dots(xx.xx.xx.xx) notion).
*
* @return ip string
*/
CHAR_T* tal_net_addr2str(TUYA_IP_ADDR_T ipaddr);

/**
* @brief Set socket options
*
* @param[in] fd: file descriptor
* @param[in] level: setting level
* @param[in] optname: the name of the option
* @param[in] optval: the value of option
* @param[in] optlen: the length of the option value
*
* @note This API is used for setting socket options.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_setsockopt(CONST INT_T fd, CONST TUYA_OPT_LEVEL level, CONST TUYA_OPT_NAME optname, CONST VOID_T *optval, CONST INT_T optlen);

/**
* @brief Get socket options
*
* @param[in] fd: file descriptor
* @param[in] level: getting level
* @param[in] optname: the name of the option
* @param[out] optval: the value of option
* @param[out] optlen: the length of the option value
*
* @note This API is used for getting socket options.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tal_net_getsockopt(CONST INT_T fd, CONST TUYA_OPT_LEVEL level, CONST TUYA_OPT_NAME optname, VOID_T *optval, INT_T *optlen);

#ifdef __cplusplus
}
#endif

#endif // __TAL_NETWORK_H__

