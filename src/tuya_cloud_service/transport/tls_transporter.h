#ifndef __TLS_TRANSPORTER_H__
#define __TLS_TRANSPORTER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_transporter.h"

/**
 * @brief 创建tls transporter，并返回handler
 *
 * @note: 创建tls transporter，并返回handler
 *
 * @return tuya_transport_array_handle_t: transport handle
 */
tuya_transporter_t tuya_tls_transporter_create();

/**
* @brief 销毁transporter
*
* @param[in] transporter: tls_transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tls_transporter_destroy(tuya_transporter_t transporter);

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
OPERATE_RET tuya_tls_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void* args);

/**
* @brief tls transporter close操作
*
* @param[in] transporter: transporter handle
*
* @note: 通过transporter handle，关闭transporter
*
* @return OPERATE_RET: >0: 读取的数据长度; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tls_transporter_close(tuya_transporter_t t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
