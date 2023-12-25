#include "tuya_endpoint.h"
#include "tal_api.h"

#include "tal_kv.h"


extern int iotdns_cloud_endpoint_get(const char* region, const char* env, tuya_endpoint_t* endpoint);

typedef struct {
    char            region[MAX_LENGTH_REGION + 1];
    char            regist_key[MAX_LENGTH_REGIST + 1];
    tuya_endpoint_t endpoint;
} endpoint_management_t;

static endpoint_management_t endpoint_mgr;

static int tuya_region_regist_key_write( const char* region, const char* regist_key )
{
    if ( NULL == region || NULL == regist_key ) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    /* Write kv storage */
    int ret = 0;
    ret = tal_kv_set("region", (const uint8_t*)region, strlen(region));
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_set region, error:0x%02x", ret);
        return OPRT_KVS_WR_FAIL;
    }

    ret = tal_kv_set("regist_key", (const uint8_t*)regist_key, strlen(regist_key));
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_set regist_key, error:0x%02x", ret);
        return OPRT_KVS_WR_FAIL;
    }

    return OPRT_OK;
}

static int tuya_region_regist_key_read( char* region, char* regist_key )
{
    if ( NULL == region || NULL == regist_key ) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    /* Read the region&env from kv storage */
    int ret = 0;
    size_t len = 0;
    uint8_t *value = NULL;
    ret = tal_kv_get("region", &value, &len);
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_get region fail:0x%02x", ret);
        return OPRT_KVS_RD_FAIL;
    }
    memcpy(region, value, MAX_LENGTH_REGION);
    region[MAX_LENGTH_REGION] = 0;
    tal_kv_free(value);

    ret = tal_kv_get("regist_key", &value, &len);
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_get regist_key fail:0x%02x", ret);
        return OPRT_KVS_RD_FAIL;
    }
    memcpy(regist_key, value, MAX_LENGTH_REGIST);
    regist_key[MAX_LENGTH_REGIST] = 0;
    tal_kv_free(value);

    return OPRT_OK;
}


int tuya_endpoint_cert_get(tuya_endpoint_t* endpoint)
{
    if (NULL == endpoint) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    int ret = tal_kv_get("endpoint.cert", &endpoint->cert, &endpoint->cert_len);
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_get endpoint.cert fail:0x%02x", ret);
    }

    return ret;

}

int tuya_endpoint_cert_set(tuya_endpoint_t* endpoint)
{
    if (NULL == endpoint) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    int ret = tal_kv_set("endpoint.cert", endpoint->cert, endpoint->cert_len);
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_set endpoint.cert fail:0x%02x", ret);
    }

    return ret;
}


int tuya_endpoint_domain_get(tuya_endpoint_t* endpoint)
{
    if (NULL == endpoint) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }
    int ret = 0;

    kv_db_t kvdb[] = {
        {"atop.host", KV_STRING,  endpoint->atop.host, sizeof(endpoint->atop.host)},
        {"atop.port", KV_USHORT, &endpoint->atop.port, sizeof(endpoint->atop.port)},
        {"atop.path", KV_STRING,  endpoint->atop.path, sizeof(endpoint->atop.path)},
        {"mqtt.host", KV_STRING,  endpoint->mqtt.host, sizeof(endpoint->mqtt.host)},
        {"mqtt.port", KV_USHORT, &endpoint->mqtt.port, sizeof(endpoint->mqtt.port)},
    };

    ret = tal_kv_serialize_get("endpoint.domain", kvdb, sizeof(kvdb)/sizeof(kvdb[0]));
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_serialize_set error:%d", ret);
    }

    return ret;
}


int tuya_endpoint_domain_set(tuya_endpoint_t *endpoint)
{
    if (NULL == endpoint ) {
        PR_ERR( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    int ret = 0;

    kv_db_t kvdb[] = {
        {"atop.host", KV_STRING,  endpoint->atop.host, sizeof(endpoint->atop.host)},
        {"atop.port", KV_USHORT, &endpoint->atop.port, sizeof(endpoint->atop.port)},
        {"atop.path", KV_STRING,  endpoint->atop.path, sizeof(endpoint->atop.path)},
        {"mqtt.host", KV_STRING,  endpoint->mqtt.host, sizeof(endpoint->mqtt.host)},
        {"mqtt.port", KV_USHORT, &endpoint->mqtt.port, sizeof(endpoint->mqtt.port)},
    };

    ret = tal_kv_serialize_set("endpoint.domain", kvdb, sizeof(kvdb)/sizeof(kvdb[0]));
    if (ret != OPRT_OK) {
        PR_ERR("tal_kv_serialize_set error:%d", ret);
    }

    return ret;
}

int tuya_endpoint_region_regist_set(const char* region, const char* regist_key)
{
    if (tuya_region_regist_key_write( region, regist_key ) != OPRT_OK) {
        PR_ERR("region_regist_key_write error");
        return OPRT_KVS_WR_FAIL;
    }

    strcpy(endpoint_mgr.region, region);
    strcpy(endpoint_mgr.regist_key, regist_key);
    return OPRT_OK;
}

int tuya_endpoint_remove(void)
{
    tal_kv_del("region");
    tal_kv_del("regist_key");
    tal_kv_del("endpoint.cert");
    tal_kv_del("endpoint.domain");

    return OPRT_OK;
}

int tuya_endpoint_init(void)
{
    int ret;

    /* Read storge region & regist record */
    ret = tuya_region_regist_key_read(endpoint_mgr.region, endpoint_mgr.regist_key);
    PR_INFO("endpoint_mgr.region:%s", endpoint_mgr.region);
    PR_INFO("endpoint_mgr.regist_key:%s", endpoint_mgr.regist_key);

    /* Default online env */
    if (endpoint_mgr.regist_key[0] == 0) {
        strcpy(endpoint_mgr.regist_key, "pro");
    }

    return ret;
}


int tuya_endpoint_update(void)
{
    int ret;

    /* If iotdns has already been called,
     * the allocated certificate memory needs to be released. */
    if (endpoint_mgr.endpoint.cert != NULL) {
        PR_TRACE("Free endpoint already exist cert.");
        tal_free(endpoint_mgr.endpoint.cert);
    }
    /* Try to get the iot-dns domain data */
    ret = iotdns_cloud_endpoint_get(endpoint_mgr.region,
                                    endpoint_mgr.regist_key,
                                    &endpoint_mgr.endpoint);
    
    return ret;
}

int tuya_endpoint_update_auto_region(void)
{
    int ret;

    /* If iotdns has already been called,
     * the allocated certificate memory needs to be released. */
    if (endpoint_mgr.endpoint.cert != NULL) {
        PR_TRACE("Free endpoint already exist cert.");
        tal_free(endpoint_mgr.endpoint.cert);
    }
    /* Try to get the iot-dns domain data */
    ret = iotdns_cloud_endpoint_get(NULL,
                                    endpoint_mgr.regist_key,
                                    &endpoint_mgr.endpoint);
    return ret;
}

const tuya_endpoint_t* tuya_endpoint_get(void)
{
    return (const tuya_endpoint_t*)&endpoint_mgr.endpoint;
}
