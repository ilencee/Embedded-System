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
#include "main.h"
#include "fan.h"
#include "pump.h"
#include "zero.h"
//#include "triac.h"

int main(void)
{
    /* 时钟与外设初始化 */
    rcu_deinit();
    systick_config();
    fan_init();
    pump_init();
    //zero_init();   /* 过零检测暂未启用，如需市电过零功能请取消注释 */

    /* 上电后直接开启风扇与水泵 */
    fan_switch_on();
    pump_switch_on();

    /* 主循环：当前为空转，预留后续业务逻辑 */
    while(1){
        delay_1ms(1);
    }
}
