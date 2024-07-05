# tuya-open-sdk-for-device
English | [简体中文](README_zh.md)

## Overview
tuya-open-sdk-for-device is an open source IoT development framework that supports cross-chip platforms and operating systems. It is designed based on a universal southbound interface and supports communication protocols such as Bluetooth, Wi-Fi, and Ethernet. It provides core functionalities for IoT development, including pairing, activation, control, and upgrading.
The sdk has robust security and compliance capabilities, including device authentication, data encryption, and communication encryption, meeting data compliance requirements in various countries and regions worldwide.

IoT products developed using the tuya-open-sdk-for-device, if utilizing the functionality of the tuya_cloud_service component, can make use of the powerful ecosystem provided by the Tuya APP and cloud services, and achieve interoperability with Power By Tuya devices.

## Getting Start

### Prerequisites
Ubuntu and Debian

```sh
$ sudo apt-get install lcov cmake-curses-gui build-essential wget git python3 libc6-i386 libsystemd-dev
```

### Clone the repository

```sh
$ git clone https://github.com/tuya/tuya-open-sdk-for-device.git
$ git submodule update --init
```

## Setup and Compilation

### Setting Environment Variables
```sh
$ cd tuya-open-sdk-for-device
$ export PATH=$PATH:$PWD
```
Or add the tuya-open-sdk-for-device path to your system environment variables.


### Configuration 
To configure the selected examples or apps project, run the following command in the corresponding project directory for menu-driven configuration:
```sh
$ cd examples/get-started/sample_project
$ tos menuconfig
```

### Compilation
Choose the current examples or apps project to compile.
```shell
$ cd examples/get-started/sample_project
$ tos build
```
After compilation, the target files will be located in the `examples/get-started/sample_project/.build/t2/bin/t2_1.0.0` directory.

## Tuya Cloud Applications Project
`switch_demo` demonstrates a simple cross-platform, cross-system switch example that supports multiple connections. Through the Tuya App and Tuya Cloud Service, this switch can be remotely controlled.

1. Create a product and obtain the Product ID (PID):

Refer to the documentation at [https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc](https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc) to create a product on [https://iot.tuya.com](https://iot.tuya.com) and obtain the PID for the created product.

2. Obtain the correct uuid and authkey:

Refer to the documentation at [https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-%E7%AC%AC%E4%BA%94%E6%AD%A5%EF%BC%9A%E9%A2%86%E5%8F%96%E6%8E%88%E6%9D%83%E7%A0%81](https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-%E7%AC%AC%E4%BA%94%E6%AD%A5%EF%BC%9A%E9%A2%86%E5%8F%96%E6%8E%88%E6%9D%83%E7%A0%81) to get the authorization code list which includes the uuid and authkey for connecting to the Tuya Cloud Service.

3. Modify the example:

In the file `apps/tuya_cloud/switch_demo/src/tuya_config.h`, the macros `TUYA_PRODUCT_KEY`, `TUYA_DEVICE_UUID`, and `TUYA_DEVICE_AUTHKEY` correspond to the PID and uuid, authkey respectively. Please correctly modify these according to the PID and uuid, authkey obtained in steps 1 and 2, then delete the `#error` statement.

## FAQ
1. The supported boards for tuya-open-sdk-for-device are dynamically downloaded through subrepositories. Updating the tuya-open-sdk-for-device repository itself will not automatically update the subrepositories. If you encounter any issues with compilation, please navigate to the corresponding directory in the "board" folder and use the `git pull` command to update, or delete the corresponding directory in the "board" folder and download it again.

2. Activation through QR code scanning requires the product PID to support the "Device Direct Cloud" feature. Otherwise, activation will result in an error and prevent normal activation.

![qrencode](docs/images/en/qrencode.png)


## License
Distributed under the Apache License Version 2.0. For more information, see `LICENSE`.


## Disclaimer and Liability Clause

Users should be clearly aware that this project may contain submodules developed by third parties. These submodules may be updated independently of this project. Considering that the frequency of updates for these submodules is uncontrollable, this project cannot guarantee that these submodules are always the latest version. Therefore, if users encounter problems related to submodules when using this project, it is recommended to update them as needed or submit an issue to this project.

If users decide to use this project for commercial purposes, they should fully recognize the potential functional and security risks involved. In this case, users should bear all responsibility for any functional and security issues, perform comprehensive functional and safety tests to ensure that it meets specific business needs. Our company does not accept any liability for direct, indirect, special, incidental, or punitive damages caused by the user's use of this project or its submodules.