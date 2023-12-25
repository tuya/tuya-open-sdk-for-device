#ifndef MATOP_SERVICE_H_
#define MATOP_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "atop_base.h"
#include "atop_service.h"
#include "mqtt_service.h"

typedef struct {
	const char* api;
	const char* version;
	uint8_t* data;
	size_t data_len;
    uint32_t timeout;
} mqtt_atop_request_t;

typedef void (*mqtt_atop_response_cb_t)(atop_base_response_t* response, void* user_data);

typedef struct mqtt_atop_message {
	struct mqtt_atop_message* next;
	uint16_t id;
    uint32_t timeout;
	mqtt_atop_response_cb_t notify_cb;
    void* user_data;
} mqtt_atop_message_t;

typedef struct matop_config {
	tuya_mqtt_context_t* mqctx;
	const char* devid;
} matop_config_t;

typedef struct matop_context {
	matop_config_t config;
	uint32_t id_cnt;
	char resquest_topic[64];
	mqtt_atop_message_t* message_list;
} matop_context_t;

int matop_serice_init(matop_context_t* context, const matop_config_t* config);

int matop_serice_yield(matop_context_t* context);

int matop_service_request_async(matop_context_t* context, 
								const mqtt_atop_request_t* request, 
								mqtt_atop_response_cb_t notify_cb,
								void* user_data);

int matop_service_client_reset(matop_context_t* context);

int matop_service_version_update(matop_context_t* context, const char *versions);

int matop_service_upgrade_status_update(matop_context_t* context, int channel, int status);

int matop_service_upgrade_info_get( matop_context_t* context, int channel, 
									mqtt_atop_response_cb_t notify_cb, 
									void* user_data);

int matop_service_auto_upgrade_info_get(matop_context_t* context, 
										mqtt_atop_response_cb_t notify_cb,
										void* user_data);

int matop_service_file_download_range(matop_context_t* context, 
										const char* url, 
										int range_start, 
										int range_end, 
										uint32_t timeout_ms,
										mqtt_atop_response_cb_t notify_cb,
										void* user_data);

int matop_service_put_rst_log(matop_context_t* context, int reason);

int matop_service_dynamic_cfg_get(matop_context_t* context,
    HTTP_DYNAMIC_CFG_TYPE type,
    mqtt_atop_response_cb_t notify_cb,
    void* user_data);

int matop_service_dynamic_cfg_ack(matop_context_t* context,
    const char* timezone_ackId,
    const char* rateRule_actId,
    mqtt_atop_response_cb_t notify_cb,
    void* user_data);

int matop_service_comm_node_enable(matop_context_t* context,
	mqtt_atop_response_cb_t notify_cb, void* user_data);

int matop_service_comm_node_disable(matop_context_t* context,
	mqtt_atop_response_cb_t notify_cb, void* user_data);

#ifdef __cplusplus
}
#endif
#endif