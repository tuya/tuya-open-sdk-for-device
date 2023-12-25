#include "tuya_protocol.h"
#include "tal_api.h"
#include "crc32i.h"
#include "mix_method.h"
#include "uni_random.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
// PV 2.2 LPV 3.2
#define PV_OFFSET_22_32 0
#define PV_LEN_22_32 3
#define CRC_OFFSET_22_32            (PV_OFFSET_22_32  + PV_LEN_22_32)
#define SEQ_OFFSET_22_32            (CRC_OFFSET_22_32 + SIZEOF(UINT_T))
#define CMD_FROM_OFFSET_22_32       (SEQ_OFFSET_22_32 + SIZEOF(UINT_T))
#define DATA_OFFSET_22_32           (CMD_FROM_OFFSET_22_32 + SIZEOF(UINT_T)) // PV/LPV(CHAR_T[3]) + CRC(UINT_T) + SEQ(UINT_T) + CMD_FROM(UINT_T)

// PV 2.3
#define PV23_VERSION_OFFSET         (0)
#define PV23_VERSION_LEN            (3)
#define PV23_SEQ_OFFSET             (PV23_VERSION_LEN)
#define PV23_CMD_FROM_OFFSET        (PV23_SEQ_OFFSET + SIZEOF(UINT_T))
#define PV23_RESERVE_OFFSET         (PV23_CMD_FROM_OFFSET + SIZEOF(UINT_T))
#define PV23_RESERVE_LEN            (1)
#define PV23_NONCE_OFFSET           (PV23_RESERVE_OFFSET + PV23_RESERVE_LEN)
#define PV23_NONCE_LEN              (12)
#define PV23_DATA_OFFSET            (PV23_NONCE_OFFSET + PV23_NONCE_LEN)
#define PV23_TAG_LEN                (16)
#define PV23_AD_DATA_LEN            (12)
#define PV23_EXCEPT_DATA_LEN        (PV23_AD_DATA_LEN + PV23_NONCE_LEN + PV23_TAG_LEN)


UINT_T tuya_pack_protocol_serial_no(VOID)
{
    STATIC UINT_T serial_no = 0;

    if (0 == serial_no) {
        uni_random_bytes((unsigned char *)&serial_no, 4);
    }

    serial_no++;

    return serial_no;
}


STATIC OPERATE_RET __parse_data_with_pv23(IN CONST DP_CMD_TYPE_E cmd, IN CONST BYTE_T *data, \
                                          IN CONST UINT_T len, IN CONST BYTE_T *key, OUT CHAR_T **out_data)
{
    OPERATE_RET op_ret = OPRT_OK;
    if (memcmp(data, TUYA_PV23, PV23_VERSION_LEN) != 0) {
        PR_ERR("verison error, must pv2.3");
        return OPRT_VERSION_FMT_ERR;
    }

    UINT_T seq = 0;
    memcpy(&seq, (data + PV23_SEQ_OFFSET), SIZEOF(UINT_T));
    seq = UNI_NTOHL(seq);

    UINT_T cmd_from = 0;
    memcpy(&cmd_from, (data + PV23_CMD_FROM_OFFSET), SIZEOF(UINT_T));
    cmd_from = UNI_NTOHL(cmd_from);

    // reserve_field must clean zore
    BYTE_T reserve_field = 0;
    if (memcmp(data + PV23_RESERVE_OFFSET, &reserve_field, PV23_RESERVE_LEN) != 0) {
        PR_ERR("reserve_field must clean zore");
        return OPRT_VERSION_FMT_ERR;
    }

    BYTE_T* ad_data = (BYTE_T*)(data + 0);
    UINT_T data_len = len - PV23_EXCEPT_DATA_LEN;
    size_t ec_len = 0;
    BYTE_T *ec_data = tal_malloc(data_len + 1);
    TUYA_CHECK_NULL_RETURN(ec_data, OPRT_MALLOC_FAILED);

    // decrypt data
    op_ret = mbedtls_cipher_auth_decrypt_wrapper(
    &(const cipher_params_t) {
        .cipher_type = MBEDTLS_CIPHER_AES_128_GCM,
        .key = (unsigned char *)key,
        .key_len = 16,
        .nonce = (unsigned char *)(data + PV23_NONCE_OFFSET),
        .nonce_len = PV23_NONCE_LEN,
        .ad = ad_data,
        .ad_len = PV23_AD_DATA_LEN,
        .data = (unsigned char *)(data + PV23_DATA_OFFSET),
        .data_len = data_len
    },
    ec_data, &ec_len,
    (unsigned char *)(data + (len - PV23_TAG_LEN)), PV23_TAG_LEN);
    if (op_ret != OPRT_OK) {
        PR_ERR("mbedtls_cipher_auth_decrypt_wrapper:0x%x", -op_ret);
        *out_data = NULL;
        tal_free(ec_data);
        return op_ret;
    }

    ec_data[ec_len] = 0;

    *out_data = (CHAR_T *)ec_data;

    return OPRT_OK;
}

STATIC OPERATE_RET __parse_data_with_lpv35(IN CONST DP_CMD_TYPE_E cmd, IN CONST BYTE_T *data, \
                                           IN CONST UINT_T len, IN CONST BYTE_T *key, OUT CHAR_T **out_data)
{
    CHAR_T pv_buf[4];
    memset(pv_buf, 0, sizeof(pv_buf));
    memcpy(pv_buf, data + PV_OFFSET_22_32, PV_LEN_22_32);

    UINT_T seq = 0;
    memcpy(&seq, (data + SEQ_OFFSET_22_32), SIZEOF(UINT_T));
    seq = UNI_NTOHL(seq);

    UINT_T cmd_from = 0;
    memcpy(&cmd_from, (data + CMD_FROM_OFFSET_22_32), SIZEOF(UINT_T));
    cmd_from = UNI_NTOHL(cmd_from);


    BYTE_T *ec_data = NULL;
    UINT_T ec_len = len - DATA_OFFSET_22_32;

    ec_data = tal_malloc(ec_len + 1);
    TUYA_CHECK_NULL_RETURN(ec_data, OPRT_MALLOC_FAILED);
    memcpy(ec_data, data + DATA_OFFSET_22_32, ec_len);

    ec_data[ec_len] = 0;

    *out_data = (CHAR_T *)ec_data;

    return OPRT_OK;
}


OPERATE_RET tuya_parse_protocol_data(IN CONST DP_CMD_TYPE_E cmd, IN BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *key, OUT CHAR_T **out_data)
{
    if ((NULL == data) || (len < DATA_OFFSET_22_32)) {
        PR_ERR("data is NULL OR Len Invalid %d", len);
        return OPRT_INVALID_PARM;
    }

    OPERATE_RET op_ret = OPRT_OK;

    CHAR_T *pv = NULL;

    if (DP_CMD_LAN == cmd) {
        pv = TUYA_LPV35;
    } else if (DP_CMD_MQ == cmd) {
        pv = TUYA_PV23;
    }

    if (DP_CMD_LAN == cmd) {
        if (0 == strcmp(pv, "3.5")) {
            PR_TRACE("Data From LAN AND V=3.5");
            op_ret = __parse_data_with_lpv35(cmd, data, len, (BYTE_T *)key, out_data);
        } else {
            PR_ERR("Data From LAN But No Match Parse %s", pv);
            return OPRT_COM_ERROR;
        }
    } else if (DP_CMD_MQ == cmd) {
        if (0 == strcmp(pv, "2.3")) {
            PR_TRACE("Data From MQTT AND V=2.3");
            op_ret = __parse_data_with_pv23(cmd, data, len, (BYTE_T *)key, out_data);
        } else {
            PR_ERR("Data From MQTT But No Match Parse %s", pv);
            return OPRT_COM_ERROR;
        }
    } else {
        PR_ERR("Invlaid Cmd:%d", cmd);
        return OPRT_COM_ERROR;
    }

    return op_ret;
}

