/**
 * @file llm_demo.h
 * @brief Header file for the Lightweight Logging Mechanism (LLM) demo,
 * providing interfaces for managing and interacting with large language models
 * (LLM) in Tuya IoT applications.
 *
 * This file declares the structures, enums, and functions used to configure and
 * interact with various large language models (LLM), manage conversation
 * histories, and perform conversations using specified AI models. It includes
 * definitions for model configurations, conversation structures, and the main
 * LLM context. Additionally, it provides prototypes for functions to set the
 * current AI model, start a conversation, and reset the model to its default
 * state.
 *
 * The LLM demo showcases how to integrate and manage AI models within IoT
 * applications, offering a flexible framework for experimenting with different
 * models and optimizing interactions based on the application's requirements.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef TUYA_EXAMPLE_LLM_H_
#define TUYA_EXAMPLE_LLM_H_

#include "tuya_cloud_types.h"
#include "tuya_list.h"

#define DEFAULT_MAX_HISTORY_CNT     1500
#define DEFAULT_TOKEN_BUFFER_LEN    128
#define DEFAULT_BODY_BUFFER_LEN     4096 + 512
#define DEFAULT_RESPONSE_BUFFER_LEN 4096 + 512
#define HTTP_TIMEOUT_MS_DEFAULT     30 * 1000

typedef int (*LLM_REQUEST_FUNC)(char *q, char **a);
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
    char *token;
    char *host;
    char *path;
    char *model;
} LLM_config_t;

/**
 * @brief
 *
 */
typedef struct {
    char *q;
    char *a;
    int q_size;
    int a_size;
    LIST_HEAD node;
} LLM_conversation_t;

/**
 * @brief define the ai model config
 *
 */
typedef struct {
    LLM_type_e current;
    LLM_config_t *config;

    int his_cnt;
    LIST_HEAD history;
} LLM_t;

/**
 * @brief set current ai model type
 *
 * @param type
 * @return int
 */
int LLM_set_model(LLM_type_e type);

/**
 * @brief get current ai model type
 *
 * @param type
 * @return int
 */
int LLM_get_model(LLM_type_e *type);

/**
 * @brief start a conversation
 *
 * @param model_config
 * @param cur_context
 * @param his_context
 * @param result
 * @return int
 */
int LLM_conversation(char *q, char *a);

/**
 * @brief clear the ai model to default, clear the history
 *
 * @return int
 */
int LLM_reset_model();

#endif