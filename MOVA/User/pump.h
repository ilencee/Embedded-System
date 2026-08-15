/**
 * @file    pump.h
 * @brief   水泵控制模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef PUMP_H
#define PUMP_H
#include "gd32e23x.h"

/* 泵控制引脚使用的 GPIO 端口 */
#define pump_port GPIOA
/* 泵控制引脚使用的 GPIO 引脚 */
#define pump_pin GPIO_PIN_8
/* 泵控制引脚使用的 GPIO 端口时钟使能枚举 */
#define pump_rcu RCU_GPIOA

/* 泵初始化 */
int pump_init(void);
/* 打开泵 */
void pump_switch_on(void);
/* 关闭泵 */
void pump_switch_off(void);

#endif