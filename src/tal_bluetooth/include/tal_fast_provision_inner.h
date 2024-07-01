/**
 * @file tal_fast_provision_inner.h
 * @brief This header file defines the internal structures and functions for
 * Tuya's fast provisioning process for Bluetooth Mesh devices. It includes
 * definitions for the states of the provisioning process, structures for device
 * information, provisioning data, and error handling, as well as the API for
 * initiating and managing the fast provisioning process.
 *
 * The fast provisioning process is designed to streamline the addition of
 * devices to a Bluetooth Mesh network, reducing the time and complexity
 * involved in provisioning multiple devices. This file contains the necessary
 * definitions and functions to manage this process internally within the Tuya
 * SDK, including initializing the provisioning process, handling incoming
 * provisioning data, and managing the state of the provisioning process.
 *
 * The structures and functions defined in this file are intended for internal
 * use within the Tuya SDK to facilitate the fast provisioning of Bluetooth Mesh
 * devices. They provide the mechanisms for handling the various stages of the
 * provisioning process, from device discovery to final confirmation and
 * completion.
 *
 * @note This file is part of the Tuya IoT Development Platform and is intended
 * for use in Tuya-based applications. It is subject to the platform's license
 * and copyright terms.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_FAST_PROVISION_INNER_H
#define __TAL_FAST_PROVISION_INNER_H

#include "tkl_bluetooth_mesh_def.h"
#include "tal_bluetooth_mesh_def.h"

#define FAST_PROVISION_TIMEOUT 60 * 1000 * 1000

enum {
    FAST_PROV_IDLE,
    FAST_PROV_INFO_GET,
    FAST_PROV_SET_ADDR,
    FAST_PROV_PROV_DATA_SET,
    FAST_PROV_CONFIRM,
    FAST_PROV_COMPLETE,
    FAST_PROV_TIME_OUT,
};

typedef struct {
    USHORT_T category;
    USHORT_T ele_addr;
} dev_info_get_t;

typedef struct {
    UCHAR_T mac[6];
    USHORT_T category;
} fast_prov_info_st;

typedef struct {
    TKL_NET_PROV_DATA_T pro_data;
    TKL_APP_KEY_DATA_T appkey_set;
} fast_prov_net_info_t;

typedef struct {
    USHORT_T ele_addr;
    UCHAR_T bind_kind;
    UCHAR_T mesh_pid[8];
    USHORT_T dev_ver;
    //  UCHAR_T extra_info;
} _PACKED_ fast_prov_mac_st;

typedef struct {
    UCHAR_T err_type;
    UCHAR_T err_rfu;
} fast_prov_mac_err_st;

typedef struct {
    UCHAR_T get_mac_en;
    uint32_t rcv_op;
    UCHAR_T cur_sts;
    UCHAR_T last_sts;
    UCHAR_T pending;
    USHORT_T delay;
    uint32_t start_tick;
    uint32_t revert_tick;
    TKL_NET_PROV_DATA_T net_info;
    TKL_APP_KEY_DATA_T app_key;
} _PACKED_ fast_prov_par_t;

void ty_mesh_fast_prov_init();
void ty_mesh_fast_prov_proc(void);
UCHAR_T tal_get_fast_prov_en_state(void);
void mesh_fast_prov_recv_callback(USHORT_T src_addr, USHORT_T dst_addr, uint32_t opcode, UCHAR_T *par, int par_len);

void tal_uuid_update(void);

#endif
