/**
 * @file tal_bluetooth_mesh_def.h
 * @brief This is tuya mesh file
 * @version 1.0
 * @date 2021-11-13
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_BLUETOOTH_MESH_DEF_H__
#define __TAL_BLUETOOTH_MESH_DEF_H__

#include "tuya_cloud_types.h"
#include "tuya_error_code.h"
#include "tal_bluetooth.h"

#ifndef PACKED
#define PACKED  __attribute__ ((packed))
#endif

/**< Tuya Needed Definitions And Macro */
/* Mesh Company ID = Tuya Company ID*/
#define COMPANY_ID                                      (0x07D0)

/* Mesh GATT Mesh Prov Services Scan UUID */
#define TAL_MESH_PROV_SVC_SCAN_UUID                     (0x1827)
/* Mesh GATT Mesh Proxy Services Scan UUID */
#define TAL_MESH_PROXY_SVC_SCAN_UUID                    (0x1828)

/* Mesh GATT Mesh Prov Services UUID */
#define TAL_MESH_PROV_DEVICE_SVC_UUID                   (0x1827)
/*Mesh GATT Mesh Prov Write Characteristic UUID*/
#define TAL_MESH_PROV_WRITE_CHAR_UUID                   (0x2adb)
/*Mesh GATT Mesh Prov Notify Characteristic UUID*/
#define TAL_MESH_PROV_NOTIFY_CHAR_UUID                  (0x2adc)

/* Mesh GATT Mesh Proxy Services UUID */
#define MESH_PROXY_DEVICE_SVC_UUID                      (0x1828)
/*Mesh GATT Mesh Proxy Write Characteristic UUID*/
#define MESH_PROXY_WRITE_CHAR_UUID                      (0x2add)
/*Mesh GATT Mesh Proxy Notify Characteristic UUID*/
#define MESH_PROXY_NOTIFY_CHAR_UUID                     (0x2ade)

/*Mesh Model ID [Tuya Use]*/
#define TAL_MODEL_ID_CONFIG_SERVER                      (0x0000)
#define TAL_MODEL_ID_CONFIG_CLIENT                      (0x0001)
#define TAL_MODEL_ID_HEALTH_SERVER                      (0x0002)
#define TAL_MODEL_ID_HEALTH_CLIENT                      (0x0003)
#define TAL_MODEL_ID_GENERIC_ONOFF_SERVER               (0x1000)
#define TAL_MODEL_ID_GENERIC_ONOFF_CLIENT               (0x1001)
#define TAL_MODEL_ID_LIGHT_LIGHTNESS_SERVER             (0x1300)
#define TAL_MODEL_ID_LIGHT_LIGHTNESS_CLIENT             (0x1302)
#define TAL_MODEL_ID_LIGHT_CTL_SERVER                   (0x1303)
#define TAL_MODEL_ID_LIGHT_CTL_CLIENT                   (0x1305)
#define TAL_MODEL_ID_LIGHT_CTL_TEMP_SERVER              (0x1306)
#define TAL_MODEL_ID_LIGHT_HSL_SERVER                   (0x1307)
#define TAL_MODEL_ID_LIGHT_HSL_CLIENT                   (0x1309)
#define TAL_MODEL_ID_VENDOR_SERVER                      (0x000407D0)
#define TAL_MODEL_ID_VENDOR_CLIENT                      (0x000507D0)

/* Onoff Model Opcode [Tuya Use] */
#define TAL_MESH_OPCODE_ON_OFF_GET                      (0x8201)
#define TAL_MESH_OPCODE_ON_OFF_SET                      (0x8202)
#define TAL_MESH_OPCODE_ON_OFF_SET_UNACK                (0x8203)
#define TAL_MESH_OPCODE_ON_OFF_STAT                     (0x8204)

/* Lightness Model Opcode [Tuya Use] */
#define TAL_MESH_OPCODE_LIGHTNESS_GET                   (0x824B)
#define TAL_MESH_OPCODE_LIGHTNESS_SET                   (0x824C)
#define TAL_MESH_OPCODE_LIGHTNESS_SET_UNACK             (0x824D)
#define TAL_MESH_OPCODE_LIGHTNESS_STAT                  (0x824E)

/* Light CTL Model Opcode [Tuya Use] */
#define TAL_MESH_OPCODE_LIGHT_CTL_GET                   (0x825D)
#define TAL_MESH_OPCODE_LIGHT_CTL_SET                   (0x825E)
#define TAL_MESH_OPCODE_LIGHT_CTL_SET_UNACK             (0x825F)
#define TAL_MESH_OPCODE_LIGHT_CTL_STAT                  (0x8260)

/* Temperature Model Opcode [Tuya Use] */
#define TAL_MESH_OPCODE_LIGHT_CTL_TEMP_GET              (0x8261)
#define TAL_MESH_OPCODE_LIGHT_CTL_TEMP_SET              (0x8264)
#define TAL_MESH_OPCODE_LIGHT_CTL_TEMP_SET_UNACK        (0x8265)
#define TAL_MESH_OPCODE_LIGHT_CTL_TEMP_STAT             (0x8266)

/* HSL Model Opcode [Tuya Use] */
#define TAL_MESH_OPCODE_LIGHT_HSL_GET                   (0x826D)
#define TAL_MESH_OPCODE_LIGHT_HSL_SET                   (0x8276)
#define TAL_MESH_OPCODE_LIGHT_HSL_SET_UNACK             (0x8277)
#define TAL_MESH_OPCODE_LIGHT_HSL_STAT                  (0x8278)

/* Tuya Mesh Opcode For Fast Provision */
#define TAL_MESH_OPCODE_FP_GET_MAC                      (0xC0D007)
#define TAL_MESH_OPCODE_FP_GET_MAC_STATUS               (0xC1D007)
#define TAL_MESH_OPCODE_FP_SET_ADDR                     (0xC2D007)
#define TAL_MESH_OPCODE_FP_SET_ADDR_STATUS              (0xC3D007)
#define TAL_MESH_OPCODE_FP_SET_PROVISON_DATA            (0xC4D007)
#define TAL_MESH_OPCODE_FP_CONFIRM                      (0xC5D007)
#define TAL_MESH_OPCODE_FP_CONFIRM_STATUS               (0xC6D007)
#define TAL_MESH_OPCODE_FP_COMMPLETE                    (0xC7D007)

/* Tuya Mesh Opcode For Vendor Model */
#define TAL_MESH_OPCODE_WRITE                           (0xC9D007)
#define TAL_MESH_OPCODE_WRITE_UNACK                     (0xCAD007)
#define TAL_MESH_OPCODE_STAT                            (0xCBD007)
#define TAL_MESH_OPCODE_READ                            (0xCCD007)
#define TAL_MESH_OPCODE_DATA                            (0xCDD007)
#define TAL_MESH_OPCODE_LOCAL_AUTO_WRITE                (0xD0D007)
#define TAL_MESH_OPCODE_LOCAL_AUTO_NOTIFY               (0xD1D007)

/* Tuya Mesh Opcode For Configuration Client */
#define TAL_MESH_OPCODE_CFG_APP_KEY_ADD                 (0x00)
#define TAL_MESH_OPCODE_CFG_APP_KEY_UPDATE              (0x01)
#define TAL_MESH_OPCODE_CFG_APP_KEY_DELETE              (0x8000)
#define TAL_MESH_OPCODE_CFG_APP_KEY_GET                 (0x8001)
#define TAL_MESH_OPCODE_CFG_APP_KEY_LIST                (0x8002)
#define TAL_MESH_OPCODE_CFG_APP_KEY_STAT                (0x8003)
#define TAL_MESH_OPCODE_CFG_COMPO_DATA_GET              (0x8008)
#define TAL_MESH_OPCODE_CFG_COMPO_DATA_STAT             (0x02)
#define TAL_MESH_OPCODE_CFG_BEACON_GET                  (0x8009)
#define TAL_MESH_OPCODE_CFG_BEACON_SET                  (0x800A)
#define TAL_MESH_OPCODE_CFG_BEACON_STAT                 (0x800B)
#define TAL_MESH_OPCODE_CFG_DEFAULT_TTL_GET             (0x800C)
#define TAL_MESH_OPCODE_CFG_DEFAULT_TTL_SET             (0x800D)
#define TAL_MESH_OPCODE_CFG_DEFAULT_TTL_STAT            (0x800E)
#define TAL_MESH_OPCODE_CFG_FRND_GET                    (0x800F)
#define TAL_MESH_OPCODE_CFG_FRND_SET                    (0x8010)
#define TAL_MESH_OPCODE_CFG_FRND_STAT                   (0x8011)
#define TAL_MESH_OPCODE_CFG_PROXY_GET                   (0x8012)
#define TAL_MESH_OPCODE_CFG_PROXY_SET                   (0x8013)
#define TAL_MESH_OPCODE_CFG_PROXY_STAT                  (0x8014)
#define TAL_MESH_OPCODE_CFG_KEY_REFRESH_PHASE_GET       (0x8015)
#define TAL_MESH_OPCODE_CFG_KEY_REFRESH_PHASE_SET       (0x8016)
#define TAL_MESH_OPCODE_CFG_KEY_REFRESH_PHASE_STAT      (0x8017)
#define TAL_MESH_OPCODE_CFG_MODEL_PUB_GET               (0x8018)
#define TAL_MESH_OPCODE_CFG_MODEL_PUB_SET               (0x03)
#define TAL_MESH_OPCODE_CFG_MODEL_PUB_STAT              (0x8019)
#define TAL_MESH_OPCODE_CFG_MODEL_PUB_VA_SET            (0x801A)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_ADD               (0x801B)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_DELETE            (0x801C)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_DELETE_ALL        (0x801D)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_OVERWRITE         (0x801E)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_STAT              (0x801F)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_VA_ADD            (0x8020)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_VA_DELETE         (0x8021)
#define TAL_MESH_OPCODE_CFG_MODEL_SUB_VA_OVERWRITE      (0x8022)
#define TAL_MESH_OPCODE_CFG_NET_TRANS_GET               (0x8023)
#define TAL_MESH_OPCODE_CFG_NET_TRANS_SET               (0x8024)
#define TAL_MESH_OPCODE_CFG_NET_TRANS_STAT              (0x8025)
#define TAL_MESH_OPCODE_CFG_RELAY_GET                   (0x8026)
#define TAL_MESH_OPCODE_CFG_RELAY_SET                   (0x8027)
#define TAL_MESH_OPCODE_CFG_RELAY_STAT                  (0x8028)
#define TAL_MESH_OPCODE_CFG_SIG_MODEL_SUB_GET           (0x8029)
#define TAL_MESH_OPCODE_CFG_SIG_MODEL_SUB_LIST          (0x802A)
#define TAL_MESH_OPCODE_CFG_VENDOR_MODEL_SUB_GET        (0x802B)
#define TAL_MESH_OPCODE_CFG_VENDOR_MODEL_SUB_LIST       (0x802C)
#define TAL_MESH_OPCODE_CFG_LPN_POLL_TO_GET             (0x802D)
#define TAL_MESH_OPCODE_CFG_LPN_POLL_TO_STAT            (0x802E)
#define TAL_MESH_OPCODE_CFG_HB_PUB_GET                  (0x8038)
#define TAL_MESH_OPCODE_CFG_HB_PUB_SET                  (0x8039)
#define TAL_MESH_OPCODE_CFG_HB_PUB_STAT                 (0x06)
#define TAL_MESH_OPCODE_CFG_HB_SUB_GET                  (0x803A)
#define TAL_MESH_OPCODE_CFG_HB_SUB_SET                  (0x803B)
#define TAL_MESH_OPCODE_CFG_HB_SUB_STAT                 (0x803C)
#define TAL_MESH_OPCODE_CFG_MODEL_APP_BIND              (0x803D)
#define TAL_MESH_OPCODE_CFG_MODEL_APP_STAT              (0x803E)
#define TAL_MESH_OPCODE_CFG_MODEL_APP_UNBIND            (0x803F)
#define TAL_MESH_OPCODE_CFG_NET_KEY_ADD                 (0x8040)
#define TAL_MESH_OPCODE_CFG_NET_KEY_DELETE              (0x8041)
#define TAL_MESH_OPCODE_CFG_NET_KEY_GET                 (0x8042)
#define TAL_MESH_OPCODE_CFG_NET_KEY_LIST                (0x8043)
#define TAL_MESH_OPCODE_CFG_NET_KEY_STAT                (0x8044)
#define TAL_MESH_OPCODE_CFG_NET_KEY_UPDATE              (0x8045)
#define TAL_MESH_OPCODE_CFG_NODE_IDENTITY_GET           (0x8046)
#define TAL_MESH_OPCODE_CFG_NODE_IDENTITY_SET           (0x8047)
#define TAL_MESH_OPCODE_CFG_NODE_IDENTITY_STAT          (0x8048)
#define TAL_MESH_OPCODE_CFG_NODE_RESET                  (0x8049)
#define TAL_MESH_OPCODE_CFG_NODE_RESET_STAT             (0x804A)
#define TAL_MESH_OPCODE_CFG_SIG_MODEL_APP_GET           (0x804B)
#define TAL_MESH_OPCODE_CFG_SIG_MODEL_APP_LIST          (0x804C)
#define TAL_MESH_OPCODE_CFG_VENDOR_MODEL_APP_GET        (0x804D)
#define TAL_MESH_OPCODE_CFG_VENDOR_MODEL_APP_LIST       (0x804E)

/* Get local WORD from external 2 BYTE, Little-Endian format */
#define TAL_MESH_EXTRN2WORD(p)                          (((*(p)) & 0xff) + ((*((p)+1)) << 8))
/* Get local DWORD from external 4 BYTE, Little-Endian format  */
#define TAL_MESH_EXTRN2DWORD(p)                         (((unsigned long)(*((p)+0)) & 0xff) + \
    ((unsigned long)(*((p)+1)) << 8) + ((unsigned long)(*((p)+2)) << 16)  + ((unsigned long)(*((p)+3)) << 24))

#define TAL_MESH_OPCODE_SIZE(mesh_opcode)               ((mesh_opcode) >= 0xc00000 ? 3 : ((mesh_opcode) >= 0x8000 ? 2 : 1))
#define TAL_MESH_OPCODE_BYTE(pbuffer, mesh_opcode)\
    do {\
        if(TAL_MESH_OPCODE_SIZE(mesh_opcode) == 1) {\
            *((UCHAR_T *)(pbuffer))         = (mesh_opcode) & 0xff;\
        }else if(TAL_MESH_OPCODE_SIZE(mesh_opcode) == 2) {\
            *((UCHAR_T *)(pbuffer))         = ((mesh_opcode) >> 8) & 0xff;\
            *((UCHAR_T *)(pbuffer) + 1)     = (mesh_opcode) & 0xff;\
        } else {\
            *((UCHAR_T *)(pbuffer))         = ((mesh_opcode) >> 16) & 0xff;\
            *((UCHAR_T *)(pbuffer) + 1)     = ((mesh_opcode) >> 8) & 0xff;\
            *((UCHAR_T *)(pbuffer) + 2)     = (mesh_opcode) & 0xff;\
        }\
    } while(0)

#define MESH_UNICAST_ADDR_MASK                  0x7FFF
#define MESH_VIRTUAL_ADDR_MASK                  0x3FFF
#define MESH_GROUP_ADDR_MASK                    0x3FFF
        
#define MESH_UNASSIGNED_ADDR                    0x0000
#define MESH_GROUP_ADDR_DYNAMIC_START           0xC000
#define MESH_GROUP_ADDR_DYNAMIC_END             0xFEFF
#define MESH_GROUP_ADDR_FIX_START               0xFF00
#define MESH_GROUP_ADDR_ALL_PROXY               0xFFFC
#define MESH_GROUP_ADDR_ALL_FRND                0xFFFD
#define MESH_GROUP_ADDR_ALL_RELAY               0xFFFE
#define MESH_GROUP_ADDR_ALL_NODE                0xFFFF
        
#define MESH_IS_UNASSIGNED_ADDR(addr)           ((addr) == MESH_UNASSIGNED_ADDR)
#define MESH_NOT_UNASSIGNED_ADDR(addr)          ((addr) != MESH_UNASSIGNED_ADDR)
#define MESH_IS_UNICAST_ADDR(addr)              ((addr) != MESH_UNASSIGNED_ADDR && ((addr) & 0x8000) == 0x0000)
#define MESH_NOT_UNICAST_ADDR(addr)             ((addr) == MESH_UNASSIGNED_ADDR || ((addr) & 0x8000) != 0x0000)
#define MESH_IS_VIRTUAL_ADDR(addr)              (((addr) & 0xC000) == 0x8000)
#define MESH_NOT_VIRTUAL_ADDR(addr)             (((addr) & 0xC000) != 0x8000)
#define MESH_IS_GROUP_ADDR(addr)                (((addr) & 0xC000) == 0xC000)
#define MESH_NOT_GROUP_ADDR(addr)               (((addr) & 0xC000) != 0xC000)
#define MESH_IS_RFU_GROUP_ADDR(addr)            ((addr) >= MESH_GROUP_ADDR_FIX_START && (addr) < MESH_GROUP_ADDR_ALL_PROXY)
#define MESH_IS_BROADCAST_ADDR(addr)            ((addr) == MESH_GROUP_ADDR_ALL_NODE)
#define MESH_NOT_BROADCAST_ADDR(addr)           ((addr) != MESH_GROUP_ADDR_ALL_NODE)
#define MESH_IS_SUBSCRIBE_ADDR(addr)            ((addr) != MESH_GROUP_ADDR_ALL_NODE && ((addr) & 0x8000) == 0x8000)
#define MESH_NOT_SUBSCRIBE_ADDR(addr)           ((addr) == MESH_GROUP_ADDR_ALL_NODE || ((addr) & 0x8000) != 0x8000)

/****************************************************** Mesh Provisioner **********************************************************************/
/**< Define parameters for mesh provisioner */
typedef enum {
    TAL_MESH_STACK_INIT = 0x01,                         /**< Init mesh stack event*/

    TAL_MESH_EVT_UNPROV_BEACON_REPORT,                  /**< Receive Mesh Unprovisioned Beacon Data */
        
    TAL_MESH_EVT_PRIVATE_BEACON_REPORT,                 /**< Receive Tuya-Private Beacon Data */

    TAL_MESH_EVT_INVITE_EVT,                            /**< Invite Mesh Device Event */

    TAL_MESH_EVT_CONFIG_DATA_RX,                        /**< Receive Mesh Configuration Message */

    TAL_MESH_EVT_MODEL_DATA_RX,                         /**< Receive Mesh Model Message */
} TAL_MESH_EVT_TYPE_E;

typedef struct {
    UCHAR_T     peer_addr[6];                           /**< Mesh Device Mac Address */
    UCHAR_T     peer_uuid[16];                          /**< Mesh Device UUID */
    
    USHORT_T    node_addr;                              /**< Mesh Node Address */
    UCHAR_T     node_devkey[16];                        /**< Mesh Device key */
}TAL_MESH_DEVICE_INFO_T;

typedef struct {
    USHORT_T    local_addr;                             /**< Mesh Provisioner Local Address */

    UCHAR_T     netkey[16];                             /**< Mesh Provisioner Local Netkey */
    UCHAR_T     appkey[16];                             /**< Mesh Provisioner Local Appkey */
}TAL_MESH_LOCAL_INFO_T;

typedef struct {
    UCHAR_T     mac[6];                                 /**< Mesh Provisioner Scan Adv Mac Address */
    UCHAR_T     uuid[16];                               /**< Mesh Provisioner Scan Mesh Device UUID */
    USHORT_T    oob;                                    /**< Mesh Provisioner Scan Mesh Device OOB */
    UINT_T      uri_hash;                               /**< Mesh Provisioner Scan Mesh Device URI Hash */
    CHAR_T      rssi;                                   /**< Mesh Provisioner Scan Mesh Device Rssi */
}TAL_MESH_UNPROV_BEACON_T;

typedef struct {
    UCHAR_T     mac[6];                                 /**< Beacon Central Scan Adv Mac Address while in mesh mode*/
    UCHAR_T     length;                                 /**< Beacon Central Scan advertising data length*/
    UCHAR_T     *p_data;                                /**< Beacon Central Scan advertising data */
    CHAR_T      rssi;                                   /**< Beacon Central Scan advertising Rssi */
}TAL_PRIVATE_BEACON_T;

typedef struct {
    UINT_T          opcode;                             /**< Mesh opcode. Indicate the mesh data with opcode */
    UCHAR_T         count;                              /**< The number of transmissions is the Transmit Count + 1 */
    UCHAR_T         interval_steps;                     /**< Transmission interval = (Network Retransmit Interval Steps + 1) * 10 */
    UCHAR_T         len;                                /**< Data length for p_data. */
    UCHAR_T         *p_data;                            /**< Data Pointer. Mesh Data should follow Mesh Profile or Mesh Model Spec */
}TAL_MESH_DATA_T;

typedef struct {
    UINT_T          opcode;                             /**< Mesh opcode. Point the mesh opcode while receiving data. */
    USHORT_T        node_addr;                          /**< Mesh Node Address */
    UCHAR_T         len;                                /**< Data length for p_data. */
    UCHAR_T         *p_data;                            /**< Data Pointer. Mesh Data should follow Mesh Profile or Mesh Model Spec */
}TAL_MESH_DATA_RECEIVE_T;

