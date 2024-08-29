/**
 * @file example_http.c
 * @brief Demonstrates HTTP client usage in Tuya SDK applications.
 *
 * This file provides an example of how to use the HTTP client interface provided by the Tuya SDK to send HTTP requests
 * and handle responses. It includes initializing the SDK, setting up network connections (both WiFi and wired,
 * depending on the configuration), sending a GET request to a specified URL, and handling the response. The example
 * also demonstrates how to handle network link status changes and perform cleanups.
 *
 * Key operations demonstrated in this file:
 * - Initialization of the Tuya SDK and network manager.
 * - Sending an HTTP GET request and receiving a response.
 * - Handling network link status changes.
 * - Cleanup and resource management.
 *
 * This example is intended for developers looking to integrate HTTP communication into their Tuya SDK-based IoT
 * applications, providing a foundation for building applications that interact with web services.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"
#include "mqtt_client_interface.h"
#include "tuya_config_defaults.h"
#include "core_mqtt_config.h"
#include "core_mqtt.h"
#include "tuya_transporter.h"
#include "backoff_algorithm.h"
#include "tal_api.h"
#include "tkl_output.h"
#include "netmgr.h"
#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
#include "netconn_wifi.h"
#endif
#if defined(ENABLE_WIRED) && (ENABLE_WIRED == 1)
#include "netconn_wired.h"
#endif

/***********************************************************
*********************** macro define ***********************
***********************************************************/
#ifdef ENABLE_WIFI
#define DEFAULT_WIFI_SSID "your-ssid-****"
#define DEFAULT_WIFI_PSWD "your-pswd-****"
#endif
/***********************************************************
********************** typedef define **********************
***********************************************************/
typedef struct {
    mqtt_client_config_t config;
    MQTTContext_t mqclient;
    tuya_transporter_t network;
    uint8_t mqttbuffer[CORE_MQTT_BUFFER_SIZE];
} mqtt_client_context_t;

/***********************************************************
********************** variable define *********************
***********************************************************/

/***********************************************************
********************** function define *********************
***********************************************************/
static void mqtt_client_connected_cb(void *client, void *userdata)
{
    PR_INFO("mqtt client connected!");
    uint16_t msgid = mqtt_client_subscribe(client, "tuya/tos-test", MQTT_QOS_0);
    if (msgid <= 0) {
        PR_ERR("Subscribe failed!");
    }
    PR_DEBUG("Subscribe topic tuya/tos-test ID:%d", msgid);
}

static void mqtt_client_disconnected_cb(void *client, void *userdata)
{
    PR_INFO("mqtt client disconnected!");

    // PR_DEBUG("MQTT Client Deinit");
    // mqtt_client_deinit(client);
}

static void mqtt_client_message_cb(void *client, uint16_t msgid, const mqtt_client_message_t *msg, void *userdata)
{
    PR_DEBUG("recv message TopicName:%s, payload len:%d", msg->topic, msg->length);
}

static void mqtt_client_subscribed_cb(void *client, uint16_t msgid, void *userdata)
{
    PR_DEBUG("Subscribe successed ID:%d", msgid);
    uint16_t new_msgid = mqtt_client_publish(client, "tuya/tos-test", "hello, tuya-open-sdk-for-device",
                                             strlen("hello, tuya-open-sdk-for-device") + 1, MQTT_QOS_1);
    if (new_msgid <= 0) {
        PR_ERR("Publish failed!");
    }
    PR_DEBUG("Publish msg ID:%d", new_msgid);
}

static void mqtt_client_puback_cb(void *client, uint16_t msgid, void *userdata)
{
    PR_DEBUG("PUBACK successed ID:%d", msgid);
    PR_DEBUG("UnSubscribe topic tuya/tos-test");
    mqtt_client_unsubscribe(client, "tuya/tos-test", MQTT_QOS_0);

    PR_DEBUG("MQTT Client Disconnect");
    mqtt_client_disconnect(client);
}

/**
 * @brief  __link_status_cb
 *
 * @param[in] param:Task parameters
 * @return none
 */
