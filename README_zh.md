# tuya-open-sdk-for-device
[English](https://github.com/tuya/tuya-open-sdk-for-device/blob/master/README.md) | 简体中文

## 简介
tuya-open-sdk-for-device 是一款跨芯片平台、操作系统的 IoT 开发框架。它基于通用南向接口设计，支持 Bluetooth、Wi-Fi、Ethernet 等通信协议，提供了物联网开发的核心功能，包括配网，激活，控制，升级等；它具备强大的安全合规能力，包括设备认证、数据加密、通信加密等，满足全球各个国家和地区的数据合规需求。

基于 tuya-open-sdk-for-device 开发的 IoT 产品，如果使用 tuya_cloud_service 组件的功能，就可以使用涂鸦APP、云服务提供的强大生态能力，并与 Power By Tuya 设备互联互通。


## 开始体验

### 安装依赖
Ubuntu and Debian

```sh
$ sudo apt-get install lcov cmake-curses-gui build-essential wget git python3 libc6-i386 libsystemd-dev
```

### 克隆仓库

```sh
$ git clone https://github.com/tuya/tuya-open-sdk-for-device.git
```

### 更新工程

```sh
$ git submodule update --init
```

### 修改示例工程
`switch_demo` 演示一个简单的，跨平台、跨系统、支持多种连接的开关示例，通过涂鸦APP、涂鸦云服务，可以对这个开关进行远程控制。

1. 创建产品并获取产品的 PID：

参考文档 [https://developer.tuya.com/cn/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc](https://developer.tuya.com/cn/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc) 在 [https://iot.tuya.com](https://iot.tuya.com) 下创建产品，并获取到创建产品的 PID 。

2. 获取正确的 uuid 和 authkey：

参考文档 [https://developer.tuya.com/cn/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-%E7%AC%AC%E4%BA%94%E6%AD%A5%EF%BC%9A%E9%A2%86%E5%8F%96%E6%8E%88%E6%9D%83%E7%A0%81](https://developer.tuya.com/cn/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-%E7%AC%AC%E4%BA%94%E6%AD%A5%EF%BC%9A%E9%A2%86%E5%8F%96%E6%8E%88%E6%9D%83%E7%A0%81) 获取授权码清单，该清单中包含连接涂鸦云服务的 uuid 和 authkey。

3. 修改示例：

`examples/switch_demo/src/tuya_config.h` 文件中 `TUYA_PRODUCT_KEY` 和 `TUYA_DEVICE_UUID` 、`TUYA_DEVICE_AUTHKEY` 宏分别对应 pid 和 uuid 、authkey，请根据步骤 1 和 2 分别获取到的 PID 及 uuid 、authkey 正确修改，修改后删除 `#error` 提示语句。 

### 配置工程
选择当前编译的目标板和 demo
```sh
$ ./configure
```

### 编译工程

```sh
$ mkdir build; cd build; cmake ..

$ make example
```

### 配置 tuya-open-sdk-for-device
在 build 目录下运行如下命令进行菜单化配置
```sh
$ make menuconfig
```

### 运行示例程序

```sh
$ ./bin/switch_demo_1.0.0/switch_demo_1.0.0
```

## FAQ
1. tuya-open-sdk-for-device 支持的 board 通过子仓库动态下载，更新 tuya-open-sdk-for-device 仓库不会主动更新子仓库，如遇到问题无法正常编译，请至 board 文件夹下对应的目录下使用 `git pull` 命令更新，或删除 board 文件夹下对应目录后再次下载。

2. 通过二维码扫码激活需产品 PID 支持 “设备直连云” 功能，否则激活会报错，导致无法正常激活。
![qrencode](docs/images/zh/qrencode.png)

## License

本项目的分发遵循 Apache License 版本 2.0。有关更多信息，请参见 LICENSE 文件。


## 免责与责任条款

用户应明确知晓，本项目可能包含由第三方开发的子模块（submodules），这些子模块可能独立于本项目进行更新。鉴于这些子模块的更新频率不受控制，本项目无法确保这些子模块始终为最新版本。因此，用户在使用本项目时，若遇到与子模块相关的问题，建议自行根据需要进行更新或于本项目提交问题（issue）。

若用户决定将本项目用于商业目的，应充分认识到其中可能涉及的功能性和安全性风险。在此情况下，用户应对产品的所有功能性和安全性问题承担全部责任，应进行全面的功能和安全测试，以确保其满足特定的商业需求。本公司不对因用户使用本项目或其子模块而造成的任何直接、间接、特殊、偶然或惩罚性损害承担责任。
