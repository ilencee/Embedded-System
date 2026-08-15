#ifndef PUMP_H
#define PUMP_H
#include "gd32e23x.h"

/* 泵控制引脚使用的 GPIO 端口 */
#define pump_port GPIOA
/* 泵控制引脚使用的 GPIO 引脚 */
#define pump_pin GPIO_PIN_8
/* 泵控制引脚使用的 GPIO 端口时钟使能枚举 */
#define pump_rcu RCU_GPIOA

/* 泵初始化函数 */
int pump_init(void);

#endif