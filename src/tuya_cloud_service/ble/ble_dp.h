/**
* @file ble_dp.h
* @brief Common process - Initialization
* @version 0.1
* @date 2020-11-09
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/
#ifndef __BLE_DP__
#define __BLE_DP__

#include "ble_mgr.h"

#ifdef __cplusplus
extern "C" {
#endif


int tuya_ble_dp_report(IN dp_rept_in_t *dpin);

void ble_session_dp_process(ble_packet_t *packet, void *priv_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
