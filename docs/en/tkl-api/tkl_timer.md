# HW Timer Driver

## Brief Description

A timer is an on-chip peripheral in a microprocessor used for time measurement. Depending on the actual configuration requirements, timers usually have different timing precisions, such as 16-bit, 32-bit, etc. In actual use, it is usually necessary to configure parameters such as the timing period, counting method, and interrupt service routine.

## API Description

### tkl_timer_init

```c
OPERATE_RET tkl_timer_init(TUYA_TIMER_NUM_E timer_id, TUYA_TIMER_BASE_CFG_T *cfg);
```

- Function Description:
  - Initializes the corresponding timer instance based on the port number and basic configuration, and returns the initialization result.
  
- Parameters:
  - `timer_id`: Port number.
  
  - `cfg`: Timer basic configuration, including timing mode, callback function, and callback function parameters.
  
    ```c
    typedef struct {
        TKL_TIMER_MODE_E    mode;
        TKL_TIMER_ISR_CB    cb;
        void                *args;
    } TUYA_TIMER_BASE_CFG_T;
    ```
  
    #### TKL_TIMER_MODE_E:
  
    | Name                  | Definition   | Remarks |
    | :-------------------- | :----------- | :------ |
    | TKL_TIMER_MODE_ONCE   | One-shot timer |          |
    | TKL_TIMER_MODE_PERIOD | Periodic timer |          |
  
- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

### tkl_timer_deinit

```c
OPERATE_RET tkl_timer_deinit(TUYA_TIMER_NUM_E timer_id);
```

- Function Description:
  - Deinitializes the timer instance.
  - This interface stops the timer and releases the software and hardware resources related to the timer.
- Parameters:
  - `timer_id`: Port number.
- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

### tkl_timer_start

```c
OPERATE_RET tkl_timer_start(TUYA_TIMER_NUM_E timer_id, uint32_t us);
```

- Function Description:
  - Starts the timer.

- Parameters:
  - `timer_id`: Port number.
  - `us`: Timer interval.

- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

### tkl_timer_stop

```c
OPERATE_RET tkl_timer_stop(TKL_TIMER_PORT_E port);
```

- Function Description:
  - Stops the timer.
- Parameters:
  - `port`: Port number.
- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

### tkl_timer_get

```c
OPERATE_RET tkl_timer_get(TKL_TIMER_PORT_E port, uint32_t *us);
```

- Function Description:
  - Gets the timer interval.
- Parameters:
  - `port`: Port number.
  - `us`: Timer interval value, in microseconds, corresponding to the value set in tkl_timer_start.
- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

### tkl_timer_get_current_value

```c
OPERATE_RET tkl_timer_get_current_value(TUYA_TIMER_NUM_E timer_id, uint32_t *us);
```

- Function Description:
  - Gets the current count value of the timer.
- Parameters:
  - `timer_id`: Port number.
  - `us`: Current count value, in microseconds.
- Return Value:
  - OPRT_OK for success, others please refer to the file `tuya_error_code.h`.

## Example

```c
static void tkl_timer_isr_cb_fun(void *args)
{
	PR_NOTICE("hw_timer test");
}

void tuya_timer_test(void)
{
    OPERATE_RET ret;
    TUYA_TIMER_BASE_CFG_T cfg;
    uint32_t interval_us;
    uint32_t get_us;

    cfg.mode = TUYA_TIMER_MODE_PERIOD;
    cfg.cb = tkl_timer_isr_cb_fun;
    cfg.arg = NULL;
    
    ret = tkl_timer_init(TUYA_TIMER_NUM_0, &cfg);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    ret = tkl_timer_start(TUYA_TIMER_NUM_0, 1000);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    tkl_system_delay(5000);
    ret = tkl_timer_stop(TUYA_TIMER_NUM_0);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    ret = tkl_timer_get(TUYA_TIMER_NUM_0, &interval_us);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
	if(interval_us != 2000){
        interval_us = 2000;
    }
    ret = tkl_timer_start(TUYA_TIMER_NUM_0, interval_us);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    tkl_system_delay(1000);
    ret = tkl_timer_get_current_value(TUYA_TIMER_NUM_0, &get_us);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    PR_DEBUG("current run time:%d us", get_us);
    tkl_system_delay(5000);
    //uninitialize timer 
    ret = tkl_timer_deinit(TUYA_TIMER_NUM_0);
    if (ret != 0) {
       //failed
    }
}
```