STATIC OPERATE_RET __pack_data_with_cmd_pv23(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *pv, IN CONST CHAR_T *src, \
                                             IN CONST UINT_T pro, IN CONST UINT_T num, IN CONST BYTE_T *key, \
                                             OUT BYTE_T **pack_out, OUT UINT_T *out_len)
{
    OPERATE_RET op_ret = OPRT_OK;
    CHAR_T        *out = NULL;
    int offset = 0;

    PR_TRACE("To:%d src:%s pro:%d num:%d", cmd, src, pro, num);
    // make json data
    int len = strlen(src) + 60;
    out = tal_malloc(len);
    if (NULL == out) {
        PR_ERR("tal_malloc Fails %d", len);
        return OPRT_MALLOC_FAILED;
    }

    offset += sprintf(out + offset, "{\"protocol\":%d,\"t\":%d,\"data\":%s", \
                        pro, (UINT_T)tal_time_get_posix(), src);
    out[offset++] = '}';
    out[offset] = 0;

    PR_TRACE("After Pack:%s offset:%d", out, offset);

    // make pack data
    BYTE_T *buf = tal_malloc(offset + PV23_EXCEPT_DATA_LEN);
    if (buf == NULL) {
        tal_free(out);
        return OPRT_MALLOC_FAILED;
    }
    memset(buf, 0, offset + PV23_EXCEPT_DATA_LEN);

    // make head data
    // version
    memcpy(buf + PV23_VERSION_OFFSET, pv, PV_LEN_22_32);

    // seq
    UINT_T tmp = UNI_HTONL(num);
    memcpy(buf + PV23_SEQ_OFFSET, (BYTE_T *)(&tmp), SIZEOF(UINT_T));

    // cmd from
    tmp = UNI_HTONL(0x00000001);
    memcpy(buf + PV23_CMD_FROM_OFFSET, (BYTE_T *)(&tmp), SIZEOF(UINT_T));

    // reserve
    memset(buf + PV23_RESERVE_OFFSET, 0, 1);

    // nonce
    uni_random_string((char*)(buf + PV23_NONCE_OFFSET), PV23_NONCE_LEN);

    // AES GCM encrypt
    size_t encrypt_olen = 0;
    op_ret = mbedtls_cipher_auth_encrypt_wrapper(
    &(const cipher_params_t) {
        .cipher_type = MBEDTLS_CIPHER_AES_128_GCM,
        .key = (unsigned char *)key,
        .key_len = 16,
        .nonce = buf + PV23_NONCE_OFFSET,
        .nonce_len = PV23_NONCE_LEN,
        .ad = buf,
        .ad_len = PV23_AD_DATA_LEN,
        .data = (unsigned char *)out,
        .data_len = strlen(out)
    },
    buf + PV23_DATA_OFFSET, &encrypt_olen,
    buf + PV23_DATA_OFFSET + offset, PV23_TAG_LEN);
    tal_free(out);
    if (op_ret != OPRT_OK) {
        PR_ERR("mbedtls_cipher_auth_encrypt_wrapper:0x%x", -op_ret);
        tal_free(buf);
        return op_ret;
    }

    *pack_out = buf;
    *out_len = PV23_EXCEPT_DATA_LEN + encrypt_olen;

    return OPRT_OK;
}

STATIC OPERATE_RET __pack_data_with_cmd_lpv35(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *pv, IN CONST CHAR_T *src, \
                                              IN CONST UINT_T pro, IN CONST UINT_T num, IN CONST BYTE_T *key, \
                                              OUT BYTE_T **pack_out, OUT UINT_T *out_len)
{
    CHAR_T *out = NULL;
    int offset = 0;

    PR_TRACE("To:%d src:%s pro:%d num:%d", cmd, src, pro, num);

    // make json data
    int len = strlen(src) + 60;
    out = tal_malloc(len);
    if (NULL == out) {
        PR_ERR("tal_malloc Fails %d", len);
        return OPRT_MALLOC_FAILED;
    }

    offset += sprintf(out + offset, "{\"protocol\":%d,\"t\":%d,\"data\":%s", \
                      pro, (UINT_T)tal_time_get_posix(), src);
    out[offset++] = '}';
    out[offset] = 0;

    PR_TRACE("After Pack:%s offset:%d", out, offset);

    // make pack data
    BYTE_T *buf = tal_malloc(DATA_OFFSET_22_32 + offset + 16);
    if (buf == NULL) {
        tal_free(out);
        return OPRT_MALLOC_FAILED;
    }
    memset(buf, 0, DATA_OFFSET_22_32 + offset + 16);

    // not aes data
    memcpy(buf + DATA_OFFSET_22_32, out, offset);
    tal_free(out);

    *pack_out = buf;
    *out_len = (DATA_OFFSET_22_32 + offset);

    // make head data
    memcpy(buf + PV_OFFSET_22_32, pv, PV_LEN_22_32);

    UINT_T tmp = UNI_HTONL(0x00000000);
    memcpy(buf + CRC_OFFSET_22_32, (BYTE_T *)(&tmp), SIZEOF(UINT_T));

    tmp = UNI_HTONL(num);
    memcpy(buf + SEQ_OFFSET_22_32, (BYTE_T *)(&tmp), SIZEOF(UINT_T));

    tmp = UNI_HTONL(0x00000001);
    memcpy(buf + CMD_FROM_OFFSET_22_32, (BYTE_T *)(&tmp), SIZEOF(UINT_T));

    return OPRT_OK;
}


OPERATE_RET tuya_pack_protocol_data(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *src, IN CONST UINT_T pro, IN BYTE_T *key, OUT CHAR_T **out, OUT UINT_T *out_len)
{
    if ((NULL == src) || NULL == out) {
        PR_ERR("Invalid Param");
        return OPRT_INVALID_PARM;
    }

    OPERATE_RET op_ret = OPRT_OK;

    CHAR_T *pv = NULL;

    if (DP_CMD_LAN == cmd) {
        pv = TUYA_LPV35;
    } else if (DP_CMD_MQ == cmd) {
        pv = TUYA_PV23;
    }
    UINT_T num = tuya_pack_protocol_serial_no();

    if (DP_CMD_LAN == cmd) {
        if (0 == strcmp(pv, "3.5")) {
            PR_TRACE("Data To LAN AND V=3.5");
            op_ret = __pack_data_with_cmd_lpv35(cmd, pv, src, pro, num, (BYTE_T *)key, (BYTE_T **)out, out_len);
        } else {
            PR_ERR("Data To LAN But No Match Parse %s", pv);
            return OPRT_COM_ERROR;
        }
    } else if (DP_CMD_MQ == cmd) {
        if (0 == strcmp(pv, "2.3")) {
            PR_TRACE("Data To MQTT AND V=2.3");
            op_ret = __pack_data_with_cmd_pv23(cmd, pv, src, pro, num, key, (BYTE_T **)out, out_len);
        } else {
            PR_ERR("Data To MQTT But No Match Parse %s", pv);
            return OPRT_COM_ERROR;
        }
    } else {
        PR_ERR("Invlaid Cmd:%d", cmd);
        return OPRT_COM_ERROR;
    }

    return op_ret;
}


INT_T lpv35_frame_buffer_size_get(lpv35_frame_object_t* frame_obj)
{
    return (LPV35_FRAME_HEAD_SIZE + SIZEOF(lpv35_additional_data_t) + \
            LPV35_FRAME_NONCE_SIZE + frame_obj->data_len + \
            LPV35_FRAME_TAG_SIZE + LPV35_FRAME_TAIL_SIZE);
}

