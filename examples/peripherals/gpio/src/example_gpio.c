/**
 * @file example_gpio.c
 * @brief GPIO driver example for Tuya IoT projects.
 * 
 * This file provides an example implementation of a GPIO (General Purpose Input/Output) driver using the Tuya SDK. 
 * It demonstrates the configuration and usage of GPIO pins for input, output, and interrupt-driven events. 
 * The example covers initializing GPIO pins in different modes, writing to output pins, reading from input pins, and setting up interrupts on GPIO pins to handle external events.
 * 
 * The GPIO driver example aims to help developers understand how to control hardware peripherals and manage event-driven programming in Tuya IoT projects. 
 * It includes detailed examples of setting up GPIO configurations, handling interrupts with callbacks, and integrating these functionalities within a task-based application structure.
 * 
 * @note This example is designed to be adaptable to various Tuya IoT devices and platforms, showcasing fundamental GPIO operations that are critical for IoT device development.
 * 
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 * 
 */

#include "tuya_cloud_types.h"
#include "tal_api.h"
#include "tkl_output.h"
#include "tkl_gpio.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define GPIO_OUT_PIN_T2       TUYA_GPIO_NUM_5
#define GPIO_IN_PIN_T2         TUYA_GPIO_NUM_26
#define GPIO_IRQ_PIN_T2        TUYA_GPIO_NUM_14

#define TASK_GPIO_PRIORITY      THREAD_PRIO_2
#define TASK_GPIO_SIZE          1024

/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/
static THREAD_HANDLE sg_gpio_handle;

/***********************************************************
***********************function define**********************
***********************************************************/
/**
* @brief interrupt callback function
*
* @param[in] args:parameters
* @return none
*/
static void __gpio_irq_callback(void *args)
{
    /* TAL_PR_ , PR_ ，这两种打印里面有锁，不要在中断里使用 */
    tkl_log_output("\r\n------------ GPIO IRQ Callbcak ------------\r\n");
}

/**
 * @brief 根据不同芯片平台选择不同引脚
 *
 * @param[in] : 
 *
 * @return none
 */
static void __example_pin_get(TUYA_GPIO_NUM_E *out_pin, TUYA_GPIO_NUM_E *in_pin, TUYA_GPIO_NUM_E *irq_pin)
{
    *out_pin = GPIO_OUT_PIN_T2;
    *in_pin = GPIO_IN_PIN_T2;
    *irq_pin = GPIO_IRQ_PIN_T2;
    return;
}

/**
* @brief gpio task
*
* @param[in] param:Task parameters
* @return none
*/
static void __example_gpio_task(void *param)
{
    OPERATE_RET rt = OPRT_OK;
    UCHAR_T i = 0;
    TUYA_GPIO_LEVEL_E read_level = 0;

    TUYA_GPIO_NUM_E out_pin, in_pin, irq_pin;

    __example_pin_get(&out_pin, &in_pin, &irq_pin);

    /*GPIO output init*/
    TUYA_GPIO_BASE_CFG_T out_pin_cfg = {
        .mode = TUYA_GPIO_PUSH_PULL,
        .direct = TUYA_GPIO_OUTPUT,
        .level = TUYA_GPIO_LEVEL_LOW
    };
    TUYA_CALL_ERR_LOG(tkl_gpio_init(out_pin, &out_pin_cfg));

    /*GPIO input init*/
    TUYA_GPIO_BASE_CFG_T in_pin_cfg = {
        .mode = TUYA_GPIO_PUSH_PULL,
        .direct = TUYA_GPIO_INPUT,
    };
    TUYA_CALL_ERR_LOG(tkl_gpio_init(in_pin, &in_pin_cfg));

    /*GPIO irq init*/
    TUYA_GPIO_IRQ_T irq_cfg = {
        .cb = __gpio_irq_callback,
        .arg = NULL,
        .mode = TUYA_GPIO_IRQ_RISE,
    };
    TUYA_CALL_ERR_LOG(tkl_gpio_irq_init(irq_pin, &irq_cfg));

    /*irq enable*/
    TUYA_CALL_ERR_LOG(tkl_gpio_irq_enable(irq_pin));

    while (1) {
        /* GPIO output */
        if (i == 0) {
            tkl_gpio_write(out_pin, TUYA_GPIO_LEVEL_HIGH);
            PR_DEBUG("pin output high");
        } else {
            tkl_gpio_write(out_pin, TUYA_GPIO_LEVEL_LOW);
            PR_DEBUG("pin output low");
        }
        i = i^1;

        /* GPIO read */
        TUYA_CALL_ERR_LOG(tkl_gpio_read(in_pin, &read_level));
        if(read_level == 1) {
            PR_DEBUG("GPIO read high level");
        } else {
            PR_DEBUG("GPIO read low level");
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
    tal_log_init(TAL_LOG_LEVEL_DEBUG,  1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);

    static THREAD_CFG_T thrd_param = {
        .priority = TASK_GPIO_PRIORITY,
        .stackDepth = TASK_GPIO_SIZE,
        .thrdname = "gpio"
    };
    TUYA_CALL_ERR_LOG(tal_thread_create_and_start(&sg_gpio_handle, NULL, NULL, __example_gpio_task, NULL, &thrd_param));

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

    while (1)
    {
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