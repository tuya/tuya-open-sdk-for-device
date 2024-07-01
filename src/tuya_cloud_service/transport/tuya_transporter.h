/**
 * @file tuya_transporter.h
 * @brief Network transport layer abstract interface.
 *
 * This file defines the abstract interface for the network transport layer,
 * facilitating the integration and management of different transport mechanisms
 * such as TCP, TLS, and WebSocket within the Tuya IoT SDK. It introduces the
 * concept of a transport array and transporters, where a transport array is
 * used to manage multiple transporters. MQTT client or HTTP client, upon
 * initialization, creates a transport array and adds the necessary transporters
 * (TCP transporter, TLS transporter, or WebSocket transporter) as required. The
 * appropriate transporter can then be retrieved from the transport array for
 * data transmission needs, providing a unified interface for utilizing TCP,
 * TLS, or WebSocket services.
 *
 * The structure of the transport component is designed to offer flexibility and
 * modularity in handling network communications, ensuring that IoT devices can
 * efficiently switch between different transport protocols as needed.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 */

#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

/*tuya transporter command definitions*/
#define TUYA_TRANSPORTER_SET_TLS_CERT         0x0001
#define TUYA_TRANSPORTER_GET_TCP_SOCKET       0x0002
#define TUYA_TRANSPORTER_SET_TCP_CONFIG       0x0003
#define TUYA_TRANSPORTER_SET_WEBSOCKET_CONFIG 0x0004
#define TUYA_TRANSPORTER_SET_TLS_CONFIG       0x0005
#define TUYA_TRANSPORTER_GET_TLS_CONFIG       0x0006

struct socket_config_t {
    uint8_t isBlock;
    uint8_t isReuse;
    uint8_t isDisableNagle;
    uint8_t isKeepAlive;
    uint16_t bindPort;
    TUYA_IP_ADDR_T bindAddr;
    uint32_t sendTimeoutMs;
    uint32_t recvTimeoutMs;
    uint32_t keepAliveIdleTime;
    uint32_t keepAliveInterval;
    uint32_t keepAliveCount;
};

typedef uint8_t TUYA_TRANSPORT_TYPE_E;

#define TRANSPORT_TYPE_TCP       (1) // tcp transporter
#define TRANSPORT_TYPE_TLS       (2) // tls transporter
#define TRANSPORT_TYPE_WEBSOCKET (3) // websocket transporter

typedef struct socket_config_t tuya_tcp_config_t;

typedef struct tuya_transport_array_handle *tuya_transport_array_handle_t;

struct tuya_transporter_inter_t;

typedef struct tuya_transporter_inter_t *tuya_transporter_t;

typedef OPERATE_RET (*transporter_destroy_fn)(tuya_transporter_t t);

typedef OPERATE_RET (*transporter_connect_fn)(tuya_transporter_t transporter, const char *host, int port,
                                              int timeout_ms);

typedef OPERATE_RET (*transporter_read_fn)(tuya_transporter_t transporter, uint8_t *buf, int len, int timeout_ms);

typedef OPERATE_RET (*transporter_write_fn)(tuya_transporter_t transporter, uint8_t *buf, int len, int timeout_ms);

typedef OPERATE_RET (*transporter_poll_read_fn)(tuya_transporter_t transporter, int timeout_ms);

typedef OPERATE_RET (*transporter_poll_write_fn)(tuya_transporter_t transporter, int timeout_ms);

typedef OPERATE_RET (*transporter_close_fn)(tuya_transporter_t transporter);

typedef OPERATE_RET (*transporter_ctrl)(tuya_transporter_t transporter, uint32_t cmd, void *args);

struct tuya_transporter_inter_t {
    char *scheme;
    transporter_connect_fn f_connect;
    transporter_read_fn f_read;
    transporter_write_fn f_write;
    transporter_poll_read_fn f_poll_read;
    transporter_poll_write_fn f_poll_write;
    transporter_close_fn f_close;
    transporter_destroy_fn f_destroy;
    transporter_ctrl f_ctrl;
};

/**
 * @brief Creates a transport array handle.
 *
 * The array is a static array with a default size of 3, used to create a
 * collection of transports for storing transporters of various protocols, such
 * as tcp transporter, tls transporter, websocket transporter.
 *
 * @return The transport array handle.
 */
tuya_transport_array_handle_t tuya_transport_array_create();

/**
 * @brief Destroys a transport array handle.
 *
 * This function destroys the transport array handle and releases any resources
 * associated with it.
 *
 * @param transport_handle The handle of the transport array to destroy.
 * @return OPERATE_RET Returns OPRT_OK if the transport array handle is
 * successfully destroyed, or an error code otherwise.
 */
OPERATE_RET
tuya_transport_array_destroy(tuya_transport_array_handle_t transport_handle);

/**
 * @brief Adds a transporter to the transport array.
 *
 * This function adds a transporter to the specified transport array handle.
 *
 * @param transport_handle The handle of the transport array.
 * @param transporter The transporter to be added.
 * @param scheme The scheme associated with the transporter.
 * @return The result of the operation.
 */
OPERATE_RET tuya_transport_array_add_transporter(tuya_transport_array_handle_t transport_handle,
                                                 tuya_transporter_t transporter, char *scheme);

/**
 * @brief Removes a transporter from the transport array.
 *
 * This function removes the specified transporter from the transport array
 * associated with the given transport handle.
 *
 * @param transport_handle The handle of the transport array.
 * @param transporter The transporter to be removed.
 * @return The result of the operation.
 *         - OPRT_OK: The transporter was successfully removed.
 *         - Other error codes: An error occurred while removing the
 * transporter.
 */
OPERATE_RET tuya_transport_array_remove_transporter(tuya_transport_array_handle_t transport_handle,
                                                    tuya_transporter_t transporter);

/**
 * @brief Retrieves a transporter from the transport array based on the
 * specified scheme.
 *
 * @param transport_handle The handle to the transport array.
 * @param scheme The scheme used to identify the transporter.
 * @return The transporter associated with the specified scheme, or NULL if not
 * found.
 */
tuya_transporter_t tuya_transport_array_get_transporter(tuya_transport_array_handle_t transport_handle, char *scheme);

/**
 * @brief Creates a Tuya transporter.
 *
 * This function creates a Tuya transporter based on the specified transport
 * type and dependency.
 *
 * @param transport_type The transport type of the Tuya transporter.
 * @param dependency The dependency of the Tuya transporter.
 * @return The created Tuya transporter.
 */
tuya_transporter_t tuya_transporter_create(TUYA_TRANSPORT_TYPE_E transport_type, tuya_transporter_t dependency);

/**
 * @brief Destroys a Tuya transporter.
 *
 * This function destroys a Tuya transporter and releases any resources
 * associated with it.
 *
 * @param transporter The Tuya transporter to destroy.
 * @return The result of the operation.
 *         - OPRT_OK: The transporter was successfully destroyed.
 *         - Other error codes may be returned in case of failure.
 */
OPERATE_RET tuya_transporter_destroy(tuya_transporter_t transporter);

/**
 * @brief Connects the transporter to the specified host and port.
 *
 * This function establishes a connection between the transporter and the
 * specified host and port.
 *
 * @param transporter The transporter to connect.
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param timeout_ms The timeout value in milliseconds for the connection
 * attempt.
 * @return The result of the connection attempt.
 *         - OPRT_OK if the connection was successful.
 *         - Other error codes if the connection failed.
 */
OPERATE_RET tuya_transporter_connect(tuya_transporter_t transporter, const char *host, int port, int timeout_ms);

/**
 * @brief Reads data from the specified transporter.
 *
 * This function reads data from the given transporter and stores it in the
 * provided buffer.
 *
 * @param transporter The transporter to read data from.
 * @param buf The buffer to store the read data.
 * @param len The maximum number of bytes to read.
 * @param timeout_ms The timeout value in milliseconds for the read operation.
 * @return The number of bytes read on success, or a negative error code on
 * failure.
 */
OPERATE_RET tuya_transporter_read(tuya_transporter_t transporter, uint8_t *buf, int len, int timeout_ms);

