#ifndef _BSP_LED_H_
#define _BSP_LED_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define LED_TAG "LED"
#define LED_INFO(fmt, ...) ESP_LOGI(LED_TAG, fmt, ##__VA_ARGS__)
#define LED_DEBUG(fmt, ...) ESP_LOGD(LED_TAG, fmt, ##__VA_ARGS__)
#define LED_ERROR(fmt, ...) ESP_LOGE(LED_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_LED_ENABLED

#define LED_GPIO CONFIG_LED_GPIO     // LED GPIO
esp_err_t led_init();                // Initialize the GPIO Pin of the LED
esp_err_t set_led_state(bool state); // Set the GPIO Pin Output Level State of the LED
void led_toggle();                   // Toggle the GPIO Pin Output Level State of the LED

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif