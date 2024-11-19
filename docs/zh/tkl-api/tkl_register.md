# REGISTER 驱动

## 简要说明

register 驱动是涂鸦统一的寄存器操作接口，用于对芯片内部寄存器的读取与写入。

## API 描述

### tkl_reg_read

```c
uint32_t tkl_reg_read(uint32_t addr);
```

- 功能描述:
  - 读取寄存器数据。
- 参数:
  - `addr`: 寄存器地址。
- 返回值:
  - 寄存器数据。

### tkl_reg_bit_read

```c
uint32_t tkl_reg_bit_read(uint32_t addr, TUYA_ADDR_BITS_DEF_E start_bit, TUYA_ADDR_BITS_DEF_E end_bit);
```

- 功能描述:
  - 读取寄存器对应 bit 的数据。
- 参数:
  - `addr`: 寄存器地址。
  - `start_bit`: 寄存器起始 bit 位，参考 TUYA_ADDR_BITS_DEF_E 定义。
  - `end_bit`: 寄存器结束 bit 位，参考 TUYA_ADDR_BITS_DEF_E 定义。
- 返回值:
  - 寄存器对应bit位的数据。

### tkl_reg_write

```c
OPERATE_RET tkl_reg_write(uint32_t addr, uint32_t data);
```

- 功能描述:
  - 写入寄存器数据。
- 参数:
  - `addr`: 写入的寄存器地址。
  - `data`: 要写入的数据。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_reg_bit_write

```c
OPERATE_RET tkl_reg_bit_write(uint32_t addr, TUYA_ADDR_BITS_DEF_E start_bit, TUYA_ADDR_BITS_DEF_E end_bit, uint32_t data);
```

- 功能描述:
  - 写入寄存器数据。
- 参数:
  - `addr`: 写入的寄存器地址。
  - `start_bit`: 寄存器起始 bit 位，参考 TUYA_ADDR_BITS_DEF_E 定义。
  - `end_bit`: 寄存器结束 bit 位，参考 TUYA_ADDR_BITS_DEF_E 定义。
  - `data`: 要写入的数据。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。