/**
 * @brief Writes data to the specified transporter.
 *
 * This function writes the provided data buffer to the specified transporter.
 *
 * @param transporter The transporter to write data to.
 * @param buf The buffer containing the data to be written.
 * @param len The length of the data buffer.
 * @param timeout_ms The timeout value in milliseconds for the write operation.
 * @return The result of the write operation.
 *         Possible return values:
 *         - OPRT_OK: Write operation successful.
 *         - OPRT_INVALID_PARM: Invalid parameter(s) provided.
 *         - OPRT_TIMEOUT: Write operation timed out.
 *         - OPRT_FAILED: Write operation failed.
 */
OPERATE_RET tuya_transporter_write(tuya_transporter_t transporter, uint8_t *buf, int len, int timeout_ms);

/**
 * @brief Reads data from the transporter using polling mechanism.
 *
 * This function reads data from the specified transporter using a polling
 * mechanism. It waits for a specified timeout period (in milliseconds) for data
 * to be available for reading. If data is available within the timeout period,
 * it is read and returned.
 *
 * @param transporter The transporter to read data from.
 * @param timeout_ms The timeout period (in milliseconds) to wait for data to be
 * available.
 * @return The result of the operation. Possible values are:
 *         - OPRT_OK: The operation was successful.
 *         - Other error codes: An error occurred during the operation.
 */
OPERATE_RET tuya_transporter_poll_read(tuya_transporter_t transporter, int timeout_ms);

/**
 * @brief Closes the specified transporter.
 *
 * This function is used to close the specified transporter.
 *
 * @param transporter The transporter to be closed.
 * @return The result of the operation.
 *         - OPRT_OK: The transporter was closed successfully.
 *         - Other error codes may be returned to indicate specific errors.
 */
OPERATE_RET tuya_transporter_close(tuya_transporter_t transporter);

/**
 * @brief Controls the Tuya transporter.
 *
 * This function is used to control the Tuya transporter by sending a command
 * and optional arguments.
 *
 * @param transporter The Tuya transporter to control.
 * @param cmd The command to send to the transporter.
 * @param args Optional arguments to pass along with the command.
 * @return The result of the operation.
 */
OPERATE_RET tuya_transporter_ctrl(tuya_transporter_t transporter, uint32_t cmd, void *args);

/**
 * @brief Sets the function pointers for the Tuya transporter.
 *
 * This function sets the function pointers for connecting, closing, reading,
 * writing, polling read, polling write, destroying, and controlling the Tuya
 * transporter.
 *
 * @param transporter The Tuya transporter to set the function pointers for.
 * @param connect The function pointer for connecting the transporter.
 * @param close The function pointer for closing the transporter.
 * @param read The function pointer for reading from the transporter.
 * @param write The function pointer for writing to the transporter.
 * @param poll_read The function pointer for polling read from the transporter.
 * @param poll_write The function pointer for polling write to the transporter.
 * @param destroy The function pointer for destroying the transporter.
 * @param ctrl The function pointer for controlling the transporter.
 * @return The operation result. Possible values are:
 *         - OPRT_OK: Operation succeeded.
 *         - Other error codes: Operation failed.
 */
OPERATE_RET tuya_transporter_set_func(tuya_transporter_t transporter, transporter_connect_fn connect,
                                      transporter_close_fn close, transporter_read_fn read, transporter_write_fn write,
                                      transporter_poll_read_fn poll_read, transporter_poll_read_fn poll_write,
                                      transporter_destroy_fn destroy, transporter_ctrl ctrl);

/**
 * @brief Set the transporter interface
 *
 * @param transport_handle: handle to the transport array
 * @param connect: connection function
 * 
 * @return OPERATE_RET: 0: Success; <0: Please refer to the Tuya error code documentation for description
 */
OPERATE_RET tuya_transporter_set_func(tuya_transporter_t transporter, transporter_connect_fn connect,
                                      transporter_close_fn close, transporter_read_fn read, transporter_write_fn write,
                                      transporter_poll_read_fn poll_read, transporter_poll_read_fn poll_write,
                                      transporter_destroy_fn destroy, transporter_ctrl ctrl);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
