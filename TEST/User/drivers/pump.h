/**
 * @file    pump.h
 * @brief   水泵控制模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#ifndef _PUMP_H_
#define _PUMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"

/* 水泵控制引脚使用的 GPIO 端口 */
#define PUMP_PORT   GPIOA
/* 水泵控制引脚使用的 GPIO 引脚 */
#define PUMP_PIN    GPIO_PIN_8
/* 水泵控制引脚使用的 GPIO 端口时钟使能枚举 */
#define PUMP_RCU    RCU_GPIOA

/**
 * @brief  初始化水泵控制引脚（GPIO 推挽输出）
 */
void Pump_Init(void);

/**
 * @brief  打开水泵（高电平有效）
 */
void Pump_SwitchOn(void);

/**
 * @brief  关闭水泵（高电平有效）
 */
void Pump_SwitchOff(void);

#ifdef __cplusplus
}
#endif

#endif /* _PUMP_H_ */
