#include "tal_api.h"
#include "ble_mgr.h"
#include "tal_event.h"
#include "netmgr.h"
#include "tuya_iot.h"
#include "tuya_cloud_com_defs.h"
#include "ble_dp.h"
#include "mix_method.h"
#include "ble_channel.h"
#include "ble_trsmitr.h"
#include "ble_cryption.h"
#include "tal_bluetooth.h"
#include "crc_16.h"
#include "uni_random.h"


/** GAP - scan response data (max size = 31 bytes) */
#define BLE_SCAN_RSP_DATA_LEN       31
/** GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
#define BLE_ADV_DATA_LEN            31
/*连接监测，非法连接30s断开*/
#define BLE_CONN_MONITOR_TIME       30000
/* ID  (id == uuid)*/
#define BLE_ID_LEN                  16
typedef struct {
    ble_session_fn_t         function;
    void                    *priv_data;
} ble_session_t;

typedef struct {
    ble_frame_trsmitr_t    *trsmitr;
    uint32_t                raw_len;
    uint8_t                 raw_buf[TUYA_BLE_AIR_FRAME_MAX];
    uint32_t                dec_len;
    uint8_t                 dec_buf[TUYA_BLE_AIR_FRAME_MAX];
} ble_packet_recv_t;

typedef struct {
    tuya_ble_cfg_t          cfg;

    uint8_t                 id[16 + 1];
    bool                    is_id_comp;
    ble_crypto_param_t      crypto_param;

    TIMER_ID                pair_timer;     //! 配对非法检测
    TIMER_ID                monitor_timer;  //！监控检测

    uint8_t                 pair_rand[6];
    bool                    is_paired;
    bool                   *is_bound;
    //! tal ble
    TAL_BLE_ROLE_E          role;
    TAL_BLE_PEER_INFO_T     peer_info;
    //! adv & scan rsp
    uint8_t                 adv_len;
    uint8_t                 adv_data[BLE_ADV_DATA_LEN];
    uint8_t                 rsp_len;
    uint8_t                 rsp_data[BLE_SCAN_RSP_DATA_LEN];
    //! packet receive 
    uint32_t                send_sn;
    uint32_t                recv_sn;
    ble_packet_recv_t      *packet_recv;
    ble_session_t           session[BLE_SESSION_MAX];
} tuya_ble_mgr_t;

STATIC tuya_ble_mgr_t   *s_ble_mgr  = NULL;
static bool             s_ble_debug = false;


void tuya_ble_raw_print(char *title, uint8_t width, uint8_t *buf, uint16_t size)
{
    if (!s_ble_debug || NULL == buf) {
        return;
    }

    PR_HEX_DUMP(title, width, buf, size);
}

void tuya_ble_enable_debug(bool enable)
{
    s_ble_debug = enable;
}

static int ble_adv_set(tuya_ble_mgr_t *ble)
{
    tuya_iot_client_t *client = ble->cfg.client;

    ble->adv_len    = 0;
    ble->rsp_len    = 0;

    /* adv data */
    ble->adv_data[ble->adv_len++] = 0x02; /* length */
    ble->adv_data[ble->adv_len++] = 0x01; /* type="Flags" */
    ble->adv_data[ble->adv_len++] = 0x06;
    //! service data
    ble->adv_data[ble->adv_len++] = 0x03; /* length */
    ble->adv_data[ble->adv_len++] = 0x02; /* type="Flags" */
    ble->adv_data[ble->adv_len++] = 0x50;
    ble->adv_data[ble->adv_len++] = 0xFD;
    //! length: 3 + 2 (frame control) + id (len + type + pid)
    ble->adv_data[ble->adv_len++] = 3 + 2 + 2 + BLE_ID_LEN;
    ble->adv_data[ble->adv_len++] = 0x16; /* type="Flags" */
    ble->adv_data[ble->adv_len++] = 0x50;
    ble->adv_data[ble->adv_len++] = 0xFD;

    uint16_t frame_ctrl = 0;
    SETBIT(frame_ctrl,  2);     //bit2, Security_V2,
    SETBIT(frame_ctrl,  3);     //bit3, Security_V2_Confirmed
    SETBIT(frame_ctrl,  8);     //bit8, id include, value:1

    netmgr_status_e status = NETMGR_LINK_DOWN;
    netmgr_conn_get(NETCONN_AUTO, NETCONN_CMD_STATUS, &status);
    if (status == NETMGR_LINK_DOWN) {
        SETBIT(frame_ctrl, 9); //bit9, 请求连接标志（1-请求连接，0-未请求连接）
    }
    
    if (*ble->is_bound) {
        SETBIT(frame_ctrl, 11); //bit11, bound flag
        PR_DEBUG("ble->is_bound %d", *ble->is_bound);
    }
    SETBIT(frame_ctrl, 14);     //bit12-15, version, value:4
    /* rsp data */
    ble->rsp_data[ble->rsp_len++] = 0x17;     /* length, 0x17 或者 0x0D */
    ble->rsp_data[ble->rsp_len++] = 0xFF;     /* type="Flags" */
    ble->rsp_data[ble->rsp_len++] = 0xD0;     /* company id */
    ble->rsp_data[ble->rsp_len++] = 0x07;
    ble->rsp_data[ble->rsp_len++] = TUYA_BLE_SECURE_CONNECTION_WITH_AUTH_KEY; //Encry Mode
    ble->rsp_data[ble->rsp_len++] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY >> 8; //communication way bit0-mesh bit1-wifi bit2-zigbee bit3-NB
    ble->rsp_data[ble->rsp_len++] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY;

    uint8_t *flag = (uint8_t *)&ble->rsp_data[ble->rsp_len++];
    *flag = 0x00;   // bond flag bit7 (8)
    if (ble->is_id_comp) {
        *flag |= ADV_FLAG_UUID_COMP;
    }
    /* adv&rsp data */
    uint8_t *key_in = (uint8_t *)&ble->adv_data[ble->adv_len];
    ble->adv_data[ble->adv_len++] = (frame_ctrl >> 8)   & 0xff;
    ble->adv_data[ble->adv_len++] = (uint8_t)frame_ctrl & 0xff;
    ble->adv_data[ble->adv_len++] = 0x00; //! id type 00-pid 01-product key
    ble->adv_data[ble->adv_len++] = BLE_ID_LEN;   //! ID len
    if (*ble->is_bound) {
        //! adv id encrypt
        *flag |= ADV_FLAG_BOND; /* flag */
        tuya_ble_adv_id_encrypt(ble->crypto_param.sec_key,
                                ble->id,
                                BLE_ID_LEN,
                                &ble->adv_data[ble->adv_len]);
        tuya_ble_rsp_id_encrypt(key_in,
                                BLE_ID_LEN + 4,
                                ble->id,
                                BLE_ID_LEN,
                                &ble->rsp_data[ble->rsp_len]);
    } else {
        *flag &= (~ADV_FLAG_BOND);
        memcpy(&ble->adv_data[ble->adv_len], client->config.productkey, BLE_ID_LEN);
        tuya_ble_rsp_id_encrypt(key_in,
                                BLE_ID_LEN + 4,
                                ble->id,
                                BLE_ID_LEN,
                                &ble->rsp_data[ble->rsp_len]);
    }
    ble->adv_len += MAX_LENGTH_PRODUCT_ID;
    ble->rsp_len += BLE_ID_LEN;
    //! device name
    uint8_t device_name_len = strlen(ble->cfg.device_name);
    if (device_name_len > TUYA_BLE_NAME_LEN) {
        device_name_len = TUYA_BLE_NAME_LEN;
    }
    memcpy(&ble->rsp_data[ble->rsp_len], ble->cfg.device_name, device_name_len);  /* device name */
    ble->rsp_data[ble->rsp_len++] = device_name_len + 1;
    ble->rsp_data[ble->rsp_len++] = 0x09; /* type */
    ble->rsp_len += device_name_len;

    tuya_ble_raw_print("adv_data", 20, (UINT8_T *)ble->adv_data, ble->adv_len);
    tuya_ble_raw_print("rsp_data", 20, (UINT8_T *)ble->rsp_data, ble->rsp_len);

    return OPRT_OK;
}

static uint32_t ble_packet_trsmitr(ble_packet_recv_t *packet_recv, uint8_t *buf, uint32_t len)
{
    static uint32_t pack_no = 0;
    uint32_t subpkg_len = 0;

    int rt = ble_frame_trsmitr_recv_pkg_decode(packet_recv->trsmitr, buf, len);
    if (OPRT_OK != rt && OPRT_SVC_BT_API_TRSMITR_CONTINUE != rt) {  //decode error
        packet_recv->raw_len = 0;
        memset(packet_recv->raw_buf, 0, sizeof(packet_recv->raw_buf));
        return rt;
    }
    //多包的第一包，或者单包的情况，需要清缓存
    if (BLE_FRAME_PKG_FIRST == packet_recv->trsmitr->pkg_desc ||
       (BLE_FRAME_PKG_END   == packet_recv->trsmitr->pkg_desc && 0 == packet_recv->trsmitr->subpkg_num)) {
        packet_recv->raw_len = 0;
        memset(packet_recv->raw_buf, 0, sizeof(packet_recv->raw_buf));
        pack_no = 0;
    }
    pack_no++;
    subpkg_len = ble_frame_subpacket_len_get(packet_recv->trsmitr);
    PR_DEBUG("ble recv sub_pkg desc:%d, no:%d, pack_len:%d, total_len:%d", 
        packet_recv->trsmitr->pkg_desc, pack_no, subpkg_len, packet_recv->raw_len + subpkg_len);

    if ((packet_recv->raw_len + subpkg_len) <= TUYA_BLE_AIR_FRAME_MAX) {
        memcpy(packet_recv->raw_buf + packet_recv->raw_len, ble_frame_subpacket_get(packet_recv->trsmitr), subpkg_len);
    } else {
        rt = OPRT_INVALID_PARM;
        PR_ERR("ble unpack overflow, desc:%d, pack_len:%d", packet_recv->trsmitr->pkg_desc, subpkg_len);
    }

    packet_recv->raw_len += subpkg_len;

    return rt;
}

/*
** SN: 4Byte
** ACK_SN: 4Byte
** CMD: 2Byte
** LEN: 2Byte
** DATA: NByte
** CRC16: 2Byte
*/
#define BLE_PACKET_SN_IND              (0)
#define BLE_PACKET_SN_LEN              (4)
#define BLE_PACKET_ACK_SN_IND          (BLE_PACKET_SN_IND+BLE_PACKET_SN_LEN)
#define BLE_PACKET_ACK_SN_LEN          (4)
#define BLE_PACKET_CMD_IND             (BLE_PACKET_ACK_SN_IND+BLE_PACKET_ACK_SN_LEN)
#define BLE_PACKET_CMD_LEN             (2)
#define BLE_PACKET_DLEN_IND            (BLE_PACKET_CMD_IND+BLE_PACKET_CMD_LEN)
#define BLE_PACKET_DLEN_LEN            (2)
#define BLE_PACKET_DATA_IND            (BLE_PACKET_DLEN_IND+BLE_PACKET_DLEN_LEN)
#define BLE_PACKET_DATA_LEN            (0)
#define BLE_PACKET_CRC16_IND           (BLE_PACKET_DATA_IND+BLE_PACKET_DATA_LEN)
#define BLE_PACKET_CRC16_LEN           (2)
#define BLE_PACKET_MIN_LEN             (BLE_PACKET_CRC16_IND+BLE_PACKET_CRC16_LEN)

static int ble_packet_recv(tuya_ble_mgr_t *ble, uint8_t *buf, uint16_t len, ble_packet_t *packet)
{
    int rt = OPRT_OK;
    ble_packet_recv_t *packet_recv = s_ble_mgr->packet_recv;

    rt = ble_packet_trsmitr(packet_recv, buf, len);
    if (OPRT_OK != rt) {
        if (rt == OPRT_SVC_BT_API_TRSMITR_CONTINUE) {
            PR_DEBUG("ble receive multi-packet...");
        } else {
            PR_ERR("ble trsmitr err:%d", rt);
        }
        return rt;
    }
    if (packet_recv->raw_len > TUYA_BLE_AIR_FRAME_MAX) {
        PR_ERR("ble packet size too large");
        return OPRT_INVALID_PARM;
    }
    if (packet_recv->trsmitr->version < 2) {
        PR_ERR("ble trsmitr version not compatibility! %d", packet_recv->trsmitr->version);
        return OPRT_INVALID_PARM;
    }
    tuya_ble_raw_print("ble raw packet", 32, packet_recv->raw_buf, packet_recv->raw_len);
    rt = tuya_ble_decryption(&ble->crypto_param, 
                              packet_recv->raw_buf, 
                              packet_recv->raw_len, 
                             &packet_recv->dec_len, 
                              packet_recv->dec_buf);
    if (rt != 0) {
        PR_ERR("ble packet decrypt err:%d", rt);
        return OPRT_INVALID_PARM;
    }
    tuya_ble_raw_print("ble dec packet", 32, packet_recv->dec_buf, packet_recv->dec_len);
    uint16_t data_len = 0;
    data_len  = packet_recv->dec_buf[BLE_PACKET_DLEN_IND] << 8;
    data_len += packet_recv->dec_buf[BLE_PACKET_DLEN_IND + 1];
    if (data_len + BLE_PACKET_MIN_LEN > TUYA_BLE_AIR_FRAME_MAX) {
        PR_ERR("ble packet len err:%d", (data_len + BLE_PACKET_MIN_LEN));
        return OPRT_INVALID_PARM;
    }
    // crc check
    uint16_t our_crc = 0;
    our_crc  = packet_recv->dec_buf[BLE_PACKET_CRC16_IND + data_len] << 8;
    our_crc += packet_recv->dec_buf[BLE_PACKET_CRC16_IND + data_len + 1];
    uint16_t his_crc = get_crc_16(packet_recv->dec_buf, data_len + BLE_PACKET_DATA_IND);
    if (our_crc != his_crc) {
        PR_ERR("ble packet crc err:0x%04x, 0x%04x", our_crc, his_crc);
        return OPRT_INVALID_PARM;
    }
    // sn check
    uint32_t recv_sn = 0;
    recv_sn  = packet_recv->dec_buf[BLE_PACKET_SN_IND]     << 24;
    recv_sn += packet_recv->dec_buf[BLE_PACKET_SN_IND + 1] << 16;
    recv_sn += packet_recv->dec_buf[BLE_PACKET_SN_IND + 2] << 8;
    recv_sn += packet_recv->dec_buf[BLE_PACKET_SN_IND + 3];
    PR_NOTICE("ble sn:%d recv sn %d", recv_sn, ble->recv_sn);
    if (recv_sn <= ble->recv_sn) {
        PR_ERR("ble recv sn err");
        tal_ble_disconnect(ble->peer_info);
        return OPRT_INVALID_PARM;
    } else {
        ble->recv_sn = recv_sn;
    }
    packet->type         = packet_recv->dec_buf[BLE_PACKET_CMD_IND] << 8;
    packet->type        += packet_recv->dec_buf[BLE_PACKET_CMD_IND + 1];
    packet->len          = data_len;
    packet->sn           = recv_sn;
    packet->data         = NULL;
    packet->encrypt_mode = packet_recv->raw_buf[0];
    if (0 != packet->len) {
        packet->data = (BYTE_T *)tal_malloc(packet->len);
        if (packet->data == NULL) {
            PR_DEBUG("ble packet malloc err");
            return OPRT_MALLOC_FAILED;
        }
        memcpy(packet->data, &packet_recv->dec_buf[BLE_PACKET_DATA_IND], packet->len);
    }    

    return OPRT_OK;
}

static void ble_adv_update(tuya_ble_mgr_t *ble)
{
    int rt = OPRT_OK;
    if (NULL == ble) {
        return;
    }
    ble_adv_set(ble);
    TAL_BLE_DATA_T adv_data;
    TAL_BLE_DATA_T rsp_data;

    adv_data.p_data = ble->adv_data;
    adv_data.len    = ble->adv_len;
    rsp_data.p_data = ble->rsp_data;
    rsp_data.len    = ble->rsp_len;

    //蓝牙连接时只更新广播内容
    if (ble->is_paired) {
        TUYA_CALL_ERR_LOG(tal_ble_advertising_data_set(&adv_data, &rsp_data));
    } else{
        TUYA_CALL_ERR_LOG(tal_ble_advertising_stop());
        TUYA_CALL_ERR_LOG(tal_ble_advertising_data_set(&adv_data, &rsp_data));
        TAL_BLE_ADV_PARAMS_T ble_adv_params = DEFAULT_ADV_PARAMS(BT_ADV_INTERVAL_MIN, BT_ADV_INTERVAL_MAX);
        TUYA_CALL_ERR_LOG(tal_ble_advertising_start(&ble_adv_params));
    }
    PR_NOTICE("ble adv updated %d", rt);
}


static void ble_pair_timeout_cb(TIMER_ID timer_id, VOID_T *arg)
{
    tuya_ble_mgr_t *ble = (tuya_ble_mgr_t *)arg;

    PR_DEBUG("ble pair timeout then disconnect!!");
    tal_ble_disconnect(ble->peer_info);
}

/* gateway auto check callback*/
static void ble_mointor_timer_cb(TIMER_ID timer_id, VOID_T *arg)
{
    static bool s_iot_conn_stat = false;
    tuya_ble_mgr_t *ble = (tuya_ble_mgr_t *)arg;

    if (tuya_iot_is_connected()) {
        if (s_iot_conn_stat) {
            return;
        }
        if (ble->is_paired) {
            tal_ble_disconnect(ble->peer_info);
        } else {
            tal_ble_advertising_stop();
        }
        PR_DEBUG("ble monitor check iot is connected, stop adv!");
        s_iot_conn_stat = true;
    } else {
        if (!s_iot_conn_stat) {
            return;
        }
        s_iot_conn_stat = false;
        PR_DEBUG("ble monitor check iot is disconnected, start adv!");
        if (ble->is_paired) {
            PR_DEBUG("ble still connected!");
            return;
        }
        //更新并起广播
        ble_adv_update(ble);
    }
}

bool tuya_ble_is_connected(void)
{
    if (NULL == s_ble_mgr) {
        return false;
    }

    return s_ble_mgr->is_paired;
}


int tuya_ble_session_add(ble_seesion_type_t type, ble_session_fn_t fn, void *priv_data)
{
    tuya_ble_mgr_t *ble = s_ble_mgr;

    if (type < BLE_SESSION_MAX) {
        ble->session[type].function  = fn;
        ble->session[type].priv_data = priv_data;
        return OPRT_OK;
    }

    return OPRT_INVALID_PARM;
}

int tuya_ble_session_del(ble_seesion_type_t type)
{
    tuya_ble_mgr_t *ble = s_ble_mgr;

    if (type < BLE_SESSION_MAX) {
        ble->session[type].function  = NULL;
        ble->session[type].priv_data = NULL;
        return OPRT_OK;
    }

    return OPRT_INVALID_PARM;
}

