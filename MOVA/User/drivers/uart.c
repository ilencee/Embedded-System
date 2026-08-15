/**
 * @file    uart.c
 * @brief   串口通信实现（USART1，与显示板通信）
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "uart.h"
#include "gd32e23x.h"
#include <stdio.h>   /* printf 重定向所需 */

/* ===== 硬件配置：USART1 @ PA2(TX)/PA3(RX)，AF1 ===== */
#define UART_GPIO_PORT GPIOA
#define UART_TX_PIN    GPIO_PIN_2
#define UART_RX_PIN    GPIO_PIN_3
#define UART_GPIO_AF   GPIO_AF_1

/* ===== 接收环形缓冲：中断写入，主循环读取 ===== */
static volatile uint8_t  s_rx_buf[UART_RX_BUF_SIZE];
static volatile uint16_t s_rx_head = 0U;   /* 写指针，在接收中断中更新 */
static volatile uint16_t s_rx_tail = 0U;   /* 读指针，在主循环中更新 */
static volatile uint32_t s_rx_overflow_cnt = 0U;   /* 溢出丢包计数 */

/**
 * @brief  初始化串口（USART1，PA2=TX/PA3=RX，115200-8-N-1）
 */
void Uart_Init(void)
{
    /* 1. 使能 GPIOA 与 USART1 外设时钟（先 GPIO 后外设） */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);

    /* 2. PA2/PA3 复用为 USART1：TX 复用推挽输出，RX 复用上拉输入 */
    gpio_af_set(UART_GPIO_PORT, UART_GPIO_AF, UART_TX_PIN | UART_RX_PIN);
    gpio_mode_set(UART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, UART_TX_PIN | UART_RX_PIN);
    gpio_output_options_set(UART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART_TX_PIN);

    /* 3. 串口参数：115200-8-N-1，收发均使能 */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, UART_BAUDRATE);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);

    /* 4. 使能接收非空中断（RBNE），数据到达自动写入环形缓冲 */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    nvic_irq_enable(USART1_IRQn, UART_IRQ_PRIO);

    /* 5. 使能 USART1 */
    usart_enable(USART1);
}

/**
 * @brief  阻塞发送单字节
 * @param[in] data 待发送字节
 * @note   只等待 TBE（数据寄存器空）即写入，由硬件流水完成移位，吞吐更高；
 *         需要确保数据完全移出时调用 Uart_SendFlush()（如批发送末尾）
 */
void Uart_SendByte(uint8_t data)
{
    /* 等待发送数据寄存器空，再写入新数据 */
    while (RESET == usart_flag_get(USART1, USART_FLAG_TBE)) {
    }
    usart_data_transmit(USART1, data);
}

/**
 * @brief  等待发送完成：确保最后一字节已完整移出移位寄存器
 * @note   用于批发送末尾，保证整包数据发出后再返回；
 *         只发送单字节且后续无依赖时可省略（不阻塞等待 TC 吞吐更高）
 */
void Uart_SendFlush(void)
{
    while (RESET == usart_flag_get(USART1, USART_FLAG_TC)) {
    }
}

/**
 * @brief  阻塞发送数据缓冲区
 * @param[in] data 数据指针
 * @param[in] len  数据长度（字节）
 */
void Uart_SendData(const uint8_t *data, uint16_t len)
{
    uint16_t i = 0U;

    for (i = 0U; i < len; i++) {
        Uart_SendByte(data[i]);
    }
    Uart_SendFlush();   /* 整包发出后等待最后一字节移出 */
}

/**
 * @brief  阻塞发送以 '\0' 结尾的字符串
 * @param[in] str 字符串指针
 */
void Uart_SendString(const char *str)
{
    while ('\0' != *str) {
        Uart_SendByte((uint8_t)(*str));
        str++;
    }
    Uart_SendFlush();   /* 整串发出后等待最后一字节移出 */
}

/**
 * @brief  获取接收缓冲区中可读取的字节数
 * @return 可读字节数
 */
uint16_t Uart_RxAvailable(void)
{
    uint16_t head = 0U;

    /* 先快照写指针，避免与接收中断并发读写时读到不一致值 */
    head = s_rx_head;
    return (uint16_t)((head + UART_RX_BUF_SIZE - s_rx_tail) % UART_RX_BUF_SIZE);
}

/**
 * @brief  从接收缓冲区读取数据（读取后数据即从缓冲区移除）
 * @param[out] buf 存储读取数据的缓冲区
 * @param[in]  len 期望读取的最大字节数
 * @return 实际读取的字节数
 */
uint16_t Uart_RxRead(uint8_t *buf, uint16_t len)
{
    uint16_t cnt = 0U;

    while ((cnt < len) && (s_rx_tail != s_rx_head)) {
        buf[cnt] = s_rx_buf[s_rx_tail];
        s_rx_tail = (uint16_t)((s_rx_tail + 1U) % UART_RX_BUF_SIZE);
        cnt++;
    }
    return cnt;
}

/**
 * @brief  获取接收溢出丢包累计次数（缓冲满导致的数据丢弃）
 * @return 溢出丢包计数
 */
uint32_t Uart_RxOverflowCount(void)
{
    return s_rx_overflow_cnt;
}

/**
 * @brief  串口接收中断服务（由 USART1_IRQHandler 调用）
 */
void Uart_IrqHandler(void)
{
    uint8_t data = 0U;
    uint16_t next = 0U;

    if (SET == usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) {
        data = (uint8_t)usart_data_receive(USART1);
        next = (uint16_t)((s_rx_head + 1U) % UART_RX_BUF_SIZE);

        /* 缓冲未满才写入，满则丢弃新数据，避免覆盖未读数据 */
        if (next != s_rx_tail) {
            s_rx_buf[s_rx_head] = data;
            s_rx_head = next;
        } else {
            s_rx_overflow_cnt++;   /* 记录溢出丢包，便于排查链路问题 */
        }
    }
}

/**
 * @brief  printf 重定向：标准库每输出一个字符调用一次
 * @param[in] ch 待输出字符
 * @param[in] f  输出流（固定为 stdout，此处不使用）
 * @return 输出成功的字符
 * @note  需在 Keil Target 页勾选 "Use MicroLIB" 方可生效；
 *        阻塞发送，禁止在中断服务函数中调用 printf；
 *        只等 TBE 不等 TC（吞吐更高），若需确认最后字符已移出请调用 Uart_SendFlush()
 */
int fputc(int ch, FILE *f)
{
    (void)f;                       /* 消除未使用参数告警 */
    Uart_SendByte((uint8_t)ch);    /* 字符交给串口发出 */
    return ch;
}

/**
 * @brief  串口回显测试：收到数据原样发回，并打印日志
 * @note   仅用于链路验证，后续实现正式协议后应移除；
 *         无数据时立即返回，不阻塞
 */
void Uart_EchoTest(void)
{
    uint8_t rx_buf[UART_RX_BUF_SIZE];
    uint16_t rx_len = 0U;

    rx_len = Uart_RxRead(rx_buf, sizeof(rx_buf));
    if (0U == rx_len) {
        return;                    /* 无数据，直接返回 */
    }

    Uart_SendData(rx_buf, rx_len);                /* 回显：原样发回 */

    printf("[RX %uB] ", (unsigned int)rx_len);    /* 日志：打印长度 */
    Uart_SendData(rx_buf, rx_len);                /* 日志：打印内容 */
    printf("\r\n");
}
