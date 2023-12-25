

#include "tuya_config_defaults.h"
#include "tuya_error_code.h"
#include "cJSON.h"
#include "matop_service.h"
#include "atop_base.h"
#include "tal_api.h"

#define MATOP_DEFAULT_BUFFER_LEN (128)

/* -------------------------------------------------------------------------- */
/*                              Internal callback                             */
/* -------------------------------------------------------------------------- */
static int matop_service_data_receive_cb(void* context, const uint8_t* input, size_t ilen)
{
	matop_context_t* matop = (matop_context_t*)context;

	PR_TRACE("atop response raw:\r\n%.*s", ilen, input);

	/* json parse */
    cJSON* root = cJSON_Parse((const char*)input);
    if (NULL == root) {
        PR_ERR("Json parse error");
        return OPRT_CJSON_PARSE_ERR;
    }

	if (cJSON_GetObjectItem(root, "id") == NULL ||
		cJSON_GetObjectItem(root, "id")->type != cJSON_Number ||
		cJSON_GetObjectItem(root, "data") == NULL) {
		cJSON_Delete(root);
		return OPRT_CJSON_GET_ERR;
	}

	uint16_t id = cJSON_GetObjectItem(root, "id")->valueint;
	cJSON* data = cJSON_GetObjectItem(root, "data");

	/* found message id */
	mqtt_atop_message_t* target_message = matop->message_list;
	while (target_message) {
		if (target_message->id == id) {
			break;
		}
		target_message = target_message->next;
	}

	if (target_message == NULL) {
		PR_WARN("not found id.");
		cJSON_Delete(root);
		return OPRT_COM_ERROR;
	}

	/* result parse */
	bool success = false;
	cJSON* result = NULL;

	if (cJSON_GetObjectItem(data, "result")) {
		result = cJSON_GetObjectItem(data, "result");
		success = cJSON_IsTrue(cJSON_GetObjectItem(result, "success"));
		result = cJSON_GetObjectItem(result, "result");
	}

	atop_base_response_t response = {
		.success = success,
		.result = result,
		.t = success ? cJSON_GetObjectItem(data, "t")->valueint:0,
		.user_data = target_message->user_data
	};

	if(target_message->notify_cb) {
		target_message->notify_cb(&response, target_message->user_data);
	}

	cJSON_Delete(root);

	/* remove target from list  */
	mqtt_atop_message_t** current;
	for(current = &matop->message_list; *current; ) {
		mqtt_atop_message_t* entry = *current;
		if (entry == target_message) {
			*current = entry->next;
			tal_free(entry);
		} else {
			current = &entry->next;
		}
	}
	return 0;
}

static int matop_service_file_rawdata_receive_cb(void* context, const uint8_t* input, size_t ilen)
{
	matop_context_t* matop = (matop_context_t*)context;

	if (ilen < sizeof(uint32_t)) {
		PR_ERR("error ilen:%d", ilen);
		return OPRT_INVALID_PARM;
	}

	uint32_t id = ((input[0]&0xff)<<24) | ((input[1]&0xff)<<16) | ((input[2]&0xff)<<8) | (input[3]&0xff);
	PR_INFO("file data id:%d", id);

	/* found message id */
	mqtt_atop_message_t* target_message = matop->message_list;
	while (target_message) {
		if (target_message->id == id) {
			break;
		}
		target_message = target_message->next;
	}

	if (target_message == NULL) {
		PR_WARN("not found id.");
		return OPRT_COM_ERROR;
	}

	atop_base_response_t response = {
		.success = true,
		.result = NULL,
		.t = 0,
		.raw_data = (uint8_t*)(input + sizeof(uint32_t)),
		.raw_data_len = ilen - sizeof(uint32_t),
		.user_data = target_message->user_data,
	};

	if(target_message->notify_cb) {
		target_message->notify_cb(&response, target_message->user_data);
	}

	/* remove target from list  */
	mqtt_atop_message_t** current;
	for(current = &matop->message_list; *current; ) {
		mqtt_atop_message_t* entry = *current;
		if (entry == target_message) {
			*current = entry->next;
			tal_free(entry);
		} else {
			current = &entry->next;
		}
	}
    return 0;
}

