/**
 * @file    systick.c
 * @brief   systick 定时器配置与延时实现
 *
 * @author  GigaDevice Semiconductor Inc.
 * @date    2026-08-15
 * @version V2.5.0
 */

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "systick.h"
#include "gd32e23x.h"

/* 阻塞延时计数（单位 ms，SysTick 中断中递减） */
static volatile uint32_t s_delay = 0U;

/* 系统运行时间累计（单位 ms） */
static volatile uint32_t s_tick_ms = 0U;

/**
 * @brief  配置 systick，1ms 中断一次
 */
void Systick_Config(void)
{
    /* 系统时钟频率除以每秒毫秒数，得到 1ms 定时 */
    if (SysTick_Config(SystemCoreClock / 1000U)) {
        while (1) {
        }
    }
    /* SysTick 使用最高优先级，保证延时精度 */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/**
 * @brief  阻塞延时指定毫秒数
 * @param[in] ms 延时毫秒数
 * @note   依赖 SysTick 中断递减计数，若 SysTick 优先级低于当前中断会死锁；
 *         严禁在中断服务函数中调用本函数
 */
void Systick_DelayMs(uint32_t ms)
{
    s_delay = ms;

    while (0U != s_delay) {
    }
}

/**
 * @brief  systick 中断中递减延时计数
 */
void Systick_DelayDecrement(void)
{
    if (0U != s_delay) {
        s_delay--;
    }
    s_tick_ms++;
}

/**
 * @brief  获取当前系统运行时间
 * @return 系统运行时间（单位 ms）
 */
uint32_t Systick_GetTickMs(void)
{
    return s_tick_ms;
}
