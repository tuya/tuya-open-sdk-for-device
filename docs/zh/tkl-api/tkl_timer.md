# HW TIMER 驱动

## 简要说明

timer 是微处理器中用来进行时间计量的片上外设，根据实际配置要求，通常有不同的定时精度，如 16 位，32 位等，实际使用时通常需要配置的参数有，定时周期，计数方式，中断服务程序等。

## API 描述

### tkl_timer_init

```c
OPERATE_RET tkl_timer_init(TUYA_TIMER_NUM_E timer_id, TUYA_TIMER_BASE_CFG_T *cfg);
```

- 功能描述:
  - 通过端口号和基础配置初始化对应的 timer 实例，返回初始化结果 。
  
- 参数:
  - `timer_id`: 端口号。
  
  - `cfg`: timer 基础配置，包含定时方式，回调函数，回调函数参数 。
  
    ```c
    typedef struct {
        TKL_TIMER_MODE_E    mode;
        TKL_TIMER_ISR_CB    cb;
        void                    *args;
    } TUYA_TIMER_BASE_CFG_T;
    ```
  
    #### TKL_TIMER_MODE_E:
  
    | 名字                  | 定义     | 备注 |
    | :-------------------- | :------- | :--- |
    | TKL_TIMER_MODE_ONCE   | 单次定时 |      |
    | TKL_TIMER_MODE_PERIOD | 周期定时 |      |
  
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_timer_deinit

```c
OPERATE_RET tkl_timer_deinit(TUYA_TIMER_NUM_E timer_id);
```

- 功能描述:
  - timer 实例反初始化。
  - 该接口会停止 timer，并且释放 timer 相关的软硬件资源。
- 参数:
  - `timer_id`: 端口号。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_timer_start

```c
OPERATE_RET tkl_timer_start(TUYA_TIMER_NUM_E timer_id, uint32_t us);
```

- 功能描述:

  - 启动 timer。

- 参数:

  - `timer_id`: 端口号。

  - `us`: timer 的定时间隔。

- 返回值:

  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_timer_stop

```c
OPERATE_RET tkl_timer_stop(TKL_TIMER_PORT_E port);
```

- 功能描述:
  - 停止 timer。
- 参数:
  - `port`: 端口号。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_timer_get

```c
OPERATE_RET tkl_timer_get(TKL_TIMER_PORT_E port, uint32_t *us);
```

- 功能描述:
  - 获取 timer 定时间隔。
- 参数:
  - `port`: 端口号。
  - us：定时间隔值，单位 us，对应 tkl_timer_start 设置的值。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_timer_get_current_value

```c
OPERATE_RET tkl_timer_get_current_value(TUYA_TIMER_NUM_E timer_id, uint32_t *us);
```

- 功能描述:
  - 获取 timer 当前计数值。
- 参数:
  - `timer_id`: 端口号。
  - us：当前计数值，单位 us
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

## 示例

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
    uint32_t get_us；

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
    //uninitialize iic 
    ret = tkl_timer_deinit(TUYA_TIMER_NUM_0);
    if (ret != 0) {
       //failed
    }
}
```



