#ifndef __BLE_MGR_H__
#define __BLE_MGR_H__

#include "tuya_cloud_types.h"
#include "ble_protocol.h"
#include "ble_cryption.h"
#include "tuya_iot.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    BLE_SESSION_SYSTEM,
    BLE_SESSION_DP,
    BLE_SESSION_CHANNEL,
    BLE_SESSION_USER,
    BLE_SESSION_MAX
} ble_seesion_type_t;

#define TUYA_BLE_NAME_LEN 5

typedef struct {
    tuya_iot_client_t   *client;
    char                 device_name[TUYA_BLE_NAME_LEN + 1];//能设置下去最大为5个字符
} tuya_ble_cfg_t;

typedef struct {
    uint32_t        sn;
    uint16_t        type;
    uint16_t        len;
    uint8_t        *data;
    uint8_t         encrypt_mode;
} ble_packet_t;

typedef void (*ble_session_fn_t)(ble_packet_t *packet, void *priv_data);


int tuya_ble_init(tuya_ble_cfg_t *cfg);

bool tuya_ble_is_connected(void);

int tuya_ble_send(uint16_t type, uint32_t ack_sn, uint8_t *data, uint32_t len);

int tuya_ble_send_packet(ble_packet_t *packet);

void tuya_ble_enable_debug(bool enable);

void tuya_ble_raw_print(char *title, uint8_t width, uint8_t *buf, uint16_t size);


#ifdef __cplusplus
}
#endif

#endif

