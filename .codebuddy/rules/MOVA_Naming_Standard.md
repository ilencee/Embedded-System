# MOVA 嵌入式项目 C 语言代码规范

> **适用范围**：GD32E23x 平台，C 语言源文件（`.c` / `.h`）  
> **版本**：V2.0  
> **更新时间**：2026-08-15  

---

## 1. 宏定义（Macro）

| 项目 | 规范 | 示例 |
|------|------|------|
| **常量宏** | 全大写，单词间下划线分隔 | `#define MAX_BUFFER_SIZE 128U` |
| **带参数宏** | 全大写，形参和宏体必须用**小括号完全包裹** | `#define MIN(a, b) (((a) < (b)) ? (a) : (b))` |
| **多行语句宏** | 使用 `do { ... } while(0)` 包裹（禁止在此类宏中返回值） | 见下方示例 |
| **条件编译** | 全大写 | `#ifdef USE_FREERTOS` |

```c
#define ZERO_DETECT_PIN     GPIO_PIN_0
#define ZERO_SAMPLE_COUNT   10U

#define ABS(x)              (((x) < 0) ? -(x) : (x))
#define BIT_SET(reg, bit)   ((reg) |= (1U << (bit)))

#define SAFE_FREE(ptr)      do {                            \
                                if ((ptr) != NULL) {        \
                                    free(ptr);              \
                                    (ptr) = NULL;           \
                                }                           \
                            } while(0)
```

**禁止：**
* 使用小写或驼峰命名宏。
* 宏定义末尾误加分号（除非语义明确需要）。
* 宏嵌套超过 3 层。
* 表达式宏未对参数使用括号包裹（如 `#define MULTI(a, b) a * b` 是严格禁止的）。

---

## 2. 变量命名

| 类型 | 规范 | 示例 |
|------|------|------|
| **全局变量** | 模块前缀 + `g_` + 小写 + 下划线 | `g_zero_detect_count` |
| **静态全局/模块内变量** | 模块前缀 + `s_` + 小写 + 下划线 | `s_adc_buffer` |
| **局部变量** | 小写 + 下划线 | `timeout_cnt`, `is_ready` |
| **指针变量** | 以 `p_` 开头 | `p_buffer`, `p_adc_data` |
| **布尔/标志量** | 以 `is_` / `has_` / `flag_` 开头 | `is_initialized`, `flag_timeout` |
| **数组** | 复数形式或 `_buf` / `_arr` 后缀 | `adc_samples[]`, `tx_buf[]` |
| **结构体/枚举实例** | 小写 + 下划线 | `zero_config`, `zero_state` |
| **中断共享变量** | 必须加 `volatile` 修正，其余同上 | `static volatile uint8_t g_zero_irq_flag = 0U;` |

```c
static uint16_t s_adc_raw_value = 0U;
static volatile uint8_t g_zero_irq_flag = 0U;

void Zero_Process(void)
{
    uint16_t sample_cnt = 0U;
    uint8_t *p_data = NULL;
    bool is_valid = false;
    uint16_t adc_buf[16] = {0};
    
    for (uint8_t i = 0U; i < 16U; i++) {
        /* 单字母循环变量仅限短循环内部使用 */
    }
}
```

**禁止：**
* 使用匈牙利命名法（如 `u16Temp`）。
* 在非 `for` 循环作用域中使用单字母命名（如 `i`, `j`, `k`）。
* 使用拼音、拼音缩写或无明确含义的组合。

---

## 3. 函数命名

| 类型 | 规范 | 示例 |
|------|------|------|
| **模块对外接口** | 模块名大写首字母 + 驼峰 | `Zero_Init()`, `Zero_GetState()` |
| **模块内部静态函数** | 模块名大写首字母 + 驼峰 | `Zero_CalculatePeriod()` |
| **中断服务函数** | 严格遵循芯片厂商 SDK 命名 | `EXTI0_IRQHandler()`, `TIM2_IRQHandler()` |
| **回调函数** | 以 `_Callback` 结尾 | `Zero_Detect_Callback()` |

```c
/* 对外接口 */
void Zero_Init(void);
uint8_t Zero_GetState(uint32_t timeout_ms, Zero_State_t *p_state);
void Zero_SetCallback(Zero_Callback_Func_t cb);

/* 内部静态函数 */
static void Zero_FilterNoise(void);
static uint16_t Zero_CalculateDelta(uint16_t a, uint16_t b);
```

**禁止：**
* 使用下划线开头的函数名作为对外接口（如 `_Zero_Init()`）。
* 函数名与 C 标准库或 SDK 冲突（如 `read`, `write`, `open`）。

---

## 4. 类型定义（typedef）

| 类型 | 规范 | 示例 |
|------|------|------|
| **结构体** | 驼峰 + `_t` 后缀 | `typedef struct Zero_Config { ... } Zero_Config_t;` |
| **枚举** | 驼峰 + `_t` 后缀 | `typedef enum Zero_State { ... } Zero_State_t;` |
| **函数指针类型** | `模块名_描述_Func_t` 后缀 | `typedef void (*Zero_Callback_Func_t)(void);` |
| **布尔类型** | 包含 `<stdbool.h>` 使用标准 `bool`, `true`, `false` | `bool is_valid = true;` |
| **基本数据类型** | 必须使用 `<stdint.h>` 标准定宽类型 | `uint8_t`, `int16_t`, `uint32_t` |

```c
typedef enum Zero_State {
    ZERO_STATE_IDLE = 0U,
    ZERO_STATE_DETECTING,
    ZERO_STATE_LOCKED,
    ZERO_STATE_ERROR
} Zero_State_t;

typedef struct Zero_Config {
    uint16_t pin;
    uint32_t timeout_ms;
    Zero_Callback_Func_t callback;
} Zero_Config_t;
```

**禁止：**
* 使用 `u8`, `u16`, `u32` 或 `UINT8` 等非标准别名。
* 自定义 `BOOL`, `TRUE`, `FALSE` 宏（统一引入 `<stdbool.h>`）。

---

## 5. 枚举常量命名

枚举成员使用 **全大写 + 下划线**，前缀与枚举类型逻辑名称一致：

```c
typedef enum Pwm_Channel {
    PWM_CH_NONE = 0U,
    PWM_CH_1,
    PWM_CH_2,
    PWM_CH_MAX
} Pwm_Channel_t;
```

---

## 6. 头文件保护（Include Guard）

所有 `.h` 文件必须使用 `#ifndef` 保护，命名格式统一为 `_文件名大写_H_`（避免使用 ANSI C 标准保留的双下划线开头）：

```c
#ifndef _ZERO_H_
#define _ZERO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件内容 */

#ifdef __cplusplus
}
#endif

#endif /* _ZERO_H_ */
```

---

## 7. 头文件包含顺序

源文件 (`.c`) 包含头文件的顺序如下，不同组之间需用一空行隔开：

```c
/* 1. 文件头注释必须在最顶部 */

/* 2. 自身对应头文件（必须是包含的第一行代码，用以检测自包含性） */
#include "zero.h"

/* 3. C 标准库头文件 */
#include <stdint.h>
#include <stdbool.h>

/* 4. 第三方/操作系统头文件 */
#include "FreeRTOS.h"

/* 5. 项目公共 / BSP 头文件 */
#include "bsp_common.h"

/* 6. 模块私有头文件 */
#include "zero_cfg.h"
```

**要求：**
* `.c` 文件中除了文件头注释外，第一行生效代码必须包含其对应的 `.h` 文件。
* 系统/标准库头文件用 `<>`，项目内部头文件用 `""`。

---

## 8. 代码格式与风格

### 8.1 缩进与排版
* **缩进**：统一使用 4 个空格，**严格禁止使用 Tab**。
* **每行长度**：代码行不应超过 120 个字符。
* **括号风格**：采用 K&R 风格（函数体左大括号单独占一行；`if`/`while`/`for`/`switch` 等控制结构左大括号与控制语句同行）。
* **空格分隔**：二元运算符两侧、`if`/`while`/`for` 关键字与括号之间、逗号后必须加 1 个空格。

```c
void Zero_Init(void)
{
    if (g_zero_state == ZERO_STATE_IDLE) {
        rcu_periph_clock_enable(RCU_CFGCMP);
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    }
}
```

### 8.2 条件与循环语句
* **强制大括号**：即使分支或循环体只有一行代码，也**必须使用大括号**包覆。

```c
if (is_ready) {
    return;
}
```

---

## 9. 常量与字面量数值

| 数据类型 | U/L/F 后缀使用规范 | 示例 |
|---------|--------------------|------|
| `uint32_t` | `U` / `UL` | `1000U`, `0xFFFFFFFFUL` |
| `int64_t` | `LL` | `1000LL` |
| `uint64_t` | `ULL` | `1000ULL` |
| `float` | `F` | `3.14F` |
| `double` | 无后缀 | `3.14` |

**硬编码控制：**
* 禁止在业务代码中使用“魔数”（Unexplained Magic Numbers）。
* 所有数值常量必须通过 `#define` 或 `enum` 进行语义化声明。
* **浮点数比较**：禁止使用 `==` 或 `!=` 直接判断浮点数相等，必须判断两数之差的绝对值是否小于允许误差阈值（如 `fabsf(a - b) < 0.00001F`）。

---

## 10. 嵌入式与硬件相关规范

### 10.1 寄存器与外设库
* 优先使用 GD32 标准外设库（固件库）函数操作外设。
* 若因性能极度敏感（如高频中断）需要直接操作寄存器，必须在代码旁附加详细注释说明原因。

### 10.2 中断服务函数（ISR）
* 被中断服务函数更新、且在主流程或其他中断中读取的变量，**必须声明为 `volatile`**。
* ISR 代码应当尽量精简，避免在 ISR 内执行耗时等待、复杂浮点计算或阻塞式 I/O。

```c
static volatile uint32_t g_irq_counter = 0U;

void EXTI0_IRQHandler(void)
{
    if (exti_interrupt_flag_get(EXTI_0) != RESET) {
        g_irq_counter++;
        exti_interrupt_flag_clear(EXTI_0);
    }
}
```

---

## 11. 命名规范速查表

| 元素 | 规范示例 | 规则要点 |
|------|---------|----------|
| **宏常量** | `MAX_BUFFER_SIZE` | 全大写 + 下划线 + 类型后缀 `U` |
| **全局变量** | `g_zero_detect_count` | `g_` 前缀 + 小写 + 下划线 |
| **静态变量** | `s_adc_buffer` | `s_` 前缀 + 小写 + 下划线 |
| **局部变量** | `timeout_cnt` | 全小写 + 下划线 |
| **指针变量** | `p_data` | `p_` 前缀 |
| **布尔变量** | `is_ready`, `flag_timeout` | `is_` / `flag_` 前缀 + `<stdbool.h>` |
| **数组变量** | `adc_samples[]`, `tx_buf[]` | 复数形式或 `_buf` / `_arr` 后缀 |
| **对外函数接口** | `Zero_Init()` | 模块前缀 + 驼峰命名 |
| **结构体类型** | `Zero_Config_t` | 驼峰命名 + `_t` 后缀 |
| **枚举类型** | `Zero_State_t` | 驼峰命名 + `_t` 后缀 |
| **枚举常量** | `ZERO_STATE_IDLE` | 全大写 + 模块逻辑前缀 |
| **函数指针类型** | `Zero_Callback_Func_t` | 驼峰命名 + `_Func_t` 后缀 |

---

## 12. 规范落地与质量控制 (CI/CD)

1. **格式化配置 (`.clang-format`)**：团队提供统一的配置文件，集成至 VS Code / CLion，保存时自动完成缩进、空格与括号格式化。
2. **静态检查 (Static Analysis)**：提交代码前需通过 `clang-tidy` / `cppcheck` 检查，重点卡扣定宽数据类型的使用与未初始化变量。
