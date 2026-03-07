/**
 * @file core.c
 * @brief ZYRTHI Core 层核心实现（Arduino 风格流程封装）
 * @note 1. 实现 core.h 中声明的弱符号接口，用户可重写芯片专属逻辑；
 *       2. 严格遵循错误闭环：初始化失败时终止流程，反馈错误码；
 *       3. 兼容 HAL 层状态码，类型统一为 core_status_t（复用 sys_status_t）。
 */
#include "zyrthi/core/core.h"

// ==========================
// 弱符号：用户初始化空实现（用户可重写）
// ==========================
__attribute__((weak)) core_status_t setup(void) {
    return SYS_OK;
}

// ==========================
// 弱符号：用户主循环空实现（用户可重写）
// ==========================
__attribute__((weak)) void loop(void) {
    // 默认空循环：用户未实现时，无限空转
}

// ==========================
// 弱符号：系统初始化默认实现（用户可重写）
// ==========================
__attribute__((weak)) core_status_t zyrthi_system_init(void) {
    // 默认调用 HAL 层系统核心初始化
    return system_init();
}

// ==========================
// 核心框架入口函数（封装完整执行流程）
// ==========================
core_status_t zyrthi_core_run(void) {
    // 第一步：执行系统初始化
    core_status_t init_ret = zyrthi_system_init();
    if (init_ret != SYS_OK) {
        return init_ret;
    }

    // 第二步：执行用户自定义初始化
    core_status_t setup_ret = setup();
    if (setup_ret != SYS_OK) {
        return setup_ret;
    }

    // 第三步：进入用户主循环（无限执行）
    while (1) {
        loop();
    }

    // 理论上不会执行到这里
    return SYS_OK;
}

// ==========================
// 框架默认 main 函数（弱符号，用户可覆盖）
// ==========================
__attribute__((weak)) int main(void) {
    core_status_t core_ret = zyrthi_core_run();
    return (int)core_ret;
}
