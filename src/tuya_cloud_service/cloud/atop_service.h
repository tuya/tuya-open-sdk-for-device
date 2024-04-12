#ifndef __ATOP_SERVICE_H_
#define __ATOP_SERVICE_H_

#include "atop_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* token;
    const char* product_key;
    const char* firmware_key;
    const char* uuid;
    const char* devid;
    const char* authkey;
    const char* sw_ver;
    const char* pv;
    const char* bv;
    const char* modules;
    const char* feature;
    const char* skill_param;
    const void* user_data;
} tuya_activite_request_t;

typedef enum {
    HTTP_DYNAMIC_CFG_ALL,//all
    HTTP_DYNAMIC_CFG_TZ, //time zone
    HTTP_DYNAMIC_CFG_RATERULE,//rate rule for dp
} HTTP_DYNAMIC_CFG_TYPE;

typedef enum {
    DEV_STATUS_UNKNOWN,
    DEV_STATUS_RESET,
    DEV_STATUS_RESET_FACTORY,
    DEV_STATUS_ENABLE,
} DEV_SYNC_STATUS_E;

int atop_service_activate_request(const tuya_activite_request_t* request, atop_base_response_t* response);

int atop_service_client_reset(const char* id, const char* key);

int atop_service_dynamic_cfg_get_v20(const char* id, const char* key, HTTP_DYNAMIC_CFG_TYPE type, atop_base_response_t* response);

int atop_service_upgrade_info_get_v44(const char* id, const char* key, int channel, atop_base_response_t* response);

int atop_service_upgrade_status_update_v41(const char* id, const char* key, int channel, int status);

int atop_service_version_update_v41(const char* id, const char* key, const char *versions);

int atop_service_auto_upgrade_info_get_v44(const char* id, const char* key, atop_base_response_t* response);

int atop_service_put_rst_log_v10(const char* id, const char* key,const char *rst_buffer);

int atop_service_outdoors_property_upload(const char* id, const char* key, const char *countryCode, const char *phone);

int atop_service_iccid_upload(const char* id, const char* key, const char *iccid);

int atop_service_cache_dp_get(const char* id, const char* key,const char *req_dps, atop_base_response_t* response);

int atop_service_sync_check(const char* id, const char* key, DEV_SYNC_STATUS_E *p_status);

int atop_service_comm_node_enable(const char* id, const char* key);

int atop_service_comm_node_disable(const char* id, const char* key);

#ifdef __cplusplus
}
#endif
#endif
