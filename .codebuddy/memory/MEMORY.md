# MEMORY.md

## 用户代码风格偏好（ilencee / Embedded 工程）

- **不喜欢抽象过度的写法**：不要枚举、不要结构体表驱动（table-driven）、不要"绕弯子"的间接映射；固定少量通道/负载时要求直白展开（switch 或逐行写死）。
- **定义偏好**：负载/通道用独立大写宏；IO 口直接定义 `xxx_PORT` + `xxx_PIN` 宏，注释里写明引脚（如 `PB1`）。
- **易读性优先**：用户明确说过"不要写看不懂""直接定义 IO 口"。宁可少量代码重复，也不要引入复杂抽象。
- 头文件风格：ilencee 头注释、`@version V1.x.x`、`extern "C"` 包裹、include guard `_XXX_H_`。
- 变更后需用工程 ARMCLANG `-fsyntax-only` 验证（`D:\Software\Keil\ARM\ARMCLANG\bin\armclang.exe`，`-mcpu=cortex-m23 -DGD32E235`，include 路径见 .vscode/c_cpp_properties.json 或命令行历史）。

## 工程结构（GD32E235，Keil 工程）

- 工作区 `c:\Users\86249\Desktop\Embedded\TEST`（原 MOVA 文件夹，已重命名）。
- 源码：`TEST\User\main.c`、`TEST\User\drivers\`（fan/pump/uart/triac/zero 等驱动）。
- 器件头文件来自 DFP 包：`C:/Users/86249/AppData/Local/Arm/Packs/GigaDevice/GD32E23x_DFP/2.5.0/Device/Include`。
- 新 .c 文件需手动加入 `TEST\Project\MOVA.uvprojx` 的 USER 组才参与编译。
