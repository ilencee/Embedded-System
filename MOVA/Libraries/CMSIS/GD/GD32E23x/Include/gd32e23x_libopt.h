/*!
    \file    gd32e23x_libopt.h
    \brief   library optional definitions - enable all standard peripherals

    This file is a local stand-in for the missing Keil pack file
    (GigaDevice/GD32E23x_DFP/.../Device/Include/gd32e23x_libopt.h).
    It includes every standard peripheral header so that gd32e23x.h
    resolves completely for IDE indexing (clangd).
*/

#ifndef GD32E23X_LIBOPT_H
#define GD32E23X_LIBOPT_H

#include "gd32e23x_adc.h"
#include "gd32e23x_cmp.h"
#include "gd32e23x_crc.h"
#include "gd32e23x_dbg.h"
#include "gd32e23x_dma.h"
#include "gd32e23x_exti.h"
#include "gd32e23x_fmc.h"
#include "gd32e23x_fwdgt.h"
#include "gd32e23x_gpio.h"
#include "gd32e23x_i2c.h"
#include "gd32e23x_misc.h"
#include "gd32e23x_pmu.h"
#include "gd32e23x_rcu.h"
#include "gd32e23x_rtc.h"
#include "gd32e23x_spi.h"
#include "gd32e23x_syscfg.h"
#include "gd32e23x_timer.h"
#include "gd32e23x_usart.h"
#include "gd32e23x_wwdgt.h"

#endif /* GD32E23X_LIBOPT_H */
