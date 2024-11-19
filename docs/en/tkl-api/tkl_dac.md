# DAC Driver

## Brief Description

DAC, Digital to Analog Converter, converts digital signals into analog signals. Its function is opposite to that of an ADC.

In common digital signal systems, most sensor signals are converted into voltage signals, and the ADC converts the voltage analog signals into digital codes that are easy for computers to store and process. After the computer processes them, the DAC outputs voltage analog signals, which are often used to drive certain actuators, making them easier for humans to perceive. For example, the acquisition and restoration of audio signals is such a process.

## API Description

### tkl_dac_init

```c
OPERATE_RET tkl_dac_init(TUYA_DAC_NUM_E port_num);
```

- Function Description:
  - Initialize the corresponding DAC instance through the port number and return the initialization result.
  
- Parameters:
  - port_num: Port number
  
- Return Value:
  - OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_deinit

```c
OPERATE_RET tkl_dac_deinit(TUYA_DAC_NUM_E port_num);
```

- Function Description:
  - De-initialize the DAC instance. This interface will stop the DAC.
  
- Parameters:
  - port_num: Port number.
  
- Return Value:
  - OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_controller_config

```c
OPERATE_RET tkl_dac_controller_config(TUYA_DAC_NUM_E port_num, TUYA_DAC_CMD_E cmd, void * argu);
```

- Function Description:
  - Configure the corresponding DAC device.
  
- Parameters:
  - port_num: Port number.
  - cmd: Command word

    **TUYA_DAC_CMD_E**:

    | Name                  | Meaning              |
    | --------------------- | ----------------- |
    | TUYA_DAC_WRITE_FIFO   | Set DAC's FIFO data |
    | TUYA_DAC_SET_BASE_CFG | Set DAC's basic configuration |
    
  - argu: Parameters can be one of the following two structures:

  ```c
  typedef struct{	
  	uint8_t *data;       //data
  	uint32_t len;        // data length
  }TUYA_DAC_DATA_T;
  ```

  ```c
  typedef struct {
      TUYA_AD_DA_CH_LIST_U  ch_list;       // dac channel list
      uint8_t  ch_nums;       // dac channel number
      uint8_t  width;         // output width
      uint32_t freq;          // convert freq
  } TUYA_DAC_BASE_CFG_T;
  ```

- Return Value:
  - OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_base_cfg_get

```c
OPERATE_RET  tkl_dac_base_cfg_get (TUYA_DAC_NUM_E port_num,TUYA_DAC_BASE_CFG_T *cfg);
```

- Function Description:
  - Get the basic configuration of the corresponding DAC.
  
- Parameters:
  
  - `port_num`: Port number.
  
  - `cfg`: DAC basic configuration structure
  
    ```c
    typedef struct {
        TUYA_AD_DA_CH_LIST_U  ch_list;       // dac channel list
        uint8_t  ch_nums;       // dac channel number
        uint8_t  width;         // output width
        uint32_t freq;          // convert freq
    } TUYA_DAC_BASE_CFG_T;
    ```
- Return Value:
  
  - OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_start

```c
OPERATE_RET tkl_dac_start(TUYA_DAC_NUM_E port_num);
```

- Function Description:
  - Start the corresponding DAC for conversion.
  
- Parameters:
  - `port_num`: Port number
  
- Return Value:
  - OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_stop

```c
OPERATE_RET tkl_dac_stop(TUYA_DAC_NUM_E port_num);
```

- Function Description:
  - Stop the corresponding DAC.
  
- Parameters:
  - `port_num`: Port number.
  
- Return Value:
  - Error code, OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

### tkl_dac_fifo_reset

```c
OPERATE_RET  tkl_dac_fifo_reset (TUYA_DAC_NUM_E port_num);
```

- Function Description:
  - Reset the FIFO of the corresponding DAC.
  
- Parameters:
  - `port_num`: Port number.
  
- Return Value:
  - Error code, OPRT_OK for success, others please refer to the definition section in the file `tuya_error_code.h`.

## Examples

### Example 1: DAC Example One

```c
/* Initialize the 0th DAC peripheral */
tkl_dac_init(0);

/* Configure the basic configuration of the 0th DAC */
TUYA_DAC_BASE_CFG_T dac_base_cfg;

dac_base_cfg.freq = 8000;  /* DAC conversion speed is 8000 times per second */
dac_base_cfg.width = 16;   /* DAC precision is 16 */
dac_base_cfg.ch_nums = 1;  /* DAC is single-channel output, port number is 2 (chs_cfg = 2) */
dac_base_cfg.ch_list.bits.ch_2 = 1;

tkl_dac_controller_config(0,TUYA_DAC_SET_BASE_CFG, &dac_base_cfg);

/* Write to DAC's FIFO */
TUYA_DAC_DATA_T data_fifo;
uint8_t dac_data[1024];

data_fifo.len = sizeof(dac_data);
data_fifo.data = dac_data;
tkl_dac_controller_config(0,TUYA_DAC_WRITE_FIFO, &data_fifo);

/* Start the 0th DAC for conversion */
tkl_dac_start(0);

/* Continuously write data to DAC's FIFO to let DAC convert */
while(1)
{
    data_fifo.len = sizeof(dac_data);
	data_fifo.data = dac_data;
	tkl_dac_controller_config(0,TUYA_DAC_WRITE_FIFO, &data_fifo);
}

/* Stop the 0th DAC for conversion */
tkl_dac_stop(0);

/* Deinitialize the 0th DAC */
tkl_dac_deinit(0);
```
