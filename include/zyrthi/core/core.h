/**
 * @file zyrthi_core_core.h
 * @brief ZYRTHI Core 层核心接口（Arduino 风格封装）
 * @note 1. 封装「系统初始化+用户setup+用户loop」的核心执行流程；
 *       2. 弱符号接口支持用户重写，兼顾通用性和芯片专属定制；
 *       3. 完全兼容 HAL 层规范，错误闭环、类型统一。
 */
#ifndef ZYRTHI_CORE_CORE_H
#define ZYRTHI_CORE_CORE_H

// ==========================
// 规范1：统一使用 HAL 层类型定义（移除重复的 stdint/stdbool）
// ==========================
#include "zyrthi/hal/zyrthi_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

// ==========================
// Core 层专属状态码（复用 HAL 系统状态码，避免冗余）
// ==========================
// 核心层初始化状态复用 sys_status_t，无需新增冗余枚举
typedef sys_status_t core_status_t;

// ==========================
// Core 层核心接口（Arduino 风格+错误闭环+弱符号）
// ==========================
/**
 * @brief 系统初始化函数（外设级，HAL 层封装）
 * @return core_status_t: 初始化状态码（SYS_OK 成功，其他为错误码）
 * @note 1. 弱符号定义（__attribute__((weak))），用户可重写为芯片专属逻辑；
 *       2. 默认实现调用 HAL 层 system_init()，适配通用场景；
 *       3. 执行时机：main() 开头，setup() 之前，仅执行一次。
 */
__attribute__((weak)) core_status_t zyrthi_system_init(void);

/**
 * @brief 用户初始化函数（Arduino 风格）
 * @note 1. 弱符号定义，用户必须重写（默认空实现）；
 *       2. 执行时机：zyrthi_system_init() 成功后，loop() 执行前，仅执行一次；
 *       3. 用于用户自定义初始化（如 GPIO/UART/I2C 配置）。
 */
__attribute__((weak)) void setup(void);

/**
 * @brief 用户主循环函数（Arduino 风格）
 * @note 1. 弱符号定义，用户必须重写（默认空实现）；
 *       2. 执行时机：setup() 完成后，无限循环执行；
 *       3. 用于用户核心业务逻辑（如传感器读取、外设控制）。
 */
__attribute__((weak)) void loop(void);

/**
 * @brief 核心框架入口函数
 * @return core_status_t: 执行状态码（初始化失败返回对应错误码，循环中永不返回）
 * @note 1. 封装完整执行流程：zyrthi_system_init() → setup() → loop() 无限循环；
 *       2. 供用户自定义 main() 调用，示例：
 *          int main(void) {
 *              core_status_t ret = zyrthi_core_run();
 *              if (ret != SYS_OK) return ret; // 仅初始化失败时返回
 *              while(1); // 理论上不会执行到这里
 *          }
 *       3. 若 zyrthi_system_init() 失败，直接返回错误码，不执行 setup/loop。
 */
core_status_t zyrthi_core_run(void);

#ifdef __cplusplus
}
#endif

#endif // ZYRTHI_CORE_CORE_H