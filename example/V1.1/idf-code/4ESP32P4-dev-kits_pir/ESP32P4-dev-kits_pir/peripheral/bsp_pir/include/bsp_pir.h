#ifndef _BSP_PIR_H_
#define _BSP_PIR_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_attr.h"    //API Function References Related to Memory Configuration
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define PIR_TAG "PIR"
#define PIR_INFO(fmt, ...) ESP_LOGI(PIR_TAG, fmt, ##__VA_ARGS__)
#define PIR_DEBUG(fmt, ...) ESP_LOGD(PIR_TAG, fmt, ##__VA_ARGS__)
#define PIR_ERROR(fmt, ...) ESP_LOGE(PIR_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_PIR_ENABLED

#define PIR_GPIO CONFIG_PIR_GPIO // PIR GPIO
esp_err_t pir_init();            // Initialize the GPIO Pin of the PIR
esp_err_t get_pir_state();       // Get the status for the PIR

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif
