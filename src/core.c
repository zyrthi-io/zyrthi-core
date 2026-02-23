/**
 * @file code.c
 * @brief ZYRTHI Core 层核心实现（Arduino 风格流程封装）
 * @note 1. 实现 core.h 中声明的弱符号接口，用户可重写芯片专属逻辑；
 *       2. 严格遵循错误闭环：初始化失败时终止流程，反馈错误码；
 *       3. 兼容 HAL 层状态码，类型统一为 core_status_t（复用 sys_status_t）。
 */
#include "zyrthi/core/core.h"

// ==========================
// 弱符号：用户初始化空实现（用户可重写）
// ==========================
// 弱符号：用户初始化空实现
__attribute__((weak)) core_status_t setup(void) {
    return SYS_OK; // 默认返回成功
}

// ==========================
// 弱符号：用户主循环空实现（用户可重写）
// ==========================
/**
 * @brief loop 空实现（弱符号）
 * @note 用户未重写时，执行空循环，框架不会卡死（仅空转）
 */
__attribute__((weak)) void loop(void) {
    // 默认空循环：用户未实现时，无限空转
}

// ==========================
// 弱符号：系统初始化默认实现（用户可重写）
// ==========================
/**
 * @brief 系统初始化默认实现（弱符号）
 * @return core_status_t: 初始化状态码（SYS_OK 成功，其他为错误码）
 * @note 1. 默认实现调用 HAL 层 system_init()，适配通用场景；
 *       2. 用户可重写为芯片专属逻辑（如 STM32 SystemInit()、ESP32 esp_system_init()）；
 *       3. 初始化失败时返回对应错误码，框架会终止后续流程。
 */
__attribute__((weak)) core_status_t zyrthi_system_init(void) {
    // 默认调用 HAL 层系统核心初始化（通用实现）
    core_status_t ret = system_init();
    // 可在此扩展通用初始化逻辑（如全局中断使能）
    return ret;
}

// ==========================
// 核心框架入口函数（封装完整执行流程）
// ==========================
// 核心框架入口函数（封装完整执行流程）
core_status_t zyrthi_core_run(void) {
    // 第一步：执行系统初始化，检查是否成功
    core_status_t init_ret = zyrthi_system_init();
    if (init_ret != SYS_OK) {
        return init_ret; // 初始化失败，直接返回错误码
    }

    // 第二步：执行用户自定义初始化，新增状态检查
    core_status_t setup_ret = setup();
    if (setup_ret != SYS_OK) {
        return setup_ret; // setup 失败，终止流程
    }

    // 第三步：进入用户主循环（无限执行，永不返回）
    while (1) {
        loop();
    }

    // 理论上不会执行到这里
    return SYS_OK;
}

// ==========================
// 框架默认 main 函数（弱符号，用户可覆盖）
// ==========================
/**
 * @brief 框架默认 main 函数（弱符号）
 * @return int: 程序退出码（仅初始化失败时返回）
 * @note 1. 用户可重写 main 函数，自定义执行逻辑；
 *       2. 默认逻辑：调用 core_run()，处理初始化错误；
 *       3. 初始化成功后，进入 loop 无限循环，永不返回。
 */
__attribute__((weak)) int main(void) {
    // 调用 Core 层核心入口函数，处理初始化错误
    core_status_t core_ret = zyrthi_core_run();
    
    // 仅初始化失败时执行到这里，返回错误码
    return (int)core_ret;
}