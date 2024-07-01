/**
 * @file tuya_health.h
 * @brief Header file for Tuya device health monitoring system.
 *
 * This file contains definitions and configurations for the health monitoring
 * system of Tuya IoT devices. It includes settings for health check intervals,
 * memory thresholds, and queue capacities to ensure the device operates within
 * healthy parameters.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
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

// Default health monitor check interval (tentative)
#define HEALTH_SLEEP_INTERVAL (5)
// Default system health status report interval
#define HEALTH_REPORT_INTERVAL (60 * 60)
// Default minimum free memory threshold set to 5K, normal access to the
// cloud/FLASH requires more than 4K of memory (tentative)
#ifndef HEALTH_FREE_MEM_THRESHOLD
#define HEALTH_FREE_MEM_THRESHOLD (1024 * 8)
#endif
// Default minimum memory block threshold set to 5K, normal access to the
// cloud/FLASH requires a one-time allocation of more than 4K of memory
// (tentative)
#define HEALTH_LARGEST_MEM_BLOK_THRESHOLD (1024 * 5)

// Default maximum workq depth
#define HEALTH_WORKQ_THRESHOLD (50)
// Default maximum msgq number
#define HEALTH_MSGQ_THRESHOLD (50)
// Default maximum timeq number
#define HEALTH_TIMEQ_THRESHOLD (100)

// Default watchdog timer interval, must be a multiple of 20 seconds
#define HEALTH_WATCHDOG_INTERVAL 60
// Default health monitoring scan interval, in seconds, must be a multiple of 20
// seconds
#define HEALTH_DETECT_INTERVAL 600

// Health indicators, must be defined in the order of g_health_policy, otherwise
// the reallocation of global type will be inaccurate
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
    int type;                   // Detection metric
    uint32_t threshold;         // Threshold (number of occurrences)
    uint32_t detect_period;     // Detection period
    health_check_cb check_cb;   // Metric query callback, can be NULL for event-based metrics
    health_notify_cb notify_cb; // Metric notification callback
} health_policy_t;

typedef struct {
    int type;
    void *data;
} health_alert_t;

/**
 * @brief health init function
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
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

/**
 * @brief Disables the watchdog for Tuya health monitoring.
 *
 * This function disables the watchdog feature used for Tuya health monitoring.
 * Watchdog is a timer that resets the system if it is not periodically
 * refreshed. By calling this function, the watchdog timer will be disabled.
 */
void tuya_health_disable_watchdog(void);

#ifdef __cplusplus
}
#endif

#endif
