
#include "tuya_error_code.h"
#include "tal_api.h"
#include "tuya_transporter.h"
#include "tcp_transporter.h"
#include "tal_network.h"

typedef struct tcp_transporter_inter_t {
    struct tuya_transporter_inter_t base;
    tuya_tcp_config_t config;
    int socket_fd;
} *tuya_tcp_transporter_t;

/**
* @brief tls transporter 连接
*
* @param[in] transporter: transporter handle
* @param[in] host: host to be connected
* @param[in] port: port of the host
* @param[in] timeout_ms: timeout in ms to connect
*
* @note: 通过transporter，连接到指定的host,port，在指定的超时时间timeout内
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_connect(tuya_transporter_t t, const char* host, int port, int timeout_ms)
{

    OPERATE_RET op_ret = OPRT_OK;
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;


    /*resolve ip addr of host*/
    TUYA_IP_ADDR_T hostaddr;
    op_ret = tal_net_gethostbyname(host, &hostaddr);
    if (op_ret != OPRT_OK) {
        PR_ERR("DNS parser host %s failed %d", host, op_ret);
        return  OPRT_MID_TRANSPORT_DNS_PARSED_FAILED;
    }

    tcp_transporter->socket_fd = tal_net_socket_create(PROTOCOL_TCP);
    if (tcp_transporter->socket_fd < 0) {
        op_ret = OPRT_MID_TRANSPORT_SOCK_CREAT_FAILED;
        goto err_out;
    }
    // reuse socket port
    if (tcp_transporter->config.isReuse && (OPRT_OK != tal_net_set_reuse(tcp_transporter->socket_fd))) {
        op_ret = OPRT_MID_TRANSPORT_SOCK_SET_REUSE_FAILED;
        goto err_out;
    }
    // disable Nagle Algorithm
    if (tcp_transporter->config.isDisableNagle && (OPRT_OK != tal_net_disable_nagle(tcp_transporter->socket_fd))) {
        op_ret = OPRT_MID_TRANSPORT_SOCK_SET_DISABLE_NAGLE_FAILED;
        goto err_out;
    }
    //keepalive ,idle time, interval, count setting
    if (tcp_transporter->config.isKeepAlive &&
        (OPRT_OK != tal_net_set_keepalive(tcp_transporter->socket_fd, TRUE, 
                            tcp_transporter->config.keepAliveIdleTime, 
                            tcp_transporter->config.keepAliveInterval, 
                            tcp_transporter->config.keepAliveCount))) {
        op_ret = OPRT_MID_TRANSPORT_SOCK_SET_KEEP_ALIVE_FAILED;
        goto err_out;
    }
    // block socket port
    if (tcp_transporter->config.isBlock && (OPRT_OK != tal_net_set_block(tcp_transporter->socket_fd, TRUE))) {
        op_ret = OPRT_MID_TRANSPORT_SOCK_SET_BLOCK_FAILED;
        goto err_out;
    }

    // socket bind random port
    if ((tcp_transporter->config.bindPort || tcp_transporter->config.bindAddr)  && 
        (OPRT_OK != tal_net_bind(tcp_transporter->socket_fd, tcp_transporter->config.bindAddr, tcp_transporter->config.bindPort))) { // socket bind port
        op_ret = OPRT_MID_TRANSPORT_SOCK_NET_BIND_FAILED;
        goto err_out;
    } else {
        PR_DEBUG("bind ip:%08x port:%d ok", tcp_transporter->config.bindAddr, tcp_transporter->config.bindPort);
    }

    /*
    socket send timeout
    rtos大部分都在lwip中关闭了send超时功能，linux上还在使用send/recv timeout，为了兼容两种情况，暂时不判断超时返回值
    */
    if (tcp_transporter->config.sendTimeoutMs && (OPRT_OK != tal_net_set_timeout(tcp_transporter->socket_fd, \
                                                  tcp_transporter->config.sendTimeoutMs, TRANS_SEND))) {
        // PR_DEBUG("socket fd set sendTimeout:%d failed",tcp_transporter->config.sendTimeoutMs);
        //op_ret = OPRT_MID_TRANSPORT_SOCK_SET_TIMEOUT_FAILED;
        //goto err_out;
    }
    /*
       socket recv timeout
       rtos大部分都在lwip中关闭了send超时功能，linux上还在使用send/recv timeout，为了兼容两种情况，暂时不判断超时返回值
       */
    if (tcp_transporter->config.recvTimeoutMs && (OPRT_OK != tal_net_set_timeout(tcp_transporter->socket_fd, \
                                                  tcp_transporter->config.recvTimeoutMs, TRANS_RECV))) {
        // op_ret = OPRT_MID_TRANSPORT_SOCK_SET_TIMEOUT_FAILED;
        // goto err_out;
    }

    if (tal_net_connect(tcp_transporter->socket_fd, hostaddr, port) < 0) {
        op_ret = OPRT_MID_TRANSPORT_TCP_CONNECD_FAILED;
        goto err_out;
    }


    return OPRT_OK;
err_out:
    if (tcp_transporter->socket_fd >= 0) {
        tal_net_close(tcp_transporter->socket_fd);
        tcp_transporter->socket_fd = -1;
    }
    return op_ret;//tbd
}

/**
* @brief transporter 相关的控制选项
*
* @param[in] transporter: transporter handle
*
* @param[in] cmd: 控制命令字
*
* @param[in] args: 命令字相关的参数
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void* args)
{
    OPERATE_RET ret = OPRT_OK;
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;

    switch (cmd) {
    case TUYA_TRANSPORTER_SET_TCP_CONFIG: {
        tuya_tcp_config_t* pConfig = (tuya_tcp_config_t*)args;
        if (pConfig) {
            memcpy(&tcp_transporter->config, pConfig, sizeof(tuya_tcp_config_t));
        } else {
            ret = OPRT_INVALID_PARM;
        }
        break;
    }
    case TUYA_TRANSPORTER_GET_TCP_SOCKET: {
        int* fd = (int*)args;
        if (fd && tcp_transporter->socket_fd >= 0) {
            *fd = tcp_transporter->socket_fd;
        } else {
            ret = OPRT_INVALID_PARM;
        }
        break;
    }
    default: {
        break;
    }
    }

    return ret;
}

/**
* @brief transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return int: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_close(tuya_transporter_t t)
{

    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;

    if (tcp_transporter->socket_fd >= 0) {
        PR_DEBUG("tcp transporter close socket fd:%d", tcp_transporter->socket_fd);
        tal_net_close(tcp_transporter->socket_fd);
    }
    tcp_transporter->socket_fd = -1;

    return OPRT_OK;
}

/**
* @brief transporter poll_read操作
*
* @param[in] transporter: transporter handle
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，poll读取状态，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 可读; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_poll_read(tuya_transporter_t t, int timeout_ms)
{
    int ret = 0;
    TUYA_FD_SET_T    readfd;
    TUYA_FD_SET_T    errfd;
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;
    int socket_fd = tcp_transporter->socket_fd;

    if (socket_fd < 0) {
        PR_ERR("socket fd:%d", socket_fd);
        return OPRT_INVALID_PARM;
    }

    tal_net_fd_zero(&readfd);
    tal_net_fd_zero(&errfd);
    tal_net_fd_set(socket_fd, &readfd);
    tal_net_fd_set(socket_fd, &errfd);

    ret = tal_net_select(socket_fd + 1, &readfd, NULL, &errfd, timeout_ms);
    if ((ret > 0) && tal_net_fd_isset(socket_fd, &errfd)) {
        ret = -1; // socket is fault
        PR_ERR("socket fd %d is fault", socket_fd);
    }

    return ret;
}

/**
* @brief transporter poll_write操作
*
* @param[in] transporter: transporter handle
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，poll可写状态，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 可写; =0:超时 <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_poll_write(tuya_transporter_t t, int timeout_ms)
{

    TUYA_FD_SET_T    writefd;
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;

    tal_net_fd_zero(&writefd);
    tal_net_fd_set(tcp_transporter->socket_fd, &writefd);

    return tal_net_select(tcp_transporter->socket_fd + 1, NULL, &writefd, NULL, timeout_ms);
}

/**
* @brief transporter read操作
*
* @param[in] websocket_transporter: transporter handle
* @param[inout] buf: 为了读取内容传入的buffer地址
* @param[in] len: 读取长度
* @param[in] timeout_ms: > 0:如果不可读，最多等待多久; 0:不可读时，立即返回。
*
* @note: 通过transporter handle，读取数据，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_read(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;
    if (tcp_transporter->socket_fd < 0) {
        PR_ERR("socket fd:%d", tcp_transporter->socket_fd);
        return OPRT_INVALID_PARM;
    }

    int ret = 0;
    if (timeout_ms > 0) {
        ret = tuya_tcp_transporter_poll_read(t, timeout_ms);
    }

    if (ret < 0) {
        return ret;
    }

    if ((timeout_ms > 0) && (ret == 0)) {
       return OPRT_RESOURCE_NOT_READY;
    }

    return tal_net_recv(tcp_transporter->socket_fd, buf, len);
}

/**
* @brief transporter write操作
*
* @param[in] transporter: transporter handle
* @param[in] buf: 为了读取内容传入的buffer地址
* @param[in] len: 读取长度
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，读取数据，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 写入的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_write(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{
    int ret = OPRT_COM_ERROR;
    tuya_tcp_transporter_t tcp_transporter = (tuya_tcp_transporter_t)t;
    if (tcp_transporter->socket_fd < 0) {
        PR_ERR("socket fd:%d", tcp_transporter->socket_fd);
        return OPRT_INVALID_PARM;
    }

    if (timeout_ms > 0 && tuya_tcp_transporter_poll_write(t, timeout_ms) <= 0) {
        return OPRT_RESOURCE_NOT_READY;
    }

    ret = tal_net_send(tcp_transporter->socket_fd, buf, len);
    if (ret < 0) {
        if ((tal_net_get_errno() == UNW_EINTR) || (tal_net_get_errno() == UNW_EAGAIN)) {
            tal_system_sleep(30);
            ret = tal_net_send(tcp_transporter->socket_fd, buf, len);
        }
    }

    return ret;
}

/**
* @brief 销毁transporter
*
* @param[in] transporter: tls_transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_destroy(tuya_transporter_t transporter)
{
    if (transporter) {
        tal_free(transporter);
    }
    return OPRT_OK;
}

/**
 * @brief 创建tcp transporter，并返回handler
 *
 * @note: 创建tcp transporter
 *
 *
 * @return tuya_tcp_transporter_t: transport handle
 */
tuya_transporter_t tuya_tcp_transporter_create()
{

    tuya_tcp_transporter_t t = tal_malloc(sizeof(struct tcp_transporter_inter_t));
    if (t == NULL) {
        PR_ERR("websocket transporter malloc fail");
        return NULL;
    }
    memset(t, 0, sizeof(struct tcp_transporter_inter_t));

    t->socket_fd = -1;

    tuya_transporter_set_func((tuya_transporter_t)&t->base, tuya_tcp_transporter_connect, tuya_tcp_transporter_close,
                              tuya_tcp_transporter_read, tuya_tcp_transporter_write,
                              tuya_tcp_transporter_poll_read, tuya_tcp_transporter_poll_write, tuya_tcp_transporter_destroy, tuya_tcp_transporter_ctrl);

    return &t->base;
}

