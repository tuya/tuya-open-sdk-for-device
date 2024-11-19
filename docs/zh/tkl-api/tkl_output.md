# 日志输出

`tkl_output.c` 文件提供了日志输出功能，允许在涂鸦智能系统（TuyaOS）环境中进行日志的输出、关闭和打开操作。该文件是自动生成的，旨在让开发者在特定的区域内添加自己的实现代码。

## API 说明

### tkl_log_output

```c
void tkl_log_output(const char *format, ...);
```

#### 功能

输出日志信息。

#### 参数

- `format`: 一个指向格式化字符串的指针，后面可跟随变长参数列表。

#### 返回值

无返回值。


### tkl_log_close

```c
OPERATE_RET tkl_log_close(void);
```

#### 功能

关闭日志端口。

#### 参数

无参数。

#### 返回值

返回 `OPRT_OK` 表示成功关闭日志端口。其他值表示失败，具体错误代码见 `tuya_error_code.h`。

### tkl_log_open

```c
OPERATE_RET tkl_log_open(void);
```

#### 功能

打开日志端口。

#### 参数

无参数。

#### 返回值

返回 `OPRT_OK` 表示成功打开日志端口。其他值表示失败，具体错误代码见 `tuya_error_code.h`。
