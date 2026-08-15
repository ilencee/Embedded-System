#ifndef ZERO_H
#define ZERO_H

#include "gd32e23x.h"
#include <stdint.h>

/* 过零检测引脚定义：专用过零芯片输出方波连接至 PB0 */
#define zero_port       GPIOB
#define zero_pin        GPIO_PIN_0
#define zero_rcu        RCU_GPIOB
#define zero_exti_line  EXTI_0

/* 过零检测初始化 */
int zero_init(void);

/* 过零中断服务函数（由 EXTI0_1_IRQHandler 调用） */
void zero_cross_isr(void);

/* 获取过零事件计数（每次边沿 +1，50Hz 市电每秒约 100 次） */
uint32_t zero_count_get(void);

/* 获取相邻两次过零间隔（半周期，单位 ms，50Hz 约 10ms） */
uint32_t zero_half_period_get(void);

/* 获取市电全周期（单位 ms，50Hz 约 20ms） */
uint32_t zero_full_period_get(void);

/* 获取市电频率（单位 Hz，计算得到 50） */
uint32_t zero_frequency_get(void);

/* 获取最近一次过零发生的系统 tick（单位 ms） */
uint32_t zero_last_tick_get(void);

#endif /* ZERO_H */