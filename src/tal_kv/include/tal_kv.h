
#ifndef __TAL_KV_H__
#define __TAL_KV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

/**
 * @brief tuya key-value database type define, used for serialize/deserialize data to json format
 *
 */
typedef uint8_t kv_tp_t;
#define KV_CHAR     0       // char, same as int, need 11+6 byte
#define KV_BYTE     1       // byte, same as int, need 11+6 byte
#define KV_SHORT    2       // short, same as int, need 11+6 byte
#define KV_USHORT   3       // unsigned short, same as int, need 11+6 byte
#define KV_INT      4       // int, need 11+6 byte when serialize to json format
#define KV_BOOL     5       // bool, need 6+6 byte when serialize to json format
#define KV_STRING   6       // string, need len+6 byte when serialize to json format
#define KV_RAW      7       // raw, need len/3*4+len%3?4:0+6 (base64 encode) when serialize to json format

/**
 * @brief tuya key-value database, used for serialize/deserialize data to json format
 *
 */
typedef struct {
    char       *key;        // property name
    kv_tp_t     tp;         // property type
    void       *val;        // property value
    uint16_t    len;        // property length
} kv_db_t;


#define TAL_LV_KEY_LEN      16

typedef struct {
    char    seed[TAL_LV_KEY_LEN + 1];
    char    key [TAL_LV_KEY_LEN + 1];
} tal_kv_cfg_t;

int tal_kv_init(tal_kv_cfg_t *kv_cfg);
int tal_kv_set(const char* key, const uint8_t *value, size_t length);
int tal_kv_get(const char* key, uint8_t **value, size_t *length);
int tal_kv_free(uint8_t *value);
int tal_kv_del(const char* key);

int tal_kv_serialize_set(const char* key, kv_db_t *db, size_t dbcnt);
int tal_kv_serialize_get(const char* key, kv_db_t *db, size_t dbcnt);

void tal_kv_cmd(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif //__TIMER_INTERFACE_H_
