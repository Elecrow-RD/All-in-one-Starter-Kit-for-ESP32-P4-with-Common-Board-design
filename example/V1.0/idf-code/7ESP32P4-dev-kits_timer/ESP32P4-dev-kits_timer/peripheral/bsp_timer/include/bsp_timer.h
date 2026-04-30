#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"   //References for LOG Printing Function-related API Functions
#include "esp_err.h"   //References for Error Type Function-related API Functions
#include "esp_timer.h" //References for high-precision timers Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define TIMER_TAG "TIMER"
#define TIMER_INFO(fmt, ...) ESP_LOGI(TIMER_TAG, fmt, ##__VA_ARGS__)
#define TIMER_DEBUG(fmt, ...) ESP_LOGD(TIMER_TAG, fmt, ##__VA_ARGS__)
#define TIMER_ERROR(fmt, ...) ESP_LOGE(TIMER_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_TIMER_ENABLED
esp_err_t timer_init();                         /*Initialize the high-precision timer*/
esp_err_t start_timer(uint64_t time);           /*Start timer*/
esp_err_t stop_timer(esp_timer_handle_t timer); /*Stop timer*/
bool get_timer_flag();                          /*Get the timer flag*/
void reset_timer_flag();                        /*Reset the timer flag*/
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif