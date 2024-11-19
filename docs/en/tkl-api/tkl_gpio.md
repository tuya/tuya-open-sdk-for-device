# GPIO Driver

## Brief Description

GPIO (General Purpose Input/Output Ports) are pins that can output high or low levels or read the state of the pin (high or low level).

### GPIO Modes

| Mode            | Enumeration                          |
| --------------- | ----------------------------------- |
| Pull-up Input   | TUYA_GPIO_PULLUP                    |
| Pull-down Input | TUYA_GPIO_PULLDOWN                  |
| High-impedance Input | TUYA_GPIO_HIGH_IMPEDANCE |
| Floating Input  | TUYA_GPIO_FLOATING                  |
| Push-pull Output| TUYA_GPIO_PUSH_PULL                 |
| Open-drain Output | TUYA_GPIO_OPENDRAIN                |
| Open-drain with Pull-up Output | TUYA_GPIO_OPENDRAIN_PULLUP |

The above modes depend on whether the chip itself supports them. This is just the maximum set of modes.

## API Description

### tkl_gpio_init

```c
OPERATE_RET tkl_gpio_init(TUYA_GPIO_NUM_E pin_id, const TUYA_GPIO_BASE_CFG_T *cfg);
```

- Function Description:
  - GPIO Initialization
  
- Parameters:
  - `pin_id`: GPIO pin number, this number is different from the original chip pin number, it is numbered sequentially by Tuya according to the number of pins on the chip's PA, PB ... PN:
  
    | Name         | Definition | Remarks |
    | :---------- | :--------- | :------|
    | TUYA_GPIO_NUM_0 | Pin 0 | Starting number |
    | TUYA_GPIO_NUM_1 | Pin 1 |      |
    | TUYA_GPIO_NUM_3 | Pin 2 |      |
    | ...          | Pin n |      |
    | TUYA_GPIO_NUM_60 | Pin 60 | Maximum number |
  
  - `cfg`: GPIO base configuration, values as follows:
  
    ```c
    typedef struct {
        TUYA_GPIO_MODE_E  mode;    // GPIO mode
        TUYA_GPIO_DRCT_E  direct;  // GPIO input/output direction
        TUYA_GPIO_LEVEL_E level;   // GPIO initial level
    } TUYA_GPIO_BASE_CFG_T;
    ```
    
    `mode` is defined as:
    | Name                | Definition                          | Remarks |
    | :----------------- | :--------------------------------- | :------ |
    | TUYA_GPIO_PULLUP    | Pull-up Input                       |         |
    | TUYA_GPIO_PULLDOWN  | Pull-down Input                     |         |
    | TUYA_GPIO_HIGH_IMPEDANCE | High-impedance Input              |         |
    | TUYA_GPIO_FLOATING  | Floating Input                      |         |
    | TUYA_GPIO_PUSH_PULL | Push-pull Output                    |         |
    | TUYA_GPIO_OPENDRAIN | Open-drain Output                   |         |
    | TUYA_GPIO_OPENDRAIN_PULLUP | Open-drain with Pull-up Output |         |
    
    `direct` is defined as:
    | Name                | Definition                  | Remarks |
    | :----------------- | :-------------------------- | :------ |
    | TUYA_GPIO_INPUT     | Input Mode                  |         |
    | TUYA_GPIO_OUTPUT    | Output Mode                 |         |
    
    `level` is defined as:
    
    | Name                | Definition                  | Remarks |
    | :------------------- | :-------------------------- | :------ |
    | TUYA_GPIO_LEVEL_LOW  | Low Level                   |         |
    | TUYA_GPIO_LEVEL_HIGH | High Level                  |         |
  
- Return Value:
  
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_deinit

```c
OPERATE_RET tkl_gpio_deinit(TUYA_GPIO_NUM_E pin_id)
```

- Function Description:
  - Restore GPIO to initial state
- Parameters:
  - `pin_id`: GPIO pin number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_write

```c
OPERATE_RET tkl_gpio_write(TUYA_GPIO_NUM_E pin_id, TUYA_GPIO_LEVEL_E level);
```

- Function Description:
  - GPIO output level
- Parameters:
  - `pin_id`: GPIO pin number
  - `level`: GPIO output level
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_read

