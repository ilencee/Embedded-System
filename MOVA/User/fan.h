#ifndef FAN_H
#define FAN_H
#include "gd32e23x.h"

/* 风扇控制引脚使用的 GPIO 端口 */
#define fan_port GPIOA
/* 风扇控制引脚使用的 GPIO 引脚 */
#define fan_pin GPIO_PIN_9
/* 风扇控制引脚使用的 GPIO 端口时钟使能枚举 */
#define fan_rcu  RCU_GPIOA

/* 风扇初始化函数 */
int fan_init(void);

#endif
