# 系统接口

## 说明
系统相关的功能接口

## API 描述
### tkl_system_reset

```c
void tkl_system_reset(void)
```

**功能简介**：
执行系统重启。

**参数**：
- 无

**返回**：
- 无

**备注**：
调用该函数会导致系统重启。

### tkl_system_get_tick_count

```c
SYS_TICK_T tkl_system_get_tick_count(void)
```

**功能简介**：
获取系统的滴答计数。

**参数**：
- 无

**返回**：
- `SYS_TICK_T`：系统滴答计数的当前值。

### tkl_system_get_millisecond

```c
SYS_TIME_T tkl_system_get_millisecond(void)
```

**功能简介**：
获取从系统启动开始的毫秒数。

**参数**：
- 无

**返回**：
- `SYS_TIME_T`：从系统启动到当前时刻的总毫秒数。

### tkl_system_get_random

```c
int tkl_system_get_random(uint32_t range)
```

**功能简介**：
获取一个指定范围内的随机数。

**参数**：
- `range`：随机数的范围，从 `0` 到 `range`（不包括`range`本身）。

**返回**：
- `int`：生成的随机数。

**备注**：
随机数生成器在初次调用时会被初始化。

### tkl_system_get_reset_reason

```c
TUYA_RESET_REASON_E tkl_system_get_reset_reason(char** describe)
```

**功能简介**：
获取系统复位的原因。

**参数**：
- `describe`：指向描述系统复位原因的字符串的指针。

**返回**：
- `TUYA_RESET_REASON_E`：复位原因的枚举值。
```c   
    typedef enum {
        TUYA_RESET_REASON_POWERON    = 0,  ///< Poweron reset type, supply voltage < power-on threshold (TY_RST_POWER_OFF)
        TUYA_RESET_REASON_HW_WDOG    = 1,  ///< Hardware watchdog reset occurred (TY_RST_HARDWARE_WATCHDOG)
        TUYA_RESET_REASON_FAULT      = 2,  ///< A access fault occurred (TY_RST_FATAL_EXCEPTION)
        TUYA_RESET_REASON_SW_WDOG    = 3,  ///< Software watchdog reset occurred (TY_RST_SOFTWARE_WATCHDOG)
        TUYA_RESET_REASON_SOFTWARE   = 4,  ///< Software triggered reset (TY_RST_SOFTWARE)
        TUYA_RESET_REASON_DEEPSLEEP  = 5,  ///< Reset caused by entering deep sleep (TY_RST_DEEPSLEEP)
        TUYA_RESET_REASON_EXTERNAL   = 6,  ///< External reset trigger        (TY_RST_HARDWARE)
        TUYA_RESET_REASON_UNKNOWN    = 7,  ///< Underterminable cause
        TUYA_RESET_REASON_FIB        = 8,  ///< Reset originated from the FIB bootloader
        TUYA_RESET_REASON_BOOTLOADER = 8,  ///< Reset relates to an bootloader
        TUYA_RESET_REASON_CRASH      = 10, ///< Software crash
        TUYA_RESET_REASON_FLASH      = 11, ///< Flash failure cause reset
        TUYA_RESET_REASON_FATAL      = 12, ///< A non-recoverable fatal error occurred
        TUYA_RESET_REASON_BROWNOUT   = 13, ///< Brown out
        TUYA_RESET_REASON_UNSUPPORT  = 0xFF,
    } TUYA_RESET_REASON_E;
```    
**备注**：
不同的复位来源会被映射到特定的枚举值上。

### tkl_system_sleep

```c
void tkl_system_sleep(uint32_t num_ms)
```

**功能简介**：
使系统进入休眠状态一定时间。

**参数**：
- `num_ms`：休眠的时间，单位为毫秒。

**返回**：
- 无

**备注**：
调用此函数会导致任务调度器暂停调用当前任务指定的时间。

### tkl_system_delay

```c
void tkl_system_delay(uint32_t num_ms)
```

**功能简介**：
使系统延时一定时间。

**参数**：
- `num_ms`：延时的时间，单位为毫秒。

**返回**：
- 无

### tkl_system_get_cpu_info

```c
OPERATE_RET tkl_system_get_cpu_info(TUYA_CPU_INFO_T **cpu_ary, int *cpu_cnt)
```

**功能简介**：
获取系统CPU信息。

**参数**：
- `cpu_ary`：指向 `TUYA_CPU_INFO_T` 类型数组的指针，用于接收 CPU 信息。
- `cpu_cnt`：指针，用于接收 CPU 数量。

**返回**：
- `OPERATE_RET`：操作结果，`OPRT_OK` 表示成功，其他值请参考 `tuya_error_code.h`。

**备注**：
该函数在当前实现中不支持，调用将返回`OPRT_NOT_SUPPORTED`。
