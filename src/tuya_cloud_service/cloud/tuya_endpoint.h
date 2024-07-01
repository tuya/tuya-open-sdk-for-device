/**
 * @file tuya_endpoint.h
 * @brief Header file for Tuya endpoint management.
 *
 * This header file defines the structures and constants used for managing the
 * endpoints of Tuya IoT devices. It includes definitions for maximum lengths of
 * various fields such as region, registration key, and hostnames for both ATOP
 * and MQTT services. The `struct` definitions provide a way to store and manage
 * endpoint information including hostnames, ports, and paths necessary for the
 * device to communicate with Tuya's cloud services.
 *
 * The endpoint information is critical for ensuring that the device can
 * successfully connect to the appropriate Tuya cloud services, which may vary
 * based on the device's geographical location and operational environment. This
 * file facilitates the configuration and management of these endpoints in a
 * structured manner.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_ENDPOINT_H_
#define __TUYA_ENDPOINT_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LENGTH_REGION    (2) // max string length of REGIN IN TOKEN
#define MAX_LENGTH_REGIST    (4) // max string length of REGIST_KEY IN TOKEN
#define MAX_LENGTH_TUYA_HOST (64)
#define MAX_LENGTH_ATOP_PATH (16)

typedef struct {
    char region[MAX_LENGTH_REGION + 1]; // get from token
    struct {
        char host[MAX_LENGTH_TUYA_HOST + 1];
        uint16_t port;
        char path[MAX_LENGTH_ATOP_PATH + 1];
    } atop;
    struct {
        char host[MAX_LENGTH_TUYA_HOST + 1];
        uint16_t port;
    } mqtt;

    uint8_t *cert;
    size_t cert_len;
} tuya_endpoint_t;

/**
 * @brief Initializes the Tuya endpoint.
 *
 * This function initializes the Tuya endpoint and performs any necessary setup.
 *
 * @return 0 if the initialization is successful, otherwise a negative error
 * code.
 */
int tuya_endpoint_init(void);

/**
 * @brief Sets the region and registration key for the Tuya endpoint.
 *
 * This function is used to set the region and registration key for the Tuya
 * endpoint.
 *
 * @param region The region to set for the Tuya endpoint.
 * @param regist_key The registration key to set for the Tuya endpoint.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int tuya_endpoint_region_regist_set(const char *region, const char *regist_key);

/**
 * @brief Removes the Tuya endpoint.
 *
 * This function removes the Tuya endpoint, freeing up any resources associated
 * with it.
 *
 * @return Returns an integer value indicating the status of the operation.
 *         - 0: Success
 *         - Other values: Error codes indicating the reason for failure
 */
int tuya_endpoint_remove(void);

/**
 * @brief Updates the Tuya endpoint.
 *
 * This function is responsible for updating the Tuya endpoint.
 *
 * @return An integer value indicating the status of the update process.
 *         - 0: Update successful.
 *         - Other values: Error occurred during the update process.
 */
int tuya_endpoint_update(void);

/**
 * @brief Updates the auto region for the Tuya endpoint.
 *
 * This function updates the auto region for the Tuya endpoint.
 * It returns an integer value indicating the status of the update process.
 *
 * @return An integer value indicating the status of the update process.
 * A return value of 0 indicates success, while a non-zero value indicates an
 * error.
 */
int tuya_endpoint_update_auto_region(void);

/**
 * @brief Retrieves the Tuya endpoint.
 *
 * This function returns a pointer to the Tuya endpoint structure.
 *
 * @return A pointer to the Tuya endpoint structure.
 */
const tuya_endpoint_t *tuya_endpoint_get(void);

/**
 * @brief Sets the domain for the Tuya endpoint.
 *
 * This function sets the domain for the Tuya endpoint specified by the
 * `endpoint` parameter.
 *
 * @param endpoint Pointer to the Tuya endpoint structure.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int tuya_endpoint_domain_set(tuya_endpoint_t *endpoint);

/**
 * @brief Retrieves the domain of the Tuya endpoint.
 *
 * This function retrieves the domain of the Tuya endpoint and stores it in the
 * provided endpoint structure.
 *
 * @param endpoint Pointer to a tuya_endpoint_t structure where the domain will
 * be stored.
 * @return Returns an integer value indicating the success or failure of the
 * operation. 0 indicates success, while a negative value indicates failure.
 */
int tuya_endpoint_domain_get(tuya_endpoint_t *endpoint);

/**
 * @brief Retrieves the certificate for the Tuya endpoint.
 *
 * This function retrieves the certificate for the Tuya endpoint specified by
 * the `endpoint` parameter.
 *
 * @param endpoint Pointer to the `tuya_endpoint_t` structure representing the
 * Tuya endpoint.
 * @return Returns an integer value indicating the success or failure of the
 * operation. A return value of 0 indicates success, while a non-zero value
 * indicates failure.
 */
int tuya_endpoint_cert_get(tuya_endpoint_t *endpoint);

/**
 * @brief Sets the certificate for the Tuya endpoint.
 *
 * This function sets the certificate for the specified Tuya endpoint.
 *
 * @param endpoint Pointer to the Tuya endpoint structure.
 * @return Returns an integer value indicating the success or failure of the
 * operation. A return value of 0 indicates success, while a non-zero value
 * indicates failure.
 */
int tuya_endpoint_cert_set(tuya_endpoint_t *endpoint);

#ifdef __cplusplus
}
#endif

#endif
