#include "tuya_cloud_types.h"
#include "tuya_config_defaults.h"
#include "tuya_endpoint.h"
#include "tal_log.h"
#include "cJSON.h"
#include "mbedtls/base64.h"
#include "tuya_error_code.h"
#include "tal_memory.h"
#include "http_client_interface.h"
#include "tuya_register_center.h"
#include "mix_method.h"

#define IOTDNS_REQUEST_FMT "{\"config\":[{\"key\":\"httpsSelfUrl\",\"need_ca\":true},{\"key\":\"mqttsSelfUrl\",\"need_ca\":true}],\"region\":\"%s\",\"env\":\"%s\"}"
#define IOTDNS_REQUEST_FMT_NOREGION "{\"config\":[{\"key\":\"httpsSelfUrl\",\"need_ca\":true},{\"key\":\"mqttsSelfUrl\",\"need_ca\":true}],\"env\":\"%s\"}"


static int iotdns_response_decode(const uint8_t* input, size_t ilen, tuya_endpoint_t* endport)
{
    cJSON* root = cJSON_Parse((const char *)input);
    if (root == NULL) {
        return OPRT_CJSON_PARSE_ERR;
    }

    if (cJSON_GetObjectItem(root, "httpsSelfUrl") == NULL || \
        cJSON_GetObjectItem(root, "mqttsSelfUrl") == NULL) {
        cJSON_Delete(root);
        return OPRT_CR_CJSON_ERR;
    }

    char* httpsSelfUrl = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "httpsSelfUrl"), "addr")->valuestring;
    char* mqttsSelfUrl = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "mqttsSelfUrl"), "addr")->valuestring;
    char* caArr0 = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "caArr"), 0)->valuestring;
    PR_DEBUG("httpsSelfUrl:%s", httpsSelfUrl);
    PR_DEBUG("mqttsSelfUrl:%s", mqttsSelfUrl);

    /* ATOP url decode */
    int port = 443;
    sscanf(httpsSelfUrl, "https://%64[^/]%16[^\n]", endport->atop.host, endport->atop.path);
    endport->atop.port = (uint16_t)port;
    PR_DEBUG("endport->atop.host = \"%s\"", endport->atop.host);
    PR_DEBUG("endport->atop.port = %d", endport->atop.port);
    PR_DEBUG("endport->atop.path = \"%s\"", endport->atop.path);

    /* MQTT host decode */
    sscanf(mqttsSelfUrl, "%64[^:]:%5d[^\n]", endport->mqtt.host, &port);
    endport->mqtt.port = (uint16_t)port;
    PR_DEBUG("endport->mqtt.host = \"%s\"", endport->mqtt.host);
    PR_DEBUG("endport->mqtt.port = %d", endport->mqtt.port);

    /* cert decode */
    // base64 decode buffer
    size_t caArr0_len = strlen(caArr0);
    size_t buffer_len = caArr0_len * 3 / 4;
    uint8_t* caArr_raw = tal_malloc(buffer_len);
    size_t caArr_raw_len = 0;

    // base64 decode
    if (mbedtls_base64_decode(caArr_raw, buffer_len, &caArr_raw_len, (const uint8_t*)caArr0, caArr0_len) != 0) {
        PR_ERR("base64 decode error");
        tal_free(caArr_raw);
        cJSON_Delete(root);
        return OPRT_COM_ERROR;
    }

    endport->cert = caArr_raw;
    endport->cert_len = caArr_raw_len;

    cJSON_Delete(root);
    return OPRT_OK;
}

static int iotdns_base_request(char *body, char *path, http_client_response_t *http_response)
{
    http_client_status_t http_status;

    /* HTTP headers */
    http_client_header_t headers[] = {
        {.key = "User-Agent",   .value = "TUYA_OPEN_SDK"},
        {.key = "Content-Type", .value = "application/x-www-form-urlencoded;charset=UTF-8"},
    };
    uint8_t headers_count = sizeof(headers)/sizeof(http_client_header_t);

    register_center_t rcs;
    tuya_register_center_get(&rcs);

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_status = http_client_request(
        &(const http_client_request_t) {
            .cacert = rcs.ca_cert,
            .cacert_len = rcs.ca_cert_len,
            .host = rcs.urlx ? rcs.urlx : rcs.url0,
            .port = 443,
            .method = "POST",
            .path = path,
            .headers = headers,
            .headers_count = headers_count,
            .body = (const uint8_t*)body,
            .body_length = strlen(body),
            .timeout_ms = HTTP_TIMEOUT_MS_DEFAULT,
        },
        http_response);

    if (HTTP_CLIENT_SUCCESS != http_status) {
        PR_ERR("http_request_send error:%d", http_status);
        return OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
    }

    return OPRT_OK;
}


