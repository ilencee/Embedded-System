/**
 * @file    12VON.c
 * @brief   12V 电源控制实现
 *
 * @author  ilencee
 * @date    2026-08-17
 * @version V1.0.0
 */

#include "12VON.h"

/**
 * @brief  初始化 12V 电源控制引脚（推挽输出，默认关闭）
 */
void V12ON_Init(void)
{
    rcu_periph_clock_enable(V12ON_GPIO_CLK);   /* 使能 GPIOB 时钟 */
    gpio_mode_set(V12ON_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, V12ON_PIN);
    /* 12V 开关为低速控制信号，2MHz 输出速度满足需求并降低 EMI */
    gpio_output_options_set(V12ON_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, V12ON_PIN);
    gpio_bit_reset(V12ON_GPIO, V12ON_PIN);   /* 默认关闭，避免上电误动作 */
}


/**
 * @brief  开启 12V 电源
 */
void V12ON_Set(void)
{
    gpio_bit_set(V12ON_GPIO, V12ON_PIN);
}

/**
 * @brief  关闭 12V 电源
 */
void V12ON_Reset(void)
{
    gpio_bit_reset(V12ON_GPIO, V12ON_PIN);
}

/**
 * @brief  翻转 12V 电源状态
 */
void V12ON_Toggle(void)
{
    if (gpio_output_bit_get(V12ON_GPIO, V12ON_PIN) == RESET) {
        gpio_bit_set(V12ON_GPIO, V12ON_PIN);
    } else {
        gpio_bit_reset(V12ON_GPIO, V12ON_PIN);
    }
}
