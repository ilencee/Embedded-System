/**
 * @file    pump.c
 * @brief   水泵控制模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "pump.h"

int pump_init(void)
{
    rcu_periph_clock_enable(pump_rcu);
    gpio_mode_set(pump_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pump_pin);
    gpio_output_options_set(pump_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pump_pin);
    gpio_bit_reset(pump_port, pump_pin);
    return 0;
}

void pump_switch_on(void)
{
    gpio_bit_set(pump_port, pump_pin);
}

void pump_switch_off(void)
{
    gpio_bit_reset(pump_port, pump_pin);
}
