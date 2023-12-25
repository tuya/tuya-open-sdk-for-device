/*******************************************************************
*  File: tuya_error_code.h
*  Author: auto generate by tuya code gen system
*  Date: 2023-01-06
*  Description:this file defined the error code of tuya IOT 
*  you can change it manully if needed
*  Copyright(C),2018-2020, tuya inc, www.tuya.comm
*******************************************************************/

#ifndef TUYA_ERROR_CODE_H
#define TUYA_ERROR_CODE_H

#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************
            the error code marco define for module GLOBAL
****************************************************************************/
#define OPRT_OK                                            (0x0000)  //0, 执行成功
#define OPRT_COM_ERROR                                     (-0x0001)  //-1, 通用错误
#define OPRT_INVALID_PARM                                  (-0x0002)  //-2, 无效的入参
#define OPRT_MALLOC_FAILED                                 (-0x0003)  //-3, 内存分配失败
#define OPRT_NOT_SUPPORTED                                 (-0x0004)  //-4, 不支持
#define OPRT_NETWORK_ERROR                                 (-0x0005)  //-5, 网络错误
#define OPRT_NOT_FOUND                                     (-0x0006)  //-6, 没有找到对象
#define OPRT_CR_CJSON_ERR                                  (-0x0007)  //-7, 创建json对象失败
#define OPRT_CJSON_PARSE_ERR                               (-0x0008)  //-8, json解析失败
#define OPRT_CJSON_GET_ERR                                 (-0x0009)  //-9, 获取json对象失败
#define OPRT_CR_MUTEX_ERR                                  (-0x000a)  //-10, 创建信号量失败
#define OPRT_SOCK_ERR                                      (-0x000b)  //-11, 创建socket失败
#define OPRT_SET_SOCK_ERR                                  (-0x000c)  //-12, socket设置失败
#define OPRT_SOCK_CONN_ERR                                 (-0x000d)  //-13, socket连接失败
#define OPRT_SEND_ERR                                      (-0x000e)  //-14, 发送失败
#define OPRT_RECV_ERR                                      (-0x000f)  //-15, 接收失败
#define OPRT_RECV_DA_NOT_ENOUGH                            (-0x0010)  //-16, 接收数据不完整
#define OPRT_KVS_WR_FAIL                                   (-0x0011)  //-17, KV写失败
#define OPRT_KVS_RD_FAIL                                   (-0x0012)  //-18, KV读失败
#define OPRT_CRC32_FAILED                                  (-0x0013)  //-19, CRC校验失败
#define OPRT_TIMEOUT                                       (-0x0014)  //-20, 超时
#define OPRT_INIT_MORE_THAN_ONCE                           (-0x0015)  //-21, 初始化超过一次
#define OPRT_INDEX_OUT_OF_BOUND                            (-0x0016)  //-22, 索引越界
#define OPRT_RESOURCE_NOT_READY                            (-0x0017)  //-23, 资源未完善
#define OPRT_EXCEED_UPPER_LIMIT                            (-0x0018)  //-24, 超过上限
#define OPRT_FILE_NOT_FIND                                 (-0x0019)  //-25, 文件未找到
#define OPRT_FILE_IS_FULL                                  (-0x001a)  //-26, 文件已满
#define OPRT_FILE_OPEN_FAILED                              (-0x001b)  //-27, 文件打开失败
#define OPRT_FILE_READ_FAILED                              (-0x001c)  //-28, 文件读取失败
#define OPRT_FILE_WRITE_FAILED                             (-0x001d)  //-29, 文件写入失败
#define OPRT_NOT_EXIST                                     (-0x001e)  //-30, 不存在
#define OPRT_BUFFER_NOT_ENOUGH                             (-0x001f)  //-31, 缓存不足
#define OPRT_ROUTER_NOT_FOUND                              (-0x0020)  //-32, 路由器未找到
#define OPRT_AUTHENTICATION_NOT_FOUND                      (-0x0021)  //-33, 未认证
#define OPRT_AUTHENTICATION_FAIL                           (-0x0022)  //-34, 认证失败
#define OPRT_MSG_OUT_OF_LIMIT                              (-0x0023)  //-35, 消息超出上限
#define OPRT_VERSION_FMT_ERR                               (-0x0024)  //-36, 版本不匹配
#define OPRT_GLOBAL_ERRCODE_MAX_CNT 37


/****************************************************************************
            the error code marco define for module BASE_OS_ADAPTER
****************************************************************************/
#define OPRT_BASE_OS_ADAPTER_REG_NULL_ERROR                (-0x0100)  //-256, 系统适配注册失败
#define OPRT_BASE_OS_ADAPTER_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module BASE_UTILITIES
****************************************************************************/
#define OPRT_BASE_UTILITIES_PARTITION_EMPTY                (-0x0200)  //-512, 无空闲链表
#define OPRT_BASE_UTILITIES_PARTITION_FULL                 (-0x0201)  //-513, 链表已满
#define OPRT_BASE_UTILITIES_PARTITION_NOT_FOUND            (-0x0202)  //-514, 链表未遍历到
#define OPRT_BASE_UTILITIES_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module BASE_SECURITY
****************************************************************************/
#define OPRT_BASE_SECURITY_CRC32_FAILED                    (-0x0300)  //-768, CRC32错误
#define OPRT_BASE_SECURITY_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module BASE_LOG_MNG
****************************************************************************/
#define OPRT_BASE_LOG_MNG_DONOT_FOUND_MODULE               (-0x0400)  //-1024, 未发现log模块
#define OPRT_BASE_LOG_MNG_PRINT_LOG_LEVEL_HIGHER           (-0x0401)  //-1025, log级别低
#define OPRT_BASE_LOG_MNG_FORMAT_STRING_FAILED             (-0x0402)  //-1026, log字符串格式化失败
#define OPRT_BASE_LOG_MNG_LOG_SEQ_OPEN_FILE_FAIL           (-0x0403)  //-1027, 打开日志序文件失败
#define OPRT_BASE_LOG_MNG_LOG_SEQ_WRITE_FILE_FAIL          (-0x0404)  //-1028, 写日志序文件失败
#define OPRT_BASE_LOG_MNG_LOG_SEQ_FILE_FULL                (-0x0405)  //-1029, 日志序文件满
#define OPRT_BASE_LOG_MNG_LOG_SEQ_FILE_NOT_EXIST           (-0x0406)  //-1030, 日志序文件不存在
#define OPRT_BASE_LOG_MNG_LOG_SEQ_NAME_INVALIDE            (-0x0407)  //-1031, 日志序名称无效
#define OPRT_BASE_LOG_MNG_LOG_SEQ_CREATE_FAIL              (-0x0408)  //-1032, 日志序创建失败
#define OPRT_BASE_LOG_MNG_ERRCODE_MAX_CNT 9


/****************************************************************************
            the error code marco define for module BASE_TIMEQ
****************************************************************************/
#define OPRT_BASE_TIMEQ_TIMERID_EXIST                      (-0x0600)  //-1536, 定时器ID已存在
#define OPRT_BASE_TIMEQ_TIMERID_NOT_FOUND                  (-0x0601)  //-1537, 未找到指定定时器ID
#define OPRT_BASE_TIMEQ_TIMERID_UNVALID                    (-0x0602)  //-1538, 定时器ID非法
#define OPRT_BASE_TIMEQ_GET_IDLE_TIMERID_ERROR             (-0x0603)  //-1539, 获取空闲定时器ID错误
#define OPRT_BASE_TIMEQ_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module BASE_MSGQ
****************************************************************************/
#define OPRT_BASE_MSGQ_NOT_FOUND                           (-0x0700)  //-1792, 消息未找到
#define OPRT_BASE_MSGQ_LIST_EMPTY                          (-0x0701)  //-1793, 列表为空
#define OPRT_BASE_MSGQ_ERRCODE_MAX_CNT 2


/****************************************************************************
            the error code marco define for module MID_HTTP
****************************************************************************/
#define OPRT_MID_HTTP_BUF_NOT_ENOUGH                       (-0x0800)  //-2048, 缓冲区长度不足
#define OPRT_MID_HTTP_URL_PARAM_OUT_LIMIT                  (-0x0801)  //-2049, URL长度超出限制
#define OPRT_MID_HTTP_OS_ERROR                             (-0x0802)  //-2050, 系统错误
#define OPRT_MID_HTTP_PR_REQ_ERROR                         (-0x0803)  //-2051, 准备请求错误,请检查网络是否正常
#define OPRT_MID_HTTP_SD_REQ_ERROR                         (-0x0804)  //-2052, 发送请求错误,请检查网络是否正常
#define OPRT_MID_HTTP_RD_ERROR                             (-0x0805)  //-2053, 读取错误,请检查网络是否正常
#define OPRT_MID_HTTP_AD_HD_ERROR                          (-0x0806)  //-2054, 添加头错误
#define OPRT_MID_HTTP_GET_RESP_ERROR                       (-0x0807)  //-2055, 获取应答错误
#define OPRT_MID_HTTP_AES_INIT_ERR                         (-0x0808)  //-2056, AES初始化错误
#define OPRT_MID_HTTP_AES_OPEN_ERR                         (-0x0809)  //-2057, AES打开错误
#define OPRT_MID_HTTP_AES_SET_KEY_ERR                      (-0x080a)  //-2058, AES设置KEY错误
#define OPRT_MID_HTTP_AES_ENCRYPT_ERR                      (-0x080b)  //-2059, AES加密错误
#define OPRT_MID_HTTP_CR_HTTP_URL_H_ERR                    (-0x080c)  //-2060, 创建HTTP URL头错误
#define OPRT_MID_HTTP_HTTPS_HANDLE_FAIL                    (-0x080d)  //-2061, HTTPS句柄错误
#define OPRT_MID_HTTP_HTTPS_RESP_UNVALID                   (-0x080e)  //-2062, HTTPS无效应答
#define OPRT_MID_HTTP_NO_SUPPORT_RANGE                     (-0x080f)  //-2063, 不支持断点续传
#define OPRT_MID_HTTP_ERRCODE_MAX_CNT 16