/* 数据前后封装55aa */
OPERATE_RET lpv35_frame_serialize(CONST BYTE_T* key, INT_T key_len,
                                  CONST lpv35_frame_object_t* input,
                                  BYTE_T* output, INT_T* olen)
{
    if (key == NULL || key_len == 0 || input == NULL || output == NULL || olen == NULL) {
        PR_ERR("PARAM ERROR");
        return OPRT_INVALID_PARM;
    }

    OPERATE_RET op_ret = OPRT_OK;
    INT_T offset = 0;

    // HEAD
    memcpy(output, LPV35_FRAME_HEAD, LPV35_FRAME_HEAD_SIZE);
    offset += LPV35_FRAME_HEAD_SIZE;

    // AD
    lpv35_additional_data_t ad = {
        .version = 0,
        .sequence = UNI_HTONL(input->sequence),
        .type = UNI_HTONL(input->type),
        .length = UNI_HTONL(LPV35_FRAME_NONCE_SIZE + input->data_len + LPV35_FRAME_TAG_SIZE)
    };
    memcpy(output + offset, (BYTE_T*)&ad, SIZEOF(lpv35_additional_data_t));
    offset += SIZEOF(lpv35_additional_data_t);

    //  tmp nonce
    BYTE_T i = 0;
    BYTE_T nonce[LPV35_FRAME_NONCE_SIZE];
    for (i = 0; i < LPV35_FRAME_NONCE_SIZE; i++) {
        nonce[i] = uni_random_range(0xFF);
    }
    memcpy(output + offset, &(nonce[0]), LPV35_FRAME_NONCE_SIZE);
    offset += LPV35_FRAME_NONCE_SIZE;

    // TAG buffer
    BYTE_T tag[LPV35_FRAME_TAG_SIZE] = {0};

    // AES GCM encrypt
    size_t encrypt_olen = 0;
    op_ret = mbedtls_cipher_auth_encrypt_wrapper(
    &(CONST cipher_params_t) {
        .cipher_type = MBEDTLS_CIPHER_AES_128_GCM,
        .key = (unsigned char *)key,
        .key_len = key_len,
        .nonce = nonce,
        .nonce_len = LPV35_FRAME_NONCE_SIZE,
        .ad = (BYTE_T*)(&ad),
        .ad_len = SIZEOF(lpv35_additional_data_t),
        .data = input->data,
        .data_len = input->data_len
    },
    output + offset, &encrypt_olen,
    tag, LPV35_FRAME_TAG_SIZE);
    if (op_ret != OPRT_OK) {
        PR_ERR("mbedtls_cipher_auth_encrypt_wrapper:0x%x", -op_ret);
        return op_ret;
    }
    offset += encrypt_olen;

    // TAG
    memcpy(output + offset, tag, LPV35_FRAME_TAG_SIZE);
    offset += LPV35_FRAME_TAG_SIZE;

    // TAIL
    memcpy(output + offset, LPV35_FRAME_TAIL, LPV35_FRAME_TAIL_SIZE);
    offset += LPV35_FRAME_TAIL_SIZE;
    *olen = offset;

    PR_TRACE("lpv35_frame_buffer_size_get:%d", lpv35_frame_buffer_size_get((lpv35_frame_object_t *)input));
    PR_TRACE("offset:%d", offset);

    return op_ret;
}

