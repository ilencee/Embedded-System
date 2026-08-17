/**
 * @file    disp.c
 * @brief   显示板通信协议解析模块实现
 *
 * 默认帧格式（可按显示板协议修改 disp.h 中的宏）：
 *   [HEAD1][HEAD2][LEN][DATA0..DATA(LEN-1)][CHK][TAIL1][TAIL2]
 *     AA     55     N   数据域 N 字节        和    0D     0A
 * 校验 CHK = (LEN + SUM(DATA)) & 0xFF，即长度字节与数据域各字节求和取低 8 位
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "disp.h"
#include "uart.h"
#include <stdio.h>

/* ===== 解析状态机 ===== */
typedef enum Disp_ParseState {
    DISP_PARSE_IDLE = 0U,    /* 等待帧头 1 */
    DISP_PARSE_HEAD2,        /* 等待帧头 2 */
    DISP_PARSE_LEN,          /* 接收长度字节 */
    DISP_PARSE_DATA,         /* 接收数据域 */
    DISP_PARSE_CHK,          /* 接收校验字节 */
    DISP_PARSE_TAIL1,        /* 等待帧尾 1 */
    DISP_PARSE_TAIL2         /* 等待帧尾 2 */
} Disp_ParseState_t;

/* ===== 解析器运行状态 ===== */
static Disp_ParseState_t s_parse_state = DISP_PARSE_IDLE;
static uint8_t  s_frame_buf[DISP_DATA_MAX_LEN];   /* 数据域缓冲 */
static uint16_t s_data_len = 0U;                  /* 期望数据长度 */
static uint16_t s_data_cnt = 0U;                  /* 已收数据字节数 */
static uint32_t s_rx_frame_cnt = 0U;              /* 成功解析帧计数 */
static uint32_t s_rx_err_cnt = 0U;                /* 解析错误计数 */

/**
 * @brief  计算校验值：长度字节 + 数据域各字节求和，取低 8 位
 * @param[in] len    数据域长度
 * @param[in] p_data 数据域指针
 * @return 校验字节
 */
static uint8_t Disp_CalcCheck(uint8_t len, const uint8_t *p_data)
{
    uint8_t sum = len;
    uint16_t i = 0U;

    for (i = 0U; i < (uint16_t)len; i++) {
        sum = (uint8_t)(sum + p_data[i]);
    }
    return sum;
}

/**
 * @brief  完整帧接收处理入口（当前打印帧摘要，后续替换为协议命令分发）
 * @param[in] p_data 数据域指针（当前仅打印长度摘要，未逐字节输出）
 * @param[in] len    数据域长度
 * @note   不逐帧回显数据域：避免串口阻塞放大 RX 溢出风险（可用 Uart_RxOverflowCount 观测）
 */
static void Disp_OnFrameReceived(const uint8_t *p_data, uint16_t len)
{
    (void)p_data;   /* 协议命令分发实现后使用数据域 */
    printf("[FRAME len=%u]\r\n", (unsigned int)len);
}

/**
 * @brief  解析失败后回到 IDLE 重新同步，并把当前字节作为可能的新帧头再匹配一次
 * @param[in] byte 当前输入字节
 */
static void Disp_Resync(uint8_t byte)
{
    s_parse_state = DISP_PARSE_IDLE;
    s_rx_err_cnt++;
    if (DISP_FRAME_HEAD1 == byte) {
        s_parse_state = DISP_PARSE_HEAD2;
    }
}

/**
 * @brief  解析器逐字节喂入（状态机核心）
 * @param[in] byte 输入字节
 */
