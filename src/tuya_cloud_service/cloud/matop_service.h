/**
 * @file matop_service.h
 * @brief Header file for the MATOP service, defining structures and functions
 * for MQTT atop protocol operations.
 *
 * This file declares the data structures and callback types used by the MATOP
 * service to handle MQTT messages according to the ATOP protocol. It includes
 * definitions for request and response handling, as well as message queue
 * management for asynchronous communication with the Tuya cloud platform.
 *
 * The MATOP service facilitates the integration of MQTT communication with the
 * ATOP protocol, enabling efficient and reliable message exchange between IoT
 * devices and the Tuya cloud services.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef MATOP_SERVICE_H_
#define MATOP_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "atop_base.h"
#include "atop_service.h"
#include "mqtt_service.h"

typedef struct {
    const char *api;
    const char *version;
    uint8_t *data;
    size_t data_len;
    uint32_t timeout;
} mqtt_atop_request_t;

typedef void (*mqtt_atop_response_cb_t)(atop_base_response_t *response, void *user_data);

typedef struct mqtt_atop_message {
    struct mqtt_atop_message *next;
    uint16_t id;
    uint32_t timeout;
    mqtt_atop_response_cb_t notify_cb;
    void *user_data;
} mqtt_atop_message_t;

typedef struct matop_config {
    tuya_mqtt_context_t *mqctx;
    const char *devid;
} matop_config_t;

typedef struct matop_context {
    matop_config_t config;
    uint32_t id_cnt;
    char resquest_topic[64];
    mqtt_atop_message_t *message_list;
} matop_context_t;

/**
 * @brief Initializes the MATOP service.
 *
 * This function initializes the MATOP service with the provided context and
 * configuration.
 *
 * @param context Pointer to the MATOP context.
 * @param config Pointer to the MATOP configuration.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_serice_init(matop_context_t *context, const matop_config_t *config);

/**
 * @brief Yields the execution of the matop service.
 *
 * This function allows the matop service to yield the execution and perform any
 * necessary operations.
 *
 * @param context Pointer to the matop context.
 * @return Returns an integer value indicating the status of the operation.
 */
int matop_serice_yield(matop_context_t *context);

/**
 * @brief Destroys the matop service context.
 *
 * This function is used to destroy the matop service context and free up any
 * allocated resources.
 *
 * @param context The pointer to the matop service context.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_serice_destory(matop_context_t *context);

/**
 * @brief Sends an asynchronous request to the MATOP service.
 *
 * This function sends an asynchronous request to the MATOP service using the
 * provided context, request, notification callback, and user data.
 *
 * @param context The MATOP context.
 * @param request The MQTT ATOP request.
 * @param notify_cb The notification callback function to be called when a
 * response is received.
 * @param user_data User data to be passed to the notification callback.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_service_request_async(matop_context_t *context, const mqtt_atop_request_t *request,
                                mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * @brief Resets the MATOP service client.
 *
 * This function resets the MATOP service client by clearing the specified
 * context.
 *
 * @param context A pointer to the MATOP context.
 * @return An integer value indicating the result of the operation.
 *         Returns 0 if the operation is successful, otherwise returns an error
 * code.
 */
int matop_service_client_reset(matop_context_t *context);

/**
 * @brief Updates the version of the matop service.
 *
 * This function updates the version of the matop service in the specified
 * context.
 *
 * @param context A pointer to the matop_context_t structure.
 * @param versions The new version to be set for the matop service.
 *
 * @return An integer value indicating the status of the operation.
 *         - 0: Success
 *         - Other values: Error codes indicating the cause of failure
 */
int matop_service_version_update(matop_context_t *context, const char *versions);

/**
 * @brief Updates the upgrade status for the MATOP service.
 *
 * This function is used to update the upgrade status for the MATOP service.
 *
 * @param context A pointer to the MATOP context.
 * @param channel The channel number.
 * @param status The upgrade status to be updated.
 *
 * @return The result of the upgrade status update operation.
 */
int matop_service_upgrade_status_update(matop_context_t *context, int channel, int status);

/**
 * @brief Retrieves upgrade information for the MATOP service.
 *
 * This function retrieves upgrade information for the MATOP service based on
 * the provided context and channel.
 *
 * @param context The MATOP context.
 * @param channel The channel to retrieve upgrade information for.
 * @param notify_cb The callback function to be called when the upgrade
 * information is available.
 * @param user_data User data to be passed to the callback function.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_service_upgrade_info_get(matop_context_t *context, int channel, mqtt_atop_response_cb_t notify_cb,
                                   void *user_data);

/**
 * @brief Retrieves the auto upgrade information from the MATOP service.
 *
 * This function retrieves the auto upgrade information from the MATOP service
 * using the provided MATOP context. The result of the operation will be
 * notified through the specified callback function.
 *
 * @param context The MATOP context.
 * @param notify_cb The callback function to notify the result of the operation.
 * @param user_data User data to be passed to the callback function.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int matop_service_auto_upgrade_info_get(matop_context_t *context, mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * Downloads a file from the specified URL within the given range.
 *
 * @param context The MATOP context.
 * @param url The URL of the file to download.
 * @param range_start The starting byte position of the range to download.
 * @param range_end The ending byte position of the range to download.
 * @param timeout_ms The timeout value in milliseconds for the download
 * operation.
 * @param notify_cb The callback function to be called when the download
 * operation completes or encounters an error.
 * @param user_data User-defined data to be passed to the callback function.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_service_file_download_range(matop_context_t *context, const char *url, int range_start, int range_end,
                                      uint32_t timeout_ms, mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * @brief Puts a reset log for the MATOP service.
 *
 * This function is used to put a reset log for the MATOP service.
 *
 * @param context The MATOP context.
 * @param reason The reason for the reset.
 * @return The result of the operation.
 */
int matop_service_put_rst_log(matop_context_t *context, int reason);

/**
 * @brief Retrieves the dynamic configuration for the MATOP service.
 *
 * This function is used to retrieve the dynamic configuration for the MATOP
 * service.
 *
 * @param context The MATOP context.
 * @param type The type of dynamic configuration to retrieve.
 * @param notify_cb The callback function to be called when the configuration is
 * retrieved.
 * @param user_data User data to be passed to the callback function.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int matop_service_dynamic_cfg_get(matop_context_t *context, HTTP_DYNAMIC_CFG_TYPE type,
                                  mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * @brief Sends an acknowledgement for dynamic configuration changes in the
 * MATOP service.
 *
 * This function is used to send an acknowledgement for dynamic configuration
 * changes in the MATOP service.
 *
 * @param context The MATOP context.
 * @param timezone_ackId The acknowledgement ID for the timezone configuration
 * change.
 * @param rateRule_actId The acknowledgement ID for the rate rule configuration
 * change.
 * @param notify_cb The callback function to be called when the acknowledgement
 * is received.
 * @param user_data User data to be passed to the callback function.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */
int matop_service_dynamic_cfg_ack(matop_context_t *context, const char *timezone_ackId, const char *rateRule_actId,
                                  mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * @brief Enables communication with the MATOP service on a specific node.
 *
 * This function enables communication with the MATOP service on a specific
 * node. It registers a callback function to receive notifications from the
 * MATOP service.
 *
 * @param context The MATOP context.
 * @param notify_cb The callback function to be called when a notification is
 * received.
 * @param user_data User data to be passed to the callback function.
 *
 * @return 0 if successful, otherwise an error code.
 */
int matop_service_comm_node_enable(matop_context_t *context, mqtt_atop_response_cb_t notify_cb, void *user_data);

/**
 * @brief Disables communication with a MATOP service node.
 *
 * This function disables communication with a MATOP service node. It takes a
 * pointer to a `matop_context_t` structure representing the MATOP context, a
 * callback function `notify_cb` to receive notifications, and a pointer
 * `user_data` to user-defined data.
 *
 * @param context The MATOP context.
 * @param notify_cb The callback function to receive notifications.
 * @param user_data User-defined data.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int matop_service_comm_node_disable(matop_context_t *context, mqtt_atop_response_cb_t notify_cb, void *user_data);

#ifdef __cplusplus
}
#endif
#endif