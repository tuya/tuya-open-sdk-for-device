#include "tuya_cloud_types.h"
#include "tal_api.h"
#include "tuya_transporter.h"
#include "tls_transporter.h"
#include "tcp_transporter.h"
#include "websocket_transporter.h"
#include "mix_method.h"

#define MAX_TRANSPORTER_NUM     (2)

struct tuya_transport_array_handle {
    tuya_transporter_t array[MAX_TRANSPORTER_NUM];
    uint8_t index;
};

tuya_transport_array_handle_t tuya_transport_array_create()
{
    tuya_transport_array_handle_t p_transport_array = (tuya_transport_array_handle_t)tal_malloc(sizeof(struct tuya_transport_array_handle));
    PR_DEBUG("p_trans array:0x%x", p_transport_array);
    if (p_transport_array) {
        memset(p_transport_array, 0, sizeof(struct tuya_transport_array_handle));
    }

    return p_transport_array;
}

//tuya_transport_array_add_transporter(mq->transporter_array, tls_transporter, "mqtts");
/**
 * @brief 添加transporter到transport array
 *
 * @param[in] transport_handle: transport array handle
 * @param[in] transporter: 待加入的transporter
 * @param[in] scheme: 应用层url使用的scheme，比如http,https,mqtt,mqtts,wss,ws
 * @note:
 *       根据应用层需求，添加对应的transporter，比如http client需要添加scheme=tcp的transporter
 *
 * @return OPERATE_RET: OPRT_OK成功，其它请参照tuya error code描述文档
 */
OPERATE_RET tuya_transport_array_add_transporter(tuya_transport_array_handle_t transport_handle, tuya_transporter_t transporter, char* scheme)
{
    if (transport_handle->index >= MAX_TRANSPORTER_NUM) {
        return OPRT_INDEX_OUT_OF_BOUND;
    }

    transporter->scheme = mm_strdup(scheme);
    transport_handle->array[transport_handle->index++] = transporter;
    return OPRT_OK;
}



/**
* @brief 从transport array获取transporter
*
* @param[in] transport_handle: transport array handle
* @param[in] scheme: 应用层url使用的scheme，比如http,https,mqtt,mqtts,wss,ws
*
* @note: 根据应用层需求，获取对应的transporter，比如http client需要获取scheme=tcp的transporter
*
* @return OPERATE_RET: OPRT_OK成功，其它请参照tuya error code描述文档
*/
tuya_transporter_t tuya_transport_array_get_transporter(tuya_transport_array_handle_t transport_handle, char* scheme)
{
    int i = 0;
    for (; i < MAX_TRANSPORTER_NUM; i++) {
        if (strcmp(transport_handle->array[i]->scheme, scheme) == 0) {
            return transport_handle->array[i];
        }
    }
    return NULL;
}


/**
 * @brief 销毁transport array
 *
 * @param[in] transport_handle: transport array handle
 *
 * @note: 销毁transport array,并卸载array中的所有transporter
 *
 *
 * @return OPERATE_RET: OPRT_OK成功，其它请参照tuya error code描述文档
 */
OPERATE_RET tuya_transport_array_destroy(tuya_transport_array_handle_t transport_handle)
{
    int i = 0;

    for (; i < MAX_TRANSPORTER_NUM; i++) {
        if (transport_handle->array[i]) {
            tuya_transporter_destroy(transport_handle->array[i]);
        }
    }
    tal_free(transport_handle);
    return OPRT_OK;
}


/**
* @brief 创建transporter
*
* @param[in] transport_type: transporter对应的类型：tcp, tls, 或websocket
* @param[in] dependency: transporter的依赖项：比如websocket依赖tls
*
* @note: 根据应用层需求，获取对应的transporter，比如http client需要获取scheme=tcp的transporter
*
* @return tuya_transporter_t: !=NULL: 成功创建了transport_type对应的transorter
*/
tuya_transporter_t tuya_transporter_create(TUYA_TRANSPORT_TYPE_E transport_type, tuya_transporter_t dependency)
{
    if (transport_type == TRANSPORT_TYPE_TCP) {
        return tuya_tcp_transporter_create();
    } else if (transport_type == TRANSPORT_TYPE_TLS) {
        return tuya_tls_transporter_create();
    }
#if defined(ENABLE_WEBSOCKET) && (ENABLE_WEBSOCKET==1)
    else if (transport_type == TRANSPORT_TYPE_WEBSOCKET) {
        return tuya_websocket_transporter_create();
    }
#endif
    return NULL;
}

/**
* @brief 销毁transporter
*
* @param[in] transporter: transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_destroy(tuya_transporter_t t)
{
    if (t && t->f_destroy) {
        t->f_destroy(t);
    }
    return OPRT_OK;
}

/**
* @brief transporter read操作
*
* @param[in] transporter: transporter handle
* @param[inout] buf: 为了读取内容传入的buffer地址
* @param[in] len: 读取长度
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，读取数据，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_read(tuya_transporter_t transporter, uint8_t* buf, int len, int timeout_ms)
{

    if (transporter && transporter->f_read) {
        return transporter->f_read(transporter, buf, len, timeout_ms);
    }

    return OPRT_INVALID_PARM;
}



/**
* @brief transporter write操作
*
* @param[in] transporter: transporter handle
* @param[in] buf: 待写入buffer地址
* @param[in] len: 写入长度
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，读取数据，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 发送的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_write(tuya_transporter_t t, uint8_t* buf, int len, int timeout_ms)
{
    if (t && t->f_write) {
        return t->f_write(t, buf, len, timeout_ms);
    }
    return OPRT_INVALID_PARM;
}


/**
* @brief transporter poll_read操作
*
* @param[in] transporter: transporter handle
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久
*
* @note: 通过transporter handle，poll读取状态，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 可读; =0:超时 <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_poll_read(tuya_transporter_t t, int timeout_ms)
{
    if (t && t->f_poll_read) {
        return t->f_poll_read(t, timeout_ms);
    }
    return OPRT_INVALID_PARM;

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
OPERATE_RET tuya_transporter_poll_write(tuya_transporter_t t, int timeout_ms)
{
    if (t && t->f_poll_write) {
        return t->f_poll_write(t, timeout_ms);
    }
    return OPRT_INVALID_PARM;

}
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
* @return OPERATE_RET: 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_connect(tuya_transporter_t t, const char* host, int port, int timeout_ms)
{
    if (t && t->f_connect) {
        return t->f_connect(t, host, port, timeout_ms);
    }
    return OPRT_INVALID_PARM;
}


/**
* @brief transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_close(tuya_transporter_t t)
{
    if (t && t->f_close) {
        return t->f_close(t);
    }
    return OPRT_INVALID_PARM;
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
OPERATE_RET tuya_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void* args)
{
    if (t && t->f_ctrl) {
        return t->f_ctrl(t, cmd, args);
    }
    return OPRT_INVALID_PARM;
}
/**
* @brief 设置transporter接口
*
* @param[in] transport_handle: transport array handle
* @param[in] connect: connect
*
* @note: 设置具体transporter的接口
*
* @return tuya_transporter_t: !NULL:获取到的transporter, NULL:没找到
*/
OPERATE_RET tuya_transporter_set_func(tuya_transporter_t t,
                                      transporter_connect_fn connect, transporter_close_fn close,
                                      transporter_read_fn read, transporter_write_fn write, transporter_poll_read_fn poll_read,
                                      transporter_poll_read_fn poll_write,
                                      transporter_destroy_fn destroy, transporter_ctrl ctrl)
{

    t->f_connect = connect;
    t->f_close = close;
    t->f_read = read;
    t->f_write = write;
    t->f_poll_read = poll_read;
    t->f_poll_write = poll_write;
    t->f_destroy = destroy;
    t->f_ctrl = ctrl;

    return OPRT_OK;

}

