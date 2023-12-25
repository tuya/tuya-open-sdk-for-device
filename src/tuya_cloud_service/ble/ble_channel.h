/**
 * \file ble_channel.h
 *
 * \brief
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk
 */

#ifndef __BLE_CHANNEL_H__
#define __BLE_CHANNEL_H__

#include "ble_mgr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BLE_CHANNLE_NETCFG = 1,
    BLE_CHANNEL_MAX,
} ble_channel_type_t;

typedef void (*ble_channel_fn_t)(void *data, void *user_data);


int ble_channel_add(ble_channel_type_t type, ble_channel_fn_t fn, void *priv_data);
int ble_channel_del(ble_channel_type_t type);
void ble_session_channel_process(ble_packet_t *req, void *priv_data);

#ifdef __cplusplus
}
#endif

#endif

