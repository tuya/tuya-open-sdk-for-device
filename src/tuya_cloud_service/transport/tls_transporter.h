/**
 * @file tls_transporter.h
 * @brief Header file for TLS Transporter functions.
 *
 * This file declares the interface for the TLS Transporter within the Tuya IoT
 * SDK. The TLS Transporter provides secure communication channels for Tuya
 * devices by establishing TLS connections over TCP/IP networks. It includes
 * functions for creating and destroying TLS transporter instances, thereby
 * enabling secure data exchange with encryption between Tuya devices and the
 * Tuya cloud platform.
 *
 * The creation function initializes a new TLS transporter and returns a handle
 * for further operations. The destruction function releases any resources
 * allocated to the TLS transporter, ensuring a clean shutdown of the TLS
 * connection.
 *
 * @note: For successful operation, ensure that the underlying TCP transporter
 * is properly configured.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TLS_TRANSPORTER_H__
#define __TLS_TRANSPORTER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_transporter.h"

/**
 * @brief Creates a TLS transporter for Tuya Cloud service.
 *
 * This function creates and returns a Tuya transporter object that uses TLS
 * (Transport Layer Security) for secure communication with the Tuya Cloud
 * service.
 *
 * @return The created Tuya transporter object.
 */
tuya_transporter_t tuya_tls_transporter_create();

/**
 * @brief Destroys a TLS transporter.
 *
 * This function destroys a TLS transporter and frees any associated resources.
 *
 * @param transporter The TLS transporter to destroy.
 * @return The result of the operation.
 */
OPERATE_RET tuya_tls_transporter_destroy(tuya_transporter_t transporter);

/**
 * @brief Controls the TLS transporter.
 *
 * This function is used to control the TLS transporter by sending different
 * commands.
 *
 * @param t The TLS transporter to control.
 * @param cmd The command to send.
 * @param args The arguments for the command.
 *
 * @return The result of the operation.
 */
OPERATE_RET tuya_tls_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void *args);

/**
 * @brief Closes the TLS transporter.
 *
 * This function is used to close the TLS transporter associated with the given
 * `tuya_transporter_t` object.
 *
 * @param t The TLS transporter to close.
 * @return The result of the operation. Possible values are:
 *         - OPRT_OK: The TLS transporter was closed successfully.
 *         - Other error codes indicating the failure reason.
 */
OPERATE_RET tuya_tls_transporter_close(tuya_transporter_t t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