/****************************************************************************
            the error code marco define for module MID_MQTT
****************************************************************************/
#define OPRT_MID_MQTT_DEF_ERR                              (-0x0900)  //-2304, 定义失败
#define OPRT_MID_MQTT_INVALID_PARM                         (-0x0901)  //-2305, 参数无效
#define OPRT_MID_MQTT_MALLOC_FAILED                        (-0x0902)  //-2306, 内存申请失败
#define OPRT_MID_MQTT_DNS_PARSED_FAILED                    (-0x0903)  //-2307, DNS解析失败,请检查网络是否正常
#define OPRT_MID_MQTT_SOCK_CREAT_FAILED                    (-0x0904)  //-2308, socket创建失败
#define OPRT_MID_MQTT_SOCK_SET_FAILED                      (-0x0905)  //-2309, socket set失败
#define OPRT_MID_MQTT_TCP_CONNECD_FAILED                   (-0x0906)  //-2310, tcp连接失败,请检查网络是否正常
#define OPRT_MID_MQTT_TCP_TLS_CONNECD_FAILED               (-0x0907)  //-2311, tcp tls连接失败,请检查网络是否正常
#define OPRT_MID_MQTT_PACK_SEND_FAILED                     (-0x0908)  //-2312, 包发送失败,请检查网络是否正常
#define OPRT_MID_MQTT_RECV_DATA_FORMAT_WRONG               (-0x0909)  //-2313, 接收数据格式错误
#define OPRT_MID_MQTT_MSGID_NOT_MATCH                      (-0x090a)  //-2314, 接收数据msgid未找到
#define OPRT_MID_MQTT_START_TM_MSG_ERR                     (-0x090b)  //-2315, 开始事件msg错误
#define OPRT_MID_MQTT_OVER_MAX_MESSAGE_LEN                 (-0x090c)  //-2316, 超过消息最大长度
#define OPRT_MID_MQTT_PING_SEND_ERR                        (-0x090d)  //-2317, ping发送失败,请检查网络是否正常
#define OPRT_MID_MQTT_PUBLISH_TIMEOUT                      (-0x090e)  //-2318, 发布超时,请检查网络是否正常
#define OPRT_MID_MQTT_ERRCODE_MAX_CNT 15


/****************************************************************************
            the error code marco define for module MID_TLS
****************************************************************************/
#define OPRT_MID_TLS_NET_SOCKET_ERROR                      (-0x0a00)  //-2560, Failed to open a socket
#define OPRT_MID_TLS_NET_CONNECT_ERROR                     (-0x0a01)  //-2561, The connection to the given server / port failed.
#define OPRT_MID_TLS_UNKNOWN_HOST_ERROR                    (-0x0a02)  //-2562, Failed to get an IP address for the given hostname.
#define OPRT_MID_TLS_CONNECTION_ERROR                      (-0x0a03)  //-2563, TLS连接失败,请检查网络是否正常
#define OPRT_MID_TLS_DRBG_ENTROPY_ERROR                    (-0x0a04)  //-2564, mbedtls随机种子生成失败
#define OPRT_MID_TLS_X509_ROOT_CRT_PARSE_ERROR             (-0x0a05)  //-2565, X509根证书解析失败
#define OPRT_MID_TLS_X509_DEVICE_CRT_PARSE_ERROR           (-0x0a06)  //-2566, X509设备证书解析失败
#define OPRT_MID_TLS_CTR_DRBG_ENTROPY_SOURCE_ERROR         (-0x0a07)  //-2567, The entropy source failed
#define OPRT_MID_TLS_PK_PRIVATE_KEY_PARSE_ERROR            (-0x0a08)  //-2568, 秘钥解析失败
#define OPRT_MID_TLS_ERRCODE_MAX_CNT 9


/****************************************************************************
            the error code marco define for module SVC_WIFI
****************************************************************************/
#define OPRT_SVC_WIFI_RECV_CONTINUE                        (-0x0b00)  //-2816, 继续接收配网包
#define OPRT_SVC_WIFI_DONOT_FOUND_MODULE                   (-0x0b01)  //-2817, 模块未找到
#define OPRT_SVC_WIFI_PEGASUS_DECODE_FAILED                (-0x0b02)  //-2818, 闪电包解析错误
#define OPRT_SVC_WIFI_NEED_FACTORY_RESET                   (-0x0b03)  //-2819, wifi初始化配置校验失败，需要恢复出厂设置
#define OPRT_SVC_WIFI_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module SVC_MF_TEST
****************************************************************************/
#define OPRT_SVC_MF_TEST_UPDATE_DATA_LEN_EXECED            (-0x0d00)  //-3328, 升级数据长度超过处理上限
#define OPRT_SVC_MF_TEST_UPDATE_CRC_ERROR                  (-0x0d01)  //-3329, 升级crc校验失败
#define OPRT_SVC_MF_TEST_ERRCODE_MAX_CNT 2


/****************************************************************************
            the error code marco define for module SVC_DP
****************************************************************************/
#define OPRT_SVC_DP_ALREADY_PROCESS                        (-0x0e00)  //-3584, DP已经处理
#define OPRT_SVC_DP_ID_NOT_FOUND                           (-0x0e01)  //-3585, DP ID没有发现，请检查上报DP ID是否正确
#define OPRT_SVC_DP_TP_NOT_MATCH                           (-0x0e02)  //-3586, DP 类型未匹配，请检查上报DP类型是否正确
#define OPRT_SVC_DP_DEVICE_NOT_BINDED                      (-0x0e03)  //-3587, 设备未绑定
#define OPRT_SVC_DP_TYPE_PROP_ILLEGAL                      (-0x0e04)  //-3588, 类型属性不合法，枚举型值不存在
#define OPRT_SVC_DP_NW_INVALID                             (-0x0e05)  //-3589, 网络无效，请检查网络是否正常
#define OPRT_SVC_DP_SECURITY_VERIFY_ERR                    (-0x0e06)  //-3590, 安全校验失败
#define OPRT_SVC_DP_REPORT_FINISH                          (-0x0e07)  //-3591, DP上报已结束
#define OPRT_SVC_DP_ERRCODE_MAX_CNT 8


/****************************************************************************
            the error code marco define for module BASE_EVENT
****************************************************************************/
#define OPRT_BASE_EVENT_INVALID_EVENT_NAME                 (-0x0f00)  //-3840, 无效的事件名
#define OPRT_BASE_EVENT_INVALID_EVENT_DESC                 (-0x0f01)  //-3841, 无效的事件描述
#define OPRT_BASE_EVENT_ERRCODE_MAX_CNT 2


