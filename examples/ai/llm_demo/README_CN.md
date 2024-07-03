# llm_demo

## 概览
这是一个简单的跨平台、多连接的大型语言模型（LLM）示例。它展示了如何在嵌入式系统上与语言模型进行交互，包括文本和语音交互（需要硬件支持）。

## 目录结构
```
└── llm_demo
    ├── src
    │   ├── cli_cmd.c
    │   ├── audio_asr.c
    │   ├── audio_asr.h
    │   ├── audio_tts.c
    │   ├── audio_tts.h
    │   ├── llm_demo.c
    │   ├── llm_demo.h
    │   ├── llm_config.h
    │   └── README_CN.md
    ├── CMakeLists.txt
    └── README.md
```

### src
- `audio_asr.c`: 音频自动识别（ASR）演示，用于识别音频并获取结果
- `audio_asr.h`: 音频ASR演示的头文件
- `audio_tts.c`: 音频文本转语音（TTS）演示，用于将文本转换为音频
- `audio_tts.h`: 音频TTS演示的头文件
- `llm_demo.c`: LLM演示，用于从LLM模型获取结果
- `llm_demo.h`: LLM演示的头文件
- `llm_config.h`: LLM模型配置文件，目前支持ali-qwen和moonshot AI
- `cli_cmd.c`: 用于操作演示的CLI命令

### CMakeLists.txt
- `CMakeLists.txt`: CMake配置文件

### README_CN.md & README.md
- [README_CN.md](./README_CN.md): 中文README文件
- README.md: 英文README文件

## 使用方法

### 1. 修改 llm_config.h
根据注释修改llm_config.h，更改token和API-key。

### 2. 构建
按照[README.md](../../README.md)配置和构建llm_demo项目

### 3. 运行

在Linux上运行llm_demo
```sh
./bin/llm_demo_1.0.0/llm_demo_1.0.0
```

在T2上运行llm_demo

1. 使用[tyutool](../../board/t2/tools/tyutool/README.md)工具将镜像编程到T2开发板
2. 重启T2板
3. 使用串行工具通过`/dev/ttyACM0`连接T2开发板

### 4. 与LLM模型聊天

在Linux上与LLM模型聊天
1. 使用"chat"发送消息并获取响应
2. 使用"switch"切换LLM模型
3. 使用"asr"将音频转换为文本
4. 使用"tts"将文本转换为音频

在T2上与LLM模型聊天
1. 使用"connect"将T2板连接到路由器
2. 使用"chat"发送消息并获取响应
3. 使用"switch"切换LLM模型
4. 使用"asr"将音频转换为文本
5. 使用"tts"将文本转换为音频