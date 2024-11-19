# thread

文件 `tkl_thread.c` 提供了操作系统线程（任务）创建、终止、获取信息等API，适用于涂鸦智能设备中的多任务环境。本文件是由涂鸦操作系统（TuyaOS）自动生成的，其中留有一定范围供开发者在标记的区域内自定义和实现相关逻辑。

## API 说明

### tkl_thread_create

```c
OPERATE_RET tkl_thread_create(TKL_THREAD_HANDLE* thread,
                              const char* name,
                              uint32_t stack_size,
                              uint32_t priority,
                              const THREAD_FUNC_T func,
                              void* const arg);
```

#### 功能

创建一个任务线程。

#### 参数

- `thread`: 输出参数，用于返回创建的线程句柄。
- `name`: 线程的名称。
- `stack_size`: 线程栈大小。
- `priority`: 线程优先级。
- `func`: 线程执行的函数指针。
- `arg`: 传递给线程函数的参数。

#### 返回值

返回 `OPRT_OK` 代表创建成功，其他值代表错误，具体错误代码见 `tuya_error_code.h`。



### tkl_thread_release

```c
OPERATE_RET tkl_thread_release(const TKL_THREAD_HANDLE thread);
```

#### 功能

终止一个线程并释放其资源。

#### 参数

- `thread`: 待终止的线程句柄。

#### 返回值

返回 `OPRT_OK` 代表成功，其他值代表错误，详见 `tuya_error_code.h`。

### tkl_thread_get_watermark

```c
OPERATE_RET tkl_thread_get_watermark(const TKL_THREAD_HANDLE thread, uint32_t* watermark);
```

#### 功能

获取线程栈的最高水位（最小剩余栈空间）。

#### 参数

- `thread`: 线程句柄。
- `watermark`: 输出参数，栈的最高水位字节数。

#### 返回值

返回 `OPRT_OK` 表示获取成功，其他值为失败。

### tkl_thread_get_id

```c
OPERATE_RET tkl_thread_get_id(TKL_THREAD_HANDLE *thread);
```

#### 功能

获取当前线程的句柄。

#### 参数

- `thread`: 输出参数，当前线程句柄。

#### 返回值

总是返回 `OPRT_OK`，表示调用成功。

### tkl_thread_set_self_name

```c
OPERATE_RET tkl_thread_set_self_name(const char* name);
```

#### 功能

设置当前线程的名称。

#### 参数

- `name`: 线程的新名称。

#### 返回值

返回值 `OPRT_OK` 表示成功，其他值代表错误。

### tkl_thread_is_self

```c
OPERATE_RET tkl_thread_is_self(TKL_THREAD_HANDLE thread, BOOL_T* is_self);
```

#### 功能

判断指定的线程是否为当前线程。

#### 参数

- `thread`: 线程句柄。
- `is_self`: 输出参数，用于指示是否为当前线程。

#### 返回值

返回 `OPRT_OK` 代表成功，其他值代表失败。

### tkl_thread_get_priority

```c
OPERATE_RET tkl_thread_get_priority(TKL_THREAD_HANDLE thread, int *priority);
```

#### 功能

获取线程的优先级。

#### 参数

- `thread`: 线程句柄，如果为 NULL 则表示获取当前线程的优先级。
- `priority`: 输出参数，用于返回线程优先级。

#### 返回值

返回 `OPRT_OK` 代表成功，其他值代表失败。

### tkl_thread_set_priority

```c
OPERATE_RET tkl_thread_set_priority(TKL_THREAD_HANDLE thread, int priority);
```

#### 功能

设置线程优先级。

#### 参数

- `thread`: 线程句柄，如果为 NULL 则表示设置当前线程的优先级。
- `priority`: 新的线程优先级。

#### 返回值

返回 `OPRT_OK` 代表成功，其他值代表失败。

### tkl_thread_diagnose

```c
OPERATE_RET tkl_thread_diagnose(TKL_THREAD_HANDLE thread);
```

#### 功能

诊断线程，如打印任务栈信息等。

#### 参数

- `thread`: 线程句柄。

#### 返回值

返回 `OPRT_OK` 表示诊断成功。