```c
OPERATE_RET tkl_gpio_read(TUYA_GPIO_NUM_E pin_id, TUYA_GPIO_LEVEL_E *level);
```

- Function Description:
  - GPIO read level
- Parameters:
  - `pin_id`: GPIO pin number
  - `*level`: GPIO read level return value
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_irq_init

```c
OPERATE_RET tkl_gpio_irq_init(TUYA_GPIO_NUM_E pin_id, const TUYA_GPIO_IRQ_T *cfg);
```

- Function Description:
  - GPIO interrupt initialization
- Parameters:
  - `pin_id`: GPIO pin number
  - `*cfg`: GPIO interrupt configuration
  - `cfg`: GPIO base configuration, values as follows:
  
    ```c
    typedef struct {
        TUYA_GPIO_IRQ_E      mode;  // Interrupt mode
        TUYA_GPIO_IRQ_CB     cb;    // Interrupt callback function
        void              *arg;   // Callback function argument
    } TUYA_GPIO_IRQ_T;
    ```
    `mode` is defined as:
    
    | Name                | Definition                  | Remarks |
    | :------------------- | :-------------------------- | :------ |
    | TUYA_GPIO_IRQ_RISE   | Rising Edge Mode            |         |
    | TUYA_GPIO_IRQ_FALL   | Falling Edge Mode           |         |
    | TUYA_GPIO_IRQ_RISE_FALL | Both Edges Mode            |         |
    | TUYA_GPIO_IRQ_LOW    | Low Level Mode              |         |
    | TUYA_GPIO_IRQ_HIGH   | High Level Mode             |         |
    
    
    `cb` is defined as:
     ```c
    typedef void (*TUYA_GPIO_IRQ_CB)(void *args);
     ```
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_irq_enable

```c
OPERATE_RET tkl_gpio_irq_enable(TUYA_GPIO_NUM_E pin_id);
```

- Function Description:
  - Enable GPIO interrupt
- Parameters:
  - `pin_id`: GPIO pin number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

### tkl_gpio_irq_disable

```c
OPERATE_RET tkl_gpio_irq_disable(TUYA_GPIO_NUM_E pin_id);
```

- Function Description:
  - Disable GPIO interrupt
- Parameters:
  - `pin_id`: GPIO pin number
- Return Value:
  - OPRT_OK - Success
  - Others, please refer to the `OPRT_OS_ADAPTER_GPIO_ERRCODE` section in the file `tuya_error_code.h`

## Examples

### Example 1

```c
// GPIO Initialization
void tuya_gpio_test(void)
{
    TUYA_GPIO_BASE_CFG_T cfg = {
        .mode = TUYA_GPIO_PUSH_PULL,
        .direct = TUYA_GPIO_OUTPUT,
        .level = TUYA_GPIO_LEVEL_LOW,
    };
    tkl_gpio_init(GPIO_NUM_3, &cfg);
    tkl_gpio_init(GPIO_NUM_4, &cfg);
    
    // GPIO Output
    tkl_gpio_write(GPIO_NUM_3, TUYA_GPIO_LEVEL_HIGH);
    tkl_gpio_write(GPIO_NUM_4, TUYA_GPIO_LEVEL_HIGH);
}
```

### Example 2

```c
// Interrupt Callback Function
static void __gpio_irq_callback7(void *args)
{
    //to do
}

static void __gpio_irq_callback8(void *args)
{
     //to do
}
// GPIO Interrupt Initialization
void tuya_gpio_irq_test(void)
{
    TUYA_GPIO_IRQ_T irq_cfg_7 = {
        .mode = TUYA_GPIO_IRQ_RISE,
        .cb = __gpio_irq_callback7,
        .arg = NULL,
    };
    TUYA_GPIO_IRQ_T irq_cfg_8 = {
        .mode = TUYA_GPIO_IRQ_RISE,
        .cb = __gpio_irq_callback8,
        .arg = NULL,
    };
    tkl_gpio_irq_init(GPIO_NUM_7, &irq_cfg_7);
    tkl_gpio_irq_init(GPIO_NUM_8, &irq_cfg_8);
    tKl_gpio_irq_enable(GPIO_NUM_7);
    tKl_gpio_irq_enable(GPIO_NUM_8);
}
```