/****************************************************************************
            the error code marco define for module SVC_TIMER_TASK
****************************************************************************/
#define OPRT_SVC_TIMER_TASK_LOAD_INVALID_CJSON             (-0x1000)  //-4096, K/V中保存的数据JSON格式错误
#define OPRT_SVC_TIMER_TASK_LOAD_INVALID_DATA              (-0x1001)  //-4097, K/V中保存的数据缺少cnt字段
#define OPRT_SVC_TIMER_TASK_UPDATE_LAST_FETCH_INVALID      (-0x1002)  //-4098, 云端返回的数据缺少lastFetchTime字段
#define OPRT_SVC_TIMER_TASK_UPDATE_TIMER_CNT_INVALID       (-0x1003)  //-4099, 云端返回的数据缺少count字段
#define OPRT_SVC_TIMER_TASK_UPDATE_TIMER_CNT_EXCEED        (-0x1004)  //-4100, 云端返回的定时任务数量超过30个
#define OPRT_SVC_TIMER_TASK_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module SVC_HTTP
****************************************************************************/
#define OPRT_SVC_HTTP_NOT_ENCRYPT_RET                      (-0x1100)  //-4352, 结果未加密，请检查是否有权限访问
#define OPRT_SVC_HTTP_FILL_URL_H_ERR                       (-0x1101)  //-4353, 构造header错误
#define OPRT_SVC_HTTP_FILL_URL_FULL_ERR                    (-0x1102)  //-4354, 构造整个URL错误
#define OPRT_SVC_HTTP_FILL_DATA_ERR                        (-0x1103)  //-4355, 构造data错误
#define OPRT_SVC_HTTP_URL_CFG_AI_SPEAKER_ERR               (-0x1104)  //-4356, 音响配置失败
#define OPRT_SVC_HTTP_URL_CFG_URL_ERR                      (-0x1105)  //-4357, httpUrl/mqttUrl字段缺失
#define OPRT_SVC_HTTP_URL_CFG_URL2IP_ERR                   (-0x1106)  //-4358, httpUrl/mqttUrl解析错误
#define OPRT_SVC_HTTP_URL_CFG_URL2IP_SELF_ERR              (-0x1107)  //-4359, httpsSelfUrl/mqttsSelfUrl解析错误
#define OPRT_SVC_HTTP_URL_CFG_URL2IP_VERIFY_ERR            (-0x1108)  //-4360, httpsVerifyUrl/mqttsVerifyUrl解析错误
#define OPRT_SVC_HTTP_URL_CFG_URL2IP_PSK_ERR               (-0x1109)  //-4361, httpsPSKUrl/mqttsPSKUrl解析错误
#define OPRT_SVC_HTTP_RECV_ERR                             (-0x110a)  //-4362, 接收数据错误，请检查网络是否正常
#define OPRT_SVC_HTTP_RECV_DA_NOT_ENOUGH                   (-0x110b)  //-4363, 接收数据不足
#define OPRT_SVC_HTTP_API_VERIFY_FAILED                    (-0x110c)  //-4364, 数据校验错误
#define OPRT_SVC_HTTP_GW_NOT_EXIST                         (-0x110d)  //-4365, 网关信息不存在
#define OPRT_SVC_HTTP_API_TOKEN_EXPIRE                     (-0x110e)  //-4366, TOKEN过期
#define OPRT_SVC_HTTP_DEV_RESET_FACTORY                    (-0x110f)  //-4367, 设备需要恢复出厂
#define OPRT_SVC_HTTP_DEV_NEED_REGISTER                    (-0x1110)  //-4368, 设备未注册
#define OPRT_SVC_HTTP_ORDER_EXPIRE                         (-0x1111)  //-4369, 订单已过期
#define OPRT_SVC_HTTP_NOT_EXISTS                           (-0x1112)  //-4370, 不存在
#define OPRT_SVC_HTTP_SIGNATURE_ERROR                      (-0x1113)  //-4371, 签名错误
#define OPRT_SVC_HTTP_API_VERSION_WRONG                    (-0x1114)  //-4372, API版本错误
#define OPRT_SVC_HTTP_DEVICE_REMOVED                       (-0x1115)  //-4373, 设备已移除
#define OPRT_SVC_HTTP_DEV_ALREADY_BIND                     (-0x1116)  //-4374, 设备已经绑定
#define OPRT_SVC_HTTP_REMOTE_API_RUN_UNKNOW_FAILED         (-0x1117)  //-4375, 无法识别API
#define OPRT_SVC_HTTP_FORMAT_STRING_FAILED                 (-0x1118)  //-4376, 字符串格式化错误
#define OPRT_SVC_HTTP_API_DECODE_FAILED                    (-0x1119)  //-4377, 数据解密失败
#define OPRT_SVC_HTTP_SERV_VRFY_FAIL                       (-0x111a)  //-4378, 服务端校验失败
#define OPRT_SVC_HTTP_DEVICE_IS_SUB_NODE_OR_IN_ELECTION    (-0x111b)  //-4379, 动态mesh设备云端处于选举中
#define OPRT_SVC_HTTP_EXT_MOD_NOT_SUPPORT_AUTO_ACT         (-0x111c)  //-4380, 扩展模块不支持自动激活
#define OPRT_SVC_HTTP_ERRCODE_MAX_CNT 29


/****************************************************************************
            the error code marco define for module SVC_LAN
****************************************************************************/
#define OPRT_SVC_LAN_SOCKET_FAULT                          (-0x1500)  //-5376, socket错误，APP主动断开或者网络异常
#define OPRT_SVC_LAN_SEND_ERR                              (-0x1501)  //-5377, socket发送错误,请检查网络是否正常
#define OPRT_SVC_LAN_NO_CLIENT_CONNECTED                   (-0x1502)  //-5378, 没有可以上报的局域网设备连接
#define OPRT_SVC_LAN_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module SVC_LAN_LINKAGE
****************************************************************************/
#define OPRT_SVC_LAN_LINKAGE_SOCK_CREAT_ERR                (-0x1600)  //-5632, socket创建失败
#define OPRT_SVC_LAN_LINKAGE_SET_SOCK_ERR                  (-0x1601)  //-5633, socket set失败
#define OPRT_SVC_LAN_LINKAGE_SOCK_CONN_ERR                 (-0x1602)  //-5634, socket连接失败,请检查网络是否正常
#define OPRT_SVC_LAN_LINKAGE_SEND_ERR                      (-0x1603)  //-5635, 发送失败,请检查网络是否正常
#define OPRT_SVC_LAN_LINKAGE_RECV_ERR                      (-0x1604)  //-5636, 接收失败,请检查网络是否正常
#define OPRT_SVC_LAN_LINKAGE_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module SVC_MQTT
****************************************************************************/
#define OPRT_SVC_MQTT_CMD_NOT_EXEC                         (-0x1700)  //-5888, 命令未执行
#define OPRT_SVC_MQTT_CMD_OUT_OF_TIME                      (-0x1701)  //-5889, 命令未在规定时间内执行
#define OPRT_SVC_MQTT_GW_MQ_OFFLILNE                       (-0x1702)  //-5890, MQTT离线,请检查网络是否正常
#define OPRT_SVC_MQTT_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module SVC_PEGASUS
****************************************************************************/
#define OPRT_SVC_PEGASUS_DECODE_FAILED                     (-0x1800)  //-6144, 解码失败
#define OPRT_SVC_PEGASUS_DONOT_FOUND_MODULE                (-0x1801)  //-6145, 模块未找到
#define OPRT_SVC_PEGASUS_ERRCODE_MAX_CNT 2


/****************************************************************************
            the error code marco define for module SVC_UPGRADE
****************************************************************************/
#define OPRT_SVC_UPGRADE_APP_NOT_READY                     (-0x1900)  //-6400, 应用尚未就绪
#define OPRT_SVC_UPGRADE_NO_VALID_FIRMWARE                 (-0x1901)  //-6401, 升级信息字段校验失败
#define OPRT_SVC_UPGRADE_LAN_OTA_FINISH                    (-0x1902)  //-6402, 局域网ota完成
#define OPRT_SVC_UPGRADE_HMAC_NOT_MATCH                    (-0x1903)  //-6403, HMAC不匹配
#define OPRT_SVC_UPGRADE_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module SVC_API_IOT
****************************************************************************/
#define OPRT_SVC_API_IOT_DISCONNECTED_WITH_ROUTER          (-0x1a00)  //-6656, 路由器断开
#define OPRT_SVC_API_IOT_DEV_NOT_BIND                      (-0x1a01)  //-6657, 设备未绑定
#define OPRT_SVC_API_IOT_ERRCODE_MAX_CNT 2


/****************************************************************************
            the error code marco define for module SVC_CELLULAR
****************************************************************************/
#define OPRT_SVC_CELLULAR_CALL_FAILED                      (-0x1b00)  //-6912, 发起呼叫失败
#define OPRT_SVC_CELLULAR_CALL_VOICE_START_ERR             (-0x1b01)  //-6913, 开启呼叫语音失败
#define OPRT_SVC_CELLULAR_CALL_ANSWER_ERR                  (-0x1b02)  //-6914, 呼叫应答失败
#define OPRT_SVC_CELLULAR_CALL_GET_INFO_ERR                (-0x1b03)  //-6915, 获取呼叫信息失败
#define OPRT_SVC_CELLULAR_CALL_HUNGUP_ERR                  (-0x1b04)  //-6916, 呼叫挂机失败
#define OPRT_SVC_CELLULAR_SMS_SEND_ERR                     (-0x1b05)  //-6917, 短信发送失败
#define OPRT_SVC_CELLULAR_SMS_SEND_TIMEOUT                 (-0x1b06)  //-6918, 短信发送超时
#define OPRT_SVC_CELLULAR_SMS_ENCODE_ERR                   (-0x1b07)  //-6919, 短信编码错误
#define OPRT_SVC_CELLULAR_SMS_SIZE_ERR                     (-0x1b08)  //-6920, 短信超长
#define OPRT_SVC_CELLULAR_ERRCODE_MAX_CNT 9


/****************************************************************************
            the error code marco define for module SVC_DEVOS
****************************************************************************/
#define OPRT_SVC_DEVOS_NOT_EXISTS                          (-0x1c00)  //-7168, 不存在
#define OPRT_SVC_DEVOS_SCMA_INVALID                        (-0x1c01)  //-7169, SCMA无效
#define OPRT_SVC_DEVOS_DEV_DP_CNT_INVALID                  (-0x1c02)  //-7170, 设备DP数量无效，不能超过255个
#define OPRT_SVC_DEVOS_NO_AUTHENTICATION                   (-0x1c03)  //-7171, 无授权
#define OPRT_SVC_DEVOS_ROUTER_NOT_FIND                     (-0x1c04)  //-7172, 路由器未找到
#define OPRT_SVC_DEVOS_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module BASE_DB
****************************************************************************/
#define OPRT_BASE_DB_FLASH_NOT_ENOUGH_PAGE                 (-0x1d00)  //-7424, flash页不够
#define OPRT_BASE_DB_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module LINK_CORE
****************************************************************************/
#define OPRT_LINK_CORE_NET_SOCKET_ERROR                    (-0x1e00)  //-7680, Failed to open a socket
#define OPRT_LINK_CORE_NET_CONNECT_ERROR                   (-0x1e01)  //-7681, The connection to the given server / port failed.
#define OPRT_LINK_CORE_UNKNOWN_HOST_ERROR                  (-0x1e02)  //-7682, Failed to get an IP address for the given hostname.
#define OPRT_LINK_CORE_TLS_CONNECTION_ERROR                (-0x1e03)  //-7683, TLS连接失败
#define OPRT_LINK_CORE_DRBG_ENTROPY_ERROR                  (-0x1e04)  //-7684, mbedtls随机种子生成失败
#define OPRT_LINK_CORE_X509_ROOT_CRT_PARSE_ERROR           (-0x1e05)  //-7685, X509根证书解析失败
#define OPRT_LINK_CORE_X509_DEVICE_CRT_PARSE_ERROR         (-0x1e06)  //-7686, X509设备证书解析失败
#define OPRT_LINK_CORE_PK_PRIVATE_KEY_PARSE_ERROR          (-0x1e07)  //-7687, 秘钥解析失败
#define OPRT_LINK_CORE_HTTP_CLIENT_HEADER_ERROR            (-0x1e08)  //-7688, HTTP头初始化失败
#define OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR              (-0x1e09)  //-7689, HTTP请求发送失败
#define OPRT_LINK_CORE_HTTP_RESPONSE_BUFFER_EMPTY          (-0x1e0a)  //-7690, HTTPb buffer为空
#define OPRT_LINK_CORE_HTTP_GW_NOT_EXIST                   (-0x1e0b)  //-7691, 网关不存在，可能设备已被删除
#define OPRT_LINK_CORE_ERRCODE_MAX_CNT 12


/****************************************************************************
            the error code marco define for module SVC_BT
****************************************************************************/
#define OPRT_SVC_BT_API_TRSMITR_CONTINUE                   (-0x1f00)  //-7936, 传输未结束
#define OPRT_SVC_BT_API_TRSMITR_ERROR                      (-0x1f01)  //-7937, 传输错误
#define OPRT_SVC_BT_NETCFG_ERROR_ACK                       (-0x1f02)  //-7938, bt命令出错，给app发送ack
#define OPRT_SVC_BT_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module SVC_NETMGR
****************************************************************************/
#define OPRT_SVC_NETMGR_NEED_FACTORY_RESET                 (-0x2000)  //-8192, 网络初始化配置校验失败，需要恢复出厂设置
#define OPRT_SVC_NETMGR_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module GW_BT_DEV
****************************************************************************/
#define OPRT_GW_BT_DEV_BLE_CONNECT_SUCCESS                 (-0x3100)  //-12544, ble设备连接成功
#define OPRT_GW_BT_DEV_BLE_CONNECT_CNT_EXCEED              (-0x3101)  //-12545, ble设备连接数量已超出最大限制
#define OPRT_GW_BT_DEV_BLE_CONNECT_HAL_FAILED              (-0x3102)  //-12546, ble设备连接HAL层接口返回失败
#define OPRT_GW_BT_DEV_BLE_CONNECT_HANDS_FAILED            (-0x3103)  //-12547, ble设备连接握手失败
#define OPRT_GW_BT_DEV_BLE_CONNECT_HANDS_TIMEOUT           (-0x3104)  //-12548, ble设备连接握手超时,请检查下设备是否正常上电状态,是否距离过远,是否被手机蓝牙连接
#define OPRT_GW_BT_DEV_BLE_CONNECT_GET_INFO_FAILED         (-0x3105)  //-12549, ble设备连接get_info失败
#define OPRT_GW_BT_DEV_BLE_CONNECT_GET_LOGIN_KEY_FAILED    (-0x3106)  //-12550, ble设备连接get_login_key失败
#define OPRT_GW_BT_DEV_BLE_CONNECT_PAIR_DATA_LEN_ERR       (-0x3107)  //-12551, ble设备连接pair_data_len_err失败
#define OPRT_GW_BT_DEV_BLE_CONNECT_SESSION_OPEN_TIMEOUT    (-0x3108)  //-12552, ble设备连接session_open超时
#define OPRT_GW_BT_DEV_BLE_CONNECT_SEND_FAILED             (-0x3109)  //-12553, ble设备连接数据发送失败
#define OPRT_GW_BT_DEV_BLE_LOCAL_RESET                     (-0x310a)  //-12554, ble设备本地重置,请确认是否设备被手动重置
#define OPRT_GW_BT_DEV_BLE_SCAN_TIMEOUT                    (-0x310b)  //-12555, ble设备广播包扫描超时,请检查下设备是否正常上电状态,是否距离过远,是否被手机蓝牙连接
#define OPRT_GW_BT_DEV_BLE_DISCONNECT_SUCCESS              (-0x310c)  //-12556, ble设备断开成功
#define OPRT_GW_BT_DEV_BEACON_DEV_NOT_FOUND                (-0x310d)  //-12557, 设备没有发现
#define OPRT_GW_BT_DEV_BEACON_DEV_KEY1_NOT_FOUND           (-0x310e)  //-12558, 设备KEY1没有发现，通信数据加密的key
#define OPRT_GW_BT_DEV_BEACON_DEV_NOT_BIND                 (-0x310f)  //-12559, 设备未绑定,请重新绑定设备
#define OPRT_GW_BT_DEV_BEACON_DECODE_PAIR_MSG_FAILED       (-0x3110)  //-12560, 设备配网时,网关请求云端解密配网信息失败
#define OPRT_GW_BT_DEV_BEACON_KEY_RESP_TIMEOUT             (-0x3111)  //-12561, 设备配网时,设备beaconkey应答超时
#define OPRT_GW_BT_DEV_BEACON_JOIN_NOTIFY_RESP_TIMEOUT     (-0x3112)  //-12562, 设备配网时,设备入网通知应答超时
#define OPRT_GW_BT_DEV_ERRCODE_MAX_CNT 19


/****************************************************************************
            the error code marco define for module GW_ZG_DEV
****************************************************************************/
#define OPRT_GW_ZG_DEV_ZB_HOST_TABLE_NULL                  (-0x3200)  //-12800, 设备未存在host表,将被移除,网关没有存储过该设备的信息
#define OPRT_GW_ZG_DEV_ZB_SEQ_INVALID                      (-0x3201)  //-12801, ZCL seq无效,当前数据包的seq小于上一个数据包的seq
#define OPRT_GW_ZG_DEV_ZB_HOST_TO_GATEWAY_ERR              (-0x3202)  //-12802, 从host端发送数据到gateway组件出错,本地udp数据传输出错,建议重启网关
#define OPRT_GW_ZG_DEV_ZB_DATA_TOO_BIG                     (-0x3203)  //-12803, zigbee的数据长度太大被丢弃,建议检查数据格式
#define OPRT_GW_ZG_DEV_ZB_GW_TABLE_NULL                    (-0x3204)  //-12804, 网关设备表未初始化,建议重试配网
#define OPRT_GW_ZG_DEV_ZB_GW_RELIABLE_ERR                  (-0x3205)  //-12805, 可靠上报失败,建议检查数据格式
#define OPRT_GW_ZG_DEV_ZB_GW_TRD_DEV_ERR                   (-0x3206)  //-12806, 三级设备上报数据失败,建议检查数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_DEV_ID_ERR               (-0x3207)  //-12807, 数据下发设备id不存在,建议重新配网
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_UPDATE_SCE_LIST_ERR      (-0x3208)  //-12808, 数据下发场景列表不存在,可能没有验证该dp,建议重新创建场景
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_MALLOC_ERR               (-0x3209)  //-12809, GATEWAY层申请内存失败,内存不足,建议重启网关
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_DATA_TYPE_ERR            (-0x320a)  //-12810, 数据下发DP数据类型错误,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_TRD_DEV_DW_ERR           (-0x320b)  //-12811, 三级设备数据下发失败,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_COM_DATALEN_ERR          (-0x320c)  //-12812, 通用对接数据长度错误,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_SEND_DATA_FAILED         (-0x320d)  //-12813, GATEWAY层向HOST层发送数据失败,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_NCRYPT_DATA_FAILED       (-0x320e)  //-12814, 数据加密失败
#define OPRT_GW_ZG_DEV_ZB_GATWAY_STDRELIABLE_DW_FAILED     (-0x320f)  //-12815, 标准透传命令发送失败,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATWAY_CMD_TYPE_NOT_SUPPORT      (-0x3210)  //-12816, 下发命令类型不支持,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_HOST_RCV_SLABSCMD_MSG_LEN_ERR    (-0x3211)  //-12817, 下发数据包长度错误,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_HSOT_DEV_REJOIN                  (-0x3212)  //-12818, 下发数据时,设备处于rejoin过程中,建议等会重试
#define OPRT_GW_ZG_DEV_ZB_HOST_CMD_TYPE_NOT_SUPPORT        (-0x3213)  //-12819, host命令类型不支持,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_HOST_SEND_DATA_FAILED            (-0x3214)  //-12820, host底层发送数据失败,建议重试
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_DEV_BIND_ERR             (-0x3215)  //-12821, 绑定出错,建议重新配网
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_SET_HB_STAT_FAILED       (-0x3216)  //-12822, GATEWAY层设置子设备心跳状态失败,该设备可能未适配
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_JOIN_STAT_FALSE          (-0x3217)  //-12823, 网关未开启配网,确保网关开启配网状态
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_STAND_JOIN_ERR           (-0x3218)  //-12824, 标准入网失败,该设备可能未适配
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_DEV_NOT_SUPPORT          (-0x3219)  //-12825, 子设备不支持,该设备可能未适配
#define OPRT_GW_ZG_DEV_ZB_HOST_RCV_JOIN_MSG_ERR            (-0x321a)  //-12826, zigbeehost层接收MSG长度异常,检查下发数据格式
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_TRD_DEVID_ERR            (-0x321b)  //-12827, 三级设备的设备ID错误,建议重新配网
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_THR_DEV_TPE_ERR          (-0x321c)  //-12828, 三级设备的设备类型错误,建议重新配网
#define OPRT_GW_ZG_DEV_ZB_GATEWAY_RS_PROFILE_NOT_SUPPORT   (-0x321d)  //-12829, 接收ZIGBEE PROFILE不支持,该设备可能未适配
#define OPRT_GW_ZG_DEV_ZB_SUBDEV_LEAVE_SELF                (-0x321e)  //-12830, 子设备主动离网,可能设备被重置
#define OPRT_GW_ZG_DEV_ERRCODE_MAX_CNT 31


