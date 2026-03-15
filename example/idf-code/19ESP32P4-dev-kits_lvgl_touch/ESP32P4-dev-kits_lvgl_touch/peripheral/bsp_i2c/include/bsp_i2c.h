#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"           //References for LOG Printing Function-related API Functions
#include "esp_err.h"           //References for Error Type Function-related API Functions
#include "driver/i2c_master.h" //References for I2C Master Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define I2C_TAG "I2C"
#define I2C_INFO(fmt, ...) ESP_LOGI(I2C_TAG, fmt, ##__VA_ARGS__)
#define I2C_DEBUG(fmt, ...) ESP_LOGD(I2C_TAG, fmt, ##__VA_ARGS__)
#define I2C_ERROR(fmt, ...) ESP_LOGE(I2C_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_I2C_ENABLED

#define I2C_MASTER_PORT CONFIG_I2C_PORT_NUM // I2C port number
#define I2C_GPIO_SCL CONFIG_I2C_GPIO_SCL    // I2C SCL GPIO
#define I2C_GPIO_SDA CONFIG_I2C_GPIO_SDA    // I2C SDA GPIO

esp_err_t i2c_init(void);                                                                                                              // Initialize I2C
i2c_master_dev_handle_t i2c_dev_register(uint16_t dev_device_address);                                                                 // The I2C slave device registration function
esp_err_t i2c_read(i2c_master_dev_handle_t i2c_dev, uint8_t *read_buffer, size_t read_size);                                           // I2C Read Function
esp_err_t i2c_write(i2c_master_dev_handle_t i2c_dev, uint8_t *write_buffer, size_t write_size);                                        // I2C Write Function
esp_err_t i2c_write_read(i2c_master_dev_handle_t i2c_dev, uint8_t read_reg, uint8_t *read_buffer, size_t read_size, uint16_t delayms); // I2C Write and Read Function
esp_err_t i2c_read_reg(i2c_master_dev_handle_t i2c_dev, uint8_t reg_addr, uint8_t *read_buffer, size_t read_size);                     // I2C Read Register Function
esp_err_t i2c_write_reg(i2c_master_dev_handle_t i2c_dev, uint8_t reg_addr, uint8_t data);                                              // I2C Write Register Function
extern i2c_master_bus_handle_t i2c_bus_handle;
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif
