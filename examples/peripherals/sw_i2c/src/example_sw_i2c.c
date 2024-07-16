/**
 * @file example_sw_i2c.c
 * @brief Software I2C driver implementation for SDK.
 *
 * This file contains the implementation of a software-based I2C (Inter-Integrated Circuit) driver, designed for Tuya
 * IoT devices that lack hardware I2C support or require additional I2C interfaces. It demonstrates how to perform basic
 * I2C operations such as sending and receiving data over GPIO pins, using bit-banging techniques. The implementation
 * includes functions for initializing the I2C bus, starting and stopping I2C transactions, reading from and writing to
 * I2C slave devices, and checking data integrity using CRC8. The example focuses on interfacing with the SHT3x series
 * temperature and humidity sensors, but it can be adapted to communicate with any I2C device.
 *
 * Key features:
 * - Initialization and deinitialization of software I2C bus.
 * - Reading from and writing to I2C devices.
 * - CRC8 data integrity check for communication with devices like SHT3x.
 * - Example usage with SHT3x temperature and humidity sensor.
 *
 * This implementation is intended for developers working on Tuya IoT projects that require custom I2C communication
 * solutions, offering a flexible and portable approach to I2C device management on platforms without native I2C
 * support.
 *
 * @note This software I2C driver is designed for educational and development purposes and may not be optimized for all
 * production environments. For critical applications, hardware I2C modules, if available, are recommended.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"

#include "tal_api.h"
#include "tkl_output.h"
#include "tkl_gpio.h"
#include "tdd_sw_i2c.h"

/***********************************************************
*********************** macro define ***********************
***********************************************************/
/**
 * @brief CRC result
 */
#define CRC_OK  (0)
#define CRC_ERR (-1)

/**
 * @brief pin define
 */
#define SHT30_SCL_PIN TUYA_GPIO_NUM_20
#define SHT30_SDA_PIN TUYA_GPIO_NUM_22
#define SHT30_ALT_PIN TUYA_GPIO_NUM_14

#define SHT3X_ADDR          0x44
#define SHT3X_CMD_R_FETCH_H 0xE0 /* readout measurements for periodic mode */
#define SHT3X_CMD_R_FETCH_L 0x00

#define I2C_NUM_ID SW_I2C_PORT_NUM_0

#define TASK_IIC_PRIORITY THREAD_PRIO_1
#define TASK_IIC_SIZE     1024

#define I2C_WRITE_BUFLEN 2u
#define I2C_READ_BUFLEN  6u
/***********************************************************
********************** typedef define **********************
***********************************************************/

/***********************************************************
********************** variable define *********************
***********************************************************/
static THREAD_CFG_T sg_task = {.priority = TASK_IIC_PRIORITY, .stackDepth = TASK_IIC_SIZE, .thrdname = "sw_i2c"};

static THREAD_HANDLE sg_i2c_handle;

/***********************************************************
********************** function define *********************
***********************************************************/
/**
 * @brief get CRC8 value for sht3x
 *
 * @param[in] data: data to be calculated
 * @param[in] len: data length
 *
 * @return CRC8 value
 */
static uint8_t __sht3x_get_crc8(const uint8_t *data, uint16_t len)
{
    uint8_t i;
    uint8_t crc = 0xFF;

    while (len--) {
        crc ^= *data;

        for (i = 8; i > 0; --i) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = (crc << 1);
            }
        }
        data++;
    }

    return crc;
}

/**
 * @brief check CRC8
 *
 * @param[in] data: data to be checked
 * @param[in] len: data length
 * @param[in] crc_val: crc value
 *
 * @return check result
 */
static int32_t __sht3x_check_crc8(const uint8_t *data, const uint16_t len, const uint8_t crc_val)
{
    if (__sht3x_get_crc8(data, len) != crc_val) {
        return CRC_ERR;
    }
    return CRC_OK;
}

/**
 * @brief i2c task
 *
 * @param[in] param:Task parameters
 * @return none
 */
void __sw_i2c_task(void *param)
{
    OPERATE_RET op_ret = OPRT_OK;
    SW_I2C_MSG_T i2c_msg = {0};
    uint8_t read_buf[I2C_READ_BUFLEN] = {0};
    uint8_t write_buff[I2C_WRITE_BUFLEN] = {SHT3X_CMD_R_FETCH_H, SHT3X_CMD_R_FETCH_L};
    uint16_t temper = 0, humi = 0;

    /*i2c init*/
    SW_I2C_GPIO_T sw_i2c_gpio = {
        .scl = SHT30_SCL_PIN,
        .sda = SHT30_SDA_PIN,
    };
    op_ret = tdd_sw_i2c_init(I2C_NUM_ID, sw_i2c_gpio);
    if (OPRT_OK != op_ret) {
        PR_ERR("err<%d>,i2c init fail!", op_ret);
    }

    while (1) {
        /*IIC write data*/
        i2c_msg.addr = SHT3X_ADDR;
        i2c_msg.flags = SW_I2C_FLAG_WR;
        i2c_msg.buff = write_buff;
        i2c_msg.len = I2C_WRITE_BUFLEN;
        tdd_sw_i2c_xfer(I2C_NUM_ID, &i2c_msg);

        /*IIC read data*/
        i2c_msg.addr = SHT3X_ADDR;
        i2c_msg.flags = SW_I2C_FLAG_RD;
        i2c_msg.buff = read_buf;
        i2c_msg.len = I2C_READ_BUFLEN;
        tdd_sw_i2c_xfer(I2C_NUM_ID, &i2c_msg);

        /*check read data*/
        if ((CRC_ERR == __sht3x_check_crc8(read_buf, 2, read_buf[2])) ||
            (CRC_ERR == __sht3x_check_crc8(read_buf + 3, 2, read_buf[5]))) {
            PR_ERR("[SHT3x] The received temp_humi data can't pass the CRC8 check.");
        } else {
            temper = ((uint16_t)read_buf[0] << 8) | read_buf[1];
            humi = ((uint16_t)read_buf[3] << 8) | read_buf[4];

            PR_NOTICE("temper = %d humi = %d", temper, humi);
        }

        tal_system_sleep(2000);
    }
}

/**
 * @brief user_main
 *
 * @return none
 */
void user_main()
{
    OPERATE_RET rt = OPRT_OK;

    /* basic init */
    tal_log_init(TAL_LOG_LEVEL_DEBUG, 1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);

    /* a sw i2c thread */
    TUYA_CALL_ERR_LOG(tal_thread_create_and_start(&sg_i2c_handle, NULL, NULL, __sw_i2c_task, NULL, &sg_task));

    return;
}

/**
 * @brief main
 *
 * @param argc
 * @param argv
 * @return void
 */
#if OPERATING_SYSTEM == SYSTEM_LINUX
void main(int argc, char *argv[])
{
    user_main();

    while (1) {
        tal_system_sleep(500);
    }
}
#else

/* Tuya thread handle */
static THREAD_HANDLE ty_app_thread = NULL;

/**
 * @brief  task thread
 *
 * @param[in] arg:Parameters when creating a task
 * @return none
 */
static void tuya_app_thread(void *arg)
{
    user_main();

    tal_thread_delete(ty_app_thread);
    ty_app_thread = NULL;
}

void tuya_app_main(void)
{
    THREAD_CFG_T thrd_param = {4096, 4, "tuya_app_main"};
    tal_thread_create_and_start(&ty_app_thread, NULL, NULL, tuya_app_thread, NULL, &thrd_param);
}
#endif