OPERATE_RET __link_status_cb(void *data)
{
    int rt = OPRT_OK;
    static netmgr_status_e status = NETMGR_LINK_DOWN;
    if (status == (netmgr_status_e)data && NETMGR_LINK_UP == (netmgr_status_e)data)
        return OPRT_OK;

    status = (netmgr_status_e)data;
    PR_DEBUG("start mqtt client");
    /* MQTT Client init */
    mqtt_client_context_t mqtt_client = {0};
    mqtt_client_status_t mqtt_status;
    const mqtt_client_config_t mqtt_config = {.cacert = NULL,
                                              .cacert_len = 0,
                                              .host = "broker.emqx.io",
                                              .port = 1883,
                                              .keepalive = MQTT_KEEPALIVE_INTERVALIN,
                                              .timeout_ms = MATOP_TIMEOUT_MS_DEFAULT,
                                              .clientid = "tuya-open-sdk-for-device-01",
                                              .username = "emqx",
                                              .password = "public",
                                              .on_connected = mqtt_client_connected_cb,
                                              .on_disconnected = mqtt_client_disconnected_cb,
                                              .on_message = mqtt_client_message_cb,
                                              .on_subscribed = mqtt_client_subscribed_cb,
                                              .on_published = mqtt_client_puback_cb,
                                              .userdata = NULL};
    mqtt_status = mqtt_client_init(&mqtt_client, &mqtt_config);
    if (mqtt_status != MQTT_STATUS_SUCCESS) {
        PR_ERR("MQTT init failed: Status = %d.", mqtt_status);
        return OPRT_COM_ERROR;
    }

    BackoffAlgorithmContext_t backoff_algorithm;
    BackoffAlgorithm_InitializeParams(&backoff_algorithm, MQTT_CONNECT_RETRY_MIN_DELAY_MS,
                                      MQTT_CONNECT_RETRY_MAX_DELAY_MS, MQTT_CONNECT_RETRY_MAX_ATTEMPTS);

    mqtt_status = mqtt_client_connect(&mqtt_client);
    if (MQTT_STATUS_NOT_AUTHORIZED == mqtt_status) {
        PR_ERR("MQTT connect fail:%d", mqtt_status);
        return OPRT_AUTHENTICATION_FAIL;
    }

    mqtt_client_yield(&mqtt_client);

err_exit:
    return rt;
}

/**
 * @brief user_main
 *
 * @return void
 */
void user_main()
{
    OPERATE_RET rt = OPRT_OK;

    /* basic init */
    tal_log_init(TAL_LOG_LEVEL_DEBUG, 1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);
    tal_kv_init(&(tal_kv_cfg_t){
        .seed = "vmlkasdh93dlvlcy",
        .key = "dflfuap134ddlduq",
    });
    tal_sw_timer_init();
    tal_workq_init();
    tal_event_subscribe(EVENT_LINK_STATUS_CHG, "tcp_client", __link_status_cb, SUBSCRIBE_TYPE_NORMAL);

    // 初始化LWIP
#if defined(ENABLE_LIBLWIP) && (ENABLE_LIBLWIP == 1)
    TUYA_LwIP_Init();
#endif

    // network init
    netmgr_type_e type = 0;
#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
    type |= NETCONN_WIFI;
#endif
#if defined(ENABLE_WIRED) && (ENABLE_WIRED == 1)
    type |= NETCONN_WIRED;
#endif
    netmgr_init(type);

#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
    // connect wifi
    netconn_wifi_info_t wifi_info = {0};
    strcpy(wifi_info.ssid, DEFAULT_WIFI_SSID);
    strcpy(wifi_info.pswd, DEFAULT_WIFI_PSWD);
    netmgr_conn_set(NETCONN_WIFI, NETCONN_CMD_SSID_PSWD, &wifi_info);
#endif

    return;
}

/**
 * @brief main
 *
 * @param argc
 * @param argv
 * @return void
 */
#if OPERATING_SYSTEM == SYSTEM_LINUX
void main(int argc, char *argv[])
{
    user_main();
    while (1) {
        tal_system_sleep(500);
    }
}
#else

/* Tuya thread handle */
static THREAD_HANDLE ty_app_thread = NULL;

/**
 * @brief  task thread
 *
 * @param[in] arg:Parameters when creating a task
 * @return none
 */
static void tuya_app_thread(void *arg)
{
    user_main();

    tal_thread_delete(ty_app_thread);
    ty_app_thread = NULL;
}

void tuya_app_main(void)
{
    THREAD_CFG_T thrd_param = {4096, 4, "tuya_app_main"};
    tal_thread_create_and_start(&ty_app_thread, NULL, NULL, tuya_app_thread, NULL, &thrd_param);
}
#endif