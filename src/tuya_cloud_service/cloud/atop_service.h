/**
 * @file atop_service.h
 * @brief Header file for ATOP service functions.
 *
 * This file defines the structures and enumerations for the ATOP service
 * functions. It includes the definition of the activation request structure
 * `tuya_activite_request_t` and the enumeration for HTTP dynamic configuration
 * options. These components are essential for the activation process and
 * communication with the Tuya cloud platform, facilitating device activation,
 * data reporting, and other cloud services.
 *
 * The ATOP service functions aim to provide a secure and efficient way for IoT
 * devices to communicate with the Tuya cloud, supporting a wide range of
 * operations such as device activation, status updates, and firmware upgrades.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __ATOP_SERVICE_H_
#define __ATOP_SERVICE_H_

#include "atop_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *token;
    const char *product_key;
    const char *firmware_key;
    const char *uuid;
    const char *devid;
    const char *authkey;
    const char *sw_ver;
    const char *pv;
    const char *bv;
    const char *modules;
    const char *feature;
    const char *skill_param;
    const void *user_data;
} tuya_activite_request_t;

typedef enum {
    HTTP_DYNAMIC_CFG_ALL,      // all
    HTTP_DYNAMIC_CFG_TZ,       // time zone
    HTTP_DYNAMIC_CFG_RATERULE, // rate rule for dp
} HTTP_DYNAMIC_CFG_TYPE;

typedef enum {
    DEV_STATUS_UNKNOWN,
    DEV_STATUS_RESET,
    DEV_STATUS_RESET_FACTORY,
    DEV_STATUS_ENABLE,
} DEV_SYNC_STATUS_E;

/**
 * @brief Sends an activate request to the Tuya cloud service.
 *
 * This function sends an activate request to the Tuya cloud service using the
 * provided request data. The response from the cloud service is stored in the
 * provided response structure.
 *
 * @param request Pointer to the activate request data.
 * @param response Pointer to the response structure to store the cloud service
 * response.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int atop_service_activate_request(const tuya_activite_request_t *request, atop_base_response_t *response);

/**
 * @brief Resets the atop service client with the specified ID and key.
 *
 * This function resets the atop service client by providing the client ID and
 * key.
 *
 * @param id The client ID.
 * @param key The client key.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int atop_service_client_reset(const char *id, const char *key);

/**
 * @brief Retrieves the dynamic configuration for a service.
 *
 * This function retrieves the dynamic configuration for a service identified by
 * the given ID and key. The configuration type is specified by the 'type'
 * parameter. The retrieved configuration is stored in the 'response' parameter.
 *
 * @param id The ID of the service.
 * @param key The key of the service.
 * @param type The type of the dynamic configuration.
 * @param response Pointer to the structure where the retrieved configuration
 * will be stored.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int atop_service_dynamic_cfg_get_v20(const char *id, const char *key, HTTP_DYNAMIC_CFG_TYPE type,
                                     atop_base_response_t *response);

/**
 * @brief Retrieves upgrade information for a specific device.
 *
 * This function retrieves upgrade information for a device identified by its ID
 * and key. The upgrade information includes details such as the upgrade channel
 * and the response status.
 *
 * @param id The ID of the device.
 * @param key The key of the device.
 * @param channel The upgrade channel.
 * @param response Pointer to the structure where the response will be stored.
 * @return Returns 0 on success, or an error code on failure.
 */
int atop_service_upgrade_info_get_v44(const char *id, const char *key, int channel, atop_base_response_t *response);

/**
 * @brief Updates the upgrade status for the ATOP service.
 *
 * This function is used to update the upgrade status for the ATOP service.
 *
 * @param id The ID of the service.
 * @param key The key of the service.
 * @param channel The channel of the service.
 * @param status The status to be updated.
 * @return The result of the update operation.
 */
int atop_service_upgrade_status_update_v41(const char *id, const char *key, int channel, int status);

/**
 * @brief Updates the version information for a service.
 *
 * This function updates the version information for a service identified by the
 * given ID and key. The updated version information is provided as a string in
 * the `versions` parameter.
 *
 * @param id The ID of the service.
 * @param key The key of the service.
 * @param versions The updated version information.
 * @return An integer value indicating the status of the version update.
 *         - 0: Version update successful.
 *         - -1: Version update failed.
 */
int atop_service_version_update_v41(const char *id, const char *key, const char *versions);

