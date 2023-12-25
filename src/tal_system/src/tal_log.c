#include <string.h>
#include <stdio.h>
#include "tal_log.h"
#include "tuya_list.h"
#include "tal_mutex.h"
#include "tal_system.h"
#include "tal_time_service.h"
#include "tal_memory.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define LOG_LEVEL_MIN		0
#define LOG_LEVEL_MAX		5

typedef struct {
    LIST_HEAD            node;
    CHAR_T              *name;
    TAL_LOG_OUTPUT_CB    out_term;
} LOG_OUT_NODE_S;

typedef struct {
    LOG_LEVEL       curLogLevel;          
    LIST_HEAD       listHead;             
    LIST_HEAD       log_list;             
    MUTEX_HANDLE    mutex;

    INT_T           log_buf_len;
    BOOL_T          ms_level;
    CHAR_T         *log_buf;
} LOG_MANAGE, *P_LOG_MANAGE;

#define DEF_OUTPUT_NAME "def_output"

/***********************************************************
*************************variable define********************
***********************************************************/
CONST PCHAR_T sLevelStr[] = { "E", "W", "N", "I", "D", "T"};
P_LOG_MANAGE pLogManage = NULL;

/***********************************************************
*************************function define********************
***********************************************************/
OPERATE_RET tal_log_init(CONST TAL_LOG_LEVEL_E level, CONST INT_T buf_len, CONST TAL_LOG_OUTPUT_CB output)
{
    if (level < LOG_LEVEL_MIN || level > LOG_LEVEL_MAX ||  0 == buf_len || NULL == output) {
       return OPRT_INVALID_PARM;
    }

    if (!pLogManage) {
        OPERATE_RET op_ret = OPRT_OK;

        P_LOG_MANAGE tmp_log_mng = (P_LOG_MANAGE)tal_malloc(SIZEOF(LOG_MANAGE) + buf_len + 1);
        if (!tmp_log_mng) {
            return OPRT_MALLOC_FAILED; 
        }
        tmp_log_mng->log_buf_len = buf_len;
        tmp_log_mng->log_buf     = (CHAR_T *)(tmp_log_mng + 1);
        op_ret = tal_mutex_create_init(&tmp_log_mng->mutex);
        if (OPRT_OK != op_ret) {
            tal_free(tmp_log_mng);
            return op_ret;
        }
        INIT_LIST_HEAD(&(tmp_log_mng->listHead));
        INIT_LIST_HEAD(&(tmp_log_mng->log_list));
        tmp_log_mng->curLogLevel = level;
        tmp_log_mng->ms_level    = FALSE;
        pLogManage = tmp_log_mng;
        op_ret = tal_log_add_output_term(DEF_OUTPUT_NAME, output);
        if (OPRT_OK != op_ret) {
            tal_mutex_release(tmp_log_mng->mutex);
            tal_free(tmp_log_mng);
            return op_ret;
        }
    } else {
        pLogManage->curLogLevel = level;
    }

    return OPRT_OK;
}

void __output_logManage_buf(void)
{
    P_LIST_HEAD pPos;
    LOG_OUT_NODE_S *output_node;
    tuya_list_for_each(pPos, &(pLogManage->log_list)) {
        output_node = tuya_list_entry(pPos, LOG_OUT_NODE_S, node);
        if (output_node->out_term) {
            output_node->out_term(pLogManage->log_buf);
        }
    }
}

OPERATE_RET __find_out_term_node(CONST CHAR_T *name, LOG_OUT_NODE_S **node)
{
    P_LIST_HEAD pPos;
    LOG_OUT_NODE_S *output_node;

    tuya_list_for_each(pPos, &(pLogManage->log_list)) {
        output_node = tuya_list_entry(pPos, LOG_OUT_NODE_S, node);
        if (!strcmp(output_node->name,name)) {
            *node = output_node;
            return OPRT_OK;
        }
    }

    return OPRT_COM_ERROR;
}