OPERATE_RET lpv35_frame_parse(CONST BYTE_T* key, INT_T key_len,
                              CONST BYTE_T* input, INT_T ilen,
                              lpv35_frame_object_t* output)
{
    OPERATE_RET op_ret = OPRT_OK;
    INT_T offset = 0;

    if (key == NULL || key_len == 0 || input == NULL || ilen == 0 || output == NULL) {
        PR_ERR("PARAM ERROR");
        return OPRT_INVALID_PARM;
    }

    // head tail verify
    if ((memcmp(input, LPV35_FRAME_HEAD, LPV35_FRAME_HEAD_SIZE) != 0) || \
        (memcmp(input + (ilen - LPV35_FRAME_TAIL_SIZE), LPV35_FRAME_TAIL, LPV35_FRAME_TAIL_SIZE) != 0)) {
        PR_ERR("LPV35 HEAD OR TAIL ERROR");
        return OPRT_VERSION_FMT_ERR;
    }
    offset += LPV35_FRAME_HEAD_SIZE;

    // version
    //BYTE_T version = *(input + offset) & 0x0f;
    //PR_DEBUG("version:%d", version);
    offset += LPV35_FRAME_VERSION_SIZE;

    // reserve
    offset += LPV35_FRAME_RESERVE_SIZE;

    // sequence
    memcpy(&output->sequence, input + offset, LPV35_FRAME_SEQUENCE_SIZE);
    output->sequence = UNI_HTONL(output->sequence);
    //PR_DEBUG("sequence:0x%x", output->sequence);
    offset += LPV35_FRAME_SEQUENCE_SIZE;

    // type
    memcpy(&output->type, input + offset, LPV35_FRAME_TYPE_SIZE);
    output->type = UNI_HTONL(output->type);
    //PR_DEBUG("type:0x%x", output->type);
    offset += LPV35_FRAME_TYPE_SIZE;

    // length
    UINT_T length = 0;
    memcpy(&length, input + offset, LPV35_FRAME_DATALEN_SIZE);
    length = UNI_HTONL(length);
    //PR_DEBUG("length:%d", length);
    offset += LPV35_FRAME_DATALEN_SIZE;

    // length verify
    if (length != ilen - offset - LPV35_FRAME_TAIL_SIZE) {
        PR_ERR("length error, length:%d", length);
        return OPRT_COM_ERROR;
    }

    // nonce
    BYTE_T nonce[LPV35_FRAME_NONCE_SIZE];
    memcpy(nonce, input + offset, LPV35_FRAME_NONCE_SIZE);
    //tuya_debug_hex_dump("nonce", LPV35_FRAME_NONCE_SIZE, nonce, LPV35_FRAME_NONCE_SIZE);
    offset += LPV35_FRAME_NONCE_SIZE;

    // encryption data
    BYTE_T* data = (BYTE_T *)(input + offset);
    output->data_len = length - LPV35_FRAME_NONCE_SIZE - LPV35_FRAME_TAG_SIZE;
    offset += output->data_len;

    // tag
    BYTE_T tag[LPV35_FRAME_TAG_SIZE];
    memcpy(tag, input + offset, LPV35_FRAME_TAG_SIZE);

    // AD
    lpv35_additional_data_t ad;
    memcpy(&ad, input + LPV35_FRAME_HEAD_SIZE, SIZEOF(lpv35_additional_data_t));

    // decrypt data
    output->data = tal_malloc(output->data_len + 1);
    TUYA_CHECK_NULL_RETURN(output->data, OPRT_MALLOC_FAILED);
    memset(output->data, 0, output->data_len + 1);
    size_t decrypt_olen = 0;
    op_ret = mbedtls_cipher_auth_decrypt_wrapper(
    &(CONST cipher_params_t) {
        .cipher_type = MBEDTLS_CIPHER_AES_128_GCM,
        .key = (unsigned char *)key,
        .key_len = key_len,
        .nonce = nonce,
        .nonce_len = LPV35_FRAME_NONCE_SIZE,
        .ad = (BYTE_T*)(&ad),
        .ad_len = SIZEOF(lpv35_additional_data_t),
        .data = data,
        .data_len = output->data_len
    },
    output->data, &decrypt_olen,
    tag, LPV35_FRAME_TAG_SIZE);
    if (op_ret != OPRT_OK) {
        PR_ERR("mbedtls_cipher_auth_decrypt_wrapper:0x%x", -op_ret);
        tal_free(output->data);
        output->data = NULL;
        return op_ret;
    }
    output->data_len = (uint32_t)decrypt_olen;
    offset += output->data_len;

    return op_ret;
}
