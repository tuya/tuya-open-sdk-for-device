#include "tuya_config_defaults.h"
#include "tuya_error_code.h"

#include "tuya_iot.h"
#include "tuya_endpoint.h"
#include "mqtt_service.h"
#include "cJSON.h"
#include "tal_system.h"

typedef enum {
    STATE_MQTT_BIND_START,
    STATE_MQTT_BIND_CONNECT,
    STATE_MQTT_BIND_COMPLETE,
    STATE_MQTT_BIND_TIMEOUT,
    STATE_MQTT_BIND_FAILED,
    STATE_MQTT_BIND_EXIT,
    STATE_MQTT_BIND_CONNECTED_WAIT,
    STATE_MQTT_BIND_TOKEN_WAIT,
} mqtt_bind_state_t;

static void mqtt_bind_activate_token_on(tuya_protocol_event_t* ev)
{
    cJSON* data = (cJSON*)(ev->data);
    tuya_binding_info_t* binding = (tuya_binding_info_t*)(ev->user_data);

    if (NULL == cJSON_GetObjectItem(data, "token")) {
        PR_ERR("not found token");
        return;
    }

    if (NULL == cJSON_GetObjectItem(data, "region")) {
        PR_ERR("not found region");
        return;
    }

    /* get token from cJSON object */
    char* token = cJSON_GetObjectItem(data, "token")->valuestring;
    char* region = cJSON_GetObjectItem(data, "region")->valuestring;
    char* regist_key = "pro"; // online env default

    if  (cJSON_GetObjectItem(data, "env")) {
        regist_key = cJSON_GetObjectItem(data, "env")->valuestring;
    }

    if (strlen(token) > MAX_LENGTH_TOKEN) {
        PR_ERR("token length error");
        return;
    }

    if (strlen(region) > MAX_LENGTH_REGION) {
        PR_ERR("region length error");
        return;
    }

    if (strlen(regist_key) > MAX_LENGTH_REGIST) {
        PR_ERR("regist_key length error");
        return;
    }

    strcpy(binding->token, token);
    strcpy(binding->region, region);
    strcpy(binding->regist_key, regist_key);
}

int mqtt_bind_token_get(const tuya_iot_config_t* config, tuya_binding_info_t* binding)
{
    int ret = OPRT_OK;
    mqtt_bind_state_t mqtt_bind_state = STATE_MQTT_BIND_START;
    tuya_mqtt_context_t mqctx;

    while(mqtt_bind_state != STATE_MQTT_BIND_EXIT) {
        switch(mqtt_bind_state) {
        case STATE_MQTT_BIND_START: {
            /* Update region demain */
            if (OPRT_OK != tuya_endpoint_update_auto_region()) {
                tal_system_sleep(1000);
                continue;
            }

            /* mqtt init */
            const tuya_endpoint_t* endpoint = tuya_endpoint_get();
            ret = tuya_mqtt_init(&mqctx, &(const tuya_mqtt_config_t){
                .cacert = endpoint->cert,
                .cacert_len =  endpoint->cert_len,
                .host =  endpoint->mqtt.host,
                .port = endpoint->mqtt.port,
                .uuid = config->uuid,
                .authkey = config->authkey,
                .timeout = MQTT_BIND_TIMEOUT_MS_DEFAULT,
            });
            if (OPRT_OK != ret) {
                PR_ERR("tuya mqtt init error:%d", ret);
                tuya_mqtt_destory(&mqctx);
                return OPRT_AUTHENTICATION_FAIL;
            }

            /* register token callback */
            tuya_mqtt_protocol_register(&mqctx, PRO_MQ_ACTIVE_TOKEN_ON,
                                    mqtt_bind_activate_token_on, binding);
            mqtt_bind_state = STATE_MQTT_BIND_CONNECT;
            break;
        }

        case STATE_MQTT_BIND_CONNECT:
            ret = tuya_mqtt_start(&mqctx);
            if (OPRT_OK != ret) {
                PR_ERR("tuya mqtt connect fail:%d, retry..", ret);
                tal_system_sleep(2000);
                break;
            }
            mqtt_bind_state = STATE_MQTT_BIND_CONNECTED_WAIT;
            break;

        case STATE_MQTT_BIND_CONNECTED_WAIT:
            if (tuya_mqtt_connected(&mqctx)) {
                /* Send connected event*/
                tuya_iot_client_t * client = tuya_iot_client_get();
                client->event.id = TUYA_EVENT_DIRECT_MQTT_CONNECTED;
                client->event.type = TUYA_DATE_TYPE_UNDEFINED;
                tuya_iot_dispatch_event(client);

                PR_INFO("MQTT direct connected!");
                PR_INFO("Wait Tuya APP scan the Device QR code...");
                mqtt_bind_state = STATE_MQTT_BIND_TOKEN_WAIT;
            }
            break;

        case STATE_MQTT_BIND_TOKEN_WAIT:
            tuya_mqtt_loop(&mqctx);
            if (strlen(binding->token) == 0) {
                break;
            }
            mqtt_bind_state = STATE_MQTT_BIND_COMPLETE;
            break;

        case STATE_MQTT_BIND_COMPLETE:
            PR_DEBUG("STATE_MQTT_BIND_COMPLETE");
            tuya_mqtt_stop(&mqctx);
            tuya_mqtt_destory(&mqctx);
            mqtt_bind_state = STATE_MQTT_BIND_EXIT;
            break;

        default:
            PR_ERR("state error:%d", mqtt_bind_state);
            break;
        }

    }

    return ret;
}
