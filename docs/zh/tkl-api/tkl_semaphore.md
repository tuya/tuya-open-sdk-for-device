# semaphore

文件 `tkl_semaphore.c` 用于创建和管理信号量，用于嵌入式系统或多任务操作系统中实现任务间同步或事件通知。本文件提供了创建信号量、等待信号量、发送信号量以及释放信号量的接口。文件同样是由涂鸦操作系统（TuyaOS）自动生成，并为开发者预留了代码实现的区域。

## API 说明

### tkl_semaphore_create_init

```c
OPERATE_RET tkl_semaphore_create_init(TKL_SEM_HANDLE *handle, uint32_t sem_cnt, uint32_t sem_max);
```

#### 功能

创建并初始化一个计数信号量。

#### 参数

- `handle`: 输出参数，用于接收创建的信号量句柄。
- `sem_cnt`: 信号量的初始计数。
- `sem_max`: 信号量的最大计数。

#### 返回值

返回值为 `OPRT_OK` 表示成功创建信号量，其余返回值表示出现错误，具体错误码可参见 `tuya_error_code.h`。

### tkl_semaphore_wait

```c
OPERATE_RET tkl_semaphore_wait(const TKL_SEM_HANDLE handle, uint32_t timeout);
```

#### 功能

等待一个信号量。

#### 参数

- `handle`: 信号量句柄。
- `timeout`: 等待超时时间，单位毫秒。`TKL_SEM_WAIT_FOREVER` 表示一直等待直到获得信号量。

#### 返回值

`OPRT_OK` 表示成功获取信号量，`OPRT_OS_ADAPTER_SEM_WAIT_TIMEOUT` 表示发生超时，其他返回值表示出现错误，具体错误码可查阅 `tuya_error_code.h`。

### tkl_semaphore_post

```c
OPERATE_RET tkl_semaphore_post(const TKL_SEM_HANDLE handle);
```

#### 功能

发送（释放）一个信号量，增加信号量的计数。

#### 参数

- `handle`: 信号量句柄。

#### 返回值

`OPRT_OK` 表示成功发送信号量，其它返回值表示出现错误，详细的错误码可查询 `tuya_error_code.h`。

### tkl_semaphore_release

```c
OPERATE_RET tkl_semaphore_release(const TKL_SEM_HANDLE handle);
```

#### 功能

释放并删除一个信号量。

#### 参数

- `handle`: 信号量句柄。

#### 返回值

`OPRT_OK` 表示成功释放资源，其他返回值代表发生错误，可以通过 `tuya_error_code.h` 获得详细信息。

