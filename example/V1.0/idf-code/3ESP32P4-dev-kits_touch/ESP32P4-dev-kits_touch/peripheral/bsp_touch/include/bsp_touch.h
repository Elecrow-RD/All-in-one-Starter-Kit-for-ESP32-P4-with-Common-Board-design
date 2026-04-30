#ifndef _BSP_TOUCH_H_
#define _BSP_TOUCH_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define TOUCH_TAG "TOUCH"
#define TOUCH_INFO(fmt, ...) ESP_LOGI(TOUCH_TAG, fmt, ##__VA_ARGS__)
#define TOUCH_DEBUG(fmt, ...) ESP_LOGD(TOUCH_TAG, fmt, ##__VA_ARGS__)
#define TOUCH_ERROR(fmt, ...) ESP_LOGE(TOUCH_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_INDEPENDENT_TOUCH_ENABLED

#define INDEPENDENT_TOUCH_GPIO CONFIG_INDEPENDENT_TOUCH_GPIO // Touch GPIO
esp_err_t independent_touch_init();                          // Initialize the GPIO Pin of the Touch
int get_touch_state();                                       // Get the current corresponding level of the GPIO for the Touch button

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif