#ifndef _BSP_LSM6DS3TR_H_
#define _BSP_LSM6DS3TR_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "freertos/FreeRTOS.h" //References for Freertos Function-related API Functions
#include "freertos/task.h"     //References for Freertos Task Function-related API Functions
#include "esp_log.h"           //References for LOG Printing Function-related API Functions
#include "esp_err.h"           //References for Error Type Function-related API Functions
#include "bsp_i2c.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/
/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define LSM_TAG "LSM6DS3TR"
#define LSM_INFO(fmt, ...) ESP_LOGI(LSM_TAG, fmt, ##__VA_ARGS__)
#define LSM_DEBUG(fmt, ...) ESP_LOGD(LSM_TAG, fmt, ##__VA_ARGS__)
#define LSM_ERROR(fmt, ...) ESP_LOGE(LSM_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED

#define LSM6DS3_I2C_ADDRESS CONFIG_LSM6DS3_I2C_ADDRESS // The 7-bit I2C address of LSM6DS3TR
#define WHO_AM_I_ADDRESS 0x0F                          // This register is a read-only register. Its value is fixed at 69h
#define CTRL1_XL_ADDRESS 0x10                          // Linear acceleration sensor control register 1
#define CTRL2_G_ADDRESS 0x11                           // Angular rate sensor control register 2
#define CTRL3_C_ADDRESS 0x12                           // Control register 3
#define CTRL4_C_ADDRESS 0x13                           // Control register 4
#define CTRL6_C_ADDRESS 0x15                           // Angular rate sensor control register 6
#define CTRL7_G_ADDRESS 0x16                           // Angular rate sensor control register 7
#define CTRL8_XL_ADDRESS 0x17                          // Linear acceleration sensor control register 8
#define STATUS_REG_ADDRESS 0x1E                        // Status register
#define OUTX_L_G_ADDRESS 0x22                          // Angular rate sensor pitch axis (X) angular rate output register(L_Bit)
#define OUTX_H_G_ADDRESS 0x23                          // Angular rate sensor pitch axis (X) angular rate output register(H_Bit)
#define OUTY_L_G_ADDRESS 0x24                          // Angular rate sensor roll axis (Y) angular rate output register(L_Bit)
#define OUTY_H_G_ADDRESS 0x25                          // Angular rate sensor roll axis (Y) angular rate output register(H_Bit)
#define OUTZ_L_G_ADDRESS 0x26                          // Angular rate sensor yaw axis (Z) angular rate output register(L_Bit)
#define OUTZ_H_G_ADDRESS 0x27                          // Angular rate sensor yaw axis (Z) angular rate output register(H_Bit)
#define OUTX_L_XL_ADDRESS 0x28                         // Linear acceleration sensor X-axis output register(L_Bit)
#define OUTX_H_XL_ADDRESS 0x29                         // Linear acceleration sensor X-axis output register(H_Bit)
#define OUTY_L_XL_ADDRESS 0x2A                         // Linear acceleration sensor Y-axis output register(L_Bit)
#define OUTY_H_XL_ADDRESS 0x2B                         // Linear acceleration sensor Y-axis output register(H_Bit)
#define OUTZ_L_XL_ADDRESS 0x2C                         // Linear acceleration sensor Z-axis output register(L_Bit)
#define OUTZ_H_XL_ADDRESS 0x2D                         // Linear acceleration sensor Z-axis output register(H_Bit)
/*Output data rate and power mode selection*/
#define LSM6DS3TRC_RATE_0 0x00
#define LSM6DS3TRC_RATE_1HZ6 0xB0
#define LSM6DS3TRC_RATE_12HZ5 0x10
#define LSM6DS3TRC_RATE_26HZ 0x20
#define LSM6DS3TRC_RATE_52HZ 0x30
#define LSM6DS3TRC_RATE_104HZ 0x40
#define LSM6DS3TRC_RATE_208HZ 0x50
#define LSM6DS3TRC_RATE_416HZ 0x60
#define LSM6DS3TRC_RATE_833HZ 0x70
#define LSM6DS3TRC_RATE_1660HZ 0x80
#define LSM6DS3TRC_RATE_3330HZ 0x90
#define LSM6DS3TRC_RATE_6660HZ 0xA0
/*Accelerometer full-scale selection*/
#define LSM6DS3TRC_ACC_FSXL_2G 0x00
#define LSM6DS3TRC_ACC_FSXL_16G 0x04
#define LSM6DS3TRC_ACC_FSXL_4G 0x08
#define LSM6DS3TRC_ACC_FSXL_8G 0x0C
/*Gyroscope full-scale selection*/
#define LSM6DS3TRC_GYR_FSG_245 0x00
#define LSM6DS3TRC_GYR_FSG_500 0x04
#define LSM6DS3TRC_GYR_FSG_1000 0x08
#define LSM6DS3TRC_GYR_FSG_2000 0x0C
/*Anti-aliasing filter bandwidth selection*/
#define LSM6DS3TRC_ACC_BW0XL_1500HZ 0x00
#define LSM6DS3TRC_ACC_BW0XL_400HZ 0x01
/*Accelerometer bandwidth selection*/
#define LSM6DS3TRC_CTRL4_DEN_XL_EN_DISABLE 0x00
#define LSM6DS3TRC_CTRL4_DEN_XL_EN_ENABLE 0x80
/*Gyroscope sleep mode enable*/
#define LSM6DS3TRC_CTRL4_SLEEP_ENABLE 0x40
#define LSM6DS3TRC_CTRL4_SLEEP_DISABLE 0x00
/*All interrupt signals available on INT1 pad enable*/
#define LSM6DS3TRC_CTRL4_DEN_DRDY_INT1_DISBALE 0x00
#define LSM6DS3TRC_CTRL4_DEN_DRDY_INT1_ENABLE 0x20
/*Data-ready mask enable*/
#define LSM6DS3TRC_CTRL4_DRDY_MASK_DISABLE 0x00
#define LSM6DS3TRC_CTRL4_DRDY_MASK_ENABLE 0x08
/*Disable I2C interface*/
#define LSM6DS3TRC_CTRL4_I2C_DISABLE 0x04
#define LSM6DS3TRC_CTRL4_I2C_ENABLE 0x00
/* Enable gyroscope digital LPF1. The bandwidth can be selected through */
#define LSM6DS3TRC_CTRL4_LPF1_SELG_ENABLE 0x02
#define LSM6DS3TRC_CTRL4_LPF1_SELG_DISABLE 0x00

#define LSM6DS3TRC_CTRL6_C_EDGE_TRIGGER 0x80 // Gyroscope data edge-sensitive trigger enable

#define LSM6DS3TRC_CTRL6_C_LEVEL_TRIGGER 0x40 // Gyroscope data level-sensitive trigger enable
#define LSM6DS3TRC_CTRL6_C_LEVEL_LATCHED 0x60 // Gyroscope level-sensitive latched enable.
#define LSM6DS3TRC_CTRL6_C_LEVEL_FIFO 0xC0    // LSM6DS3TRC_CTRL6_C_EDGE_TRIGGER|LSM6DS3TRC_CTRL6_C_LEVEL_TRIGGER
/* High-performance operating mode disable for accelerometer*/
#define LSM6DS3TRC_CTRL6_C_XL_HM_MODE_ENABLE 0x00
#define LSM6DS3TRC_CTRL6_C_XL_HM_MODE_DISABLE 0x10
/* Gyroscope's low-pass filter (LPF1) bandwidth selection */
#define LSM6DS3TRC_CTRL6_C_FTYPE_1 0x00
#define LSM6DS3TRC_CTRL6_C_FTYPE_2 0x01
#define LSM6DS3TRC_CTRL6_C_FTYPE_3 0x02
#define LSM6DS3TRC_CTRL6_C_FTYPE_4 0x03
/*High-performance operating mode*/
#define LSM6DS3TRC_CTRL7_G_HM_MODE_ENABLE 0x00
#define LSM6DS3TRC_CTRL7_G_HM_MODE_DISABLE 0x80
/*Gyroscope digital high-pass filter enable*/
#define LSM6DS3TRC_CTRL7_G_HP_EN_DISABLE 0x00
#define LSM6DS3TRC_CTRL7_G_HP_EN_ENABLE 0x40
/*Gyroscope digital HP filter cutoff selection*/
#define LSM6DS3TRC_CTRL7_G_HPM_16MHZ 0x00
#define LSM6DS3TRC_CTRL7_G_HPM_65MHZ 0x10
#define LSM6DS3TRC_CTRL7_G_HPM_260MHZ 0x20
#define LSM6DS3TRC_CTRL7_G_HPM_1HZ04 0x30
/*Source register rounding function enable*/
#define LSM6DS3TRC_CTRL7_G_ROUNDING_STATUS_DISABLE 0x04
#define LSM6DS3TRC_CTRL7_G_ROUNDING_STATUS_ENABLE 0x00
/*Accelerometer ODR register setting*/
#define LSM6DS3TRC_ACC_LOW_PASS_ODR_50 0x88
#define LSM6DS3TRC_ACC_LOW_PASS_ODR_100 0xA8
#define LSM6DS3TRC_ACC_LOW_PASS_ODR_9 0xC8
#define LSM6DS3TRC_ACC_LOW_PASS_ODR_400 0xE8

#define LSM6DS3TRC_ACC_HIGH_PASS_ODR_50 0x04
#define LSM6DS3TRC_ACC_HIGH_PASS_ODR_100 0x24
#define LSM6DS3TRC_ACC_HIGH_PASS_ODR_9 0x44
#define LSM6DS3TRC_ACC_HIGH_PASS_ODR_400 0x64

#define LSM6DS3TRC_STATUS_GYROSCOPE 0x02     // Gyroscope new data available
#define LSM6DS3TRC_STATUS_ACCELEROMETER 0x01 // Accelerometer new data available

typedef struct
{
    float acc_x;
    float acc_y;
    float acc_z;
    float gry_x;
    float gry_y;
    float gry_z;
} lsm6ds3tr; // lsm6ds3tr data structure

extern lsm6ds3tr my_lsm6ds3;
esp_err_t lsm6ds3_begin(void);           // Initialization of Lsm6ds3tr sensor
esp_err_t lsm6ds3_get_acc(uint8_t fsxl); // lsm6ds3 acquires acceleration data
esp_err_t lsm6ds3_get_gry(uint8_t fsg);  // lsm6ds3 acquires gyroscope data
esp_err_t lsm6ds3_scan();                // The lsm6ds3 scans to determine whether the currently read data is from the gyroscope or the accelerometer
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif
