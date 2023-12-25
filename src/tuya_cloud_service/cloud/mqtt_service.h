#ifndef TUYA_MQTT_SERVICE_H_
#define TUYA_MQTT_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mqtt_client_interface.h"
#include "backoff_algorithm.h"

// data max len
#define TUYA_MQTT_CLIENTID_MAXLEN (32U)
#define TUYA_MQTT_USERNAME_MAXLEN (32U)
#define TUYA_MQTT_PASSWORD_MAXLEN (32U)
#define TUYA_MQTT_CIPHER_KEY_MAXLEN (32U)
#define TUYA_MQTT_DEVICE_ID_MAXLEN (32U)
#define TUYA_MQTT_UUID_MAXLEN (32U)
#define TUYA_MQTT_TOPIC_MAXLEN (64U)
#define TUYA_MQTT_TOPIC_MAXLEN (64U)

// Tuya mqtt protocol
#define PRO_DATA_PUSH               4   /* dev -> cloud push dp data */
#define PRO_CMD                     5   /* cloud -> dev send dp data */
#define PRO_DEV_UNBIND              8   /* cloud -> dev */
#define PRO_GW_RESET                11  /* cloud -> dev reset dev */
#define PRO_TIMER_UG_INF            13  /* cloud -> dev update timer */
#define PRO_UPGD_REQ                15  /* cloud -> dev update dev/gw */
#define PRO_UPGE_PUSH               16  /* dev -> cloud update upgrade percent */
#define PRO_IOT_DA_REQ              22  /* cloud -> dev send data req */
#define PRO_IOT_DA_RESP             23  /* dev -> cloud send data resp */
#define PRO_DEV_LINE_STAT_UPDATE    25  /* dev -> sub device online status update */
#define PRO_CMD_ACK                 26  /* dev -> cloud  dev send ackId to cloud */
#define PRO_MQ_EXT_CFG_INF          27  /* cloud -> dev runtime cfg update */
#define PRO_MQ_QUERY_DP             31  /* cloud -> dev query dp stat */
#define PRO_GW_SIGMESH_TOPO_UPDATE  33  /* cloud -> dev sigmesh topo update */
#define PRO_GW_LINKAGE_UPDATE       49  /* cloud -> dev 场景更新推送 */
#define PRO_UG_SUMMER_TABLE         41  // ug sumer timer table
#define PRO_GW_UPLOAD_LOG           45  /* dev -> cloud, upload log*/
#define PRO_MQ_ACTIVE_TOKEN_ON      46  /* cloud -> dev 直连设备激活token下发 */
#define PRO_GW_LINKAGE_UPDATE       49  /* cloud -> dev 场景更新推送 */
#define PRO_MQ_THINGCONFIG          51  /* 设备免密配网 */
#define PRO_MQ_LOG_CONFIG           55  /* log config */
#define PRO_MQ_DPCACHE_NOTIFY       103 /* dp cache notify */
#define PRO_MQ_EN_GW_ADD_DEV_REQ    200 // gw enable add sub device request
#define PRO_MQ_EN_GW_ADD_DEV_RESP   201 // gw enable add sub device respond
#define PRO_DEV_LC_GROUP_OPER       202 /* cloud -> dev */
#define PRO_DEV_LC_GROUP_OPER_RESP  203 /* dev -> cloud */
#define PRO_DEV_LC_SENCE_OPER       204 /* cloud -> dev */
#define PRO_DEV_LC_SENCE_OPER_RESP  205 /* dev -> cloud */
#define PRO_DEV_LC_SENCE_EXEC       206 /* cloud -> dev */
#define PRO_CLOUD_STORAGE_ORDER_REQ 300 /* 云存储订单 */
#define PRO_3RD_PARTY_STREAMING_REQ 301 /* echo show/chromecast request */
#define PRO_RTC_REQ                 302 /* cloud -> dev */
#define PRO_AI_DETECT_DATA_SYNC_REQ 304 /* 本地AI数据更新，当前用于人脸检测样本数据更新(新增/删除/变更) */
#define PRO_FACE_DETECT_DATA_SYNC   306 /* 人脸识别数据同步通知,门禁设备使用 */
#define PRO_CLOUD_STORAGE_EVENT_REQ 307 /* 联动触发云存储 */
#define PRO_DOORBELL_STATUS_REQ     308 /* 门铃请求被用户处理，接听或者拒绝 */
#define PRO_MQ_CLOUD_STREAM_GATEWAY 312
#define PRO_GW_COM_SENCE_EXE        403 /* cloud -> dev 原云端场景放到本地执行 */
#define PRO_DEV_ALARM_DOWN    		701 /* cloud -> dev */
#define PRO_DEV_ALARM_UP      		702 /* dev -> cloud */

typedef struct {
    const char* uuid;
    const char* authkey;
    const char* devid;
    const char* seckey;
    const char* localkey;
} tuya_meta_info_t;

