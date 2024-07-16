/**
 * @file tdd_sw_i2c.h
 * @brief Software I2C interface for  devices.
 *
 * This header file defines the software I2C (Inter-Integrated Circuit) interface for devices, providing a
 * software-based solution for I2C communication. It includes definitions for initializing and deinitializing the I2C
 * interface, configuring I2C pins, and transferring data over the I2C bus using software bit-banging. The interface
 * supports both read and write operations, handling of 10-bit addressing, and various flags for customizing the I2C
 * communication behavior.
 *
 * The software I2C implementation is particularly useful for devices that do not have hardware I2C support or require
 * additional I2C interfaces beyond what is available in hardware. I t allows for flexible I2C communication using any
 * general-purpose input/output (GPIO) pins.
 *
 * @note This implementation is designed to be portable and can be adapted to various Tuya IoT devices and platforms.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TDD_SENSOR_I2C_H__
#define __TDD_SENSOR_I2C_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/

/**
 * @brief i2c flag
 */
#define SW_I2C_FLAG_WR          (1u << 0) // write
#define SW_I2C_FLAG_RD          (1u << 1) // read
#define SW_I2C_FLAG_ADDR_10BIT  (1u << 2) // 10bits address
#define SW_I2C_FLAG_NO_START    (1u << 3) // no start
#define SW_I2C_FLAG_IGNORE_NACK (1u << 4) // ignore NACK
#define SW_I2C_FLAG_NO_READ_ACK (1u << 5) // read without ACK
#define SW_I2C_FLAG_NO_ADDR     (1u << 6) // massage wuthout address

/***********************************************************
***********************typedef define***********************
***********************************************************/
typedef uint8_t SW_I2C_PORT_NUM_E;
#define SW_I2C_PORT_NUM_0   0x00
#define SW_I2C_PORT_NUM_1   0x01
#define SW_I2C_PORT_NUM_2   0x02
#define SW_I2C_PORT_NUM_3   0x03
#define SW_I2C_PORT_NUM_MAX (SW_I2C_PORT_NUM_3 + 1)

/**
 * @brief i2c pin
 */
typedef struct {
    TUYA_GPIO_NUM_E scl;
    TUYA_GPIO_NUM_E sda;
} SW_I2C_GPIO_T;

/**
 * @brief i2c message struct
 *
 */
typedef struct {
    uint32_t flags;
    uint16_t addr;
    uint16_t len;
    uint8_t *buff;
} SW_I2C_MSG_T;

/***********************************************************
********************function declaration********************
***********************************************************/
/**
 * @brief i2c init
 *
 * @param[in] port: i2c port
 * @param[in] i2c_pin: i2c pin number
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_init(uint8_t port, SW_I2C_GPIO_T i2c_pin);

/**
 * @brief i2c deinit
 *
 * @param[in] i2c_pin: i2c pin number
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_deinit(uint8_t port);

/**
 * @brief i2c transfer
 *
 * @param[in] i2c_pin: i2c pin number
 * @param[in] msg: msg send to i2c bus
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_xfer(uint8_t port, SW_I2C_MSG_T *msg);

#ifdef __cplusplus
}
#endif

#endif /* __TDD_SENSOR_I2C_H__ */
