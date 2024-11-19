# LWIP

`tkl_lwip.c` 文件包含底层以太网接口的初始化和数据包的发送接收功能实现，依赖于 lwIP 网络协议栈。此文件是由涂鸦智能操作系统自动生成和维护的，开发者可以在用户定义的 "BEGIN" 和 "END" 注释之间添加自己的实现代码。此种设计能够在涂鸦智能操作系统的版本和验证工具执行时，保留用户的自定义更改，而其他部分则会被自动生成的更新覆盖。

注意：该文件中的接口仅在 `ENABLE_LIBLWIP` 开启的时候需要适配。

## API 说明

### tkl_ethernetif_init

```c
OPERATE_RET tkl_ethernetif_init(TKL_NETIF_HANDLE netif);
```

#### 功能

初始化以太网接口硬件。

#### 参数

- `netif`：指向网络接口结构的指针，将用于发送数据包。

#### 返回值

- `err_t`：见 `err_enum_t` 在 `lwip/err.h` ，成功时返回 `ERR_OK`，失败时返回其他错误码。

### tkl_ethernetif_output

```c
OPERATE_RET tkl_ethernetif_output(TKL_NETIF_HANDLE netif, TKL_PBUF_HANDLE p);
```

#### 功能

通过以太网接口发送 pbuf 格式的数据包。

#### 参数

- `netif`：网络接口的指针，该接口将用于发送数据包。
- `p`：以 pbuf 格式表示的待发送数据包。

#### 返回值

- `err_t`：见 `err_enum_t` 在 `lwip/err.h` ，成功时返回 `ERR_OK`，失败时返回其他错误码。

### tkl_ethernetif_recv

```c
OPERATE_RET tkl_ethernetif_recv(TKL_NETIF_HANDLE netif, TKL_PBUF_HANDLE p);
```

#### 功能

接收来自以太网接口的数据包。

#### 参数

- `netif`：网络接口的指针，用于接收数据包。
- `p`：表示接收数据的pbuf结构。

#### 返回值

- `err_t`：见 `err_enum_t` 在 `lwip/err.h` ，成功时返回E RR_OK，失败时返回其他错误码。
