#ifndef ZYRTHI_CORE_I2C_HELPER_H
#define ZYRTHI_CORE_I2C_HELPER_H

#include "zyrthi/hal/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief I2C 一站式读写传输（Core 层辅助封装）
 * @param hdl: I2C 句柄
 * @param addr: 设备地址（7位）
 * @param tx_buf: 发送缓冲区（NULL 则仅读）
 * @param tx_len: 发送长度（0 则仅读）
 * @param rx_buf: 接收缓冲区（NULL 则仅写）
 * @param rx_len: 接收长度（0 则仅写）
 * @return i2c_result_t: 操作结果（状态+实际接收长度）
 * @note 内部复用 HAL 层 i2c_begin/write/read/end 原子接口
 */
i2c_result_t helper_i2c_transfer(i2c_hdl_t hdl, u8_t addr, 
                                 const u8_t* tx_buf, u32_t tx_len,
                                 u8_t* rx_buf, u32_t rx_len);

#ifdef __cplusplus
}
#endif

#endif // ZYRTHI_CORE_HELPER_H