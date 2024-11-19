# BLUETOOTH

# API Description

### tkl_ble_stack_init

```c
OPERATE_RET tkl_ble_stack_init(uint8_t role);
```

- Function Description:

  Initialize the BLE protocol stack
  
- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | role | BLE role configuration |

  | Name | Definition | Remarks |
  | :------------------ | :--------- | :--- |
  | TKL_BLE_ROLE_SERVER | BLE Server |      |
  | TKL_BLE_ROLE_CLIENT | BLE Client |      |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_stack_deinit

```c
OPERATE_RET tkl_ble_stack_deinit(uint8_t role);
```

- Function Description:

  Deinitialize the BLE protocol stack

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | role | BLE role, as shown below |

  | Name | Definition | Remarks |
  | :------------------ | :--------- | :--- |
  | TKL_BLE_ROLE_SERVER | BLE Server |      |
  | TKL_BLE_ROLE_CLIENT | BLE Client |      |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_callback_register

```c
OPERATE_RET tkl_ble_gap_callback_register(const TKL_BLE_GAP_EVT_FUNC_CB gap_evt);
```

- Function Description:

  Register the GAP callback function

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | gap_evt | Register the GAP callback function, as defined below |

  ```c
  typedef void(*TKL_BLE_GAP_EVT_FUNC_CB)(TKL_BLE_GAP_PARAMS_EVT_T *p_event);
  ```

  **TKL_BLE_GAP_PARAMS_EVT_T**

  ```c
  typedef struct {
        TKL_BLE_GAP_EVT_TYPE_E          type;           /**< Gap Event */
        uint16_t                        conn_handle;    /**< Connection Handle */
        int                           result;         /**< Will Refer to HOST STACK Error Code */
  
        union {
            TKL_BLE_GAP_CONNECT_EVT_T       connect;        /**< Receive connect callback, This value can be used with TKL_BLE_EVT_PERIPHERAL_CONNECT and TKL_BLE_EVT_CENTRAL_CONNECT_DISCOVERY*/
            TKL_BLE_GAP_DISCONNECT_EVT_T    disconnect;     /**< Receive disconnect callback*/
            TKL_BLE_GAP_ADV_REPORT_T        adv_report;     /**< Receive Adv and Respond report*/
            TKL_BLE_GAP_CONN_PARAMS_T       conn_param;     /**< We will update connect parameters.This value can be used with TKL_BLE_EVT_CONN_PARAM_REQ and TKL_BLE_EVT_CONN_PARAM_UPDATE*/
            char                          link_rssi;      /**< Peer device RSSI value */
        }gap_event;
  
    } TKL_BLE_GAP_PARAMS_EVT_T;
  ```

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatt_callback_register

```c
OPERATE_RET tkl_ble_gatt_callback_register(const TKL_BLE_GATT_EVT_FUNC_CB gatt_evt);
```

- Function Description:

  Register the GATT callback function

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | gatt_evt | GATT callback function |

  ```c
  typedef void(*TKL_BLE_GATT_EVT_FUNC_CB)(TKL_BLE_GATT_PARAMS_EVT_T *p_event);
  ```

  **TKL_BLE_GATT_PARAMS_EVT_T**

  GATT event definition as follows:

  ```c
  typedef struct {
      TKL_BLE_GATT_EVT_TYPE_E             type;           /**< Gatt Event */
      uint16_t                            conn_handle;    /**< Connection Handle */
      int                               result;         /**< Will Refer to HOST STACK Error Code */
  
      union {
          uint16_t                        exchange_mtu;   /**< This value can be used with TKL_BLE_GATT_EVT_MTU_REQUEST and TKL_BLE_GATT_EVT_MTU_RSP*/
          TKL_BLE_GATT_SVC_DISC_TYPE_T    svc_disc;       /**< Discovery All Service */
          TKL_BLE_GATT_CHAR_DISC_TYPE_T   char_disc;      /**< Discovery Specific Characteristic */
          TKL_BLE_GATT_DESC_DISC_TYPE_T   desc_disc;      /**< Discovery Specific Descriptors*/
          TKL_BLE_NOTIFY_RESULT_EVT_T     notify_result;  /**< This value can be used with TKL_BLE_GATT_EVT_NOTIFY_TX*/
          TKL_BLE_DATA_REPORT_T           write_report;   /**< This value can be used with TKL_BLE_GATT_EVT_WRITE_REQ*/
          TKL_BLE_DATA_REPORT_T           data_report;    /**< This value can be used with TKL_BLE_GATT_EVT_NOTIFY_INDICATE_RX*/
          TKL_BLE_DATA_REPORT_T           data_read;      /**< After we do read attr in central mode, we will get the callback*/
          TKL_BLE_SUBSCRBE_EVT_T          subscribe;      /**< ccc callback event, used with TKL_BLE_GATT_EVT_SUBSCRIBE*/
          TKL_BLE_READ_CHAR_EVT_T         char_read;      /**< read char event, used with TKL_BLE_GATT_EVT_READ_CHAR_VALUE*/
      } gatt_event;
  } TKL_BLE_GATT_PARAMS_EVT_T;
  ```

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_addr_set

