/**
 * @file tal_log.h
 * @brief This is tal_log file
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_LOG_H__
#define __TAL_LOG_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/
/**
 * @brief Definition of log level
 */
typedef enum {
    TAL_LOG_LEVEL_ERR,
    TAL_LOG_LEVEL_WARN,
    TAL_LOG_LEVEL_NOTICE,
    TAL_LOG_LEVEL_INFO,
    TAL_LOG_LEVEL_DEBUG,
    TAL_LOG_LEVEL_TRACE,
} TAL_LOG_LEVEL_E;

typedef TAL_LOG_LEVEL_E LOG_LEVEL;

#if defined(MAX_SIZE_OF_DEBUG_BUF)
#define DEF_LOG_BUF_LEN     MAX_SIZE_OF_DEBUG_BUF
#else
#define DEF_LOG_BUF_LEN     4096
#endif

#ifdef ENABLE_PRINTF_CHECK
#define PRINTF_CHECK(formatArg, firstVarArg)          __attribute__((format(printf, formatArg, firstVarArg)))
#else
#define PRINTF_CHECK(...)
#endif

PRINTF_CHECK(4, 5)
OPERATE_RET tal_log_print(CONST TAL_LOG_LEVEL_E         level,
                          CONST CHAR_T                  *file,
                          CONST INT32_T                   line,
                          CHAR_T                        *fmt,
                          ...);

// file name maybe define from complie parameter
#ifndef _THIS_FILE_NAME_
#define _THIS_FILE_NAME_ __FILE__
#endif

#define PR_ERR(fmt, ...)                        tal_log_print(TAL_LOG_LEVEL_ERR,     _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)
#define PR_WARN(fmt, ...)                       tal_log_print(TAL_LOG_LEVEL_WARN,    _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)
#define PR_NOTICE(fmt, ...)                     tal_log_print(TAL_LOG_LEVEL_NOTICE,  _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)
#define PR_INFO(fmt, ...)                       tal_log_print(TAL_LOG_LEVEL_INFO,    _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...)                      tal_log_print(TAL_LOG_LEVEL_DEBUG,    _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)
#define PR_TRACE(fmt, ...)                      tal_log_print(TAL_LOG_LEVEL_TRACE,    _THIS_FILE_NAME_,  __LINE__, fmt, ##__VA_ARGS__)

#define PR_HEXDUMP_ERR(title, buf, size)        tal_log_hex_dump(TAL_LOG_LEVEL_ERR,     _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEXDUMP_WARN(title, buf, size)       tal_log_hex_dump(TAL_LOG_LEVEL_WARN,    _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEXDUMP_NOTICE(title, buf, size)     tal_log_hex_dump(TAL_LOG_LEVEL_NOTICE,  _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEXDUMP_INFO(title, buf, size)       tal_log_hex_dump(TAL_LOG_LEVEL_INFO,    _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEXDUMP_DEBUG(title, buf, size)      tal_log_hex_dump(TAL_LOG_LEVEL_DEBUG,    _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEXDUMP_TRACE(title, buf, size)      tal_log_hex_dump(TAL_LOG_LEVEL_TRACE,    _THIS_FILE_NAME_,  __LINE__, title, 64, buf, size)
#define PR_HEX_DUMP(title, width, buf, size)    tal_log_hex_dump(TAL_LOG_LEVEL_NOTICE, __FILE__, __LINE__, title, width, buf, size)

#define PR_DEBUG_RAW(fmt, ...)                  tal_log_print_raw(fmt, ##__VA_ARGS__)
#define PR_TRACE_ENTER()                        PR_TRACE("enter [%s]",  (const char*)__func__)
#define PR_TRACE_LEAVE()                        PR_TRACE(("leave [%s]", (const char*)__func__))


/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/
// prototype of log output function
typedef VOID (*TAL_LOG_OUTPUT_CB)(IN CONST  CHAR_T *str);

/***********************************************************************
 ********************* variable ****************************************
 **********************************************************************/


/***********************************************************************
 ********************* function ****************************************
 **********************************************************************/


/**
 * @brief initialize log management.
 *
 * @param[in] level , set log level
 * @param[in] buf_len , set log buffer size
 * @param[in] output , log print function pointer
 *
 * @note This API is used for initializing log management.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_init(CONST TAL_LOG_LEVEL_E level, CONST INT32_T buf_len, CONST TAL_LOG_OUTPUT_CB output);

/**
 * @brief add one output terminal.
 *
 * @param[in] name , terminal name
 * @param[in] term , output function pointer
 *
 * @note This API is used for adding one output terminal.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_add_output_term(CONST CHAR_T *name, CONST TAL_LOG_OUTPUT_CB term);

/**
 * @brief delete one output terminal.
 *
 * @param[in] name , terminal name
 *
 * @note This API is used for delete one output terminal.
 *
 * @return NONE
 */
VOID tal_log_del_output_term(CONST CHAR_T *name);

/**
 * @brief set global log level.
 *
 * @param[in] curLogLevel , log level
 *
 * @note This API is used for setting global log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_set_level(CONST TAL_LOG_LEVEL_E level);

/**
 * @brief set log time whether show in millisecond.
 *
 * @param[in] if_ms_level, whether log time include millisecond
 *
 * @note This API is used for setting log time whether include milisecond.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_set_ms_info(BOOL_T if_ms_level);

/**
 * @brief get global log level.
 *
 * @param[in] pCurLogLevel, global log level
 *
 * @note This API is used for getting global log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_get_level(TAL_LOG_LEVEL_E *level);

/**
 * @brief add one module's log level
 *
 * @param[in] module_name, module name
 * @param[in] logLevel, this module's log level
 *
 * @note This API is used for adding one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_add_module_level(CONST PCHAR_T module_name, CONST TAL_LOG_LEVEL_E level);

/**
 * @brief This API is used for adding one module's log level.
 *
 * @param[in] module_name: module_name
 * @param[in] level: level
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_set_module_level(CONST PCHAR_T module_name, TAL_LOG_LEVEL_E level);
/**
 * @brief get one module's log level
 *
 * @param[in] pModuleName, module name
 * @param[in] logLevel, this module's log level
 *
 * @note This API is used for getting one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_get_module_level(CONST PCHAR_T module_name, TAL_LOG_LEVEL_E *level);

/**
 * @brief delete one module's log level
 *
 * @param[in] pModuleName, module name
 *
 * @note This API is used for deleting one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_delete_module_level(CONST PCHAR_T module_name);

PRINTF_CHECK(1, 2)

/**
 * @brief This API is used for print only user log info.
 *
 * @param[in] pFmt: format string
 * @param[in] ...: parameter
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tal_log_print_raw(CONST PCHAR_T pFmt,...);

/**
 * @brief destroy log management
 *
 * @param[in] pFmt, format string
 * @param[in] ..., parameter
 *
 * @note This API is used for destroy log management.
 *
 * @return NONE
 */
VOID tal_log_release(VOID);

/**
 * @brief print a buffer in hex format
 *
 * @param[in] title, buffer title for print
 * @param[in] width, one line width
 * @param[in] buf, buffer address
 * @param[in] size, buffer size
 *
 * @note This API is used for print one buffer.
 *
 * @return NONE
 */
VOID tal_log_hex_dump(CONST TAL_LOG_LEVEL_E     level,
                      CONST CHAR_T              *file,
                      CONST INT32_T               line,
                      CONST CHAR_T              *title,
                      UINT8_T                   width,
                      UINT8_T                   *buf,
                      UINT16_T                  size);


#ifdef __cplusplus
}
#endif /* __TAL_LOG_H__ */

#endif
