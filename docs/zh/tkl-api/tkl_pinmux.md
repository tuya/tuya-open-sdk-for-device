# PINMUX 驱动

## 简要说明

pinmux 用于外设管脚的映射及复用。

## API 描述

### tkl_io_pinmux_config

```c
OPERATE_RET tkl_io_pinmux_config(TUYA_PIN_NAME_E pin, TUYA_PIN_FUNC_E pin_func);
```

- 功能描述:
  - 配置 IO 管脚功能。
- 参数:
  - `pin`: 管脚号。
  - `pin_func`:管脚功能 。
- 返回值:
  - 错误码，参考文件 `tuya_error_code.h`。

### tkl_multi_io_pinmux_config

```c
OPERATE_RET tkl_multi_io_pinmux_config(TUYA_MUL_PIN_CFG_T *cfg, uint16_t num);
```

- 功能描述:

  - pinmux多路配置IO管脚功能。

- 参数:

  - `cfg`: 配置结构体指针 。

    **TUYA_MUL_PIN_CFG_T**

    ```c
    typedef  struct {
        TUYA_PIN_NAME_E pin;			//管脚号
        TUYA_PIN_FUNC_E pin_func;		//管脚功能
    } TUYA_MUL_PIN_CFG_T;
    ```

  - `num`:配置数量。

- 返回值:

  - 错误码，参考文件 `tuya_error_code.h`。

### tkl_io_pin_to_func

```c
int32_t tkl_io_pin_to_func(uint32_t pin, TUYA_PIN_TYPE_E pin_type);
```

- 功能描述:
  - 从IO管脚到功 port 和 channel 号的查询。
- 参数:
  - `pin`: 管脚号。
  - `pin_func`: 查询类型。
- 返回值:
  - 管脚对应 port 和 channel 号。
  - 其中 bit0-bit7 位对应 channel(通道)，bit8-bit15 对应 port(端口)。
  - < 0，未查询到。

## 示例

```c
 tkl_io_pinmux_config(TUYA_IO_PIN_0, TUYA_IIC0_SCL);
 tkl_io_pinmux_config(TUYA_IO_PIN_1, TUYA_IIC0_SDA);
```

```c
 TUYA_MUL_PIN_CFG_T cfg[2];
 cfg[0].pin = TUYA_IO_PIN_0;
 cfg[0].pin_func = TUYA_IIC0_SCL;
 
 cfg[1].pin = TUYA_IO_PIN_1;
 cfg[1].pin_func = TUYA_IIC0_SDA;

 tkl_multi_io_pinmux_config(cfg,2);
```