static int ble_packet_encode(tuya_ble_mgr_t *ble, ble_packet_t *packet, uint8_t **outbuf, uint32_t *outlen)
{
    uint8_t *ble_frame = NULL;
    uint8_t *enc_buf   = NULL;

    ble_frame   = tal_malloc(TUYA_BLE_AIR_FRAME_MAX);
    enc_buf     = tal_malloc(TUYA_BLE_AIR_FRAME_MAX);
    if (NULL == enc_buf  || NULL == ble_frame) {
        PR_ERR("ble enc_buf malloc err");
        goto __exit;
    }
    uint32_t send_sn = ble->send_sn++;
    uint32_t frame_len = 0;
    //! SN offset = 0
    ble_frame[frame_len++]  = send_sn >> 24;
    ble_frame[frame_len++]  = send_sn >> 16;
    ble_frame[frame_len++]  = send_sn >> 8;
    ble_frame[frame_len++]  = send_sn;
    //! ACK_SN offset = 4
    ble_frame[frame_len++]  = packet->sn >> 24;   
    ble_frame[frame_len++]  = packet->sn >> 16;
    ble_frame[frame_len++]  = packet->sn >> 8;
    ble_frame[frame_len++]  = packet->sn;
    //! CMD offset = 8
    ble_frame[frame_len++]  = packet->type >> 8;   
    ble_frame[frame_len++]  = packet->type;
    //! LEN offset = 10
    ble_frame[frame_len++]  = packet->len >> 8;   
    ble_frame[frame_len++]  = packet->len;
    //! DATA offset = 12
    if (packet->data != NULL) {
        memcpy(&ble_frame[frame_len], packet->data, packet->len);
    }
    //! CRC16 offset(12) + app_data->len
    frame_len += packet->len;
    uint16_t crc16 = get_crc_16(ble_frame, frame_len);
    ble_frame[frame_len++]  = crc16 >> 8;
    ble_frame[frame_len++]  = crc16;
    //! flag + iv = 17
    enc_buf[0] = packet->encrypt_mode;
    uint16_t padding_len = 17;
    if (frame_len % 16) {
        padding_len += 16 - frame_len % 16;
    }
    if ((frame_len + padding_len) > TUYA_BLE_AIR_FRAME_MAX) {
        PR_ERR("ble packet len exceed");
        goto __exit;
    }
    uint32_t enc_len = 0;
    uint8_t iv[16];
    uni_random_bytes(iv, 16);
    memcpy(&enc_buf[1], iv, 16);
    if (tuya_ble_encryption(&ble->crypto_param,  
                            packet->encrypt_mode, 
                            iv, 
                            ble_frame,
                            frame_len,
                            &enc_len, 
                            &enc_buf[17]) == 0) {
        *outbuf = enc_buf;
        *outlen = enc_len + 17;
    } else {
        PR_ERR("ble frame encrypt err");
        goto __exit;
    }
    tal_free(ble_frame);
    return OPRT_OK;

__exit:
    if (ble_frame) {
        tal_free(ble_frame);
    }
    if (enc_buf) {
        tal_free(enc_buf);
    }

    return OPRT_COM_ERROR;
}


static int ble_packet_resp(tuya_ble_mgr_t *ble, ble_packet_t *resp)
{
    int  rt = OPRT_OK;
    uint8_t *pbuf = NULL;
    ble_frame_trsmitr_t *trsmitr = NULL;
    uint8_t *outbuf = NULL;
    uint32_t outlen;

    TUYA_CALL_ERR_GOTO(ble_packet_encode(ble, resp, &outbuf, &outlen), __exit);
    uint16_t buf_len = ble_frame_packet_len_get();
    rt = OPRT_MALLOC_FAILED;
    TUYA_CHECK_NULL_GOTO(pbuf = (uint8_t*)tal_malloc(buf_len), __exit);
    memset(pbuf, 0, buf_len);
    TUYA_CHECK_NULL_GOTO(trsmitr = ble_frame_trsmitr_create(), __exit);
    do {
        rt = ble_frame_trsmitr_send_pkg_encode(trsmitr, TUYA_BLE_PROTOCOL_VERSION_HIGN, outbuf, outlen);
        if (OPRT_OK != rt && OPRT_SVC_BT_API_TRSMITR_CONTINUE != rt) {
            PR_ERR("ble_send_data_to_app  pkg_encode error %d", rt);
            goto __exit;
        }
        uint32_t send_len = ble_frame_subpacket_len_get(trsmitr);
        memcpy(pbuf, ble_frame_subpacket_get(trsmitr), send_len);
        // tuya_ble_raw_print("ble trsmitr pbuf", 32, pbuf, send_len);
        TAL_BLE_DATA_T ble_data;

        ble_data.p_data = pbuf;
        ble_data.len    = send_len;

        TUYA_CALL_ERR_GOTO(tal_ble_server_common_send(&ble_data), __exit);
        tal_system_sleep(20);
    } while (rt == OPRT_SVC_BT_API_TRSMITR_CONTINUE);

    PR_DEBUG("ble resp finish. len:%d, rt:0x%x", outlen, rt);

__exit:
    if (outbuf) {
        tal_free(outbuf);
    }
    if (pbuf) {
        tal_free(pbuf);
    }
    if (trsmitr) {
        ble_frame_trsmitr_delete(trsmitr);
    }

    return rt;
}

int tuya_ble_send_packet(ble_packet_t *packet)
{
    tuya_ble_mgr_t *ble = s_ble_mgr;

    if (!ble->is_paired) {
        PR_NOTICE("ble not paired");
        return OPRT_OK;
    }
 
    if (FRM_QRY_DEV_INFO_REQ == packet->type) {
        packet->encrypt_mode = *ble->is_bound ? ENCRYPTION_MODE_KEY_14 : ENCRYPTION_MODE_KEY_11;
    } else {
        packet->encrypt_mode = *ble->is_bound ? ENCRYPTION_MODE_SESSION_KEY15 : ENCRYPTION_MODE_KEY_12;
    }

    tuya_ble_raw_print("ble packet", 32, packet->data, packet->len);
    PR_TRACE("ble send. type:0x%x encrpyt:%d", packet->type, packet->encrypt_mode);

    return ble_packet_resp(ble, packet);
}

