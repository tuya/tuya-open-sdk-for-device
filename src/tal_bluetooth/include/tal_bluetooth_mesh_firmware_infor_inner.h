/**
 * @file tal_bluetooth_mesh_firmware_infor_inner.h
 * @brief This is tuya tal_adc file
 * @version 1.0
 * @date 2021-09-10
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_BLUETOOTH_MESH_FIRMWARE_INFOR_INNER_H__
#define __TAL_BLUETOOTH_MESH_FIRMWARE_INFOR_INNER_H__

typedef struct {
    UCHAR_T     is_key;                             /**< Mesh Node Address */
    UCHAR_T     pid_or_key[8];                             /**< Mesh Data, structure refer to @TAL_BLE_DATA_T */
    UINT_T      version;
    UINT_T      mesh_category;
    UCHAR_T     need_publish_addr;
}TAL_MESH_FIRMWARE_INFOR_T;

UCHAR_T tal_get_firmware_key_or_pid(UCHAR_T *pid_or_key);

UINT_T tal_get_firmware_version(VOID);

UINT_T tal_get_firmware_mesh_category(VOID);

UCHAR_T tal_get_firmware_if_need_publish_addr(VOID);


#endif
