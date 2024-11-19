# GPIO 驱动

## 简要说明

GPIO（General Purpose I/O Ports）是通用输入/输出端口，通俗地说，就是一些引脚，可以通过它们输出高低电平或者通过它们读入引脚的状态是高电平或是低电平。

### GPIO 模式

| 模式          | 枚举 |
| ------------- | --------------------------- |
| 上拉输入       | TUYA_GPIO_PULLUP   			|
| 下拉输入       | TUYA_GPIO_PULLDOWN   		|
| 高阻输入       | TUYA_GPIO_HIGH_IMPEDANCE   	|
| 浮空输入       | TUYA_GPIO_FLOATING   		|
| 推完输出       | TUYA_GPIO_PUSH_PULL 			|
| 开漏输出       | TUYA_GPIO_OPENDRAIN    		|
| 开漏带上拉输出  | TUYA_GPIO_OPENDRAIN_PULLUP   |

以上模式需要关注芯片本身是否支持，这里只是模式的最大集合。



## API 描述

### tkl_gpio_init

```c
OPERATE_RET tkl_gpio_init(TUYA_GPIO_NUM_E pin_id, const TUYA_GPIO_BASE_CFG_T *cfg);
```

- 功能描述：
  - GPIO 初始化
  
- 参数：
  - `pin_id`：GPIO 引脚编号，此编号区别于芯片原始引脚号，是由涂鸦根据芯片 PA、PB ... PN 上的引脚个数顺序往下编号的：
  
    | 名字         | 定义   | 备注 |
    | :---------- | :----- | :---|
    | TUYA_GPIO_NUM_0 | 引脚 0 | 起始编号 |
    | TUYA_GPIO_NUM_1 | 引脚 1 |      |
    | TUYA_GPIO_NUM_3 | 引脚 2 |      |
    | ...  	      | 引脚 n |      |
    | TUYA_GPIO_NUM_60 | 引脚 60 | 最大编号 |
  
  - `cfg`：GPIO 基础配置，取值如下：
  
    ```c
    typedef struct {
        TUYA_GPIO_MODE_E  mode;		// gpio 模式
        TUYA_GPIO_DRCT_E  direct;	// gpio 输入输出方向
        TUYA_GPIO_LEVEL_E level;	// gpio 初始化电平
    } TUYA_GPIO_BASE_CFG_T;
    ```
    
    `mode` 定义如下：
    | 名字                | 定义                          | 备注 |
    | :----------------- | :---------------------------- | :--- |
    | TUYA_GPIO_PULLUP   			| 上拉输入       |  |
    | TUYA_GPIO_PULLDOWN   		    | 下拉输入       |  |
    | TUYA_GPIO_HIGH_IMPEDANCE   	| 高阻输入       |  |
    | TUYA_GPIO_FLOATING   		    | 浮空输入       |  |
    | TUYA_GPIO_PUSH_PULL 			| 推完输出       |  |
    | TUYA_GPIO_OPENDRAIN    		| 开漏输出       |  |
    | TUYA_GPIO_OPENDRAIN_PULLUP    | 开漏带上拉输出  |  |
    
    `direct` 定义如下：
    | 名字                | 定义                  | 备注 |
    | :----------------- | :-------------------- | :--- |
    | TUYA_GPIO_INPUT   			| 输入模式       |  |
    | TUYA_GPIO_OUTPUT   		    | 输出模式       |  |
    
    `level` 定义如下：
    
    | 名字                | 定义                  | 备注 |
    | :------------------- | :-------------------- | :--- |
    | TUYA_GPIO_LEVEL_LOW  | 低电平       |  |
    | TUYA_GPIO_LEVEL_HIGH | 高电平       |  |
  
- 返回值：
  
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_deinit

```c
OPERATE_RET tkl_gpio_deinit(TUYA_GPIO_NUM_E pin_id)
```

- 功能描述：
  - GPIO 恢复初始状态
