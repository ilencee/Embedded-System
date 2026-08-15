# 文件头注释规则

本规则适用于 MOVA 嵌入式项目（GD32E23x，C 语言）。

## 规则

当你新建或重写任何 `.c` / `.h` 源文件时，必须在文件顶部添加以下标准文件头注释（放在所有 `#include` 之前）：

/**
 * @file    zero.c
 * @brief   市电过零检测模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */


## 示例

```c
/**
 * @file    zero.c
 * @brief   市电过零检测模块实现
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */
```

```h
/**
 * @file    zero.h
 * @brief   市电过零检测模块接口声明
 *
 * @author  ilencee
 * @date    2026-08-15
 * @version V1.0.0
 */
```





要求：
1. 文件头注释必须包含 `@file`、`@brief`、`@author`、`@date`、`@version` 四个标签。
2. `@file` 标签的值必须与源文件名一致。
3. `@brief` 描述文件的功能。
4. `@author` 描述文件的作者。
5. `@date` 描述文件的创建时间。
6. `@version` 描述文件的版本。
7. `@file` 、`@brief` 、`@author` 、`@date` 、`@version` 四个标签之间必须用空行隔开。


# 函数注释规则
函数注释规则适用于 MOVA 嵌入式项目（GD32E23x，C 语言）。

## 函数注释
普通函数优先使用简洁的单行注释：
```
/* 初始化过零检测模块 */
void Zero_Init(void)
{
    ...
}
```
对于复杂函数、对外接口或参数较多的函数，可使用 Doxygen 多行注释：
```
/**
 * @brief  获取当前过零检测状态
 *
 * @return 过零检测状态
 */
uint8_t Zero_GetState(void)
{
    ...
}
```
