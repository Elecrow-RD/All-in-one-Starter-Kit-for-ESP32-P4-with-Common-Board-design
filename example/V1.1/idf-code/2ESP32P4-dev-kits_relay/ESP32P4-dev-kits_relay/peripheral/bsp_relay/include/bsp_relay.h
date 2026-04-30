#ifndef _BSP_RELAY_H_
#define _BSP_RELAY_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define RELAY_TAG "RELAY"
#define RELAY_INFO(fmt, ...) ESP_LOGI(RELAY_TAG, fmt, ##__VA_ARGS__)
#define RELAY_DEBUG(fmt, ...) ESP_LOGD(RELAY_TAG, fmt, ##__VA_ARGS__)
#define RELAY_ERROR(fmt, ...) ESP_LOGE(RELAY_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_RELAY_ENABLED

#define RELAY_GPIO CONFIG_RELAY_GPIO   // Relay GPIO
esp_err_t relay_init();                // Initialize the GPIO Pin of the Relay
esp_err_t set_relay_state(bool state); // Set the GPIO Pin Output Level State of the Relay
void relay_toggle();                   // Toggle the GPIO Pin Output Level State of the Relay

#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif