#include "fan.h"

int fan_init(void)
{
    rcu_periph_clock_enable(fan_rcu);
    gpio_mode_set(fan_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, fan_pin);
    gpio_output_options_set(fan_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, fan_pin);
    gpio_bit_reset(fan_port, fan_pin);
    return 0;
}