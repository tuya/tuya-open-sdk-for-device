
#include "tuya_error_code.h"
#include "tal_log.h"
#include "tuya_transporter.h"
#include "tcp_transporter.h"
#include "tls_transporter.h"
#include "tal_network.h"
#include "tal_memory.h"
#include "tuya_tls.h"

typedef struct tls_transporter_inter_t {
    struct tuya_transporter_inter_t base;
    tuya_transporter_t tcp_transporter;
    tuya_tls_hander tls_handler;

    int socket_fd;
    int write_timeout;
    int read_timeout;
} *tuya_tls_transporter_t;

static int __tls_transporter_send_cb(void *ctx, const unsigned char *buf, size_t len)
{
    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)ctx;

    return  tuya_transporter_write(tls_transporter->tcp_transporter, (uint8_t*)buf, len, tls_transporter->write_timeout);
}
static int __tls_transporter_recv_cb(void *ctx, unsigned char *buf, size_t len)
{
    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)ctx;

    return tuya_transporter_read(tls_transporter->tcp_transporter, buf, len, tls_transporter->read_timeout);
}

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
* @return OPERATE_RET: 0: connect成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tls_transporter_connect(tuya_transporter_t t, const char* host, int port, int timeout_ms)
{

    OPERATE_RET op_ret = OPRT_OK;
    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    op_ret = tuya_transporter_connect(tls_transporter->tcp_transporter, host, port, timeout_ms);
    if (op_ret != OPRT_OK) {
        PR_ERR("tcp transporter connect failed,ret:-0x%x", op_ret * (-1));
        return op_ret;
    }

    tuya_transporter_ctrl(tls_transporter->tcp_transporter, TUYA_TRANSPORTER_GET_TCP_SOCKET, &tls_transporter->socket_fd);

    /* default config tls bio */
    tuya_tls_config_t* config = tuya_tls_config_get(tls_transporter->tls_handler);
    if (config->f_send == NULL || config->f_recv == NULL) {
        config->f_send = __tls_transporter_send_cb;
        config->f_recv = __tls_transporter_recv_cb;
        config->user_data = tls_transporter;
        tuya_tls_transporter_ctrl((tuya_transporter_t)tls_transporter, TUYA_TRANSPORTER_SET_TLS_CONFIG, config);
    }

    op_ret = tuya_tls_connect(tls_transporter->tls_handler, (char*)host, port,
                              tls_transporter->socket_fd, timeout_ms);
    if (OPRT_OK != op_ret) {
        PR_ERR("tls transporter connect err:%d", op_ret);
        tuya_tls_transporter_close(t);
        return OPRT_COM_ERROR;//tbd
    }

    return OPRT_OK;
}

/**
* @brief tls transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tls_transporter_close(tuya_transporter_t t)
{

    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    PR_DEBUG("tls transporter close socket fd:%d", tls_transporter->socket_fd);
    if (tls_transporter->socket_fd >= 0) {
        tuya_transporter_close(tls_transporter->tcp_transporter);
        tls_transporter->socket_fd = -1;
    } else {
        PR_DEBUG("tls transporter already closed");
        return OPRT_OK;
    }
    PR_DEBUG("tls transporter close tls handler:%p", tls_transporter->tls_handler);

    if (tls_transporter->tls_handler) {
        tuya_tls_disconnect(tls_transporter->tls_handler);
    }


    return OPRT_OK;
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
OPERATE_RET tuya_tls_transporter_read(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{

    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    tls_transporter->read_timeout = timeout_ms;

    return tuya_tls_read(tls_transporter->tls_handler, buf, len);

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
OPERATE_RET tuya_tls_transporter_write(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{

    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    tls_transporter->write_timeout = timeout_ms;
    return tuya_tls_write(tls_transporter->tls_handler, buf, len);
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
OPERATE_RET tuya_tls_transporter_poll_read(tuya_transporter_t t, int timeout_ms)
{

    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    return tuya_transporter_poll_read(tls_transporter->tcp_transporter, timeout_ms);
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
OPERATE_RET tuya_tls_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void* args)
{
    OPERATE_RET ret = OPRT_OK;
    tuya_tls_transporter_t tls_transporter = (tuya_tls_transporter_t)t;

    switch (cmd) {
    case TUYA_TRANSPORTER_SET_TLS_CERT: {
        char* cert_data = (char*)args;
        tuya_tls_config_t* config = tuya_tls_config_get(tls_transporter->tls_handler);
        config->ca_cert = cert_data;
        config->ca_cert_size = strlen(cert_data) + 1;
        tuya_tls_config_set(tls_transporter->tls_handler, config);
        break;
    }
    case TUYA_TRANSPORTER_SET_TLS_CONFIG: {
        tuya_tls_config_set(tls_transporter->tls_handler, (tuya_tls_config_t*)args);
        break;
    }
    case TUYA_TRANSPORTER_GET_TLS_CONFIG: {
        tuya_tls_config_t* config = tuya_tls_config_get(tls_transporter->tls_handler);
        tuya_tls_config_t **s = args;
        *s = (void *)config;
        break;
    }

    default: {
        ret = tuya_transporter_ctrl(tls_transporter->tcp_transporter, cmd, args);
        break;
    }
    }

    return ret;
}


/**
 * @brief 创建tls transporter，并返回handler
 *
 * @note: array是静态数组，默认size=3，用于创建一个trasport的集合体，用于保存各种协议的trasporter,
 *        比如,tcp transporter, tls transporter, websocket transporter
 *
 * @return tuya_transport_array_handle_t: transport handle
 */
tuya_transporter_t tuya_tls_transporter_create(void)
{

    tuya_tls_transporter_t t = tal_malloc(sizeof(struct tls_transporter_inter_t));
    if (t == NULL) {
        PR_ERR("websocket transporter malloc fail");
        return NULL;
    }
    memset(t, 0, sizeof(struct tls_transporter_inter_t));
    t->socket_fd = -1;

    tuya_transporter_set_func((tuya_transporter_t)&t->base, tuya_tls_transporter_connect, tuya_tls_transporter_close,
                              tuya_tls_transporter_read, tuya_tls_transporter_write,
                              tuya_tls_transporter_poll_read, NULL, tuya_tls_transporter_destroy, tuya_tls_transporter_ctrl);
    t->tcp_transporter = tuya_tcp_transporter_create();
    t->tls_handler = tuya_tls_connect_create();
    if (t->tls_handler == NULL) {
        tuya_tcp_transporter_destroy(t->tcp_transporter);
        return NULL;
    }

    /* Default config */
    tuya_tls_transporter_ctrl((tuya_transporter_t)&t->base, TUYA_TRANSPORTER_SET_TLS_CONFIG,
    &(tuya_tls_config_t) {
        .mode = TUYA_TLS_SERVER_CERT_MODE,
        .verify = true,
    });


    return &t->base;
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
OPERATE_RET tuya_tls_transporter_destroy(tuya_transporter_t transporter)
{
    tuya_tls_transporter_t t = (tuya_tls_transporter_t)transporter;
    tuya_tls_connect_destroy(t->tls_handler);
    t->tls_handler = NULL;

    tuya_tcp_transporter_destroy(t->tcp_transporter);
    if (t) {
        tal_free(t);
    }

    return OPRT_OK;
}

