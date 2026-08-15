/**
 * @file    fan.h
 * @brief   风扇控制模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#ifndef _FAN_H_
#define _FAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"

/* 风扇控制引脚使用的 GPIO 端口 */
#define FAN_PORT    GPIOA
/* 风扇控制引脚使用的 GPIO 引脚
 * 注意：PA9 默认 AF0 为 USART1_TX，使用前请确认工程未将 USART1 复用到此引脚，
 * 否则会产生引脚驱动竞争。 */
#define FAN_PIN     GPIO_PIN_9
/* 风扇控制引脚使用的 GPIO 端口时钟使能枚举 */
#define FAN_RCU     RCU_GPIOA

/**
 * @brief  初始化风扇控制引脚（GPIO 推挽输出）
 */
void Fan_Init(void);

/**
 * @brief  打开风扇（高电平有效）
 */
void Fan_SwitchOn(void);

/**
 * @brief  关闭风扇（高电平有效）
 */
void Fan_SwitchOff(void);

#ifdef __cplusplus
}
#endif

#endif /* _FAN_H_ */
