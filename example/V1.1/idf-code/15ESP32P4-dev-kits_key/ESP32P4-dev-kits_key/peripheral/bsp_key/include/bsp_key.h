#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/gpio.h" //References for GPIO Function-related API Functions
#include "iot_button.h"  //References for IOT Button Function-related API Functions
#include "button_adc.h"  //References for ADC Button Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define KEY_TAG "KEY"
#define KEY_INFO(fmt, ...) ESP_LOGI(KEY_TAG, fmt, ##__VA_ARGS__)
#define KEY_DEBUG(fmt, ...) ESP_LOGD(KEY_TAG, fmt, ##__VA_ARGS__)
#define KEY_ERROR(fmt, ...) ESP_LOGE(KEY_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_KEY_ENABLED

#define KEY_GPIO CONFIG_KEY_GPIO // The GPIO used for the ADC button channel

typedef enum
{
    Key_idle = 0,
    Key_left,
    Key_middle,
    Key_right,
    Key_up,
    Key_release,
} key_state; // Key Press State Structure

esp_err_t key_init(adc_oneshot_unit_handle_t handle); // ADC Key Initialization Function
esp_err_t key_register_cb();                          // Key press status registration callback function
extern key_state key_status;
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif