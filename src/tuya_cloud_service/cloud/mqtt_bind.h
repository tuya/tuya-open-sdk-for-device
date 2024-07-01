/**
 * @file mqtt_bind.c
 * @brief Implementation of the MQTT binding functionality for Tuya devices.
 *
 * This file provides the implementation for obtaining an MQTT token required
 * for the MQTT binding process. It utilizes the Tuya IoT SDK to communicate
 * with the Tuya cloud platform, facilitating a secure and reliable connection
 * for MQTT-based device communication.
 *
 * The function `mqtt_bind_token_get` defined in this file is responsible for
 * initiating the token retrieval process, which is a critical step in
 * establishing an MQTT session between the device and the Tuya cloud services.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __MQTT_BIND_H_
#define __MQTT_BIND_H_

#include "tuya_iot.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieves the MQTT bind token from the Tuya IoT configuration.
 *
 * This function retrieves the MQTT bind token from the provided Tuya IoT
 * configuration.
 *
 * @param config Pointer to the Tuya IoT configuration structure.
 * @return The MQTT bind token.
 */
int mqtt_bind_token_get(tuya_iot_config_t *config);

#ifdef __cplusplus
}
#endif
#endif
