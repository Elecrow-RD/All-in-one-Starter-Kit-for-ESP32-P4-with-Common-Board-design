// bsp_i2c.cpp
#include "bsp_i2c.h"
#include "esp_log.h"

static const char *TAG = "BSP_I2C";

esp_err_t i2c_init(void)
{
    esp_err_t err = ESP_OK;

    // 1. 声明配置结构体，然后逐字段赋值（避免 designated initializers 语法问题）
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_GPIO_SDA;          // 请确保这两个宏已正确定义
    conf.scl_io_num = I2C_GPIO_SCL;
    // 上拉配置（根据硬件选择）
#ifdef CONFIG_I2C_GPIO_PULLUP
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
#else
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
#endif
    conf.master.clk_speed = 400000;           // 时钟频率 400kHz
    conf.clk_flags = 0;  

    // 2. 设置参数
    err = i2c_param_config(I2C_MASTER_PORT, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c_param_config failed: %s", esp_err_to_name(err));
        return err;
    }

    // 3. 安装驱动（主机模式下，后三个参数填 0）
    err = i2c_driver_install(I2C_MASTER_PORT, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c_driver_install failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "I2C initialized successfully");
    return ESP_OK;
}

// 读操作：直接从设备读取数据
esp_err_t i2c_read(uint8_t dev_addr, uint8_t *read_buffer, size_t read_size)
{
    return i2c_master_read_from_device(I2C_MASTER_PORT, dev_addr,
                                        read_buffer, read_size, 1000 / portTICK_PERIOD_MS);
}

// 写操作：直接向设备写入数据
esp_err_t i2c_write(uint8_t dev_addr, uint8_t *write_buffer, size_t write_size)
{
    return i2c_master_write_to_device(I2C_MASTER_PORT, dev_addr,
                                       write_buffer, write_size, 1000 / portTICK_PERIOD_MS);
}

// 先写寄存器地址，再读数据（两次独立事务，共用 delayms 作为超时）
esp_err_t i2c_write_read(uint8_t dev_addr, uint8_t read_reg, uint8_t *read_buffer, size_t read_size, uint16_t delayms)
{
    // 写寄存器地址
    esp_err_t err = i2c_master_write_to_device(I2C_MASTER_PORT, dev_addr,
                                                &read_reg, 1, delayms / portTICK_PERIOD_MS);
    if (err != ESP_OK) return err;

    // 读数据
    return i2c_master_read_from_device(I2C_MASTER_PORT, dev_addr,
                                        read_buffer, read_size, delayms / portTICK_PERIOD_MS);
}

// 读寄存器：一次事务完成写寄存器地址和读数据（最常用）
esp_err_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_buffer, size_t read_size)
{
    return i2c_master_write_read_device(I2C_MASTER_PORT, dev_addr,
                                         &reg_addr, 1,
                                         read_buffer, read_size,
                                         1000 / portTICK_PERIOD_MS);
}

// 写寄存器：写地址 + 数据
esp_err_t i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_write_to_device(I2C_MASTER_PORT, dev_addr,
                                       write_buf, sizeof(write_buf),
                                       1000 / portTICK_PERIOD_MS);
}