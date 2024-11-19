# network

`tkl_network.c` 文件提供了一系列网络通信过程中所需的 API，以适配不同的网络接口。这些 API 包括创建套接字、连接、绑定、监听、发送数据、接收数据、设置和获取套接字选项等网络操作。文件定义了对网络 API 的常见处理，并允许通过这些封装的函数来实现跨平台的网络通信。

## API 说明

### tkl_net_get_errno

```c
TUYA_ERRNO tkl_net_get_errno(void);
```

#### 功能

获取网络错误码。

#### 参数

无参数。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_fd_set

```c
OPERATE_RET tkl_net_fd_set(const int fd, TUYA_FD_SET_T* fds);
```

#### 功能

将文件描述符添加到集合中。

#### 参数

- `fd`：待添加的文件描述符。
- `fds`：文件描述符集合的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_fd_clear

```c
OPERATE_RET tkl_net_fd_clear(const int fd, TUYA_FD_SET_T* fds);
```

#### 功能

从集合中清除文件描述符。

#### 参数

- `fd`：待清除的文件描述符。
- `fds`：文件描述符集合的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_fd_isset

```c
OPERATE_RET tkl_net_fd_isset(const int fd, TUYA_FD_SET_T* fds);
```

#### 功能

检查文件描述符是否在集合中。

#### 参数

- `fd`：待检查的文件描述符。
- `fds`：文件描述符集合的指针。

#### 返回值

如果文件描述符在集合中，返回 `TRUE`，否则返回 `FALSE`。

### tkl_net_fd_zero

```c
OPERATE_RET tkl_net_fd_zero(TUYA_FD_SET_T* fds);
```

#### 功能

清除文件描述符集合中的所有文件描述符。

#### 参数

- `fds`：文件描述符集合的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_select

```c
int tkl_net_select(const int maxfd, TUYA_FD_SET_T *readfds, TUYA_FD_SET_T *writefds, TUYA_FD_SET_T *errorfds, const uint32_t ms_timeout);
```

#### 功能

获取可用的文件描述符集合。

#### 参数

- `maxfd`：要检查的文件描述符集合中的最大文件描述符数加一。
- `readfds`：可读取文件描述符集合的指针。
- `writefds`：可写文件描述符集合的指针。
- `errorfds`：出错的文件描述符集合的指针。
- `ms_timeout`：超时时间，单位为毫秒。

#### 返回值

返回可用文件描述符的数量；错误时返回小于或等于 0 的值。

### tkl_net_get_nonblock

```c
int tkl_net_get_nonblock(const int fd);
```

#### 功能

获取非阻塞文件描述符。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回非阻塞文件描述符的数量；错误时返回小于或等于 0 的值。

### tkl_net_set_block

```c
OPERATE_RET tkl_net_set_block(const int fd, const BOOL_T block);
```

#### 功能

设置文件描述符阻塞标志。

#### 参数

- `fd`：文件描述符。
- `block`：若为 `TRUE` 则设置为阻塞模式；`FALSE` 则设置为非阻塞模式。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_close

```c
TUYA_ERRNO tkl_net_close(const int fd);
```

#### 功能

关闭文件描述符。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_shutdown

```c
TUYA_ERRNO tkl_net_shutdown(const int fd, const int how);
```

#### 功能

关闭文件描述符的读写操作。

#### 参数

- `fd`：文件描述符。
- `how`：关闭类型。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_socket_create

```c
int tkl_net_socket_create(const TUYA_PROTOCOL_TYPE_E type);
```

#### 功能

创建 TCP 或 UDP 套接字。

#### 参数

- `type`：协议类型，TCP 或 UDP。

#### 返回值

返回文件描述符。

### tkl_net_connect