```c
OPERATE_RET tkl_ble_gap_addr_set(TKL_BLE_GAP_ADDR_T const *p_peer_addr);
```

- Function Description:

  Set the BLE MAC address

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_peer_addr | MAC address, optional public/random |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

- **Remarks**

  Generally, the module has a MAC address after production and does not need to be configured.

### tkl_ble_gap_address_get

```c
OPERATE_RET tkl_ble_gap_address_get(TKL_BLE_GAP_ADDR_T *p_peer_addr);
```

- Function Description:

  Read the BLE device MAC address

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [out] | p_peer_addr | Device MAC address |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

- **Remarks**

  Rarely used

### tkl_ble_gap_adv_start

```c
OPERATE_RET tkl_ble_gap_adv_start(TKL_BLE_GAP_ADV_PARAMS_T const *p_adv_params);
```

- Function Description:

  Start BLE advertising

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_adv_params | Advertising parameters, as shown below |

  **TKL_BLE_GAP_ADV_PARAMS_T**

  ```c
  typedef struct {
      uint8_t                 adv_type;                   /**< Adv Type. Refer to TKL_BLE_GAP_ADV_TYPE_CONN_SCANNABLE_UNDIRECTED etc. */
      TKL_BLE_GAP_ADDR_T      direct_addr;                /**< For Directed Advertising, you can fill in direct address */
      
      uint16_t                adv_interval_min;           /**< Range: 0x0020 to 0x4000  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec */
      uint16_t                adv_interval_max;           /**< Range: 0x0020 to 0x4000  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec */
      uint8_t                 adv_channel_map;            /**< Advertising Channel Map, 0x01 = adv channel index 37,  0x02 = adv channel index 38,
                                                                  0x04 = adv channel index 39. Default Value: 0x07 */
  } TKL_BLE_GAP_ADV_PARAMS_T;
  ```

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_adv_stop

```c
OPERATE_RET tkl_ble_gap_adv_stop(void);
```

- Function Description:

  Stop BLE advertising

- Parameters:

  None

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_adv_rsp_data_set

```c
OPERATE_RET tkl_ble_gap_adv_rsp_data_set(TKL_BLE_DATA_T const *p_adv, TKL_BLE_DATA_T const *p_scan_rsp);
```

- Function Description:

  Set advertising data

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_adv | Advertising data |
  | [in] | p_scan_rsp | Scan response data |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_adv_rsp_data_update

```c
OPERATE_RET tkl_ble_gap_adv_rsp_data_update(TKL_BLE_DATA_T const *p_adv, TKL_BLE_DATA_T const *p_scan_rsp);
```

- Function Description:

  Update advertising data

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_adv | Advertising data |
  | [in] | p_scan_rsp | Scan response data |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_scan_start

```c
OPERATE_RET tkl_ble_gap_scan_start(TKL_BLE_GAP_SCAN_PARAMS_T const *p_scan_params);
```

- Function Description:

  Start scanning

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_scan_params | Scan parameters, as shown below |

  **TKL_BLE_GAP_SCAN_PARAMS_T**

  ```c
  typedef struct {
      uint8_t                 extended;                   /**< If 1, the scanner will accept extended advertising packets.
                                                              If set to 0, the scanner will not receive advertising packets
                                                              on secondary advertising channels, and will not be able
                                                              to receive long advertising PDUs. */
      uint8_t                 active : 1;                 /**< [Tuya Need]!!!! If 1, perform active scanning by sending scan requests.
                                                              This parameter is ignored when used with @ref tkl_ble_gap_connect. */
      uint8_t                 scan_phys;                  /**< Refer to @TKL_BLE_GAP_PHY_1MBPS. TKL_BLE_GAP_PHY_2MBPS */
      uint16_t                interval;                   /**< Scan interval in 625 us units. */
      uint16_t                window;                     /**< Scan window in 625 us units. */
      uint16_t                timeout;                    /**< Scan timeout in 10 ms units. */
      uint8_t                 scan_channel_map;           /**< Scan Channel Index, refer to @TKL_BLE_GAP_ADV_PARAMS_T */
  } TKL_BLE_GAP_SCAN_PARAMS_T;
  ```

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

- Remarks

  tkl_ble_gap_scan_start is used when enabling the controller or supporting the central mode.

