/**
 * @file cpu.h
 * @brief LWIP 芯片相关
 *
 * @copyright copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
#ifndef __CPU_H__
#define __CPU_H__

#include "tuya_iot_config.h"

#if !defined(BYTE_ORDER)

//LITTLE_END是TUYA配置宏，已在"tuya_iot_config.h"中定义
#if defined(LITTLE_END) && (LITTLE_END==1)
#define BYTE_ORDER LITTLE_ENDIAN
#else
#define BYTE_ORDER BIG_ENDIAN
#endif

#endif

#endif /* __CPU_H__ */