/****************************************************************************
            the error code marco define for module GW_BASE_LINKAGE
****************************************************************************/
#define OPRT_GW_BASE_LINKAGE_INIT_FAIL                     (-0x3300)  //-13056, 联动模块初始化失败
#define OPRT_GW_BASE_LINKAGE_COND_CHECK_FAIL               (-0x3301)  //-13057, 联动条件不满足
#define OPRT_GW_BASE_LINKAGE_RULE_ID_NOT_FIND              (-0x3302)  //-13058, 联动规则ID不存在,请检查规则是否存在
#define OPRT_GW_BASE_LINKAGE_RULE_ID_DISABLE               (-0x3303)  //-13059, 联动规则未启用,请检查规则是否启用
#define OPRT_GW_BASE_LINKAGE_RULE_IS_UPDATED               (-0x3304)  //-13060, 联动规在更新中,请稍后重试
#define OPRT_GW_BASE_LINKAGE_FLOW_CNTL                     (-0x3305)  //-13061, 联动流控,可能触发过于频繁,请稍后再重试,也可能存在死循环,请检查联动是否死循环
#define OPRT_GW_BASE_LINKAGE_THREAD_CREATE_FAIL            (-0x3306)  //-13062, 联动线程创建失败
#define OPRT_GW_BASE_LINKAGE_COND_CHECK_NO_ID_MATCH        (-0x3307)  //-13063, 联动条件不满足(设备ID无匹配项)
#define OPRT_GW_BASE_LINKAGE_COND_CHECK_NO_DP_MATCH        (-0x3308)  //-13064, 联动条件不满足(条件DP无匹配项)
#define OPRT_GW_BASE_LINKAGE_ERRCODE_MAX_CNT 9


/****************************************************************************
            the error code marco define for module GW_ZG_SUB
****************************************************************************/
#define OPRT_GW_ZG_SUB_ERR0                                (-0x3400)  //-13312, 错误0
#define OPRT_GW_ZG_SUB_ERR1                                (-0x3401)  //-13313, 错误1
#define OPRT_GW_ZG_SUB_ERR2                                (-0x3402)  //-13314, 错误2:FIB重启
#define OPRT_GW_ZG_SUB_ERR3                                (-0x3403)  //-13315, 错误3:升级重启
#define OPRT_GW_ZG_SUB_ERR4                                (-0x3404)  //-13316, 错误4:外部IO异常重启
#define OPRT_GW_ZG_SUB_ERR5                                (-0x3405)  //-13317, 错误5:低电压重启,可能是断电重启,也可能是供电问题
#define OPRT_GW_ZG_SUB_ERR6                                (-0x3406)  //-13318, 错误6:看门狗重启
#define OPRT_GW_ZG_SUB_ERR7                                (-0x3407)  //-13319, 错误7:软件主动重启
#define OPRT_GW_ZG_SUB_ERR8                                (-0x3408)  //-13320, 错误8:软件crash重启
#define OPRT_GW_ZG_SUB_ERR9                                (-0x3409)  //-13321, 错误9:Flash异常重启
#define OPRT_GW_ZG_SUB_ERR10                               (-0x340a)  //-13322, 错误10:不可恢复硬件异常重启
#define OPRT_GW_ZG_SUB_ERR11                               (-0x340b)  //-13323, 错误11:硬件内部CPU访问错误
#define OPRT_GW_ZG_SUB_ERR12                               (-0x340c)  //-13324, 错误12:掉电重启,可能是断电重启,也可能是供电问题
#define OPRT_GW_ZG_SUB_ERR13                               (-0x340d)  //-13325, 错误13
#define OPRT_GW_ZG_SUB_ERR14                               (-0x340e)  //-13326, 错误14
#define OPRT_GW_ZG_SUB_ERR15                               (-0x340f)  //-13327, 错误15
#define OPRT_GW_ZG_SUB_ERR16                               (-0x3410)  //-13328, 错误16:重启
#define OPRT_GW_ZG_SUB_ERR17                               (-0x3411)  //-13329, 错误17:失联
#define OPRT_GW_ZG_SUB_ERR18                               (-0x3412)  //-13330, 错误18:主动离网,请确认客户是否本地触发退网
#define OPRT_GW_ZG_SUB_ERR19                               (-0x3413)  //-13331, 错误19:被动离网,请确认是不是被APP或者遥控器移除
#define OPRT_GW_ZG_SUB_ERR20                               (-0x3414)  //-13332, 错误20:IO异常
#define OPRT_GW_ZG_SUB_ERR21                               (-0x3415)  //-13333, 错误21:内存泄漏
#define OPRT_GW_ZG_SUB_ERR22                               (-0x3416)  //-13334, 错误22:短地址改变
#define OPRT_GW_ZG_SUB_ERR23                               (-0x3417)  //-13335, 错误23:收到恢复出厂设置
#define OPRT_GW_ZG_SUB_ERR24                               (-0x3418)  //-13336, 错误24:收到F0命令
#define OPRT_GW_ZG_SUB_ERR25                               (-0x3419)  //-13337, 错误25:唤醒时间过长
#define OPRT_GW_ZG_SUB_ERR26                               (-0x341a)  //-13338, 错误26:收到异常恢复出厂设置
#define OPRT_GW_ZG_SUB_ERRCODE_MAX_CNT 27


/****************************************************************************
            the error code marco define for module OS_ADAPTER_MUTEX
****************************************************************************/
#define OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED                 (-0x6500)  //-25856, 创建失败
#define OPRT_OS_ADAPTER_MUTEX_LOCK_FAILED                  (-0x6501)  //-25857, lock失败
#define OPRT_OS_ADAPTER_MUTEX_UNLOCK_FAILED                (-0x6502)  //-25858, unlock失败
#define OPRT_OS_ADAPTER_MUTEX_RELEASE_FAILED               (-0x6503)  //-25859, 释放失败
#define OPRT_OS_ADAPTER_MUTEX_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module OS_ADAPTER_SEM
****************************************************************************/
#define OPRT_OS_ADAPTER_SEM_CREAT_FAILED                   (-0x6600)  //-26112, 创建失败
#define OPRT_OS_ADAPTER_SEM_WAIT_FAILED                    (-0x6601)  //-26113, wait失败
#define OPRT_OS_ADAPTER_SEM_POST_FAILED                    (-0x6602)  //-26114, post失败
#define OPRT_OS_ADAPTER_SEM_RELEASE_FAILED                 (-0x6603)  //-26115, 释放失败
#define OPRT_OS_ADAPTER_SEM_WAIT_TIMEOUT                   (-0x6604)  //-26116, 等待超时
#define OPRT_OS_ADAPTER_SEM_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module OS_ADAPTER_QUEUE
****************************************************************************/
#define OPRT_OS_ADAPTER_QUEUE_CREAT_FAILED                 (-0x6700)  //-26368, 创建失败
#define OPRT_OS_ADAPTER_QUEUE_SEND_FAIL                    (-0x6701)  //-26369, send失败
#define OPRT_OS_ADAPTER_QUEUE_RECV_FAIL                    (-0x6702)  //-26370, recv失败
#define OPRT_OS_ADAPTER_QUEUE_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module OS_ADAPTER_THRD
****************************************************************************/
#define OPRT_OS_ADAPTER_THRD_CREAT_FAILED                  (-0x6800)  //-26624, 创建失败
#define OPRT_OS_ADAPTER_THRD_RELEASE_FAILED                (-0x6801)  //-26625, 释放失败
#define OPRT_OS_ADAPTER_THRD_JUDGE_SELF_FAILED             (-0x6802)  //-26626, 判断是否self失败
#define OPRT_OS_ADAPTER_THRD_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module OS_ADAPTER
****************************************************************************/
#define OPRT_OS_ADAPTER_COM_ERROR                          (-0x6900)  //-26880, 通用异常
#define OPRT_OS_ADAPTER_INVALID_PARM                       (-0x6901)  //-26881, 参数非法
#define OPRT_OS_ADAPTER_MALLOC_FAILED                      (-0x6902)  //-26882, 内存分配失败
#define OPRT_OS_ADAPTER_NOT_SUPPORTED                      (-0x6903)  //-26883, 不支持操作
#define OPRT_OS_ADAPTER_NETWORK_ERROR                      (-0x6904)  //-26884, 网络错误
#define OPRT_OS_ADAPTER_AP_NOT_FOUND                       (-0x6905)  //-26885, AP没有找到
#define OPRT_OS_ADAPTER_AP_SCAN_FAILED                     (-0x6906)  //-26886, AP扫描失败
#define OPRT_OS_ADAPTER_AP_RELEASE_FAILED                  (-0x6907)  //-26887, AP释放失败
#define OPRT_OS_ADAPTER_CHAN_SET_FAILED                    (-0x6908)  //-26888, 信道设置失败
#define OPRT_OS_ADAPTER_CHAN_GET_FAILED                    (-0x6909)  //-26889, 信道获取失败
#define OPRT_OS_ADAPTER_IP_GET_FAILED                      (-0x690a)  //-26890, IP获取失败
#define OPRT_OS_ADAPTER_MAC_SET_FAILED                     (-0x690b)  //-26891, MAC设置失败
#define OPRT_OS_ADAPTER_MAC_GET_FAILED                     (-0x690c)  //-26892, MAC获取失败
#define OPRT_OS_ADAPTER_WORKMODE_SET_FAILED                (-0x690d)  //-26893, 工作模式设置失败
#define OPRT_OS_ADAPTER_WORKMODE_GET_FAILED                (-0x690e)  //-26894, 工作模式获取失败
#define OPRT_OS_ADAPTER_SNIFFER_SET_FAILED                 (-0x690f)  //-26895, SNIFFER设置失败
#define OPRT_OS_ADAPTER_AP_START_FAILED                    (-0x6910)  //-26896, AP启动失败
#define OPRT_OS_ADAPTER_AP_STOP_FAILED                     (-0x6911)  //-26897, AP停止失败
#define OPRT_OS_ADAPTER_APINFO_GET_FAILED                  (-0x6912)  //-26898, AP信息获取失败
#define OPRT_OS_ADAPTER_FAST_CONN_FAILED                   (-0x6913)  //-26899, 快连失败
#define OPRT_OS_ADAPTER_CONN_FAILED                        (-0x6914)  //-26900, 连接失败
#define OPRT_OS_ADAPTER_DISCONN_FAILED                     (-0x6915)  //-26901, 断开失败
#define OPRT_OS_ADAPTER_RSSI_GET_FAILED                    (-0x6916)  //-26902, RSSI获取失败
#define OPRT_OS_ADAPTER_BSSID_GET_FAILED                   (-0x6917)  //-26903, BSSID获取失败
#define OPRT_OS_ADAPTER_STAT_GET_FAILED                    (-0x6918)  //-26904, 状态获取失败
#define OPRT_OS_ADAPTER_CCODE_SET_FAILE                    (-0x6919)  //-26905, CCODE设置失败
#define OPRT_OS_ADAPTER_MGNT_SEND_FAILED                   (-0x691a)  //-26906, 管理包发送失败
#define OPRT_OS_ADAPTER_MGNT_REG_FAILED                    (-0x691b)  //-26907, 管理包回调注册失败
#define OPRT_OS_ADAPTER_WF_LPMODE_SET_FAILED               (-0x691c)  //-26908, Fi低功耗设置失败
#define OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED              (-0x691d)  //-26909, CPU低功耗模式设置失败
#define OPRT_OS_ADAPTER_ERRCODE_MAX_CNT 30


/****************************************************************************
            the error code marco define for module OS_ADAPTER_FLASH
****************************************************************************/
#define OPRT_OS_ADAPTER_FLASH_READ_FAILED                  (-0x6a00)  //-27136, 读取flash失败
#define OPRT_OS_ADAPTER_FLASH_WRITE_FAILED                 (-0x6a01)  //-27137, 写入flash失败
#define OPRT_OS_ADAPTER_FLASH_ERASE_FAILED                 (-0x6a02)  //-27138, 擦除flash失败
#define OPRT_OS_ADAPTER_FLASH_ERRCODE_MAX_CNT 3


/****************************************************************************
            the error code marco define for module OS_ADAPTER_OTA
****************************************************************************/
#define OPRT_OS_ADAPTER_OTA_START_INFORM_FAILED            (-0x6b00)  //-27392, 升级启动通知失败
#define OPRT_OS_ADAPTER_OTA_PKT_SIZE_FAILED                (-0x6b01)  //-27393, 升级包尺寸非法
#define OPRT_OS_ADAPTER_OTA_PROCESS_FAILED                 (-0x6b02)  //-27394, 升级包下载写入失败
#define OPRT_OS_ADAPTER_OTA_VERIFY_FAILED                  (-0x6b03)  //-27395, 升级包校验失败
#define OPRT_OS_ADAPTER_OTA_END_INFORM_FAILED              (-0x6b04)  //-27396, 升级结束通知失败
#define OPRT_OS_ADAPTER_OTA_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module OS_ADAPTER_WD
****************************************************************************/
#define OPRT_OS_ADAPTER_WD_WD_INIT_FAILED                  (-0x6c00)  //-27648, watch dog初始化失败
#define OPRT_OS_ADAPTER_WD_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module OS_ADAPTER_GPIO
****************************************************************************/
#define OPRT_OS_ADAPTER_GPIO_INOUT_SET_FAILED              (-0x6d00)  //-27904, GPIO INOUT设置失败
#define OPRT_OS_ADAPTER_GPIO_MODE_SET_FAILED               (-0x6d01)  //-27905, GPIO 模式设置失败
#define OPRT_OS_ADAPTER_GPIO_WRITE_FAILED                  (-0x6d02)  //-27906, GPIO 写入失败
#define OPRT_OS_ADAPTER_GPIO_IRQ_INIT_FAILED               (-0x6d03)  //-27907, GPIO 中断初始化失败
#define OPRT_OS_ADAPTER_GPIO_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module OS_ADAPTER_UART
****************************************************************************/
#define OPRT_OS_ADAPTER_UART_INIT_FAILED                   (-0x6e00)  //-28160, UART初始化失败
#define OPRT_OS_ADAPTER_UART_DEINIT_FAILED                 (-0x6e01)  //-28161, UART释放失败
#define OPRT_OS_ADAPTER_UART_SEND_FAILED                   (-0x6e02)  //-28162, UART发送失败
#define OPRT_OS_ADAPTER_UART_READ_FAILED                   (-0x6e03)  //-28163, UART接收失败
#define OPRT_OS_ADAPTER_UART_ERRCODE_MAX_CNT 4


/****************************************************************************
            the error code marco define for module OS_ADAPTER_I2C
****************************************************************************/
#define OPRT_OS_ADAPTER_I2C_OPEN_FAILED                    (-0x6f00)  //-28416, I2C 打开失败
#define OPRT_OS_ADAPTER_I2C_CLOSE_FAILED                   (-0x6f01)  //-28417, I2C 关闭失败
#define OPRT_OS_ADAPTER_I2C_READ_FAILED                    (-0x6f02)  //-28418, I2C 读取失败
#define OPRT_OS_ADAPTER_I2C_WRITE_FAILED                   (-0x6f03)  //-28419, I2C 写入失败
#define OPRT_OS_ADAPTER_I2C_INVALID_PARM                   (-0x6f04)  //-28420, 参数无效
#define OPRT_OS_ADAPTER_I2C_INIT_FAILED                    (-0x6f05)  //-28421, 初始化失败
#define OPRT_OS_ADAPTER_I2C_DEINIT_FAILED                  (-0x6f06)  //-28422, 释放失败
#define OPRT_OS_ADAPTER_I2C_MODE_ERR                       (-0x6f07)  //-28423, 模式错误
#define OPRT_OS_ADAPTER_I2C_BUSY                           (-0x6f08)  //-28424, 忙
#define OPRT_OS_ADAPTER_I2C_TIMEOUT                        (-0x6f09)  //-28425, 超时
#define OPRT_OS_ADAPTER_I2C_MASTER_LOST_ARB                (-0x6f0a)  //-28426, 主机仲裁失败
#define OPRT_OS_ADAPTER_I2C_BUS_ERR                        (-0x6f0b)  //-28427, 总线错误
#define OPRT_OS_ADAPTER_I2C_TRANS_ERR                      (-0x6f0c)  //-28428, 传输错误
#define OPRT_OS_ADAPTER_I2C_ADDR_NO_ACK                    (-0x6f0d)  //-28429, 无ACK
#define OPRT_OS_ADAPTER_I2C_IRQ_INIT_FAILED                (-0x6f0e)  //-28430, 中断初始化失败
#define OPRT_OS_ADAPTER_I2C_RESERVE1_ERR                   (-0x6f0f)  //-28431, RESERVE ERR1
#define OPRT_OS_ADAPTER_I2C_RESERVE2_ERR                   (-0x6f10)  //-28432, RESERVE ERR2
#define OPRT_OS_ADAPTER_I2C_RESERVE3_ERR                   (-0x6f11)  //-28433, RESERVE ERR3
#define OPRT_OS_ADAPTER_I2C_ERRCODE_MAX_CNT 18


