# ADC 驱动

## 说明

### 简介

ADC，Analog to Digital Converter，模拟/数字转换器，通常是指一个将时间连续、幅值也连续的模拟信号转变为时间离散、幅值也离散的数字信号的电子元件。

A/D 转换一般要经过取样、保持、量化及编码 4 个过程。在实际电路中，这些过程有的是合并进行的，例如，取样和保持，量化和编码往往都是在转换过程中同时实现的。

### 分类

| 直接 ADC | 并联比较型 ADC |
| -------- | -------------- |
|          | 逐次逼近型 ADC |
|          | 闪存型 ADC     |
| 间接 ADC | 双积分型 ADC   |

### 技术指标

分辨率：理论上 ADC 对输入信号的分辨能力，一般有 8 位、10 位、12 位、16 位等

精度：实际模拟电压与采样电压之间的差值，差值的最大值为绝对精度，差值的最大值与满刻度模拟电压得百分比称为相对误差

转换时间：每次采样所需的时间，表征 ADC 的转换速度，与 ADC 的时钟频率、采样周期、转换周期有关

数据输出方式：并口输出、串口输出

工作电压：需要注意 ADC 的工作电压范围、能否直接测量负电压等；

## API 描述

### tkl_adc_init

```c
OPERATE_RET tkl_adc_init(TUYA_ADC_NUM_E port_num, TUYA_ADC_BASE_CFG_T *cfg);
```

- 功能描述：
  - ADC 初始化
  
- 参数：
  - `port_num`：ADC 端口号，一个端口对应一个 ADC 实体器件（一般有多个通道），取值如下：
  
    | 名字           | 定义       | 备注 |
    | :------------- | :--------- | :--- |
    | TUYA_ADC_NUM_0 | ADC 端口 0 |      |
    | TUYA_ADC_NUM_1 | ADC 端口 1 |      |
    | TUYA_ADC_NUM_2 | ADC 端口 2 |      |
    | TUYA_ADC_NUM_3 | ADC 端口 3 |      |
    | TUYA_ADC_NUM_4 | ADC 端口 4 |      |
    | TUYA_ADC_NUM_5 | ADC 端口 5 |      |
    
  - `cfg`：ADC 基础配置，取值如下：
  
    ```c
    typedef struct {
        TUYA_AD_DA_CH_LIST_U  ch_list;  // ADC 通道列表
        uint8_t ch_nums;        // 需要采集的ADC 通道个数
        uint8_t  width;         // 分辨率（位宽）
        uint32_t freq;          // 采样频率
        TUYA_ADC_TYPE_E type;	// ADC 采样类型
        TUYA_ADC_MODE_E mode;   // ADC 采样模式
        uint16_t   conv_cnt;    // ADC 采样次数
        uint32_t   ref_vol;     // ADC 参考电压（单位：mv）
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
    
    采集通道列表可以使用 bit 位操作，也可以通过 data 操作，两者效果相同。
    
    `ch_nums` 采集的通道数量。
    
    #### TUYA_ADC_TYPE_E：
    
    | 名字                         | 定义                                 | 备注 |
    | :--------------------------- | :----------------------------------- | :--- |
    | TUYA_ADC_INNER_SAMPLE_VOL    | ADC 采样内部电压（例如电源电压）     |      |
    | TUYA_ADC_EXTERNAL_SAMPLE_VOL | ADC 采样外部电压（例如外部引脚电压） |      |
    
    #### TUYA_ADC_MODE_E：
    
    ```c
    typedef enum {
        TUYA_ADC_SINGLE = 0,       ///< Single conversion mode
        TUYA_ADC_CONTINUOUS,       ///< Continuous conversion mode
        TUYA_ADC_SCAN,             ///< Scan mode
    } TUYA_ADC_MODE_E;
    ```
    
    | 名字                | 定义           | 备注         |
    | :------------------ | :------------- | :----------- |
    | TUYA_ADC_SINGLE     | 单通道采集     |              |
    | TUYA_ADC_CONTINUOUS | 单通道多次采集 |              |
    | TUYA_ADC_SCAN       | 扫描模式采集   | 可多通道采集 |
    
    `conv_cnt` 指定采集模式下的采集次数。
  
- 返回值：
  
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_deinit

```c
OPERATE_RET tkl_adc_deinit(TUYA_ADC_NUM_E port_num);
```

- 功能描述：
  - ADC 去初始化状态
- 参数：
  - `port_num`：ADC 端口号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_width_get

```c
uint8_t tkl_adc_width_get(TUYA_ADC_NUM_E port_num);
```

- 功能描述：
  - ADC 查询分辨率（位宽）
- 参数：
  - `port_num`：ADC 端口号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_ref_voltage_get

```c
uint32_t tkl_adc_ref_voltage_get(TUYA_ADC_NUM_E port_num);
```

- 功能描述：
  - ADC 查询参考电压
- 参数：
  - `port_num`：ADC 端口号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_temperature_get

```c
int32_t tkl_adc_temperature_get(void);
```

- 功能描述：
  - ADC 查询温度（此处指查询芯片温度）
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_read_data

```c
OPERATE_RET tkl_adc_read_data(TUYA_ADC_NUM_E port_num, int32_t *buff, uint16_t len);
```

- 功能描述：
  - ADC 读取数据
- 参数：
  - `port_num`：ADC 端口号
  - `buff`：ADC 数据缓冲区
  - `len`：ADC 数据缓冲区的长度
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_read_single_channel

```c
OPERATE_RET tkl_adc_read_single_channel(TUYA_ADC_NUM_E port_num, uint8_t ch_id, int32_t *data);
```

- 功能描述：
  - ADC 读取数据（单通道）
- 参数：
  - `port_num`：ADC 端口号
  - `ch_id`：ADC 通道号
  - `data`：ADC 数据缓冲区
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

### tkl_adc_read_voltage

```c
OPERATE_RET tkl_adc_read_voltage(TUYA_ADC_NUM_E port_num, int32_t *buff, uint16_t len);
```

- 功能描述：
  - ADC 读取电压
- 参数：
  - `port_num`：ADC 端口号
  - `buf`：ADC 电压缓冲区
  - `len`：ADC 电压缓冲区长度
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h` 中 `OS_ADAPTER_ADC` 定义部分

## 示例

### 示例 1

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

    // 输出 adc_value 的值

    ret = tkl_adc_deinit(ADC_NUM_0);
    if(ret != OPRT_OK) {
        // failed
        return;
    }
}
```

### 示例 2

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

    // 输出 adc_value[ADC_CHANNEL_NUM] 的值

    ret = tkl_adc_deinit(ADC_NUM_0);
    if(ret != OPRT_OK) {
        // failed
        return;
    }
}
```

