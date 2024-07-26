/**
 * @file audio_tts.c
 * @brief Text-to-Speech (TTS) interface for Baidu TTS service.
 *
 * This file provides the implementation of functions to interact with Baidu's
 * TTS service, including obtaining access tokens, converting text to speech,
 * and handling the TTS response. It utilizes HTTP requests for communication
 * with Baidu's TTS service and cJSON for parsing the responses. The
 * implementation demonstrates how to use the Tuya IoT SDK's HTTP client
 * interface and memory management functions for TTS feature integration.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "audio_tts.h"
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
int __tts_baidu_get_token(char *token)
{
    int rt = OPRT_OK;
    cJSON *response = NULL;
    uint16_t cacert_len = 0;
    uint8_t *cacert = NULL;
    char *path_buf = NULL;
    size_t path_buf_length = 256;

    /* HTTP Response */
    http_client_response_t http_response = {0};

    /* HTTP path */
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
 * @brief
 *
 * @param format the format of audio file
 * @param text the text to be translated
 * @param voice the voice of tts:0-xiaomei,1-xiaoyu,3-xiaoyao,4-yaya
 * @param lang default is "zh"
 * @param speed the speed of tts:0-15,default is 5
 * @param pitch the pitch of tts:0-15,default is 5
 * @param volume the volume of tts:0-15,default is 5
 * @return int OPRT_OK:success;other:fail
 */
int tts_request_baidu(TTS_format_e format, char *text, int voice, char *lang, int speed, int pitch, int volume)
{
    int rt = OPRT_OK;
    uint16_t cacert_len = 0;
    uint8_t *cacert = NULL;
    char *path_buf = NULL;
    char *body_buf = NULL;
    size_t path_buf_length = 128;

    /* HTTP Response */
    http_client_response_t http_response = {0};

    /* HTTP path */
    path_buf = tal_malloc(path_buf_length);
    TUYA_CHECK_NULL_GOTO(path_buf, err_exit);
    memset(path_buf, 0, path_buf_length);
    snprintf(path_buf, 128, "%s", TTS_BAIDU_SHORT_PATH);

    /* get token */
    char token[128] = {};
    TUYA_CALL_ERR_GOTO(__tts_baidu_get_token(token), err_exit);

    /* make HTTP body */
    size_t body_buf_length = strlen(text) + 512;
    body_buf = tal_malloc(body_buf_length);
    TUYA_CHECK_NULL_GOTO(body_buf, err_exit);
    memset(body_buf, 0, body_buf_length);
    // body_buf_length = sprintf(body_buf, "{\"text\":\"%s\", \"format\":\"%s\",
    // \"voice\":%d, \"speed\":%d, \"pitch\":%d, \"volume\":%d, \"lang\":\"%s\"}",
    // text, format, voice, speed, pitch, volume, lang);
    body_buf_length = sprintf(body_buf, "tex=%s&tok=%s&aue=%d&per=%d&spd=%d&pit=%d&vol=%d&lan=%s&ctp=1&cuid=%s", text,
                              token, format, voice, speed, pitch, volume, lang, BAIDU_CUID);
    PR_DEBUG("https body: %s", body_buf);

    /* HTTP headers */
    http_client_header_t headers[] = {
        {.key = "Content-Type", .value = "application/x-www-form-urlencoded"},
        {.key = "Accept", .value = "*/*"},
    };

    /* HTTPS cert */
    TUYA_CALL_ERR_GOTO(tuya_iotdns_query_domain_certs(TTS_BAIDU_SHORT_URL, &cacert, &cacert_len), err_exit);

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_client_status_t http_status = http_client_request(
        &(const http_client_request_t){.cacert = cacert,
                                       .cacert_len = cacert_len,
                                       .host = TTS_BAIDU_SHORT_URL,
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

    // response.body
    PR_DEBUG("response: %s", http_response.body);

err_exit:
    http_client_free(&http_response);

    if (path_buf)
        tal_free(path_buf);
    if (body_buf)
        tal_free(body_buf);
    if (cacert)
        tal_free(cacert);
    return rt;
}
