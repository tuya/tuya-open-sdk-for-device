/**
 * @file example_llm.h
 * @brief 
 */
#ifndef TUYA_EXAMPLE_LLM_H_
#define TUYA_EXAMPLE_LLM_H_

#include "tuya_cloud_types.h"
#include "tuya_list.h"

#define DEFAULT_MAX_HISTORY_CNT     1500
#define DEFAULT_TOKEN_BUFFER_LEN    128
#define DEFAULT_BODY_BUFFER_LEN     4096+512
#define DEFAULT_RESPONSE_BUFFER_LEN 4096+512
#define HTTP_TIMEOUT_MS_DEFAULT     30*1000

typedef INT_T (*LLM_REQUEST_FUNC)(CHAR_T * q, CHAR_T **a);
/**
 * @brief define the large language model type
 * 
 */
typedef enum {
    MODEL_ALI_QWEN = 0,
    MODEL_MOONSHOT_AI = 1,
    MODEL_MAX,
} LLM_type_e;


/**
 * @brief define the big language model config
 * 
 */
typedef struct {
    CHAR_T *token;
    CHAR_T *host;
    CHAR_T *path;
    CHAR_T *model;
} LLM_config_t;

/**
 * @brief 
 * 
 */
typedef struct {
    CHAR_T *q;
    CHAR_T *a;
    INT_T q_size;
    INT_T a_size;
    LIST_HEAD node;
} LLM_conversation_t;

/**
 * @brief define the ai model config
 * 
 */
typedef struct {
    LLM_type_e current;
    LLM_config_t *config;
    
    INT_T his_cnt;
    LIST_HEAD history;
} LLM_t;

/**
 * @brief set current ai model type
 * 
 * @param type 
 * @return INT_T 
 */
INT_T LLM_set_model(LLM_type_e type);

/**
 * @brief get current ai model type
 * 
 * @param type 
 * @return INT_T 
 */
INT_T LLM_get_model(LLM_type_e *type);

/**
 * @brief start a conversation
 * 
 * @param model_config 
 * @param cur_context 
 * @param his_context 
 * @param result 
 * @return INT_T 
 */
INT_T LLM_conversation(CHAR_T *q, CHAR_T *a);

/**
 * @brief clear the ai model to default, clear the history
 * 
 * @return INT_T 
 */
INT_T LLM_reset_model();

#endif