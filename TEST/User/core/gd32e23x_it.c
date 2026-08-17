/**
 * @file    gd32e23x_it.c
 * @brief   中断服务函数实现
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

#include "gd32e23x_it.h"
#include "gd32e23x.h"
#include "systick.h"
#include "uart.h"
#include "zero.h"

/* ===== HardFault 现场信息（供调试器查看，定位死机原因） =====
 * 注：GD32E23x 的 core_cm23.h 为精简版，SCB_Type 未封装 CFSR/HFSR/BFAR/MMFAR，
 *     故不读取这些故障寄存器，仅保存控制寄存器与栈帧 PC/LR 用于定位 */
typedef struct {
    uint32_t control;   /* CONTROL 寄存器：bit1(SPSEL) 指示线程模式栈选择 */
    uint32_t msp;       /* 异常入口时 MSP */
    uint32_t psp;       /* 异常入口时 PSP */
    uint32_t pc;        /* 故障发生处 PC（从栈帧提取） */
    uint32_t lr;        /* 故障发生处 LR（从栈帧提取） */
} HardFault_Info_t;

/* 全局变量：HardFault 后调试器 Watch 中直接查看 g_hf_info */
HardFault_Info_t g_hf_info;

/* NMI 异常处理 */
void NMI_Handler(void)
{
}

/* 硬件错误处理：保存现场信息后关闭全局中断并进入死循环等待复位 */
void HardFault_Handler(void)
{
    uint32_t sp = 0U;

    __disable_irq();

    /* 1. 保存异常入口寄存器现场 */
    g_hf_info.control = __get_CONTROL();
    g_hf_info.msp     = __get_MSP();
    g_hf_info.psp     = __get_PSP();

    /* 2. 从故障栈帧提取 PC/LR（线程模式用 PSP，handler 模式用 MSP） */
    sp = (0U != (g_hf_info.control & 0x02U)) ? g_hf_info.psp : g_hf_info.msp;
    if ((sp >= 0x20000000U) && (sp < 0x20010000U)) {
        /* 异常自动压栈：...[R12][LR][PC][xPSR]，PC 位于 SP+24，LR 位于 SP+20 */
        g_hf_info.pc = *((volatile uint32_t *)(sp + 24U));
        g_hf_info.lr = *((volatile uint32_t *)(sp + 20U));
    } else {
        g_hf_info.pc = 0U;
        g_hf_info.lr = 0U;
    }

    while (1) {
    }
}

/* SVC 异常处理（裸机工程未使用，留空即可） */
void SVC_Handler(void)
{
}

/* PendSV 异常处理（裸机工程未使用，留空即可） */
void PendSV_Handler(void)
{
}

/* SysTick 中断：递减延时计数并累计运行时间 */
void SysTick_Handler(void)
{
    Systick_DelayDecrement();
}

/* EXTI0_1 中断：处理市电过零信号 */
void EXTI0_1_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(EXTI_0)) {
        exti_interrupt_flag_clear(EXTI_0);
        Zero_CrossIsr();
    }
}

/* USART1 中断：串口接收数据 */
void USART1_IRQHandler(void)
{
    Uart_IrqHandler();
}
