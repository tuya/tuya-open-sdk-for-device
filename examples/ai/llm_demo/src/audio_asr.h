/**
 * @file audio_asr.h
 * @brief Header file for Audio ASR (Automatic Speech Recognition) interface for
 * Baidu ASR service.
 *
 * This header file declares the interface for interacting with Baidu's ASR
 * service, including the enumeration for supported audio file formats and the
 * function prototype for sending audio data to Baidu ASR for speech-to-text
 * conversion. It is designed to be used in conjunction with the audio_asr.c
 * source file.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
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
 * @return int
 */
int asr_request_baidu(ASR_format_e format, int rate, int channel, void *data, int len, char *output_text,
                      int *output_len);
#endif