OPERATE_RET tal_log_add_output_term(CONST CHAR_T *name, CONST TAL_LOG_OUTPUT_CB term)
{
    if (NULL == name || NULL == term || NULL == pLogManage) {
        return OPRT_INVALID_PARM;
    }

    LOG_OUT_NODE_S *output_node;
    OPERATE_RET ret = __find_out_term_node(name, &output_node);
    if (ret == OPRT_OK) {
        output_node->out_term = term;
        return OPRT_OK;
    }

    NEW_LIST_NODE(LOG_OUT_NODE_S, output_node); 
    if (!output_node) {
        return OPRT_MALLOC_FAILED;
    }
    output_node->name = tal_malloc(strlen(name) + 1); 
    if (!output_node->name) {
        tal_free(output_node);
        return OPRT_MALLOC_FAILED;
    }
    strcpy(output_node->name, name);
    output_node->out_term = term;
    tuya_list_add(&(output_node->node), &(pLogManage->log_list));

    return OPRT_OK;
}

STATIC INT_T tal_log_strrchr(CHAR_T *str, CHAR_T ch)
{
    CHAR_T *ta;

    ta = strrchr(str, ch);
    if (ta) {
        return (INT_T)(ta - str);
    }

    return -1;
}

VOID tal_log_del_output_term(CONST CHAR_T *name)
{
    if (NULL == name || NULL == pLogManage) {
        return;
    }
    tal_mutex_lock(pLogManage->mutex);
    LOG_OUT_NODE_S *output_node;
    OPERATE_RET ret = __find_out_term_node(name, &output_node);
    if (ret == OPRT_OK) {
        tuya_list_del(&(output_node->node));
        tal_free(output_node->name); 
        tal_free(output_node); 
    }
    tal_mutex_unlock(pLogManage->mutex);
    return;
}

OPERATE_RET tal_log_set_ms_info(BOOL_T if_ms_level)
{
    if (!pLogManage) {
        return OPRT_INVALID_PARM;
    }
    pLogManage->ms_level = if_ms_level;

    return OPRT_OK;
}

OPERATE_RET tal_log_set_level(CONST TAL_LOG_LEVEL_E level)
{
    if (!pLogManage) {
        return OPRT_INVALID_PARM;
    }
    
    if (level < LOG_LEVEL_MIN || level > LOG_LEVEL_MAX) {
       return OPRT_INVALID_PARM;
    }

    pLogManage->curLogLevel = level;

    return OPRT_OK;
}

OPERATE_RET tal_log_get_level(TAL_LOG_LEVEL_E *level)
{
    if (!pLogManage) {
        return OPRT_INVALID_PARM;
    }
    *level = pLogManage->curLogLevel;

    return OPRT_OK;
}

OPERATE_RET PrintLogV(LOG_LEVEL      logLevel,
                      CHAR_T        *pFile,
                      UINT_T         line,
                      CHAR_T        *pFmt,
                      va_list        ap)
{
    if (!pLogManage) {
        return OPRT_INVALID_PARM;
    }
    if (logLevel < LOG_LEVEL_MIN || logLevel > LOG_LEVEL_MAX) {
       return OPRT_INVALID_PARM;
    }
    LOG_LEVEL tmpLogLevel = pLogManage->curLogLevel;   
    if(logLevel > tmpLogLevel)  { 
        return OPRT_BASE_LOG_MNG_PRINT_LOG_LEVEL_HIGHER;
    }
    const char *pTmpModuleName = "ty";
    const char *pTmpFilename = NULL;

    if (NULL == pFile) { 
        pTmpFilename = "Null";
    } else {
        int pos = 0;
        pTmpFilename = pFile;
        pos = tal_log_strrchr((CHAR_T *)pFile,'/');
        if (pos < 0) {
            pos = tal_log_strrchr((CHAR_T *)pFile, '\\');
            if(pos >= 0) {
                pTmpFilename = pFile + pos + 1;
            }
        } else {
            pTmpFilename = pFile + pos + 1;
        }
    }
    tal_mutex_lock(pLogManage->mutex);
    POSIX_TM_S tm;
    memset(&tm, 0, SIZEOF(tm));

    INT_T len = 0;
    INT_T cnt = 0;
    if (pLogManage->ms_level == FALSE) {
        tal_time_get_local_time_custom(0, &tm);
        cnt = snprintf(pLogManage->log_buf, pLogManage->log_buf_len, "[%02d-%02d %02d:%02d:%02d %s %s][%s:%d] ",
                       tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 
                       pTmpModuleName, sLevelStr[logLevel], pTmpFilename, line);
    } else {
        SYS_TICK_T time_ms = tal_time_get_posix_ms();
        TIME_T sec = (TIME_T)(time_ms / 1000);
        UINT_T ms  = (UINT_T)(time_ms % 1000);
        tal_time_get_local_time_custom(sec, &tm);
        cnt = snprintf(pLogManage->log_buf, pLogManage->log_buf_len, "[%02d-%02d %02d:%02d:%02d:%d %s %s][%s:%d] ",
                       tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,ms, 
                       pTmpModuleName, sLevelStr[logLevel], pTmpFilename, line);
    }
    if (cnt <= 0 ) {
        goto ERR_EXIT;
    }
    len = cnt;
    cnt = vsnprintf(pLogManage->log_buf + len,pLogManage->log_buf_len - len, pFmt, ap);
    if ( cnt <= 0 ) {
        goto ERR_EXIT;
    }
    len += cnt;
    if (len > (INT_T)(pLogManage->log_buf_len - 3) ) {  // 2
        len = pLogManage->log_buf_len - 3;
    }
    pLogManage->log_buf[len]     = '\r';
    pLogManage->log_buf[len + 1] = '\n';
    pLogManage->log_buf[len + 2] = 0;
    __output_logManage_buf();
    tal_mutex_unlock(pLogManage->mutex);

    return OPRT_OK;

ERR_EXIT:
    tal_mutex_unlock(pLogManage->mutex);
    return OPRT_BASE_LOG_MNG_FORMAT_STRING_FAILED;

}

