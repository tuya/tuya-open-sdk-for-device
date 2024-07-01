/**
 * @file iotdns.h
 * @brief Header file for IoT DNS resolution functions.
 *
 * This file declares the functions for resolving IoT DNS queries and retrieving
 * SSL/TLS certificates for secure communication with Tuya cloud services. It
 * includes functions for querying domain and host certificates necessary for
 * establishing secure HTTPS and MQTT connections.
 *
 * The DNS resolution and certificate retrieval are essential steps in ensuring
 * secure and reliable communication between IoT devices and the Tuya cloud
 * platform, facilitating device activation, data reporting, and command
 * reception.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __IOTDNS_H_
#define __IOTDNS_H_

#include "tuya_cloud_types.h"
#include "tuya_endpoint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Queries the domain certificates for a given URL.
 *
 * This function queries the domain certificates for a given URL and returns the
 * certificate and its length.
 *
 * @param url The URL for which to query the domain certificates.
 * @param cacert A pointer to a pointer that will store the domain certificate.
 * @param cacert_len A pointer to a variable that will store the length of the
 * domain certificate.
 *
 * @return Returns OPRT_MALLOC_FAILED if memory allocation fails, otherwise
 * returns the result of the tuya_iotdns_query_host_certs function.
 */

int tuya_iotdns_query_domain_certs(char *url, uint8_t **cacert, uint16_t *cacert_len);
/**
 * @brief Queries the host certificates using the IoT DNS service.
 *
 * This function queries the host certificates for a given host and port using
 * the IoT DNS service.
 *
 * @param[in] host The host name or IP address.
 * @param[in] port The port number.
 * @param[out] cacert A pointer to the buffer that will hold the retrieved CA
 * certificate.
 * @param[out] cacert_len A pointer to the variable that will hold the length of
 * the retrieved CA certificate.
 *
 * @return OPRT_OK if the operation is successful, otherwise an error code.
 */
int tuya_iotdns_query_host_certs(char *host, uint16_t port, uint8_t **cacert, uint16_t *cacert_len);

#ifdef __cplusplus
}
#endif
#endif
