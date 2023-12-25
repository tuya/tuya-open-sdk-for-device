
#include <stdlib.h>
#include "tal_api.h"
#include "tuya_iot.h"

extern void tal_kv_cmd(int argc, char *argv[]);
extern void netmgr_cmd(int argc, char *argv[]);

/**
 * @brief switch demo on/off cmd
 * 
 * @param argc 
 * @param argv 
 * @return VOID 
 */
VOID switch_test(INT_T argc, CHAR_T *argv[])
{
    if (argc < 2) {
        PR_INFO("usge: switch <on/off>");
        return;
    } 

    CHAR_T bool_value[128];
    if (0 == strcmp(argv[1], "on")) {
        sprintf(bool_value, "{\"1\": true}");
    } else if (0 == strcmp(argv[1], "off")) {
        sprintf(bool_value, "{\"1\": false}");
    } else {
        PR_INFO("usge: switch <on/off>");
        return;        
    }

    tuya_iot_dp_report_json(tuya_iot_client_get(), bool_value);
}

/**
 * @brief excute system cmd
 * 
 * @param argc 
 * @param argv 
 * @return VOID 
 */
VOID system_cmd(INT_T argc, CHAR_T *argv[])
{
    CHAR_T  cmd[256];

    if (argc < 2) {
        PR_INFO("usge: sys <cmd>");        
        return;
    }

    size_t offset = 0;

    for (int i = 1; i < argc; i++) {
        offset += sprintf(cmd + offset, "%s ", argv[i]);
    }

    PR_DEBUG("system %s", cmd);
    system(cmd);
}

/**
 * @brief get free heap size cmd
 * 
 * @param argc 
 * @param argv 
 */
void mem(int argc, char *argv[])
{
    INT_T free_heap = 0;
    free_heap = tal_system_get_free_heap_size();
    PR_NOTICE("cur free heap: %d", free_heap);
}

/**
 * @brief reset iot to unactive/unregister
 * 
 * @param argc 
 * @param argv 
 */
void reset(int argc, char *argv[])
{
    tuya_iot_reset(tuya_iot_client_get());
}

/**
 * @brief cli cmd list
 * 
 */
static cli_cmd_t s_cli_cmd[] = {
    {
        .name  = "switch",
        .func  = switch_test,
        .help  = "switch test"
    },
    {
        .name  = "kv",
        .func  = tal_kv_cmd,
        .help  = "kv test"
    },
    {
        .name  = "sys",
        .func  = system_cmd,
        .help  = "system  cmd"
    },
    {
        .name  = "reset",
        .func  = reset,
        .help  = "reset iot"
    },
    {
        .name  = "mem",
        .func  = mem,
        .help  = "mem size"
    },
    {
        .name  = "netmgr",
        .func  = netmgr_cmd,
        .help  = "netmgr cmd"
    },
};

/**
 * @brief 
 * 
 */
void tuya_app_cli_init(void)
{
    tal_cli_cmd_register(s_cli_cmd, sizeof(s_cli_cmd)/sizeof(s_cli_cmd[0]));
}