#ifndef _BSP_HALL_H_
#define _BSP_HALL_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_attr.h"    //API Function References Related to Memory Configuration
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define HALL_TAG "HALL"
#define HALL_INFO(fmt, ...) ESP_LOGI(HALL_TAG, fmt, ##__VA_ARGS__)
#define HALL_DEBUG(fmt, ...) ESP_LOGD(HALL_TAG, fmt, ##__VA_ARGS__)
#define HALL_ERROR(fmt, ...) ESP_LOGE(HALL_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_HALL_ENABLED

#define HALL_GPIO CONFIG_HALL_GPIO // HALL GPIO
esp_err_t hall_init();             // Initialize the GPIO Pin of the HALL
bool get_hall_status();            // Get the status for the HALL

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif