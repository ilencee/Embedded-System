/**
 * @file    main.c
 * @brief   主程序入口
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V2.0.0
 */

#include "gd32e23x.h"
#include "systick.h"
#include "fan.h"
#include "pump.h"
#include "zero.h"
//#include "triac.h"
1
int main(void)
{
    /* 时钟与外设初始化（系统时钟已在 SystemInit() 配置为 72MHz，勿调用 rcu_deinit() 复位） */
    systick_config();
    fan_init();
    pump_init();
    zero_init();   /* 启用市电过零检测（PB0 + EXTI0 双边沿中断） */

    /* 上电后直接开启风扇与水泵 */
    fan_switch_on();
    pump_switch_on();

    /* 主循环：当前为空转，预留后续业务逻辑 */
    while(1){
        delay_1ms(1);
       // __WFI();  /* 进入低功耗模式，等待中断唤醒 */
    }
}
