#ifndef _BSP_SERVO_H_
#define _BSP_SERVO_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
#include "driver/ledc.h" //References for LEDC PWM Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define SERVO_TAG "SERVO"
#define SERVO_INFO(fmt, ...) ESP_LOGI(SERVO_TAG, fmt, ##__VA_ARGS__)
#define SERVO_DEBUG(fmt, ...) ESP_LOGD(SERVO_TAG, fmt, ##__VA_ARGS__)
#define SERVO_ERROR(fmt, ...) ESP_LOGE(SERVO_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_SERVO_ENABLED

#define SERVO_GPIO CONFIG_SERVO_GPIO // Servo GPIO

typedef enum
{
    forward_dir = 1,
    reverse_dir,
    stop,
} servo_dir; /*Servo Motor Direction Type*/

esp_err_t servo_init();                                            /* Initialize the GPIO Pin and PWM channel of the LED*/
esp_err_t set_servo_status(servo_dir servo_dir_status, int speed); /*Set the servo motor direction and speed*/

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif