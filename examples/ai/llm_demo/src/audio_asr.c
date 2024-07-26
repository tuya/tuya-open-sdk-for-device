/**
 * @file audio_asr.c
 * @brief Audio ASR (Automatic Speech Recognition) interface for Baidu ASR
 * service.
 *
 * This file provides the implementation of functions to interact with Baidu's
 * ASR service, including obtaining access tokens and sending audio data for
 * speech-to-text conversion. It utilizes HTTP requests for communication and
 * cJSON for parsing the responses.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "audio_asr.h"
#include "cJSON.h"
#include "http_client_interface.h"
#include "iotdns.h"
#include "llm_config.h"
#include "mix_method.h"
#include "tal_log.h"
#include "tal_memory.h"
#include "tal_system.h"
#include "tuya_error_code.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * @brief get token
 *
 * @param[in/out] token
 * @return int
 */
int __asr_baidu_get_token(char *token)
{
    int rt = OPRT_OK;
    cJSON *response = NULL;
    uint16_t cacert_len = 0;
    uint8_t *cacert = NULL;
    char *path_buf = NULL;
    size_t path_buf_length = 256;

    /* HTTP Response */
    http_client_response_t http_response = {0};

    path_buf = tal_malloc(path_buf_length);
    TUYA_CHECK_NULL_GOTO(path_buf, err_exit);
    memset(path_buf, 0, path_buf_length);
    snprintf(path_buf, 256, "%s?client_id=%s&client_secret=%s&grant_type=client_credentials", ASR_BAIDU_TOKEN_PATH,
             ASR_BAIDU_CLIENTID, ASR_BAIDU_CLIENT_SECURET);

    /* make HTTP body */
    char body_buf[8] = {0};
    snprintf(body_buf, 8, "{}");

    /* HTTP headers */
    http_client_header_t headers[] = {{.key = "Content-Type", .value = "application/json"}};

    /* HTTPS cert */
    TUYA_CALL_ERR_GOTO(tuya_iotdns_query_domain_certs(BAIDU_TOKEN_URL, &cacert, &cacert_len), err_exit);

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_client_status_t http_status = http_client_request(
        &(const http_client_request_t){.cacert = cacert,
                                       .cacert_len = cacert_len,
                                       .host = BAIDU_TOKEN_URL,
                                       .port = 443,
                                       .method = "POST",
                                       .path = path_buf,
                                       .headers = headers,
                                       .headers_count = sizeof(headers) / sizeof(http_client_header_t),
                                       .body = (uint8_t *)body_buf,
                                       .body_length = strlen(body_buf),
                                       .timeout_ms = LLM_HTTP_REQUEST_TIMEOUT},
        &http_response);

    if (HTTP_CLIENT_SUCCESS != http_status) {
        PR_ERR("http_request_send error:%d", http_status);
        rt = OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
        goto err_exit;
    }

    response = cJSON_Parse((char *)http_response.body);
    if (response) {
        PR_DEBUG("response: %s", cJSON_PrintUnformatted(response));
        strcpy(token, cJSON_GetObjectItem(response, "access_token")->valuestring);
        cJSON_Delete(response);
    }

err_exit:
    http_client_free(&http_response);

    if (path_buf)
        tal_free(path_buf);
    if (cacert)
        tal_free(cacert);
    return rt;
}

/**
 * @brief asr request to baidu
 *
 * @param format audio format, wav or pcm
 * @param rate the sample rate of the audio
 * @param channel the number of channels of the audio
 * @param data the audio data
 * @param len the length of the audio data
 * @param output_text the output text
 * @param output_len the length of the output text
 * @return int OPRT_OK match success, other match failed
 */
int asr_request_baidu(ASR_format_e format, int rate, int channel, void *data, int len, char *output_text,
                      int *output_len)
{
    int rt = OPRT_OK;
    uint16_t cacert_len = 0;
    uint8_t *cacert = NULL;
    char *path_buf = NULL;
    char *base64_data = NULL;
    char *body_buf = NULL;
    size_t path_buf_length = 128;
    int array_size = 0;
    int index = 0;
    int offset = 0;
    cJSON *response = NULL;

    /* HTTP Response */
    http_client_response_t http_response = {0};

    /* HTTPS path */
    path_buf = tal_malloc(path_buf_length);
    TUYA_CHECK_NULL_GOTO(path_buf, err_exit);
    memset(path_buf, 0, path_buf_length);
    snprintf(path_buf, 128, "%s", ASR_BAIDU_PATH);

    /* get token */
    char token[128] = {};
    TUYA_CALL_ERR_GOTO(__asr_baidu_get_token(token), err_exit);

    /* data base64 encode*/
    base64_data = tal_malloc(len / 3 * 4 + 5);
    TUYA_CHECK_NULL_GOTO(base64_data, err_exit);
    memset(base64_data, 0, len / 3 * 4 + 5);
    tuya_base64_encode(data, base64_data, len);

    /* make HTTP body */
    size_t body_buf_length = len / 3 * 4 + 5 + 512;
    body_buf = tal_malloc(body_buf_length);
    TUYA_CHECK_NULL_GOTO(body_buf, err_exit);
    memset(body_buf, 0, body_buf_length);
    body_buf_length =
        sprintf(body_buf,
                "{\"format\":\"%s\", \"cuid\":\"%s\", \"token\":\"%s\", "
                "\"rate\":%d, \"channel\":%d, \"len\":%d, \"speech\":\"%s\"}",
                format == ASR_FORMAT_PCM ? "pcm" : "wav", BAIDU_CUID, token, rate, channel, len, base64_data);
    PR_DEBUG("https body: %s", body_buf);

    /* HTTP headers */
    http_client_header_t headers[] = {{.key = "Content-Type", .value = "application/json"}};

    /* HTTPS cert */
    TUYA_CALL_ERR_GOTO(tuya_iotdns_query_domain_certs(BAIDU_TOKEN_URL, &cacert, &cacert_len), err_exit);

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_client_status_t http_status = http_client_request(
        &(const http_client_request_t){.cacert = cacert,
                                       .cacert_len = cacert_len,
                                       .host = ASR_BAIDU_URL,
                                       .port = 443,
                                       .method = "POST",
                                       .path = path_buf,
                                       .headers = headers,
                                       .headers_count = sizeof(headers) / sizeof(http_client_header_t),
                                       .body = (uint8_t *)body_buf,
                                       .body_length = body_buf_length,
                                       .timeout_ms = LLM_HTTP_REQUEST_TIMEOUT},
        &http_response);

    if (HTTP_CLIENT_SUCCESS != http_status) {
        PR_ERR("http_request_send error:%d", http_status);
        rt = OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
        goto err_exit;
    }

    response = cJSON_Parse((char *)http_response.body);
    if (response) {
        PR_DEBUG("result: %s", cJSON_PrintUnformatted(response));
        cJSON *result_array = cJSON_GetObjectItem(response, "result");
        array_size = cJSON_GetArraySize(result_array);
        for (index = 0; index < array_size; index++) {
            if (offset > 0) {
                offset += sprintf(output_text + offset, ",");
            }

            if (*output_len > offset + strlen(cJSON_GetArrayItem(result_array, index)->valuestring)) {
                offset += sprintf(output_text + offset, "%s", cJSON_GetArrayItem(result_array, index)->valuestring);
            } else {
                PR_ERR("asr len excced!");
                break;
            }
        }

        *output_len = offset;
        cJSON_Delete(response);
    }

err_exit:
    http_client_free(&http_response);

    if (path_buf)
        tal_free(path_buf);
    if (body_buf)
        tal_free(body_buf);
    if (cacert)
        tal_free(cacert);
    if (base64_data)
        tal_free(base64_data);

    return rt;
}
