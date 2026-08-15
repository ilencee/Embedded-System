/**
 * @file    fan.h
 * @brief   风扇控制模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef FAN_H
#define FAN_H
#include "gd32e23x.h"

/* 风扇控制引脚使用的 GPIO 端口 */
#define fan_port GPIOA
/* 风扇控制引脚使用的 GPIO 引脚 */
#define fan_pin GPIO_PIN_9
/* 风扇控制引脚使用的 GPIO 端口时钟使能枚举 */
#define fan_rcu  RCU_GPIOA

/* 风扇初始化 */
int fan_init(void);
/* 打开风扇 */
void fan_switch_on(void);
/* 关闭风扇 */
void fan_switch_off(void);

#endif
