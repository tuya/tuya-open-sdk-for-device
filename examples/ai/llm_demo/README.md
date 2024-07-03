# llm_demo

## Overview
This is a simple, cross-platform, and multi-connection example of a large language model (LLM). It demonstrates how to interact with the language model on embedded systems, including both text and voice interactions (which require hardware support).

## Directory
```
+- llm_demo
    +- src
        -- cli_cmd.c
        -- audio_asr.c
        -- audio_asr.h
        -- audio_tts.c
        -- audio_tts.h
        -- llm_demo.c
        -- llm_demo.h
        -- llm_config.h
    -- CMakeLists.txt
    -- README_CN.md
    -- README.md
```

### src
* audio_asr.c: the audio asr demo, used to recognize the audio and get the result
* audio_asr.h: the audio asr demo header file
* audio_tts.c: the audio tts demo, used to translate text to the audio
* audio_tts.h: the audio tts demo header file
* llm_demo.c: the llm demo, used to get the result from the llm model
* llm_demo.h: the llm demo header file
* llm_config.h: the llm model configuration file, now can support ali-qwen and moonshot AI
* cli_cmd.c: cli cmmand which used to operater the demos


### CMakeLists.txt
* CMakeLists.txt: the cmake file


### README_CN.md & README.md
* [README_CN.md](./README_CN.md): 中文README文件
* README.md: the English README file


## Usage

### 1. modify the llm_config.h
modify the llm_config.h, change the token, API-key according the comment.

### 2. Build
configure and build the llm_demo project flowing the [README.md](../../README.md)


### 3. Run

* run the llm_demo on the Linux
```sh
./bin/llm_demo_1.0.0/llm_demo_1.0.0
```

* run the llm_demo on the T2

1, program the image to the T2 board using the [tyutool](../../board/t2/tools/tyutool/README.md)

2, reboot the T2 board

3, connect the T2 board with serial tool with /dev/ttyACM0


### 4. chat wit the llm model

* chat with the llm model on Linux

    1, use "chat" to send message and got the response

    2, use "switch" to switch the llm model

    3, use "asr" to transate audio to text

    4, use "tts" to convert text to audio

* chat with the llm model on T2

    1, use "connect" to connect the T2 board to router

    1, use "chat" to send message and got the response

    2, use "switch" to switch the llm model

    3, use "asr" to transate audio to text

    4, use "tts" to convert text to audio
