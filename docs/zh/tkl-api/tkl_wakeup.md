# WAKEUP 驱动

## 简要说明

wakeup 用于设置芯片唤醒源，唤醒源可以是 GPIO，定时器等。

## API 描述

### tkl_wakeup_source_set

```c
OPERATE_RET tkl_wakeup_source_set(const TUYA_WAKEUP_SOURCE_BASE_CFG_T  *param);
```

- 功能描述：
  - 设置唤醒源。
- 参数：
  - `param`唤醒源，可选GPIO和timer。
- 返回值：
  
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h`。

### tkl_wakeup_source_clear

```c
OPERATE_RET tkl_wakeup_source_clear(const TUYA_WAKEUP_SOURCE_BASE_CFG_T *param);
```

- 功能描述：
  - 清除唤醒源。
- 参数：
  - `param`：唤醒源参数
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h`。