### tkl_ble_gap_scan_stop

```c
OPERATE_RET tkl_ble_gap_scan_stop(void);
```

- Function Description:

  Stop Bluetooth scanning

- Parameters:

  None

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gap_connect

```c
OPERATE_RET tkl_ble_gap_connect(TKL_BLE_GAP_ADDR_T const *p_peer_addr, TKL_BLE_GAP_SCAN_PARAMS_T const *p_scan_params, TKL_BLE_GAP_CONN_PARAMS_T const *p_conn_params);
```

- Function Description:

  As a client, initiate a connection.

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | p_peer_addr | Peer address |
  | [in] | p_scan_params | Scan parameters |
  | [in] | p_conn_params | Connection parameters |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

- **Explanation**

  Only needed when adapting as a central.

### tkl_ble_gap_disconnect

```c
OPERATE_RET tkl_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_reason);
```

- Function Description:

  Disconnect BLE connection

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | p_conn_params | Disconnect reason, use 0x13 for active disconnection in normal cases. |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

- **Remarks**

  Used by both client and server.

### tkl_ble_gap_conn_param_update

```c
OPERATE_RET tkl_ble_gap_conn_param_update(uint16_t conn_handle, TKL_BLE_GAP_CONN_PARAMS_T const *p_conn_params);
```

- Function Description:

  Update GAP connection parameters

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | p_conn_params | Connection parameters |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_service_add

```c
OPERATE_RET tkl_ble_gatts_service_add(TKL_BLE_GATTS_PARAMS_T *p_service);
```

- Function Description:

  GATT add service, used by server.

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in/out] | p_service | GATT service |

  **TKL_BLE_GATTS_PARAMS_T**

  ```c
  typedef struct {
      uint8_t                     svc_num;                /**< If we only use service(0xFD50), the svc_num will be set into 1 */
      TKL_BLE_SERVICE_PARAMS_T    *p_service;
  } TKL_BLE_GATTS_PARAMS_T;
  ```

  **TKL_BLE_SERVICE_PARAMS_T**

  Handle is issued as 0xFF by default, and the corresponding handle value will be updated after the service is added.

  ```c
  typedef struct {
      uint16_t                    handle;                 /**< After init the service, we will get the svc-handle */
  
      TKL_BLE_UUID_T              svc_uuid;               /**< Service UUID */
      TKL_BLE_SERVICE_TYPE_E      type;                   /**< Service Type */
      
      uint8_t                     char_num;               /**< Number of characteristic */
      TKL_BLE_CHAR_PARAMS_T       *p_char;                /**< Pointer of characteristic */
  } TKL_BLE_SERVICE_PARAMS_T;
  ```

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_value_set

```c
OPERATE_RET tkl_ble_gatts_value_set(uint16_t conn_handle, uint16_t char_handle, uint8_t *p_data, uint16_t length);
```

- Function Description:

  GATT update attribute value, such as configuring read characteristic, can use this function to update

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | char_handle | Characteristic handle |
  | [in] | p_data | Data |
  | [in] | length | Data length |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_value_get

```c
OPERATE_RET tkl_ble_gatts_value_get(uint16_t conn_handle, uint16_t char_handle, uint8_t *p_data, uint16_t length);
```

- Function Description:

  GATT read attribute value, not used yet

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | char_handle | Characteristic handle |
  | [out] | p_data | Read data |
  | [in] | length | Read length |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_value_notify

```c
OPERATE_RET tkl_ble_gatts_value_notify(uint16_t conn_handle, uint16_t char_handle, uint8_t *p_data, uint16_t length);
```

- Function Description:

  Server sends notification data.

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | char_handle | Characteristic handle |
  | [in] | p_data | Notification data |
  | [in] | length | Notification data length |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_value_indicate

```c
OPERATE_RET tkl_ble_gatts_value_indicate(uint16_t conn_handle, uint16_t char_handle, uint8_t *p_data, uint16_t length);
```

- Function Description:

  Server sends indication data.

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | char_handle | Characteristic handle |
  | [in] | p_data | Indication data |
  | [in] | length | Indication data length |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure

### tkl_ble_gatts_exchange_mtu_reply

```c
OPERATE_RET tkl_ble_gatts_exchange_mtu_reply(uint16_t conn_handle, uint16_t server_rx_mtu);
```

- Function Description:

  Server configures MTU, i.e., replies to the client's MTU exchange request by sending an MTU exchange response to the client

- Parameters:

  | Input/Output | Parameter Name | Description |
  |-------------|----------------|-------------|
  | [in] | conn_handle | Connection handle |
  | [in] | server_rx_mtu | Server-side receive MTU size |

- Return Value:

  - OPRT_OK : Success
  - Others : Failure
