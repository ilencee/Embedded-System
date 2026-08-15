/**
 * @file    pump.c
 * @brief   水泵控制模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.1.0
 */

#include "pump.h"

void pump_init(void)
{
    rcu_periph_clock_enable(PUMP_RCU);
    gpio_mode_set(PUMP_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PUMP_PIN);
    /* 泵为低速开关信号，2MHz 输出速度足以满足需求并降低 EMI/振铃 */
    gpio_output_options_set(PUMP_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, PUMP_PIN);
    gpio_bit_reset(PUMP_PORT, PUMP_PIN);
}

/*
 * 注意：以下实现基于"高电平有效"（active-high）驱动电路假设。
 * 若硬件为低电平导通，需将 on/off 的置位逻辑互换。
 */
void pump_switch_on(void)
{
    gpio_bit_set(PUMP_PORT, PUMP_PIN);
}

void pump_switch_off(void)
{
    gpio_bit_reset(PUMP_PORT, PUMP_PIN);
}
