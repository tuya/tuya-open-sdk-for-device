/**
 * @file websocket_transporter.c
 * @brief Implementation of WebSocket transporter for Tuya devices.
 *
 * This file contains the implementation of the WebSocket transporter for Tuya
 * devices, providing functionalities for establishing WebSocket connections,
 * sending and receiving data over WebSocket, and handling WebSocket connection
 * errors. The WebSocket transporter is a crucial component for enabling
 * real-time communication between Tuya devices and the Tuya cloud platform or
 * other networked services.
 *
 * The implementation leverages the generic transporter interface defined in
 * tuya_transporter.h, extending it with WebSocket-specific functionalities. It
 * includes support for configuring WebSocket connection parameters, managing
 * WebSocket connection lifecycle, and ensuring data integrity and security
 * during WebSocket communication.
 *
 * @note The WebSocket transporter is conditionally compiled and included in the
 * build if ENABLE_WEBSOCKET is set to 1.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"
#include "tal_log.h"
#include "tuya_transporter.h"
#include "tuya_tls.h"
#include "tls_transporter.h"

#if defined(ENABLE_WEBSOCKET) && (ENABLE_WEBSOCKET == 1)

#include "websocket_client.h"
#include "websocket_transporter.h"
typedef struct websocket_transporter_inter_t {
    struct tuya_transporter_inter_t base;
    char *path;
    char *scheme;
    websocket_client_handle_t ws_client;
    uint32_t readable_len;
    uint8_t *read_buffer;
    tuya_tcp_config_t tcpConfig;
    MUTEX_HANDLE mutex;
} * tuya_websocket_transporter_t;

/**
 * @brief Connects to a WebSocket server.
 *
 * This function establishes a connection to a WebSocket server using the
 * specified host, port, and timeout.
 *
 * @param t The WebSocket transporter object.
 * @param host The host address of the WebSocket server.
 * @param port The port number of the WebSocket server.
 * @param timeout_ms The connection timeout in milliseconds.
 *
 * @return The result of the connection operation.
 *         - OPRT_OK: The connection was successful.
 *         - Other error codes: The connection failed.
 */
OPERATE_RET websocket_transporter_connect(tuya_transporter_t t, char *host, int port, int timeout_ms)
{
    OPERATE_RET rt = OPRT_OK;

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;
    websocket_client_cfg_t cfg = {0};
    cfg.host = host;
    cfg.port = port;
    cfg.event_cb = NULL;

    cfg.priv_data = (void *)wst;
    cfg.scheme = wst->scheme;

    memcpy(&cfg.tcpConfig, &wst->tcpConfig, sizeof(wst->tcpConfig));

    TUYA_CALL_ERR_RETURN(websocket_client_init(&wst->ws_client, &cfg));
    TUYA_CALL_ERR_RETURN(websocket_client_set_path(wst->ws_client, wst->path));
    if (OPRT_OK != websocket_client_open(wst->ws_client, timeout_ms)) {
        websocket_client_close(wst->ws_client);
    }

    return OPRT_OK;
}

/**
 * @brief Closes the WebSocket transporter.
 *
 * This function is used to close the WebSocket transporter associated with the
 * given `tuya_transporter_t` object.
 *
 * @param t The WebSocket transporter to be closed.
 * @return The result of the operation. Possible values are:
 *         - OPRT_OK: The WebSocket transporter was closed successfully.
 *         - Other error codes indicating the failure of the operation.
 */
OPERATE_RET websocket_transporter_close(tuya_transporter_t t)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    if (wst->ws_client) {
        websocket_client_close(wst->ws_client);
        Free(wst->ws_client);
        wst->ws_client = NULL;
    }
    return OPRT_OK;
}

/**
 * @brief Reads data from the WebSocket transporter.
 *
 * This function reads data from the WebSocket transporter specified by the
 * parameter `t`. The read data is stored in the buffer `buf` with a maximum
 * length of `len`. The function will block for a maximum of `timeout_ms`
 * milliseconds until data is available to read.
 *
 * @param t The WebSocket transporter to read data from.
 * @param buf The buffer to store the read data.
 * @param len The maximum length of the buffer.
 * @param timeout_ms The maximum time to wait for data in milliseconds.
 * @return The number of bytes read on success, or a negative error code on
 * failure.
 */
OPERATE_RET websocket_transporter_read(tuya_transporter_t t, uint8_t *buf, int len, int timeout_ms)
{
    int ret = 0;
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    ret = websocket_client_poll(wst->ws_client, timeout_ms);
    if (ret <= 0) {
        return OPRT_TIMEOUT;
    }

    tal_mutex_lock(wst->mutex);
    int read_len = websocket_client_read(wst->ws_client, buf, len);
    tal_mutex_unlock(wst->mutex);
    PR_TRACE("websocket read len:%d, return len:%d", len, read_len);
    return read_len;
}

/**
 * @brief Writes data to the WebSocket transporter.
 *
 * This function is used to write data to the WebSocket transporter.
 *
 * @param t The WebSocket transporter.
 * @param buf The buffer containing the data to be written.
 * @param len The length of the data to be written.
 * @param timeout_ms The timeout value in milliseconds.
 * @return The result of the operation.
 */
OPERATE_RET websocket_transporter_write(tuya_transporter_t t, uint8_t *buf, int len, int timeout_ms)
{

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    return websocket_client_send_bin(wst->ws_client, buf, len);
}

/**
 * @brief Polls the WebSocket transporter for incoming data to read.
 *
 * This function is used to check if there is any incoming data available to
 * read from the WebSocket transporter.
 *
 * @param t The WebSocket transporter to poll.
 * @param timeout_ms The timeout value in milliseconds for the polling
 * operation. A value of -1 indicates an infinite timeout.
 *
 * @return The result of the operation. Possible return values are:
 *         - OPRT_OK: The operation was successful.
 *         - OPRT_TIMEOUT: The polling operation timed out.
 *         - Other error codes indicating a failure.
 */
OPERATE_RET websocket_transporter_poll_read(tuya_transporter_t t, int timeout_ms)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    return websocket_client_poll(wst->ws_client, timeout_ms);
    ;
}

/**
 * @brief Controls the WebSocket transporter.
 *
 * This function is used to control the WebSocket transporter by sending
 * different commands.
 *
 * @param t The WebSocket transporter to control.
 * @param cmd The command to send.
 * @param args Additional arguments for the command.
 *
 * @return The result of the operation.
 */
OPERATE_RET websocket_transporter_ctrl(tuya_transporter_t t, uint32_t cmd, void *args)
{
    OPERATE_RET ret = OPRT_OK;
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    switch (cmd) {
    case TUYA_TRANSPORTER_SET_TLS_CERT: {
        websocket_client_t *t_client = (websocket_client_t *)wst->ws_client;
        tuya_tls_transporter_ctrl((tuya_transporter_t)(t_client->transporter), TUYA_TRANSPORTER_SET_TLS_CERT, args);
        break;
    }
    case TUYA_TRANSPORTER_SET_WEBSOCKET_CONFIG: {
        tuya_websocket_config_t *p_websocket_config = (tuya_websocket_config_t *)args;

        if (wst->path != NULL) {
            Free(wst->path);
            wst->path = NULL;
        }
        wst->path = mm_strdup(p_websocket_config->path);
        if (wst->path == NULL) {
            PR_ERR("malloc failed");
            return OPRT_MALLOC_FAILED;
        }

        if (wst->scheme != NULL) {
            Free(wst->scheme);
            wst->scheme = NULL;
        }
        wst->scheme = mm_strdup(p_websocket_config->scheme);
        if (wst->scheme == NULL) {
            PR_ERR("malloc failed");
            return OPRT_MALLOC_FAILED;
        }
        break;
    }
    case TUYA_TRANSPORTER_SET_TCP_CONFIG: {
        tuya_tcp_config_t *pConfig = (tuya_tcp_config_t *)args;
        if (pConfig) {
            memcpy(&wst->tcpConfig, pConfig, sizeof(tuya_tcp_config_t));
        } else {
            ret = OPRT_INVALID_PARM;
        }
        break;
    }
    default: {
        break;
    }
    }

    return ret;
}

/**
 * Creates a WebSocket transporter for Tuya Cloud service.
 *
 * @return The created WebSocket transporter.
 */
tuya_transporter_t tuya_websocket_transporter_create()
{
    tuya_websocket_transporter_t t = Malloc(sizeof(struct websocket_transporter_inter_t));
    if (t == NULL) {
        PR_ERR("websocket transporter malloc fail");
        return NULL;
    }
    memset(t, 0, sizeof(struct websocket_transporter_inter_t));

    tuya_transporter_set_func((tuya_transporter_t)t, (transporter_connect_fn)websocket_transporter_connect,
                              websocket_transporter_close, websocket_transporter_read, websocket_transporter_write,
                              websocket_transporter_poll_read, NULL, tuya_websocket_transporter_destroy,
                              websocket_transporter_ctrl);

    tal_mutex_create_init(&t->mutex);

    return &t->base;
}

/**
 * @brief Destroys a WebSocket transporter.
 *
 * This function is used to destroy a WebSocket transporter and free up any
 * allocated resources.
 *
 * @param transporter The WebSocket transporter to destroy.
 * @return OPERATE_RET Returns OPRT_OK on success, or an error code on failure.
 */
OPERATE_RET tuya_websocket_transporter_destroy(tuya_transporter_t transporter)
{
    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)transporter;

    if (wst->ws_client) {
        websocket_client_close(wst->ws_client);
        Free(wst->ws_client);
        wst->ws_client = NULL;
    }
    if (wst->mutex) {
        tal_mutex_release(wst->mutex);
        wst->mutex = NULL;
    }
    if (wst->path) {
        Free(wst->path);
        wst->path = NULL;
    }
    if (wst->scheme) {
        Free(wst->scheme);
        wst->scheme = NULL;
    }
    if (transporter) {
        Free(transporter);
    }
    return OPRT_OK;
}

/**
 * @brief Sets the path for the WebSocket transporter.
 *
 * This function is used to set the path for the WebSocket transporter.
 *
 * @param t The WebSocket transporter.
 * @param path The path to set.
 * @return The result of the operation.
 */
OPERATE_RET tuya_websocket_set_path(tuya_transporter_t t, const char *path)
{

    tuya_websocket_transporter_t wst = (tuya_websocket_transporter_t)t;

    wst->path = mm_strdup(path);
    if (wst->path == NULL) {
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

#endif