typedef struct {
    USHORT_T        node_addr;                          /**< Mesh Node Address */
    UCHAR_T         devkey[16];                         /**< After finsih provision, we can get mesh dev-key */
}TAL_MESH_PROV_T;

typedef struct {
    TAL_MESH_EVT_TYPE_E         type;                   /**< Receive Mesh Event Type */
    INT_T                       state;                  /**< Receive Mesh Event State */

    union {
        TAL_MESH_UNPROV_BEACON_T    unprov_report;      /**< Receive Mesh Adv report */

        TAL_PRIVATE_BEACON_T        beacon_report;      /**< Receive Tuya-Private beacon report */

        TAL_MESH_PROV_T             prov;               /**< Provision Callback State */

        TAL_MESH_DATA_RECEIVE_T     config_report;      /**< Report Mesh Model Configuration Data */

        TAL_MESH_DATA_RECEIVE_T     model_report;       /**< Report Mesh Model Message Data */
    } mesh_event;
} TAL_MESH_EVT_PARAMS_T;

/**< Define Event Callback for mesh*/
typedef VOID(*TAL_MESH_EVT_FUNC_CB)(TAL_MESH_EVT_PARAMS_T *p_event);

/**< Define All Configuration Structures For Mesh, Should Not Align For These Parameters!!!!*/
/**< [Mesh Profile] 4.3.2.4 Config Composition Data Get */
typedef struct {
    UCHAR_T     page;
} PACKED TAL_MESH_COMPO_DATA_GET_T;

/**< [Mesh Profile] 4.3.2.5 Config Composition Data Status */
typedef struct {
    UCHAR_T     page;
    UCHAR_T     *p_data;
} PACKED TAL_MESH_COMPO_DATA_STATUS_T;

/**< [Mesh Profile] 4.3.2.19-21 Config Model Subscription or Delete*/
typedef struct {
    USHORT_T    element_addr;
    USHORT_T    addr;
    UINT_T      model_id;   /**<  2 bytes or 4 bytes */
} PACKED TAL_MESH_MODEL_SUB_ADD_T, TAL_MESH_MODEL_SUB_DELETE_T;

/**< [Mesh Profile] 4.3.2.26 Config Model Subscription Status */
typedef struct {
    UCHAR_T     status;
    USHORT_T    element_addr;
    USHORT_T    addr;
    UINT_T      model_id;   /**<  2 bytes or 4 bytes */
} PACKED TAL_MESH_MODEL_SUB_STATUS_T;

/**< [Mesh Profile] 4.3.2.31~33 Config NetKey Add Delete Update*/
typedef struct {
    USHORT_T    netkey_index;
    UCHAR_T     netkey[16];     // While doing delete netkey, we dont need netkey
} PACKED TAL_MESH_NETKEY_ADD_T, TAL_MESH_NETKEY_DELETE_T, TAL_MESH_NETKEY_UPDATE_T;

/**< [Mesh Profile] 4.3.2.34 Config NetKey Status */
typedef struct {
    UCHAR_T     status;
    USHORT_T    netkey_index;
} PACKED TAL_MESH_NETKEY_STATUS_T;

/**< [Mesh Profile] 4.3.2.38~40 Config AppKey Delete Update*/
typedef struct {
    UCHAR_T     keyindex[3];
    UCHAR_T     appkey[16];     // While doing delete appkey, we dont need appkey
} PACKED TAL_MESH_APPKEY_ADD_T, TAL_MESH_APPKEY_UPDATE_T, TAL_MESH_APPKEY_DELETE_T;

/**< [Mesh Profile] 4.3.2.40 Config AppKey Status */
typedef struct {
    UCHAR_T     status;
    UCHAR_T     keyindex[3];
} PACKED TAL_MESH_APPKEY_STATUS_T;

/**< [Mesh Profile] 4.3.2.46-47 Config Model App Bind Unbind*/
typedef struct {
    USHORT_T    element_addr;
    USHORT_T    appkey_index;
    UINT_T      model_id;           /**<  2 bytes or 4 bytes */
} PACKED TAL_MESH_MODEL_APPKEY_BIND_T, TAL_MESH_MODEL_APPKEY_UNBIND_T;

