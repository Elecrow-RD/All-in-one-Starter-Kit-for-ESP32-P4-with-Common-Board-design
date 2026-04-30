#ifndef _BSP_SERVO_H_
#define _BSP_SERVO_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define SERVO_TAG "SERVO"
#define SERVO_INFO(fmt, ...) ESP_LOGI(SERVO_TAG, fmt, ##__VA_ARGS__)
#define SERVO_DEBUG(fmt, ...) ESP_LOGD(SERVO_TAG, fmt, ##__VA_ARGS__)
#define SERVO_ERROR(fmt, ...) ESP_LOGE(SERVO_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_SERVO_ENABLED

#define SERVO_GPIO CONFIG_SERVO_GPIO
#define SERVO_MIN_PULSEWIDTH_US 500  // The pulse width corresponding to 0 degree
#define SERVO_MAX_PULSEWIDTH_US 2500 // The pulse width corresponding to 180 degree
#define SERVO_MAX_DEGREE 180

esp_err_t servo_init();
esp_err_t set_servo_angle(int degree);
bool parse_angle_command(const char *str, int *out_angle);
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif