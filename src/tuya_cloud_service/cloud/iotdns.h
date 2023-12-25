#ifndef __IOTDNS_H_
#define __IOTDNS_H_

#include "tuya_cloud_types.h"
#include "tuya_endpoint.h"

#ifdef __cplusplus
extern "C" {
#endif

int tuya_iotdns_query_domain_certs(char *url, uint8_t **cacert, uint16_t *cacert_len);

#ifdef __cplusplus
}
#endif
#endif
