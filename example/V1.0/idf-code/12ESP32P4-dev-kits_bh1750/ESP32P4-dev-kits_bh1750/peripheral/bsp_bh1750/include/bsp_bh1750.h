#ifndef _BSP_BH1750_H_
#define _BSP_BH1750_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h" //References for LOG Printing Function-related API Functions
#include "esp_err.h" //References for Error Type Function-related API Functions
#include "bsp_i2c.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define BH1750_TAG "BH1750"
#define BH1750_INFO(fmt, ...) ESP_LOGI(BH1750_TAG, fmt, ##__VA_ARGS__)
#define BH1750_DEBUG(fmt, ...) ESP_LOGD(BH1750_TAG, fmt, ##__VA_ARGS__)
#define BH1750_ERROR(fmt, ...) ESP_LOGE(BH1750_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED

#define BH1750_I2C_ADDRESS CONFIG_BH1750_I2C_ADDRESS // The 7-bit I2C address of BH1750

#define BH1750_PWR_DOWN 0x00 // Power down
#define BH1750_PWR_ON 0x01   // Power on
#define BH1750_RST 0x07      // reset register
#define BH1750_CON_H 0x10    // 1lx，120ms
#define BH1750_CON_H2 0x11   // 0.5lx，120ms
#define BH1750_CON_L 0x13    // 4lx，16ms
#define BH1750_ONE_H 0x20    // one test ：1lx，120ms，
#define BH1750_ONE_H2 0x21   // one test ：0.5lx，120ms，
#define BH1750_ONE_L 0x23    // one test ：4lx，16ms，

esp_err_t bh1750_begin(void); // Initialization of BH1750 sensor
float bh1750_read_data();     // BH1750 Sensor lux Data Reading Function

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif