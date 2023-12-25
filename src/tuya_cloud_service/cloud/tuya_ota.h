#ifndef _TUYA_OTA_H_
#define _TUYA_OTA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"
#include "http_download.h"

#define TUS_RD 1
#define TUS_UPGRDING 2
#define TUS_UPGRD_FINI 3
#define TUS_UPGRD_EXEC 4

#define TUS_DOWNLOAD_START 10
#define TUS_DOWNLOAD_COMPLETE 11
#define TUS_UPGRADE_START 12
#define TUS_UPGRADE_SUCCESS 3

#define TUS_DOWNLOAD_ERROR_UNKONW 40
#define TUS_DOWNLOAD_ERROR_LOW_BATTERY 41
#define TUS_DOWNLOAD_ERROR_STORAGE_NOT_ENOUGH 42
#define TUS_DOWNLOAD_ERROR_MALLOC_FAIL 43
#define TUS_DOWNLOAD_ERROR_TIMEOUT 44
#define TUS_DOWNLOAD_ERROR_HMAC 45
#define TUS_UPGRADE_ERROR_LOW_BATTERY 46
#define TUS_UPGRADE_ERROR_MALLOC_FAIL 47
#define TUS_UPGRADE_ERROR_VERSION 48
#define TUS_UPGRADE_ERROR_HMAC 49

typedef enum{
    TUYA_OTA_EVENT_START,
    TUYA_OTA_EVENT_ON_DATA,
    TUYA_OTA_EVENT_FINISH,
    TUYA_OTA_EVENT_FAULT
} tuya_ota_event_id_t;

typedef struct {
    tuya_ota_event_id_t id;
    void*   data;
    size_t  data_len;
    size_t  offset;
    size_t  file_size;
    void*   user_data;
} tuya_ota_event_t;

typedef struct {
    /** firmware type */
    uint8_t channel;
    /** firmware download url */
    char    fw_url[FW_URL_LEN + 1];
    /** firmware version */
    char    sw_ver[SW_VER_LEN + 1];
    /** firmware size in BYTE */
    size_t  file_size;
    /** firmware hmac */
    char    fw_hmac[FW_HMAC_LEN + 1];
    /** firmware md5 */
    char    fw_md5[SW_MD5_LEN + 1];
} tuya_ota_msg_t;


// typedef void (*tuya_ota_event_cb_t)(tuya_ota_handle_t* handle, tuya_ota_event_t* event);

typedef void (*tuya_ota_event_cb_t)(tuya_ota_msg_t* msg, tuya_ota_event_t* event);

typedef struct {
    void               *client;
    tuya_ota_event_cb_t event_cb;
    size_t              range_size;
    uint32_t            timeout_ms;
    void               *user_data;
} tuya_ota_config_t;


int tuya_ota_init(tuya_ota_config_t* config);
int tuya_ota_start(cJSON* upgrade);


#ifdef __cplusplus
}
#endif
#endif