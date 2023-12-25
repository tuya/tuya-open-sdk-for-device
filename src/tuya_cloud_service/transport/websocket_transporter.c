
#include "tuya_cloud_types.h"
#include "tal_log.h"
#include "tuya_transporter.h"
#include "tuya_tls.h"
#include "tls_transporter.h"

#if defined(ENABLE_WEBSOCKET) && (ENABLE_WEBSOCKET==1)

#include "websocket_client.h"
#include "websocket_transporter.h"
typedef struct websocket_transporter_inter_t {
    struct tuya_transporter_inter_t base;
    char * path;
    char * scheme;
    websocket_client_handle_t ws_client;
    uint32_t readable_len;
    uint8_t* read_buffer;
    tuya_tcp_config_t tcpConfig;
    MUTEX_HANDLE mutex;/*保护readable_len和read_buffer*/
} *tuya_websocket_transporter_t;


/**
* @brief transporter 连接
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
OPERATE_RET websocket_transporter_connect(tuya_transporter_t t, char* host, int port, int timeout_ms)
{
    OPERATE_RET rt = OPRT_OK;

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;
    websocket_client_cfg_t cfg = {0};
    cfg.host = host;
    cfg.port = port;
    cfg.event_cb = NULL;

    cfg.priv_data = (void*)wst;
    cfg.scheme = wst->scheme;

    memcpy(&cfg.tcpConfig, &wst->tcpConfig, sizeof(wst->tcpConfig));

    TUYA_CALL_ERR_RETURN(websocket_client_init(&wst->ws_client, &cfg));
    TUYA_CALL_ERR_RETURN(websocket_client_set_path(wst->ws_client, wst->path));
    if (OPRT_OK != websocket_client_open(wst->ws_client, timeout_ms)) {
        websocket_client_close(wst->ws_client);
    }

    return OPRT_OK;

}


/**
* @brief transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET websocket_transporter_close(tuya_transporter_t t)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

//  目前在mqtt下使用，总是在网络不通时调用websocket_client_disconnect，会阻塞，导致
//  close无法退出，暂时先注释该流程
//    rt = websocket_client_disconnect(wst->ws_client);
//    if(rt != OPRT_OK){
//        PR_ERR("websocket client disconnect err,rt:%d",rt);
//    }

    if (wst->ws_client) {
        websocket_client_close(wst->ws_client);
        Free(wst->ws_client);
        wst->ws_client = NULL;
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
OPERATE_RET websocket_transporter_read(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{
    int ret = 0;
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    ret = websocket_client_poll(wst->ws_client, timeout_ms);
    if (ret <= 0) {
        return OPRT_TIMEOUT;
    }

    tal_mutex_lock(wst->mutex);
    int read_len = websocket_client_read(wst->ws_client, buf, len);
    tal_mutex_unlock(wst->mutex);
    PR_TRACE("websocket read len:%d, return len:%d", len, read_len);
    return read_len;
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
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET websocket_transporter_write(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    return websocket_client_send_bin(wst->ws_client, buf, len);
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
OPERATE_RET websocket_transporter_poll_read(tuya_transporter_t t, int timeout_ms)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    return websocket_client_poll(wst->ws_client, timeout_ms);;
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
OPERATE_RET websocket_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void* args)
{
    OPERATE_RET ret = OPRT_OK;
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    switch (cmd) {
    case TUYA_TRANSPORTER_SET_TLS_CERT: {
        websocket_client_t* t_client = (websocket_client_t *)wst->ws_client;
        tuya_tls_transporter_ctrl((tuya_transporter_t)(t_client->transporter), TUYA_TRANSPORTER_SET_TLS_CERT, args);
        break;
    }
    case TUYA_TRANSPORTER_SET_WEBSOCKET_CONFIG: {
        tuya_websocket_config_t *p_websocket_config = (tuya_websocket_config_t *)args;

        if (wst->path != NULL) {
            Free(wst->path);
            wst->path = NULL;
        }
        wst->path = mm_strdup(p_websocket_config->path);
        if (wst->path == NULL) {
            PR_ERR("malloc failed");
            return OPRT_MALLOC_FAILED;
        }

        if (wst->scheme != NULL) {
            Free(wst->scheme);
            wst->scheme = NULL;
        }
        wst->scheme = mm_strdup(p_websocket_config->scheme);
        if (wst->scheme == NULL) {
            PR_ERR("malloc failed");
            return OPRT_MALLOC_FAILED;
        }
        break;
    }
    case TUYA_TRANSPORTER_SET_TCP_CONFIG: {
        tuya_tcp_config_t* pConfig = (tuya_tcp_config_t*)args;
        if (pConfig) {
            memcpy(&wst->tcpConfig, pConfig, sizeof(tuya_tcp_config_t));
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
 * @brief 创建websocket transporter，并返回handler
 *
 * @note: 创建websocket trasporter,
 *
 * @return tuya_websocket_transporter_t: transporter handle
 */
tuya_transporter_t tuya_websocket_transporter_create()
{
    tuya_websocket_transporter_t t = Malloc(sizeof(struct websocket_transporter_inter_t));
    if (t == NULL) {
        PR_ERR("websocket transporter malloc fail");
        return NULL;
    }
    memset(t, 0, sizeof(struct websocket_transporter_inter_t));

    tuya_transporter_set_func((tuya_transporter_t)t, (transporter_connect_fn)websocket_transporter_connect, websocket_transporter_close, \
                              websocket_transporter_read, websocket_transporter_write, \
                              websocket_transporter_poll_read, NULL, tuya_websocket_transporter_destroy, websocket_transporter_ctrl);

    tal_mutex_create_init(&t->mutex);

    return &t->base;
}

/**
* @brief 销毁transporter
*
* @param[in] websocket_transporter: websocket_transporter handle
*
* @note: 根据应用层需求，获取对应的transporter，比如http client需要获取scheme=tcp的transporter
*
* @return tuya_transporter_t: !=NULL: 成功创建了transport_type对应的transorter
*/
OPERATE_RET tuya_websocket_transporter_destroy(tuya_transporter_t transporter)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)transporter;

    if (wst->ws_client) {
        websocket_client_close(wst->ws_client);
        Free(wst->ws_client);
        wst->ws_client = NULL;
    }
    if (wst->mutex) {
        tal_mutex_release(wst->mutex);
        wst->mutex = NULL;
    }
    if (wst->path) {
        Free(wst->path);
        wst->path = NULL;
    }
    if (wst->scheme) {
        Free(wst->scheme);
        wst->scheme = NULL;
    }
    if (transporter) {
        Free(transporter);
    }
    return OPRT_OK;
}

/**
* @brief :websocket设置path
*
* @param[in] transporter: transporter handle
* @param[in] path: host to be connected
*
* @note: path用于websocket连接时使用
*
* @return OPERATE_RET: 0: 设置成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_websocket_set_path(tuya_transporter_t t, const char* path)
{

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    wst->path = mm_strdup(path);
    if (wst->path == NULL) {
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

#endif

