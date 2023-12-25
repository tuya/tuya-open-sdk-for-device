/**
 * @file tal_time_service.h
 * @brief time service, support UTC time, local time and summer time
 * 
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 * 
 */

#ifndef __TAL_TIME_SERVICE_H__
#define __TAL_TIME_SERVICE_H__

#include "tuya_cloud_types.h"
#include "tal_system.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief posix time
 * 
 */
typedef struct {
    INT_T tm_sec;     /* seconds [0-59] */
    INT_T tm_min;     /* minutes [0-59] */
    INT_T tm_hour;    /* hours [0-23] */
    INT_T tm_mday;    /* day of the month [1-31] */
    INT_T tm_mon;     /* month [0-11] */
    INT_T tm_year;    /* year. The number of years since 1900 */
    INT_T tm_wday;    /* day of the week [0-6] 0-Sunday...6-Saturday */
}POSIX_TM_S;

/**
 * @brief summer time zone
 * 
 */
typedef struct {
    TIME_T posix_min;   // summer time start
    TIME_T posix_max;   // summer time end
}SUM_ZONE_S;

/**
 * @brief number of time zone table
 * 
 */
#define SUM_ZONE_TAB_LMT 6

/**
 * @brief sum zone info
 * 
 */
typedef struct {
    UINT_T cnt; //sum zone count
    SUM_ZONE_S zone[SUM_ZONE_TAB_LMT];//zone info
}SUM_ZONE_TBL_S;

/**
 * @brief time-management module initialization
 * 
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h    
 */
OPERATE_RET tal_time_service_init(VOID);

/**
 * @brief change posix time to TIME_T, redefine the std C func mktime
 * 
 * @param[in] tm the time in posix time format
 * @return the time in TIME_T format 
 */
TIME_T tal_time_mktime(IN CONST POSIX_TM_S *tm);

/**
 * @brief change TIME_T to posix time, redefine the std C func gmtime_r
 * 
 * @param[in] tm the time in TIME_T format
 * @param[out] result the time in posix time format
 * @return the time in posix time format
 */
POSIX_TM_S *tal_time_gmtime_r(IN CONST TIME_T *tm, OUT POSIX_TM_S *result);

/**
 * @brief check IoTOS time synchronize status
 * 
 * @return OPRT_OK on synchronized. Others on not
 */
OPERATE_RET tal_time_check_time_sync(VOID);

/**
 * @brief check IoTOS time zone synchronized status
 * 
 * @return OPRT_OK on synchronized. Others on not 
 */
OPERATE_RET tal_time_check_time_zone_sync(VOID);

/**
 * @brief set IoTOS UTC time
 * 
 * @param[in] tm the new time in posix time format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h   
 */
OPERATE_RET tal_time_set(IN CONST POSIX_TM_S *tm);

/**
 * @brief set IoTOS UTC time in time_t format
 * 
 * @param[in] time the new time in time_t format
 * @param[in] update_source the source of time (0:rtc 1:cloud 2:other)
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h    
 */
OPERATE_RET tal_time_set_posix(IN CONST TIME_T time, IN CONST INT_T update_source);

/**
 * @brief get IoTOS UTC time in posix time format
 * 
 * @param[out] tm the IoTOS UTC time in posix time format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h   
 */
OPERATE_RET tal_time_get(OUT POSIX_TM_S *tm);

/**
 * @brief get IoTOS UTC time in TIME_T format
 * 
 * @return the current second time in TIME_T format 
 */
TIME_T tal_time_get_posix(VOID);

/**
 * @brief get IoTOS UTC time in SYS_TICK_T format
 * 
 * @return the current micro-second time in SYS_TICK_T format 
 */
SYS_TICK_T tal_time_get_posix_ms(VOID);

/**
 * @brief get IoTOS last synchronized UTC time in TIME_T format
 * 
 * @return the time in TIME_T format 
 */
TIME_T tal_time_get_cur_posix(VOID);

/**
 * @brief get IoTOS UTC time, both second and micro-second
 * 
 * @param[out] pSecTime the current time in second
 * @param[out] pMsTime the current time in micro-second
 * @return VOID 
 */
VOID tal_time_get_system_time(OUT TIME_S *pSecTime,OUT TIME_MS *pMsTime);

/**
 * @brief set IoTOS time zone in "+/-hh:mm" format
 * 
 * @param[in] time_zone the new time zone in "+/-hh:mm" format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_set_time_zone(IN CONST CHAR_T *time_zone);

/**
 * @brief set IoTOS time zone in second format
 * 
 * @param[in] time_zone the new time zone in second format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_set_time_zone_seconds(IN INT_T time_zone_sec);

/**
 * @brief get IoTOS time zone in second format
 * 
 * @param[out] time_zone the current time zone in second format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_get_time_zone_seconds(OUT INT_T *time_zone);

/**
 * @brief set IoTOS summer time zone
 * 
 * @param[in] zone the summer time zone table
 * @param[in] cnt the summer time counts
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID tal_time_set_sum_zone_tbl(IN CONST SUM_ZONE_S *zone,IN CONST UINT_T cnt);

/**
 * @brief get IoTOS UTC summer time in posix time format
 * 
 * @param[out] tm the summer time in posix format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_time_get_sum_time(OUT POSIX_TM_S *tm);

/**
 * @brief get IoTOS UTC summer time int TIME_T format 
 * 
 * @return the summer time in TIME_T format
 */
TIME_T tal_time_get_sum_time_posix(VOID);

/**
 * @brief get IoTOS UTC summer time int TIME_T format 
 * 
 * @return the summer time in TIME_T format
 */
BOOL_T tal_time_is_in_sum_zone(TIME_T time);


/**
 * @brief get IoTOS local time (local, contains the time zone and summer time zone)
 * 
 * @param[in] in_time the time need translate
 * @param[out] tm the local time in posix format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note if in_time is 0, return the IoTOS local time, otherwise, translate the in_time to
 * local time
 */
OPERATE_RET tal_time_get_local_time_custom(IN TIME_T in_time, OUT POSIX_TM_S *tm);

/**
 * @brief get sum zone info
 * 
 * @param[out] sum zone info
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_get_sum_zone(OUT SUM_ZONE_TBL_S *sum_zone);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TAL_TIME_SERVICE_H__ */