static void on_matop_service_data_receive(uint16_t msgid, const mqtt_client_message_t* msg, void* userdata)
{
	matop_service_data_receive_cb(userdata, msg->payload, msg->length);
}

static void on_matop_service_file_rawdata_receive(uint16_t msgid, const mqtt_client_message_t* msg, void* userdata)
{
	matop_service_file_rawdata_receive_cb(userdata, msg->payload, msg->length);
}

static int matop_request_send(matop_context_t* context, const uint8_t* data, size_t datalen)
{
	uint16_t msgid = mqtt_client_publish(context->config.mqctx->mqtt_client, context->resquest_topic, data, datalen, MQTT_QOS_0);

	if (msgid <= 0) {
		// TODO add error code
		PR_ERR("mqtt connect err:%d", msgid);
		return OPRT_COM_ERROR;
	}
	return OPRT_OK;
}

/* -------------------------------------------------------------------------- */
/*                                 MATOP base                                 */
/* -------------------------------------------------------------------------- */
int matop_serice_init(matop_context_t* context, const matop_config_t* config)
{
	int ret;
	char topic_buffer[48];

	memset(context, 0, sizeof(matop_context_t));
	context->config = *config;

	sprintf(topic_buffer, "rpc/rsp/%s", config->devid);
	ret = tuya_mqtt_subscribe_message_callback_register(context->config.mqctx, topic_buffer, on_matop_service_data_receive, context);
	if (ret != OPRT_OK) {
		PR_ERR("Topic subscribe error:%s", topic_buffer);
		return ret;
	}

	sprintf(topic_buffer, "rpc/file/%s", config->devid);
	tuya_mqtt_subscribe_message_callback_register(context->config.mqctx, topic_buffer, on_matop_service_file_rawdata_receive, context);
	if (ret != OPRT_OK) {
		PR_ERR("Topic subscribe error:%s", topic_buffer);
		return ret;
	}

	sprintf(context->resquest_topic, "rpc/req/%s", config->devid);
	return OPRT_OK;
}

int matop_serice_yield(matop_context_t* context)
{
	if (context == NULL) {
		return OPRT_INVALID_PARM;
	}

	/* remove target from list  */
	mqtt_atop_message_t** current;
	for(current = &context->message_list; *current; ) {
		mqtt_atop_message_t* entry = *current;
		if (tal_system_get_millisecond() > entry->timeout) {
			PR_WARN("Message id %d timeout.", entry->id);
			if (entry->notify_cb) {
				entry->notify_cb(&(atop_base_response_t){.success = false}, entry->user_data);
			}
			*current = entry->next;
			tal_free(entry);
			return OPRT_TIMEOUT;
		} else {
			current = &entry->next;
		}
	}
	return OPRT_OK;
}

int matop_service_request_async(matop_context_t* context,
								const mqtt_atop_request_t* request,
								mqtt_atop_response_cb_t notify_cb,
								void* user_data)
{
    if (NULL == context || NULL == request) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
	matop_context_t* matop = context;

	/* handle init */
	mqtt_atop_message_t* message_handle = tal_malloc(sizeof(mqtt_atop_message_t));
	if (message_handle == NULL) {
		PR_ERR("response_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
	}
	message_handle->next = NULL;
	message_handle->id = ++matop->id_cnt;
	message_handle->timeout = tal_system_get_millisecond() + (request->timeout == 0 ? MATOP_TIMEOUT_MS_DEFAULT:request->timeout);
	message_handle->notify_cb = notify_cb;
	message_handle->user_data = user_data;

	/* request buffer make */
    size_t request_datalen = 0;
    size_t request_bufferlen = 128 + (request->data ? strlen((char*)request->data) : 0);
    char* request_buffer = tal_malloc(request_bufferlen);
    if (request_buffer == NULL) {
        PR_ERR("response_buffer malloc fail");
		tal_free(message_handle);
        return OPRT_MALLOC_FAILED;
    }

	/* buffer format */
    request_datalen = snprintf(request_buffer, request_bufferlen,
						"{\"id\":%d,\"a\":\"%s\",\"t\":%d,\"data\":%s",
						message_handle->id, request->api, tal_time_get_posix(), request->data?((char*)request->data):"{}");
	if (request->version) {
		request_datalen += snprintf(request_buffer + request_datalen, request_bufferlen - request_datalen,
									",\"v\":\"%s\"",
									request->version);
	}
	request_datalen += snprintf(request_buffer + request_datalen, request_bufferlen - request_datalen, "}");
	PR_DEBUG("atop request: %s", request_buffer);

    rt = matop_request_send(matop, (const uint8_t*)request_buffer, request_datalen);
	tal_free(request_buffer);

	if (rt != OPRT_OK) {
		PR_ERR("mqtt_atop_request_send error:%d", rt);
		tal_free(message_handle);
		return rt;
	}

	/* first head message */
	if (matop->message_list == NULL) {
		matop->message_list = message_handle;
		return OPRT_OK;
	}

	/* add to message list */
	mqtt_atop_message_t* target = matop->message_list;
	while(target->next) {
		target = target->next;
	}
	target->next = message_handle;

	return OPRT_OK;
}

/* -------------------------------------------------------------------------- */
/*                           ATOP Over MQTT Service                           */
/* -------------------------------------------------------------------------- */
int matop_service_client_reset(matop_context_t* context)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DEFAULT_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"t\":%d}", tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.reset",
            .version = "4.0",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        NULL,
        context);
	tal_free(buffer);
    return rt;
}

int matop_service_version_update(matop_context_t* context, const char *versions)
{
    if (NULL == context || NULL == versions) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    #define UPDATE_VERSION_BUFFER_LEN 196
    size_t buffer_len = 0;
    char* buffer = tal_malloc(UPDATE_VERSION_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, UPDATE_VERSION_BUFFER_LEN, "{\"versions\":\"%s\",\"t\":%d}", versions, tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.versions.update",
            .version = "4.1",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        NULL,
        context);
	tal_free(buffer);
    return rt;
}

int matop_service_upgrade_status_update(matop_context_t* context, int channel, int status)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DEFAULT_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN,
        "{\"type\":%d,\"upgradeStatus\":%d,\"t\":%d}", channel, status, tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.upgrade.status.update",
            .version = "4.1",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        NULL,
        context);
	tal_free(buffer);
    return rt;
}

int matop_service_upgrade_info_get(matop_context_t* context, int channel,
									mqtt_atop_response_cb_t notify_cb, void* user_data)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DEFAULT_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"type\":%d,\"t\":%d}", channel, tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.upgrade.get",
            .version = "4.4",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
            .timeout = 10000
        },
        notify_cb,
        user_data);
	tal_free(buffer);
    return rt;
}

int matop_service_auto_upgrade_info_get(matop_context_t* context,
										mqtt_atop_response_cb_t notify_cb,
										void* user_data)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DEFAULT_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"subId\":null,\"t\":%d}", tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.upgrade.silent.get",
            .version = "4.4",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        notify_cb,
        user_data);
	tal_free(buffer);
    return rt;
}

int matop_service_file_download_range(matop_context_t* context,
										const char* url,
										int range_start,
										int range_end,
										uint32_t timeout_ms,
										mqtt_atop_response_cb_t notify_cb,
										void* user_data)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
	#define MATOP_DOWNLOAD_BUFFER_LEN 511
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DOWNLOAD_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, MATOP_DOWNLOAD_BUFFER_LEN,
						"{\"url\":\"%s\",\"range\":\"bytes=%d-%d\",\"type\":%d}",
						url, range_start, range_end, (range_start == 0 && range_end == 0) ? 1:2);
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.file.download",
            .version = "1.0",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
            .timeout = timeout_ms
        },
        notify_cb,
        user_data);
	tal_free(buffer);
    return rt;
}

int matop_service_put_rst_log(matop_context_t* context, int reason)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

	#define RST_BUFFER_MAX (128)
    char* rst_buffer = tal_malloc(RST_BUFFER_MAX);
    if (rst_buffer == NULL) {
		PR_ERR("rst_buffer buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    /* Format rst_info JSON buffer */
    snprintf(rst_buffer, RST_BUFFER_MAX,
        "\"data\":%d", reason);

    /* post data */
    #define UPDATE_VERSION_BUFFER_LEN 196
    size_t buffer_len = 0;
    char* buffer = tal_malloc(UPDATE_VERSION_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
		tal_free(rst_buffer);
        return OPRT_MALLOC_FAILED;
    }

	buffer_len = snprintf(buffer, UPDATE_VERSION_BUFFER_LEN, "{%s,\"t\":%d}", rst_buffer, tal_time_get_posix());
    PR_TRACE("POST JSON:%s", buffer);

    /* ATOP service request send */
	rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "atop.online.debug.log",
            .version = NULL,
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        NULL,
        context);
	tal_free(buffer);
	tal_free(rst_buffer);
    return rt;
}

int matop_service_dynamic_cfg_get(matop_context_t* context,
								  HTTP_DYNAMIC_CFG_TYPE type,
								  mqtt_atop_response_cb_t notify_cb,
								  void* user_data)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(MATOP_DEFAULT_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();

    switch (type) {
    case HTTP_DYNAMIC_CFG_TZ:
        snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"type\":\"[\\\"timezone\\\"]\",\"t\":%d}", timestamp);
        break;
    case HTTP_DYNAMIC_CFG_RATERULE:
        snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"type\":\"[\\\"rateRule\\\"]\",\"t\":%d}", timestamp);
        break;
    case HTTP_DYNAMIC_CFG_ALL:
    default:
        snprintf(buffer, MATOP_DEFAULT_BUFFER_LEN, "{\"type\":\"[\\\"timezone\\\",\\\"rateRule\\\"]\",\"t\":%d}", timestamp);
        break;
    }

    buffer_len = strlen(buffer) + 1;
    PR_TRACE("dynamic cfg get data:%s", buffer);

    /* ATOP service request send */
    rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t) {
            .api = "tuya.device.dynamic.config.get",
            .version = "2.0",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        notify_cb,
        user_data);
    tal_free(buffer);
    return rt;
}

int matop_service_dynamic_cfg_ack(matop_context_t* context,
								  const char* timezone_ackId,
								  const char* rateRule_actId,
								  mqtt_atop_response_cb_t notify_cb,
								  void* user_data)
{
    if (NULL == context) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    uint16_t offset = 0;

#define DYNAMIC_CFG_ACK_BUFFER_LEN MATOP_DEFAULT_BUFFER_LEN
    size_t buffer_len = 0;
    char* buffer = tal_malloc(DYNAMIC_CFG_ACK_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    memset(buffer, 0, DYNAMIC_CFG_ACK_BUFFER_LEN);
    offset = snprintf(buffer, DYNAMIC_CFG_ACK_BUFFER_LEN, "{\"ackList\":[");

    if (timezone_ackId) {
        offset += snprintf(buffer + offset, DYNAMIC_CFG_ACK_BUFFER_LEN - offset, "{\"type\":\"timezone\",\"ackId\":\"%s\"}", timezone_ackId);
    }

    if (rateRule_actId) {
        offset += snprintf(buffer + offset, DYNAMIC_CFG_ACK_BUFFER_LEN - offset, "{\"type\":\"rateRule\",\"ackId\":%s}", rateRule_actId);
    }

    snprintf(buffer + offset, DYNAMIC_CFG_ACK_BUFFER_LEN, "],\"t\":%d}", tal_time_get_posix());

    buffer_len = strlen(buffer) + 1;
    PR_TRACE("dynamic cfg ack data:%s", buffer);

    rt = matop_service_request_async(context,
        &(const mqtt_atop_request_t) {
            .api = "tuya.device.dynamic.config.ack",
            .version = "2.0",
            .data = (uint8_t*)buffer,
            .data_len = buffer_len,
        },
        notify_cb,
        user_data);

    tal_free(buffer);
    return rt;
}

int matop_service_comm_node_enable(matop_context_t* context, mqtt_atop_response_cb_t notify_cb, void* user_data)
{
    /* ATOP service request send */
	return matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.comm.node.enable",
            .version = "1.0",
            .data = NULL,
            .data_len = 0,
        },
        notify_cb,
        user_data);
}

int matop_service_comm_node_disable(matop_context_t* context, mqtt_atop_response_cb_t notify_cb, void* user_data)
{
    /* ATOP service request send */
	return matop_service_request_async(context,
        &(const mqtt_atop_request_t){
            .api = "tuya.device.comm.node.disable",
            .version = "1.0",
            .data = NULL,
            .data_len = 0,
        },
        notify_cb,
        user_data);
}
