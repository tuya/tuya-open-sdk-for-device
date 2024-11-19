# PWM Driver

## Brief Description

PWM (Pulse Width Modulation) is an effective technology that uses the digital output of a microprocessor to control analog circuits.

![img](https://images.tuyacn.com/fe-static/docs/img/ba656efd-316e-412a-9370-7b8f10fb94d9.png)

As shown in the figure above, a PWM waveform with a pulse period of 10ms: the first period has a high level of 7ms (duty cycle 70%); the second period has a high level of 4ms (duty cycle 40%). By adjusting the duty cycle value of the high level, the size of the analog output value can be changed.

## API Description

### tkl_pwm_init

```c
OPERATE_RET tkl_pwm_init(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_BASE_CFG_T *cfg);
```

- Function Description:
  - Initializes the corresponding PWM instance through the port number and basic configuration, returning the initialization result.
  
- Parameters:
  - `ch_id`: Channel number.
  
  - `cfg`: PWM basic configuration, including output polarity, duty cycle, and frequency.
  
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
    
    | Name              | Definition          | Remarks |
    | :---------------- | :------------------ | :------ |
    | TUYA_PWM_NEGATIVE | PWM low-level output |         |
    | TUYA_PWM_POSITIVE | PWM high-level output |         |
    
    #### count_mode:
    
    Counting mode, with `TUYA_PWM_CNT_UP` and `TUYA_PWM_CNT_UP_AND_DOWN` as options.
    
    #### duty:
    
    Duty cycle, used with cycle, output = duty / cycle.
    
    #### cycle:
    
    Period, or granularity, output = duty / cycle.
    
    #### frequency:
    
    Output frequency, in Hz.
  
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_deinit

```c
OPERATE_RET tkl_pwm_deinit(TUYA_PWM_NUM_E ch_id);
```

- Function Description:
  - Deinitializes the PWM instance.
  - This interface stops any ongoing transmission of the PWM instance (if any) and releases related software and hardware resources.
- Parameters:
  - `ch_id`: Channel number.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_start

```c
OPERATE_RET tkl_pwm_start(TUYA_PWM_NUM_E ch_id);
```

- Function Description:
  - Starts PWM.
- Parameters:
  - `ch_id`: Channel number.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_stop

```c
OPERATE_RET tkl_pwm_stop(TUYA_PWM_NUM_E ch_id);
```

- Function Description:
  - Stops PWM.
- Parameters:
  - `port`: Port number.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_multichannel_start

```c
OPERATE_RET tkl_pwm_multichannel_start(TUYA_PWM_NUM_E *ch_id, uint8_t num);
```

- Function Description:
  - Simultaneously starts multiple channel PWMs for combined output, used in scenarios with strict timing requirements.
- Parameters:
  - `ch_id`: List of channel numbers, array.
  - `num`: Number of channels to start.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_multichannel_stop

```c
OPERATE_RET tkl_pwm_multichannel_stop(TUYA_PWM_NUM_E *ch_id, uint8_t num);
```

- Function Description:
  - Simultaneously stops multiple channel PWMs for combined output, used in scenarios with strict timing requirements.
- Parameters:
  - `ch_id`: List of channel numbers, array.
  - `num`: Number of channels to stop.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_info_set

```c
OPERATE_RET tkl_pwm_info_set(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_BASE_CFG_T *info);
```

- Function Description:
  - Resets PWM configuration parameters, allowing dynamic modification of the configuration after PWM start, and then restarts.
- Parameters:
  - `ch_id`: Channel number.
  - `info`: PWM basic configuration, including output polarity, duty cycle, frequency. The structure parameters are described above.
- Return Value:
  - Error code, OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_info_get

```c
OPERATE_RET tkl_pwm_info_get(TUYA_PWM_NUM_E ch_id, TUYA_PWM_BASE_CFG_T *info);
```

- Function Description:
- Parameters:
  - `ch_id`: Channel number.
  - `info`: PWM basic configuration, including output polarity, duty cycle, frequency. The structure parameters are described above.
- Return Value:
  - Error code, OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_cap_start

```c
OPERATE_RET tkl_pwm_cap_start(TUYA_PWM_NUM_E ch_id, const TUYA_PWM_CAP_IRQ_T *cfg);
```

- Function Description:
  - Enables PWM input capture mode.
- Parameters:
  - `ch_id`: Channel number.
  - `cfg`: PWM input capture configuration, as detailed below.

    #### cap_mode:

    | Name                         | Definition         | Remarks |
    | :--------------------------- | :----------------- | :------ |
    | TUYA_PWM_CAPTURE_MODE_ONCE   | Single trigger mode |         |
    | TUYA_PWM_CAPTURE_MODE_PERIOD | Multiple trigger mode |         |

    #### trigger_level:

    | Name              | Definition             | Remarks |
    | :---------------- | :--------------------- | :------ |
    | TUYA_PWM_NEGATIVE | Trigger signal is falling edge |         |
    | TUYA_PWM_POSITIVE | Trigger signal is rising edge |         |

    #### clk:

    Sampling clock for captured signal.

    #### cb:

    Callback function for captured signal, as follows:

    ```c
    typedef void(*TUYA_PWM_IRQ_CB)(TUYA_PWM_NUM_E port, TUYA_PWM_CAPTURE_DATA_T data, void *arg);
    ```

    ```c
    typedef struct {
        uint32_t      cap_value;            /* Captured data */
        TUYA_PWM_POLARITY_E cap_edge;     /* Capture edge, TUYA_PWM_NEGATIVE:falling edge, TUYA_PWM_POSITIVE:rising edge */
    } TUYA_PWM_CAPTURE_DATA_T;
    ```

    #### arg:

    Callback function parameters.

- Return Value:
  - Error code, OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_pwm_cap_stop

```c
OPERATE_RET tkl_pwm_cap_stop(TUYA_PWM_NUM_E ch_id);
```

- Function Description:
  - Disables PWM input capture mode.
- Parameters:
  - `ch_id`: Channel number.
- Return Value:
  - Error code, OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

# PWM Example
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
