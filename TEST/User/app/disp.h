/**
 * @file    disp.h
 * @brief   显示板通信协议解析模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef _DISP_H_
#define _DISP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ===== 协议参数（按显示板协议约定修改，默认帧格式见 disp.c 文件头） ===== */
#define DISP_FRAME_HEAD1     0xAAU    /* 帧头字节 1 */
#define DISP_FRAME_HEAD2     0x55U    /* 帧头字节 2 */
#define DISP_FRAME_TAIL1     0x0DU    /* 帧尾字节 1 */
#define DISP_FRAME_TAIL2     0x0AU    /* 帧尾字节 2 */
#define DISP_FRAME_TAIL_EN   1U       /* 帧尾使能：1=检查帧尾，0=校验通过即完成 */
#define DISP_DATA_MAX_LEN    32U      /* 数据域最大长度（字节） */

/**
 * @brief  初始化显示板协议解析器（复位状态机与计数）
 */
void Disp_Init(void);

/**
 * @brief  主循环轮询：从串口读取数据并解析完整帧
 * @note   解析出完整帧后调用内部处理（当前打印帧内容，后续替换为命令分发）
 */
void Disp_Process(void);

/**
 * @brief  组帧并发送一帧数据（帧头+长度+数据+校验+帧尾）
 * @param[in] p_data 数据域指针
 * @param[in] len    数据域长度（字节，须 ≤ DISP_DATA_MAX_LEN）
 */
void Disp_SendFrame(const uint8_t *p_data, uint16_t len);

/**
 * @brief  获取成功解析帧累计计数
 * @return 成功解析帧数
 */
uint32_t Disp_GetFrameCount(void);

/**
 * @brief  获取解析错误累计计数（帧头/长度/校验/帧尾匹配失败）
 * @return 解析错误次数
 */
uint32_t Disp_GetErrorCount(void);

#ifdef __cplusplus
}
#endif

#endif /* _DISP_H_ */
