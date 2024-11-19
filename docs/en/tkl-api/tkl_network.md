# Network

The `tkl_network.c` file provides a series of APIs required for network communication processes to adapt to different network interfaces. These APIs include creating sockets, connecting, binding, listening, sending data, receiving data, setting and getting socket options, and other network operations. The file defines common handling of network APIs and allows for cross-platform network communication through these encapsulated functions.

## API Description

### tkl_net_get_errno

```c
TUYA_ERRNO tkl_net_get_errno(void);
```

#### Function

Retrieves the network error code.

#### Parameters

No parameters.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_fd_set

```c
OPERATE_RET tkl_net_fd_set(const int fd, TUYA_FD_SET_T* fds);
```

#### Function

Adds a file descriptor to the set.

#### Parameters

- `fd`: The file descriptor to be added.
- `fds`: Pointer to the file descriptor set.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_fd_clear

```c
OPERATE_RET tkl_net_fd_clear(const int fd, TUYA_FD_SET_T* fds);
```

#### Function

Removes a file descriptor from the set.

#### Parameters

- `fd`: The file descriptor to be cleared.
- `fds`: Pointer to the file descriptor set.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_fd_isset

```c
OPERATE_RET tkl_net_fd_isset(const int fd, TUYA_FD_SET_T* fds);
```

#### Function

Checks if a file descriptor is in the set.

#### Parameters

- `fd`: The file descriptor to be checked.
- `fds`: Pointer to the file descriptor set.

#### Return Value

Returns `TRUE` if the file descriptor is in the set, otherwise returns `FALSE`.

### tkl_net_fd_zero

```c
OPERATE_RET tkl_net_fd_zero(TUYA_FD_SET_T* fds);
```

#### Function

Clears all file descriptors in the file descriptor set.

#### Parameters

- `fds`: Pointer to the file descriptor set.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_select

```c
int tkl_net_select(const int maxfd, TUYA_FD_SET_T *readfds, TUYA_FD_SET_T *writefds, TUYA_FD_SET_T *errorfds, const uint32_t ms_timeout);
```

#### Function

Retrieves the set of available file descriptors.

#### Parameters

- `maxfd`: One more than the maximum number of file descriptors to be checked.
- `readfds`: Pointer to the set of readable file descriptors.
- `writefds`: Pointer to the set of writable file descriptors.
- `errorfds`: Pointer to the set of erroneous file descriptors.
- `ms_timeout`: Timeout period in milliseconds.

#### Return Value

Returns the number of available file descriptors; returns a value less than or equal to 0 on error.

### tkl_net_get_nonblock

```c
int tkl_net_get_nonblock(const int fd);
```

#### Function

Retrieves a non-blocking file descriptor.

#### Parameters

- `fd`: The file descriptor.

#### Return Value

Returns the number of non-blocking file descriptors on success; returns a value less than or equal to 0 on error.

### tkl_net_set_block

```c
OPERATE_RET tkl_net_set_block(const int fd, const BOOL_T block);
```

#### Function

Sets the blocking flag for a file descriptor.

#### Parameters

- `fd`: The file descriptor.
- `block`: If `TRUE`, sets to blocking mode; if `FALSE`, sets to non-blocking mode.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_close

```c
TUYA_ERRNO tkl_net_close(const int fd);
```

#### Function

Closes a file descriptor.

#### Parameters

- `fd`: The file descriptor.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_shutdown

```c
TUYA_ERRNO tkl_net_shutdown(const int fd, const int how);
```

#### Function

Shuts down read/write operations on a file descriptor.

#### Parameters

- `fd`: The file descriptor.
- `how`: The type of shutdown.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_socket_create

```c
int tkl_net_socket_create(const TUYA_PROTOCOL_TYPE_E type);
```

#### Function

Creates a TCP or UDP socket.

#### Parameters

- `type`: The protocol type, either TCP or UDP.

#### Return Value

Returns the file descriptor.

### tkl_net_connect

