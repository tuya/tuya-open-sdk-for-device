/**
 * @file tuya_health.h
 * @brief 设备健康监控
 * @version 0.2
 * @date 2022-03-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef __TUYA_DEVOS_HEALTH_H__
#define __TUYA_DEVOS_HEALTH_H__

#include "tuya_cloud_com_defs.h"
#include "tal_thread.h"
#include "tal_mutex.h"
#include "tuya_list.h"

#ifdef __cplusplus
extern "C" {
#endif

// 默认health monitor检测时间间隔（暂定）
#define HEALTH_SLEEP_INTERVAL               (5)
// 默认系统上报健康状态时间
#define HEALTH_REPORT_INTERVAL              (60 * 60)
// 默认系统最小free内存门限设置为5K，正常访问云端、FLASH需要4K内存以上（暂定）
#ifndef HEALTH_FREE_MEM_THRESHOLD
#define HEALTH_FREE_MEM_THRESHOLD           (1024*8)
#endif
// 默认系统最小内存块门限设置为5K，正常访问云端、FLASH需要一次性分配4K内存以上（暂定）
#define HEALTH_LARGEST_MEM_BLOK_THRESHOLD   (1024*5)

// 默认系统最大workq depth
#define HEALTH_WORKQ_THRESHOLD              (50)
// 默认系统最大msgq num
#define HEALTH_MSGQ_THRESHOLD               (50)
// 默认系统最大timeq num
#define HEALTH_TIMEQ_THRESHOLD              (100)

//默认喂狗时间,必须是20秒钟的倍数
#define HEALTH_WATCHDOG_INTERVAL            60
//默认健康监控扫描间隔,单位是秒,必须是20秒钟的倍数
#define HEALTH_DETECT_INTERVAL              600

//health指标,必须在g_health_policy顺序定义,不然global type重新分配会不准
typedef enum {
    HEALTH_RULE_FREE_MEM_SIZE,
    HEALTH_RULE_MAX_MEM_SIZE,
    HEALTH_RULE_ATOP_REFUSE,
    HEALTH_RULE_ATOP_SIGN_FAILED,
    HEALTH_RULE_WORKQ_DEPTH,
    HEALTH_RULE_MSGQ_NUM,
    HEALTH_RULE_TIMER_NUM,
    HEALTH_RULE_FEED_WATCH_DOG,
    HEALTH_RULE_RUNTIME_REPT
} HEALTH_MONITOR_RULE_E;

typedef void (*health_notify_cb)(void);
typedef bool (*health_check_cb)(void);

typedef struct {
    int type;                      // 检测指标
    uint32_t threshold;            // 门限(发生次数)
    uint32_t detect_period;        // 检测周期
    health_check_cb check_cb;      // 指标查询回调,如果事件型可填NULL
    health_notify_cb notify_cb;    // 指标通知回调
} health_policy_t;

typedef struct {
    int     type;
    void   *data;
} health_alert_t;

/**
 * @brief health init function
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_health_monitor_init(void);

/**
 * @brief add health item
 *
 * @param[in] threshold threshold
 * @param[in] period period
 * @param[in] check check cb
 * @param[in] notify notify cb
 *
 * @return type id, success when large than 0,others failed
 */
int tuya_health_item_add(uint32_t threshold, uint32_t period, health_check_cb check, health_notify_cb notify);

/**
 * @brief delete health item
 *
 * @param[in] type type
 *
 */
void tuya_health_item_del(int type);

/**
 * @brief update health item period
 *
 * @param[in] type type
 * @param[in] period period
 *
 */
void tuya_health_update_item_period(int type, uint32_t period);

/**
 * @brief update health item threshold
 *
 * @param[in] type type
 * @param[in] threshold threshold
 *
 */
void tuya_health_update_item_threshold(int type, uint32_t threshold);

/**
 * @brief dump health item
 *
 */
void tuya_health_item_dump(void);

void tuya_health_disable_watchdog(void);

#ifdef __cplusplus
}
#endif

#endif
