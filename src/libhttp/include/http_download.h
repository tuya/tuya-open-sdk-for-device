#ifndef _FILE_DOWNLOAD_H_
#define _FILE_DOWNLOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "http_client_interface.h"

typedef enum {
    DL_EVENT_CONNECTED,
    DL_EVENT_START,
    DL_EVENT_ON_FILESIZE,
    DL_EVENT_ON_DATA,
    DL_EVENT_FINISH,
    DL_EVENT_FAULT,
} http_download_event_id_t;

typedef struct {
    void *data;
    size_t offset;
    size_t data_len;
    size_t file_size;
    uint32_t remain_len;
    void *user_data;
} http_download_event_t;

typedef void (*http_download_event_cb_t)(http_download_event_id_t id, http_download_event_t *event);

typedef struct {
    const char *url;
    const uint8_t *cacert;
    size_t cacert_len;
    uint32_t timeout_ms;
    size_t range_length;
    size_t file_size;
    void *user_data;
    http_download_event_cb_t event_handler;
} http_download_config_t;

int http_file_download(http_download_config_t *config);

#ifdef __cplusplus
}
#endif
#endif