/****************************************************************************
            the error code marco define for module OS_ADAPTER_BLE
****************************************************************************/
#define OPRT_OS_ADAPTER_BLE_HANDLE_ERROR                   (-0x7000)  //-28672, Ble 句柄错误
#define OPRT_OS_ADAPTER_BLE_BUSY                           (-0x7001)  //-28673, Ble 繁忙
#define OPRT_OS_ADAPTER_BLE_TIMEOUT                        (-0x7002)  //-28674, Ble 超时
#define OPRT_OS_ADAPTER_BLE_RESERVED1                      (-0x7003)  //-28675, Ble Reserved Error Code 1
#define OPRT_OS_ADAPTER_BLE_RESERVED2                      (-0x7004)  //-28676, Ble Reserved Error Code 2
#define OPRT_OS_ADAPTER_BLE_RESERVED3                      (-0x7005)  //-28677, Ble Reserved Error Code 3
#define OPRT_OS_ADAPTER_BLE_INIT_FAILED                    (-0x7006)  //-28678, Ble 初始化失败
#define OPRT_OS_ADAPTER_BLE_DEINIT_FAILED                  (-0x7007)  //-28679, Ble 释放失败
#define OPRT_OS_ADAPTER_BLE_GATT_CONN_FAILED               (-0x7008)  //-28680, Ble GATT连接失败
#define OPRT_OS_ADAPTER_BLE_GATT_DISCONN_FAILED            (-0x7009)  //-28681, Ble GATT断开失败
#define OPRT_OS_ADAPTER_BLE_ADV_START_FAILED               (-0x700a)  //-28682, Ble 开启广播失败
#define OPRT_OS_ADAPTER_BLE_ADV_STOP_FAILED                (-0x700b)  //-28683, Ble 停止广播失败
#define OPRT_OS_ADAPTER_BLE_SCAN_START_FAILED              (-0x700c)  //-28684, Ble 开启扫描失败
#define OPRT_OS_ADAPTER_BLE_SCAN_STAOP_FAILED              (-0x700d)  //-28685, Ble 停止扫描失败
#define OPRT_OS_ADAPTER_BLE_SVC_DISC_FAILED                (-0x700e)  //-28686, Ble 服务发现失败
#define OPRT_OS_ADAPTER_BLE_CHAR_DISC_FAILED               (-0x700f)  //-28687, Ble 特征值发现失败
#define OPRT_OS_ADAPTER_BLE_DESC_DISC_FAILED               (-0x7010)  //-28688, Ble 特征值描述符发现失败
#define OPRT_OS_ADAPTER_BLE_NOTIFY_FAILED                  (-0x7011)  //-28689, Ble Peripheral Notify失败
#define OPRT_OS_ADAPTER_BLE_INDICATE_FAILED                (-0x7012)  //-28690, Ble Peripheral Indicate失败
#define OPRT_OS_ADAPTER_BLE_WRITE_FAILED                   (-0x7013)  //-28691, Ble Central 写失败
#define OPRT_OS_ADAPTER_BLE_READ_FAILED                    (-0x7014)  //-28692, Ble Central 读失败
#define OPRT_OS_ADAPTER_BLE_MTU_REQ_FAILED                 (-0x7015)  //-28693, Ble Central MTU请求失败
#define OPRT_OS_ADAPTER_BLE_MTU_REPLY_FAILED               (-0x7016)  //-28694, Ble Peripheral MTU 响应失败
#define OPRT_OS_ADAPTER_BLE_CONN_PARAM_UPDATE_FAILED       (-0x7017)  //-28695, Ble 连接参数更新失败
#define OPRT_OS_ADAPTER_BLE_CONN_RSSI_GET_FAILED           (-0x7018)  //-28696, Ble 连接信号强度获取失败
#define OPRT_OS_ADAPTER_BLE_MESH_INVALID_OPCODE            (-0x7019)  //-28697, Ble Mesh 无效的opcode
#define OPRT_OS_ADAPTER_BLE_MESH_INVALID_ELEMENT           (-0x701a)  //-28698, Ble Mesh 无效的element
#define OPRT_OS_ADAPTER_BLE_MESH_INVALID_MODEL             (-0x701b)  //-28699, Ble Mesh 无效的model
#define OPRT_OS_ADAPTER_BLE_MESH_INVALID_ADDR              (-0x701c)  //-28700, Ble Mesh 无效的source, virtual或destination address
#define OPRT_OS_ADAPTER_BLE_MESH_INVALID_INDEX             (-0x701d)  //-28701, Ble Mesh 无效的序号，如：appkey index, netkey index等
#define OPRT_OS_ADAPTER_BLE_MESH_NO_MEMORY                 (-0x701e)  //-28702, Ble Mesh 内存占用已满，如：发包过快导致底层缓存过多等
#define OPRT_OS_ADAPTER_BLE_MESH_APPKEY_NOT_BOUND_MODEL    (-0x701f)  //-28703, Ble Mesh Appkey未绑定错误
#define OPRT_OS_ADAPTER_BLE_MESH_RESERVED1                 (-0x7020)  //-28704, Ble Mesh Reserved Error Code 1
#define OPRT_OS_ADAPTER_BLE_MESH_RESERVED2                 (-0x7021)  //-28705, Ble Mesh Reserved Error Code 2
#define OPRT_OS_ADAPTER_BLE_MESH_RESERVED3                 (-0x7022)  //-28706, Ble Mesh Reserved Error Code 3
#define OPRT_OS_ADAPTER_BLE_MESH_RESERVED4                 (-0x7023)  //-28707, Ble Mesh Reserved Error Code 4
#define OPRT_OS_ADAPTER_BLE_MESH_RESERVED5                 (-0x7024)  //-28708, Ble Mesh Reserved Error Code 5
#define OPRT_OS_ADAPTER_BLE_MESH_PROVISION_FAIL            (-0x7025)  //-28709, Ble Mesh 配网失败
#define OPRT_OS_ADAPTER_BLE_MESH_COMPO_GET_FAIL            (-0x7026)  //-28710, Ble Mesh 获取Composition数据失败
#define OPRT_OS_ADAPTER_BLE_MESH_MODEL_BIND_FAIL           (-0x7027)  //-28711, Ble Mesh 绑定model失败
#define OPRT_OS_ADAPTER_BLE_MESH_APPKEY_ADD_FAIL           (-0x7028)  //-28712, Ble Mesh Appkey添加失败
#define OPRT_OS_ADAPTER_BLE_MESH_NETKEY_ADD_FAIL           (-0x7029)  //-28713, Ble Mesh Netkey添加失败
#define OPRT_OS_ADAPTER_BLE_MESH_APPKEY_BIND_FAIL          (-0x702a)  //-28714, Ble Mesh Appkey绑定失败
#define OPRT_OS_ADAPTER_BLE_ERRCODE_MAX_CNT 43


/****************************************************************************
            the error code marco define for module MID_TRANSPORT
****************************************************************************/
#define OPRT_MID_TRANSPORT_INVALID_PARM                    (-0x7100)  //-28928, 参数无效
#define OPRT_MID_TRANSPORT_MALLOC_FAILED                   (-0x7101)  //-28929, 内存申请失败
#define OPRT_MID_TRANSPORT_DNS_PARSED_FAILED               (-0x7102)  //-28930, DNS解析失败，请检查网络是否正常
#define OPRT_MID_TRANSPORT_SOCK_CREAT_FAILED               (-0x7103)  //-28931, socket创建失败，请检查是否socket配置数量太小
#define OPRT_MID_TRANSPORT_SOCK_SET_REUSE_FAILED           (-0x7104)  //-28932, socket set reuse 失败
#define OPRT_MID_TRANSPORT_SOCK_SET_DISABLE_NAGLE_FAILED   (-0x7105)  //-28933, socket set disable nagle失败
#define OPRT_MID_TRANSPORT_SOCK_SET_KEEP_ALIVE_FAILED      (-0x7106)  //-28934, socket set keep alive失败
#define OPRT_MID_TRANSPORT_SOCK_SET_BLOCK_FAILED           (-0x7107)  //-28935, socket set block失败
#define OPRT_MID_TRANSPORT_SOCK_NET_BIND_FAILED            (-0x7108)  //-28936, socket net bind失败
#define OPRT_MID_TRANSPORT_SOCK_SET_TIMEOUT_FAILED         (-0x7109)  //-28937, socket set timeout失败
#define OPRT_MID_TRANSPORT_TCP_CONNECD_FAILED              (-0x710a)  //-28938, tcp连接失败，请检查网络是否正常
#define OPRT_MID_TRANSPORT_TCP_TLS_CONNECD_FAILED          (-0x710b)  //-28939, tcp tls连接失败，请检查网络是否正常
#define OPRT_MID_TRANSPORT_PACK_SEND_FAILED                (-0x710c)  //-28940, 包发送失败，请检查网络是否正常
#define OPRT_MID_TRANSPORT_ERRCODE_MAX_CNT 13


/****************************************************************************
            the error code marco define for module OS_ADAPTER_SPI
****************************************************************************/
#define OPRT_OS_ADAPTER_SPI_INVALID_PARM                   (-0x7200)  //-29184, 参数无效
#define OPRT_OS_ADAPTER_SPI_INIT_FAILED                    (-0x7201)  //-29185, 初始化失败
#define OPRT_OS_ADAPTER_SPI_DEINIT_FAILED                  (-0x7202)  //-29186, 释放失败
#define OPRT_OS_ADAPTER_SPI_MODE_ERR                       (-0x7203)  //-29187, 模式错误
#define OPRT_OS_ADAPTER_SPI_BUSY                           (-0x7204)  //-29188, 忙
#define OPRT_OS_ADAPTER_SPI_DATA_LOST                      (-0x7205)  //-29189, 数据丢失
#define OPRT_OS_ADAPTER_SPI_TIMEOUT                        (-0x7206)  //-29190, 超时
#define OPRT_OS_ADAPTER_SPI_BUS_ERR                        (-0x7207)  //-29191, 总线错误
#define OPRT_OS_ADAPTER_SPI_TRANS_ERR                      (-0x7208)  //-29192, 传输错误
#define OPRT_OS_ADAPTER_SPI_IRQ_ERR                        (-0x7209)  //-29193, 中断异常
#define OPRT_OS_ADAPTER_SPI_RESERVE1_ERR                   (-0x720a)  //-29194, RESERVE ERR1
#define OPRT_OS_ADAPTER_SPI_RESERVE2_ERR                   (-0x720b)  //-29195, RESERVE ERR2
#define OPRT_OS_ADAPTER_SPI_RESERVE3_ERR                   (-0x720c)  //-29196, RESERVE ERR3
#define OPRT_OS_ADAPTER_SPI_ERRCODE_MAX_CNT 13


