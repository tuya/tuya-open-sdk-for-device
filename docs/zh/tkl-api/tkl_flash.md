# Flash Driver

`tkl_flash.c` 文件是由 TuyaOS v&v 工具自动生成的，提供了针对 Flash 存储的基本操作功能，包括读取、写入、擦除 Flash、获取 Flash 信息等。在自动生成的代码框架中，开发者可以在 "BEGIN" 与 "END" 注释之间添加自己的实现来保持用户代码的持久化。

## API 说明

### tkl_flash_read

```c
OPERATE_RET tkl_flash_read(uint32_t addr, uint8_t *dst, uint32_t size);
```

#### 功能

从Flash中读取数据。

#### 参数

- `addr`: 要读取的 Flash 地址。
- `dst`: 存储读取数据的缓冲区指针。
- `size`: 要读取的数据大小。

#### 返回值

- `OPRT_OK`: 读取成功。
- 其他错误码: 请参见 `tuya_error_code.h`。

### tkl_flash_write

```c
OPERATE_RET tkl_flash_write(uint32_t addr, const uint8_t *src, uint32_t size);
```

#### 功能

向Flash写入数据。

#### 参数

- `addr`: 要写入的 Flash 地址。
- `src`: 包含要写入数据的缓冲区指针。
- `size`: 要写入的数据大小。

#### 返回值

- `OPRT_OK`: 写入成功。
- 其他错误码: 请参见 `tuya_error_code.h`。

### tkl_flash_erase

```c
OPERATE_RET tkl_flash_erase(uint32_t addr, uint32_t size);
```

#### 功能

擦除 Flash 中的数据。

#### 参数

- `addr`: 要擦除的 Flash 地址。
- `size`: 要擦除的 Flash 块大小。

#### 返回值

- `OPRT_OK`: 擦除成功。
- 其他错误码: 请参见 `tuya_error_code.h`。

### tkl_flash_lock/unlock

```c
OPERATE_RET tkl_flash_lock(uint32_t addr, uint32_t size);
OPERATE_RET tkl_flash_unlock(uint32_t addr, uint32_t size);
```

#### 功能

锁定或解锁 Flash 的一块区域。这些函数当前实现为未支持操作。

#### 参数

- `addr`: 开始锁定或解锁的 Flash 地址。
- `size`: 锁定或解锁区域的大小。

#### 返回值

- `OPRT_OK`: 锁定或解锁成功。
- 其他错误码: 请参见 `tuya_error_code.h`。

### tkl_flash_get_one_type_info

```c
OPERATE_RET tkl_flash_get_one_type_info(TUYA_FLASH_TYPE_E type, TUYA_FLASH_BASE_INFO_T* info);
```

#### 功能

获取指定Flash类型的信息。

#### 参数

- `type`: 要查询信息的 Flash 类型。
- `info`: 存储 Flash 信息的结构体指针，一般是某个存储类型在 Flash 里的布局。

#### 返回值

- `OPRT_OK`: 成功获取信息。
- 其他错误码: 请参见 `tuya_error_code.h`。

#### 注意事项

- `TUYA_FLASH_TYPE_E` 是 Flash 类型的枚举。
- `TUYA_FLASH_BASE_INFO_T` 是 Flash 信息的结构体。
