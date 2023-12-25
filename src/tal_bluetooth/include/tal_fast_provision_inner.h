/**
 * @file tal_fast_provision_inner.h
 * @brief This is tuya tal_adc file
 * @version 1.0
 * @date 2021-09-10
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_FAST_PROVISION_INNER_H
#define __TAL_FAST_PROVISION_INNER_H

#include "tkl_bluetooth_mesh_def.h"
#include "tal_bluetooth_mesh_def.h"

#define FAST_PROVISION_TIMEOUT      60*1000*1000

enum{
    FAST_PROV_IDLE,
    FAST_PROV_INFO_GET,
    FAST_PROV_SET_ADDR,
    FAST_PROV_PROV_DATA_SET,
    FAST_PROV_CONFIRM,
    FAST_PROV_COMPLETE,
    FAST_PROV_TIME_OUT,
};


typedef struct{
    USHORT_T category;
    USHORT_T ele_addr;
}dev_info_get_t;

typedef struct{
    UCHAR_T mac[6];
    USHORT_T category;   
}fast_prov_info_st;

typedef struct{
    TKL_NET_PROV_DATA_T pro_data;
    TKL_APP_KEY_DATA_T appkey_set;
}fast_prov_net_info_t;

typedef struct{
    USHORT_T ele_addr;
    UCHAR_T bind_kind;
    UCHAR_T mesh_pid[8];
    USHORT_T dev_ver;
//  UCHAR_T extra_info;
} _PACKED_ fast_prov_mac_st;

typedef struct{
    UCHAR_T err_type;
    UCHAR_T err_rfu;
}fast_prov_mac_err_st;

typedef struct{
    UCHAR_T get_mac_en;
    UINT_T rcv_op;
    UCHAR_T cur_sts;
    UCHAR_T last_sts;
    UCHAR_T pending;
    USHORT_T delay;
    UINT_T start_tick;
    UINT_T revert_tick;
    TKL_NET_PROV_DATA_T net_info;
    TKL_APP_KEY_DATA_T  app_key;
} _PACKED_ fast_prov_par_t;

VOID ty_mesh_fast_prov_init();
VOID ty_mesh_fast_prov_proc(VOID);
UCHAR_T tal_get_fast_prov_en_state(VOID);
void mesh_fast_prov_recv_callback(USHORT_T src_addr, USHORT_T dst_addr, UINT_T opcode, UCHAR_T *par, int par_len);

VOID tal_uuid_update(VOID);


#endif
