# WAKEUP Driver

## Brief Description

The wakeup driver is used to set the wake-up source for the chip. The wake-up source can be GPIO, timer, etc.

## API Description

### tkl_wakeup_source_set

```c
OPERATE_RET tkl_wakeup_source_set(const TUYA_WAKEUP_SOURCE_BASE_CFG_T  *param);
```

- Function Description:
  - Sets the wake-up source.
- Parameters:
  - `param`: Wake-up source, options include GPIO and timer.
- Return Value:
  - OPRT_OK: Success
  - Others, please refer to the file `tuya_error_code.h`.

### tkl_wakeup_source_clear

```c
OPERATE_RET tkl_wakeup_source_clear(const TUYA_WAKEUP_SOURCE_BASE_CFG_T *param);
```

- Function Description:
  - Clears the wake-up source.
- Parameters:
  - `param`: Wake-up source parameters.
- Return Value:
  - OPRT_OK: Success
  - Others, please refer to the file `tuya_error_code.h`.
