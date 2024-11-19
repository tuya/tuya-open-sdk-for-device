```markdown
# PINMUX Driver

## Brief Description

Pinmux is used for mapping and multiplexing peripheral pins.

## API Description

### tkl_io_pinmux_config

```c
OPERATE_RET tkl_io_pinmux_config(TUYA_PIN_NAME_E pin, TUYA_PIN_FUNC_E pin_func);
```

- **Function Description:**
  - Configure the function of an IO pin.
- **Parameters:**
  - `pin`: Pin number.
  - `pin_func`: Pin function.
- **Return Value:**
  - Error code, refer to the file `tuya_error_code.h`.

### tkl_multi_io_pinmux_config

```c
OPERATE_RET tkl_multi_io_pinmux_config(TUYA_MUL_PIN_CFG_T *cfg, uint16_t num);
```

- **Function Description:**
  - Multiplexed configuration of multiple IO pin functions using pinmux.
- **Parameters:**
  - `cfg`: Pointer to the configuration structure.

    **TUYA_MUL_PIN_CFG_T**

    ```c
    typedef  struct {
        TUYA_PIN_NAME_E pin;           // Pin number
        TUYA_PIN_FUNC_E pin_func;       // Pin function
    } TUYA_MUL_PIN_CFG_T;
    ```

  - `num`: Number of configurations.
- **Return Value:**
  - Error code, refer to the file `tuya_error_code.h`.

### tkl_io_pin_to_func

```c
int32_t tkl_io_pin_to_func(uint32_t pin, TUYA_PIN_TYPE_E pin_type);
```

- **Function Description:**
  - Query the port and channel numbers from an IO pin.
- **Parameters:**
  - `pin`: Pin number.
  - `pin_type`: Query type.
- **Return Value:**
  - The port and channel numbers corresponding to the pin.
  - Bits 0-7 correspond to the channel, and bits 8-15 correspond to the port.
  - < 0 indicates not found.

## Examples

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

tkl_multi_io_pinmux_config(cfg, 2);
```
```