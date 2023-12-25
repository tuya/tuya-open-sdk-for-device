/**
* @file tuya_tls.h
* @brief Common process - tls include
* @version 0.1
* @date 2019-08-20
*
* @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
*
*/

#ifndef TUYA_TLS_H
#define TUYA_TLS_H

//mbedtls only used to encryption the seesion,not used to create the seesion
#include "tuya_cloud_types.h"
// #include "ssl.h"
// #include "tuya_cert_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *tuya_tls_hander;

typedef enum {
    TSS_INIT = 0,
    TSS_START,
    TSS_ACCEPT,
    TSS_TLS_HAND,
    TSS_TLS_APP,
} TLS_TCP_STAT_E;

typedef VOID(*tuya_tls_pre_conn_cb)(CONST CHAR_T *hostname, CONST tuya_tls_hander p_tls_hander);
typedef INT_T(*tuya_tls_send_cb)(VOID *p_custom_net_ctx, CONST BYTE_T *buf, SIZE_T len);
typedef INT_T(*tuya_tls_recv_cb)(VOID *p_custom_net_ctx, BYTE_T *buf, SIZE_T len);

typedef enum {
    TUYA_TLS_PSK_MODE,
    TUYA_TLS_SERVER_CERT_MODE,
    TUYA_TLS_MUTUAL_CERT_MODE,
    TUYA_TLS_HARDWARE_CERT_MODE,
    // TUYA_TLS_AWS_FFS_CERT_MODE,
} tuya_tls_mode_t;

typedef enum {
    TUYA_TLS_CERT_EXPIRED,
} tuya_tls_event_t;
/**
 * @brief tls event cb
 *
 * @param[in] event event id
 * @param[in] p_args cb args
 *
 */
typedef VOID (*tuya_tls_event_cb)(tuya_tls_event_t event, VOID *p_args);


typedef struct {
    tuya_tls_mode_t     mode;
    char               *hostname;
    uint16_t            port;
    uint32_t            timeout;

    char               *psk_key;
    uint32_t            psk_key_size;
    char               *psk_id;
    int                 psk_id_size;

    bool                verify;
    char               *ca_cert;
    int                 ca_cert_size;

    char               *client_cert;
    int                 client_cert_size;
    char               *client_pkey;
    int                 client_pkey_size;

    size_t              in_content_len;
    size_t              out_content_len;

    tuya_tls_send_cb    f_send;
    tuya_tls_recv_cb    f_recv;
    tuya_tls_event_cb   exception_cb;
    void* user_data;
} tuya_tls_config_t;

/**
 * @brief Get mbedtls random data in the specified length
 *
 * @param output
 * @param output_len
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_tls_random(unsigned char *output, size_t output_len);

/**
 * @brief tls register x509 ca
 *
 * @param[in] p_ctx ca content        
 * @param[in] p_der ca
 * @param[in] der_len ca len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_register_x509_crt_der(VOID *p_ctx, UCHAR_T *p_der, UINT_T der_len);

/**
 * @brief register cb invoked before tls handshake
 *
 * @param[in] pre_conn callback
 */
VOID tuya_tls_register_pre_conn_cb(tuya_tls_pre_conn_cb pre_conn);

/**
 * @brief tls init
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_init();

/**
 * @brief tls hander create
 *
 * @return tuya_tls_hander*
 */
tuya_tls_hander* tuya_tls_connect_create(void);

/**
 * @brief
 *
 * @param[in/out] p_tls_hander
 */
void tuya_tls_connect_destroy(tuya_tls_hander p_tls_hander);

/**
 * @brief
 *
 * @param[in/out] p_tls_handler
 * @param[in/out] config
 * @return OPERATE_RET
 */
OPERATE_RET tuya_tls_config_set(tuya_tls_hander p_tls_handler, tuya_tls_config_t* config);

/**
 * @brief
 *
 * @param[in/out] p_tls_handler
 * @return tuya_tls_config_t*
 */
tuya_tls_config_t* tuya_tls_config_get(tuya_tls_hander p_tls_handler);

/**
 * @brief tls connect
 *
 * @param[in] p_tls_handler refer to tuya_tls_hander
 * @param[in] hostname url
 * @param[in] port_num port
 * @param[in] socket_fd fd
 * @param[in] overtime_s connect timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_connect(tuya_tls_hander p_tls_handler, IN CHAR_T *hostname, IN INT_T port_num,
                             IN INT_T socket_fd, IN INT_T overtime_s);

/**
 * @brief tls write
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 * @param[in] buf write data
 * @param[in] len write length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_write(IN tuya_tls_hander tls_handler, BYTE_T *buf, UINT_T len);

/**
 * @brief tls read
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 * @param[out] buf read data
 * @param[in] len read length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_read(IN tuya_tls_hander tls_handler, BYTE_T *buf, UINT_T len);

/**
 * @brief generated random
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_disconnect(IN tuya_tls_hander tls_handler);

/**
 * @brief
 *
 * @return const tuya_tls_config_t*
 */
const tuya_tls_config_t* tuya_tls_psk_mode_config_get(void);

#ifdef __cplusplus
}

#endif
#endif


