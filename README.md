# tuya-open-sdk-for-device

## Overview
tuya-open-sdk-embeded-c is an open source IoT development framework that supports cross-chip platforms and operating systems. It is designed based on a universal southbound interface and supports communication protocols such as Bluetooth, Wi-Fi, and Ethernet. It provides core functionalities for IoT development, including pairing, activation, control, and upgrading.
The sdk has robust security and compliance capabilities, including device authentication, data encryption, and communication encryption, meeting data compliance requirements in various countries and regions worldwide.

IoT products developed using the tuya-open-sdk-embeded-c, if utilizing the functionality of the tuya_cloud_service component, can make use of the powerful ecosystem provided by the Tuya APP and cloud services, and achieve interoperability with Power By Tuya devices.

## Getting Start

### Prerequisites
Ubuntu and Debian

```sh
sudo apt-get install lcov cmake-curses-gui
```

### Clone the repository

```sh
git clone https://registry.code.tuya-inc.top/embtuyaoscategory/wifi/open-iot-development-kit.git
```

### update the project

```sh
git submodule update --init
```

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
./bin/switch_demo
```

## License
Distributed under the Apache License Version 2.0. For more information, see `LICENSE`.


## Disclaimer and Liability Clause

Users should be clearly aware that this project may contain submodules developed by third parties. These submodules may be updated independently of this project. Considering that the frequency of updates for these submodules is uncontrollable, this project cannot guarantee that these submodules are always the latest version. Therefore, if users encounter problems related to submodules when using this project, it is recommended to update them as needed or submit an issue to this project.

If users decide to use this project for commercial purposes, they should fully recognize the potential functional and security risks involved. In this case, users should bear all responsibility for any functional and security issues, perform comprehensive functional and safety tests to ensure that it meets specific business needs. Our company does not accept any liability for direct, indirect, special, incidental, or punitive damages caused by the user's use of this project or its submodules.