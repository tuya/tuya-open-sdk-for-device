#ifndef __MQTT_BIND_H_
#define __MQTT_BIND_H_

#include "tuya_iot.h"

#ifdef __cplusplus
extern "C" {
#endif

int mqtt_bind_token_get(const tuya_iot_config_t* config, tuya_binding_info_t* binding);

#ifdef __cplusplus
}
#endif
#endif
