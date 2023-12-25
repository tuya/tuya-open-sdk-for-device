/**
 * \file ble_trsmitr.h
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
#ifndef __BLE_TRSMITR_H__
#define __BLE_TRSMITR_H__

#include "tuya_cloud_types.h"
#include "tuya_error_code.h"
#include "ble_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  __BLE_TRSMITR_GLOBALS
#define __BLE_TRSMITR_EXT
#else
#define __BLE_TRSMITR_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define BLE_FRAME_VERSION_OFFSET          (0x0f << 4)
#define BLE_FRAME_SEQ_OFFSET              (0x0f << 0)
#define BLE_FRAME_SEQ_LMT 16

// frame total len
typedef uint32_t ble_frame_total_t;
// frame subpackage num
typedef uint32_t ble_frame_subpkg_num_t;
// frame sequence
typedef uint8_t ble_frame_seq_t;
// frame subpackage len
typedef uint32_t ble_frame_subpkg_len_t;

// frame package description
typedef uint8_t ble_frame_pkg_desc_t;
#define BLE_FRAME_PKG_INIT                0   // frame package init
#define BLE_FRAME_PKG_FIRST               1   // frame package first
#define BLE_FRAME_PKG_MIDDLE              2   // frame package middle
#define BLE_FRAME_PKG_END                 3   // frame package end

// frame transmitter process
typedef struct {
    ble_frame_total_t       total;              //4字节，数据总长度，不包括包头
    uint8_t                 version;            //1字节,协议主版本号
    ble_frame_seq_t         seq;                //1字节，
    ble_frame_pkg_desc_t    pkg_desc;           //1字节 当前分包帧类型（init/first/middle/end）
    ble_frame_subpkg_num_t  subpkg_num;         //4字节，当前分包编号
    uint32_t                pkg_trsmitr_cnt;    //  package process count  已发送出去的字节数
    ble_frame_subpkg_len_t  subpkg_len;         //1字节，当前分包中的数据长度（）
    uint8_t                *subpkg;
} ble_frame_trsmitr_t;

/***********************************************************
*************************function define********************
***********************************************************/

/***********************************************************
*  Function: ble_frame_trsmitr_create
*  description: create a transmitter and initialize
*  Input: none
*  Output:
*  Return: transmitter handle
***********************************************************/
__BLE_TRSMITR_EXT \
ble_frame_trsmitr_t *ble_frame_trsmitr_create(void);

/***********************************************************
*  Function: ble_frame_trsmitr_delete
*  description: delete transmitter
*  Input: transmitter handle
*  Output:
*  Return:
***********************************************************/
__BLE_TRSMITR_EXT \
void ble_frame_trsmitr_delete(ble_frame_trsmitr_t *frm_trsmitr);

/***********************************************************
*  Function: ble_frame_subpacket_len_get
*  description:
*  Input: transmitter handle
*  Output:
*  Return: ble_frame_subpkg_len_t
***********************************************************/
__BLE_TRSMITR_EXT \
ble_frame_subpkg_len_t ble_frame_subpacket_len_get(ble_frame_trsmitr_t *trsmitr);

//设置蓝牙单包最大长度
uint16_t ble_frame_packet_len_get(void);

//获取蓝牙单包最大长度
void ble_frame_packet_len_set(uint16_t len);

/***********************************************************
*  Function: ble_frame_subpacket_get
*  description:
*  Input: transmitter handle
*  Output:
*  Return: subpackage buf
***********************************************************/
__BLE_TRSMITR_EXT \
uint8_t *ble_frame_subpacket_get(ble_frame_trsmitr_t *frm_trsmitr);

/***********************************************************
*  Function: ble_frame_trsmitr_send_pkg_encode
*  description: frm_trsmitr->transmitter handle
*               type->frame type
*               buf->data buf
*               len->data len
*  Input:
*  Output:
*  Return: OPRT_OK->buf send up
*          OPRT_SVC_BT_API_TRSMITR_CONTINUE->need call again to be continue
*          other->error
*  Note: could get from encode data len and encode data by calling method
         get_trsmitr_subpkg_len() and ble_frame_subpacket_get()
***********************************************************/
__BLE_TRSMITR_EXT \
int ble_frame_trsmitr_send_pkg_encode(ble_frame_trsmitr_t *trsmitr, unsigned char version, unsigned char *buf, unsigned int len);


/***********************************************************
*  Function: ble_frame_trsmitr_recv_pkg_decode
*  description: frm_trsmitr->transmitter handle
*               raw_data->raw encode data
*               raw_data_len->raw encode data len
*  Input:
*  Output:
*  Return: OPRT_OK->buf receive up
*          OPRT_SVC_BT_API_TRSMITR_CONTINUE->need call again to be continue
*          other->error
*  Note: could get from decode data len and decode data by calling method
         get_trsmitr_subpkg_len() and ble_frame_subpacket_get()
***********************************************************/
__BLE_TRSMITR_EXT \
int ble_frame_trsmitr_recv_pkg_decode(ble_frame_trsmitr_t *trsmitr, unsigned char *raw_data, uint16_t raw_data_len);

#endif

#ifdef __cplusplus
}
#endif