- 参数：
  - `pin_id`：GPIO 引脚编号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_write

```c
OPERATE_RET tkl_gpio_write(TUYA_GPIO_NUM_E pin_id, TUYA_GPIO_LEVEL_E level);
```

- 功能描述：
  - GPIO 输出电平
- 参数：
  - `pin_id`：GPIO 引脚编号
  - `level`： GPIO 输出电平
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_read

```c
OPERATE_RET tkl_gpio_read(TUYA_GPIO_NUM_E pin_id, TUYA_GPIO_LEVEL_E *level);
```

- 功能描述：
  - GPIO 读取电平
- 参数：
  - `pin_id`：GPIO 引脚编号
  - `*level`：GPIO 读取电平返回值
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_irq_init

```c
OPERATE_RET tkl_gpio_irq_init(TUYA_GPIO_NUM_E pin_id, const TUYA_GPIO_IRQ_T *cfg);
```
- 功能描述：
  - GPIO 中断初始
- 参数：
  - `pin_id`：GPIO 引脚编号
  - `*cfg`：    GPIO 中断配置
 - `cfg`：GPIO 基础配置，取值如下：
  
    ```c
    typedef struct {
        TUYA_GPIO_IRQ_E      mode;	// 中断模式
        TUYA_GPIO_IRQ_CB     cb;	// 中断回调函数
        void              *arg;	// 回调函数参数
    } TUYA_GPIO_IRQ_T;
    ```
    `mode` 定义如下：
    
    | 名字                | 定义                  | 备注 |
    | :------------------- | :-------------------- | :--- |
    | TUYA_GPIO_IRQ_RISE   			|上升沿模式       |  |
    | TUYA_GPIO_IRQ_FALL   		    |下降沿模式       |  |
    | TUYA_GPIO_IRQ_RISE_FALL   	|双边沿模式       |  |
    | TUYA_GPIO_IRQ_LOW   		    |低电平模式       |  |
    | TUYA_GPIO_IRQ_HIGH 			|搞电平模式       |  |
    
    
    `cb` 定义如下：
     ```c
    typedef void (*TUYA_GPIO_IRQ_CB)(void *args);
     ```
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_irq_enable

```c
OPERATE_RET tkl_gpio_irq_enable(TUYA_GPIO_NUM_E pin_id);
```

- 功能描述：
  - GPIO 中断使能函数
- 参数：
  - `pin_id`：GPIO 引脚编号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分

### tkl_gpio_irq_disable

```c
OPERATE_RET tkl_gpio_irq_disable(TUYA_GPIO_NUM_E pin_id);
```

- 功能描述：
  - GPIO 中断失能函数
- 参数：
  - `pin_id`：GPIO 引脚编号
- 返回值：
  - OPRT_OK - 成功
  - 其他请参考文件 `tuya_error_code.h``OPRT_OS_ADAPTER_GPIO_ERRCODE` 定义部分


## 示例

### 示例 1

```c
// gpio 初始化
void tuya_gpio_test(void)
{
    TUYA_GPIO_BASE_CFG_T cfg = {
        .mode = TUYA_GPIO_PUSH_PULL,
        .direct = TUYA_GPIO_OUTPUT,
        .level = TUYA_GPIO_LEVEL_LOW,
    };
	  tkl_gpio_init(GPIO_NUM_3, &cfg);
    tkl_gpio_init(GPIO_NUM_4, &cfg);
    
    // gpio 输出
    tkl_gpio_write(GPIO_NUM_3, TUYA_GPIO_LEVEL_HIGH);
    tkl_gpio_write(GPIO_NUM_4, TUYA_GPIO_LEVEL_HIGH);
}
```

### 示例 2

```c
// 中断回调函数
static void __gpio_irq_callback7(void *args)
{
    //to do
}

static void __gpio_irq_callback8(void *args)
{
     //to do
}
// gpio 中断初始
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

