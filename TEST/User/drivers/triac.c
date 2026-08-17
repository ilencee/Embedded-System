/**
 * @file    triac.c
 * @brief   可控硅（Triac）控制模块实现
 *
 *          四路可控硅分别控制四个交流负载：
 *          HEAT（加热）、PTC（PTC 加热器）、UVA（UVA 紫外灯）、UVB（UVB 紫外灯）
 *
 * @author  ilencee
 * @date    2026-08-17
 * @version V1.6.0
 *
 * @note    光耦驱动，低电平有效（导通=输出低电平，关断=输出高电平）
 */

#include "triac.h"

/**
 * @brief  初始化四个可控硅控制引脚（GPIO 推挽输出，默认关断）
 */
void Triac_Init(void)
{
    rcu_periph_clock_enable(TRIAC_RCU);

    /* HEAT 加热负载 -> PB1 */
    gpio_mode_set(TRIAC_HEAT_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TRIAC_HEAT_PIN);
    /* 可控硅触发为低速开关量，2MHz 输出速度可降低 EMI/振铃 */
    gpio_output_options_set(TRIAC_HEAT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TRIAC_HEAT_PIN);
    gpio_bit_set(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN);          /* 低电平有效：输出高电平=关断 */

    /* PTC 加热负载 -> PB2 */
    gpio_mode_set(TRIAC_PTC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TRIAC_PTC_PIN);
    gpio_output_options_set(TRIAC_PTC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TRIAC_PTC_PIN);
    gpio_bit_set(TRIAC_PTC_PORT, TRIAC_PTC_PIN);

    /* UVA 紫外灯 -> PB10 */
    gpio_mode_set(TRIAC_UVA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TRIAC_UVA_PIN);
    gpio_output_options_set(TRIAC_UVA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TRIAC_UVA_PIN);
    gpio_bit_set(TRIAC_UVA_PORT, TRIAC_UVA_PIN);

    /* UVB 紫外灯 -> PB11 */
    gpio_mode_set(TRIAC_UVB_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TRIAC_UVB_PIN);
    gpio_output_options_set(TRIAC_UVB_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TRIAC_UVB_PIN);
    gpio_bit_set(TRIAC_UVB_PORT, TRIAC_UVB_PIN);
}

/*
 * 注意：光耦驱动，低电平有效（active-low）：
 *   导通 = 输出低电平（gpio_bit_reset），关断 = 输出高电平（gpio_bit_set）。
 * 若光耦为 MOC3021 等需触发脉冲的型号，须由应用层配合过零检测
 * （zero.c）在合适的相位角输出触发脉冲，本模块提供基础电平开关接口。
 */
void Triac_SwitchOn(uint8_t ch)
{
    switch (ch) {
        case TRIAC_HEAT: gpio_bit_reset(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN); break;
        case TRIAC_PTC:  gpio_bit_reset(TRIAC_PTC_PORT,  TRIAC_PTC_PIN);  break;
        case TRIAC_UVA:  gpio_bit_reset(TRIAC_UVA_PORT,  TRIAC_UVA_PIN);  break;
        case TRIAC_UVB:  gpio_bit_reset(TRIAC_UVB_PORT,  TRIAC_UVB_PIN);  break;
        default: break;     /* 无效通道号忽略 */
    }
}

void Triac_SwitchOff(uint8_t ch)
{
    switch (ch) {
        case TRIAC_HEAT: gpio_bit_set(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN); break;
        case TRIAC_PTC:  gpio_bit_set(TRIAC_PTC_PORT,  TRIAC_PTC_PIN);  break;
        case TRIAC_UVA:  gpio_bit_set(TRIAC_UVA_PORT,  TRIAC_UVA_PIN);  break;
        case TRIAC_UVB:  gpio_bit_set(TRIAC_UVB_PORT,  TRIAC_UVB_PIN);  break;
        default: break;     /* 无效通道号忽略 */
    }
}

void Triac_Toggle(uint8_t ch)
{
    switch (ch) {
        case TRIAC_HEAT: gpio_bit_toggle(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN); break;
        case TRIAC_PTC:  gpio_bit_toggle(TRIAC_PTC_PORT,  TRIAC_PTC_PIN);  break;
        case TRIAC_UVA:  gpio_bit_toggle(TRIAC_UVA_PORT,  TRIAC_UVA_PIN);  break;
        case TRIAC_UVB:  gpio_bit_toggle(TRIAC_UVB_PORT,  TRIAC_UVB_PIN);  break;
        default: break;     /* 无效通道号忽略 */
    }
}

void Triac_SwitchOnAll(void)
{
    gpio_bit_reset(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN);
    gpio_bit_reset(TRIAC_PTC_PORT, TRIAC_PTC_PIN);
    gpio_bit_reset(TRIAC_UVA_PORT, TRIAC_UVA_PIN);
    gpio_bit_reset(TRIAC_UVB_PORT, TRIAC_UVB_PIN);
}

void Triac_SwitchOffAll(void)
{
    gpio_bit_set(TRIAC_HEAT_PORT, TRIAC_HEAT_PIN);
    gpio_bit_set(TRIAC_PTC_PORT, TRIAC_PTC_PIN);
    gpio_bit_set(TRIAC_UVA_PORT, TRIAC_UVA_PIN);
    gpio_bit_set(TRIAC_UVB_PORT, TRIAC_UVB_PIN);
}

/* 各负载便捷开关函数 */
void Triac_HeatOn(void)  { Triac_SwitchOn(TRIAC_HEAT); }
void Triac_HeatOff(void) { Triac_SwitchOff(TRIAC_HEAT); }
void Triac_PtcOn(void)   { Triac_SwitchOn(TRIAC_PTC); }
void Triac_PtcOff(void)  { Triac_SwitchOff(TRIAC_PTC); }
void Triac_UvaOn(void)   { Triac_SwitchOn(TRIAC_UVA); }
void Triac_UvaOff(void)  { Triac_SwitchOff(TRIAC_UVA); }
void Triac_UvbOn(void)   { Triac_SwitchOn(TRIAC_UVB); }
void Triac_UvbOff(void)  { Triac_SwitchOff(TRIAC_UVB); }