/**
 * @brief Retrieves auto upgrade information for a service.
 *
 * This function retrieves auto upgrade information for a service identified by
 * the provided ID and key. The retrieved information is stored in the provided
 * response structure.
 *
 * @param id The ID of the service.
 * @param key The key of the service.
 * @param response Pointer to the response structure where the retrieved
 * information will be stored.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int atop_service_auto_upgrade_info_get_v44(const char *id, const char *key, atop_base_response_t *response);

/**
 * @brief Sends a reset log to the Tuya cloud service.
 *
 * This function sends a reset log to the Tuya cloud service using the provided
 * ID, key, and reset buffer.
 *
 * @param id The ID of the device.
 * @param key The key of the device.
 * @param rst_buffer The reset buffer containing the log data.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */
int atop_service_put_rst_log_v10(const char *id, const char *key, const char *rst_buffer);

/**
 * @brief Uploads outdoors property to the Tuya cloud service.
 *
 * This function uploads the outdoors property identified by the given `id` and
 * `key` to the Tuya cloud service. The `countryCode` and `phone` parameters are
 * used for authentication purposes.
 *
 * @param id The ID of the outdoors property.
 * @param key The key of the outdoors property.
 * @param countryCode The country code for authentication.
 * @param phone The phone number for authentication.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int atop_service_outdoors_property_upload(const char *id, const char *key, const char *countryCode, const char *phone);

/**
 * @brief Uploads the ICCID (Integrated Circuit Card Identifier) to the Tuya
 * Cloud service.
 *
 * This function is used to upload the ICCID of a device to the Tuya Cloud
 * service.
 *
 * @param id The ID of the device.
 * @param key The key of the device.
 * @param iccid The ICCID to be uploaded.
 *
 * @return Returns an integer value indicating the success or failure of the
 * operation.
 *         - 0: Success
 *         - Other values: Failure
 */
int atop_service_iccid_upload(const char *id, const char *key, const char *iccid);

/**
 * @brief Retrieves the cached data points (DPs) for a specific device.
 *
 * This function retrieves the cached data points (DPs) for a specific device
 * identified by the provided `id` and `key`. The requested DPs are specified
 * by the `req_dps` parameter. The retrieved DPs are stored in the `response`
 * parameter.
 *
 * @param id The identifier of the device.
 * @param key The key of the device.
 * @param req_dps The requested data points (DPs).
 * @param response The response structure to store the retrieved DPs.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int atop_service_cache_dp_get(const char *id, const char *key, const char *req_dps, atop_base_response_t *response);

/**
 * @brief Synchronizes the check for the device with the cloud service.
 *
 * This function synchronizes the check for the device with the cloud service.
 *
 * @param id The device ID.
 * @param key The device key.
 * @param p_status Pointer to a variable to store the synchronization status.
 *
 * @return The result of the synchronization check.
 */
int atop_service_sync_check(const char *id, const char *key, DEV_SYNC_STATUS_E *p_status);

/**
 * @brief Enables communication with a specific node in the ATOP service.
 *
 * This function enables communication with a specific node in the ATOP service
 * by providing the node's ID and key.
 *
 * @param id The ID of the node to enable communication with.
 * @param key The key of the node to enable communication with.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int atop_service_comm_node_enable(const char *id, const char *key);

/**
 * @brief Disables the communication node for the ATOP service.
 *
 * This function disables the communication node for the ATOP service using the
 * provided ID and key.
 *
 * @param id The ID of the communication node.
 * @param key The key of the communication node.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int atop_service_comm_node_disable(const char *id, const char *key);

/**
 * @brief Posts a simple communication request to the Tuya Cloud Service.
 *
 * This function sends a communication request to the Tuya Cloud Service using
 * the specified API, version, body, and user data.
 *
 * @param api The API endpoint to send the request to.
 * @param version The version of the API.
 * @param body The request body.
 * @param user_data Additional user data to include in the request.
 * @param result A pointer to a cJSON object that will store the response from
 * the Tuya Cloud Service.
 *
 * @return An integer value indicating the success or failure of the
 * communication request.
 *         - 0: Success.
 *         - Other values: Failure.
 */
int atop_service_comm_post_simple(const char *api, const char *version, const char *body, const char *user_data,
                                  cJSON **result);

#ifdef __cplusplus
}
#endif
#endif
