/**
 * @file tdd_sw_i2c.c
 * @brief Software I2C implementation for SDK.
 *
 * This file provides the implementation of a software-based I2C (Inter-Integrated Circuit) protocol
 * specifically designed for Tuya IoT devices. It includes functions for initializing I2C pins,
 * starting and stopping I2C communication, sending and receiving bytes, and reading and writing data
 * over the I2C bus using software bit-banging technique.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_iot_config.h"

#if defined(ENABLE_GPIO) && (ENABLE_GPIO)
#include "tkl_gpio.h"
#include "tal_log.h"
#include "tkl_output.h"

#include "tdd_sw_i2c.h"
/***********************************************************
************************macro define************************
***********************************************************/
#define I2C_WRITE 0
#define I2C_READ  1

/* You can change the I2C frequency by changing this value */
#define DELAY_US 5

/*
 * Adaptation is needed based on the chip platform.
 */
#define TICK_US 2

#define I2C_SCL_INIT() __sw_i2c_scl_init(i2c_pin.scl)
#define I2C_SCL_H()    __sw_i2c_write(i2c_pin.scl, TUYA_GPIO_LEVEL_HIGH)
#define I2C_SCL_L()    __sw_i2c_write(i2c_pin.scl, TUYA_GPIO_LEVEL_LOW)

#define I2C_SDA_INIT_OUT() __sw_i2c_sda_init(i2c_pin.sda, FALSE)
#define I2C_SDA_H()        __sw_i2c_write(i2c_pin.sda, TUYA_GPIO_LEVEL_HIGH)
#define I2C_SDA_L()        __sw_i2c_write(i2c_pin.sda, TUYA_GPIO_LEVEL_LOW)

#define I2C_SDA_INIT_IN() __sw_i2c_sda_init(i2c_pin.sda, TRUE)
#define I2C_SDA_READ()    __sw_i2c_sda_read(i2c_pin.sda)

#define I2C_DELAY(us)                                                                                                  \
    do {                                                                                                               \
        volatile uint32_t i = us * TICK_US;                                                                            \
        while (i--)                                                                                                    \
            ;                                                                                                          \
    } while (0)

/***********************************************************
***********************typedef define***********************
***********************************************************/

/***********************************************************
********************function declaration********************
***********************************************************/

/***********************************************************
***********************variable define**********************
***********************************************************/

static SW_I2C_GPIO_T sg_i2c_pin[SW_I2C_PORT_NUM_MAX] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};

/***********************************************************
***********************function define**********************
***********************************************************/
/**
 * @brief SCL pin init
 *
 * @param[in] : pin number
 *
 * @return none
 */
static void __sw_i2c_scl_init(TUYA_GPIO_NUM_E pin)
{
    TUYA_GPIO_BASE_CFG_T pin_cfg = {
        .mode = TUYA_GPIO_PUSH_PULL, .direct = TUYA_GPIO_OUTPUT, .level = TUYA_GPIO_LEVEL_LOW};

    tkl_gpio_init(pin, &pin_cfg);

    return;
}

/**
 * @brief SDA pin init
 *
 * @param[in] pin: pin number
 * @param[in] in: TRUE on in, FALSE on out
 *
 * @return none
 */
static void __sw_i2c_sda_init(TUYA_GPIO_NUM_E pin, BOOL_T in)
{
    TUYA_GPIO_BASE_CFG_T pin_cfg;
    if (in) {
        pin_cfg.mode = TUYA_GPIO_PULLUP;
        pin_cfg.direct = TUYA_GPIO_INPUT;
        pin_cfg.level = TUYA_GPIO_LEVEL_HIGH;
    } else {
        pin_cfg.mode = TUYA_GPIO_PUSH_PULL;
        pin_cfg.direct = TUYA_GPIO_OUTPUT;
        pin_cfg.level = TUYA_GPIO_LEVEL_LOW;
    }
    tkl_gpio_init(pin, &pin_cfg);

    return;
}

/**
 * @brief read SDA pin
 *
 * @param[in] pin: pin number
 *
 * @return pin level
 */
static TUYA_GPIO_LEVEL_E __sw_i2c_sda_read(TUYA_GPIO_NUM_E pin)
{
    TUYA_GPIO_LEVEL_E level;

    tkl_gpio_read(pin, &level);

    return level;
}

/**
 * @brief write i2c pin
 *
 * @param[in] pin: pin number
 * @param[in] level: pin level
 *
 * @return none
 */
static void __sw_i2c_write(TUYA_GPIO_NUM_E pin, TUYA_GPIO_LEVEL_E level)
{
    tkl_gpio_write(pin, level);

    return;
}

/**
 * @brief i2c start
 *
 * @param[in] i2c_pin: pin number
 *
 * @return none
 */
