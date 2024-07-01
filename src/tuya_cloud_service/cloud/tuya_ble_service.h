/**
 * @file tuya_ble_service.h
 * @brief Header file for Tuya's BLE (Bluetooth Low Energy) services.
 *
 * This file contains the declarations and data structures necessary for
 * initializing and using Tuya's BLE services. It includes the definition of
 * initialization parameters for the BLE service and a structure for handling
 * user data in BLE operations. The BLE service facilitates communication
 * between Tuya's IoT devices and the Tuya cloud platform or mobile applications
 * over Bluetooth Low Energy, supporting various operations such as device
 * pairing, data transmission, and command execution.
 *
 * The structures and functions defined in this file are essential for
 * developers to integrate Tuya's BLE capabilities into their IoT solutions,
 * enabling secure and efficient wireless communication.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_BLE_SERVICE_H__
#define __TUYA_BLE_SERVICE_H__

#include <stdint.h>
#include "tuya_cloud_types.h"
#include "tuya_iot.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/
typedef struct {
    uint8_t *pid;
    uint8_t *uuid;
    uint8_t *auth_key;
} tuya_ble_service_init_params_t;

typedef struct {
    uint16_t cmd;
    uint16_t in_len;
    uint8_t *in_data;
    uint8_t *out_data;
    uint16_t *out_len;
} tuya_ble_user_data_t;

/***********************************************************
***********************typedef define***********************
***********************************************************/
#define BLE_USER_SESSION_MAX 2

/***********************************************************
********************function declaration********************
***********************************************************/

typedef void (*ble_user_session_t)(tuya_ble_user_data_t *user_data);

/**
 * @brief Starts the Tuya BLE service.
 *
 * This function initializes and starts the Tuya BLE service with the provided
 * initialization parameters.
 *
 * @param init_params Pointer to the initialization parameters for the Tuya BLE
 * service.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int tuya_ble_service_start(tuya_ble_service_init_params_t *init_params);

/**
 * @brief Stops the Tuya BLE service.
 *
 * This function stops the Tuya BLE service and performs any necessary cleanup.
 * After calling this function, the Tuya BLE service will no longer be active.
 */
void tuya_ble_service_stop(void);

/**
 * @brief Executes the Tuya BLE service loop.
 *
 * This function is responsible for executing the Tuya BLE service loop. It
 * should be called in the main loop of the application to handle Tuya BLE
 * service events and process data.
 *
 * @param argv A pointer to any additional arguments that need to be passed to
 * the function.
 */
void tuya_ble_service_loop(void *argv);

/**
 * @brief Checks if the Tuya BLE service is stopped.
 *
 * This function returns the status of the Tuya BLE service, indicating whether
 * it is currently stopped or not.
 *
 * @return 1 if the Tuya BLE service is stopped, 0 otherwise.
 */
int tuya_ble_service_is_stop(void);

/**
 * @brief Registers a BLE user session for the Tuya BLE service.
 *
 * This function is used to register a BLE user session for the Tuya BLE
 * service.
 *
 * @param session The BLE user session to register.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int tuya_ble_service_session_register(ble_user_session_t session);

/**
 * @brief Unregisters a BLE user session from the Tuya BLE service.
 *
 * This function unregisters the specified BLE user session from the Tuya BLE
 * service.
 *
 * @param session The BLE user session to unregister.
 * @return Returns an integer value indicating the status of the operation.
 *         - 0: Success
 *         - Other values: Error codes indicating the reason for failure
 */
int tuya_ble_service_session_unregister(ble_user_session_t session);

#ifdef __cplusplus
}
#endif

#endif /* __TUYA_BLE_SERVICE_H__ */
