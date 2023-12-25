#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @file transport.h
 * @author yangkq@tuya.com
 * @brief 网络传输层抽象接口
 * @version 1.0
 * @date 2020-12-29
 *
 * @copyright Copyright (c) tuya.inc 2021
 *
 *      transport组件包括两个角色：transport array 和 transporter
 *      mqtt client或http client初始化时，创建transport array，并按需加入tcp transporter, tls transporter或websocket transporter，
 *  根据需要通过transport array获取相应的transporter。
 *      mqtt client或http client需要数据传输需求时，通过transporter提供的统一接口使用tcp, tls或websocket服务。
        |----------------|
        | transport array|
        |----------------|
                |
                |        |-----------------|
                |------->| tcp transporter |
                |        |-----------------|
                |
                |        |-----------------|
                |------->| tls transporter |
                |        |-----------------|
                |
                |        |-----------------|
                |------->| wss transporter |
                |        |-----------------|


 *
 */

#include "tuya_cloud_types.h"

/*tuya transporter command definitions*/
#define TUYA_TRANSPORTER_SET_TLS_CERT               0x0001
#define TUYA_TRANSPORTER_GET_TCP_SOCKET             0x0002
#define TUYA_TRANSPORTER_SET_TCP_CONFIG             0x0003
#define TUYA_TRANSPORTER_SET_WEBSOCKET_CONFIG       0x0004
#define TUYA_TRANSPORTER_SET_TLS_CONFIG             0x0005
#define TUYA_TRANSPORTER_GET_TLS_CONFIG             0x0006

/**
 * @brief 定义了tcp transporter配置选项
 */

struct socket_config_t {
    uint8_t isBlock;
    uint8_t isReuse;
    uint8_t isDisableNagle;
    uint8_t isKeepAlive;
    uint16_t bindPort;
    TUYA_IP_ADDR_T bindAddr;
    uint32_t sendTimeoutMs;
    uint32_t recvTimeoutMs;
    uint32_t keepAliveIdleTime;
    uint32_t keepAliveInterval;
    uint32_t keepAliveCount;
};

/**
 *
 */
typedef struct socket_config_t tuya_tcp_config_t;


/**
 * @brief 定义了transport array handle，用于指示transport array的handler
 */
//struct tuya_transport_array_handle;
// struct tuya_transporter_inter_t;

typedef struct tuya_transport_array_handle* tuya_transport_array_handle_t;

struct tuya_transporter_inter_t;
/**
 * @brief 定义了transporter ，用于指示transporter
 */
typedef struct tuya_transporter_inter_t* tuya_transporter_t;


/**
 * @brief 创建transport array，并返回handler
 *
 * @note: array是静态数组，默认size=3，用于创建一个trasport的集合体，用于保存各种协议的trasporter,
 *        比如,tcp transporter, tls transporter, websocket transporter
 *
 * @return tuya_transport_array_handle_t: transport handle
 */
tuya_transport_array_handle_t tuya_transport_array_create();

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
OPERATE_RET tuya_transport_array_destroy(tuya_transport_array_handle_t transport_handle);

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
OPERATE_RET tuya_transport_array_add_transporter(tuya_transport_array_handle_t transport_handle, tuya_transporter_t transporter, char* scheme);

/**
 * @brief 从transport array移除transporter
 *
 * @param[in] transport_handle: transport array handle
 * @param[in] transporter: 待移除的transporter
 *
 * @note:
 *
 * @return OPERATE_RET: OPRT_OK成功，其它请参照tuya error code描述文档
 */
OPERATE_RET tuya_transport_array_remove_transporter(tuya_transport_array_handle_t transport_handle, tuya_transporter_t transporter);

/**
* @brief 从transport array获取transporter
*
* @param[in] transport_handle: transport array handle
* @param[in] scheme: 应用层url使用的scheme，比如http,https,mqtt,mqtts,wss,ws
*
* @note: 根据应用层需求，获取对应的transporter，比如http client需要获取scheme=tcp的transporter
*
* @return tuya_transporter_t: !NULL:获取到的transporter, NULL:没找到
*/
tuya_transporter_t tuya_transport_array_get_transporter(tuya_transport_array_handle_t transport_handle, char* scheme);




/**
 * @brief 定义transporter类型
 */

typedef uint8_t TUYA_TRANSPORT_TYPE_E;

#define TRANSPORT_TYPE_TCP         (1) //tcp transporter
#define TRANSPORT_TYPE_TLS         (2) //tls transporter
#define TRANSPORT_TYPE_WEBSOCKET   (3) //websocket transporter 

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
tuya_transporter_t tuya_transporter_create(TUYA_TRANSPORT_TYPE_E transport_type, tuya_transporter_t dependency);

/**
* @brief 销毁transporter
*
* @param[in] transporter: transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_destroy(tuya_transporter_t transporter);

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
OPERATE_RET tuya_transporter_connect(tuya_transporter_t transporter, const char* host, int port, int timeout_ms);
/**
* @brief transporter read操作
*
* @param[in] transporter: transporter handle
* @param[inout] buf: 为了读取内容传入的buffer地址
* @param[in] len: 读取长度
* @param[in] timeout_ms: 调用时，如果不可读，最多等待多久，0:永久阻塞
*
* @note: 通过transporter handle，读取数据，可以阻塞timeout_ms
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_read(tuya_transporter_t transporter, uint8_t* buf, int len, int timeout_ms);

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
* @return int: >0: 发送的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_write(tuya_transporter_t transporter, uint8_t* buf, int len, int timeout_ms);


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
OPERATE_RET tuya_transporter_poll_read(tuya_transporter_t transporter, int timeout_ms);
/**
* @brief transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return int: 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_close(tuya_transporter_t transporter);

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
OPERATE_RET tuya_transporter_ctrl(tuya_transporter_t transporter, uint32_t cmd, void* args);

/**
* @brief 销毁transporter
*
* @param[in] transporter: transporter handle
*
* @note: 根据应用层需求，获取对应的transporter，比如http client需要获取scheme=tcp的transporter
*
* @return tuya_transporter_t: !=NULL: 成功创建了transport_type对应的transorter
*/

typedef OPERATE_RET(*transporter_destroy_fn)(tuya_transporter_t t);

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
typedef OPERATE_RET(*transporter_connect_fn)(tuya_transporter_t transporter, const char* host, int port, int timeout_ms);

/**
 * @brief 创建transporter read 接口定义
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
typedef OPERATE_RET(*transporter_read_fn)(tuya_transporter_t transporter, uint8_t* buf, int len, int timeout_ms);

/**
* @brief 创建transporter write 接口定义
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
typedef OPERATE_RET(*transporter_write_fn)(tuya_transporter_t transporter, uint8_t* buf, int len, int timeout_ms);


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
typedef OPERATE_RET(*transporter_poll_read_fn)(tuya_transporter_t transporter, int timeout_ms);

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

typedef OPERATE_RET(*transporter_poll_write_fn)(tuya_transporter_t transporter, int timeout_ms);
/**
* @brief transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
typedef OPERATE_RET(*transporter_close_fn)(tuya_transporter_t transporter);

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
typedef OPERATE_RET(*transporter_ctrl)(tuya_transporter_t transporter, uint32_t cmd, void* args);

/**
* @brief 设置transporter接口
*
* @param[in] transport_handle: transport array handle
* @param[in] connect: connect
*
* @note: 设置具体transporter的接口
*
* @return OPERATE_RET: 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_transporter_set_func(tuya_transporter_t transporter,
                                      transporter_connect_fn connect, transporter_close_fn close,
                                      transporter_read_fn read, transporter_write_fn write, transporter_poll_read_fn poll_read,
                                      transporter_poll_read_fn poll_write, transporter_destroy_fn destroy, transporter_ctrl ctrl);


struct tuya_transporter_inter_t {
    char* scheme;
    transporter_connect_fn f_connect;
    transporter_read_fn f_read;
    transporter_write_fn f_write;
    transporter_poll_read_fn f_poll_read;
    transporter_poll_write_fn f_poll_write;
    transporter_close_fn f_close;
    transporter_destroy_fn f_destroy;
    transporter_ctrl f_ctrl;
};



#ifdef __cplusplus
} // extern "C"
#endif

#endif
