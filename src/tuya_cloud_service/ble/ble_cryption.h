/**
 * \file ble_cryption.h
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

#ifndef __BLE_ENCRYPTION_H__
#define __BLE_ENCRYPTION_H__

#include "tuya_cloud_types.h"
#include "ble_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ENCRYPTION_MODE_NONE,
    ENCRYPTION_MODE_KEY_11 = 11, //MD5(auth key+device uuid+server rand)
    ENCRYPTION_MODE_KEY_12,//MD5（secret_key_11,srand）的结果16字节转32个字符，取第8-24个字符，只用于配对请求指令。
    ENCRYPTION_MODE_KEY_13,//rsvd
    ENCRYPTION_MODE_KEY_14,//MD5（16字节login_key + 16字节secret_key）的结果16字节转32个字符，取第8-24个字符
    ENCRYPTION_MODE_SESSION_KEY15,//MD5（16字节login_key,srand）的结果16字节转32个字符，取第8-24个字符
    ENCRYPTION_MODE_KEY_16,//MD5(auth key + md5(device uuid) + server rand)，用于解绑指令
    ENCRYPTION_MODE_MAX,
} ble_key_mode_t;

typedef struct {
    uint8_t *auth_key;
    uint8_t *user_rand;
    uint8_t *login_key;
    uint8_t *sec_key;
    uint8_t *uuid;
    uint8_t *pair_rand;
} ble_crypto_param_t;


uint8_t tuya_ble_encryption(ble_crypto_param_t *p, 
                            uint8_t             encryption_mode, 
                            uint8_t             *iv, 
                            uint8_t             *in_buf, 
                            uint32_t            in_len, 
                            uint32_t            *out_len, 
                            uint8_t             *out_buf);

uint8_t tuya_ble_decryption(ble_crypto_param_t  *p, 
                            uint8_t             *in_buf, 
                            uint32_t            in_len, 
                            uint32_t            *out_len, 
                            uint8_t             *out_buf);


bool tuya_ble_register_key_generate(uint8_t *output, uint8_t *auth_key);

int tuya_ble_adv_id_encrypt(uint8_t *key, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf);

int tuya_ble_rsp_id_encrypt(uint8_t *key, uint8_t key_len, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf);

void tuya_ble_id_compress(uint8_t *in, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif


