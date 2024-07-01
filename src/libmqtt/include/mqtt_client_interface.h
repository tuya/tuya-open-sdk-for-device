/**
 * @file mqtt_client_interface.h
 * @brief Contains MQTT Statuses, function type definitions and mqtt interface
 * structure.
 */

#ifndef MQTT_CLIENT_INTERFACE_H
#define MQTT_CLIENT_INTERFACE_H

/* Standard library includes. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @ingroup mqtt_enum_types
 * @brief The MQTT interface return status.
 */
typedef enum mqtt_client_status {
    MQTT_STATUS_SUCCESS = 0, /*!< @brief MQTT interface success. */
    MQTT_STATUS_INVALID_PARAM,
    MQTT_STATUS_CONNECT_FAILED,
    MQTT_STATUS_NOT_AUTHORIZED,
    MQTT_STATUS_NETWORK_INIT_FAILED,
    MQTT_STATUS_NETWORK_CONNECT_FAILED,
    MQTT_STATUS_NETWORK_TIMEOUT,
} mqtt_client_status_t;

typedef enum mqtt_cleint_qos {
    MQTT_QOS_0 = 0, /**< Delivery at most once. */

    MQTT_QOS_1 = 1, /**< Delivery at least once. */
    MQTT_QOS_2 = 2  /**< Delivery exactly once. */
} mqtt_client_qos_t;

typedef struct mqtt_client_message {
    const char *topic;
    const uint8_t *payload;
    size_t length;
    mqtt_client_qos_t qos;
} mqtt_client_message_t;

typedef struct {
    const uint8_t *cacert;
    size_t cacert_len;
    const char *host;
    uint16_t port;
    uint16_t keepalive;
    uint32_t timeout_ms;
    const char *clientid;
    const char *username;
    const char *password;
    void *userdata;
    void (*on_connected)(void *client, void *userdata);
    void (*on_disconnected)(void *client, void *userdata);
    void (*on_message)(void *client, uint16_t msgid, const mqtt_client_message_t *msg, void *userdata);
    void (*on_published)(void *client, uint16_t msgid, void *userdata);
    void (*on_subscribed)(void *client, uint16_t msgid, void *userdata);
    void (*on_unsubscribed)(void *client, uint16_t msgid, void *userdata);
} mqtt_client_config_t;

void *mqtt_client_new(void);

void mqtt_client_free(void *client);

mqtt_client_status_t mqtt_client_init(void *client, const mqtt_client_config_t *config);

mqtt_client_status_t mqtt_client_deinit(void *client);

mqtt_client_status_t mqtt_client_connect(void *client);

mqtt_client_status_t mqtt_client_disconnect(void *client);

mqtt_client_status_t mqtt_client_yield(void *client);

uint16_t mqtt_client_subscribe(void *client, const char *topic, uint8_t qos);

uint16_t mqtt_client_unsubscribe(void *client, const char *topic, uint8_t qos);

uint16_t mqtt_client_publish(void *client, const char *topic, const uint8_t *payload, size_t length, uint8_t qos);

#endif /* ifndef MQTT_CLIENT_INTERFACE_H */
