/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_bh1750.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/

/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/

esp_err_t bh1750_begin(void)
{
    esp_err_t err = ESP_OK;
    uint8_t tx_buf;
 
    // 不再需要注册设备，直接使用设备地址操作

    // 发送上电命令
    tx_buf = BH1750_PWR_ON;
    err = i2c_write(BH1750_I2C_ADDRESS, &tx_buf, 1); /*Set the BH1750 sensor to start working*/
    if (err != ESP_OK)
        return err;

    // 发送连续高分辨率测量命令
    tx_buf = BH1750_CON_H;
    err = i2c_write(BH1750_I2C_ADDRESS, &tx_buf, 1); /*Start measurement at 1lx resolution, Measurement Time is typically 120ms*/
    if (err != ESP_OK)
        return err;

    return ESP_OK; // 原代码返回 err，但 err 此时应为 ESP_OK，直接返回 ESP_OK 更清晰
}

float bh1750_read_data(void)
{
    esp_err_t err = ESP_OK;
    float lux;
    uint8_t sensorData[2] = {0};

    // 从设备读取 2 字节数据
    err = i2c_read(BH1750_I2C_ADDRESS, sensorData, 2); /*Read sensor data*/
    if (err != ESP_OK)
        return -1; // 读取失败返回 -1

    // 将两个字节合并为 16 位数据，并转换为照度值（单位 lx）
    lux = (float)((sensorData[0] << 8) | sensorData[1]) / 1.2f;

    return lux;
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/