/**
 * @file websocket_transporter.h
 * @brief Header file for WebSocket transporter interface.
 *
 * This header file defines the interface for the WebSocket transporter used in
 * Tuya devices. It provides the necessary structures and function prototypes
 * for establishing WebSocket connections, enabling the sending and receiving of
 * data over WebSocket protocols. The WebSocket transporter facilitates
 * real-time communication between Tuya devices and the Tuya cloud platform or
 * other networked services, leveraging the full-duplex communication
 * capabilities of WebSockets.
 *
 * The file includes definitions for configuring WebSocket connections, such as
 * URL scheme and path, and outlines the process for creating and managing
 * WebSocket transporter instances.
 *
 * @note This interface is conditionally available based on the compilation flag
 * ENABLE_WEBSOCKET.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __WEBSOCKET_TRANSPORTER_H__
#define __WEBSOCKET_TRANSPORTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ENABLE_WEBSOCKET) && (ENABLE_WEBSOCKET == 1)

/*websocket transporter command definitions*/

struct websocket_config_t {
    char *scheme;
    char *path;
};

/**
 *
 */
typedef struct websocket_config_t tuya_websocket_config_t;

/**
 * @brief Creates a WebSocket transporter for Tuya Cloud service.
 *
 * This function creates and initializes a WebSocket transporter for
 * communicating with the Tuya Cloud service. The transporter is responsible for
 * establishing and maintaining a WebSocket connection with the server.
 *
 * @return The created WebSocket transporter.
 */
tuya_transporter_t tuya_websocket_transporter_create();

/**
 * @brief Destroys a WebSocket transporter.
 *
 * This function destroys the WebSocket transporter specified by the
 * `transporter` parameter. It releases any resources associated with the
 * transporter and frees the memory.
 *
 * @param transporter The WebSocket transporter to destroy.
 * @return OPERATE_RET Returns OPRT_OK if the transporter is successfully
 * destroyed, or an error code otherwise.
 */
OPERATE_RET tuya_websocket_transporter_destroy(tuya_transporter_t transporter);

/**
 * @brief Sets the path for the WebSocket transporter.
 *
 * This function is used to set the path for the WebSocket transporter. The path
 * is a string that specifies the endpoint to which the WebSocket connection
 * will be established.
 *
 * @param transporter The WebSocket transporter to set the path for.
 * @param path The path to set for the WebSocket transporter.
 * @return OPERATE_RET Returns OPRT_OK if the path is set successfully, or an
 * error code if an error occurs.
 */
OPERATE_RET tuya_websocket_set_path(tuya_transporter_t transporter, const char *path);

#ifdef __cplusplus
}
#endif

#endif

#endif /* __WEBSOCKET_TRANSPORTER_H__ */
