#ifndef _BSP_LED_H_
#define _BSP_LED_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "freertos/FreeRTOS.h" //References for Freertos Function-related API Functions
#include "freertos/task.h"     //References for Freertos Task Function-related API Functions
#include "esp_log.h"           //References for LOG Printing Function-related API Functions
#include "esp_err.h"           //References for Error Type Function-related API Functions
#include "driver/rmt_tx.h"     //References for RMT TX Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define LED_TAG "LED"
#define LED_INFO(fmt, ...) ESP_LOGI(LED_TAG, fmt, ##__VA_ARGS__)
#define LED_DEBUG(fmt, ...) ESP_LOGD(LED_TAG, fmt, ##__VA_ARGS__)
#define LED_ERROR(fmt, ...) ESP_LOGE(LED_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_LED_ENABLED

#define LED_GPIO CONFIG_LED_GPIO             // LED GPIO
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // Channel clock resolution, in Hz

typedef enum
{
    LED_BYTE_RED = 0,
    LED_BYTE_YELLOW,
    LED_BYTE_GREEN,
    LED_BYTE_BLUE,
    LED_BYTE_DONE,
} led_byte_order_t;

esp_err_t led_init();                                                        // RMT LED Initialization Function
esp_err_t set_led_status(uint32_t bit);                                      // Set led display status
esp_err_t set_single_led_status(led_byte_order_t color, uint8_t brightness); // Set single led display status
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif