/**
 * @file audio_tts.h
 * @brief Header file for Text-to-Speech (TTS) interface for Baidu TTS service.
 *
 * This header file defines the interface for the Text-to-Speech (TTS) service
 * provided by Baidu, including the enumeration for supported audio file formats
 * and the function prototype for converting text to speech using Baidu's TTS
 * service. It outlines the parameters required for making a TTS request, such
 * as the audio format, text to convert, voice type, language, speed, pitch, and
 * volume. This interface is designed to be used in IoT projects that require
 * TTS capabilities, facilitating the integration of Baidu's TTS service into
 * Tuya IoT applications.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef AUDIO_TTS_H_
#define AUDIO_TTS_H_

#include "tuya_cloud_types.h"

/**
 * @brief define the audio file format
 *
 */
typedef enum {
    TTS_FORMAT_MP3 = 3,
    TTS_FORMAT_PCM8K = 4,
    TTS_FORMAT_PCM16K = 5,
    TTS_FORMAT_WAV = 6,
} TTS_format_e;

// #define TTS_MP316K "mp3-16k"
// #define TTS_MP348K "mp3-48k"
// #define TTS_WAV "wav"
// #define TTS_PCM8K "pcm-8k"
// #define TTS_PCM16K "pcm-16k"

/**
 * @brief
 *
 * @param format the format of audio file
 * @param text the text to be translated
 * @param voice the voice of tts:0-xiaomei,1-xiaoyu,3-xiaoyao,4-yaya
 * @param lang default is "zh"
 * @param speed the speed of tts:0-15,default is 5
 * @param pitch the pitch of tts:0-15,default is 5
 * @param volume the volume of tts:0-15,default is 5
 * @return int OPRT_OK:success;other:fail
 */
int tts_request_baidu(TTS_format_e format, char *text, int voice, char *lang, int speed, int pitch, int volume);
#endif