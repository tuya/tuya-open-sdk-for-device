# OTA

`tkl_ota.c`固件 OTA(Over The Air)，它包含处理固件升级流程的各个阶段：获取固件升级能力、OTA 启动通知、OTA 数据处理、OTA 结束通知。该文件是由涂鸦操作系统自动生成的，并允许开发者在提供的用户定义区域内添加自己的实现。


## API 说明

### tkl_ota_get_ability

```c
OPERATE_RET tkl_ota_get_ability(uint32_t *image_size, TUYA_OTA_TYPE_E *type);
```

#### 功能

获取当前设备的 OTA 能力信息，包括最大的固件镜像大小以及 OTA 类型。

#### 参数

- `image_size`: 输出参数，用于存储固件镜像的最大尺寸。
- `type`: 输出参数，OTA 类型，可以是全量包或者差异包，使用 `TUYA_OTA_FULL` 表示全量包，使用 `TUYA_OTA_DIFF` 表示差分包。
    ```c
        typedef enum {
            TUYA_OTA_FULL        = 1,            ///< AB area switch, full package upgrade
            TUYA_OTA_DIFF        = 2,            ///< fixed area, difference package upgrade
        } TUYA_OTA_TYPE_E;
    ```

#### 返回值

返回值为 `OPRT_OK` 表示成功，其他值表示错误，具体请查阅 `tuya_error_code.h`。

### tkl_ota_start_notify

```c
OPERATE_RET tkl_ota_start_notify(uint32_t image_size, TUYA_OTA_TYPE_E type, TUYA_OTA_PATH_E path);
```

#### 功能

通知 OTA 升级开始，该函数会初始化 OTA 过程的所需变量和状态。

#### 参数

- `image_size`: 输入参数，即将升级的固件镜像大小。
- `type`: 输入参数，OTA 类型。
- `path`: 输入参数，OTA 路径，数据从什么通道传输。
    ```c
        typedef enum {
        TUYA_OTA_PATH_AIR    = 0,            ///< OTA from Wired/Wi-Fi/Cellular/NBIoT
        TUYA_OTA_PATH_UART   = 1,            ///< OTA from uart for MF
        TUYA_OTA_PATH_BLE    = 2,            ///< OTA from BLE protocol for subdev
        TUYA_OTA_PATH_ZIGBEE = 3,            ///< OTA from Zigbee protocol for subdev
        TUYA_OTA_PATH_SEC_A = 4,             ///< OTA from multi-section A
        TUYA_OTA_PATH_SEC_B = 5,             ///< OTA from multi-section B
        TUYA_OTA_PATH_INVALID = 255          ///< OTA from multi-section invalid
        }TUYA_OTA_PATH_E;
    ```
#### 返回值

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

### tkl_ota_data_process

```c
OPERATE_RET tkl_ota_data_process(TUYA_OTA_DATA_T *pack, uint32_t* remain_len);
```

#### 功能

处理接收到的 OTA 数据包，该函数会根据状态不同执行不同的升级操作。

#### 参数

- `pack`: 输入参数，指向 OTA 数据包的指针。
- `remain_len`: 输出参数，指示 OTA 数据包中剩余未处理的数据长度。

#### 返回值

如果成功处理数据包，返回值为 `OPRT_OK`，否则返回错误代码，具体错误信息可查看 `tuya_error_code.h`。

### tkl_ota_end_notify

```c
OPERATE_RET tkl_ota_end_notify(BOOL_T reset);
```

#### 功能

通知 OTA 升级已结束，进行校验和后续处理，如果需要会进行设备重置。

#### 参数

- `reset`: 输入参数，表示 OTA 结束后是否需要重置设备。

#### 返回值

返回值为 `OPRT_OK` 表示操作成功，如果校验失败或其他原因导致失败，返回错误代码。错误详情请参考 `tuya_error_code.h`。

### tkl_ota_get_old_firmware_info

```c
OPERATE_RET tkl_ota_get_old_firmware_info(TUYA_OTA_FIRMWARE_INFO_T **info);
```

#### 功能

获取旧固件信息，本函数通常用于断点续传场景。

#### 参数

- `info`: 输出参数，指向旧固件信息结构体的指针的指针。

#### 返回值

如果成功获取旧固件信息，返回值为 `OPRT_OK`，否则返回错误代码，具体错误信息可查看 `tuya_error_code.h`。