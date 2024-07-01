/**
 * @file tcp_transporter.h
 * @brief Header file for TCP transporter functions.
 *
 * This file declares the interface for creating and destroying TCP transporters
 * within the Tuya IoT SDK. A TCP transporter is responsible for establishing
 * and managing TCP connections, enabling the transmission of data between Tuya
 * devices and the Tuya cloud platform or other networked services. The
 * functions provided here allow for the creation of a TCP transporter instance
 * and its subsequent destruction, facilitating clean resource management and
 * termination of TCP connections.
 *
 * The creation function initializes a new TCP transporter and returns a handle
 * for further operations, while the destruction function releases any resources
 * allocated to the TCP transporter and properly closes the TCP connection.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TCP_TRANSPORTER_H__
#define __TCP_TRANSPORTER_H__

#include "tuya_transporter.h"

/**
 * @brief Creates a TCP transporter for Tuya Cloud Service.
 *
 * This function creates and returns a TCP transporter object that can be used
 * to establish a connection with the Tuya Cloud Service.
 *
 * @return The created TCP transporter object.
 */
tuya_transporter_t tuya_tcp_transporter_create(void);

/**
 * @brief Destroys a TCP transporter.
 *
 * This function destroys a TCP transporter and releases any resources
 * associated with it.
 *
 * @param transporter The TCP transporter to destroy.
 * @return The result of the operation.
 */
OPERATE_RET tuya_tcp_transporter_destroy(tuya_transporter_t transporter);

#endif