```c
TUYA_ERRNO tkl_net_connect(const int fd, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### 功能

连接到网络。

#### 参数

- `fd`：文件描述符。
- `addr`：服务器的地址信息。
- `port`：服务器的端口信息。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_connect_raw

```c
TUYA_ERRNO tkl_net_connect_raw(const int fd, void *p_socket_addr, const int len);
```

#### 功能

使用原始数据连接到网络。

#### 参数

- `fd`：文件描述符。
- `p_socket`：原始套接字数据。
- `len`：数据长度。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_bind

```c
TUYA_ERRNO tkl_net_bind(const int fd, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### 功能

将套接字绑定到网络。

#### 参数

- `fd`：文件描述符。
- `addr`：服务器的地址信息。
- `port`：服务器的端口信息。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_listen

```c
TUYA_ERRNO tkl_net_listen(const int fd, const int backlog);
```

#### 功能

监听网络连接请求。

#### 参数

- `fd`：文件描述符。
- `backlog`：等待队列中最大可能的连接数。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_accept

```c
TUYA_ERRNO tkl_net_accept(const int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### 功能

接受一个网络连接请求。

#### 参数

- `fd`：文件描述符。
- `addr`：接收端的IP地址。
- `port`：接收端的端口号。

#### 返回值

成功时返回 0，否则返回特定于目标系统的错误码。

### tkl_net_send

```c
TUYA_ERRNO tkl_net_send(const int fd, const void *buf, const uint32_t nbytes);
```

#### 功能

将数据发送到网络。

#### 参数

- `fd`：文件描述符。
- `buf`：发送的数据缓冲区。
- `nbytes`：缓冲区长度。

#### 返回值

发送的数据的字节数量；错误时返回小于 0 的值。

### tkl_net_send_to

```c
TUYA_ERRNO tkl_net_send_to(const int fd, const void *buf, const uint32_t nbytes, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### 功能

将数据发送给指定的服务器。

#### 参数

- `fd`：文件描述符。
- `buf`：发送的数据缓冲区。
- `nbytes`：缓冲区长度。
- `addr`：服务器的地址信息。
- `port`：服务器的端口信息。

#### 返回值

发送的数据的字节数量；错误时返回小于 0 的值。

### tkl_net_recv

```c
TUYA_ERRNO tkl_net_recv(const int fd, void *buf, const uint32_t nbytes);
```

#### 功能

从网络接收数据。

#### 参数

- `fd`：文件描述符。
- `buf`：接收数据的缓冲区。
- `nbytes`：缓冲区长度。

#### 返回值

接收的数据的字节数量；错误时返回小于 0 的值。

### tkl_net_recv_nd_size

```c
int tkl_net_recv_nd_size(const int fd, void *buf, const uint32_t buf_size, const uint32_t nd_size);
```

#### 功能

从网络接收指定大小的数据。

#### 参数

- `fd`：文件描述符。
- `buf`：接收数据的缓冲区。
- `buf_size`：缓冲区的大小。
- `nd_size`：期望接收的数据大小。

#### 返回值

成功时返回实际接收的数据量，错误时返回小于 0 的值。

### tkl_net_recvfrom

```c
TUYA_ERRNO tkl_net_recvfrom(const int fd, void *buf, const uint32_t nbytes, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### 功能

从指定的服务器接收数据。

#### 参数

- `fd`：文件描述符。
- `buf`：接收数据的缓冲区。
- `nbytes`：缓冲区长度。
- `addr`[OUT]：服务器的地址信息。
- `port`[OUT]：服务器的端口信息。

#### 返回值

接收的数据的字节数量；错误时返回小于 0 的值。

### tkl_net_gethostbyname

```c
OPERATE_RET tkl_net_gethostbyname(const char *domain, TUYA_IP_ADDR_T *addr);
```

#### 功能

通过域名获取地址信息。

#### 参数

- `domain`：域名信息。
- `addr`：地址信息存储的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_socket_bind

```c
OPERATE_RET tkl_net_socket_bind(const int fd, const char *ip);
```

#### 功能

将套接字绑定到具有指定 IP 的网络。

#### 参数

- `fd`：文件描述符。
- `ip`：IP地址。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_cloexec

```c
OPERATE_RET tkl_net_set_cloexec(const int fd);
```

#### 功能

设置套接字在 `fork` 调用的子进程中不关闭。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_get_socket_ip

```c
OPERATE_RET tkl_net_get_socket_ip(const int fd, TUYA_IP_ADDR_T *addr);
```

#### 功能

通过套接字描述符获取 IP 地址。

#### 参数

- `fd`：文件描述符。
- `addr`：IP地址存储的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_str2addr

```c
TUYA_IP_ADDR_T tkl_net_str2addr(const char *ip_str);
```

#### 功能

将IP字符串转换为地址。

#### 参数

- `ip_str`：IP 字符串。

#### 返回值

返回转换后的IP地址。

### tkl_net_addr2str

```c
char* tkl_net_addr2str(const TUYA_IP_ADDR_T ipaddr);
```

#### 功能

将 IP 地址转换为字符串。

#### 参数

- `ipaddr`：IP  地址。

#### 返回值

返回转换后的IP字符串。

### tkl_net_setsockopt

```c
OPERATE_RET tkl_net_setsockopt(const int fd, const TUYA_OPT_LEVEL level, const TUYA_OPT_NAME optname, const void *optval, const int optlen);
```

#### 功能

设置套接字选项。

#### 参数

- `fd`：文件描述符。
- `level`：设置级别。
- `optname`：选项名。
- `optval`：选项值的指针。
- `optlen`：选项值的长度。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_getsockopt

```c
OPERATE_RET tkl_net_getsockopt(const int fd, const TUYA_OPT_LEVEL level, const TUYA_OPT_NAME optname, void *optval, int *optlen);
```

#### 功能

获取套接字选项值。

#### 参数

- `fd`：文件描述符。
- `level`：获取级别。
- `optname`：选项名。
- `optval`：用于接收选项值的指针。
- `optlen`：用于接收选项值长度的指针。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_timeout

```c
OPERATE_RET tkl_net_set_timeout(const int fd, const int ms_timeout, const TUYA_TRANS_TYPE_E type);
```

#### 功能

设置套接字超时选项。

#### 参数

- `fd`：文件描述符。
- `ms_timeout`：超时时间，以毫秒为单位。
- `type`：传输类型，接收或发送。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_bufsize

```c
OPERATE_RET tkl_net_set_bufsize(const int fd, const int buf_size, const TUYA_TRANS_TYPE_E type);
```

#### 功能

设置套接字缓冲区大小选项。

#### 参数

- `fd`：文件描述符。
- `buf_size`：缓冲区大小，以字节为单位。
- `type`：传输类型，接收或发送。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_reuse

```c
OPERATE_RET tkl_net_set_reuse(const int fd);
```

#### 功能

启用套接字的重用选项。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_disable_nagle

```c
OPERATE_RET tkl_net_disable_nagle(const int fd);
```

#### 功能

禁用套接字的Nagle算法。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_broadcast

```c
OPERATE_RET tkl_net_set_broadcast(const int fd);
```

#### 功能

启用套接字的广播选项。

#### 参数

- `fd`：文件描述符。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。

### tkl_net_set_keepalive

```c
OPERATE_RET tkl_net_set_keepalive(int fd, const BOOL_T alive, const uint32_t idle, const uint32_t intr, const uint32_t cnt);
```

#### 功能

设置套接字的 `keepalive` 选项以监视连接。

#### 参数

- `fd`：文件描述符。
- `alive`：`keepalive` 选项，启用或禁用。
- `idle`：`keep idle` 选项，闲置时间（秒）；如果连接在该时间内没有数据交换，则开始探测。
- `intr`：`keep interval` 选项，探测时间间隔。
- `cnt`：`keep count` 选项，探测次数。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。


### tkl_net_getsockname

```c
OPERATE_RET tkl_net_getsockname(int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### 功能

获取特定的 socket 的信息。

#### 参数

- `fd`：文件描述符。
- `addr`：源 IP 地址。
- `port`：源端口。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。


### tkl_net_getpeername

```c
OPERATE_RET tkl_net_getpeername(int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### 功能

获取特定的 socket 的目的信息。

#### 参数

- `fd`：文件描述符。
- `addr`：目的 IP 地址。
- `port`：目的端口。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。


### tkl_net_sethostname

```c
OPERATE_RET tkl_net_sethostname(const char *hostname);
```

#### 功能

设置系统 hostname，该 hostname 会在路由器上展示。

#### 参数

- `hostname`：主机名。

#### 返回值

成功时返回 `OPRT_OK`，否则请参考 `tuya_error_code.h` 中的错误码。