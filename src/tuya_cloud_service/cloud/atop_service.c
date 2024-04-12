/**
 * @file atop_service.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2020-10-29
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "tuya_cloud_types.h"
#include "atop_base.h"
#include "tal_log.h"
#include "tuya_error_code.h"
#include "atop_service.h"
#include "tal_time_service.h"
#include "tal_memory.h"

#define ATOP_DEFAULT_POST_BUFFER_LEN (128)

#define CAD_VER "1.0.3"
#define CD_VER "1.0.0"
#define ATTRIBUTE_OTA  (11)

int atop_service_activate_request(const tuya_activite_request_t* request,
                                        atop_base_response_t* response)
{
    if (NULL == request || NULL == response) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    #define ACTIVATE_POST_BUFFER_LEN (255)
    size_t prealloc_size = ACTIVATE_POST_BUFFER_LEN;

    if(request->devid) {
        prealloc_size += strlen(request->devid) + 10;
    }

    if(request->modules) {
        prealloc_size += strlen(request->modules) + 10;
    }

    if(request->feature) {
        prealloc_size += strlen(request->feature) + 10;
    }

    if(request->skill_param) {
        prealloc_size += strlen(request->skill_param) + 10;
    }

    if(request->firmware_key) {
        prealloc_size += strlen(request->firmware_key) + 10;
    }

    char* buffer = tal_malloc(prealloc_size);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }
    uint32_t timestamp = tal_time_get_posix();

    /* activate JSON format */
    size_t offset = 0;

    /* Requires params */
    offset = sprintf(buffer, "{\"token\":\"%s\",\"softVer\":\"%s\",\"productKey\":\"%s\",\"protocolVer\":\"%s\",\"baselineVer\":\"%s\"",
                        request->token, request->sw_ver, request->product_key, request->pv, request->bv);

    /* option params */        
    offset += sprintf(buffer + offset,",\"options\": \"%s", "{");
    if(request->firmware_key && request->firmware_key[0]) {
        offset += sprintf(buffer + offset,"\\\"isFK\\\":true");
    } else {
        offset += sprintf(buffer + offset,"\\\"isFK\\\":false");
    }
    offset += sprintf(buffer + offset,"}\"");

    /* firmware_key */
    if(request->firmware_key && request->firmware_key[0]) {
        offset += sprintf(buffer + offset,",\"productKeyStr\":\"%s\"", request->firmware_key);
    }

    /* Activated atop */
    if(request->devid && strlen(request->devid) > 0) {
        offset += sprintf(buffer + offset,",\"devId\":\"%s\"", request->devid);
    }

    /* modules */
    if(request->modules && strlen(request->modules) > 0) {
        offset += sprintf(buffer + offset,",\"modules\":\"%s\"", request->modules);
    }

    /* feature */
    if(request->feature && strlen(request->feature) > 0) {
        offset += sprintf(buffer + offset,",\"feature\":\"%s\"", request->feature);
    }

    /* skill_param */
    if(request->skill_param && strlen(request->skill_param) > 0) {
        offset += sprintf(buffer + offset,",\"skillParam\":\"%s\"", request->skill_param);
    }

    /* default support device OTA */
    offset += sprintf(buffer + offset,",\"devAttribute\":%u", 1 << ATTRIBUTE_OTA);

    offset += sprintf(buffer + offset,",\"cadVer\":\"%s\",\"cdVer\":\"%s\",\"t\":%d}",
                        CAD_VER, CD_VER, timestamp);

    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .uuid = request->uuid,
        .key = request->authkey,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.active",
        .version = "4.4",
        .data = buffer,
        .datalen = offset,
        .user_data = request->user_data
    };

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }
    return rt;
}

int atop_service_client_reset(const char* id, const char* key)
{
    if (NULL == id || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    #define RESET_POST_BUFFER_LEN 32
    size_t buffer_len = 0;
    char* buffer = tal_malloc(RESET_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, RESET_POST_BUFFER_LEN, "{\"t\":%d}", timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.reset",
        .version = "4.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_dynamic_cfg_get_v20(const char* id, const char* key, HTTP_DYNAMIC_CFG_TYPE type, atop_base_response_t* response)
{
    if (NULL == id || NULL == key || NULL == response) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    /* timestamp */
    uint32_t timestamp = tal_time_get_posix();

    //当前种类较少，后续如果增加，需要动态拼装消息内容
    switch(type){
        case HTTP_DYNAMIC_CFG_TZ:
            snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"type\":\"[\\\"timezone\\\"]\",\"t\":%d}", timestamp);
            break;
        case HTTP_DYNAMIC_CFG_RATERULE:
            snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"type\":\"[\\\"rateRule\\\"]\",\"t\":%d}", timestamp);
            break;
        case HTTP_DYNAMIC_CFG_ALL:
        default:
            snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"type\":\"[\\\"timezone\\\",\\\"rateRule\\\"]\",\"t\":%d}", timestamp);
            break;
    }

    buffer_len = strlen(buffer) + 1;
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.dynamic.config.get",
        .version = "2.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL
    };

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }
    return rt;
}

int atop_service_upgrade_info_get_v44(const char* id, const char* key, int channel, atop_base_response_t* response)
{
    if (NULL == id || NULL == key || NULL == response) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"type\":%d,\"t\":%d}", channel, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.upgrade.get",
        .version = "4.4",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }
    return rt;
}

int atop_service_auto_upgrade_info_get_v44(const char* id, const char* key, atop_base_response_t* response)
{
    if (NULL == id || NULL == key || NULL == response) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"subId\":null,\"t\":%d}", timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.upgrade.silent.get",
        .version = "4.4",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }
    return rt;
}

int atop_service_upgrade_status_update_v41(const char* id, const char* key, int channel, int status)
{
    if (NULL == id || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN,
        "{\"type\":%d,\"upgradeStatus\":%d,\"t\":%d}", channel, status, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.upgrade.status.update",
        .version = "4.1",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_version_update_v41(const char* id, const char* key, const char *versions)
{
    if (NULL == id || NULL == key || NULL == versions) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    #define UPDATE_VERSION_BUFFER_LEN 196
    size_t buffer_len = 0;
    char* buffer = tal_malloc(UPDATE_VERSION_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, UPDATE_VERSION_BUFFER_LEN, "{\"versions\":\"%s\",\"t\":%d}", versions, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.versions.update",
        .version = "4.1",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_put_rst_log_v10(const char* id, const char* key,const char *rst_buffer)
{
    if (NULL == id || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }
    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN,
        "{%s,\"t\":%d}", rst_buffer, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "atop.online.debug.log",
        .version = NULL,
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_outdoors_property_upload(const char* id, const char* key, const char *countryCode, const char *phone)
{
    if (NULL == id || NULL == key || NULL == countryCode || NULL == phone) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    uint32_t timestamp = tal_time_get_posix();

    /* post data */
    #define UPDATE_PROPERTY_BUFFER_LEN 255
    size_t buffer_len = 0;
    char* buffer = tal_malloc(UPDATE_PROPERTY_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    // {"countryCode":"86","phone":"15656065877"}
    buffer_len = snprintf(buffer, UPDATE_PROPERTY_BUFFER_LEN,
        "{\"devId\":\"%s\",\"property\":{\"code\":\"phoneInfo\",\"value\":{\"countryCode\":\"%s\",\"phone\":\"%s\"}},\"t\":%d}",
        id, countryCode, phone, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.trip.outdoors.device.property",
        .version = "1.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_iccid_upload(const char* id, const char* key, const char *iccid)
{
    if (NULL == id || NULL == key || NULL == iccid) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    uint32_t timestamp = tal_time_get_posix();

    /* post data */
    #define UPDATE_PROPERTY_BUFFER_LEN 255
    size_t buffer_len = 0;
    char* buffer = tal_malloc(UPDATE_PROPERTY_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    // {"countryCode":"86","phone":"15656065877"}
    buffer_len = snprintf(buffer, UPDATE_PROPERTY_BUFFER_LEN,
        "{\"metas\":{\"catIccId\":\"%s\"},\"t\":%d}", iccid, timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.meta.save",
        .version = "1.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_sync_check(const char* id, const char* key, DEV_SYNC_STATUS_E *p_status)
{
    if (NULL == id || NULL == key || NULL == p_status) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    uint32_t timestamp = tal_time_get_posix();

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"t\":%d}", timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.info.sync",
        .version = "1.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    cJSON *js_ret = cJSON_GetObjectItem(response.result, "status");
    if (js_ret != NULL && js_ret->type == cJSON_String) {
        if (0 == strcmp("reset_factory", js_ret->valuestring)) {
            *p_status = DEV_STATUS_RESET_FACTORY;
            PR_NOTICE("RESET_FACTORY.");
        } else if (0 == strcmp("reset", js_ret->valuestring)) {
            *p_status = DEV_STATUS_RESET;
            PR_NOTICE("RESET.");
        } else if (0 == strcmp("enable", js_ret->valuestring)) {
            *p_status = DEV_STATUS_ENABLE;
            PR_NOTICE("ENABLE.");
        } else {
            PR_NOTICE("INVALID CODE.");
            rt = OPRT_COM_ERROR;
        }
    } else {
        rt = OPRT_COM_ERROR;
    }

    atop_base_response_free(&response);
    return rt;
}

int atop_service_cache_dp_get(const char* id, const char* key,
						const char *req_dps, atop_base_response_t* response)
{
    if (NULL == id || key == NULL || response == NULL) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    uint32_t timestamp = tal_time_get_posix();
    /* post data */

    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"dps\":[%s],\"t\":%d}",req_dps,timestamp);
    PR_DEBUG("POST JSON:%s", buffer);


    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.dev.dp.get",
        .version = "2.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL,
    };

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, response);
    tal_free(buffer);
    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }
    return rt;
}

int atop_service_comm_node_enable(const char* id, const char* key)
{
    if (NULL == id || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"t\":%d}", timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.comm.node.enable",
        .version = "1.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}

int atop_service_comm_node_disable(const char* id, const char* key)
{
    if (NULL == id || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;

    /* post data */
    size_t buffer_len = 0;
    char* buffer = tal_malloc(ATOP_DEFAULT_POST_BUFFER_LEN);
    if (NULL == buffer) {
        PR_ERR("post buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    uint32_t timestamp = tal_time_get_posix();
    buffer_len = snprintf(buffer, ATOP_DEFAULT_POST_BUFFER_LEN, "{\"t\":%d}", timestamp);
    PR_DEBUG("POST JSON:%s", buffer);

    /* atop_base_request object construct */
    atop_base_request_t atop_request = {
        .devid = id,
        .key = key,
        .path = "/d.json",
        .timestamp = timestamp,
        .api = "tuya.device.comm.node.disable",
        .version = "1.0",
        .data = buffer,
        .datalen = buffer_len,
        .user_data = NULL
    };

    atop_base_response_t response = {0};

    /* ATOP service request send */
    rt = atop_base_request(&atop_request, &response);
    tal_free(buffer);

    bool success = response.success;
    atop_base_response_free(&response);

    if (OPRT_OK != rt) {
        PR_ERR("atop_base_request error:%d", rt);
        return rt;
    }

    if (success == false) {
        return OPRT_COM_ERROR;
    }

    return rt;
}
