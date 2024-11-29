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
```
The tuyeopen repository contains multiple submodules. The tos tool will check and automatically download the submodules before compilation, or you can manually download them using the command `git submodule update --init`.

## Setup and Compilation

### step1. Setting Environment Variables
```sh
$ cd tuyaopen
$ export PATH=$PATH:$PWD
```
Or add the tuyaopen path to your system environment variables.


tuyaopen can be compiled and debugged using the tos command, which will search for the tuyaopen repository based on the path set in the environment variables and execute the corresponding operations.

For detailed usage of the tos command, please refer to [tos command](./docs/en/tos_guide.md).

### Step 2. Set Platform
The tos tool configures the build platform through the `project_build.ini` file located in the project's directory. The `project_build.ini` file includes the following fields:
- project: The name of the project, which can be customized. It is recommended to use the format <project directory name>_<platform/chip name>.
- platform: The target platform for compilation, with available options including: ubuntu, t2, t3, t5, esp32, ln882h, bk7231x. This name should match the name defined in `platform/platform_config.yaml`.
- chip: Optional values; when the selected platform supports multiple chips, the specific chip name must be specified.
    - When the platform is set to esp32, the available options are: esp32, esp32c3.
    - When the platform is set to bk7231x, the available options are: bk7231n.

An example configuration is as follows:
```bash
[project:sample_project_bk7231x]
platform = bk7231x
chip = bk7231n
```

Additionally, the tos tool can configure the simultaneous compilation of projects across multiple platforms via the `project_build.ini` file. For more information, refer to [Multi-platform Configuration](#Multi-platform Configuration).

### step3. Compilation
Select the corresponding project for the current compilation in examples or apps, and then run the following command to compile:
```shell
$ cd examples/get-started/sample_project
$ tos build
```
After compilation, the target files will be located in the `examples/get-started/sample_project/.build/t2/bin/t2_1.0.0` directory.

### Configuration 
To configure the selected examples or apps project, run the following command in the corresponding project directory for menu-driven configuration:
```sh
$ cd examples/get-started/sample_project
$ tos menuconfig
```
Configure the current project, save and exit after configuration, and then compile the project.

## Multi-platform Configuration
The tos tool configures multi-platform compilation through the `project_build.ini` file in the project engineering directory. The [multi-platform configuration file](examples/get-started/sample_project/project_build.ini) format is as follows:
```ini
[project:sample_project_t2]
platform = t2

[project:sample_project_t3]
platform = t3

[project:sample_project_ubuntu]
platform = ubuntu

[project:sample_project_t5]
platform = t5

[project:sample_project_esp32]
platform = esp32
chip = esp32c3      # esp32/esp32c3 Optional
```

By default, there is only 1 project in project. If you need to compile multiple projects, you need to add multiple project configurations in the `project_build.ini` file.

When there are multiple projects in the configuration file, the tos build command will compile multiple projects in sequence.

### Supported platform list
| Name  | Support Status | Introduction | Debug log serial port |
| ---- | ---- | ---- | ---- |
| Ubuntu | Supported  | Can be run directly on Linux hosts such as ubuntu. | |
| T2 |  Supported  | Supported Module List: [T2-U](https://developer.tuya.com/en/docs/iot/T2-U-module-datasheet?id=Kce1tncb80ldq) | Uart2/115200 |
| T3 |  Supported  | Supported Module List: [T3-U](https://developer.tuya.com/en/docs/iot/T3-U-Module-Datasheet?id=Kdd4pzscwf0il) [T3-U-IPEX](https://developer.tuya.com/en/docs/iot/T3-U-IPEX-Module-Datasheet?id=Kdn8r7wgc24pt) [T3-2S](https://developer.tuya.com/en/docs/iot/T3-2S-Module-Datasheet?id=Ke4h1uh9ect1s) [T3-3S](https://developer.tuya.com/en/docs/iot/T3-3S-Module-Datasheet?id=Kdhkyow9fuplc) [T3-E2](https://developer.tuya.com/en/docs/iot/T3-E2-Module-Datasheet?id=Kdirs4kx3uotg) etc. | Uart1/460800 |
| T5 | Supported | Supported Module List: [T5-E1](https://developer.tuya.com/en/docs/iot/T5-E1-Module-Datasheet?id=Kdar6hf0kzmfi) [T5-E1-IPEX](https://developer.tuya.com/en/docs/iot/T5-E1-IPEX-Module-Datasheet?id=Kdskxvxe835tq) etc. | Uart1/460800 |
| ESP32/ESP32C3 | Supported | | Uart0/115200 |
| LN882H | Supported |  | Uart1/921600 |
| BK7231N | Supported | Supported Module List:  [CBU](https://developer.tuya.com/en/docs/iot/cbu-module-datasheet?id=Ka07pykl5dk4u)  [CB3S](https://developer.tuya.com/en/docs/iot/cb3s?id=Kai94mec0s076) [CB3L](https://developer.tuya.com/en/docs/iot/cb3l-module-datasheet?id=Kai51ngmrh3qm) [CB3SE](https://developer.tuya.com/en/docs/iot/CB3SE-Module-Datasheet?id=Kanoiluul7nl2) [CB2S](https://developer.tuya.com/en/docs/iot/cb2s-module-datasheet?id=Kafgfsa2aaypq) [CB2L](https://developer.tuya.com/en/docs/iot/cb2l-module-datasheet?id=Kai2eku1m3pyl) [CB1S](https://developer.tuya.com/en/docs/iot/cb1s-module-datasheet?id=Kaij1abmwyjq2) [CBLC5](https://developer.tuya.com/en/docs/iot/cblc5-module-datasheet?id=Ka07iqyusq1wm) [CBLC9](https://developer.tuya.com/en/docs/iot/cblc9-module-datasheet?id=Ka42cqnj9r0i5) [CB8P](https://developer.tuya.com/en/docs/iot/cb8p-module-datasheet?id=Kahvig14r1yk9) etc. | Uart2/115200 |
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

## Adding and Adapting New Platforms

tuyaopen supports adding and adapting new platforms. For specific operations, please refer to [Adding and Adapting New Platforms](./docs/en/new_platform.md).

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
- Luanode for tuyaopen：[https://github.com/tuya/luanode-tuyaopen](https://github.com/tuya/luanode-tuyaopen)