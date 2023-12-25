#ifndef __WEBSOCKET_TRANSPORTER_H__

#define __WEBSOCKET_TRANSPORTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ENABLE_WEBSOCKET) && (ENABLE_WEBSOCKET==1)
/**
 * @file websocket_transporter.h
 * @author yangkq@tuya.com
 * @brief websocket 网关传输层
 * @version 1.0
 * @date 2021-01-07
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

/*websocket transporter command definitions*/

struct websocket_config_t {
    char* scheme;
    char* path;
};

/**
 *
 */
typedef struct websocket_config_t tuya_websocket_config_t;

/**
 * @brief 创建websocket transporter，并返回handler
 *
 * @note: 创建websocket trasporter,
 *
 * @return tuya_websocket_transporter_t: transporter handle
 */
tuya_transporter_t tuya_websocket_transporter_create();

/**
* @brief 销毁transporter
*
* @param[in] transporter: websocket_transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_websocket_transporter_destroy(tuya_transporter_t transporter);

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
OPERATE_RET tuya_websocket_set_path(tuya_transporter_t transporter, const char* path);


#endif
#endif

