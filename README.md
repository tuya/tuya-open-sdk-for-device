# tuyaopen
English | [简体中文](README_zh.md)

## Overview
tuyaopen is an open source IoT development framework that supports cross-chip platforms and operating systems. It is designed based on a universal southbound interface and supports communication protocols such as Bluetooth, Wi-Fi, and Ethernet. It provides core functionalities for IoT development, including pairing, activation, control, and upgrading.
The sdk has robust security and compliance capabilities, including device authentication, data encryption, and communication encryption, meeting data compliance requirements in various countries and regions worldwide.

IoT products developed using the tuyaopen, if utilizing the functionality of the tuya_cloud_service component, can make use of the powerful ecosystem provided by the Tuya APP and cloud services, and achieve interoperability with Power By Tuya devices.

At the same time, the tuyaopen will continuously expand, providing more cloud platform integration features, as well as voice, video, and facial recognition capabilities.

## Getting Start

### Prerequisites
Ubuntu and Debian

```sh
$ sudo apt-get install lcov cmake-curses-gui build-essential wget git python3 python3-pip python3-venv libc6-i386 libsystemd-dev
```

### Clone the repository

```sh
$ git clone https://github.com/tuya/tuyaopen.git
$ git submodule update --init
```

## Setup and Compilation

### Setting Environment Variables
```sh
$ cd tuyaopen
$ export PATH=$PATH:$PWD
```
Or add the tuyaopen path to your system environment variables.


tuyaopen can be compiled and debugged using the tos command, which will search for the tuyaopen repository based on the path set in the environment variables and execute the corresponding operations.

For detailed usage of the tos command, please refer to [tos command](./docs/en/tos_guide.md).


### Configuration 
To configure the selected examples or apps project, run the following command in the corresponding project directory for menu-driven configuration:
```sh
$ cd examples/get-started/sample_project
$ tos menuconfig
```
Configure the current project, save and exit after configuration, and then compile the project.

### Compilation
Select the corresponding project for the current compilation in examples or apps, and then run the following command to compile:
```shell
$ cd examples/get-started/sample_project
$ tos build
```
After compilation, the target files will be located in the `examples/get-started/sample_project/.build/t2/bin/t2_1.0.0` directory.

## Multi-platform Configuration
The tos tool configures multi-platform compilation through the project_build.ini file in the project engineering directory. The configuration file format is as follows:
```ini
[project:switch_demo_t2]
platform = t2

[project:switch_demo_t3]
platform = t3

[project:switch_demo_ubuntu]
platform = ubuntu

[project:switch_demo_t5]
platform = t5

[project:switch_demo_esp32]
platform = esp32
chip = esp32c3      # esp32/esp32c3 Optional
```

By default, there is only 1 project in project. If you need to compile multiple projects, you need to add multiple project configurations in the project_build.ini file.

When there are multiple projects in the configuration file, the tos build command will compile multiple projects in sequence.

### Supported platform list
| Name  | Support Status | Introduction | Debug log serial port |
| ---- | ---- | ---- | ---- |
| Ubuntu | Supported  | Can be run directly on Linux hosts such as ubuntu. | |
| T2 |  Supported  | [https://developer.tuya.com/en/docs/iot/T2-U-module-datasheet?id=Kce1tncb80ldq](https://developer.tuya.com/en/docs/iot/T2-U-module-datasheet?id=Kce1tncb80ldq) | Uart2/115200 |
| T3 |  Supported  | [https://developer.tuya.com/en/docs/iot/T3-U-Module-Datasheet?id=Kdd4pzscwf0il](https://developer.tuya.com/en/docs/iot/T3-U-Module-Datasheet?id=Kdd4pzscwf0il) | Uart1/468000 |
| T5 | Supported | [https://developer.tuya.com/en/docs/iot/T5-E1-Module-Datasheet?id=Kdar6hf0kzmfi](https://developer.tuya.com/en/docs/iot/T5-E1-Module-Datasheet?id=Kdar6hf0kzmfi) | Uart1/468000 |
| ESP32/ESP32C3 | Supported | | Uart0/115200 |
| raspberry pico-w | In Development, to be released in Nov 2024 | | |

## Sample Projects
The tuyaopen provides a variety of sample projects to facilitate developers in quickly getting started and understanding the usage of the tuyaopen.

```shell
$ tuyaopen
├── ai
│   └── llm_demo
├── ble
│   ├── ble_central
│   └── ble_peripher
├── get-started
│   └── sample_project
├── peripherals
│   ├── adc
│   ├── gpio
│   ├── i2c
│   ├── pwm
│   ├── spi
│   ├── timer
│   └── watchdog
├── protocols
│   ├── http_client
│   ├── mqtt
│   ├── tcp_client
│   └── tcp_server
├── system
│   ├── os_event
│   ├── os_kv
│   ├── os_mutex
│   ├── os_queue
│   ├── os_semaphore
│   ├── os_sw_timer
│   └── os_thread
└── wifi
    ├── ap
    ├── low_power
    ├── scan
    └── sta
```

Each sample project includes a README.md file that provides detailed instructions on configuring, compiling, and running the project.

## FAQ
1. The supported platform for tuyaopen are dynamically downloaded through subrepositories. Updating the tuyaopen repository itself will not automatically update the subrepositories. If you encounter any issues with compilation, please navigate to the corresponding directory in the "platform" folder and use the `git pull` command to update, or delete the corresponding directory in the "platform" folder and download it again.


## License
Distributed under the Apache License Version 2.0. For more information, see `LICENSE`.

## Contribute Code
If you are interested in the tuyaopen and wish to contribute to its development and become a code contributor, please first read the [Contribution Guide](./docs/en/contribute_guide.md).

## Disclaimer and Liability Clause

Users should be clearly aware that this project may contain submodules developed by third parties. These submodules may be updated independently of this project. Considering that the frequency of updates for these submodules is uncontrollable, this project cannot guarantee that these submodules are always the latest version. Therefore, if users encounter problems related to submodules when using this project, it is recommended to update them as needed or submit an issue to this project.

If users decide to use this project for commercial purposes, they should fully recognize the potential functional and security risks involved. In this case, users should bear all responsibility for any functional and security issues, perform comprehensive functional and safety tests to ensure that it meets specific business needs. Our company does not accept any liability for direct, indirect, special, incidental, or punitive damages caused by the user's use of this project or its submodules.

## Related Links
- Arduino for tuyaopen: [https://github.com/tuya/arduino-tuyaopen](https://github.com/tuya/arduino-tuyaopen)