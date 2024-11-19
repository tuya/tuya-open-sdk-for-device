# ADC Driver

## Description

### Introduction

ADC, Analog to Digital Converter, generally refers to an electronic component that converts a continuous-time, continuous-amplitude analog signal into a discrete-time, discrete-amplitude digital signal.

A/D conversion typically involves four processes: sampling, holding, quantization, and encoding. In actual circuits, some of these processes may be combined, such as sampling and holding, and quantization and encoding, which are often implemented simultaneously during the conversion process.

### Classification

| Direct ADC | Parallel Comparator ADC |
| -------- | -------------- |
|          | Successive Approximation ADC |
|          | Flash ADC     |
| Indirect ADC | Dual Slope ADC   |

### Technical Specifications

Resolution: Theoretically, the resolution of an ADC to the input signal, generally 8-bit, 10-bit, 12-bit, 16-bit, etc.

Accuracy: The difference between the actual analog voltage and the sampled voltage, where the maximum difference is the absolute accuracy, and the percentage of the maximum difference to the full-scale analog voltage is called the relative error.

Conversion Time: The time required for each sample, indicating the conversion speed of the ADC, related to the ADC's clock frequency, sampling period, and conversion period.

Data Output Mode: Parallel output, serial output.

Operating Voltage: Attention should be given to the ADC's operating voltage range and whether it can directly measure negative voltages, etc.

## API Description

### tkl_adc_init

```c
OPERATE_RET tkl_adc_init(TUYA_ADC_NUM_E port_num, TUYA_ADC_BASE_CFG_T *cfg);
```

- Function Description:
  - ADC Initialization
  
- Parameters:
  - `port_num`: ADC port number, each port corresponds to an ADC entity device (usually with multiple channels), values as follows:
  
    | Name           | Definition       | Remarks |
    | :------------- | :--------- | :--- |
    | TUYA_ADC_NUM_0 | ADC Port 0 |      |
    | TUYA_ADC_NUM_1 | ADC Port 1 |      |
    | TUYA_ADC_NUM_2 | ADC Port 2 |      |
    | TUYA_ADC_NUM_3 | ADC Port 3 |      |
    | TUYA_ADC_NUM_4 | ADC Port 4 |      |
    | TUYA_ADC_NUM_5 | ADC Port 5 |      |
    
  - `cfg`: ADC base configuration, values as follows:
  
    ```c
    typedef struct {
        TUYA_AD_DA_CH_LIST_U  ch_list;  // ADC channel list
        uint8_t ch_nums;        // Number of ADC channels to be sampled
        uint8_t  width;         // Resolution (bit width)
        uint32_t freq;          // Sampling frequency
        TUYA_ADC_TYPE_E type;	// ADC sampling type
        TUYA_ADC_MODE_E mode;   // ADC sampling mode
        uint16_t   conv_cnt;    // ADC number of samples
        uint32_t   ref_vol;     // ADC reference voltage (unit: mv)
    } TUYA_ADC_BASE_CFG_T;
    ```
    
    #### TUYA_AD_DA_CH_LIST_U：
    
    ```c
    typedef union {
        TUYA_AD_DA_CH_LIST_BIT_T bits;
        uint32_t data;
    }TUYA_AD_DA_CH_LIST_U;
    ```
    
    ```c
    typedef struct {
        uint32_t ch_0             : 1;
        uint32_t ch_1             : 1;
        uint32_t ch_2             : 1;
        uint32_t ch_3             : 1;
        uint32_t ch_4             : 1;
        uint32_t ch_5             : 1;
        uint32_t ch_6             : 1;
        uint32_t ch_7             : 1;
        uint32_t ch_8             : 1;
        uint32_t ch_9             : 1;
        uint32_t ch_10            : 1;
        uint32_t ch_11            : 1;
        uint32_t ch_12            : 1;
        uint32_t ch_13            : 1;
        uint32_t ch_14            : 1;
        uint32_t ch_15            : 1;
        uint32_t rsv              :16;
    }TUYA_AD_DA_CH_LIST_BIT_T;
    ```
    
    The channel list can be operated using bit manipulation or through the `data` field, both have the same effect.
    
    `ch_nums` is the number of channels to be sampled.
    
    #### TUYA_ADC_TYPE_E：
    
    | Name                         | Definition                                 | Remarks |
    | :--------------------------- | :----------------------------------- | :--- |
    | TUYA_ADC_INNER_SAMPLE_VOL    | ADC samples internal voltage (e.g., power supply voltage) |      |
    | TUYA_ADC_EXTERNAL_SAMPLE_VOL | ADC samples external voltage (e.g., external pin voltage) |      |
    
    #### TUYA_ADC_MODE_E：
    
    ```c
    typedef enum {
        TUYA_ADC_SINGLE = 0,       ///< Single conversion mode
        TUYA_ADC_CONTINUOUS,       ///< Continuous conversion mode
        TUYA_ADC_SCAN,             ///< Scan mode
    } TUYA_ADC_MODE_E;
    ```
    
    | Name                | Definition           | Remarks         |
    | :------------------ | :------------- | :----------- |
    | TUYA_ADC_SINGLE     | Single channel sampling |              |
    | TUYA_ADC_CONTINUOUS | Single channel multiple sampling |              |
    | TUYA_ADC_SCAN       | Scan mode sampling   | Multiple channel sampling |
    
    `conv_cnt` specifies the number of samples in the sampling mode.
  
- Return Value:
  
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_deinit

```c
OPERATE_RET tkl_adc_deinit(TUYA_ADC_NUM_E port_num);
```

- Function Description:
  - ADC De-initialization
- Parameters:
  - `port_num`: ADC port number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_width_get

```c
uint8_t tkl_adc_width_get(TUYA_ADC_NUM_E port_num);
```

- Function Description:
  - ADC Query Resolution (bit width)
- Parameters:
  - `port_num`: ADC port number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_ref_voltage_get

```c
uint32_t tkl_adc_ref_voltage_get(TUYA_ADC_NUM_E port_num);
```

- Function Description:
  - ADC Query Reference Voltage
- Parameters:
  - `port_num`: ADC port number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_temperature_get

```c
int32_t tkl_adc_temperature_get(void);
```

- Function Description:
  - ADC Query Temperature (refers to querying the chip temperature here)
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_read_data

```c
OPERATE_RET tkl_adc_read_data(TUYA_ADC_NUM_E port_num, int32_t *buff, uint16_t len);
```

- Function Description:
  - ADC Read Data
- Parameters:
  - `port_num`: ADC port number
  - `buff`: ADC data buffer
  - `len`: Length of the ADC data buffer
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_read_single_channel

```c
OPERATE_RET tkl_adc_read_single_channel(TUYA_ADC_NUM_E port_num, uint8_t ch_id, int32_t *data);
```

- Function Description:
  - ADC Read Data (Single Channel)
- Parameters:
  - `port_num`: ADC port number
  - `ch_id`: ADC channel number
  - `data`: ADC data buffer
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

### tkl_adc_read_voltage

```c
OPERATE_RET tkl_adc_read_voltage(TUYA_ADC_NUM_E port_num, int32_t *buff, uint16_t len);
```

- Function Description:
  - ADC Read Voltage
- Parameters:
  - `port_num`: ADC port number
  - `buf`: ADC voltage buffer
  - `len`: Length of the ADC voltage buffer
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OS_ADAPTER_ADC` section in the file `tuya_error_code.h`

## Examples

### Example 1

```c
void tuya_adc_single_channel_test(void)
{
    OPERATE_RET ret;
    TUYA_ADC_BASE_CFG_T adc_cfg;
    uint32_t adc_value = 0;
    uint16_t channel = 0;
    
    adc_cfg.ch_list.data = 1; // or adc_cfg.ch_list.bits.ch_0 = 1;
    adc_cfg.ch_nums = 1;
    adc_cfg.type = TUYA_ADC_INNER_SAMPLE_VOL;
    adc_cfg.mode = TUYA_ADC_SINGLE;
    adc_cfg.width = 10;
    adc_cfg.conv_cnt = 1;
   
    ret = tkl_adc_init(ADC_NUM_0, &adc_cfg);
    if(ret != OPRT_OK) {
        // failed
        return;
    }

    ret = tkl_adc_read_single_channel(ADC_NUM_0, channel, &adc_value);
    if(ret != OPRT_OK) {
        // failed
        return;
    }

    // Output the value of adc_value

    ret = tkl_adc_deinit(ADC_NUM_0);
    if(ret != OPRT_OK) {
        // failed
        return;
    }
}
```

### Example 2

```c
#define ADC_CHANNEL_NUM 3

void tuya_adc_multi_channel_test(void)
{
    OPERATE_RET ret;
    TUYA_ADC_BASE_CFG_T adc_cfg;
    uint32_t adc_value[ADC_CHANNEL_NUM] = {0};
    
    adc_cfg.ch_list.bits.ch_0 = 1;
    adc_cfg.ch_list.bits.ch_1 = 1;
    adc_cfg.ch_list.bits.ch_2 = 1;
    adc_cfg.ch_nums = ADC_CHANNEL_NUM;
    adc_cfg.type = TUYA_ADC_INNER_SAMPLE_VOL;
    adc_cfg.mode = TUYA_ADC_SCAN;
    adc_cfg.width = 10;
    adc_cfg.conv_cnt = 1;

    ret = tkl_adc_init(ADC_NUM_0, &adc_cfg);
    if(ret != OPRT_OK) {
        // failed
        return;
    }

    ret = tkl_adc_read_data(ADC_NUM_0, adc_value, ADC_CHANNEL_NUM);
    if(ret != OPRT_OK) {
        // failed
        return;
    }

    // Output the values of adc_value[ADC_CHANNEL_NUM]

    ret = tkl_adc_deinit(ADC_NUM_0);
    if(ret != OPRT_OK) {
        // failed
        return;
    }
}
```
