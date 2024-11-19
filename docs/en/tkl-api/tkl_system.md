# System Interface

## Description
Interfaces related to system functions.

## API Description

### tkl_system_reset

```c
void tkl_system_reset(void)
```

**Brief Description**:
Performs a system reset.

**Parameters**:
- None

**Return**:
- None

**Remarks**:
Calling this function will cause the system to restart.

### tkl_system_get_tick_count

```c
SYS_TICK_T tkl_system_get_tick_count(void)
```

**Brief Description**:
Gets the system tick count.

**Parameters**:
- None

**Return**:
- `SYS_TICK_T`: The current value of the system tick count.

### tkl_system_get_millisecond

```c
SYS_TIME_T tkl_system_get_millisecond(void)
```

**Brief Description**:
Gets the number of milliseconds since the system started.

**Parameters**:
- None

**Return**:
- `SYS_TIME_T`: The total number of milliseconds from system start to the current time.

### tkl_system_get_random

```c
int tkl_system_get_random(uint32_t range)
```

**Brief Description**:
Gets a random number within a specified range.

**Parameters**:
- `range`: The range of the random number, from `0` to `range` (not including `range` itself).

**Return**:
- `int`: The generated random number.

**Remarks**:
The random number generator is initialized upon the first call.

### tkl_system_get_reset_reason

```c
TUYA_RESET_REASON_E tkl_system_get_reset_reason(char** describe)
```

**Brief Description**:
Gets the reason for the system reset.

**Parameters**:
- `describe`: A pointer to a string describing the reason for the system reset.

**Return**:
- `TUYA_RESET_REASON_E`: An enumeration value representing the reset reason.
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
**Remarks**:
Different reset sources are mapped to specific enumeration values.

### tkl_system_sleep

```c
void tkl_system_sleep(uint32_t num_ms)
```

**Brief Description**:
Puts the system to sleep for a specified duration.

**Parameters**:
- `num_ms`: The duration to sleep, in milliseconds.

**Return**:
- None

**Remarks**:
Calling this function will pause the task scheduler for the current task for the specified duration.

### tkl_system_delay

```c
void tkl_system_delay(uint32_t num_ms)
```

**Brief Description**:
Delays the system for a specified duration.

**Parameters**:
- `num_ms`: The duration to delay, in milliseconds.

**Return**:
- None

### tkl_system_get_cpu_info

```c
OPERATE_RET tkl_system_get_cpu_info(TUYA_CPU_INFO_T **cpu_ary, int *cpu_cnt)
```

**Brief Description**:
Gets system CPU information.

**Parameters**:
- `cpu_ary`: A pointer to an array of `TUYA_CPU_INFO_T` type, used to receive CPU information.
- `cpu_cnt`: A pointer used to receive the number of CPUs.

**Return**:
- `OPERATE_RET`: The operation result, `OPRT_OK` indicates success, other values please refer to `tuya_error_code.h`.

**Remarks**:
This function is not supported in the current implementation and will return `OPRT_NOT_SUPPORTED`.
