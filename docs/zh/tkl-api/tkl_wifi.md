# Wi-Fi

`tkl_wifi.c` 文件提供了一系列 Wi-Fi 相关的 API，以适配不同的网络接口。这些 API 包括设置/读取模式、信道、MAC 地址、连接等相关操作操作。文件同样是由涂鸦操作系统（TuyaOS）自动生成，并为开发者预留了代码实现的区域。

## 索引

  * [ tkl_wifi_init](#tkl_wifi_init)
  * [ tkl_wifi_scan_ap](#tkl_wifi_scan_ap)
  * [ tkl_wifi_release_ap](#tkl_wifi_release_ap)
  * [ tkl_wifi_start_ap](#tkl_wifi_start_ap)
  * [ tkl_wifi_stop_ap](#tkl_wifi_stop_ap)
  * [ tkl_wifi_set_cur_channel](#tkl_wifi_set_cur_channel)
  * [ tkl_wifi_get_cur_channel](#tkl_wifi_get_cur_channel)
  * [ tkl_wifi_set_sniffer](#tkl_wifi_set_sniffer)
  * [ tkl_wifi_get_ip](#tkl_wifi_get_ip)
  * [ tkl_wifi_set_ip](#tkl_wifi_set_ip)
  * [ tkl_wifi_set_mac](#tkl_wifi_set_mac)
  * [ tkl_wifi_get_mac](#tkl_wifi_get_mac)
  * [ tkl_wifi_set_work_mode](#tkl_wifi_set_work_mode)
  * [ tkl_wifi_get_work_mode](#tkl_wifi_get_work_mode)
  * [ tkl_wifi_get_connected_ap_info](#tkl_wifi_get_connected_ap_info)
  * [ tkl_wifi_get_bssid](#tkl_wifi_get_bssid)
  * [ tkl_wifi_set_country_code](#tkl_wifi_set_country_code)
  * [ tkl_wifi_set_rf_calibrated](#tkl_wifi_set_rf_calibrated)
  * [ tkl_wifi_set_lp_mode](#tkl_wifi_set_lp_mode)
  * [ tkl_wifi_station_fast_connect](#tkl_wifi_station_fast_connect)
  * [ tkl_wifi_station_connect](#tkl_wifi_station_connect)
  * [ tkl_wifi_station_disconnect](#tkl_wifi_station_disconnect)
  * [ tkl_wifi_station_get_conn_ap_rssi](#tkl_wifi_station_get_conn_ap_rssi)
  * [ tkl_wifi_station_get_status](#tkl_wifi_station_get_status)
  * [ tkl_wifi_send_mgnt](#tkl_wifi_send_mgnt)
  * [ tkl_wifi_register_recv_mgnt_callback](#tkl_wifi_register_recv_mgnt_callback)

------
## API 说明

### tkl_wifi_init

```c
OPERATE_RET tkl_wifi_init(WIFI_EVENT_CB cb)
```

- 功能描述:

  设置 Wi-Fi 工作状态
  
- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | cb | Wi-Fi 工作状态回调 |

- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
### tkl_wifi_scan_ap

```c
OPERATE_RET tkl_wifi_scan_ap(const int8_t *ssid, AP_IF_S **ap_ary, uint32_t *num)
```

- 功能描述:

  扫描当前环境，获取当前环境中的 ap 信息


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | ssid |如果 ssid = NULL 则扫描所有 ap, 其他则扫描指定 ap |
  | [out] | ap_ary | 扫描结果信息 |
  | [out] | num | 扫描 ap 个数 |

- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**

  1. 阻塞函数
  2. 只能扫描到当前国家码信道列表里的 ap
  3. 当环境存在指定 ssid 的 ap，扫描到该 ssid 的成功率要超过 98%
  4. ap 需要在该 api 内分配内存（扫描到多个 ap，需要分配连续的内存空间）
  5. 释放该内存由 tuya sdk 主动调用 `tkl_wifi_release_ap` 释放

  
### tkl_wifi_release_ap

```c
OPERATE_RET tkl_wifi_release_ap(AP_IF_S *ap)
```

- 功能描述:

  释放 ap 信息资源


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | ap | 需要释放的 ap 信息 |

- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
### tkl_wifi_start_ap

```c
OPERATE_RET tkl_wifi_start_ap(const WF_AP_CFG_IF_S *cfg)
```

- 功能描述:

  启动 ap 模式


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | cfg | ap 配置参数 |

- 返回值:
返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
- **备注**

  1. 需要根据 max_conn 来设置 ap 模式下最多允许几个 sta 能连接成功。
  2. 需要根据 ip 信息来设置 ap 模式下的 ip 信息。
  3. stationap 模式，如果 ap por t有连接的情况下 staiton port 连接路由器，连接过程以及连接
     失败后 ap port 不可以有断连情况以及可以收发广播包。
  4. 配网 ez+ap 共存：
    4.1. softap 或者 stationap 模式下可以使能 sniffer 功能以及可以切换信道。
    4.2. 使能 sniffer 功能之后，扔给回调的数据要包括 ap port 的设备连接请求包。
  
   
### tkl_wifi_stop_ap

```c
OPERATE_RET tkl_wifi_stop_ap(void)
```

- 功能描述:

  停止 ap 模式


- 参数：

  无

- 返回值:
返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

- **备注**

  1. 需要针对设备当前所处模式 softap 或 stationap 做不同的关闭逻辑
  2. stationap 模式下，如果 station port 端有连接，关闭 ap port 过程 station port 不能有断连情况
  
  
### tkl_wifi_set_cur_channel

```c
OPERATE_RET tkl_wifi_set_cur_channel(const uint8_t chan)
```

- 功能描述:

  设置信道


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | chan | 信道 |

- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

- **备注**

  1. 设置当前国家码信道范围之外的信道报失败
  2. 支持在 sniffer 回调里设置信道
  
### tkl_wifi_get_cur_channel

```c
OPERATE_RET tkl_wifi_get_cur_channel(uint8_t *chan);
```

- 功能描述:

  获取当前信道


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | chan | 信道 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
### tkl_wifi_set_sniffer

```c
OPERATE_RET tkl_wifi_set_sniffer(const BOOL_T en, const SNIFFER_CALLBACK cb)
```

- 功能描述:

  设置 sniffer 功能开关


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | en | 开或关sniffer功能 |
  | [in] | cb | 抓包数据回调 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**

  返给应用的数据要括管理包数据
  
### tkl_wifi_get_ip

```c
OPERATE_RET tkl_wifi_get_ip(const WF_IF_E wf, NW_IP_S *ip)
```

- 功能描述:

  获取 Wi-Fi 的 ip 信息（ip 地址、网关地址、地址掩码）


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | wf | Wi-Fi 模式 |
  | [out] | ip | ip 信息 |
  |--------|--------|
  | WF_STATION | station type|
  | WF_AP | ap type |
  
- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**
  需要区分不同模式（依据传参 wf）下的 ip 信息
  
  
### tkl_wifi_set_ip

```c
OPERATE_RET tkl_wifi_set_ip(const WF_IF_E wf, NW_IP_S *ip)
```

- 功能描述:

  设置 Wi-Fi 的静态 ip 信息（ip地址、网关地址、地址掩码）


- 参数：

  | 输入/输出  | 参数名       | 描述     |
  | ---------- | ------------ | -------- |
  | [in]       | wf           | wifi模式 |
  | [in]       | ip           | ip 信息   |
  | --------   | --------     |          |
  | WF_STATION | station type |          |
  | WF_AP      | ap type      |          |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**
  需要区分不同模式（依据传参 wf）下的 ip 信息



### tkl_wifi_set_mac

```c
OPERATE_RET tkl_wifi_set_mac(const WF_IF_E wf, const NW_MAC_S *mac)
```

- 功能描述:

  设置 mac 地址


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | wf | wifi模式 |
  | [in] | mac | mac地址 |
  |--------|--------|
  | WF_STATION | station type|
  | WF_AP | ap type |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**

  永久生效，断电不丢失
  
  
### tkl_wifi_get_mac

```c
OPERATE_RET tkl_wifi_get_mac(const WF_IF_E wf, NW_MAC_S *mac);
```

- 功能描述:

  获取 mac 地址


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | wf | wifi模式 |
  | [out] | mac | mac地址 |
  |--------|--------|
  | WF_STATION | station type|
  | WF_AP | ap type |

- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**

  stationap 模式下的 mac 地址需要区分工作模式（依据传参 wf）

  
### tkl_wifi_set_work_mode

```c
OPERATE_RET tkl_wifi_set_work_mode(const WF_WK_MD_E mode)
```

- 功能描述:

  设置 Wi-Fi 工作模式


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | mode | wifi工作模式 |
  |--------|--------|
  | WWM_LOWPOWER | 低功耗（指关闭wifi模块）|
  | WWM_SNIFFER | monitor |
  | WWM_STATION | station |
  | WWM_SOFTAP | softap |
  | WWM_STATIONAP | stationap |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。


- **备注**

  1. 从设备上电到调用 tuya entranc e要求在 200ms 以内。
  2. 如果设备初始化时间超过 200ms，底层可以做一些必要的初始化之后就调用 tuya entrance，
     将耗时间的 Wi-Fi 初始化放在另一个线程执行。
  3. tkl_wifi_set_work_mode 是 tuya 业务使用 Wi-Fi 调用的第一个 api，在这个 api 需要判断 Wi-Fi 初始化是否结束，
     如果没结束，要在这里等到结束才开始往下执行（只需要判断一次）。
  4. 如果设备从上电到 tuya entrance 少于 200ms，无需考虑这点。

### tkl_wifi_get_work_mode

```c
OPERATE_RET tkl_wifi_get_work_mode(WF_WK_MD_E *mode)
```

- 功能描述:

  获取wifi工作模式


- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | mode | Wi-Fi 工作模式 |
  |--------|--------|
  | WWM_LOWPOWER | 低功耗（指关闭wifi模块）|
  | WWM_SNIFFER | monitor |
  | WWM_STATION | station |
  | WWM_SOFTAP | softap |
  | WWM_STATIONAP | stationap |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
### tkl_wifi_get_connected_ap_info

```c
OPERATE_RET tkl_wifi_get_connected_ap_info(FAST_WF_CONNECTED_AP_INFO_T **fast_ap_info)
```

- 功能描述:

  获取所连 ap 信息，用于快连功能

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | fast_ap_info | 所连ap信息 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
- **备注**
  
  1. fast_ap_info 需要在该 api 内动态申请内存
  2. 每次重连路由器成功都会获取一次路由器信息，如有更新会同步到 flash
  
  
  
### tkl_wifi_get_bssid

```c
OPERATE_RET tkl_wifi_get_bssid(uint8_t *mac)
```

- 功能描述:

  获取所连 ap 的 mac 地址

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | mac | mac 地址 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
  
### tkl_wifi_set_country_code

```c
OPERATE_RET tkl_wifi_set_country_code(const COUNTRY_CODE_E ccode)
```

- 功能描述:

  设置国家码

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | ccode | 国家码 |
  |--------|--------|
  | COUNTRY_CODE_CN | 中国区 1-13 |
  | COUNTRY_CODE_US | 美国区 1-11 |
  | COUNTRY_CODE_JP | 日本区 1-14 |
  | COUNTRY_CODE_EU | 欧洲区 1-13 需要考虑欧洲自适应 |
  
- 返回值:

返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
- **备注**
  1. 国家码暂时只需要支持中国区（CN 1-13）、美国区（US 1-11）、日本区（JP 1-14）、欧洲区（EU 1-13）
  2. 针对不同的国家码，需要达到的要求如下：
    2.1 扫描路由器时只能扫到国家码对应信道列表的ap。
    2.2 欧洲区需要实现欧洲自适应功能
    2.3 设置信道时，设置的信道不在当前国家码的信道列表范围之内，设置信道返回error


### tkl_wifi_set_rf_calibrated

```c
BOOL_T tkl_wifi_set_rf_calibrated(void);
```

- 功能描述:

  检查 RF 是否校准过

- 参数：

  无

- 返回值:

  - TRUE : 已校准
  - FALSE ：未校准
  
    
### tkl_wifi_set_lp_mode

```c
OPERATE_RET tkl_wifi_set_lp_mode(const BOOL_T enable, const uint8_t dtim)
```

- 功能描述:

  wifi低功耗设置

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | enable |  是否开启wifi低功耗模式|
  | [in] | dtim | dtim参数 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
### tkl_wifi_station_fast_connect

```c
OPERATE_RET tkl_wifi_station_fast_connect(const FAST_WF_CONNECTED_AP_INFO_T *fast_ap_info)
```

- 功能描述:

  快速连接路由器

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | fast_ap_info | 快连所需的ap信息 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
- **备注**

  配过网且重启之后第一次连接才会调用这个函数
  
  
### tkl_wifi_station_connect

```c
OPERATE_RET tkl_wifi_station_connect(const int8_t *ssid, const int8_t *passwd)
```

- 功能描述:

  连接路由器

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | ssid | ssid |
  | [in] | passwd | passwd |

- 返回值:
  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
- **备注**

  1. 非阻塞，启动连接流程成功后，上层会每隔 1s 钟调用 tkl_wifi_station_get_status 查询 Wi-Fi 连接状态
  2. 需要使能自动重连功能，重连时间在 1min 以内即可，上层断连情况下会每隔 1min 发起一次重连


### tkl_wifi_station_disconnect

```c
OPERATE_RET tkl_wifi_station_disconnect(void)
```

- 功能描述:
  断开路由器

- 参数：
  无

- 返回值:
  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
  
  
### tkl_wifi_station_get_conn_ap_rssi

```c
OPERATE_RET tkl_wifi_station_get_conn_ap_rssi(int8_t *rssi)
```

- 功能描述:

  获取所连 ap 的信号强度

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | rssi | 信号强度 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
  
- **备注**
  1. 不移动设备和路由器的位置，多次获取 rssi 只能出现小范围的波动
  2. 移动设备和路由器的位置，获取 rssi 会有对应的变化
  

### tkl_wifi_station_get_status

```c
OPERATE_RET tkl_wifi_station_get_status(WF_STATION_STAT_E *stat)
```

- 功能描述:

  获取wifi当前的连接状态

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [out] | stat | 连接状态 |
  |--------|--------|
  | WSS_IDLE |  |
  | WSS_CONNECTING | 连接中 |
  | WSS_PASSWD_WRONG | 密码错误 |
  | WSS_CONN_FAIL | 连接失败 |
  | WSS_CONN_SUCCESS | 连接成功 |
  | WSS_GOT_IP | dhcp成功 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
### tkl_wifi_send_mgnt

```c
OPERATE_RET tkl_wifi_send_mgnt(const uint8_t *buf, const uint32_t len)
```

- 功能描述:

  发送管理包数据

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | buf | 管理包数据 buf |
  | [in] | len | 管理包数据长度 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。

  
### tkl_wifi_register_recv_mgnt_callback

```c
OPERATE_RET tkl_wifi_register_recv_mgnt_callback(const BOOL_T enable, const WIFI_REV_MGNT_CB recv_cb)
```

- 功能描述:

  设置应用层是否接收管理包数据

- 参数：

  | 输入/输出 |  参数名  |  描述  |
  |--------|--------|--------|
  | [in] | enable |  是否开启接收管理包数据 |
  | [in] | recv_cb | 接收管理包数据回调 |

- 返回值:

  返回值为 `OPRT_OK` 表示操作成功，其他返回值表示出现错误，请参考 `tuya_error_code.h` 获取错误信息。
