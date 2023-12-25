/**
 * @file tal_bluetooth_mesh_device.h
 * @brief This is tuya tal_adc file
 * @version 1.0
 * @date 2021-09-10
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_BLUETOOTH_MESH_DEVICE_H__
#define __TAL_BLUETOOTH_MESH_DEVICE_H__

#include "tal_bluetooth_mesh_def.h"

/**
 * @brief   Register the mesh access app data callback  .
 * @param   [in] access_data_cb: mesh access data receive callback
 * @return  SUCCESS
 *          ERROR
 * */ 
OPERATE_RET tal_mesh_msg_recv_cb_init(tal_mesh_msg_recv_cb access_data_cb);

/**
 * @brief   Register the ble gap & gatt data callback  .
 * @param   [in] ble_event: ble gap & gatt data receive callback
 * @return  SUCCESS
 *          ERROR
 * */ 
OPERATE_RET tal_mesh_ble_recv_cb_init(TAL_BLE_EVT_FUNC_CB ble_event);

/**
 * @brief   Function to send mesh data to unicast/group addr
 * @param   [in] src_addr  unicast addr of node,if use 0,it will use node primary element addr
 * @param   [in] dst_addr  destinat addr
 * @param   [in] opcode  mesh op code
 * @param   [in] data  mesh data
 * @param   [in] data_len  data point
 * @return  SUCCESS
 *          ERROR
 * */
OPERATE_RET tal_mesh_data_send(USHORT_T src_addr, USHORT_T dst_addr, UINT_T opcode, UCHAR_T *data, USHORT_T data_len);

/**
 * @brief   Function to subscribe a group address for a model of the element.
 * @param   [in] opcode  to indicate subscribe or delete
 * @param   [in] ele_index  the index of element
 * @param   [in] group_addr  group address
 * @return  tx result
 * */
OPERATE_RET tal_group_addr_sub_set(UINT_T opcode, USHORT_T ele_index, USHORT_T group_addr);

/**
 * @brief   Function to get the subscribe addr list of a model of the element.
 * @param   [in] ele_index  the index of element
 * @param   [in] group_addr  group address
 * @return  subscribe addr list
 * */
USHORT_T* tal_group_addr_sub_list_get(USHORT_T ele_idx, USHORT_T model_id);

/**
 * @brief   Function to register a elemennt.
 * @param   [in] element_index  index of element
 * @return  SUCCESS
 *          ERROR
 * */
OPERATE_RET tal_element_register(USHORT_T element_index);

/**
 * @brief   Function to register a model into the element.
 * @param   [in] element_index  index of element
 * @param   [in] model_id  model id such as: 0x1000 = generic on off model
 * @return  SUCCESS
 *          ERROR
 * */
OPERATE_RET tal_model_register(USHORT_T element_index, UINT_T model_id);

/**
 * @brief   Function to enable/disable the provision.
 *          If node is unprovision state,using disable param will close the GATT unprovision adv and mesh unprovision beacon,
 *          and mesh fast provision will be disabled.
 * @param   [in] enable  enable/disable
 * @return  SUCCESS
 *          ERROR
 * */
VOID tal_mesh_node_provision_enable(MESH_PROVISION_TYPE_T enable);

/**
 * @brief   Init the mesh node uuid.
 * @param   [in] uuid  mesh universally unique identifier
 * @return  NULL
 * */
VOID tal_mesh_uuid_set(UCHAR_T* uuid);

/**
 * @brief   Get the mesh node provision state.
 * @return  the provision state
 * */
UCHAR_T tal_get_if_prov_success(VOID);

/**
 * @brief   Get the mesh node primary element address.
 * @return  the primary element address
 * */
USHORT_T tal_primary_ele_addr_get(VOID);

/**
 * @brief   Set the mesh node primary element address.
 * @param   [in] addr  unicast address
 * @param   [in] flash_save_en  save in flash or only update in ram
 * @return  NULL
 * */
VOID tal_primary_ele_addr_set(USHORT_T addr, int flash_save_en);

/**
 * @brief   Set the mesh node network state.
 * @param   [in] net_state  network state
 * @return  SUCCESS
 *          ERROR
 * */
OPERATE_RET tal_mesh_network_state_set(MESH_NETWORK_STATE_SET_T net_state);

/**
 * @brief   The callback to recieve the network state.
 * @param   [in] state  network state
 * @return  SUCCESS
 *          ERROR
 * */
TUYA_WEAK_ATTRIBUTE VOID tal_mesh_state_callback(TAL_MESH_NET_STATE_T state);

/**
 * @brief   Set the firmware information which mesh provision use.
 * @param   [in] is_key  use product key or product id
 * @param   [in] product_id  product id
 * @param   [in] product_key  product key
 * @param   [in] version  firmware version
 * @param   [in] mesh_category  mesh category define by tuya
 * @param   [in] need_publish_addr  if the device need a publish addr distributed by tuya cloud
 * @return  NULL
 * */
VOID tal_firmware_infor_set(UCHAR_T is_key, UCHAR_T *product_id, UCHAR_T *product_key, UINT_T version, UINT_T mesh_category, UCHAR_T need_publish_addr);

/**
 * @brief   Enable/Disable mesh fast provision.
 * @param   [in] enable  0:disable 1:enable
 * @return  SUCCESS
 *          ERROR
 * */
VOID tal_mesh_fast_prov_enable(UCHAR_T enable);


#endif
