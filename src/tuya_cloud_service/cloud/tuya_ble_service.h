#ifndef __TUYA_BLE_SERVICE_H__
#define __TUYA_BLE_SERVICE_H__

#include <stdint.h>
#include "tuya_cloud_types.h"
#include "tuya_iot.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/
typedef struct {
    uint8_t *pid;
    uint8_t *uuid;
    uint8_t *auth_key;
}tuya_ble_service_init_params_t;

typedef struct {
    uint16_t    cmd;
    uint16_t    in_len;
    uint8_t    *in_data;
    uint8_t    *out_data;
    uint16_t   *out_len;
}tuya_ble_user_data_t;

/***********************************************************
***********************typedef define***********************
***********************************************************/
#define BLE_USER_SESSION_MAX       2

/***********************************************************
********************function declaration********************
***********************************************************/

// typedef void (*ble_token_get_callback)(wifi_info_t wifi_info, tuya_binding_info_t binding_info);


typedef void (*ble_user_session_t)(tuya_ble_user_data_t *user_data);


// int tuya_ble_service_start(tuya_ble_service_init_params_t *init_params, ble_token_get_callback cb);
int tuya_ble_service_start(tuya_ble_service_init_params_t *init_params);

void tuya_ble_service_stop(void);

void tuya_ble_service_loop(void *argv);
// int ble_service_loop(void);
int tuya_ble_service_is_stop(void);

int tuya_ble_service_session_register  (ble_user_session_t session);
int tuya_ble_service_session_unregister(ble_user_session_t session);


#ifdef __cplusplus
}
#endif

#endif /* __TUYA_BLE_SERVICE_H__ */

