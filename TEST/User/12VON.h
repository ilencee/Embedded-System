/**
 * @file    12VON.h
 * @brief   12V 电源控制接口声明
 *
 * @author  ilencee
 * @date    2026-08-17
 * @version V1.0.0
 */

#ifndef _V12ON_H_
#define _V12ON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"

/* ===== 引脚配置（默认 PB8，按实际硬件接线修改） ===== */
#define V12ON_PIN      GPIO_PIN_8
#define V12ON_GPIO     GPIOB
#define V12ON_GPIO_CLK RCU_GPIOB

/**
 * @brief  初始化 12V 电源控制引脚（推挽输出，默认关闭）
 */
void V12ON_Init(void);

/**
 * @brief  开启 12V 电源
 */
void V12ON_Set(void);

/**
 * @brief  关闭 12V 电源
 */
void V12ON_Reset(void);

/**
 * @brief  翻转 12V 电源状态
 */
void V12ON_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* _V12ON_H_ */