/**< [Mesh Profile] 4.3.2.48 Config Model App Status*/
typedef struct {
    UCHAR_T     status;
    USHORT_T    element_addr;
    USHORT_T    appkey_index;
    UINT_T      model_id;           /**<  2 bytes or 4 bytes */
} PACKED TAL_MESH_MODEL_APPKEY_STATUS_T;

/**< [Mesh Profile] 4.3.2.70 Config Network Transmit Set, 4.3.2.71 Config Network Transmit Status */
typedef struct {
    UCHAR_T count : 3, steps : 5;
} PACKED TAL_MESH_NET_TRANSMIT_SET_T, TAL_MESH_NET_TRANSMIT_STATUS_T;

/****************************************************** Mesh Device ***************************************************************************/
/**< Define parameters for mesh device */
typedef struct {
    UINT_T    opcode;        /**< Mesh opcode. */
    UCHAR_T   *data;         /**< Mesh data. */
    USHORT_T  data_len;      /**< Mesh data lenth. */
} TAL_MESH_ACCESS_MSG_T;

typedef struct {
    USHORT_T  src_addr;           /**< Source unicast address. */
    USHORT_T  dst_addr;           /**< Destination  unicast address. */
    UINT_T    seq;                /**< Sequence num of this msg. */
    UCHAR_T   ttl;                /**< Time To Live. */
    USHORT_T  app_key_index;      /**< The appkey index of this msg ues. */
    USHORT_T  net_key_index;      /**< The networkkey index of this msg ues. */
    CHAR_T    rssi;               /**< used when rx in adv bearer. */
} TAL_MESH_NET_PARAM_T;

typedef enum {
    MESH_PB_GATT = 0x00,           /**< Enable PB-GATT, device can be provisioned by GATT connect(mobile phone use). */
    MESH_PB_ADV,                   /**< Enable PB-ADV, device can be provisioned by ADV(Gateway use). */
    MESH_PB_GATT_AND_PB_ADV,       /**< Enable PB-ADV and PB_ADV, device can be provisioned by ADV and GATT connect(Gateway use). */
    MESH_PROVISION_DISABLE,        /**< Disapble provision, close the unprovision beacon and gatt unprovision adv. */
} MESH_PROVISION_TYPE_T;

typedef enum {
    MESH_NETWORK_RESET = 0x00,            /**< Kick out, mesh node will be unprovision state, and it will clear the mesh provision data in ram and flash */
    MESH_NETWORK_RESET_WITH_RECOVER,      /**< Node reset in tam, mesh node will be unprovision state, and it will clear the mesh provision data in ram. The provision data still store in flash */
    MESH_NETWORK_RECOVER,                 /**< Revcover the network, mesh node will be provision state, it will restore the provision data from flash into ram */
} MESH_NETWORK_STATE_SET_T;

/**< Defin mesh message callback for mesh*/
typedef OPERATE_RET (*tal_mesh_msg_recv_cb)(TAL_MESH_ACCESS_MSG_T *msg_raw, TAL_MESH_NET_PARAM_T *net_param);

/**< Define parameters for mesh model */
typedef struct{
    UCHAR_T present;                    /**< The present value of the Generic OnOff state. */
    UCHAR_T target;                     /**< The target value of the Generic OnOff state(optional). */
    UCHAR_T remain_t;                   /**< If the target field is present, the Remaining Time field shall also be present; otherwise these fields shall not be present. */
}TAL_MESH_GENERIC_ONOFF_STATUS_T;

typedef struct{
    UCHAR_T onoff;                      /**< The target value of the Generic OnOff state. */
    UCHAR_T tid;                        /**< Transaction Identifier */
    UCHAR_T transit_t;                  /**< Generic Default Transition Time(optional). */
    UCHAR_T delay;                      /**< If the transit_t field is present, the Delay field shall also be present; otherwise these fields shall not be present. */
}TAL_MESH_GENERIC_ONOFF_SET_T;

typedef struct{
    USHORT_T present;                   /**< The present value of the Light Lightness Actual state. */
    USHORT_T target;                    /**< The target value of the Light Lightness Actual state. (Optional) */
    UCHAR_T remain_t;                   /**< If the target field is present, the Remaining Time field shall also be present; otherwise these fields shall not be present */
}PACKED TAL_MESH_LIGHT_LIGHTNESS_STATUS_T;
 
    typedef struct{
    USHORT_T lightness;                 /**< The target value of the Light Lightness Actual state */
    UCHAR_T tid;                        /**< Transaction Identifier */
    UCHAR_T transit_t;                  /**< Generic Default Transition Time(optional) */
    UCHAR_T delay;                      /**< If the transit_t field is present, the Delay field shall also be present; otherwise these fields shall not be present. */
}PACKED TAL_MESH_LIGHT_LIGHTNESS_SET_T;

typedef struct{
    USHORT_T present_lightness;         /**< The present value of the Light CTL Lightness state */
    USHORT_T present_temp;              /**< The present value of the Light CTL Temperature state */
    USHORT_T target_lightness;          /**< The target value of the Light CTL Lightness state (Optional) */
    USHORT_T target_temp;               /**< If the target_lightness field is present, this field shall also be present; otherwise this field shall not be present */
    UCHAR_T remain_t;                   /**< If the target_lightness field is present, this field shall also be present; otherwise this field shall not be present */
}PACKED TAL_MESH_LIGHT_CTL_STATUS_T;

typedef struct{
    USHORT_T present_temp;              /**< The present value of the Light CTL Temperature state */
    SHORT_T present_delta_uv;           /**< The present value of the Light CTL Delta UV state */
    USHORT_T target_temp;               /**< The target value of the Light CTL Temperature state (Optional) */
    SHORT_T target_delta_uv;            /**< If the target_temp field is present, this field shall also be present; otherwise this field shall not be present */
    UCHAR_T remain_t;                   /**< If the target_temp field is present, this field shall also be present; otherwise this field shall not be present */
}PACKED TAL_MESH_LIGHT_CTL_TEMP_STATUS_T;
 
typedef struct{
    USHORT_T lightness;                 /**< The target value of the Light CTL Lightness state */
    USHORT_T temp;                      /**< The target value of the Light CTL Temperature state */
    SHORT_T delta_uv;                   /**< The target value of the Light CTL Delta UV state */
    UCHAR_T tid;                        /**< Transaction Identifier */
    UCHAR_T transit_t;                  /**< Generic Default Transition Time(Optional) */
    UCHAR_T delay;                      /**< If the transit_t field is present, the Delay field shall also be present; otherwise these fields shall not be present. */
}PACKED TAL_MESH_LIGHT_CTL_SET_T;
 
typedef struct{
    USHORT_T temp;                      /**< The target value of the Light CTL Temperature state */
    SHORT_T delta_uv;                   /**< The target value of the Light CTL Delta UV state. */
    UCHAR_T tid;                        /**< Transaction Identifier */
    UCHAR_T transit_t;                  /**< Generic Default Transition Time(Optional) */
    UCHAR_T delay;                      /**< If the transit_t field is present, the Delay field shall also be present; otherwise these fields shall not be present. */
}PACKED TAL_MESH_LIGHT_CTL_TEMP_SET_T;

typedef struct{
    USHORT_T lightness;                 /**< The present value of the Light HSL Lightness state */
    USHORT_T hue;                       /**< The present value of the Light HSL Hue state */
    USHORT_T sat;                       /**< The present value of the Light HSL Saturation state */
    UCHAR_T remain_t;                   /**< Generic Default Transition Time(Optional) */
}PACKED TAL_MESH_LIGHT_HSL_STATUS_T;

typedef struct{
    USHORT_T lightness;                 /**< The target value of the Light HSL Lightness state */
    USHORT_T hue;                       /**< The target value of the Light HSL Hue state */
    USHORT_T sat;                       /**< The target value of the Light HSL Saturation state */
    UCHAR_T tid;                        /**< Transaction Identifier */
    UCHAR_T transit_t;                  /**< Generic Default Transition Time(Optional) */
    UCHAR_T delay;                      /**< If the transit_t field is present, the Delay field shall also be present; otherwise these fields shall not be present. */
}PACKED TAL_MESH_LIGHT_HSL_SET_T;


typedef enum{
    TAL_MESH_POWER_ON_UNPROVISION = 0,
    TAL_MESH_POWER_ON_PROVISIONED,
    TAL_MESH_PROVISION_SUCCESS,
    TAL_MESH_RESET,
    TAL_MESH_RESET_IN_RAM,
    TAL_MESH_REVERT_IN_MESH,
    TAL_MESH_GROUP_SUB_ADD,
    TAL_MESH_GROUP_SUB_DEL,
}TAL_MESH_NET_STATE_T;

#endif