int iotdns_cloud_endpoint_get(const char* region, const char* env, tuya_endpoint_t* endpoint)
{
    if (NULL == env || NULL == endpoint) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    http_client_status_t http_status;

    /* POST data buffer */
    size_t body_length = 0;
    char* body_buffer = tal_malloc(128);
    if (NULL == body_buffer) {
        PR_ERR("body_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    if (region) {        
        body_length = sprintf(body_buffer, IOTDNS_REQUEST_FMT, region, env);
    } else {
        body_length = sprintf(body_buffer, IOTDNS_REQUEST_FMT_NOREGION, env);
    }
    PR_DEBUG("out post data len:%d, data:%s", body_length, body_buffer);

    /* HTTP headers */
    http_client_header_t headers[] = {
        {.key = "Content-Type", .value = "application/x-www-form-urlencoded;charset=UTF-8"},
    };
    uint8_t headers_count = sizeof(headers)/sizeof(http_client_header_t);

    /* Response buffer length preview */
    uint8_t* response_buffer = NULL;
    size_t response_buffer_length = 1024 * 6;

    /* response buffer make */
    response_buffer = tal_calloc(1, response_buffer_length);
    if (NULL == response_buffer) {
        PR_ERR("response_buffer malloc fail");
        tal_free(body_buffer);
        return OPRT_MALLOC_FAILED;
    }
    http_client_response_t http_response = {
        .buffer = response_buffer,
        .buffer_length = response_buffer_length
    };

    register_center_t rcs;
    tuya_register_center_get(&rcs);

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_status = http_client_request(
        &(const http_client_request_t){
            .cacert = rcs.ca_cert,
            .cacert_len = rcs.ca_cert_len,
            .host = rcs.urlx ? rcs.urlx : rcs.url0,
            .port = 443,
            .method = "POST",
            .path = "/v2/url_config",
            .headers = headers,
            .headers_count = headers_count,
            .body = (const uint8_t*)body_buffer,
            .body_length = body_length,
            .timeout_ms = HTTP_TIMEOUT_MS_DEFAULT,
        },
        &http_response);

    /* Release http buffer */
    tal_free(body_buffer);

    if (HTTP_CLIENT_SUCCESS != http_status) {
        PR_ERR("http_request_send error:%d", http_status);
		tal_free(response_buffer);
        return OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
    }

    /* Decoded response data */
    rt = iotdns_response_decode(http_response.body, http_response.body_length, endpoint);
    if (region) {
        strcpy(endpoint->region, region);
    }
    tal_free(response_buffer);
    return rt;
}


static int iotdns_query_domain_certs_parser(const uint8_t* input, uint8_t **cacert, uint16_t *cacert_len)
{
    int rt = OPRT_OK;

    cJSON *root = cJSON_Parse((CHAR_T *)input);
    if (NULL == root) {
        PR_ERR("json parse fail. Rev:%s", input);
        return OPRT_CJSON_PARSE_ERR;
    }

    cJSON *errcode = cJSON_GetObjectItem(root, "errcode");
    if (errcode) {
        PR_ERR("errorCode:%s", errcode->valuestring);
        rt = OPRT_COM_ERROR;
        goto __exit;
    }
    cJSON *item = cJSON_GetArrayItem(root, 0);
    if (item == NULL) {
        rt = OPRT_CJSON_GET_ERR;
        goto __exit;
    }
    cJSON *ca = cJSON_GetObjectItem(item, "ca");
    if (item == NULL) {
        rt = OPRT_CJSON_GET_ERR;
        goto __exit;
    }
    *cacert = tal_calloc(1, strlen(ca->valuestring) + 1);
    if (NULL == *cacert) {
        rt = OPRT_MALLOC_FAILED;
        goto __exit;
    }
    *cacert_len = tuya_base64_decode(ca->valuestring, *cacert); 
__exit:
    cJSON_Delete(root);

    return rt;
}

int tuya_iotdns_query_domain_certs(char *url, uint8_t **cacert, uint16_t *cacert_len)
{
    char *p_tmp_url = tal_malloc(strlen(url) + 128);
    if (p_tmp_url == NULL) {
        return OPRT_MALLOC_FAILED;
    }
    strcpy(p_tmp_url, url);

    char *p_search_head = p_tmp_url;
    char *p_tmp = strstr(p_search_head, "://");

    char *p_schema = NULL;
    if (p_tmp != NULL) {
        *p_tmp = 0;
        p_schema = p_search_head;
        p_search_head = p_tmp + strlen("://");
    }
    p_tmp = strstr(p_search_head, "/");
    if (p_tmp != NULL) {
        *p_tmp = 0;
    }
    char *p_host = p_search_head;
    UINT_T port = 443;
    p_tmp = strstr(p_search_head, ":");
    if (p_tmp != NULL) {
        *p_tmp = 0;
        char *p_port = p_tmp + strlen(":");
        port = atoi(p_port);
    }
    PR_DEBUG("query URL:%s and Parse: %s %u", url, p_host, port);

    /* POST data buffer */
    char* body_buffer = tal_malloc(256);
    if (NULL == body_buffer) {
        PR_ERR("body_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }
    sprintf(body_buffer, "[{\"host\":\"%s\", \"port\":%d, \"need_ca\":true}]", p_host, port);
    tal_free(p_tmp_url);

    http_client_response_t http_response;
    
    /* Response buffer length preview */
    uint8_t* response_buffer = NULL;
    size_t response_buffer_length = 1024 * 6;

    /* response buffer make */
    response_buffer = tal_calloc(1, response_buffer_length);
    if (NULL == response_buffer) {
        tal_free(body_buffer);
        PR_ERR("response_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }
    http_response.buffer = response_buffer;
    http_response.buffer_length = response_buffer_length;

    int rt = iotdns_base_request(body_buffer, "/device/dns_query", &http_response);
    tal_free(body_buffer);
    if (OPRT_OK == rt) {
        rt = iotdns_query_domain_certs_parser(http_response.body, cacert, cacert_len);
    }
    tal_free(response_buffer);

    return rt;
}
