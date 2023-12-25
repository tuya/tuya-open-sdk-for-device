/**
 * \file ble_protocol.h
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


#ifndef __BLE_PROTOCOL_H__
#define __BLE_PROTOCOL_H__

//蓝牙指令定义
#define FRM_QRY_DEV_INFO_REQ                0x0000  //APP->BLE
#define FRM_PAIR_REQ                        0x0001  //APP->BLE
#define FRM_STATE_QUERY                     0x0003  //APP->BLE
#define FRM_UNBONDING_REQ                   0x0005  //APP->BLE
#define FRM_DEVICE_RESET                    0x0006  //APP->BLE
#define FRM_STAT_REPORT                     0x8001  //BLE->APP
#define FRM_DOWNLINK_TRANSPARENT_REQ        0x801B  //APP->APP
#define FRM_UPLINK_TRANSPARENT_REQ          0x801C  //BLE->APP
#define FRM_DOWNLINK_TRANSPARENT_SPEC_REQ   0x801E  //APP->APP,TUYAOS专用大数据传输通道
#define FRM_UPLINK_TRANSPARENT_SPEC_REQ     0x801F  //BLE->APP,TUYAOS专用大数据传输通道
#define FRM_RPT_NET_STAT_REQ                0x001E //BLE->APP

//4.0版本以上协议
//DP下发
#define FRM_DP_CMD_SEND_V4                  0x0027 // APP<->BLE
//DP上报
#define FRM_DP_STAT_REPORT_V4               0x8006  //BLE->APP
//带时间戳DP上报
#define FRM_DP_STAT_REPORT_WITH_TIME_V4     0x8007  //BLE->APP

//大数据通道传输子命令定义（0x801B、0x801C、0x801E、0x801F指令子命令统一规划，不允许冲突）
//蓝牙定时
#define FRM_DATA_TRANS_SUBCMD_TIMER_TASK    0x0000  //APP<->BLE
//psk3.0 蓝牙配网
#define FRM_DATA_TRANS_SUBCMD_BT_NETCFG     0x0001  //APP<->BLE
//psk3.0 连云激活
#define FRM_DATA_TRANS_SUBCMD_SET_WIFI      0x0002  //APP<->BLE
//查询WIFI列表
#define FRM_DATA_TRANS_SUBCMD_WIFI_LST      0x0003  //APP<->BLE
//查询配网状态
#define FRM_DATA_TRANS_SUBCMD_NETCFG_STAT   0x0004  //APP<->BLE
//查询设备日志
#define FRM_DATA_TRANS_SUBCMD_DEV_LOG       0x0005  //APP<->BLE
//查询AP热点名称
#define FRM_DATA_TRANS_SUBCMD_GET_AP_NAME   0x0006  //APP<->BLE

/** @defgroup TUY_BLE_DEVICE_COMMUNICATION_ABILITY tuya ble device communication ability
 * @{
 */
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE                0x0000
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_REGISTER_FROM_BLE  0x0001
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_MESH               0x0002
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_24G           0x0004
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_5G            0x0008
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_ZIGBEE             0x0010
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_NB                 0x0020
#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_CELLUAR            0x0040
/** End of TUY_BLE_DEVICE_COMMUNICATION_ABILITY
  * @}
  */


#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY  (TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_24G | \
                                                TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_5G)

/*Flag: 1Byte
bit7: 1:绑定，0:没绑定
bit6: 1:V2加密协议已启用，0：未启用
bit5: 1:支持V2加密协议，0：不支持
bit4: 1:蓝牙优先，0：wifi优先
bit3: 双模是否支持plug&play, 1:支持，0：不支持
bit1: 共享类标志, 1:涂鸦共享类设备，0：非共享类设备
*/
#define ADV_FLAG_BOND                               (1 << 7)
#define ADV_FLAG_ENCRPT_V2_START                    (1 << 6)
#define ADV_FLAG_ENCRPT_V2                          (1 << 5)
#define ADV_FLAG_PLUG_PLAY                          (1 << 3)
#define ADV_FLAG_BLE_FIRST                          (1 << 4)
#define ADV_FLAG_SHARE                              (1 << 1)
#define ADV_FLAG_UUID_COMP                          (1 << 0)//DEVICE UUID是否压缩


/** @defgroup TUYA_BLE_SECURE_CONNECTION_TYPE tuya ble secure connection type
 * @{
 */
#define TUYA_BLE_SECURE_CONNECTION_WITH_AUTH_KEY     0x00
#define TUYA_BLE_SECURE_CONNECTION_WITH_ECC          0x01
#define TUYA_BLE_SECURE_CONNECTION_WTIH_PASSTHROUGH  0x02
#define TUYA_BLE_SECURE_CONNECTION_WITH_AUTH_KEY_DEVICE_ID_20      0x03

/*
 * MACRO for advanced encryption,if 1 will use user rand check.
 */
#define TUYA_BLE_ADVANCED_ENCRYPTION_DEVICE       0

/*
数据来历
255:dp的最大长度是255
3: 每个分包的dp id相关信息,dp_id,dp_type,dp_len
14:帧格式的协议消耗，包括sn,ack_sn,功能码,数据长度,CRC

*/
#define TUYA_BLE_AIR_FRAME_MAX                    1024//305//256//255字节的dp下发，255+3+14+1+16+16=305

#define TUYA_BLE_TRANSMISSION_MAX_DATA_LEN        (TUYA_BLE_AIR_FRAME_MAX-29)
//透传指令大数据分包大小(长度设置为底层分包组包总长-包头长度)
#define TUYA_BLE_TRANS_DATA_SUBPACK_LEN           (TUYA_BLE_AIR_FRAME_MAX - 64)
//BLE 通讯协议版本 在这里设置
//APP 根据大版本号和小版本号解析消息，因此修改蓝牙版本为 3.7/4.4
#define TUYA_BLE_PROTOCOL_VERSION_HIGN            0x04
#define TUYA_BLE_PROTOCOL_VERSION_LOW             0x04

#define AUTH_KEY_LEN                              32
#define LOGIN_KEY_LEN_16                          16
#define SECRET_KEY_LEN                            16
#define PAIR_RANDOM_LEN                           6
#define TUYA_BLE_PRODUCT_ID_MAX_LEN               16
#define TUYA_BLE_WIFI_DEVICE_REGISTER_MODE        1

#endif

