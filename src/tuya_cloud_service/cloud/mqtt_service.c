#include "tuya_config_defaults.h"

#include "tuya_error_code.h"
#include "mqtt_client_interface.h"
#include "cJSON.h"
#include "mqtt_service.h"
#include "tal_security.h"
#include "crc32i.h"
#include "tal_api.h"
#include "tuya_protocol.h"

static void on_subscribe_message_default(uint16_t msgid, const mqtt_client_message_t* msg, void* userdata);

typedef struct {
	uint32_t sequence;
	uint32_t source;
	size_t   datalen;
	uint8_t  data[0];
} pv22_packet_object_t;

static int tuya_mqtt_signature_tool(const tuya_meta_info_t *input, tuya_mqtt_access_t *signout)
{
	if (NULL == input || signout == NULL) {
		PR_ERR("param error");
		return OPRT_INVALID_PARM;
	}

    // clear
	int i;
    uint8_t digest[16] = {0};
    memset(signout, 0, sizeof(tuya_mqtt_access_t));

    if (input->devid && input->seckey && input->localkey) {
        // ACTIVED
        memcpy(signout->cipherkey, input->localkey, 16);
        sprintf(signout->clientid, "%s",input->devid);
        sprintf(signout->username, "%s", input->devid);
		tal_md5_ret((const uint8_t*)input->seckey, strlen(input->seckey), digest);
        for (i = 0; i < 8; ++i) {
            sprintf(&signout->password[i * 2], "%02x", (unsigned char)digest[i+4]);
        }

		// IO topic
        sprintf(signout->topic_in, "smart/device/in/%s", input->devid);
        sprintf(signout->topic_out, "smart/device/out/%s", input->devid);

    } else if(input->uuid && input->authkey) {
		// UNACTIVED
        memcpy(signout->cipherkey, input->authkey, 16);
        sprintf(signout->clientid, "acon_%s",input->uuid);
        sprintf(signout->username, "acon_%s|pv=%s", input->uuid, TUYA_PV23);
		tal_md5_ret((const uint8_t*)input->authkey, strlen(input->authkey), digest);
        for (i = 0; i < 8; ++i) {
            sprintf(&signout->password[i * 2], "%02x", (unsigned char)digest[i+4]);
        }

		// IO topic
        sprintf(signout->topic_in, "d/ai/%s", input->uuid);
        sprintf(signout->topic_out, "%s", ""); // not support publish data on direct mode

    } else {
        PR_ERR("input error");
		return OPRT_INVALID_PARM;
    }
    return OPRT_OK;
}

/* -------------------------------------------------------------------------- */
/*                          Subscribe message handle                          */
/* -------------------------------------------------------------------------- */
int tuya_mqtt_subscribe_message_callback_register(tuya_mqtt_context_t* context,
												  const char* topic,
												  mqtt_subscribe_message_cb_t cb,
												  void* userdata)
{
	if (!context || !topic) {
		return OPRT_INVALID_PARM;
	}

	uint16_t msgid = mqtt_client_subscribe(context->mqtt_client, topic, MQTT_QOS_1);
	if (msgid <= 0) {
		return OPRT_COM_ERROR;
	}

	/* Repetition filter */
	mqtt_subscribe_handle_t* target = context->subscribe_list;
	while (target) {
		if (!memcmp(target->topic, topic, target->topic_length) && target->cb == cb) {
			PR_WARN("Repetition:%s", topic);
			return OPRT_OK;
		}
		target = target->next;
	}

	/* Intser new handle */
	mqtt_subscribe_handle_t* newtarget = tal_calloc(1, sizeof(mqtt_subscribe_handle_t));
	if (!newtarget) {
		PR_ERR("malloc error");
		return OPRT_MALLOC_FAILED;
	}

	newtarget->topic_length = strlen(topic);
	newtarget->topic = tal_calloc(1, newtarget->topic_length + 1); //strdup
	strcpy(newtarget->topic, topic);

	if (cb) {
		newtarget->cb = cb;
	} else {
		newtarget->cb = on_subscribe_message_default;
	}
	newtarget->userdata = userdata;
	/* LOCK */
	newtarget->next = context->subscribe_list;
	context->subscribe_list = newtarget;
	/* UNLOCK */
	return OPRT_OK;
}

int tuya_mqtt_subscribe_message_callback_unregister(tuya_mqtt_context_t* context, const char* topic)
{
	if (!context || !topic) {
		return OPRT_INVALID_PARM;
	}

	size_t topic_length = strlen(topic);

	/* LOCK */
	/* Remove object form list */
	mqtt_subscribe_handle_t** target = &context->subscribe_list;
	while (*target) {
		mqtt_subscribe_handle_t* entry = *target;
		if (entry->topic_length == topic_length &&\
			!memcmp(topic, entry->topic, topic_length)) {
			*target = entry->next;
			tal_free(entry->topic);
			tal_free(entry);
		} else {
			target = &entry->next;
		}
	}
	/* UNLOCK */

	uint16_t msgid = mqtt_client_unsubscribe(context->mqtt_client, topic, MQTT_QOS_1);
	if (msgid <= 0) {
		return OPRT_COM_ERROR;
	}

	return OPRT_OK;
}

static void mqtt_subscribe_message_distribute(tuya_mqtt_context_t* context, uint16_t msgid, const mqtt_client_message_t* msg)
{
	const char* topic = msg->topic;
	size_t topic_length = strlen(msg->topic);

	/* LOCK */
	mqtt_subscribe_handle_t* target = context->subscribe_list;
	for (; target; target = target->next) {
		if (target->topic_length == topic_length &&\
			!memcmp(topic, target->topic, target->topic_length)) {
			target->cb(msgid, msg, target->userdata);
		}
	}
	/* UNLOCK */
}

/* -------------------------------------------------------------------------- */
/*                       Tuya internal subscribe message                      */
/* -------------------------------------------------------------------------- */
static int tuya_protocol_message_parse_process(tuya_mqtt_context_t* context, const uint8_t* payload, size_t payload_len)
{
	int ret = OPRT_OK;

	char* jsonstr = NULL;
	ret = tuya_parse_protocol_data(DP_CMD_MQ, (BYTE_T *)payload, payload_len, context->signature.cipherkey, (CHAR_T **)&jsonstr);
	if (OPRT_OK != ret) {
		PR_ERR("Cmd Parse Fail:%d", ret);
		return OPRT_COM_ERROR;
	}

	PR_DEBUG("Data JSON:%s", jsonstr);

	/* json parse */
	cJSON *root = NULL;
    cJSON *json = NULL;
    root = cJSON_Parse((const char *)jsonstr);
	tal_free(jsonstr);
    if(NULL == root) {
        PR_ERR("JSON parse error");
		return OPRT_CJSON_PARSE_ERR;
    }

	/* JSON key verfiy */
    if(( NULL == cJSON_GetObjectItem(root,"protocol")) || \
       ( NULL == cJSON_GetObjectItem(root,"t")) || \
       ( NULL == cJSON_GetObjectItem(root,"data"))) {
        PR_ERR("param is no correct");
		cJSON_Delete(root);
		return OPRT_CJSON_GET_ERR;
    }

    /* protocol ID */
    int protocol_id = cJSON_GetObjectItem(root,"protocol")->valueint;
    json = cJSON_GetObjectItem(root,"data");
    if(NULL == json) {
        PR_ERR("get json err");
		cJSON_Delete(root);
        return OPRT_CJSON_GET_ERR;
    }

    /* dispatch */
	tuya_protocol_event_t event;
	event.event_id = protocol_id;
	event.root_json = root;
	event.data = cJSON_GetObjectItem(root, "data");

	/* LOCK */
	tuya_protocol_handle_t* target = context->protocol_list;
	for (; target; target = target->next) {
		if (target->id == protocol_id) {
			event.user_data = target->user_data,
			target->cb(&event);
		}
	}
	/* UNLOCK */

	cJSON_Delete(root);
	return OPRT_OK;
}

static void on_subscribe_message_default(uint16_t msgid, const mqtt_client_message_t* msg, void* userdata)
{
	tuya_mqtt_context_t* context = (tuya_mqtt_context_t*)userdata;
	int ret = tuya_protocol_message_parse_process(context, msg->payload, msg->length);
	if(ret != OPRT_OK) {
		PR_ERR("protocol message parse error:%d", ret);
	}
}

/* -------------------------------------------------------------------------- */
/*                         MQTT Client event callback                         */
/* -------------------------------------------------------------------------- */
static void mqtt_client_connected_cb(void* client, void* userdata)
{
	client = client;
	tuya_mqtt_context_t* context = (tuya_mqtt_context_t*)userdata;
	PR_INFO("mqtt client connected!");

	tuya_mqtt_subscribe_message_callback_register(context, 
											      context->signature.topic_in, 
											      on_subscribe_message_default, 
											      userdata);
	PR_DEBUG("SUBSCRIBE sent for topic %s to broker.", context->signature.topic_in);
	context->is_connected = true;
	if (context->on_connected) {
		context->on_connected(context, context->user_data);
	}
}

static void mqtt_client_disconnected_cb(void* client, void* userdata)
{
	client = client;
	tuya_mqtt_context_t* context = (tuya_mqtt_context_t*)userdata;
	PR_INFO("mqtt client disconnected!");
	context->is_connected = false;
	if (context->on_disconnect) {
		context->on_disconnect(context, context->user_data);
	}
}

static void mqtt_client_message_cb(void* client, uint16_t msgid, const mqtt_client_message_t* msg, void* userdata)
{
	client = client;
	tuya_mqtt_context_t* context = (tuya_mqtt_context_t*)userdata;

	/* topic filter */
	PR_DEBUG("recv message TopicName:%s, payload len:%d", msg->topic, msg->length);
	mqtt_subscribe_message_distribute(context, msgid, msg);
}

static void mqtt_client_subscribed_cb(void* client, uint16_t msgid, void* userdata)
{
	client = client;
	userdata = userdata;
	PR_DEBUG("Subscribe successed ID:%d", msgid);
}

static void mqtt_client_puback_cb(void* client, uint16_t msgid, void* userdata)
{
	client = client;
	tuya_mqtt_context_t* context = (tuya_mqtt_context_t*)userdata;
	PR_DEBUG("PUBACK ID:%d", msgid);

	/* LOCK */
	/* publish async process */
	mqtt_publish_handle_t** next_handle = &context->publish_list;
    for (; *next_handle; next_handle = &(*next_handle)->next) {
        mqtt_publish_handle_t* entry = *next_handle;
		if (msgid == entry->msgid) {
			entry->cb(OPRT_OK, entry->user_data);
			*next_handle = entry->next;
			tal_free(entry->payload);
			tal_free(entry);
			break;
		}
    }
	/* UNLOCK */
}

/* -------------------------------------------------------------------------- */
/*                                Tuya MQTT API                               */
/* -------------------------------------------------------------------------- */
int tuya_mqtt_init(tuya_mqtt_context_t* context, const tuya_mqtt_config_t* config)
{
	int rt = OPRT_OK;
    mqtt_client_status_t mqtt_status;

	/* Clean to zero */
	memset(context, 0, sizeof(tuya_mqtt_context_t));

	/* configuration */
	context->user_data = config->user_data;
	context->on_unbind = config->on_unbind;
	context->on_connected = config->on_connected;
	context->on_disconnect = config->on_disconnect;

	/* Device token signature */
	rt = tuya_mqtt_signature_tool(
			&(const tuya_meta_info_t){
				.uuid = config->uuid,
				.authkey = config->authkey,
				.devid = config->devid,
				.seckey = config->seckey,
				.localkey = config->localkey,
			},
			&context->signature);
	if (OPRT_OK != rt) {
		PR_ERR("mqtt token sign error:%d", rt);
		return rt;
	}

	/* MQTT Client object new */
	context->mqtt_client = mqtt_client_new();
	if (context->mqtt_client == NULL) {
		PR_ERR("mqtt client new fault.");
		return OPRT_MALLOC_FAILED;
	}

	/* MQTT Client init */
	const mqtt_client_config_t mqtt_config = {
		.cacert = config->cacert,
		.cacert_len = config->cacert_len,
		.host = config->host,
		.port = config->port,
		.keepalive = MQTT_KEEPALIVE_INTERVALIN,
		.timeout_ms = config->timeout,
		.clientid = context->signature.clientid,
		.username = context->signature.username,
		.password = context->signature.password,
		.on_connected = mqtt_client_connected_cb,
		.on_disconnected = mqtt_client_disconnected_cb,
		.on_message = mqtt_client_message_cb,
		.on_subscribed = mqtt_client_subscribed_cb,
		.on_published = mqtt_client_puback_cb,
		.userdata = context
	};
	mqtt_status = mqtt_client_init(context->mqtt_client, &mqtt_config);
    if( mqtt_status != MQTT_STATUS_SUCCESS ) {
        PR_ERR( "MQTT init failed: Status = %d.", mqtt_status);
		return OPRT_COM_ERROR;
    }

	BackoffAlgorithm_InitializeParams(&context->backoff_algorithm,
									  MQTT_CONNECT_RETRY_MIN_DELAY_MS,
									  MQTT_CONNECT_RETRY_MAX_DELAY_MS,
									  MQTT_CONNECT_RETRY_MAX_ATTEMPTS );

	// rand
    context->sequence_out = rand() & 0xffff;
	context->sequence_in = -1;

	/* Wait start task */
	context->is_inited = true;
	context->manual_disconnect = true;
	return OPRT_OK;
}

int tuya_mqtt_start(tuya_mqtt_context_t* context)
{
	if (context == NULL || context->is_inited == false) {
		return OPRT_INVALID_PARM;
	}

	PR_INFO("clientid:%s", context->signature.clientid);
	PR_INFO("username:%s", context->signature.username);
	PR_DEBUG("password:%s", context->signature.password);
	PR_INFO("topic_in:%s", context->signature.topic_in);
	PR_INFO("topic_out:%s",context->signature.topic_out);
	PR_INFO("tuya_mqtt_start...");
	context->manual_disconnect = false;

	mqtt_client_status_t mqtt_status;

	mqtt_status = mqtt_client_connect(context->mqtt_client);
	if (MQTT_STATUS_NOT_AUTHORIZED == mqtt_status) {
		PR_ERR("MQTT connect fail:%d", mqtt_status);
		if (context->on_unbind) {
			context->on_unbind(context, context->user_data);
		}
		return OPRT_AUTHENTICATION_FAIL;
	}
	//! TODO: rand
	if (MQTT_STATUS_SUCCESS != mqtt_status) {
		PR_ERR("MQTT connect fail:%d", mqtt_status);
		/* Generate a random number and get back-off value (in milliseconds) for the next connection retry. */
		uint16_t nextRetryBackOff = 0U;
		if( BackoffAlgorithm_GetNextBackoff(&context->backoff_algorithm,\
			rand(), &nextRetryBackOff ) == BackoffAlgorithmSuccess ) {
			PR_WARN("Connection to the MQTT server failed. Retrying "
					"connection after %hu ms backoff.",
					( unsigned short ) nextRetryBackOff );
			tal_system_sleep(nextRetryBackOff + 10000);
		}
		return OPRT_COM_ERROR;
	}
	return OPRT_OK;
}

int tuya_mqtt_stop(tuya_mqtt_context_t* context)
{
	if (context == NULL || context->is_inited == false) {
		return OPRT_INVALID_PARM;
	}

	int ret = tuya_mqtt_subscribe_message_callback_unregister(context, context->signature.topic_in);
	PR_DEBUG("MQTT unsubscribe result:%d", ret);

	mqtt_client_status_t mqtt_status;
	mqtt_status = mqtt_client_disconnect(context->mqtt_client);
	PR_DEBUG("MQTT disconnect result:%d", mqtt_status);

	context->manual_disconnect = true;
	return OPRT_OK;
}

int tuya_mqtt_protocol_register(tuya_mqtt_context_t* context, uint16_t protocol_id, tuya_protocol_callback_t cb, void* user_data)
{
	if (context == NULL || context->is_inited == false || cb == NULL) {
		return OPRT_INVALID_PARM;
	}

	/* LOCK */
	/* Repetition filter */
	tuya_protocol_handle_t* target = context->protocol_list;
	while (target) {
		if (target->id == protocol_id && target->cb == cb) {
			return OPRT_COM_ERROR;
		}
		target = target->next;
	}

	tuya_protocol_handle_t* new_handle = tal_calloc(1, sizeof(tuya_protocol_handle_t));
	if (!new_handle) {
		return OPRT_MALLOC_FAILED;
	}
	new_handle->id = protocol_id;
	new_handle->cb = cb;
	new_handle->user_data = user_data;
	new_handle->next = context->protocol_list;
	context->protocol_list = new_handle;
	/* UNLOCK */

	return OPRT_OK;
}

int tuya_mqtt_protocol_unregister(tuya_mqtt_context_t* context, uint16_t protocol_id, tuya_protocol_callback_t cb)
{
	if (context == NULL || context->is_inited == false || cb == NULL) {
		return OPRT_INVALID_PARM;
	}

	/* LOCK */
	/* Remove object form list */
	tuya_protocol_handle_t** target = &context->protocol_list;
	while (*target) {
		tuya_protocol_handle_t* entry = *target;
		if (entry->id == protocol_id && entry->cb == cb) {
			*target = entry->next;
			tal_free(entry);
		} else {
			target = &entry->next;
		}
	}
	/* UNLOCK */

	return OPRT_OK;
}

int tuya_mqtt_client_publish_common(tuya_mqtt_context_t* context, const char* topic,
								   const uint8_t* payload, size_t payload_length,
								   mqtt_publish_notify_cb_t cb, void* user_data, 
								   int timeout_ms, bool async)
{
	if(context == NULL || topic == NULL || payload == NULL || (cb == NULL && async == true)) {
		return OPRT_INVALID_PARM;
	}

	if (cb == NULL) {
		uint16_t msgid = mqtt_client_publish(context->mqtt_client, topic,
											 payload, payload_length, MQTT_QOS_0);
		if (msgid <= 0) {
			return OPRT_COM_ERROR;
		}
		return OPRT_OK;
	}

	mqtt_publish_handle_t* handle = tal_malloc(sizeof(mqtt_publish_handle_t));
	TUYA_CHECK_NULL_RETURN(handle, OPRT_MALLOC_FAILED);
	handle->next = NULL;
	handle->msgid = 0;
	handle->topic = (char*)topic;
	handle->timeout = tal_time_get_posix() + timeout_ms;
	handle->cb = cb;
	handle->user_data = user_data;
	handle->payload_length = payload_length;
	handle->payload = tal_malloc(payload_length);
	if (handle->payload == NULL) {
		return OPRT_MALLOC_FAILED;
	}
	memcpy(handle->payload, payload, payload_length);

	if (async == false) {
		handle->msgid = mqtt_client_publish(context->mqtt_client, handle->topic,
											handle->payload, handle->payload_length,
											MQTT_QOS_1);
	}

	if (context->publish_list == NULL) {
		context->publish_list = handle;
		return OPRT_OK;
	}

	mqtt_publish_handle_t* last = context->publish_list;
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = handle;

	return OPRT_OK;
}

int tuya_mqtt_protocol_data_publish_with_topic_common(tuya_mqtt_context_t* context, const char* topic, 
													  uint16_t protocol_id, const uint8_t* data, uint16_t length,
													  mqtt_publish_notify_cb_t cb, void* user_data,
													  int timeout_ms, bool async)
{
	if (context == NULL || context->is_inited == false) {
		return OPRT_INVALID_PARM;
	}

	if (context->is_connected == false) {
		return OPRT_COM_ERROR;
	}

	int ret = OPRT_OK;

	CHAR_T *buffer = NULL;
	UINT_T buffer_len = 0;

	ret = tuya_pack_protocol_data(DP_CMD_MQ, (const CHAR_T *)data, protocol_id, (BYTE_T *)context->signature.cipherkey, &buffer, &buffer_len);
	if (ret != OPRT_OK) {
		PR_ERR("tuya_pack_protocol_data error:%d", ret);
		return ret;
	}

	/* mqtt client publish */
	ret = tuya_mqtt_client_publish_common(context, (const char*)topic,
								   		  (const uint8_t*)buffer, buffer_len,
								   		  cb, user_data, timeout_ms, async);
	tal_free(buffer);
	return ret;
}

int tuya_mqtt_protocol_data_publish_common(tuya_mqtt_context_t* context, uint16_t protocol_id,
										   const uint8_t* data, uint16_t length,
										   mqtt_publish_notify_cb_t cb, void* user_data,
										   int timeout_ms, bool async)
{
	return tuya_mqtt_protocol_data_publish_with_topic_common(context, context->signature.topic_out, 
															 protocol_id, data, length,
															 cb, user_data, timeout_ms, async);
}

