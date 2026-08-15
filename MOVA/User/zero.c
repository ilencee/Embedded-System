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

/*!
    \brief      过零检测初始化
    \param[in]  none
    \retval     0 成功
*/
int zero_init(void)
{
    /* 使能 SYSCFG 外设时钟（EXTI 源选择需要，将 EXTI0 映射到 PB0） */
    rcu_periph_clock_enable(RCU_CFGCMP);
    /* 使能 GPIOB 端口时钟（PB0 引脚需要） */
    rcu_periph_clock_enable(zero_rcu);

    /* 将 EXTI0 连接到 PB0 */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN0);

    /* 配置 PB0 为输入模式（专用过零芯片输出方波，推挽驱动，无需上下拉） */
    gpio_mode_set(zero_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, zero_pin);

    /* EXTI0 双边沿触发：过零芯片方波的上升沿/下降沿各对应一次过零 */
    exti_init(zero_exti_line, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_enable(zero_exti_line);

    /* 使能 EXTI0_1 中断（低优先级，不阻塞主循环） */
    nvic_irq_enable(EXTI0_1_IRQn, 0x01U);

    return 0;
}

/*!
    \brief      过零中断服务函数（由 EXTI0_1_IRQHandler 调用）
    \param[in]  none
    \param[out] none
    \retval     none
*/

void zero_cross_isr(void)
{
    uint32_t now;
    uint32_t interval;

    /* 读取当前系统 tick（ms） */
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

/*!
    \brief      获取过零事件计数
    \param[in]  none
    \retval     过零事件计数
*/
uint32_t zero_count_get(void)
{
    return zero_count;
}

/*!
    \brief      获取相邻两次过零间隔（半周期，单位 ms，50Hz 约 10ms）
    \param[in]  none
    \retval     半周期时间
*/
uint32_t zero_half_period_get(void)
{
    return zero_half_period;
}

/*!
    \brief      获取市电全周期（单位 ms，50Hz 约 20ms）
    \param[in]  none
    \retval     全周期时间
*/
uint32_t zero_full_period_get(void)
{
    return zero_full_period;
}

/*!
    \brief      获取市电频率（单位 Hz，50Hz 时返回 50）
    \param[in]  none
    \retval     频率值；数据无效时返回 0
*/
uint32_t zero_frequency_get(void)
{
    if(zero_half_period == 0U){
        return 0U;
    }
    /* 频率 = 1000 / 半周期 / 2 = 500 / 半周期 */
    return 500U / zero_half_period;
}

/*!
    \brief      获取最近一次过零发生的系统 tick（单位 ms）
    \param[in]  none
    \retval     系统 tick
*/
uint32_t zero_last_tick_get(void)
{
    return zero_last_tick;
}