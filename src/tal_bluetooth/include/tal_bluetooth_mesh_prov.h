#ifndef __TAL_BLUETOOTH_MESH_H__
#define __TAL_BLUETOOTH_MESH_H__

#include "tal_bluetooth_def.h"
#include "tal_bluetooth_mesh_def.h"

/**
 * @brief   Function for initializing the mesh provisioner
 * @param   [in] role:          Configurate the mesh stack role
 *          [in] p_event:       Event handler provided by the user. will get the event from mesh stack
 * @return  SUCCESS             Initialized successfully.
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_init(TAL_BLE_ROLE_E role, CONST TAL_MESH_EVT_FUNC_CB mesh_event);

/**
 * @brief   Function for reset the mesh provisioner, or reset some configurations; eg: node info
 * @param   config_reset:       0: Only Reset Mesh Stack; 1: Reset Stack And reset mesh configurations
 * @return  SUCCESS             Successfully mesh reset procedure.
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_stack_reset(UCHAR_T config_reset);

/**
 * @brief   We need to set mesh info, include netkey and appkey
 * @param   [in] local_info:    set netkey/appkey and local node address.
 * @return  SUCCESS             Successfully mesh info set.
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_info_set(TAL_MESH_LOCAL_INFO_T local_info);

/**
 * @brief   [Mesh Provisioner] Control Scan unprovisioned-beacon while running in mesh provisioner.
 * @param   VOID
 * @return  SUCCESS             Successfully set scanning procedure.
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_scan_set(UCHAR_T enable, UINT_T timeout);

/**
 * @brief   [Mesh Provisioner] Start to invite one mesh device into network,(Only Provision Invite)
 * @param   [in] device:    assign device info while doing invite. 
 *                          need node_addr and peer_uuid while using PB-ADV
 *                          need node_addr and peer_addr while using PB-GATT
 * @return  SUCCESS
 *          ERROR
 * */ 
OPERATE_RET tal_ble_mesh_adv_provision_invite(TAL_MESH_DEVICE_INFO_T device);

/**
 * @brief   Add Device key into local database
 * @param   [in] device:    Node info, need node_devkey and node_addr
 * @return  SUCCESS         OPRT_OK
 *          ERROR           OPRT_NOT_SUPPORT: the mesh stack will store any mesh info 
 * */ 
OPERATE_RET tal_ble_mesh_node_devkey_add(TAL_MESH_DEVICE_INFO_T device);

/**
 * @brief   Delete Device key from local database
 * @param   [in] device:    Node info, need node_devkey and node_addr
 * @return  SUCCESS         OPRT_OK
 *          ERROR           OPRT_NOT_SUPPORT: the mesh stack will store any mesh info 
 * */ 
OPERATE_RET tal_ble_mesh_node_devkey_delete(TAL_MESH_DEVICE_INFO_T device);

/**
 *@brief    Get seq number from local node
 *@param    [out] seq       current sequence number
 *@param    [out] iv_index  current IV Index
 *@return   SUCCESS         OPRT_OK
 *          ERROR
 */
OPERATE_RET tal_ble_mesh_device_get_seq(UINT_T *seq, UINT_T *iv_index);

/**
 *@brief    Set seq number for local node
 *@param    [in] seq        sequence number
 *@return   SUCCESS         set sequence into local node
 *          ERROR
 */
OPERATE_RET tal_ble_mesh_device_set_seq(UINT_T seq);

/**
 *@brief    Update IV index for local node
 *@param    [in] iv_index   current IV Index
 *@param    [in] flags      IV Update Flag:
 *          0: Normal operation 1: IV Update active
 *@return   SUCCESS
 *          ERROR
 */
OPERATE_RET tal_ble_mesh_device_update_iv_info(UINT_T iv_index, UCHAR_T flags);

/**
 *@brief    Set ttl for local node
 *@param    [in] ttl        ttl number
 *@return   SUCCESS
 *          ERROR
 */
OPERATE_RET tal_ble_mesh_local_node_set_ttl(UCHAR_T ttl);

/**
 *@brief    set local provisioner network transmit parameters.
 *@param    [in] [in] count :       The number of transmissions is the Transmit Count + 1, 
 *                                  For example,  if a count value of 0 represents a single transmission.
 *                                  if a count value of 7 represents 8 transmissions.
 *@param    [in] interval_steps     transmission interval = (Network Retransmit Interval Steps + 1) * 10.
 *@return   SUCCESS
 *          ERROR
 */
OPERATE_RET tal_ble_mesh_local_network_transmit_set(UCHAR_T count, UCHAR_T interval_steps);

/**
 * @brief   [Mesh Provisioner] Send data to mesh node
 * @param   [in] device     config the device info, if we send generic data, we only need to set the node address.
                p_data      pointer to data, refer to TAL_MESH_DATA_T               
 * @return  SUCCESS         Successfully send model message
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_common_message_send(TAL_MESH_DEVICE_INFO_T device, TAL_MESH_DATA_T *p_data);

/**
 * @brief   [Mesh Provisioner] Send configuration data to mesh node
 * @param   [in] device     Device Info
 *               p_data     pointer to data, refer to TAL_MESH_DATA_T               
 * @return  SUCCESS         Successfully send configuration message
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_config_message_send(TAL_MESH_DEVICE_INFO_T device, TAL_MESH_DATA_T *p_data);

/**
 * @brief   [Beacon Central]    Send data to beacon devices, and will be sended in mesh-bear layer.
 * @param   [in] device:        Device Info, if you only post tuya-beacon data, the device info can be NULL
 *               p_data:        pointer to data, refer to TAL_MESH_DATA_T
 * @return  SUCCESS             Successfully send beacon data.
 *          ERROR
 * */
OPERATE_RET tal_ble_mesh_beacon_message_send(TAL_MESH_DEVICE_INFO_T device, TAL_MESH_DATA_T *p_data);
#endif