int tuya_mqtt_protocol_data_publish_with_topic(tuya_mqtt_context_t* context, const char* topic,
											   uint16_t protocol_id, const uint8_t* data, uint16_t length)
{
	return tuya_mqtt_protocol_data_publish_with_topic_common(context, topic, 
															 protocol_id, data, length,
															 NULL, NULL, 0, false);
}

int tuya_mqtt_protocol_data_publish(tuya_mqtt_context_t* context, uint16_t protocol_id, const uint8_t* data, uint16_t length)
{
	return tuya_mqtt_protocol_data_publish_with_topic(context, context->signature.topic_out, protocol_id, data, length);
}

int tuya_mqtt_loop(tuya_mqtt_context_t* context)
{
	if (context == NULL) {
		return OPRT_COM_ERROR;
	}

	int rt = OPRT_OK;
	mqtt_client_status_t mqtt_status;

	if (context->is_inited == false ||
		context-> manual_disconnect == true) {
		return rt;
	}

	/* reconnect */
	if (context->is_connected == false) {
		mqtt_status = mqtt_client_connect(context->mqtt_client);
		if (mqtt_status == MQTT_STATUS_NOT_AUTHORIZED) {
			if(context->on_unbind) {
				context->on_unbind(context, context->user_data);
			}
			return rt;

		} else if (mqtt_status != MQTT_STATUS_SUCCESS) {
			uint16_t nextRetryBackOff = 0U;
			if( BackoffAlgorithm_GetNextBackoff(&context->backoff_algorithm,\
				rand(), &nextRetryBackOff ) == BackoffAlgorithmSuccess ) {
				PR_WARN("Connection to the MQTT server failed. Retrying "
						"connection after %hu ms backoff.",
						( unsigned short ) nextRetryBackOff );
				tal_system_sleep(nextRetryBackOff);
				return rt;
			}
		}
		return rt;
	}

	/* LOCK */
	/* publish async process */
	mqtt_publish_handle_t** next_handle = &context->publish_list;
    for (; *next_handle; next_handle = &(*next_handle)->next) {
        mqtt_publish_handle_t* entry = *next_handle;

		if (entry->timeout <= tal_time_get_posix()) {
			entry->cb(OPRT_TIMEOUT, entry->user_data);
			*next_handle = entry->next;
			tal_free(entry->payload);
			tal_free(entry);
			continue;
		}

		if (entry->msgid <= 0) {
			entry->msgid = mqtt_client_publish(context->mqtt_client, entry->topic,
											   entry->payload, entry->payload_length, 1);
		}
    }
	/* UNLOCK */

	/* yield */
	mqtt_client_yield(context->mqtt_client);

	return rt;
}

int tuya_mqtt_destory(tuya_mqtt_context_t* context)
{
	if (context == NULL || context->is_inited != true) {
		return OPRT_COM_ERROR;
	}

	mqtt_client_status_t mqtt_status = mqtt_client_deinit(context->mqtt_client);
	mqtt_client_free(context->mqtt_client);
	if (mqtt_status != MQTT_STATUS_SUCCESS) {
		return OPRT_COM_ERROR;
	}

	return OPRT_OK;
}

bool tuya_mqtt_connected(tuya_mqtt_context_t* context)
{
	if (context == NULL) {
		return false;
	}
	return context->is_connected;
}

int tuya_mqtt_upgrade_progress_report(tuya_mqtt_context_t* context, int channel, int percent)
{
    if(percent > 100) {
        PR_ERR("input invalid:%d", percent);
        return OPRT_INVALID_PARM;
    }

    uint8_t *data_buf = tal_malloc(128);
    if(NULL == data_buf) {
        return OPRT_MALLOC_FAILED;
    }

    int buffer_size = sprintf((char*)data_buf,"{\"progress\":\"%d\",\"firmwareType\":%d}", percent, channel);
	uint16_t msgid = tuya_mqtt_protocol_data_publish(context, PRO_UPGE_PUSH, data_buf, (uint16_t)buffer_size);
    tal_free(data_buf);
	if (msgid <= 0) {
    	return OPRT_COM_ERROR;
	}
	return OPRT_OK;
}
