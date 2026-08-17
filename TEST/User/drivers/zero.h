/**
 * @file    zero.h
 * @brief   市电过零检测模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#ifndef _ZERO_H_
#define _ZERO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"

/* 过零检测引脚使用的 GPIO 端口（PB0，需配合 SYSCFG 将 EXTI0 映射到 PB0） */
#define ZERO_PORT       GPIOB
/* 过零检测引脚使用的 GPIO 引脚 */
#define ZERO_PIN        GPIO_PIN_0
/* 过零检测引脚使用的 GPIO 端口时钟使能枚举 */
#define ZERO_RCU        RCU_GPIOB
/* 过零检测使用的 EXTI 线 */
#define ZERO_EXTI_LINE  EXTI_0
/* 过零检测中断优先级（低于 SysTick，高于 USART1） */
#define ZERO_IRQ_PRIO   1U

/**
 * @brief  初始化过零检测（配置 GPIO、EXTI 双边沿中断）
 */
void Zero_Init(void);

/**
 * @brief  过零中断服务处理（由 EXTI0_1_IRQHandler 调用）
 */
void Zero_CrossIsr(void);

/**
 * @brief  获取过零事件计数
 * @return 过零事件累计次数
 */
uint32_t Zero_GetCount(void);

/**
 * @brief  获取相邻两次过零间隔（半周期）
 * @return 半周期时长（单位 ms）
 */
uint32_t Zero_GetHalfPeriod(void);

/**
 * @brief  获取市电全周期
 * @return 全周期时长（单位 ms）
 */
uint32_t Zero_GetFullPeriod(void);

/**
 * @brief  获取市电频率
 * @return 频率（单位 Hz），数据无效时返回 0
 */
uint32_t Zero_GetFrequency(void);

/**
 * @brief  获取最近一次过零发生的系统 tick
 * @return 系统运行时间（单位 ms）
 */
uint32_t Zero_GetLastTick(void);

#ifdef __cplusplus
}
#endif

#endif /* _ZERO_H_ */
