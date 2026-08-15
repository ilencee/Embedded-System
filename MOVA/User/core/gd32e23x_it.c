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
#include "main.h"
#include "systick.h"
#include "zero.h"
//#include "triac.h"

/* NMI 异常处理 */
void NMI_Handler(void)
{

}

/* 硬件错误处理：进入死循环等待复位 */
void HardFault_Handler(void)
{
    while(1) {
    }
}

/* SVC 异常处理 */
void SVC_Handler(void)
{
    while(1) {
    }
}

/* PendSV 异常处理 */
void PendSV_Handler(void)
{
    while(1) {
    }
}

/* SysTick 中断：递减延时计数并累计运行时间 */
void SysTick_Handler(void)
{
    delay_decrement();
}

/* EXTI0_1 中断：处理市电过零信号 */
void EXTI0_1_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_0)){
        exti_interrupt_flag_clear(EXTI_0);
        zero_cross_isr();
    }
}