int tuya_ble_send(uint16_t type, uint32_t ack_sn, uint8_t *data, uint32_t len)
{
    ble_packet_t packet;

    packet.type         = type;
    packet.data         = data;
    packet.len          = len;
    packet.sn           = ack_sn;
    packet.encrypt_mode = 0;

    return tuya_ble_send_packet(&packet);
}

static int ble_unbind_req(ble_packet_t *req, void *priv_data)
{
    uint8_t result_code = 1;
    tuya_ble_mgr_t *ble = (tuya_ble_mgr_t *)priv_data;
    ble_packet_t resp;

    resp.sn           = req->sn;
    resp.type         = req->type;
    resp.len          = 1;
    resp.data         = &result_code;
    resp.encrypt_mode = req->encrypt_mode;

    ble_packet_resp(ble, &resp);
    tuya_iot_reset(tuya_iot_client_get());
    tuya_iot_client_get()->is_activated = false;
    tal_ble_disconnect(ble->peer_info);

    return OPRT_OK;
}

static int ble_pair_req(ble_packet_t *req, void *priv_data)
{
    int rt;
    uint8_t result;
    tuya_ble_mgr_t *ble = (tuya_ble_mgr_t *)priv_data;

    if (0 == memcmp(req->data, ble->crypto_param.uuid, BLE_ID_LEN)) {
        tal_sw_timer_stop(ble->pair_timer);
        if (*ble->is_bound) {
            result = 2;
        } else {
            result = 0;
        }
        ble->is_paired = true;
        PR_NOTICE("Ble is paired"); 
    } else {
        result = 1;
        PR_ERR("ble pair id not match"); 
    }
    ble_packet_t resp;

    resp.sn           = req->sn;
    resp.type         = req->type;
    resp.len          = 1;
    resp.data         = &result;
    resp.encrypt_mode = req->encrypt_mode;

    TUYA_CALL_ERR_GOTO(ble_packet_resp(ble, &resp), __exit);

    netmgr_status_e  netstat;
    netmgr_conn_get(NETCONN_AUTO, NETCONN_CMD_STATUS, &netstat);
    PR_DEBUG("ble send netstat %d", netstat);
    TUYA_CALL_ERR_GOTO(tuya_ble_send(FRM_RPT_NET_STAT_REQ, 0, &netstat, 1), __exit);

__exit: 
    if (result == 1) {
        tal_ble_disconnect(ble->peer_info);
    }

    return rt;
}


static uint8_t ble_dev_info_make(tuya_ble_mgr_t *ble, uint8_t *pbuf, UINT8_T buflen)
{
    uint8_t payload_len = 0;

    //! protocol version
    pbuf[0] = 0x00;
    pbuf[1] = 0x00;
    pbuf[2] = TUYA_BLE_PROTOCOL_VERSION_HIGN;
    pbuf[3] = TUYA_BLE_PROTOCOL_VERSION_LOW;
    // flag
    pbuf[4] = (uint8_t)((1 << 0) | (1 << 2));
    //! has bound
    pbuf[5] = *ble->is_bound;
    //! srand 6
    uni_random_bytes(ble->pair_rand, sizeof(ble->pair_rand));
    memcpy(&pbuf[6], ble->pair_rand, 6);
    // register_key
    tuya_ble_register_key_generate(&pbuf[14], (uint8_t *)ble->cfg.client->config.authkey);
    //! COMMUNICATION_ABILITY
    pbuf[52] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY >> 8;
    pbuf[53] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY; //communication ability
    //! v2 support
    pbuf[54] = (uint8_t)((1 << 1) | (1 << 2)); 
    //! wifi flag
    pbuf[83] = TUYA_BLE_WIFI_DEVICE_REGISTER_MODE;
    //! security flag
    pbuf[86] = (uint8_t)(1 << 0);

    pbuf[95] = PRODUCT_KEY_LEN;
    memset(&pbuf[96], 0, PRODUCT_KEY_LEN);
    payload_len = 96 + PRODUCT_KEY_LEN;
    // mac_len
    pbuf[payload_len++] = 0; //payload_len=112
    // attach_len
    pbuf[payload_len++] = 0; //payload_len=113
    //PacketMaxSize_len+PacketMaxSize
    uint16_t pkg_len = TUYA_BLE_TRANS_DATA_SUBPACK_LEN;
    if (pkg_len < 256) {
        pbuf[payload_len++] = 1;       // PacketMaxSize_len, payload_len=114
        pbuf[payload_len++] = pkg_len; // PacketMaxSize, payload_len=115
    } else {
        pbuf[payload_len++] = 2;
        pbuf[payload_len++] = (pkg_len & 0xFF00) >> 8;
        pbuf[payload_len++] = pkg_len & 0x00FF; // PacketMaxSize, payload_len=116
    }
    pbuf[payload_len++] = 1;
    //sl_value
    pbuf[payload_len++] = TUYA_SECURITY_LEVEL;
    pbuf[payload_len++] = 1;
    //CombosFlag Length
    //bit3：1 – 支持查询设备AP名称；0 – 不支持。
    //bit2：1 – 支持日志采集传输；0 – 不支持。
    //bit1：1 – 支持配网阶段各状态上报；0 – 不支持。
    //bit0：1 – 支持查询 wifi 热点列表；0 – 不支持。
    pbuf[payload_len++] = 0;

    return payload_len;
}

