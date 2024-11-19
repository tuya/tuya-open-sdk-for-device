# PWM 驱动

## 简要说明

PWM（Pulse Width Modulation）,即脉冲宽度调制，其是利用微处理器的数字输出来对模拟电路进行控制的一种有效的技术。

![img](https://images.tuyacn.com/fe-static/docs/img/ba656efd-316e-412a-9370-7b8f10fb94d9.png)

如上图所示，一个脉冲周期为 10ms 的 PWM 波形图,第一个周期高电平为 7ms(占空比70%);第二个周期高电平为 4ms(占空比40%)。通过调节高电平占空比的值，即可以改变模拟输出值的大小。

## API 描述

### tkl_pwm_init

```c
OPERATE_RET tkl_pwm_init(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_BASE_CFG_T *cfg);
```

- 功能描述:
  - 通过端口号和基础配置初始化对应的 PWM 实例，返回初始化结果。
  
- 参数:
  - `ch_id`: 通道号。
  
  - `cfg`: PWM 基础配置，包含输出极性，占空比，频率。
  
    ```c
    typedef struct {
        TUYA_PWM_POLARITY_E polarity;
        TUYA_PWM_COUNT_E    count_mode;
       //pulse duty cycle = duty / cycle; exp duty = 5000,cycle = 10000; pulse duty cycle = 50%
        uint32_t              duty;
        uint32_t              cycle;
        uint32_t              frequency;  // (bet: Hz)
    } TUYA_PWM_BASE_CFG_T;
    ```
    
    #### polarity:
    
    | 名字              | 定义          | 备注 |
    | :---------------- | :------------ | :--- |
    | TUYA_PWM_NEGATIVE | PWM低有效输出 |      |
    | TUYA_PWM_POSITIVE | PWM高有效输出 |      |
    
    #### count_mode：
    
    计数模式，有 `TUYA_PWM_CNT_UP` 和 `TUYA_PWM_CNT_UP_AND_DOWN` 两种。
    
    #### duty：
    
    占空比，和cycle搭配使用，输出 = duty / cycle 。
    
    #### cycle：
    
    周期，或颗粒度，输出 = duty / cycle。
    
    #### frequency：
    
    输出频率，单位Hz。
  
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_deinit

```c
OPERATE_RET tkl_pwm_deinit(TUYA_PWM_NUM_E ch_id);
```

- 功能描述:
  - PWM 实例反初始化。
  - 该接口会停止 PWM 实例正在进行的传输（如果有），并且释放相关的软硬件资源。
- 参数:
  - `ch_id`: 通道号。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_start

```c
OPERATE_RET tkl_pwm_start(TUYA_PWM_NUM_E ch_id);
```

- 功能描述:
  - 启动pwm。
- 参数:
  - `ch_id`: 通道号。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_stop

```c
OPERATE_RET tkl_pwm_stop(TUYA_PWM_NUM_E ch_id);
```

- 功能描述:
  - 停止 PWM。
- 参数:
  - `port`: 端口号。
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_multichannel_start

```c
OPERATE_RET tkl_pwm_multichannel_start(TUYA_PWM_NUM_E *ch_id, uint8_t num);
```

- 功能描述:
  - 同时启动多通道 PWM，用于多路组合输出，用于对时序要求比较严格的场景。
- 参数:
  - `ch_id`: 通道号列表，数组。
  - num：启动的通道号数目
- 返回值:
  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_multichannel_stop

```c
OPERATE_RET tkl_pwm_multichannel_stop(TUYA_PWM_NUM_E *ch_id, uint8_t num);
```

- 功能描述:

  - 同时停止多通道 PWM，用于多路组合输出，对时序要求比较严格的场景。

- 参数:

  - `ch_id`: 通道号列表，数组。
  - num：需要关闭的通道号数目

- 返回值:

  - OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

  

### tkl_pwm_info_set

```c
OPERATE_RET tkl_pwm_info_set(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_BASE_CFG_T *info);
```

- 功能描述:
  - 重设 PWM 配置参数，pwm start之后可动态修改配置，补充重新s tart。
- 参数:
  - `ch_id`: 通道号。
  - `info`: PWM 基础配置，包含输出极性，占空比，频率 。其结构体参数见上文描述
- 返回值:
  - 错误码，OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_info_get

```c
OPERATE_RET tkl_pwm_info_get(TUYA_PWM_NUM_E ch_id, TUYA_PWM_BASE_CFG_T *info);
```

- 功能描述:
- 参数:
  - `ch_id`: 通道号。
  - `info`: PWM 基础配置，包含输出极性，占空比，频率 。其结构体参数见上文描述
- 返回值:
  - 错误码，OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

### tkl_pwm_cap_start

```c
OPERATE_RET tkl_pwm_cap_start(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_CAP_IRQ_T *cfg);
```

- 功能描述:

  - 开启 PWM 输入捕获模式。

- 参数:

  - `ch_id`: 通道号。

  - `cfg`: PWM 输入捕获配置，如下详述。

    #### cap_mode：

    | 名字                         | 定义         | 备注 |
    | :--------------------------- | :----------- | :--- |
    | TUYA_PWM_CAPTURE_MODE_ONCE   | 单次触发模式 |      |
    | TUYA_PWM_CAPTURE_MODE_PERIOD | 多次触发模式 |      |

    #### trigger_level:

    | 名字              | 定义             | 备注 |
    | :---------------- | :--------------- | :--- |
    | TUYA_PWM_NEGATIVE | 触发信号为下降沿 |      |
    | TUYA_PWM_POSITIVE | 触发信号为上升沿 |      |

    #### clk：

    抓取信号的采样时钟。

    #### cb：

    抓取信号的回调函数,如下：

    ```c
    typedef void(*TUYA_PWM_IRQ_CB)(TUYA_PWM_NUM_E port, TUYA_PWM_CAPTURE_DATA_T data, void *arg);
    ```

    ```c
    typedef struct {
        uint32_t      cap_value;            /* Captured data */
        TUYA_PWM_POLARITY_E cap_edge;     /* Capture edge, TUYA_PWM_NEGATIVE:falling edge, TUYA_PWM_POSITIVE:rising edge */
    } TUYA_PWM_CAPTURE_DATA_T;
    ```

    #### arg：

    回调函数参数。  

- 返回值:

  - 错误码，OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。


### tkl_pwm_cap_stop

```c
OPERATE_RET tkl_pwm_cap_stop(TUYA_PWM_NUM_E ch_id);
```

- 功能描述:
  - 关闭 PWM 输入捕获模式。

- 参数:
  - `ch_id`: 通道号。
- 返回值:
  - 错误码，OPRT_OK 成功，其他请参考文件 `tuya_error_code.h`。

# PWM 示例
```c
void tuya_pwm_test(void)
{
    OPERATE_RET ret;    
 	  TUYA_PWM_BASE_CFG_T cfg = {.polarity = TUYA_PWM_POSITIVE,\
                         .duty = 1000,
                         .cycle = 10000,
                         .frequency = 1000};
    ret = tkl_pwm_init(TUYA_PWM_NUM_0, &cfg);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    ret = tkl_pwm_start(TUYA_PWM_NUM_0);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    tkl_system_delay(5000);
    ret = tkl_pwm_info_get(TUYA_PWM_NUM_0, &cfg);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    if(cfg.duty !=5000){
        cfg.duty =5000;
    }
    ret = tkl_pwm_info_set(TUYA_PWM_NUM_0, &cfg);
    //delay
    tkl_system_delay(5000);
    ret = tkl_pwm_stop(TUYA_PWM_NUM_0);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
    ret = tkl_pwm_deinit(TKL_PWM1_CH);
    if (ret != OPRT_OK) {
        //fail
        return;
    }
}
```