OPERATE_RET tal_log_print(CONST TAL_LOG_LEVEL_E     level,
                          CONST CHAR_T              *file, 
                          CONST INT_T               line, 
                          CHAR_T              *fmt,
                          ...)
{
	OPERATE_RET opRet = 0;
	va_list ap;
	va_start(ap, fmt);
	opRet = PrintLogV(level, (CHAR_T*)file, line, fmt, ap);
	va_end(ap);

	return opRet;
}

STATIC OPERATE_RET __PrintLogVRaw(IN CONST PCHAR_T pFmt,va_list ap)
{
    INT_T cnt = 0;
    cnt = vsnprintf(pLogManage->log_buf,pLogManage->log_buf_len,pFmt,ap);
    if (cnt <= 0) {
      return OPRT_BASE_LOG_MNG_FORMAT_STRING_FAILED;
    }
    __output_logManage_buf();

    return OPRT_OK;
}

OPERATE_RET tal_log_print_raw(CONST PCHAR_T pFmt,...)
{
    if(NULL == pLogManage) {
        return OPRT_INVALID_PARM;
    }

    OPERATE_RET opRet = 0;
    va_list ap;

    tal_mutex_lock(pLogManage->mutex);
    va_start(ap, pFmt);
    opRet = __PrintLogVRaw(pFmt,ap);
    va_end(ap);
    tal_mutex_unlock(pLogManage->mutex);

    return opRet;
}

VOID tal_log_release(VOID)
{
    if (!pLogManage) {
        return;
    }

    while(!tuya_list_empty(&(pLogManage->log_list))) {
        LOG_OUT_NODE_S *log_out_nd = NULL;
        log_out_nd = tuya_list_entry(&(pLogManage->log_list.next), LOG_OUT_NODE_S, node);
        tuya_list_del(&(log_out_nd->node));
        if (log_out_nd->name) {
            tal_free(log_out_nd->name); 
        }
        tal_free(log_out_nd);
    }
    tal_free(pLogManage); 
    pLogManage = NULL;
}


VOID tal_log_hex_dump(CONST TAL_LOG_LEVEL_E     level,
                      CONST CHAR_T              *file,
                      CONST INT_T               line,
                      CONST CHAR_T              *title,
                      UINT8_T                   width,
                      UINT8_T                   *buf,
                      UINT16_T                  size)
{
    int i = 0;

    if(!pLogManage || level > pLogManage->curLogLevel)  { 
        return;
    }

    if (width >= 64) {
        width = 64;
    }
    tal_log_print(level, file, line, "%s %d <%p>", title, size, buf);
    for (i = 0; i < size; i++) {
        tal_log_print_raw("%02x ", buf[i]&0xFF);
        if ((i+1)%width == 0) {
            tal_log_print_raw("\r\n");
        }
    }
    tal_log_print_raw("\r\n\r\n");
}
