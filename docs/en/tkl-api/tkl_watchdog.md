# WATCHDOG Driver

## Brief Description

A watchdog is a timer circuit responsible for resetting the processor in the event of a program running amok or hardware stagnation, allowing the program to execute from the beginning.

Working Principle: The watchdog is a timer circuit with an input and an output. The input is the watchdog feeding operation (for what feeding the watchdog means, see below). Feeding the watchdog means reloading the watchdog counter value through an external input, and the output is connected to the reset terminal of another circuit. When the watchdog counter decrements from the initial value to 0, it sends a signal to the reset terminal of another circuit, causing the program to perform a reset operation.

Usage: To monitor whether the program is running amok, perform a watchdog feeding operation at regular intervals in the program. That is, insert multiple watchdog feeding operations intermittently in a complete program segment. If the program starts running amok at a certain point, the next watchdog feeding operation will not be executed. In this case, when the watchdog counter decrements from the initial value to 0, the program performs a reset operation. It is important to note that attention should be paid to the interval for feeding the watchdog. It should not be too late, otherwise, if it exceeds the overflow time of the counter, the program will perform a reset operation, which would be equivalent to mistakenly judging that the program is running amok.

## API Description

### tkl_watchdog_init

```c
uint32_t tkl_watchdog_init(TUYA_WDOG_BASE_CFG_T *cfg);
```

- Function Description:
  
  - Initializes the watchdog based on the configuration and returns the initialization result.
  
- Parameters:
  
  - `cfg`: Basic configuration of the watchdog.
  
    ```c
    typedef struct {
        uint32_t interval_ms; // Watchdog time interval
    } TUYA_WDOG_BASE_CFG_T;
    ```
  
- Return Value:
  
  - OPRT_OK Success, if greater than 0, it is the actual watchdog time.

### tkl_watchdog_deinit

```c
OPERATE_RET tkl_watchdog_deinit(void);
```

- Function Description:
  - Deinitializes the watchdog and stops it.
- Parameters:
  - `void`
- Return Value:
  - OPRT_OK Success, others please refer to the `tuya_error_code.h` file definition section.

### tkl_watchdog_refresh

```c
OPERATE_RET tkl_watchdog_refresh(void);
```

- Function Description:

  - Feeds the watchdog.

- Parameters:

  - `void`.

- Return Value:

  - OPRT_OK Success, others please refer to the `tuya_error_code.h` file definition section.

## Examples

### WATCHDOG Example

```c
/* Initialize the watchdog */
TUYA_WDOG_BASE_CFG_T cfg;
uint32_t actual_interval_ms = 0;
cfg.interval_ms = 100;
actual_interval_ms = tkl_watchdog_init(&cfg);

if (actual_interval_ms) {
    /* The actual watchdog time is actual_interval_ms */    
} else {
    /* The actual watchdog time is cfg.interval_ms */   
}

/* Feed the watchdog */
tkl_watchdog_refresh();

/* Deinitialize the watchdog */
tkl_watchdog_deinit();
```
