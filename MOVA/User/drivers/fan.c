/**
 * @file    fan.c
 * @brief   风扇控制模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#include "fan.h"

/**
 * @brief  初始化风扇控制引脚（GPIO 推挽输出）
 */
void Fan_Init(void)
{
    rcu_periph_clock_enable(FAN_RCU);
    gpio_mode_set(FAN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FAN_PIN);
    /* 风扇为低速开关信号，2MHz 输出速度足以满足需求并降低 EMI/振铃 */
    gpio_output_options_set(FAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, FAN_PIN);
    gpio_bit_reset(FAN_PORT, FAN_PIN);
}

/*
 * 注意：以下实现基于"高电平有效"（active-high）驱动电路假设。
 * 若硬件为低电平导通，需将 on/off 的置位逻辑互换。
 */
void Fan_SwitchOn(void)
{
    gpio_bit_set(FAN_PORT, FAN_PIN);
}

void Fan_SwitchOff(void)
{
    gpio_bit_reset(FAN_PORT, FAN_PIN);
}