/****************************************************************************
            the error code marco define for module OS_ADAPTER_PWM
****************************************************************************/
#define OPRT_OS_ADAPTER_PWM_INVALID_PARM                   (-0x7300)  //-29440, 参数无效
#define OPRT_OS_ADAPTER_PWM_INIT_FAILED                    (-0x7301)  //-29441, 初始化失败
#define OPRT_OS_ADAPTER_PWM_DEINIT_FAILED                  (-0x7302)  //-29442, 释放失败
#define OPRT_OS_ADAPTER_PWM_MODE_ERR                       (-0x7303)  //-29443, 模式错误
#define OPRT_OS_ADAPTER_PWM_BUSY                           (-0x7304)  //-29444, 忙
#define OPRT_OS_ADAPTER_PWM_IRQ_INIT_FAILED                (-0x7305)  //-29445, 中断初始化失败
#define OPRT_OS_ADAPTER_PWM_RESERVE1_ERR                   (-0x7306)  //-29446, RESERVE ERR1
#define OPRT_OS_ADAPTER_PWM_RESERVE2_ERR                   (-0x7307)  //-29447, RESERVE ERR2
#define OPRT_OS_ADAPTER_PWM_RESERVE3_ERR                   (-0x7308)  //-29448, RESERVE ERR3
#define OPRT_OS_ADAPTER_PWM_ERRCODE_MAX_CNT 9


/****************************************************************************
            the error code marco define for module OS_ADAPTER_ADC
****************************************************************************/
#define OPRT_OS_ADAPTER_ADC_INVALID_PARM                   (-0x7400)  //-29696, 参数无效
#define OPRT_OS_ADAPTER_ADC_INIT_FAILED                    (-0x7401)  //-29697, 初始化失败
#define OPRT_OS_ADAPTER_ADC_DEINIT_FAILED                  (-0x7402)  //-29698, 释放失败
#define OPRT_OS_ADAPTER_ADC_READ_FAILED                    (-0x7403)  //-29699, ADC读取失败
#define OPRT_OS_ADAPTER_ADC_MODE_ERR                       (-0x7404)  //-29700, 模式错误
#define OPRT_OS_ADAPTER_ADC_BUSY                           (-0x7405)  //-29701, 忙
#define OPRT_OS_ADAPTER_ADC_TIMEOUT                        (-0x7406)  //-29702, 超时
#define OPRT_OS_ADAPTER_ADC_IRQ_INIT_FAILED                (-0x7407)  //-29703, 中断初始化失败
#define OPRT_OS_ADAPTER_ADC_RESERVE1_ERR                   (-0x7408)  //-29704, RESERVE ERR1
#define OPRT_OS_ADAPTER_ADC_RESERVE2_ERR                   (-0x7409)  //-29705, RESERVE ERR2
#define OPRT_OS_ADAPTER_ADC_RESERVE3_ERR                   (-0x740a)  //-29706, RESERVE ERR3
#define OPRT_OS_ADAPTER_ADC_ERRCODE_MAX_CNT 11


/****************************************************************************
            the error code marco define for module OS_ADAPTER_DAC
****************************************************************************/
#define OPRT_OS_ADAPTER_DAC_INVALID_PARM                   (-0x7500)  //-29952, 参数无效
#define OPRT_OS_ADAPTER_DAC_INIT_FAILED                    (-0x7501)  //-29953, 初始化失败
#define OPRT_OS_ADAPTER_DAC_DEINIT_FAILED                  (-0x7502)  //-29954, 释放失败
#define OPRT_OS_ADAPTER_DAC_WRITE_FAILED                   (-0x7503)  //-29955, DAC写失败
#define OPRT_OS_ADAPTER_DAC_MODE_ERR                       (-0x7504)  //-29956, 模式错误
#define OPRT_OS_ADAPTER_DAC_BUSY                           (-0x7505)  //-29957, 忙
#define OPRT_OS_ADAPTER_DAC_TIMEOUT                        (-0x7506)  //-29958, 超时
#define OPRT_OS_ADAPTER_DAC_IRQ_INIT_FAILED                (-0x7507)  //-29959, 中断初始化失败
#define OPRT_OS_ADAPTER_DAC_RESERVE1_ERR                   (-0x7508)  //-29960, RESERVE ERR1
#define OPRT_OS_ADAPTER_DAC_RESERVE2_ERR                   (-0x7509)  //-29961, RESERVE ERR2
#define OPRT_OS_ADAPTER_DAC_RESERVE3_ERR                   (-0x750a)  //-29962, RESERVE ERR3
#define OPRT_OS_ADAPTER_DAC_ERRCODE_MAX_CNT 11


/****************************************************************************
            the error code marco define for module OS_ADAPTER_I2S
****************************************************************************/
#define OPRT_OS_ADAPTER_I2S_INVALID_PARM                   (-0x7600)  //-30208, 参数无效
#define OPRT_OS_ADAPTER_I2S_INIT_FAILED                    (-0x7601)  //-30209, 初始化失败
#define OPRT_OS_ADAPTER_I2S_DEINIT_FAILED                  (-0x7602)  //-30210, 释放失败
#define OPRT_OS_ADAPTER_I2S_WRITE_FAILED                   (-0x7603)  //-30211, I2S写入失败
#define OPRT_OS_ADAPTER_I2S_READ_FAILED                    (-0x7604)  //-30212, I2S读取失败
#define OPRT_OS_ADAPTER_I2S_MODE_ERR                       (-0x7605)  //-30213, 模式错误
#define OPRT_OS_ADAPTER_I2S_BUSY                           (-0x7606)  //-30214, 忙
#define OPRT_OS_ADAPTER_I2S_TIMEOUT                        (-0x7607)  //-30215, 超时
#define OPRT_OS_ADAPTER_I2S_DATA_LOST                      (-0x7608)  //-30216, 数据丢失
#define OPRT_OS_ADAPTER_I2S_TRANS_ERR                      (-0x7609)  //-30217, 传输错误
#define OPRT_OS_ADAPTER_I2S_IRQ_INIT_FAILED                (-0x760a)  //-30218, 中断初始化失败
#define OPRT_OS_ADAPTER_I2S_RESERVE1_ERR                   (-0x760b)  //-30219, RESERVE ERR1
#define OPRT_OS_ADAPTER_I2S_RESERVE2_ERR                   (-0x760c)  //-30220, RESERVE ERR2
#define OPRT_OS_ADAPTER_I2S_RESERVE3_ERR                   (-0x760d)  //-30221, RESERVE ERR3
#define OPRT_OS_ADAPTER_I2S_ERRCODE_MAX_CNT 14


/****************************************************************************
            the error code marco define for module GW_SUB_MGR
****************************************************************************/
#define OPRT_GW_SUB_MGR_INIT_FAIL                          (-0x7700)  //-30464, 子设备管理模块初始化失败
#define OPRT_GW_SUB_MGR_BIND_THREAD_CREATE_FAIL            (-0x7701)  //-30465, 绑定线程创建失败
#define OPRT_GW_SUB_MGR_EXCEED_MAX_CNT                     (-0x7702)  //-30466, 超过子设备最大数目
#define OPRT_GW_SUB_MGR_NODE_CREATE_FAIL                   (-0x7703)  //-30467, 创建子设备节点失败
#define OPRT_GW_SUB_MGR_CLOUD_HTTP_BIND_FAIL               (-0x7704)  //-30468, 与云端http交互失败
#define OPRT_GW_SUB_MGR_ERRCODE_MAX_CNT 5


/****************************************************************************
            the error code marco define for module APP_TMP
****************************************************************************/
#define OPRT_APP_TMP_xxx                                   (-0x7800)  //-30720, XXX
#define OPRT_APP_TMP_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module IMM_COM
****************************************************************************/
#define OPRT_IMM_COM_MEM_PARTITION_FULL                    (-0x7900)  //-30976, 存储缓冲区满
#define OPRT_IMM_COM_ERRCODE_MAX_CNT 1


/****************************************************************************
            the error code marco define for module IMM_CLOUD_STORAGE
****************************************************************************/
#define OPRT_IMM_CLOUD_STORAGE_ENCRYPT_KEY_UPDATED         (-0x7a00)  //-31232, 存储缓冲区满
#define OPRT_IMM_CLOUD_STORAGE_STATUS_OFF                  (-0x7a01)  //-31233, 云存储停止状态
#define OPRT_IMM_CLOUD_STORAGE_UPLOAD_INTERRUPT_ERROR      (-0x7a02)  //-31234, 网络上报引起中断
#define OPRT_IMM_CLOUD_STORAGE_EVENT_STATUS_INVALID        (-0x7a03)  //-31235, 事件状态错误
#define OPRT_IMM_CLOUD_STORAGE_ERRCODE_MAX_CNT 4


#define ERRCODE2STRING(errcode) #errcode
#define TUYA_ERROR_STRING(errcode)  ("[ErrCode: " ERRCODE2STRING(errcode) "]")

#define TUYA_CHECK_NULL_RETURN(x, y)\
do{\
    if (NULL == (x)){\
        PR_ERR("%s null", #x);\
        return (y);\
    }\
}while(0)


#define TUYA_CHECK_NULL_GOTO(x, label)\
do{\
    if (NULL == (x)){\
        PR_ERR("%s null", #x);\
        goto label;\
    }\
}while(0)


#define TUYA_CALL_ERR_LOG(func)\
do{\
    rt = (func);\
    if (OPRT_OK != (rt)){\
        PR_ERR("ret:%d", rt);\
    }\
}while(0)


#define TUYA_CALL_ERR_GOTO(func, label)\
do{\
    rt = (func);\
    if (OPRT_OK != (rt)){\
        PR_ERR("ret:%d", rt);\
        goto label;\
    }\
}while(0)


#define TUYA_CALL_ERR_RETURN(func)\
do{\
    rt = (func);\
    if (OPRT_OK != (rt)){\
       PR_ERR("ret:%d", rt);\
       return (rt);\
    }\
}while(0)


#define TUYA_CALL_ERR_RETURN_VAL(func, y)\
do{\
    rt = (func);\
    if (OPRT_OK != (rt)){\
        PR_ERR("ret:%d", rt);\
        return (y);\
    }\
}while(0)



#ifdef __cplusplus
}
#endif
#endif
