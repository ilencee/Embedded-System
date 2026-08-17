/**
 * @file    zero.c
 * @brief   市电过零检测模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#include "zero.h"
#include "systick.h"

/* 去抖阈值：过零信号两次边沿间隔小于 5ms 视为干扰，忽略 */
#define ZERO_DEBOUNCE_MS   5U
/* 半周期到频率换算系数：频率(Hz) = 1000 / 半周期(ms) / 2 */
#define ZERO_FREQ_SCALE    500U
/* 全周期换算因数：一个完整周期由两个半周期组成 */
#define ZERO_PERIOD_FACTOR 2U

/* 过零事件计数（双边沿触发，50Hz 市电每秒约 100 次） */
static volatile uint32_t s_zero_count = 0U;

/* 相邻两次过零间隔（半周期，单位 ms） */
static volatile uint32_t s_zero_half_period = 0U;

/* 市电全周期（单位 ms） */
static volatile uint32_t s_zero_full_period = 0U;

/* 最近一次过零发生的系统 tick（单位 ms） */
static volatile uint32_t s_zero_last_tick = 0U;

/**
 * @brief  初始化过零检测（PB0 + EXTI0 双边沿中断）
 */
void Zero_Init(void)
{
    /* SYSCFG 用于将 EXTI0 映射到 PB0 */
    rcu_periph_clock_enable(RCU_CFGCMP);
    rcu_periph_clock_enable(ZERO_RCU);

    /* 将 EXTI0 连接到 PB0 */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN0);

    /* PB0 输入模式，专用过零芯片推挽输出，无需上下拉 */
    gpio_mode_set(ZERO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ZERO_PIN);

    /* 双边沿触发：方波上升/下降沿各对应一次过零 */
    exti_init(ZERO_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_enable(ZERO_EXTI_LINE);

    /* 低优先级，不阻塞主循环 */
    nvic_irq_enable(EXTI0_1_IRQn, ZERO_IRQ_PRIO);
}

/**
 * @brief  过零中断服务处理（由 EXTI0_1_IRQHandler 调用）
 */
void Zero_CrossIsr(void)
{
    uint32_t now = 0U;
    uint32_t interval = 0U;

    now = Systick_GetTickMs();

    /* 去抖：与上次过零间隔过小时视为干扰 */
    if (s_zero_last_tick != 0U) {
        interval = now - s_zero_last_tick;
        if (interval < ZERO_DEBOUNCE_MS) {
            return;
        }
        /* 相邻两次边沿间隔为市电半周期（50Hz 约 10ms） */
        s_zero_half_period = interval;
        /* 一个完整周期 = 两个半周期（约 20ms） */
        s_zero_full_period = interval * ZERO_PERIOD_FACTOR;
    }

    s_zero_last_tick = now;
    s_zero_count++;
}

/**
 * @brief  获取过零事件计数
 * @return 过零事件累计次数
 */
uint32_t Zero_GetCount(void)
{
    return s_zero_count;
}

/**
 * @brief  获取相邻两次过零间隔（半周期）
 * @return 半周期时长（单位 ms，50Hz 约 10ms）
 */
uint32_t Zero_GetHalfPeriod(void)
{
    return s_zero_half_period;
}

/**
 * @brief  获取市电全周期
 * @return 全周期时长（单位 ms，50Hz 约 20ms）
 */
uint32_t Zero_GetFullPeriod(void)
{
    return s_zero_full_period;
}

/**
 * @brief  获取市电频率
 * @return 频率（单位 Hz），数据无效时返回 0
 */
uint32_t Zero_GetFrequency(void)
{
    if (s_zero_half_period == 0U) {
        return 0U;
    }
    return ZERO_FREQ_SCALE / s_zero_half_period;
}

/**
 * @brief  获取最近一次过零发生的系统 tick
 * @return 系统运行时间（单位 ms）
 */
uint32_t Zero_GetLastTick(void)
{
    return s_zero_last_tick;
}
