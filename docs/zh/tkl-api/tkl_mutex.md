# mutex

文件 `tkl_mutex.c` 用于创建和管理互斥锁，在嵌入式系统或多任务操作系统中，以确保资源的同步访问。文件中定义的函数能够创建互斥锁、加锁、尝试加锁、解锁以及销毁互斥锁。该文件是由涂鸦操作系统（TuyaOS）自动生成的，并且允许开发者在指定区域添加或修改代码实现。

## API 说明

### tkl_mutex_create_init

```c
OPERATE_RET tkl_mutex_create_init(TKL_MUTEX_HANDLE *handle);
```

#### 功能

创建并初始化一个互斥锁。

#### 参数

- `handle`: 输出参数，用于存储创建的互斥锁句柄。

#### 返回值

返回值为 `OPRT_OK` 表示成功创建互斥锁，其他值表示发生错误，具体错误代码请参照 `tuya_error_code.h`。

### tkl_mutex_lock

```c
OPERATE_RET tkl_mutex_lock(const TKL_MUTEX_HANDLE handle);
```

#### 功能

锁定一个互斥锁。如果互斥锁已被其他任务锁定，调用此函数的任务会被挂起，直到互斥锁可用。

#### 参数

- `handle`: 输入参数，互斥锁句柄。

#### 返回值

返回值为 `OPRT_OK` 表示成功锁定互斥锁，失败时返回相应的错误代码，可以从 `tuya_error_code.h` 获取详细信息。

### tkl_mutex_trylock

```c
OPERATE_RET tkl_mutex_trylock(const TKL_MUTEX_HANDLE mutexHandle);
```

#### 功能

尝试锁定一个互斥锁，但不会因互斥锁被占用而挂起调用任务。

#### 参数

- `mutexHandle`: 输入参数，互斥锁句柄。

#### 返回值

返回值为 `OPRT_OK` 表示成功尝试锁定互斥锁，错误值表示发生了错误，详细错误代码参照 `tuya_error_code.h`。

### tkl_mutex_unlock

```c
OPERATE_RET tkl_mutex_unlock(const TKL_MUTEX_HANDLE handle);
```

#### 功能

解锁一个已锁定的互斥锁。

#### 参数

- `handle`: 输入参数，互斥锁句柄。

#### 返回值

返回值为 `OPRT_OK` 表明成功解锁了互斥锁，其他值表示发生错误，详细信息请参考 `tuya_error_code.h`。

### tkl_mutex_release

```c
OPERATE_RET tkl_mutex_release(const TKL_MUTEX_HANDLE handle);
```

#### 功能

释放并删除一个互斥锁。

#### 参数

- `handle`: 输入参数，互斥锁句柄。

#### 返回值

返回值为 `OPRT_OK` 表示成功释放资源，其他值代表发生错误，错误信息可以通过 `tuya_error_code.h` 获得。
