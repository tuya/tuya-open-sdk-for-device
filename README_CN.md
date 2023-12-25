# tuya-open-sdk-for-device

## 简介
tuya-open-sdk-for-device 是一款跨芯片平台、操作系统的 IoT 开发框架。它基于通用南向接口设计，支持 Bluetooth、Wi-Fi、Ethernet 等通信协议，提供了物联网开发的核心功能，包括配网，激活，控制，升级等；它具备强大的安全合规能力，包括设备认证、数据加密、通信加密等，满足全球各个国家和地区的数据合规需求。

基于 tuya-open-sdk-for-device 开发的 IoT 产品，如果使用 tuya_cloud_service 组件的功能，就可以使用涂鸦APP、云服务提供的强大生态能力，并与 Power By Tuya 设备互联互通。


## 开始体验

### 安装依赖
Ubuntu and Debian

```sh
sudo apt-get install lcov cmake-curses-gui build-essential wget git python3 libc6-i386 libsystemd-dev
```

### 克隆仓库

```sh
git clone https://github.com/tuya/tuya-open-sdk-for-device.git
```

### 更新工程

```sh
git submodule update --init
```

### 配置工程

```sh
./configure
```

### 编译工程

```sh
mkdir build; cd build; cmake ..

make example
```

### 运行示例程序

```sh
./bin/switch_demo_1.0.0/switch_demo_1.0.0
```

## License

本项目的分发遵循 Apache License 版本 2.0。有关更多信息，请参见 LICENSE 文件。


## 免责与责任条款

用户应明确知晓，本项目可能包含由第三方开发的子模块（submodules），这些子模块可能独立于本项目进行更新。鉴于这些子模块的更新频率不受控制，本项目无法确保这些子模块始终为最新版本。因此，用户在使用本项目时，若遇到与子模块相关的问题，建议自行根据需要进行更新或于本项目提交问题（issue）。

若用户决定将本项目用于商业目的，应充分认识到其中可能涉及的功能性和安全性风险。在此情况下，用户应对产品的所有功能性和安全性问题承担全部责任，应进行全面的功能和安全测试，以确保其满足特定的商业需求。本公司不对因用户使用本项目或其子模块而造成的任何直接、间接、特殊、偶然或惩罚性损害承担责任。