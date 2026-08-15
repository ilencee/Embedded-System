#include "pump.h"

int pump_init(void)
{
    rcu_periph_clock_enable(pump_rcu);
    gpio_mode_set(pump_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pump_pin);
    gpio_output_options_set(pump_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pump_pin);
    gpio_bit_reset(pump_port, pump_pin);
    return 0;
}