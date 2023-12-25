#include "tal_time_service.h"
#include "tal_mutex.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "tal_log.h"
#include "tal_event.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define SEC_PER_DAY 86400
#define SEC_PER_HOUR 3600

/***********************************************************
*************************variable define********************
***********************************************************/
STATIC CONST TIME_T SEC_PER_YR[2] = { 31536000, 31622400 };
STATIC CONST TIME_T SEC_PER_MT[2][12] =  {
    { 2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
    { 2678400, 2505600, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
};

STATIC MUTEX_HANDLE s_time_mutex = NULL;
STATIC SYS_TIME_T s_time_last_ms = 0;
STATIC BOOL_T s_time_tz_sync = FALSE;
STATIC INT_T s_time_tz = 0;
STATIC SUM_ZONE_TBL_S s_time_sz_tbl = {0};
STATIC BOOL_T s_time_cloud_sync = FALSE;
STATIC TIME_T s_time_cloud_posix = 0;
STATIC BOOL_T s_time_disable_update = FALSE;

/***********************************************************
*************************function define********************
***********************************************************/
BOOL_T tal_time_is_in_sum_zone(TIME_T time)
{
    tal_mutex_lock(s_time_mutex);
    UINT_T i = 0;
    for(i = 0;i < s_time_sz_tbl.cnt;i++) {
        if((time >= s_time_sz_tbl.zone[i].posix_min) && (time <= s_time_sz_tbl.zone[i].posix_max)) {
            tal_mutex_unlock(s_time_mutex);
            return TRUE;
        }
    }
    tal_mutex_unlock(s_time_mutex);

    return FALSE;
}

STATIC BOOL_T __is_in_sum_zone(VOID)
{
    TIME_T time = tal_time_get_posix();
    if((time > tal_time_get_posix())) {
        return FALSE;
    }

    return tal_time_is_in_sum_zone(time);
} 

/**
 * Returns 1 if current year id a leap year
 */
STATIC INT_T __is_leap(INT_T yr)
{
    if (!(yr%100))
        return (yr%400 == 0) ? 1 : 0;
    else
        return (yr%4 == 0) ? 1 : 0;
}

STATIC INT_T __is_valid_date_time(CONST POSIX_TM_S *tm)
{
    if (tm->tm_sec > 59)
        return -1;
    if (tm->tm_min > 59)
        return -1;
    if (tm->tm_hour > 23)
        return -1;
    if (((tm->tm_year + 1900) < 1970) || ((tm->tm_year + 1900) > 2111))
        return -1;
    if (tm->tm_mon >= 12)
        return -1;
    if (tm->tm_mon == 1) {
        if (!__is_leap(tm->tm_year + 1900)) {
            if (tm->tm_mday > 28)
                return -1;
        } else  {
            if (tm->tm_mday > 29)
                return -1;
        }
    }
    switch (tm->tm_mon) {
        case 3:
        case 5:
        case 8:
        case 10:
            if(tm->tm_mday > 30)
                return -1;
    }

    if ( (tm->tm_mday < 1) || (tm->tm_mday > 31) )
        return -1;

    return 0;
}

STATIC UCHAR_T __get_day_of_week(UCHAR_T month, UCHAR_T day, USHORT_T year)
{
    /* Month should be a number 0 to 11, Day should be a number 1 to 31 */

    STATIC INT_T t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

STATIC OPERATE_RET __get_time_zone(IN CONST CHAR_T *time_zone,OUT INT_T *posix)
{
    CHAR_T *p = NULL;
    INT_T offset = 1,plus = 1;

    p = strstr(time_zone,"+");
    if(p == NULL) {
        p = strstr(time_zone,"-");
        if(p == NULL) {
            offset = 0;
        }else {
            plus = 0; //-
        }
    }

    INT_T num = 0,buf[2]={0};
    num = sscanf(time_zone+offset,"%d:%d",&buf[0],&buf[1]);
    if(num < 2) {
        return OPRT_INVALID_PARM;
    }
    *posix = (buf[0]*60 + buf[1]) * 60;
    if(plus == 0) {
        *posix = -(*posix);
    }

    return OPRT_OK;
}

/**
 * @brief time-management module initialization
 * 
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h    
 */
OPERATE_RET tal_time_service_init(VOID)
{
    if(s_time_mutex) {
        return OPRT_OK;
    }

    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tal_mutex_create_init(&s_time_mutex);
    if(OPRT_OK != op_ret) {
        return op_ret;
    }
    
    s_time_cloud_posix = 0;
    s_time_last_ms = tal_system_get_millisecond();
    memset(&s_time_sz_tbl,0,SIZEOF(s_time_sz_tbl));

    return OPRT_OK;
}

/**
 * @brief change posix time to TIME_T, redefine the std C func mktime
 * 
 * @param[in] tm the time in posix time format
 *
 * @return the time in TIME_T format 
 */
TIME_T tal_time_mktime(IN CONST POSIX_TM_S *tm)
{
    if(NULL == tm) {
        return 0;
    }

    int i;
    int leapyr=0;

    TIME_T time = 0;

    for (i = 1970; i < (tm->tm_year + 1900); i++)
        time += SEC_PER_YR[__is_leap(i)];

    if (__is_leap(tm->tm_year + 1900))
        leapyr = 1;

    for (i = 0; i < (tm->tm_mon); i++) {
        time += SEC_PER_MT[leapyr][i];
    }

    time += ((tm->tm_mday)-1) * SEC_PER_DAY;
    time += (tm->tm_hour) * SEC_PER_HOUR;
    time += (tm->tm_min) * 60;
    time += tm->tm_sec;

    return time;
}

/**
 * @brief change TIME_T to posix time, redefine the std C func gmtime_r
 * 
 * @param[in] tm the time in TIME_T format
 * @param[out] result the time in posix time format
 * @return the time in posix time format
 */
POSIX_TM_S *tal_time_gmtime_r(IN CONST TIME_T *tm, OUT POSIX_TM_S *result)
{
    int leapyr = 0;
    TIME_T ltm = *tm;

    memset(result, 0, sizeof(POSIX_TM_S));
    result->tm_year = 1970;

    while(1) {
        if(ltm < SEC_PER_YR[__is_leap(result->tm_year)]) {
            break;
        }
        ltm -= SEC_PER_YR[__is_leap(result->tm_year)];
        result->tm_year++;
    }

    leapyr = __is_leap(result->tm_year);

    while(1) {
        if(ltm < SEC_PER_MT[leapyr][result->tm_mon])
            break;
        ltm -= SEC_PER_MT[leapyr][result->tm_mon];
        result->tm_mon++;
    }

    result->tm_mday = ltm / SEC_PER_DAY;
    result->tm_mday++;
    ltm = ltm % SEC_PER_DAY;

    result->tm_hour = ltm / SEC_PER_HOUR;
    ltm = ltm % SEC_PER_HOUR;

    result->tm_min = ltm / 60;
    result->tm_sec = ltm % 60;

    result->tm_wday = __get_day_of_week(result->tm_mon + 1, result->tm_mday, result->tm_year);

    /*
     * Solve bug WMSDK-27. 'man gmtime' says:
     * "tm_year   The number of years since 1900."
     */
    result->tm_year -= 1900;

    return result;
}

/**
 * @brief set IoTOS UTC time
 * 
 * @param[in] tm the new time in posix time format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h   
 */
OPERATE_RET tal_time_set(IN CONST POSIX_TM_S *tm)
{
    TIME_T time;
    if(__is_valid_date_time(tm) == 0) {
        time = tal_time_mktime(tm);
        return tal_time_set_posix(time, 2);
    } else {
        return OPRT_COM_ERROR;
    }
}

/**
 * @brief get IoTOS UTC time in posix time format
 * 
 * @param[out] tm the IoTOS UTC time in posix time format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h   
 */
OPERATE_RET tal_time_get(OUT POSIX_TM_S *tm)
{
    if(NULL == tm) {
        return OPRT_INVALID_PARM;
    }

    TIME_T curtime = tal_time_get_posix();
    if(tal_time_gmtime_r((const TIME_T *)&curtime, tm) == NULL) {
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @brief set IoTOS UTC time in time_t format
 * 
 * @param[in] time the new time in time_t format
 * @param[in] update_source the source of time (0:rtc 1:cloud 2:other)
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h    
 */
OPERATE_RET tal_time_set_posix(IN CONST TIME_T time, IN CONST INT_T update_source)
{
    if(!s_time_disable_update) { //for aging test
        tal_mutex_lock(s_time_mutex);
        s_time_cloud_posix = time;
        s_time_last_ms = tal_system_get_millisecond();
        tal_mutex_unlock(s_time_mutex);

        if(update_source == 1) {
            s_time_cloud_sync = TRUE;
        }
    }

    return OPRT_OK;
}


/**
 * @brief get IoTOS last synchronized UTC time in TIME_T format
 * 
 * @return the time in TIME_T format 
 */
TIME_T tal_time_get_cur_posix(VOID)
{
    return s_time_cloud_posix;
}

/**
 * @brief check IoTOS time synchronize status
 * 
 * @return OPRT_OK on synchronized. Others on not
 */
OPERATE_RET tal_time_check_time_sync(VOID)
{
    if(s_time_cloud_sync == FALSE)
    {
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

/**
 * @brief check IoTOS time zone synchronized status
 * 
 * @return OPRT_OK on synchronized. Others on not 
 */
OPERATE_RET tal_time_check_time_zone_sync(VOID)
{
    if(s_time_tz_sync == FALSE)
    {
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}


/**
 * @brief get IoTOS UTC time in TIME_T format
 * 
 * @return the current second time in TIME_T format 
 */
TIME_T tal_time_get_posix(VOID)
{
    TIME_T tmp_cur_posix_time = 0;

    tal_mutex_lock(s_time_mutex);
    SYS_TIME_T curr_time_ms = tal_system_get_millisecond();

    if(s_time_last_ms > curr_time_ms) { // recycle
        s_time_cloud_posix += ((0x100000000 - s_time_last_ms) / 1000);
        s_time_last_ms = 0;
    }

    tmp_cur_posix_time = s_time_cloud_posix;
    tmp_cur_posix_time += (curr_time_ms-s_time_last_ms) / 1000;
    tal_mutex_unlock(s_time_mutex);

    return tmp_cur_posix_time;
}

/**
 * @brief get IoTOS UTC time in SYS_TICK_T format
 * 
 * @return the current micro-second time in SYS_TICK_T format 
 */
SYS_TICK_T tal_time_get_posix_ms(VOID)
{
    SYS_TIME_T tmp_cur_posix_time_ms = 0;

    tal_mutex_lock(s_time_mutex);
    SYS_TIME_T curr_time_ms = tal_system_get_millisecond();

    if (s_time_last_ms > curr_time_ms) { // recycle
        s_time_cloud_posix += ((0x100000000 - s_time_last_ms) / 1000);
        s_time_last_ms = 0;
    }

    tmp_cur_posix_time_ms  = (SYS_TICK_T)s_time_cloud_posix * 1000;
    tmp_cur_posix_time_ms += (curr_time_ms - s_time_last_ms);
    tal_mutex_unlock(s_time_mutex);

    return tmp_cur_posix_time_ms;
}

/**
 * @brief how long times system run
 * 
 * @param[out] pSecTime the current time in second
 * @param[out] pMsTime the current time in micro-second
 * @return VOID 
 */
VOID tal_time_get_system_time(OUT TIME_S *pSecTime, OUT TIME_MS *pMsTime)
{
    STATIC SYS_TIME_T last_ms     = 0;
    STATIC SYS_TIME_T roll_ms = 0;
    SYS_TIME_T curr_ms = 0;

    tal_mutex_lock(s_time_mutex);

    curr_ms = tal_system_get_millisecond();
    if (last_ms > curr_ms) { // recycle 
        roll_ms += 0x100000000;
    }
    if (pMsTime) {
        *pMsTime  = (curr_ms + roll_ms) % 1000;
    }
    if (pSecTime) {
        *pSecTime = (curr_ms + roll_ms) / 1000;
    }
    last_ms = curr_ms;

    tal_mutex_unlock(s_time_mutex);
}


/**
 * @brief set IoTOS time zone in "+/-hh:mm" format
 * 
 * @param[in] time_zone the new time zone in "+/-hh:mm" format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_set_time_zone(IN CONST CHAR_T *time_zone)
{
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = __get_time_zone(time_zone,&s_time_tz);
    if(op_ret != OPRT_OK) {
        return op_ret;
    }
    s_time_tz_sync = TRUE;
    
    return OPRT_OK;
}

/**
 * @brief get IoTOS time zone in second format
 * 
 * @param[out] time_zone the current time zone in second format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_get_time_zone_seconds(OUT INT_T *time_zone)
{
    *time_zone = s_time_tz;
    return OPRT_OK;
}

/**
 * @brief set IoTOS time zone in second format
 * 
 * @param[in] time_zone the new time zone in second format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_set_time_zone_seconds(IN INT_T time_zone_sec)
{
    s_time_tz = time_zone_sec;
    s_time_tz_sync = TRUE;
    return OPRT_OK;
}

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
OPERATE_RET tal_time_get_local_time_custom(IN TIME_T in_time, OUT POSIX_TM_S *tm)
{
    if(NULL == tm) {
        return OPRT_INVALID_PARM;
    }

    TIME_T local_time = 0;
    if(in_time == 0) {
        local_time = tal_time_get_posix() + s_time_tz;
    }else {
        local_time = in_time + s_time_tz;
    }

    if(TRUE == __is_in_sum_zone()) {
        local_time += SEC_PER_HOUR;
    }

    if(tal_time_gmtime_r((const TIME_T *)&local_time, tm) == NULL) {
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}


/**
 * @brief set IoTOS summer time zone
 * 
 * @param[in] zone the summer time zone table
 * @param[in] cnt the summer time counts
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID tal_time_set_sum_zone_tbl(IN CONST SUM_ZONE_S *zone, IN CONST UINT_T cnt)
{
    if(NULL == zone || 0 == cnt) {
        s_time_sz_tbl.cnt = 0;
        return;
    }
    
    tal_mutex_lock(s_time_mutex);
    s_time_sz_tbl.cnt = cnt;
    if(cnt > SUM_ZONE_TAB_LMT) {
        s_time_sz_tbl.cnt = SUM_ZONE_TAB_LMT;
    }

    memcpy(s_time_sz_tbl.zone, zone, SIZEOF(SUM_ZONE_S) * s_time_sz_tbl.cnt);

    tal_mutex_unlock(s_time_mutex);
    return;
}

/**
 * @brief get IoTOS UTC summer time in posix time format
 * 
 * @param[out] tm the summer time in posix format
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_time_get_sum_time(OUT POSIX_TM_S *tm)
{
    TIME_T time = tal_time_get_sum_time_posix();

    if(tal_time_gmtime_r((const TIME_T *)&time, tm) == NULL) {
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @brief get IoTOS UTC summer time int TIME_T format 
 * 
 * @return the summer time in TIME_T format
 */
TIME_T tal_time_get_sum_time_posix(VOID)
{
    TIME_T time = tal_time_get_posix();

    if(TRUE == __is_in_sum_zone()) {
        time += SEC_PER_HOUR;
    }

    return time;
}

/**
 * @brief get sum zone info
 * 
 * @param[out] sum zone info
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h     
 */
OPERATE_RET tal_time_get_sum_zone(OUT SUM_ZONE_TBL_S *sum_zone)
{
    if(NULL == sum_zone){
        return OPRT_INVALID_PARM;
    }
    
    tal_mutex_lock(s_time_mutex);
    memcpy(sum_zone,&s_time_sz_tbl,sizeof(SUM_ZONE_TBL_S));
    tal_mutex_unlock(s_time_mutex);
    
    return OPRT_OK;
}

OPERATE_RET tal_time_config_update(BOOL_T disable)
{
    s_time_disable_update = disable;
    return OPRT_OK;
}