typedef struct {
    const uint8_t* cacert;
    size_t         cacert_len;
    const char*    host;
    uint16_t       port;
    uint32_t       timeout;
    const char*    uuid;
    const char*    authkey;
    const char*    devid;
    const char*    seckey;
    const char*    localkey;
    void*          user_data;
    void           (*on_connected)(void* context, void* user_data);
    void           (*on_disconnect)(void* context, void* user_data);
    void           (*on_unbind)(void* context, void* user_data);
} tuya_mqtt_config_t;

typedef struct {
    char clientid[TUYA_MQTT_CLIENTID_MAXLEN+1];
    char username[TUYA_MQTT_USERNAME_MAXLEN+1];
    char password[TUYA_MQTT_PASSWORD_MAXLEN+1];
    char cipherkey[TUYA_MQTT_CIPHER_KEY_MAXLEN+1];
    char topic_in[TUYA_MQTT_TOPIC_MAXLEN+1];
    char topic_out[TUYA_MQTT_TOPIC_MAXLEN+1];
} tuya_mqtt_access_t;

typedef struct {
    uint16_t event_id;
    cJSON*   root_json;
    cJSON*   data;
    void*    user_data;
} tuya_protocol_event_t;

typedef tuya_protocol_event_t tuya_mqtt_event_t; // compat TODO:remove

typedef void (*tuya_protocol_callback_t)(tuya_protocol_event_t* event);

typedef struct tuya_protocol_handle {
    struct tuya_protocol_handle* next;
    uint16_t id;
    tuya_protocol_callback_t cb;
    void* user_data;
} tuya_protocol_handle_t;

typedef void(*mqtt_subscribe_message_cb_t)(uint16_t msgid, const mqtt_client_message_t* msg, void* userdata);

typedef struct mqtt_subscribe_handle {
	struct mqtt_subscribe_handle* next;
	char* topic;
    size_t topic_length;
	mqtt_subscribe_message_cb_t cb;
	void* userdata;
} mqtt_subscribe_handle_t;

typedef void(*mqtt_publish_notify_cb_t)(int result, void* user_data);

typedef struct mqtt_publish_handle {
    struct mqtt_publish_handle* next;
    uint16_t msgid;
    int timeout;
    char* topic;
    uint8_t* payload;
    size_t payload_length;
    mqtt_publish_notify_cb_t cb;
    void* user_data;
} mqtt_publish_handle_t;

typedef struct {
    void* mqtt_client;
    tuya_mqtt_access_t signature;
    tuya_protocol_handle_t* protocol_list;
    mqtt_subscribe_handle_t* subscribe_list;
    mqtt_publish_handle_t* publish_list;
    BackoffAlgorithmContext_t backoff_algorithm;
    uint32_t sequence_in;
    uint32_t sequence_out;
    bool manual_disconnect;
    bool is_inited;
    bool is_connected;
    void* user_data;
    void (*on_connected)(void* context, void* user_data);
    void (*on_disconnect)(void* context, void* user_data);
    void (*on_unbind)(void* context, void* user_data);
} tuya_mqtt_context_t;


int tuya_mqtt_init(tuya_mqtt_context_t* context, const tuya_mqtt_config_t* config);

int tuya_mqtt_start(tuya_mqtt_context_t* context);

int tuya_mqtt_stop(tuya_mqtt_context_t* context);

int tuya_mqtt_loop(tuya_mqtt_context_t* context);

int tuya_mqtt_destory(tuya_mqtt_context_t* context);

bool tuya_mqtt_connected(tuya_mqtt_context_t* context);

int tuya_mqtt_protocol_register(tuya_mqtt_context_t* context, uint16_t protocol_id, tuya_protocol_callback_t cb, void* user_data);

int tuya_mqtt_protocol_unregister(tuya_mqtt_context_t* context, uint16_t protocol_id, tuya_protocol_callback_t cb);

int tuya_mqtt_protocol_data_publish(tuya_mqtt_context_t* context, uint16_t protocol_id, const uint8_t* data, uint16_t length);

int tuya_mqtt_protocol_data_publish_with_topic(tuya_mqtt_context_t* context, const char* topic, uint16_t protocol_id, const uint8_t* data, uint16_t length);

int tuya_mqtt_protocol_data_publish_common(tuya_mqtt_context_t* context,
										   uint16_t protocol_id, const uint8_t* data, uint16_t length,
										   mqtt_publish_notify_cb_t cb, void* user_data,
										   int timeout_ms, bool async);

int tuya_mqtt_protocol_data_publish_with_topic_common(tuya_mqtt_context_t* context, const char* topic, 
										              uint16_t protocol_id, const uint8_t* data, uint16_t length,
										              mqtt_publish_notify_cb_t cb, void* user_data,
										              int timeout_ms, bool async);

int tuya_mqtt_subscribe_message_callback_register(tuya_mqtt_context_t* context, const char* topic, mqtt_subscribe_message_cb_t cb, void* userdata);

int tuya_mqtt_subscribe_message_callback_unregister(tuya_mqtt_context_t* context, const char* topic);

int tuya_mqtt_upgrade_progress_report(tuya_mqtt_context_t* context, int channel, int percent);

#ifdef __cplusplus
}
#endif
#endif
