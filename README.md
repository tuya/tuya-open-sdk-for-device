# tuya-open-sdk-for-device
English | [简体中文](README_CN.md)

## Overview
tuya-open-sdk-for-device is an open source IoT development framework that supports cross-chip platforms and operating systems. It is designed based on a universal southbound interface and supports communication protocols such as Bluetooth, Wi-Fi, and Ethernet. It provides core functionalities for IoT development, including pairing, activation, control, and upgrading.
The sdk has robust security and compliance capabilities, including device authentication, data encryption, and communication encryption, meeting data compliance requirements in various countries and regions worldwide.

IoT products developed using the tuya-open-sdk-for-device, if utilizing the functionality of the tuya_cloud_service component, can make use of the powerful ecosystem provided by the Tuya APP and cloud services, and achieve interoperability with Power By Tuya devices.

## Getting Start

### Prerequisites
Ubuntu and Debian

```sh
sudo apt-get install lcov cmake-curses-gui build-essential wget git python3 libc6-i386 libsystemd-dev
```

### Clone the repository

```sh
git clone https://github.com/tuya/tuya-open-sdk-for-device.git
```

### update the project

```sh
git submodule update --init
```

### Modifying the Example Project
The `switch_demo` demonstrates a simple, cross-platform, cross-system example of a switch that supports multiple connections. Through the Tuya app and Tuya cloud service, this switch can be remotely controlled.

1. Obtain the correct UUID and authkey:
Refer to the document [https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-Step%205%3A%20Get%20license](https://developer.tuya.com/en/docs/iot-device-dev/application-creation?id=Kbxw7ket3aujc#title-4-Step%205%3A%20Get%20license) to obtain the list of authorization codes, which contains the UUID and authkey for connecting to the Tuya cloud service.

2. Modify the example:
In the `examples/switch_demo/src/tuya_config.h` file, the `TUYA_DEVICE_UUID` and `TUYA_DEVICE_AUTHKEY` macros correspond to the UUID and authkey, please update them correctly according to the UUID and authkey obtained above, and delete the `#error` prompt statement after modification.

### Config the project
configure board and demo

```sh
./configure
```

### compile the project

```sh
mkdir build; cd build; cmake ..

make example
```

### run the demo

```sh
./bin/switch_demo_1.0.0/switch_demo_1.0.0
```

## License
Distributed under the Apache License Version 2.0. For more information, see `LICENSE`.


## Disclaimer and Liability Clause

Users should be clearly aware that this project may contain submodules developed by third parties. These submodules may be updated independently of this project. Considering that the frequency of updates for these submodules is uncontrollable, this project cannot guarantee that these submodules are always the latest version. Therefore, if users encounter problems related to submodules when using this project, it is recommended to update them as needed or submit an issue to this project.

If users decide to use this project for commercial purposes, they should fully recognize the potential functional and security risks involved. In this case, users should bear all responsibility for any functional and security issues, perform comprehensive functional and safety tests to ensure that it meets specific business needs. Our company does not accept any liability for direct, indirect, special, incidental, or punitive damages caused by the user's use of this project or its submodules.