```c
TUYA_ERRNO tkl_net_connect(const int fd, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### Function

Connects to a network.

#### Parameters

- `fd`: The file descriptor.
- `addr`: Server address information.
- `port`: Server port information.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_connect_raw

```c
TUYA_ERRNO tkl_net_connect_raw(const int fd, void *p_socket_addr, const int len);
```

#### Function

Connects to a network using raw data.

#### Parameters

- `fd`: The file descriptor.
- `p_socket_addr`: Raw socket data.
- `len`: Length of the data.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_bind

```c
TUYA_ERRNO tkl_net_bind(const int fd, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### Function

Binds a socket to a network.

#### Parameters

- `fd`: The file descriptor.
- `addr`: Server address information.
- `port`: Server port information.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_listen

```c
TUYA_ERRNO tkl_net_listen(const int fd, const int backlog);
```

#### Function

Listens for network connection requests.

#### Parameters

- `fd`: File descriptor.
- `backlog`: The maximum number of pending connections the queue can hold.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_accept

```c
TUYA_ERRNO tkl_net_accept(const int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### Function

Accepts a network connection request.

#### Parameters

- `fd`: File descriptor.
- `addr`: The IP address of the receiver.
- `port`: The port number of the receiver.

#### Return Value

Returns 0 on success, otherwise returns an error code specific to the target system.

### tkl_net_send

```c
TUYA_ERRNO tkl_net_send(const int fd, const void *buf, const uint32_t nbytes);
```

#### Function

Sends data over the network.

#### Parameters

- `fd`: File descriptor.
- `buf`: The buffer containing the data to be sent.
- `nbytes`: The length of the buffer.

#### Return Value

The number of bytes sent; returns a negative value on error.

### tkl_net_send_to

```c
TUYA_ERRNO tkl_net_send_to(const int fd, const void *buf, const uint32_t nbytes, const TUYA_IP_ADDR_T addr, const uint16_t port);
```

#### Function

Sends data to a specified server.

#### Parameters

- `fd`: File descriptor.
- `buf`: The buffer containing the data to be sent.
- `nbytes`: The length of the buffer.
- `addr`: Server address information.
- `port`: Server port information.

#### Return Value

The number of bytes sent; returns a negative value on error.

### tkl_net_recv

```c
TUYA_ERRNO tkl_net_recv(const int fd, void *buf, const uint32_t nbytes);
```

#### Function

Receives data from the network.

#### Parameters

- `fd`: File descriptor.
- `buf`: The buffer to store the received data.
- `nbytes`: The length of the buffer.

#### Return Value

The number of bytes received; returns a negative value on error.

### tkl_net_recv_nd_size

```c
int tkl_net_recv_nd_size(const int fd, void *buf, const uint32_t buf_size, const uint32_t nd_size);
```

#### Function

Receives data of a specified size from the network.

#### Parameters

- `fd`: File descriptor.
- `buf`: The buffer to store the received data.
- `buf_size`: The size of the buffer.
- `nd_size`: The expected size of the data to be received.

#### Return Value

The actual amount of data received on success, or a negative value on error.

### tkl_net_recvfrom

```c
TUYA_ERRNO tkl_net_recvfrom(const int fd, void *buf, const uint32_t nbytes, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### Function

Receives data from a specified server.

#### Parameters

- `fd`: File descriptor.
- `buf`: The buffer to store the received data.
- `nbytes`: The length of the buffer.
- `addr`[OUT]: Server address information.
- `port`[OUT]: Server port information.

#### Return Value

The number of bytes received; returns a negative value on error.

### tkl_net_gethostbyname

```c
OPERATE_RET tkl_net_gethostbyname(const char *domain, TUYA_IP_ADDR_T *addr);
```

#### Function

Obtains address information by domain name.

#### Parameters

- `domain`: Domain name information.
- `addr`: Pointer to store the address information.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_socket_bind

```c
OPERATE_RET tkl_net_socket_bind(const int fd, const char *ip);
```

#### Function

Binds the socket to a network with the specified IP.

#### Parameters

- `fd`: File descriptor.
- `ip`: IP address.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_cloexec

```c
OPERATE_RET tkl_net_set_cloexec(const int fd);
```

#### Function

Sets the socket to remain open in child processes after a `fork` call.

#### Parameters

- `fd`: File descriptor.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_get_socket_ip

```c
OPERATE_RET tkl_net_get_socket_ip(const int fd, TUYA_IP_ADDR_T *addr);
```

#### Function

Obtains the IP address via the socket descriptor.

#### Parameters

- `fd`: File descriptor.
- `addr`: Pointer to store the IP address.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_str2addr

```c
TUYA_IP_ADDR_T tkl_net_str2addr(const char *ip_str);
```

#### Function

Converts an IP string to an address.

#### Parameters

- `ip_str`: IP string.

#### Return Value

Returns the converted IP address.

### tkl_net_setsockopt

```c
OPERATE_RET tkl_net_setsockopt(const int fd, const TUYA_OPT_LEVEL level, const TUYA_OPT_NAME optname, const void *optval, const int optlen);
```

#### Function

Sets socket options.

#### Parameters

- `fd`: File descriptor.
- `level`: The level at which the option is defined.
- `optname`: The option name.
- `optval`: A pointer to the option value.
- `optlen`: The length of the option value.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_getsockopt

```c
OPERATE_RET tkl_net_getsockopt(const int fd, const TUYA_OPT_LEVEL level, const TUYA_OPT_NAME optname, void *optval, int *optlen);
```

#### Function

Gets the socket option value.

#### Parameters

- `fd`: File descriptor.
- `level`: The level at which the option is defined.
- `optname`: The option name.
- `optval`: A pointer to where the option value will be stored.
- `optlen`: A pointer to where the length of the option value will be stored.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_timeout

```c
OPERATE_RET tkl_net_set_timeout(const int fd, const int ms_timeout, const TUYA_TRANS_TYPE_E type);
```

#### Function

Sets the socket timeout option.

#### Parameters

- `fd`: File descriptor.
- `ms_timeout`: Timeout period in milliseconds.
- `type`: Type of transmission, either receiving or sending.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_bufsize

```c
OPERATE_RET tkl_net_set_bufsize(const int fd, const int buf_size, const TUYA_TRANS_TYPE_E type);
```

#### Function

Sets the socket buffer size option.

#### Parameters

- `fd`: File descriptor.
- `buf_size`: Buffer size in bytes.
- `type`: Type of transmission, either receiving or sending.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_reuse

```c
OPERATE_RET tkl_net_set_reuse(const int fd);
```

#### Function

Enables the socket reuse option.

#### Parameters

- `fd`: File descriptor.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_disable_nagle

```c
OPERATE_RET tkl_net_disable_nagle(const int fd);
```

#### Function

Disables the Nagle algorithm for the socket.

#### Parameters

- `fd`: File descriptor.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_broadcast

```c
OPERATE_RET tkl_net_set_broadcast(const int fd);
```

#### Function

Enables the socket broadcast option.

#### Parameters

- `fd`: File descriptor.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_set_keepalive

```c
OPERATE_RET tkl_net_set_keepalive(int fd, const BOOL_T alive, const uint32_t idle, const uint32_t intr, const uint32_t cnt);
```

#### Function

Sets the `keepalive` option for the socket to monitor the connection.

#### Parameters

- `fd`: File descriptor.
- `alive`: `keepalive` option, enable or disable.
- `idle`: `keep idle` option, idle time (seconds); if there is no data exchange in this time, the probe starts.
- `intr`: `keep interval` option, probe interval.
- `cnt`: `keep count` option, number of probes.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_getsockname

```c
OPERATE_RET tkl_net_getsockname(int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### Function

Gets information about a specific socket.

#### Parameters

- `fd`: File descriptor.
- `addr`: Source IP address.
- `port`: Source port.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_getpeername

```c
OPERATE_RET tkl_net_getpeername(int fd, TUYA_IP_ADDR_T *addr, uint16_t *port);
```

#### Function

Gets the destination information of a specific socket.

#### Parameters

- `fd`: File descriptor.
- `addr`: Destination IP address.
- `port`: Destination port.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.

### tkl_net_sethostname

```c
OPERATE_RET tkl_net_sethostname(const char *hostname);
```

#### Function

Sets the system hostname, which will be displayed on the router.

#### Parameters

- `hostname`: Hostname.

#### Return Value

Returns `OPRT_OK` on success, otherwise refer to error codes in `tuya_error_code.h`.
