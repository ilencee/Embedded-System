/**
 * @file    uart.h
 * @brief   串口通信接口声明（与显示板通信）
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 串口通信参数（与显示板约定，可按需修改） */
#define UART_BAUDRATE    115200U    /* 波特率 */
#define UART_RX_BUF_SIZE 64U        /* 接收环形缓冲区大小（字节） */
#define UART_IRQ_PRIO    2U         /* 串口中断优先级（低于 SysTick/EXTI0_1） */

/**
 * @brief  初始化串口（USART1，PA2=TX/PA3=RX，115200-8-N-1）
 */
void Uart_Init(void);

/**
 * @brief  阻塞发送单字节
 * @param[in] data 待发送字节
 */
void Uart_SendByte(uint8_t data);

/**
 * @brief  阻塞发送数据缓冲区
 * @param[in] data 数据指针
 * @param[in] len  数据长度（字节）
 */
void Uart_SendData(const uint8_t *data, uint16_t len);

/**
 * @brief  阻塞发送以 '\0' 结尾的字符串
 * @param[in] str 字符串指针
 */
void Uart_SendString(const char *str);

/**
 * @brief  等待发送完成：确保最后一字节已完整移出移位寄存器
 * @note   用于批发送末尾，保证整包数据发出后再返回
 */
void Uart_SendFlush(void);

/**
 * @brief  获取接收缓冲区中可读取的字节数
 * @return 可读字节数
 */
uint16_t Uart_RxAvailable(void);

/**
 * @brief  从接收缓冲区读取数据（读取后数据即从缓冲区移除）
 * @param[out] buf 存储读取数据的缓冲区
 * @param[in]  len 期望读取的最大字节数
 * @return 实际读取的字节数
 */
uint16_t Uart_RxRead(uint8_t *buf, uint16_t len);

/**
 * @brief  获取接收溢出丢包累计次数（缓冲满导致的数据丢弃）
 * @return 溢出丢包计数
 */
uint32_t Uart_RxOverflowCount(void);

/**
 * @brief  串口接收中断服务（由 USART1_IRQHandler 调用）
 */
void Uart_IrqHandler(void);

/**
 * @brief  串口回显测试：收到数据原样发回，并打印日志
 * @note   仅用于链路验证，后续实现正式协议后应移除；
 *         无数据时立即返回，不阻塞
 */
void Uart_EchoTest(void);

#ifdef __cplusplus
}
#endif

#endif /* _UART_H_ */
