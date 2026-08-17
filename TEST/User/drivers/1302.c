/**
 * @file    1302.c
 * @brief   DS1302 实时时钟芯片驱动实现（三线 GPIO 模拟时序）
 *
 * 通信协议：CE/SCLK/IO 三线，命令与数据均 LSB 先行；
 * 写：SCLK 上升沿锁存；读：SCLK 下降沿后 IO 输出数据。
 * 芯片内部以 BCD 存储，本驱动对外提供十进制时间结构体。
 *
 * @author  ilencee
 * @date    2026-08-16
 * @version V1.0.0
 */

#include <stddef.h>   /* NULL */
#include "1302.h"

/* ===== 内部辅助：CE/SCLK 电平与 IO 方向控制 ===== */
#define DS1302_CE_HIGH()      gpio_bit_set(DS1302_CE_PORT, DS1302_CE_PIN)
#define DS1302_CE_LOW()       gpio_bit_reset(DS1302_CE_PORT, DS1302_CE_PIN)
#define DS1302_SCLK_HIGH()    gpio_bit_set(DS1302_SCLK_PORT, DS1302_SCLK_PIN)
#define DS1302_SCLK_LOW()     gpio_bit_reset(DS1302_SCLK_PORT, DS1302_SCLK_PIN)
#define DS1302_IO_HIGH()      gpio_bit_set(DS1302_IO_PORT, DS1302_IO_PIN)
#define DS1302_IO_LOW()       gpio_bit_reset(DS1302_IO_PORT, DS1302_IO_PIN)

/**
 * @brief  微秒级延时（软件空循环）
 * @note   按 72MHz 主频估算：1us ≈ 72 个 NOP；
 *         若实际主频不同或时序不稳，请按实测调整系数
 */
static void ds1302_delay_us(uint32_t us)
{
    volatile uint32_t i;
    for (i = 0U; i < (us * 72U); i++) {
        __NOP();
    }
}

/**
 * @brief  将 IO 线配置为推挽输出模式
 */
static void ds1302_io_output(void)
{
    gpio_mode_set(DS1302_IO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_IO_PIN);
    gpio_output_options_set(DS1302_IO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, DS1302_IO_PIN);
}

/**
 * @brief  将 IO 线配置为输入模式（浮空，由 DS1302 驱动）
 */
static void ds1302_io_input(void)
{
    gpio_mode_set(DS1302_IO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DS1302_IO_PIN);
}

/**
 * @brief  写一个字节（LSB 先行，SCLK 上升沿锁存）
 */
static void ds1302_write_byte(uint8_t byte)
{
    uint8_t i;
    for (i = 0U; i < 8U; i++) {
        if ((byte & (1U << i)) != 0U) {
            DS1302_IO_HIGH();
        } else {
            DS1302_IO_LOW();
        }
        ds1302_delay_us(1U);
        DS1302_SCLK_HIGH();          /* 上升沿：DS1302 锁存数据 */
        ds1302_delay_us(1U);
        DS1302_SCLK_LOW();
        ds1302_delay_us(1U);
    }
}

/**
 * @brief  读一个字节（LSB 先行，SCLK 下降沿后采样 IO）
 */
static uint8_t ds1302_read_byte(void)
{
    uint8_t i;
    uint8_t byte = 0U;
    for (i = 0U; i < 8U; i++) {
        DS1302_SCLK_HIGH();
        ds1302_delay_us(1U);
        DS1302_SCLK_LOW();
        ds1302_delay_us(1U);
        byte |= (uint8_t)(gpio_input_bit_get(DS1302_IO_PORT, DS1302_IO_PIN) << i);
    }
    return byte;
}

/**
 * @brief  初始化 DS1302 三线引脚
 */
void Rtc1302_Init(void)
{
    rcu_periph_clock_enable(DS1302_RCU);

    /* CE 与 SCLK：推挽输出，初始为低 */
    gpio_mode_set(DS1302_CE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_CE_PIN);
    gpio_output_options_set(DS1302_CE_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, DS1302_CE_PIN);
    gpio_mode_set(DS1302_SCLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_SCLK_PIN);
    gpio_output_options_set(DS1302_SCLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, DS1302_SCLK_PIN);
    /* IO：双向线，先配为输出 */
    ds1302_io_output();

    DS1302_CE_LOW();
    DS1302_SCLK_LOW();
    DS1302_IO_LOW();
}

/**
 * @brief  写 DS1302 寄存器
 * @param[in] reg  寄存器写地址（如 DS1302_REG_SEC）
 * @param[in] data 待写数据（BCD 或控制值）
 */
void Rtc1302_WriteReg(uint8_t reg, uint8_t data)
{
    ds1302_io_output();
    DS1302_SCLK_LOW();
    DS1302_CE_LOW();
    ds1302_delay_us(1U);

    DS1302_CE_HIGH();                /* CE 拉高启动传输 */
    ds1302_delay_us(1U);
    ds1302_write_byte(reg);          /* 命令字节（bit0=0 表示写） */
    ds1302_write_byte(data);         /* 数据字节 */
    DS1302_CE_LOW();                 /* CE 拉低结束传输 */
    ds1302_delay_us(1U);
}

/**
 * @brief  读 DS1302 寄存器
 * @param[in] reg 寄存器写地址（如 DS1302_REG_SEC）
 * @return 寄存器值（BCD 或控制值）
 */
uint8_t Rtc1302_ReadReg(uint8_t reg)
{
    uint8_t data;
    ds1302_io_output();
    DS1302_SCLK_LOW();
    DS1302_CE_LOW();
    ds1302_delay_us(1U);

    DS1302_CE_HIGH();                /* CE 拉高启动传输 */
    ds1302_delay_us(1U);
    ds1302_write_byte(reg | 0x01U);  /* 命令字节（bit0=1 表示读） */
    ds1302_io_input();               /* IO 切输入，等待 DS1302 驱动 */
    ds1302_delay_us(1U);
    data = ds1302_read_byte();
    DS1302_CE_LOW();                 /* CE 拉低结束传输 */
    ds1302_delay_us(1U);
    ds1302_io_output();              /* 恢复输出态，避免悬空 */
    return data;
}

/* BCD 转十进制 */
static uint8_t ds1302_bcd2dec(uint8_t bcd)
{
    return (uint8_t)(((bcd >> 4U) * 10U) + (bcd & 0x0FU));
}

/* 十进制转 BCD */
static uint8_t ds1302_dec2bcd(uint8_t dec)
{
    return (uint8_t)(((dec / 10U) << 4U) | (dec % 10U));
}

/**
 * @brief  设置 DS1302 写保护
 * @param[in] en 1=写保护使能，0=关闭
 */
void Rtc1302_WriteProtect(uint8_t en)
{
    Rtc1302_WriteReg(DS1302_REG_WP, (en != 0U) ? 0x80U : 0x00U);
}

/**
 * @brief  设置时间（内部临时关闭写保护，写完恢复）
 * @param[in] t 时间结构体指针（十进制），须非空
 */
void Rtc1302_SetTime(const rtc1302_time_t *t)
{
    if (t == NULL) {
        return;
    }
    Rtc1302_WriteProtect(0U);                         /* 关写保护 */
    Rtc1302_WriteReg(DS1302_REG_SEC,   (uint8_t)(ds1302_dec2bcd(t->sec) & 0x7FU));  /* 清 CH 位，启动走时 */
    Rtc1302_WriteReg(DS1302_REG_MIN,   ds1302_dec2bcd(t->min));
    Rtc1302_WriteReg(DS1302_REG_HOUR,  (uint8_t)(ds1302_dec2bcd(t->hour) & 0x3FU)); /* bit7=0 固定 24 小时制 */
    Rtc1302_WriteReg(DS1302_REG_DAY,   ds1302_dec2bcd(t->day));
    Rtc1302_WriteReg(DS1302_REG_MONTH, ds1302_dec2bcd(t->month));
    Rtc1302_WriteReg(DS1302_REG_WEEK,  ds1302_dec2bcd(t->week));
    Rtc1302_WriteReg(DS1302_REG_YEAR,  ds1302_dec2bcd(t->year));
    Rtc1302_WriteProtect(1U);                         /* 恢复写保护 */
}

/**
 * @brief  读取时间
 * @param[out] t 时间结构体指针（十进制），须非空
 */
void Rtc1302_GetTime(rtc1302_time_t *t)
{
    if (t == NULL) {
        return;
    }
    t->sec   = ds1302_bcd2dec((uint8_t)(Rtc1302_ReadReg(DS1302_REG_SEC) & 0x7FU));
    t->min   = ds1302_bcd2dec(Rtc1302_ReadReg(DS1302_REG_MIN));
    t->hour  = ds1302_bcd2dec((uint8_t)(Rtc1302_ReadReg(DS1302_REG_HOUR) & 0x3FU)); /* 屏蔽 12/24 与 AP 位 */
    t->day   = ds1302_bcd2dec(Rtc1302_ReadReg(DS1302_REG_DAY));
    t->month = ds1302_bcd2dec(Rtc1302_ReadReg(DS1302_REG_MONTH));
    t->week  = ds1302_bcd2dec(Rtc1302_ReadReg(DS1302_REG_WEEK));
    t->year  = ds1302_bcd2dec(Rtc1302_ReadReg(DS1302_REG_YEAR));
}