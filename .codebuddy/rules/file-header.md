# 文件头与函数注释规则

本规则适用于 MOVA 嵌入式项目（GD32E23x，C 语言）。

---

## 1. 文件头注释规则

当你新建或重写任何 `.c` / `.h` 源文件时，必须在文件顶部添加以下标准文件头注释（放在所有 `#include` 之前）。

### 1.1 必填标签
文件头注释必须包含以下五个标签：
1. `@file`：源文件名（必须与实际文件名一致，区分大小写）。
2. `@brief`：描述文件的核心功能与模块作用。
3. `@author`：作者姓名/ID（统一使用小写全拼或团队标识，如 `ilencee`）。
4. `@date`：文件的创建日期，格式统一为 `YYYY-MM-DD`。
5. `@version`：文件的起始版本号，格式为 `VX.Y.Z`（如 `V1.0.0`）。

### 1.2 格式规范
* 使用标准 Doxygen 格式 `/** ... */`。
* 标签排版要对齐，建议按 `@file`、`@brief`、`@author`、`@date`、`@version` 的顺序罗列。
* 标签之间不强制插入空行（提升 Doxygen 解析兼容性），可在属性块与长描述文本块之间留空行。

---

## 2. 示例

### 2.1 源文件 (`.c`) 示例

```c
/**
 * @file    zero.c
 * @brief   市电过零检测模块实现
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#include "zero.h"
#include <stdbool.h>
#include "bsp_gpio.h"
```

### 2.2 头文件 (`.h`) 示例

```c
/**
 * @file    zero.h
 * @brief   市电过零检测模块接口声明与类型定义
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */

#ifndef _ZERO_H_
#define _ZERO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件声明内容 */

#ifdef __cplusplus
}
#endif

#endif /* _ZERO_H_ */
```

---

## 3. 函数注释规则

### 3.1 内部普通/简短函数
对于模块内部简单的私有函数，可采用单行多行注释风格：

```c
/* 初始化过零检测所用 GPIO 与时钟 */
static void Zero_InitHardware(void)
{
    /* ... */
}
```

### 3.2 对外接口与复杂内部函数
对于模块对外 API、复杂逻辑函数或带参/有返回值的函数，必须使用 Doxygen 规范注释：

```c
/**
 * @brief  获取当前过零检测状态并更新异常计数
 * @param[in]  timeout_ms 超时等待时间（单位：毫秒）
 * @param[out] p_state    存储最新状态的变量指针
 * @return 错误码（0: 成功，1: 超时，2: 参数空指针）
 */
uint8_t Zero_GetState(uint32_t timeout_ms, Zero_State_t *p_state)
{
    /* ... */
}
```

#### 注释标签说明：
* `@brief`：简要说明函数功能。
* `@param[in]`：输入参数，说明参数含义、单位及合法取值范围。
* `@param[out]`：输出参数（指针形式），说明传出数据的意义。
* `@param[in,out]`：输入输出参数。
* `@return`：返回值含义及可能的错误码说明。
