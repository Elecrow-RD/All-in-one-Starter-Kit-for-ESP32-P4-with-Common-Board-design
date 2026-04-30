#ifndef _BSP_ULTRASONIC_H_
#define _BSP_ULTRASONIC_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "freertos/FreeRTOS.h"   //References for Freertos Function-related API Functions
#include "freertos/task.h"       //References for Freertos Task Function-related API Functions
#include "esp_log.h"             //References for LOG Printing Function-related API Functions
#include "esp_err.h"             //References for Error Type Function-related API Functions
#include "esp_timer.h"           //References for high-precision timers Function-related API Functions
#include "esp32p4/rom/ets_sys.h" //References for system timers Function-related API Functions
#include "driver/gpio.h"         //References for GPIO Function-related API Functions
#include "driver/mcpwm_cap.h"    ///References for MCPWM capture timer Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define ULTRASONIC_TAG "ULTRASONIC"
#define ULTRASONIC_INFO(fmt, ...) ESP_LOGI(ULTRASONIC_TAG, fmt, ##__VA_ARGS__)
#define ULTRASONIC_DEBUG(fmt, ...) ESP_LOGD(ULTRASONIC_TAG, fmt, ##__VA_ARGS__)
#define ULTRASONIC_ERROR(fmt, ...) ESP_LOGE(ULTRASONIC_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_ULTRASONIC_ENABLED

#define ULTRASONIC_GPIO_ECHO CONFIG_ULTRASONIC_GPIO_ECHO // ULTRASONIC ECHO GPIO
#define ULTRASONIC_GPIO_TRIG CONFIG_ULTRASONIC_GPIO_TRIG // ULTRASONIC TRIG GPIO

typedef struct
{
    uint32_t ultrasonic_time;       // Total Ultrasonic Transmission Time
    uint32_t ultrasonic_start_time; // Ultrasonic transmission start time
    uint32_t ultrasonic_stop_time;  // Ultrasonic transmission end time
    float ultrasonic_distance;      // Distance measured by ultrasonic
} ultrasonic_struct;

esp_err_t ultrasonic_init();                                 // Ultrasonic Initialization function
esp_err_t ultrasonic_callback_register(TaskHandle_t handle); // Ultrasonic task callback registration function
esp_err_t send_ultrasonic_start();                           // Send the ultrasonic start signal function
float get_ultrasonic_distance(uint32_t time);                // Convert the collected time data into distance function

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif