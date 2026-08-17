/* 最小 stub：仅用于验证 12VON.c 语法（临时文件，验证后删除） */
#ifndef GD32E23X_H
#define GD32E23X_H

typedef unsigned int   uint32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

typedef enum { RESET = 0, SET = 1 } FlagStatus;
typedef enum { DISABLE = 0, ENABLE = 1 } ControlStatus;
typedef enum { bit_reset = 0, bit_set = 1 } bit_status;

#define NULL ((void *)0)

#define GPIOA       ((uint32_t)0x48000000U)
#define GPIO_PIN_5  ((uint32_t)0x00000020U)
#define RCU_GPIOA   ((uint32_t)0x1U)
#define GPIO_MODE_OUTPUT ((uint32_t)0x1U)
#define GPIO_MODE_INPUT  ((uint32_t)0x0U)
#define GPIO_PUPD_NONE   ((uint32_t)0x0U)
#define GPIO_OTYPE_PP    ((uint32_t)0x0U)
#define GPIO_OSPEED_2MHZ ((uint32_t)0x0U)

void rcu_periph_clock_enable(uint32_t periph);
void gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin);
void gpio_output_options_set(uint32_t gpio_periph, uint32_t otype, uint32_t speed, uint32_t pin);
void gpio_bit_set(uint32_t gpio_periph, uint32_t pin);
void gpio_bit_reset(uint32_t gpio_periph, uint32_t pin);
void gpio_bit_write(uint32_t gpio_periph, uint32_t pin, bit_status bit_value);
FlagStatus gpio_input_bit_get(uint32_t gpio_periph, uint32_t pin);
FlagStatus gpio_output_bit_get(uint32_t gpio_periph, uint32_t pin);

#endif