static void __sw_i2c_start(SW_I2C_GPIO_T i2c_pin)
{
    I2C_SCL_H();
    I2C_SDA_H();
    I2C_DELAY(DELAY_US);

    I2C_SDA_L();
    I2C_DELAY(DELAY_US);

    I2C_SCL_L();

    return;
}

/**
 * @brief i2c stop
 *
 * @param[in] i2c_pin: pin number
 *
 * @return none
 */
static void __sw_i2c_stop(SW_I2C_GPIO_T i2c_pin)
{
    I2C_SCL_L();
    I2C_SDA_INIT_OUT();

    I2C_SDA_L();

    I2C_SCL_H();
    I2C_DELAY(DELAY_US);

    I2C_SDA_H();
    I2C_DELAY(DELAY_US);

    return;
}

/**
 * @brief i2c ack
 *
 * @param[in] i2c_pin: pin number
 *
 * @return none
 */
static void __sw_i2c_ack(SW_I2C_GPIO_T i2c_pin)
{
    I2C_SCL_L();
    I2C_SDA_INIT_OUT();

    I2C_SDA_L();
    I2C_DELAY(DELAY_US);
    I2C_SCL_H();
    I2C_DELAY(DELAY_US);
    I2C_SCL_L();

    return;
}

/**
 * @brief i2c no ack
 *
 * @param[in] i2c_pin: pin number
 *
 * @return none
 */
static void __sw_i2c_no_ack(SW_I2C_GPIO_T i2c_pin)
{
    I2C_SCL_L();
    I2C_SDA_INIT_OUT();

    I2C_SDA_H();
    I2C_DELAY(DELAY_US);
    I2C_SCL_H();
    I2C_DELAY(DELAY_US);
    I2C_SCL_L();

    return;
}

/**
 * @brief i2c get ack
 *
 * @param[in] i2c_pin: pin number
 *
 * @return none
 */
static BOOL_T __sw_i2c_get_ack(SW_I2C_GPIO_T i2c_pin, uint32_t flags)
{
    uint32_t timeout_count = 0;
    I2C_SCL_L();
    I2C_SDA_INIT_IN();
    I2C_DELAY(1);

    if (0 == (flags & SW_I2C_FLAG_IGNORE_NACK)) {
        while (I2C_SDA_READ()) {
            if (timeout_count >= DELAY_US) {
                __sw_i2c_stop(i2c_pin);
                PR_ERR("wait ack timeout");
                return FALSE;
            }
            I2C_DELAY(1);
            timeout_count++;
        }
    }

    I2C_SCL_H();
    I2C_DELAY(DELAY_US);
    I2C_SCL_L();

    return TRUE;
}

/**
 * @brief i2c send byte
 *
 * @param[in] i2c_pin: pin number
 * @param[in] data: data to send
 *
 * @return none
 */
static void __sw_i2c_send_byte(SW_I2C_GPIO_T i2c_pin, uint8_t data)
{
    uint8_t i = 0;

    I2C_SCL_L();
    I2C_SDA_INIT_OUT();

    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            I2C_SDA_H();
        } else {
            I2C_SDA_L();
        }
        data = data << 1;

        I2C_DELAY(DELAY_US);
        I2C_SCL_H();
        I2C_DELAY(DELAY_US);
        I2C_SCL_L();
    }

    return;
}

/**
 * @brief i2c read byte
 *
 * @param[in] i2c_pin: pin number
 * @param[in] need_ack: if need ack
 *
 * @return read byte
 */
static uint8_t __sw_i2c_read_byte(SW_I2C_GPIO_T i2c_pin, BOOL_T need_ack)
{
    uint8_t read_byte = 0x00;
    uint8_t i = 0;

    I2C_SCL_L();
    I2C_SDA_INIT_IN();

    for (i = 0; i < 8; i++) {
        I2C_SCL_L();
        I2C_DELAY(DELAY_US);
        I2C_SCL_H();
        read_byte <<= 1;
        if (I2C_SDA_READ()) {
            read_byte++;
        }
    }

    if (need_ack) {
        __sw_i2c_ack(i2c_pin);
    } else {
        __sw_i2c_no_ack(i2c_pin);
    }

    return read_byte;
}

/**
 * @brief software i2c write data
 *
 * @param[in] port: i2c port
 * @param[in] addr: slave address
 * @param[in] buf: data buffer
 * @param[in] len: data length
 * @param[in] flags: flags
 *
 * @return operation result
 */
