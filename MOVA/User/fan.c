/**
 * @file    fan.c
 * @brief   风扇控制模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "fan.h"

int fan_init(void)
{
    rcu_periph_clock_enable(fan_rcu);
    gpio_mode_set(fan_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, fan_pin);
    gpio_output_options_set(fan_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, fan_pin);
    gpio_bit_reset(fan_port, fan_pin);
    return 0;
}

void fan_switch_on(void)
{
    gpio_bit_set(fan_port, fan_pin);
}

void fan_switch_off(void)
{
    gpio_bit_reset(fan_port, fan_pin);
}
