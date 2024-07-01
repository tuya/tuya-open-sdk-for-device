/**
 * @file atop_base.h
 * @brief Header file for ATOP base functions.
 *
 * This file defines the structures and interfaces for the ATOP  base functions.
 * It includes the definition of request and response structures used in
 * communication between devices and the Tuya cloud platform. The ATOP base
 * functions facilitate the encoding and decoding of URL parameters, request
 * data, and response data, as well as the parsing of response results.
 *
 * The structures and functions defined in this file are essential for
 * implementing the communication protocol that allows devices to interact with
 * the Tuya cloud platform securely and efficiently.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __ATOP_BASE_H_
#define __ATOP_BASE_H_

#include "tuya_cloud_types.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *path;
    const char *key;
    const char *header;
    const char *api;
    const char *version;
    const char *uuid;
    const char *devid;
    uint32_t timestamp;
    void *data;
    size_t datalen;
    const void *user_data;
} atop_base_request_t;

typedef struct {
    bool success;
    cJSON *result;
    int32_t t;
    void *user_data;
    uint8_t *raw_data;
    size_t raw_data_len;
} atop_base_response_t;

/**
 * @brief Sends a request to the atop base service.
 *
 * This function sends a request to the atop base service using the provided
 * request data. The response data will be stored in the provided response
 * structure.
 *
 * @param request Pointer to the `atop_base_request_t` structure containing the
 * request data.
 * @param response Pointer to the `atop_base_response_t` structure to store the
 * response data.
 * @return Returns an integer value indicating the status of the request. A
 * value of 0 indicates success, while a non-zero value indicates an error
 * occurred.
 */
int atop_base_request(const atop_base_request_t *request, atop_base_response_t *response);

/**
 * @brief Frees the memory allocated for an atop_base_response_t object.
 *
 * This function frees the memory allocated for the given atop_base_response_t
 * object.
 *
 * @param response Pointer to the atop_base_response_t object to be freed.
 */
void atop_base_response_free(atop_base_response_t *response);

#ifdef __cplusplus
}
#endif
#endif