static int ble_dev_info_req(ble_packet_t *req, void *priv_data)
{
    int rt;
    uint8_t* pbuf = NULL;
    uint8_t  buf_len = 128;//增加设备信息时，需要确保缓存够长
    tuya_ble_mgr_t *ble = (tuya_ble_mgr_t *)priv_data;

    //从协议中获取蓝牙分包长度
    UINT16_T pkg_len = (req->data[0] << 8 & 0xff00) + (req->data[1] & 0xff);
    ble_frame_packet_len_set(pkg_len);
    ble_frame_trsmitr_t *trsmitr = ble->packet_recv->trsmitr;
    if (trsmitr->subpkg) {
        tal_free(trsmitr->subpkg);
        trsmitr->subpkg = NULL;
    }
    trsmitr->subpkg = (uint8_t*)tal_malloc(pkg_len);
    if (trsmitr->subpkg == NULL) {
        PR_ERR("malloc err:%d", pkg_len);
        return OPRT_MALLOC_FAILED;
    }
    memset(trsmitr->subpkg, 0, pkg_len);
    PR_NOTICE("ble dev info: state:%d, pkg_len:%d", *ble->is_bound, ble_frame_packet_len_get());

    pbuf = (uint8_t*)tal_malloc(buf_len);//暂定128字节
    if (NULL == pbuf) {
        PR_ERR("malloc err");
        return OPRT_MALLOC_FAILED;
    }
    memset(pbuf, 0, buf_len);
    buf_len = ble_dev_info_make(ble, pbuf, buf_len);
    // tuya_ble_raw_print("ble dev info:", 32, pbuf, buf_len);

    ble_packet_t    resp;
    resp.sn           = req->sn;
    resp.type         = req->type;
    resp.len          = buf_len;
    resp.data         = pbuf;
    resp.encrypt_mode = req->encrypt_mode;

    TUYA_CALL_ERR_GOTO(ble_packet_resp(ble, &resp), __exit);

__exit: 
    if (pbuf) {
        tal_free(pbuf);
    }

    return rt;
}

void ble_session_system_process(ble_packet_t *packet, void *priv_data)
{
    int rt;

    switch (packet->type) {

    case FRM_QRY_DEV_INFO_REQ:
       TUYA_CALL_ERR_LOG(ble_dev_info_req(packet, priv_data));
       break;

    case FRM_PAIR_REQ:
        TUYA_CALL_ERR_LOG(ble_pair_req(packet, priv_data));
        break;

    case FRM_UNBONDING_REQ:
    case FRM_DEVICE_RESET:
        TUYA_CALL_ERR_LOG(ble_unbind_req(packet, priv_data));
        break;

    default:
        PR_TRACE("bt_dp can not process cmd: 0x%x ",packet->type);
        break;
    }

}


static void tal_ble_event_callback(TAL_BLE_EVT_PARAMS_T *msg)
{
    tuya_ble_mgr_t *ble = s_ble_mgr;

    if (NULL == ble) {
        return;
    }

    PR_TRACE("rev ble event %d", msg->type);

    switch (msg->type) {
    case TAL_BLE_STACK_INIT: {
        if (msg->ble_event.init == 0) {
            ble_adv_update(ble);
        }
    }
    break;

    case TAL_BLE_EVT_PERIPHERAL_CONNECT: {
        if (msg->ble_event.connect.result == 0) {
            memcpy(&ble->peer_info, &msg->ble_event.connect.peer, sizeof(TAL_BLE_PEER_INFO_T));
            ble->recv_sn = 0;
            ble->send_sn = 1;
            tal_sw_timer_start(ble->pair_timer, BLE_CONN_MONITOR_TIME, TAL_TIMER_ONCE);
            PR_NOTICE("Ble Connected");
        } else {
            memset(&ble->peer_info, 0, sizeof(TAL_BLE_PEER_INFO_T));
        }
    }
    break;

    case TAL_BLE_EVT_DISCONNECT: {
        memset(&ble->peer_info, 0x00, sizeof(TAL_BLE_PEER_INFO_T));
        memset(ble->pair_rand,  0x00, sizeof(ble->pair_rand));
        tal_sw_timer_stop(ble->pair_timer);
        ble->is_paired = false;
        if (!tuya_iot_is_connected()) {
            ble_adv_update(ble);
        }
        PR_NOTICE("Ble Disonnected");
    }
    break;

    case TAL_BLE_EVT_WRITE_REQ: {
        int ret = OPRT_OK;
        ble_packet_t packet;
        TAL_BLE_DATA_T *report;

        if (msg->ble_event.write_report.peer.char_handle[0] == ble->peer_info.char_handle[TAL_COMMON_WRITE_CHAR_INDEX]) {
            report = &msg->ble_event.write_report.report;
            PR_TRACE("BLE Package len %d", report->len);
            ret = ble_packet_recv(ble, report->p_data, report->len, &packet);
            if (OPRT_OK != ret) {
                if (ret != OPRT_SVC_BT_API_TRSMITR_CONTINUE) {
                    PR_ERR("tuya_ble_data_proc fail. %d", ret);
                }
                break;
            }
            PR_DEBUG("ble recv req type 0x%04x", packet.type);
            int i;
            for (i = 0; i < BLE_SESSION_MAX; i++) {
                if (ble->session[i].function) {
                    ble->session[i].function(&packet, ble->session[i].priv_data);
                }
            }
            tal_free(packet.data);
        }
    }
    break;

    default:
        break;
    }
}

int tuya_ble_deinit(void)
{
    tuya_ble_mgr_t *ble = s_ble_mgr;

    //未初始化则返回
    if (NULL == ble) {
        return OPRT_OK;
    }
    PR_NOTICE("ble deinit...");
    if (ble->pair_timer) {
        tal_sw_timer_delete(ble->pair_timer);
    }
    if (ble->monitor_timer) {
        tal_sw_timer_delete(ble->monitor_timer);
    }
    if (ble->packet_recv && ble->packet_recv->trsmitr) {
        ble_frame_trsmitr_delete(ble->packet_recv->trsmitr);
    }
    if (ble->packet_recv) {
        tal_free(ble->packet_recv);
    }
    tuya_ble_session_del(BLE_SESSION_SYSTEM);
    tuya_ble_session_del(BLE_SESSION_CHANNEL);
    tuya_ble_session_del(BLE_SESSION_DP);
    tal_ble_bt_deinit(ble->role);
    tal_free(ble);
    s_ble_mgr = NULL;

    return OPRT_OK;
}

int tuya_ble_init(tuya_ble_cfg_t *cfg)
{
    int rt = OPRT_OK;

    if (s_ble_mgr) {
        return OPRT_OK;
    }
    tuya_ble_mgr_t *ble = NULL;
    ble = tal_malloc(sizeof(tuya_ble_mgr_t));
    if (NULL == ble) {
        return OPRT_MALLOC_FAILED;
    }
    memset(ble, 0, sizeof(tuya_ble_mgr_t));
    ble->packet_recv          = tal_malloc(sizeof(ble_packet_recv_t));
    if (NULL == ble->packet_recv ) {
        tal_free(ble);
        return OPRT_MALLOC_FAILED;
    }
    ble->packet_recv->trsmitr = ble_frame_trsmitr_create();
    if (NULL == ble->packet_recv->trsmitr) {
        tal_free(ble->packet_recv);
        tal_free(ble);
        return OPRT_MALLOC_FAILED;
    }
    s_ble_mgr = ble;
    memcpy(&ble->cfg, cfg, sizeof(tuya_ble_cfg_t));
    ble->is_bound = &ble->cfg.client->is_activated;
    if (strlen(ble->cfg.client->config.uuid) >= 20) {
        tuya_ble_id_compress((uint8_t *)ble->cfg.client->config.uuid, ble->id);
        ble->is_id_comp = true;
    } else {
        memcpy(ble->id, ble->cfg.client->config.uuid, 16);
    }
    ble->crypto_param.uuid      = (uint8_t *)ble->id;
    ble->crypto_param.auth_key  = (uint8_t *)ble->cfg.client->config.authkey;
    ble->crypto_param.sec_key   = (uint8_t *)ble->cfg.client->activate.seckey;
    ble->crypto_param.login_key = (uint8_t *)ble->cfg.client->activate.localkey;
    ble->crypto_param.pair_rand = (uint8_t *)ble->pair_rand;
    TUYA_CALL_ERR_GOTO(tal_sw_timer_create(ble_pair_timeout_cb,  ble, &ble->pair_timer),  __exit);
    TUYA_CALL_ERR_GOTO(tal_sw_timer_create(ble_mointor_timer_cb, ble, &ble->monitor_timer), __exit);
    TUYA_CALL_ERR_GOTO(tal_sw_timer_start(ble->monitor_timer, 3000, TAL_TIMER_CYCLE), __exit);
    tuya_ble_session_add(BLE_SESSION_SYSTEM,  ble_session_system_process,  ble);
    tuya_ble_session_add(BLE_SESSION_CHANNEL, ble_session_channel_process, ble);
    tuya_ble_session_add(BLE_SESSION_DP,      ble_session_dp_process, ble->cfg.client);
    ble->role = TAL_BLE_ROLE_PERIPERAL | TAL_BLE_ROLE_CENTRAL;
    TUYA_CALL_ERR_GOTO(tal_ble_bt_init(ble->role, tal_ble_event_callback), __exit);
    PR_NOTICE("tuya ble init success finish");

    return OPRT_OK;

__exit:
    tuya_ble_deinit();
    PR_NOTICE("tuya ble init failed %d", rt);

    return rt;
}
