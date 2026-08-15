/**
 * @file    zero.c
 * @brief   市电过零检测模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "zero.h"
#include "systick.h"

/* 过零事件计数（双边沿触发，50Hz 市电每秒约 100 次） */
volatile uint32_t zero_count = 0U;

/* 相邻两次过零间隔（半周期，单位 ms） */
volatile uint32_t zero_half_period = 0U;

/* 市电全周期（单位 ms） */
volatile uint32_t zero_full_period = 0U;

/* 最近一次过零发生的系统 tick（单位 ms） */
volatile uint32_t zero_last_tick = 0U;

/* 去抖阈值：过零信号两次边沿间隔小于 5ms 视为干扰，忽略 */
#define ZERO_DEBOUNCE_MS  5U

/* 过零检测初始化（PB0 + EXTI0 双边沿中断） */
int zero_init(void)
{
    /* SYSCFG 用于将 EXTI0 映射到 PB0 */
    rcu_periph_clock_enable(RCU_CFGCMP);
    rcu_periph_clock_enable(zero_rcu);

    /* 将 EXTI0 连接到 PB0 */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN0);

    /* PB0 输入模式，专用过零芯片推挽输出，无需上下拉 */
    gpio_mode_set(zero_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, zero_pin);

    /* 双边沿触发：方波上升/下降沿各对应一次过零 */
    exti_init(zero_exti_line, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_enable(zero_exti_line);

    /* 低优先级，不阻塞主循环 */
    nvic_irq_enable(EXTI0_1_IRQn, 0x01U);

    return 0;
}

/* 过零中断服务函数（由 EXTI0_1_IRQHandler 调用） */
void zero_cross_isr(void)
{
    uint32_t now;
    uint32_t interval;

    now = tick_ms_get();

    /* 去抖：与上次过零间隔过小时视为干扰 */
    if(zero_last_tick != 0U){
        interval = now - zero_last_tick;
        if(interval < ZERO_DEBOUNCE_MS){
            return;
        }
        /* 相邻两次边沿间隔为市电半周期（50Hz 约 10ms） */
        zero_half_period = interval;
        /* 一个完整周期 = 两个半周期（约 20ms） */
        zero_full_period = interval * 2U;
    }

    zero_last_tick = now;
    zero_count++;
}

/* 获取过零事件计数 */
uint32_t zero_count_get(void)
{
    return zero_count;
}

/* 获取相邻两次过零间隔（半周期，单位 ms，50Hz 约 10ms） */
uint32_t zero_half_period_get(void)
{
    return zero_half_period;
}

/* 获取市电全周期（单位 ms，50Hz 约 20ms） */
uint32_t zero_full_period_get(void)
{
    return zero_full_period;
}

/* 获取市电频率（单位 Hz，数据无效时返回 0） */
uint32_t zero_frequency_get(void)
{
    if(zero_half_period == 0U){
        return 0U;
    }
    /* 频率 = 1000 / 半周期 / 2 = 500 / 半周期 */
    return 500U / zero_half_period;
}

/* 获取最近一次过零发生的系统 tick（单位 ms） */
uint32_t zero_last_tick_get(void)
{
    return zero_last_tick;
}
