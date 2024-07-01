# board

## Overview

The board_config.yaml file contains information about SDK repositories that have implemented the universal southbound interface. After completing the project configuration use `./configure`, the repository you select will be clone to the boards directory automatically.


## porting board to tuya-open-sdk-for-device

If you want to create your own board, please refer to [TuyaOS Porting](https://developer.tuya.com/cn/docs/iot-device-dev/TuyaOS-translation?id=Kcrwpd4fqalay).

After finished the porting, add the repository into board_config.yaml, and run `./configre` to select.
