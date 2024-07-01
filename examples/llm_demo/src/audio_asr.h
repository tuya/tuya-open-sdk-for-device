/**
 * @file audio_asr.h
 * @brief LLM configuration file
 */

#ifndef AUDIO_ASR_H_
#define AUDIO_ASR_H_

#include "tuya_cloud_types.h"

/**
 * @brief define the audio file format 
 * 
 */
typedef enum {
    ASR_FORMAT_PCM = 0,
    ASR_FORMAT_WAV = 1,
} ASR_format_e;


/**
 * @brief asr from baidu, default is wav format
 * 
 * @param[in] data
 * @param[in] data 
 * @param[in] len 
 * @return INT_T 
 */
INT_T asr_request_baidu(ASR_format_e format, INT_T rate, INT_T channel, VOID_T *data, INT_T len, CHAR_T *output_text, INT_T *output_len);
#endif