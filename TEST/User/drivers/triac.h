/**
 * @file    triac.h
 * @brief   可控硅（Triac）控制模块接口声明
 *
 *          四路可控硅分别控制四个交流负载：
 *          HEAT（加热）、PTC（PTC 加热器）、UVA（UVA 紫外灯）、UVB（UVB 紫外灯）
 *
 * @author  ilencee
 * @date    2026-08-17
 * @version V1.5.0
 *
 * @note    光耦驱动，低电平有效（导通=输出低电平，关断=输出高电平）
 */

#ifndef _TRIAC_H_
#define _TRIAC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"

/* ============ 负载 IO 口定义（直接指定端口和引脚） ============ */

/* HEAT 加热负载 -> PB1 */
#define TRIAC_HEAT_PORT     GPIOB
#define TRIAC_HEAT_PIN      GPIO_PIN_1

/* PTC 加热负载  -> PB2 */
#define TRIAC_PTC_PORT      GPIOB
#define TRIAC_PTC_PIN       GPIO_PIN_2

/* UVA 紫外灯    -> PB10 */
#define TRIAC_UVA_PORT      GPIOB
#define TRIAC_UVA_PIN       GPIO_PIN_10

/* UVB 紫外灯    -> PB11 */
#define TRIAC_UVB_PORT      GPIOB
#define TRIAC_UVB_PIN       GPIO_PIN_11

/* 四路负载全部在 GPIOB，时钟使能用 RCU_GPIOB */
#define TRIAC_RCU           RCU_GPIOB

/* 负载通道号定义 */
#define TRIAC_HEAT          0U
#define TRIAC_PTC           1U
#define TRIAC_UVA           2U
#define TRIAC_UVB           3U

/**
 * @brief  初始化四个可控硅控制引脚（GPIO 推挽输出，默认关断）
 */
void Triac_Init(void);

/**
 * @brief  导通指定负载通道（低电平有效：输出低电平触发光耦）
 * @param  ch 负载通道号（TRIAC_HEAT/TRIAC_PTC/TRIAC_UVA/TRIAC_UVB），无效值将被忽略
 */
void Triac_SwitchOn(uint8_t ch);

/**
 * @brief  关断指定负载通道（低电平有效：输出高电平截止光耦）
 * @param  ch 负载通道号（TRIAC_HEAT/TRIAC_PTC/TRIAC_UVA/TRIAC_UVB），无效值将被忽略
 */
void Triac_SwitchOff(uint8_t ch);

/**
 * @brief  翻转指定负载通道输出状态
 * @param  ch 负载通道号（TRIAC_HEAT/TRIAC_PTC/TRIAC_UVA/TRIAC_UVB），无效值将被忽略
 */
void Triac_Toggle(uint8_t ch);

/**
 * @brief  导通全部负载通道
 */
void Triac_SwitchOnAll(void);

/**
 * @brief  关断全部负载通道
 */
void Triac_SwitchOffAll(void);

/* 各负载便捷开关函数（直接操作对应 IO 口，低电平有效） */
void Triac_HeatOn(void);
void Triac_HeatOff(void);
void Triac_PtcOn(void);
void Triac_PtcOff(void);
void Triac_UvaOn(void);
void Triac_UvaOff(void);
void Triac_UvbOn(void);
void Triac_UvbOff(void);

#ifdef __cplusplus
}
#endif

#endif /* _TRIAC_H_ */
