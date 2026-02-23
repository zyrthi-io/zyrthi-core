# zyrthi-core

Zyrthi 核心运行框架 —— 基于 zyrthi-hal 的跨芯片嵌入式主循环框架。

## 核心特点

1. **双入口兼容**：支持 Arduino 风格（setup/loop）和传统 C 风格（自定义 main）；
2. **初始化规范**：遵循嵌入式最佳实践，初始化顺序清晰可控；
3. **弱符号扩展**：核心函数均可重写，无需修改框架代码即可适配任意芯片；
4. **极简编译**：Makefile 支持主机模拟/交叉编译，一键编译/清理/运行。

## 初始化顺序规范

zyrthi-core 遵循嵌入式开发最佳实践，初始化顺序为：

1. 编译器自动完成「系统级初始化」（栈/全局变量/段映射）；
2. main() 执行 → 调用 zyrthi_system_init()（外设级初始化）；
3. 调用 setup()（用户自定义初始化）；
4. 进入 loop() 无限循环。

用户可通过重写 zyrthi_system_init() 自定义硬件初始化逻辑，或通过自定义 main() 完全掌控初始化顺序。

## 快速开始

### 1. 依赖

- zyrthi-hal：需将 zyrthi-hal 的 include 目录加入编译路径（Makefile 已默认配置）

### 2. 示例：LED 闪烁

```c
// examples/led_blink.c
#include "zyrthi_core.h"

void setup(void) {
    // 初始化 LED 引脚
    zyrthi_gpio_mode(13, ZYRTHI_OUTPUT);
}

void loop(void) {
    // 翻转 LED
    zyrthi_gpio_write(13, ZYRTHI_HIGH);
    zyrthi_delay_ms(500);
    zyrthi_gpio_write(13, ZYRTHI_LOW);
    zyrthi_delay_ms(500);
}
