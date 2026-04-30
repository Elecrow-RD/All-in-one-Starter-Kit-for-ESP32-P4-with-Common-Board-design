#ifndef _BSP_GAS_H_
#define _BSP_GAS_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include <math.h>
#include "freertos/FreeRTOS.h"       //References for Freertos Function-related API Functions
#include "freertos/task.h"           //References for Freertos Task Function-related API Functions
#include "esp_log.h"                 //References for LOG Printing Function-related API Functions
#include "esp_err.h"                 //References for Error Type Function-related API Functions
#include "esp_adc/adc_oneshot.h"     //References for ADC Oneshot Function-related API Functions
#include "esp_adc/adc_cali.h"        //References for ADC Cail Function-related API Functions
#include "esp_adc/adc_cali_scheme.h" //References for ADC Cail Scheme Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define GAS_TAG "GAS"
#define GAS_INFO(fmt, ...) ESP_LOGI(GAS_TAG, fmt, ##__VA_ARGS__)
#define GAS_DEBUG(fmt, ...) ESP_LOGD(GAS_TAG, fmt, ##__VA_ARGS__)
#define GAS_ERROR(fmt, ...) ESP_LOGE(GAS_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_GAS_ENABLED

#define GAS_CHANNEL CONFIG_GAS_CHANNEL // ADC acquisition channel

typedef struct
{
    adc_oneshot_unit_handle_t gas_handle;
    int gas_voltage;
    float gas_vaule;
    float RL;
    float R0;
} gas_config;

esp_err_t gas_init(adc_oneshot_unit_handle_t handle); // GAS ADC Initialization Function
float get_gas_vaule();                                // Obtain the ADC acquisition voltage value of GAS
float get_r0_calibration();                           // Obtain the R0 value (load resistance data under clean air conditions) through data conversion
float get_gas_data();                                 // Obtain the gas concentration value of gas(ppm)
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif