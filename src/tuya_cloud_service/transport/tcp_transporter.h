#ifndef __TCP_TRANSPORTER_H__
#define __TCP_TRANSPORTER_H__

#include "tuya_transporter.h"

/**
 * @brief 创建tcp transporter，并返回handler
 *
 * @note: 创建tcp transporter
 *
 *
 * @return tuya_tcp_transporter_t: transport handle
 */
tuya_transporter_t tuya_tcp_transporter_create(void);
/**
* @brief 销毁transporter
*
* @param[in] transporter: tls_transporter handle
*
* @note: 销毁transport，释放资源
*
* @return 0: 成功; <0:请参照tuya error code描述文档
*/
OPERATE_RET tuya_tcp_transporter_destroy(tuya_transporter_t transporter);

#endif