static void Disp_ParseByte(uint8_t byte)
{
    switch (s_parse_state) {
        case DISP_PARSE_IDLE:
            if (DISP_FRAME_HEAD1 == byte) {
                s_parse_state = DISP_PARSE_HEAD2;
            }
            break;

        case DISP_PARSE_HEAD2:
            if (DISP_FRAME_HEAD2 == byte) {
                s_parse_state = DISP_PARSE_LEN;
            } else if (DISP_FRAME_HEAD1 != byte) {
                s_parse_state = DISP_PARSE_IDLE;   /* 非帧头，重新等待 */
            }
            break;

        case DISP_PARSE_LEN:
            if (byte > DISP_DATA_MAX_LEN) {
                Disp_Resync(byte);                 /* 长度超限，重新同步 */
            } else {
                s_data_len = (uint16_t)byte;
                s_data_cnt = 0U;
                s_parse_state = (0U == byte) ? DISP_PARSE_CHK : DISP_PARSE_DATA;
            }
            break;

        case DISP_PARSE_DATA:
            s_frame_buf[s_data_cnt] = byte;
            s_data_cnt++;
            if (s_data_cnt >= s_data_len) {
                s_parse_state = DISP_PARSE_CHK;
            }
            break;

        case DISP_PARSE_CHK:
            if (Disp_CalcCheck((uint8_t)s_data_len, s_frame_buf) == byte) {
                if (0U != DISP_FRAME_TAIL_EN) {
                    s_parse_state = DISP_PARSE_TAIL1;
                } else {
                    s_rx_frame_cnt++;              /* 无帧尾：校验通过即完成 */
                    Disp_OnFrameReceived(s_frame_buf, s_data_len);
                    s_parse_state = DISP_PARSE_IDLE;
                }
            } else {
                Disp_Resync(byte);                 /* 校验失败 */
            }
            break;

        case DISP_PARSE_TAIL1:
            if (DISP_FRAME_TAIL1 == byte) {
                s_parse_state = DISP_PARSE_TAIL2;
            } else {
                Disp_Resync(byte);
            }
            break;

        case DISP_PARSE_TAIL2:
            if (DISP_FRAME_TAIL2 == byte) {
                s_rx_frame_cnt++;
                Disp_OnFrameReceived(s_frame_buf, s_data_len);
                s_parse_state = DISP_PARSE_IDLE;
            } else {
                Disp_Resync(byte);   /* 失败：当前字节当新帧头重匹配，与 TAIL1 行为一致 */
            }
            break;

        default:
            s_parse_state = DISP_PARSE_IDLE;
            break;
    }
}

/**
 * @brief  初始化显示板协议解析器（复位状态机与计数）
 */
void Disp_Init(void)
{
    s_parse_state = DISP_PARSE_IDLE;
    s_data_len = 0U;
    s_data_cnt = 0U;
    s_rx_frame_cnt = 0U;
    s_rx_err_cnt = 0U;
}

/**
 * @brief  获取成功解析帧累计计数
 * @return 成功解析帧数
 */
uint32_t Disp_GetFrameCount(void)
{
    return s_rx_frame_cnt;
}

/**
 * @brief  获取解析错误累计计数（帧头/长度/校验/帧尾匹配失败）
 * @return 解析错误次数
 */
uint32_t Disp_GetErrorCount(void)
{
    return s_rx_err_cnt;
}

/**
 * @brief  主循环轮询：从串口读取数据并解析完整帧
 * @note   无数据时立即返回，不阻塞主循环
 */
void Disp_Process(void)
{
    uint8_t rx_buf[UART_RX_BUF_SIZE];
    uint16_t rx_len = 0U;
    uint16_t i = 0U;

    rx_len = Uart_RxRead(rx_buf, sizeof(rx_buf));
    for (i = 0U; i < rx_len; i++) {
        Disp_ParseByte(rx_buf[i]);
    }
}

/**
 * @brief  组帧并发送一帧数据（帧头+长度+数据+校验+帧尾）
 * @param[in] p_data 数据域指针
 * @param[in] len    数据域长度（字节，须 ≤ DISP_DATA_MAX_LEN）
 */
void Disp_SendFrame(const uint8_t *p_data, uint16_t len)
{
    uint8_t chk = 0U;
    uint16_t i = 0U;

    if ((NULL == p_data) || (len > DISP_DATA_MAX_LEN)) {
        return;
    }

    chk = Disp_CalcCheck((uint8_t)len, p_data);

    Uart_SendByte(DISP_FRAME_HEAD1);
    Uart_SendByte(DISP_FRAME_HEAD2);
    Uart_SendByte((uint8_t)len);
    for (i = 0U; i < len; i++) {
        Uart_SendByte(p_data[i]);
    }
    Uart_SendByte(chk);
    if (0U != DISP_FRAME_TAIL_EN) {
        Uart_SendByte(DISP_FRAME_TAIL1);
        Uart_SendByte(DISP_FRAME_TAIL2);
    }
}