static int __sw_i2c_write_data(uint8_t port, uint8_t addr, const uint8_t *buf, uint8_t len, uint32_t flags)
{
    if (0 == (flags & SW_I2C_FLAG_NO_START)) {
        __sw_i2c_start(sg_i2c_pin[port]);
    }
    if (0 == (flags & SW_I2C_FLAG_NO_ADDR)) {
        __sw_i2c_send_byte(sg_i2c_pin[port], (addr << 1) | I2C_WRITE);
        if (0 == (flags & SW_I2C_FLAG_NO_READ_ACK)) {
            if (!__sw_i2c_get_ack(sg_i2c_pin[port], flags)) {
                __sw_i2c_stop(sg_i2c_pin[port]);
                return -1;
            }
        }
    }

    for (uint8_t i = 0; i < len; i++) {
        __sw_i2c_send_byte(sg_i2c_pin[port], buf[i]);
        if (0 == (flags & SW_I2C_FLAG_NO_READ_ACK)) {
            if (!__sw_i2c_get_ack(sg_i2c_pin[port], flags)) {
                __sw_i2c_stop(sg_i2c_pin[port]);
                return -1;
            }
        }
    }

    __sw_i2c_stop(sg_i2c_pin[port]);

    return 0;
}

/**
 * @brief software i2c read data
 *
 * @param[in] port: i2c port
 * @param[in] addr: slave address
 * @param[in] buf: data buffer
 * @param[in] len: data length
 * @param[in] flags: flags
 *
 * @return operation result
 */
static int __sw_i2c_read_data(uint8_t port, uint8_t addr, uint8_t *buf, uint8_t len, uint32_t flags)
{
    uint8_t i;

    if (0 == (flags & SW_I2C_FLAG_NO_START)) {
        __sw_i2c_start(sg_i2c_pin[port]);
    }
    if (0 == (flags & SW_I2C_FLAG_NO_ADDR)) {
        __sw_i2c_send_byte(sg_i2c_pin[port], (addr << 1) | I2C_READ);
        if (0 == (flags & SW_I2C_FLAG_NO_READ_ACK)) {
            if (!__sw_i2c_get_ack(sg_i2c_pin[port], flags)) {
                __sw_i2c_stop(sg_i2c_pin[port]);
                return -1;
            }
        }
    }

    for (i = 0; i < len - 1; i++) {
        buf[i] = __sw_i2c_read_byte(sg_i2c_pin[port], TRUE);
    }
    buf[i] = __sw_i2c_read_byte(sg_i2c_pin[port], FALSE);

    __sw_i2c_stop(sg_i2c_pin[port]);

    return 0;
}

/**
 * @brief software i2c init
 *
 * @param[in] i2c_pin: i2c pin number
 *
 * @return operation result
 */
static void __sw_i2c_init(SW_I2C_GPIO_T i2c_pin)
{
    I2C_SCL_INIT();
    I2C_SDA_INIT_OUT();
    I2C_SCL_H();
    I2C_SDA_H();

    return;
}

/**
 * @brief i2c init
 *
 * @param[in] i2c_pin: i2c pin number
 * @param[out] port: i2c pin number
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_init(uint8_t port, SW_I2C_GPIO_T i2c_pin)
{
    if (port >= SW_I2C_PORT_NUM_MAX) {
        return OPRT_INVALID_PARM;
    }

    sg_i2c_pin[port].scl = i2c_pin.scl;
    sg_i2c_pin[port].sda = i2c_pin.sda;
    __sw_i2c_init(sg_i2c_pin[port]);

    return OPRT_OK;
}

/**
 * @brief i2c deinit
 *
 * @param[in] i2c_pin: i2c pin number
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_deinit(uint8_t port)
{
    if (port >= SW_I2C_PORT_NUM_MAX) {
        return OPRT_INVALID_PARM;
    }

    tkl_gpio_deinit(sg_i2c_pin[port].scl);
    tkl_gpio_deinit(sg_i2c_pin[port].sda);

    return OPRT_OK;
}

/**
 * @brief i2c transfer
 *
 * @param[in] i2c_pin: i2c pin number
 * @param[in] msg: msg send to i2c bus
 *
 * @return OPRT_OK on success, others on error
 */
OPERATE_RET tdd_sw_i2c_xfer(uint8_t port, SW_I2C_MSG_T *msg)
{
    if ((port >= SW_I2C_PORT_NUM_MAX) || (NULL == msg) || (NULL == msg->buff) || (0 == msg->len)) {
        return OPRT_INVALID_PARM;
    }

    if (msg->flags & SW_I2C_FLAG_WR) {
        __sw_i2c_write_data(port, msg->addr, msg->buff, (uint8_t)msg->len, msg->flags);
    }

    if (msg->flags & SW_I2C_FLAG_RD) {
        __sw_i2c_read_data(port, msg->addr, msg->buff, (uint8_t)msg->len, msg->flags);
    }

    return OPRT_OK;
}

#endif