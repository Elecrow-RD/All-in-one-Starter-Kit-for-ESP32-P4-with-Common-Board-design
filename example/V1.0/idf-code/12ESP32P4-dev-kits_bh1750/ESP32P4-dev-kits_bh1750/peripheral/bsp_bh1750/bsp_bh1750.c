/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_bh1750.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
static i2c_master_dev_handle_t bh1750_handle = NULL;
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
esp_err_t bh1750_begin(void)
{
    esp_err_t err = ESP_OK;
    uint8_t tx_buf;
    tx_buf = BH1750_PWR_ON;
    bh1750_handle = i2c_dev_register(BH1750_I2C_ADDRESS); /*BH1750 sensor address registration*/
    if (bh1750_handle == NULL)
        return ESP_FAIL;
    err = i2c_write(bh1750_handle, &tx_buf, 1); /*Set the BH1750 sensor to start working*/
    if (err != ESP_OK)
        return err;
    tx_buf = BH1750_CON_H;
    err = i2c_write(bh1750_handle, &tx_buf, 1); /*Start measurement at 1lx resolution,Measurement Time is typically 120ms*/
    if (err != ESP_OK)
        return err;
    return err;
}

float bh1750_read_data()
{
    esp_err_t err = ESP_OK;
    float lux;
    uint8_t sensorData[2] = {0};
    err = i2c_read(bh1750_handle, sensorData, 2); /*Read sensor data*/
    if (err != ESP_OK)
        return -1;
    lux = (sensorData[0] << 8 | sensorData[1]) / 1.2; /*Convert illuminance data*/
    return lux;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/