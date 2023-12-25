
#include "tuya_cloud_types.h"
#include "tal_kv.h"
#include "tal_api.h"
#include "cJSON.h"
#include "mix_method.h"

int kv_serialize(IN CONST kv_db_t *db, IN CONST UINT_T dbcnt, OUT CHAR_T **out, OUT UINT_T *out_len)
{
    INT_T i = 0;
    // conut need buf size
    UINT_T len = 0;
    for (i = 0; i < dbcnt; i++) {
        // value len
        if (db[i].tp <= KV_INT) {
            len += 11 + 6;
        } else if (db[i].tp == KV_BOOL) {
            len += 6 + 6;
        } else if (db[i].tp == KV_STRING) {
            CHAR_T *tmp = (CHAR_T *)db[i].val;
            if (0 == tmp[0]) {
                len += db[i].len + 4 + 4;//"id":null,
            } else {
                len += db[i].len + 6;//"swv":"4.1.16",
            }

        } else if (db[i].tp == KV_RAW) {
            // len += (db[i].len / 3) * 4 + ((db[i].len % 3) ? 4 : 0) + 6;
            len += (db[i].len / 3) * 4 + ((db[i].len % 3) ? 4 : 0) ;
            len += (db[i].len ? 6 : 4 + 4);
        }

        // key len
        len += strlen(db[i].key) + 2; // 2 == ""
    }
    len += 3; // 3 == {}\0

    UINT_T offset = 0;
    CHAR_T *buf = tal_malloc(len);
    if (NULL == buf) {
        PR_ERR("maloc fails %d", len);
        return OPRT_MALLOC_FAILED;
    }
    offset = sprintf(buf, "{");

    for (i = 0; i < dbcnt; i++) {
        // add key
        offset += sprintf(buf + offset, "\"%s\":", db[i].key);

        // add value
        switch (db[i].tp) {
        case KV_CHAR: {
            offset += sprintf(buf + offset, "%d", *((CHAR_T *)(db[i].val)));
        }
        break;
        case KV_BYTE: {
            offset += sprintf(buf + offset, "%d", *((BYTE_T *)(db[i].val)));
        }
        break;
        case KV_SHORT: {
            offset += sprintf(buf + offset, "%d", *((SHORT_T *)(db[i].val)));
        }
        break;
        case KV_USHORT: {
            offset += sprintf(buf + offset, "%d", *((USHORT_T *)(db[i].val)));
        }
        break;
        case KV_INT: {
            offset += sprintf(buf + offset, "%d", *((INT_T *)(db[i].val)));
        }
        break;
        case KV_BOOL: {
            if (FALSE == *((BOOL_T *)(db[i].val))) {
                offset += sprintf(buf + offset, "false");
            } else {
                offset += sprintf(buf + offset, "true");
            }
        }
        break;

        case KV_STRING: {
            CHAR_T *tmp = (CHAR_T *)db[i].val;
            if (0 == tmp[0]) {
                offset += sprintf(buf + offset, "null");
            } else {
                offset += sprintf(buf + offset, "\"%s\"", (CHAR_T *)db[i].val);
            }
        }
        break;

        case KV_RAW: {
            if (0 == db[i].len) {
                offset += sprintf(buf + offset, "null");
            } else {
                offset += sprintf(buf + offset, "\"");
                tuya_base64_encode((unsigned char *)db[i].val, (buf + offset), db[i].len);
                offset += strlen((buf + offset));
                buf[offset] = '\"';
                offset++;
            }
        }
        break;

        default: {
            PR_ERR("type invalid %d", db[i].tp);
            tal_free(buf);
            return OPRT_COM_ERROR;
        }
        }

        offset += sprintf(buf + offset, ",");
    }

    if (i != 0) {
        /* 退出循环时后面会多加一个‘，’ 这里重置下 */
        buf[offset - 1] = ' ';
    }

    buf[offset++] = '}';
    buf[offset] = 0;

    *out = buf;
    *out_len = offset;

    return OPRT_OK;
}

int kv_deserialize(IN CONST CHAR_T *in, INOUT kv_db_t *db, IN CONST UINT_T dbcnt)
{
    cJSON *root = cJSON_Parse(in);
    if (NULL == root) {
        PR_ERR("json parse fails %s", in);
        return OPRT_CJSON_PARSE_ERR;
    }

    int op_ret = OPRT_OK;
    INT_T i = 0;
    cJSON *json = NULL;
    for (i = 0; i < dbcnt; i++) {
        json = cJSON_GetObjectItem(root, db[i].key);
        if (json != NULL) {
            if (db[i].tp <= KV_INT && (cJSON_Number != json->type)) {
                op_ret = OPRT_CJSON_GET_ERR;
                goto ERR_EXIT;
            } else if (db[i].tp == KV_BOOL && (json->type > cJSON_True)) {
                op_ret = OPRT_CJSON_GET_ERR;
                goto ERR_EXIT;
            } else if ((db[i].tp == KV_STRING || db[i].tp == KV_RAW) && \
                       (json->type != cJSON_String && json->type != cJSON_NULL)) {
                op_ret = OPRT_CJSON_GET_ERR;
                goto ERR_EXIT;
            }
        } else { // default set zero
            memset(db[i].val, 0, db[i].len);
            continue;
        }

        switch (db[i].tp) {
        case KV_CHAR: {
            if (json->valueint < -128 || json->valueint > 127) {
                op_ret = OPRT_COM_ERROR;
                goto ERR_EXIT;
            }
            *((CHAR_T *)db[i].val) = json->valueint;
        }
        break;

        case KV_BYTE: {
            if (json->valueint < 0 || json->valueint > 255) {
                op_ret = OPRT_COM_ERROR;
                goto ERR_EXIT;
            }
            *((BYTE_T *)db[i].val) = json->valueint;
        }
        break;

        case KV_SHORT: {
            if (json->valueint < -32768 || json->valueint > 32767) {
                op_ret = OPRT_COM_ERROR;
                goto ERR_EXIT;
            }
            *((SHORT_T *)db[i].val) = json->valueint;
        }
        break;

        case KV_USHORT: {
            if (json->valueint < 0 || json->valueint > 65535) {
                op_ret = OPRT_COM_ERROR;
                goto ERR_EXIT;
            }
            *((USHORT_T *)db[i].val) = json->valueint;
        }
        break;

        case KV_INT: {
            *((INT_T *)db[i].val) = json->valueint;
        }
        break;

        case KV_BOOL: {
            *((BOOL_T *)db[i].val) = (json->type == cJSON_False) ? 0 : 1;
        }
        break;

        case KV_STRING: {
            if (json->type == cJSON_NULL) {
                CHAR_T *tmp = (CHAR_T *)db[i].val;
                tmp[0] = 0;
            } else {
                UINT_T len = strlen(json->valuestring) + 1;
                if (db[i].len < len) {
                    op_ret = OPRT_COM_ERROR;
                    goto ERR_EXIT;
                }
                memcpy(db[i].val, json->valuestring, len);
            }
        }
        break;

        case KV_RAW: {
            if (json->type == cJSON_NULL) {
                db[i].len = 0;
            } else {
                tuya_base64_decode(json->valuestring, db[i].val);
            }
        }
        break;

        default: {
            PR_ERR("type invalid %d", db[i].tp);
            op_ret = OPRT_COM_ERROR;
            goto ERR_EXIT;
        }
        }
    }

    cJSON_Delete(root);
    return OPRT_OK;

ERR_EXIT:
    cJSON_Delete(root);
    PR_ERR("deserial fails %d", op_ret);

    return op_ret;
}
