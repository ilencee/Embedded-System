/**
 * @file    1302.h
 * @brief   DS1302 实时时钟芯片驱动接口声明
 *
 * @author  ilencee
 * @date    2026-08-16
 * @version V1.0.0
 */

#ifndef _1302_H_
#define _1302_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gd32e23x.h"

/* ===== 引脚配置（按实际硬件接线修改） =====
 * 默认 PA4=CE / PA5=SCLK / PA6=IO，
 * 若与硬件不符，仅需修改以下宏，无需改动实现代码。
 */
#define DS1302_CE_PORT      GPIOA
#define DS1302_CE_PIN       GPIO_PIN_4
#define DS1302_SCLK_PORT    GPIOA
#define DS1302_SCLK_PIN     GPIO_PIN_5
#define DS1302_IO_PORT      GPIOA
#define DS1302_IO_PIN       GPIO_PIN_6
#define DS1302_RCU          RCU_GPIOA

/* DS1302 寄存器地址（写地址；读时内部自动置读位 bit0=1） */
#define DS1302_REG_SEC      0x80U    /* 秒（bit7=CH 时钟暂停标志） */
#define DS1302_REG_MIN      0x82U    /* 分 */
#define DS1302_REG_HOUR     0x84U    /* 时（bit7=12/24 小时制标志） */
#define DS1302_REG_DAY      0x86U    /* 日 1-31 */
#define DS1302_REG_MONTH    0x88U    /* 月 1-12 */
#define DS1302_REG_WEEK     0x8AU    /* 星期 1-7 */
#define DS1302_REG_YEAR     0x8CU    /* 年 00-99（2000 年偏移） */
#define DS1302_REG_WP       0x8EU    /* 写保护（bit7=1 使能写保护） */

/* 时间结构体（十进制值） */
typedef struct {
    uint8_t sec;     /* 秒 0-59 */
    uint8_t min;     /* 分 0-59 */
    uint8_t hour;    /* 时 0-23（24 小时制） */
    uint8_t day;     /* 日 1-31 */
    uint8_t month;   /* 月 1-12 */
    uint8_t week;    /* 星期 1-7 */
    uint8_t year;    /* 年 0-99（2000 年偏移） */
} rtc1302_time_t;

/**
 * @brief  初始化 DS1302 三线引脚（CE/SCLK/IO）
 */
void Rtc1302_Init(void);

/**
 * @brief  写 DS1302 寄存器
 * @param[in] reg  寄存器写地址（如 DS1302_REG_SEC）
 * @param[in] data 待写数据（BCD 或控制值）
 */
void Rtc1302_WriteReg(uint8_t reg, uint8_t data);

/**
 * @brief  读 DS1302 寄存器
 * @param[in] reg 寄存器写地址（如 DS1302_REG_SEC）
 * @return 寄存器值（BCD 或控制值）
 */
uint8_t Rtc1302_ReadReg(uint8_t reg);

/**
 * @brief  设置 DS1302 写保护
 * @param[in] en 1=写保护使能，0=关闭
 */
void Rtc1302_WriteProtect(uint8_t en);

/**
 * @brief  设置时间（内部临时关闭写保护，写完恢复）
 * @param[in] t 时间结构体指针（十进制），须非空
 */
void Rtc1302_SetTime(const rtc1302_time_t *t);

/**
 * @brief  读取时间
 * @param[out] t 时间结构体指针（十进制），须非空
 */
void Rtc1302_GetTime(rtc1302_time_t *t);

#ifdef __cplusplus
}
#endif

#endif /* _1302_H_ */
