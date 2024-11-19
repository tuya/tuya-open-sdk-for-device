# RTC Driver

## Brief Description

​	A Real-Time Clock (RTC) is a timer dedicated to maintaining a one-second time base. Additionally, RTCs are typically used to track clock time and calendar dates in software or hardware. Many functions of RTCs are highly specialized and necessary for maintaining high precision and very reliable operations.

​	General SOCs have an internal RTC hardware unit that can be directly manipulated to set and read RTC time. Some SOCs can also expand RTC peripherals via IIC or SPI interfaces.

## API Description

### 1. tkl_rtc_init

```c
OPERATE_RET tkl_rtc_init(void);
```

- Function Description:
  - Initializes the RTC and returns the initialization result.
  
- Parameters:
  - void 
  
- Return Value:
  - OPRT_OK for success, others please refer to the definitions in the file `tuya_error_code.h`.

### 2. tkl_rtc_deinit

```c
OPERATE_RET tkl_rtc_deinit(void);
```

- Function Description:
  - Deinitializes the RTC, stops the RTC.
- Parameters:
  - void
- Return Value:
  - OPRT_OK for success, others please refer to the definitions in the file `tuya_error_code.h`.

### 3. tkl_rtc_time_set

```c
OPERATE_RET tkl_rtc_time_set(TIME_T time_sec);
```

- Function Description:
  - Sets the time of the RTC.

- Parameters:
  - time_sec: A UTC time.

  
```c
typedef unsigned int TIME_T;
```
  
- Return Value:
  - OPRT_OK for success, others please refer to the definitions in the file `tuya_error_code.h`.

### 4. tkl_rtc_time_get

```c
OPERATE_RET tkl_rtc_time_get(TIME_T *time_sec);
```

- Function Description:
  - Gets the time of the RTC.
- Parameters:
  - time_sec: UTC time
  
    ```c
    typedef unsigned int TIME_T;
    ```
- Return Value:
  - OPRT_OK for success, others please refer to the definitions in the file `tuya_error_code.h`.

# Example

## RTC Example

```c
/* Initialize RTC */
tkl_rtc_init(void);

/* Set RTC time */
TIME_T time_sec_set = 0x1000000;
tkl_rtc_time_set(&time_sec_set);

/* Get RTC time */
TIME_T time_sec_get;
tkl_rtc_time_get(&time_sec_get);

/* Deinitialize RTC */
tkl_rtc_deinit(void);

```
