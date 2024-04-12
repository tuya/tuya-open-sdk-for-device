
#include <stdlib.h>
#include "tal_api.h"
#include "llm_demo.h"
#include <sys/stat.h>
#include "audio_asr.h"
#include "audio_tts.h"
#include "tal_cli.h"
#include "netmgr.h"
#include "tal_workq_service.h"
#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
#include "netconn_wifi.h"
#endif
/**
 * @brief send chat information to big language model
 * 
 * @param argc 
 * @param argv 
 */
void connect_cmd(int argc, char *argv[])
{
    if (argc == 3) {
    #if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
        netconn_wifi_info_t wifi_info = {{0}};
        strcpy(wifi_info.ssid, argv[1]);
        strcpy(wifi_info.pswd, argv[2]);
        netmgr_conn_set(NETCONN_WIFI, NETCONN_CMD_SSID_PSWD, &wifi_info);
    #endif
    } else {
        PR_NOTICE("usage: connect <ssid> <password>");
    }
    
    return;
}

VOID_T __chat(VOID_T *data)
{
    CHAR_T *context = (CHAR_T *)data;
    CHAR_T *response = tal_malloc(8192);      
    memset(response, 0, 8192);  
    INT_T rt = LLM_conversation(context, response);    
    if (OPRT_OK == rt) {
        PR_NOTICE("******************************************************");
        PR_NOTICE("q: %s", context);
        PR_NOTICE("------------------------------------------------------");
        PR_NOTICE("a: %s", response);
        PR_NOTICE("******************************************************");
    }

    tal_free(response);

    return;
}
/**
 * @brief send chat information to big language model
 * 
 * @param argc 
 * @param argv 
 */
void chat_cmd(int argc, char *argv[])
{
    netmgr_status_e status = NETMGR_LINK_DOWN;
    netmgr_conn_get(NETCONN_AUTO, NETCONN_CMD_STATUS, &status);
    if (status != NETMGR_LINK_UP) {
        PR_ERR("please connect to the router first!");
        return;
    }
        
    INT_T index = 1;
    INT_T offset = 0;
    CHAR_T *context = tal_malloc(128);
    memset(context, 0, 128);
    for (index = 1; index < argc; index ++) {
        if (offset+strlen(argv[index])>= 128) {
            PR_ERR("context is too long!");
            return;
        }
        offset += sprintf(context+offset, "%s ", argv[index]);
    }

    tal_workq_schedule(WORKQ_SYSTEM, __chat, context);
    return;
}

/**
 * @brief switch the llm 
 * 
 * @param[in/out] argc 
 * @param[in/out] argv 
 */
void switch_cmd(int argc, char *argv[])
{
    if (argc == 2) {
        if (0 == strcmp(argv[1], "ali-qwen")) {
            LLM_set_model(MODEL_ALI_QWEN);
            return;
        } else if (0 == strcmp(argv[1], "moonshot")) {
            LLM_set_model(MODEL_MOONSHOT_AI);
            return;
        }
    } 

    PR_NOTICE("usage: switch ali-qwen/moonshot");
    return;
}

VOID __asr(VOID_T *data)
{
    INT_T fsize = 0;
    UINT8_T *buffer = NULL;
    CHAR_T output_text[128];
    INT_T output_len = 128;
    CHAR_T *file_name = "../../../examples/llm_demo/src/localrec1.wav";
    FILE *fd = fopen(file_name, "rb");
    if (fd) {
        struct stat file_stat;
        if (stat(file_name, &file_stat) == -1) {
            perror("Error getting file status");
            return ;
        }
        fsize = file_stat.st_size;
        PR_DEBUG("input %p, size is %d", fd, fsize);
        buffer = tal_malloc(fsize+10);
        fsize = fread(buffer, sizeof(UINT8_T), fsize, fd);
        fclose(fd);

        asr_request_baidu(ASR_FORMAT_WAV, 8000, 1, buffer, fsize, output_text, &output_len);
        PR_DEBUG("output is %s, size is %d", output_text, output_len);
    } else {
        PR_DEBUG("audio file not found!");
    }

}
/**
 * @brief test asr
 * 
 * @param[in/out] argc 
 * @param[in/out] argv 
 * @return VOID 
 */
VOID asr_cmd(INT_T argc, CHAR_T *argv[])
{
    netmgr_status_e status = NETMGR_LINK_DOWN;
    netmgr_conn_get(NETCONN_AUTO, NETCONN_CMD_STATUS, &status);
    if (status != NETMGR_LINK_UP) {
        PR_ERR("please connect to the router first!");
        return;
    }

    tal_workq_schedule(WORKQ_SYSTEM, __asr, NULL);    
    return;
}

VOID __tts(VOID_T *data)
{
    CHAR_T *context = (CHAR_T *)data;
    tts_request_baidu(TTS_FORMAT_MP3, context, 0, "zh", 5, 5, 5);
    tal_free(context);
    return;
}
/**
 * @brief test tts
 * 
 * @param argc 
 * @param argv 
 */
void tts_cmd(int argc, char *argv[])
{
    netmgr_status_e status = NETMGR_LINK_DOWN;
    netmgr_conn_get(NETCONN_AUTO, NETCONN_CMD_STATUS, &status);
    if (status != NETMGR_LINK_UP) {
        PR_ERR("please connect to the router first!");
        return;
    }

    INT_T index = 1;
    INT_T offset = 0;
    CHAR_T *context = tal_malloc(128);
    memset(context, 0, 128);
    if (argc < 2) {
        PR_ERR("usage: tts <text>");
    } else {
        for (index = 1; index < argc; index ++) {
            if (offset + strlen(argv[index]) >= 128) {
                PR_ERR("context is too long!");
                return;
            }
            offset += sprintf(context+offset, "%s ", argv[index]);
        }
        
        tal_workq_schedule(WORKQ_SYSTEM, __tts, context);
    }

    return;
}

/**
 * @brief 
 * 
 */
void tuya_app_cli_init(void)
{
    LLM_set_model(MODEL_ALI_QWEN);

    STATIC cli_cmd_t cmd [5] = {
        {.name = "connect", .help = "Connect to the router.",           .func = connect_cmd},
        {.name = "chat",    .help = "Chat with large language model.",  .func = chat_cmd},
        {.name = "switch",  .help = "Switch large language model.",     .func = switch_cmd},
        {.name = "asr",     .help = "Test ASR.",                        .func = asr_cmd},
        {.name = "tts",     .help = "Test TTS.",                        .func = tts_cmd},
    };
    tal_cli_cmd_register(cmd, 5);
    PR_NOTICE("******************************************************************************************************************");
    PR_NOTICE("now you can chat with large language model, default is ali-qwen, you can change the model according cli command!");
    PR_NOTICE("");
    PR_NOTICE("TAB for help!");
    PR_NOTICE("******************************************************************************************************************");
    return;
}