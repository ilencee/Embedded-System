/**
 * @file    zero.h
 * @brief   市电过零检测模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef ZERO_H
#define ZERO_H

#include "gd32e23x.h"

/* 过零检测引脚使用的 GPIO 端口（PB0，需配合 SYSCFG 将 EXTI0 映射到 PB0） */
#define zero_port      GPIOB
/* 过零检测引脚使用的 GPIO 引脚 */
#define zero_pin       GPIO_PIN_0
/* 过零检测引脚使用的 GPIO 端口时钟使能枚举 */
#define zero_rcu       RCU_GPIOB
/* 过零检测使用的 EXTI 线 */
#define zero_exti_line EXTI_0

/* 过零事件计数（双边沿触发，50Hz 市电每秒约 100 次） */
extern volatile uint32_t zero_count;
/* 相邻两次过零间隔（半周期，单位 ms，50Hz 约 10ms） */
extern volatile uint32_t zero_half_period;
/* 市电全周期（单位 ms，50Hz 约 20ms） */
extern volatile uint32_t zero_full_period;
/* 最近一次过零发生的系统 tick（单位 ms） */
extern volatile uint32_t zero_last_tick;

/* 过零检测初始化（配置 GPIO、EXTI 中断） */
void zero_init(void);

/* 过零中断服务函数（由 EXTI0_1_IRQHandler 调用） */
void zero_cross_isr(void);

/* 获取过零事件计数 */
uint32_t zero_count_get(void);

/* 获取相邻两次过零间隔（半周期，单位 ms） */
uint32_t zero_half_period_get(void);

/* 获取市电全周期（单位 ms） */
uint32_t zero_full_period_get(void);

/* 获取市电频率（单位 Hz，数据无效时返回 0） */
uint32_t zero_frequency_get(void);

/* 获取最近一次过零发生的系统 tick（单位 ms） */
uint32_t zero_last_tick_get(void);

#endif
