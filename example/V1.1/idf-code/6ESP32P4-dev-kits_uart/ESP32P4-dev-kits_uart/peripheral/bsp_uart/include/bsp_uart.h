#ifndef _BSP_UART_H_
#define _BSP_UART_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"     //References for LOG Printing Function-related API Functions
#include "esp_err.h"     //References for Error Type Function-related API Functions
#include "driver/uart.h" //References for Uart Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define UART_TAG "UART"
#define UART_INFO(fmt, ...) ESP_LOGI(UART_TAG, fmt, ##__VA_ARGS__)
#define UART_DEBUG(fmt, ...) ESP_LOGD(UART_TAG, fmt, ##__VA_ARGS__)
#define UART_ERROR(fmt, ...) ESP_LOGE(UART_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_UART_ENABLED
#define CMD_LED_ON "LED_ON"   /*Serial port command for turning on the LED light*/
#define CMD_LED_OFF "LED_OFF" /*Serial port command for turning off the LED light*/
esp_err_t uart_init();        /*Initialize serial port configuration*/
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif