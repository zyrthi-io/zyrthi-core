#include "zyrthi/core/helper.h"

i2c_result_t helper_i2c_transfer(i2c_hdl_t hdl, u8_t addr, 
                                 const u8_t* tx_buf, u32_t tx_len,
                                 u8_t* rx_buf, u32_t rx_len) {
    i2c_result_t ret = {0};
    
    // 1. 调用 HAL 原子接口：启动传输
    i2c_status_t begin_ret = i2c_begin(hdl, addr);
    if (begin_ret != I2C_OK) {
        ret.status = begin_ret;
        ret.value = 0;
        return ret;
    }

    // 2. 调用 HAL 原子接口：发送数据
    if (tx_buf != NULL && tx_len > 0) {
        i2c_status_t write_ret = i2c_write(hdl, tx_buf, tx_len);
        if (write_ret != I2C_OK) {
            i2c_end(hdl);
            ret.status = write_ret;
            ret.value = 0;
            return ret;
        }
    }

    // 3. 调用 HAL 原子接口：接收数据
    if (rx_buf != NULL && rx_len > 0) {
        i2c_result_t read_ret = i2c_read(hdl, rx_buf, rx_len);
        if (read_ret.status != I2C_OK) {
            i2c_end(hdl);
            ret.status = read_ret.status;
            ret.value = 0;
            return ret;
        }
        ret.value = read_ret.value;
    }

    // 4. 调用 HAL 原子接口：结束传输
    i2c_end(hdl);
    ret.status = I2C_OK;
    return ret;
}